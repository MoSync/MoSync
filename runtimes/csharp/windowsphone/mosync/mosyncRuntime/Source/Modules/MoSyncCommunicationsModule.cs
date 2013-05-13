using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Net.Sockets;
using Microsoft.Phone.Tasks;
using System.Threading;
using System.Collections.Generic;
using System.IO;

namespace MoSync
{
	public class CommunicationsModule : IIoctlModule, ISyscallModule
	{
		delegate void ResultHandler(int handle, int connOp, int result);

		static void WriteIPv4(IMemory m, int addr, IPEndPoint ie)
		{
			m.WriteInt32(addr + MoSync.Struct.MAConnAddr.family, MoSync.Constants.CONN_FAMILY_INET4);
			m.WriteBytes(addr + MoSync.Struct.MAConnAddr.inet4.addr, ie.Address.GetAddressBytes(), 4);
			m.WriteInt16(addr + MoSync.Struct.MAConnAddr.inet4.port, (short)ie.Port);
		}

		// represents either a Socket or a WebRequest
		abstract class Connection
		{
			protected int mHandle;

			// asynchronous
			public abstract void connect(ResultHandler rh);
			public abstract void recv(byte[] buffer, int offset, int size, ResultHandler rh);
			public abstract void write(byte[] buffer, int offset, int size, ResultHandler rh);

			// immediate
			public abstract void close();
			public abstract int getAddr(IMemory m, int _addr);
		}

		class SocketConnection : Connection
		{
			protected Socket mSocket;
			DnsEndPoint mHostEntry;

			public SocketConnection(Uri uri, int handle, SocketType st, ProtocolType pt)
			{
				mHandle = handle;

				mSocket = new Socket(AddressFamily.InterNetwork, st, pt);
				mHostEntry = new DnsEndPoint(uri.Host, uri.Port);
			}

			public override void connect(ResultHandler rh)
			{
				SocketAsyncEventArgs socketEventArg = new SocketAsyncEventArgs();
				socketEventArg.RemoteEndPoint = mHostEntry;
				socketEventArg.Completed += new EventHandler<SocketAsyncEventArgs>(
						delegate(object s, SocketAsyncEventArgs e)
						{
							int result;
							if (e.SocketError == SocketError.Success)
							{
								result = 1;
							}
							else
							{
								result = MoSync.Constants.CONNERR_GENERIC;
							}
							rh(mHandle, MoSync.Constants.CONNOP_CONNECT, result);
						});

				mSocket.ConnectAsync(socketEventArg);
			}

			private SocketAsyncEventArgs setupRW(byte[] buffer, int offset, int size,
					 ResultHandler rh, int connop)
			{
				SocketAsyncEventArgs socketEventArg = new SocketAsyncEventArgs();

				// Setup the buffer to receive the data
				socketEventArg.SetBuffer(buffer, offset, size);

				// Inline event handler for the Completed event.
				// Note: This even handler was implemented inline in order to make this method self-contained.
				socketEventArg.Completed += new EventHandler<SocketAsyncEventArgs>(
						delegate(object s, SocketAsyncEventArgs e)
						{
							int result;
							if (e.SocketError == SocketError.Success)
							{
								if (connop == MoSync.Constants.CONNOP_WRITE)
									result = 1;
								else
									result = e.BytesTransferred;
							}
							else
							{
								result = MoSync.Constants.CONNERR_GENERIC;
							}
							rh(mHandle, connop, result);
						});
				return socketEventArg;
			}

			public override void recv(byte[] buffer, int offset, int size,
					 ResultHandler rh)
			{
				mSocket.ReceiveAsync(setupRW(buffer, offset, size, rh, MoSync.Constants.CONNOP_READ));
			}
			public override void write(byte[] buffer, int offset, int size,
					 ResultHandler rh)
			{
				mSocket.SendAsync(setupRW(buffer, offset, size, rh, MoSync.Constants.CONNOP_WRITE));
			}

			// immediate
			public override void close()
			{
				mSocket.Close();
			}
			public override int getAddr(IMemory m, int _addr)
			{
				EndPoint endPoint = mSocket.RemoteEndPoint;
				switch (endPoint.AddressFamily)
				{
					case AddressFamily.InterNetwork:
						WriteIPv4(m, _addr, (IPEndPoint)endPoint);
						return 0;
					case AddressFamily.Unspecified:
						return -1;
					default:
						return MoSync.Constants.CONNERR_INTERNAL;
				}
			}
		}

		class WebRequestConnection : Connection
		{
			protected WebRequest mRequest;
			protected WebResponse mResponse;
			protected System.IO.Stream mStream;

			public WebRequestConnection(Uri uri, int handle, int method)
			{
				mHandle = handle;
				HttpWebRequest hwr = WebRequest.CreateHttp(uri);
				hwr.AllowAutoRedirect = false;
				mRequest = hwr;
				switch (method)
				{
					case MoSync.Constants.HTTP_GET:
						mRequest.Method = "GET";
						break;
					case MoSync.Constants.HTTP_POST:
						mRequest.Method = "POST";
						break;
					case MoSync.Constants.HTTP_HEAD:
						mRequest.Method = "HEAD";
						break;
					case MoSync.Constants.HTTP_PUT:
						mRequest.Method = "PUT";
						break;
					case MoSync.Constants.HTTP_DELETE:
						mRequest.Method = "DELETE";
						break;
					default:
						throw new Exception("HTTP method");
				}
			}

			public override void connect(ResultHandler rh)
			{
				if (mStream != null) // POST
				{
					mStream.Close();
					mStream = null;
				}
				mRequest.BeginGetResponse(new AsyncCallback(RespCallback), rh);
			}
			protected void RespCallback(IAsyncResult ar)
			{
				int result;
				ResultHandler rh = (ResultHandler)ar.AsyncState;
				try
				{
					mResponse = mRequest.EndGetResponse(ar);
					mStream = mResponse.GetResponseStream();
					if (mResponse is HttpWebResponse)
						result = (int)((HttpWebResponse)mResponse).StatusCode;
					else
						result = 1;
				}
				catch (WebException e)
				{
					if (e.Response != null)
					{
						mResponse = e.Response;
						mStream = mResponse.GetResponseStream();
					}
					switch (e.Status)
					{
						case WebExceptionStatus.ProtocolError:
							if (e.Response is HttpWebResponse)
								result = (int)((HttpWebResponse)mResponse).StatusCode;
							else
								result = MoSync.Constants.CONNERR_GENERIC;
							break;
						case WebExceptionStatus.NameResolutionFailure:
						case WebExceptionStatus.ProxyNameResolutionFailure:
							result = MoSync.Constants.CONNERR_DNS;
							break;
						case WebExceptionStatus.ConnectionClosed:
							result = MoSync.Constants.CONNERR_CLOSED;
							break;
						case WebExceptionStatus.RequestCanceled:
							result = MoSync.Constants.CONNERR_CANCELED;
							break;
						case WebExceptionStatus.SecureChannelFailure:
						case WebExceptionStatus.TrustFailure:
							result = MoSync.Constants.CONNERR_SSL;
							break;
						case WebExceptionStatus.ServerProtocolViolation:
							result = MoSync.Constants.CONNERR_PROTOCOL;
							break;
						default:
							result = MoSync.Constants.CONNERR_GENERIC;
							break;
					}
				}
				rh(mHandle, MoSync.Constants.CONNOP_CONNECT, result);
			}

			public override void recv(byte[] buffer, int offset, int size,
					 ResultHandler rh)
			{
				mStream.BeginRead(buffer, offset, size, new AsyncCallback(RecvCallback), rh);
			}
			protected void RecvCallback(IAsyncResult ar)
			{
				ResultHandler rh = (ResultHandler)ar.AsyncState;
				int result = mStream.EndRead(ar);
				if (result == 0)
				{
					result = MoSync.Constants.CONNERR_CLOSED;
				}
				rh(mHandle, MoSync.Constants.CONNOP_READ, result);
			}

			public override void write(byte[] buffer, int offset, int size,
					 ResultHandler rh)
			{
				if (mResponse != null)
					throw new Exception("HTTP write");
				if (mStream == null)
				{
					mRequest.BeginGetRequestStream(
							new AsyncCallback(delegate(IAsyncResult ar)
					{
						mStream = mRequest.EndGetRequestStream(ar);
						mStream.BeginWrite(buffer, offset, size,
								new AsyncCallback(WriteCallback), rh);
					}), rh);
					return;
				}
				mStream.BeginWrite(buffer, offset, size,
						new AsyncCallback(WriteCallback), rh);
			}
			protected void WriteCallback(IAsyncResult ar)
			{
				ResultHandler rh = (ResultHandler)ar.AsyncState;
				mStream.EndWrite(ar);
				rh(mHandle, MoSync.Constants.CONNOP_WRITE, 1);
			}

			// immediate
			public override void close()
			{
				if (mStream != null)
					mStream.Close();
				if (mResponse != null)
					mResponse.Close();
				if (mRequest != null)
					mRequest.Abort();
			}
			public override int getAddr(IMemory m, int _addr)
			{
				// not available for WebRequests on WindowsPhone.
				return -1;
			}
			public String getResponseHeader(String key)
			{
				return mResponse.Headers[key];
			}
			public void setRequestHeader(String key, String value)
			{
				key = key.ToLowerInvariant();
				if (key == "content-type")
					mRequest.ContentType = value;
				else
					mRequest.Headers[key] = value;
			}
		}

		Dictionary<int, Connection> mConnections = new Dictionary<int, Connection>();
		int mNextConnHandle = 1;
		ResultHandler mResultHandler;
		delegate void CommDelegate(Connection c, byte[] buf, ResultHandler rh);
		delegate void DataDelegate(int _conn, int _data, CommDelegate rh);

		public void Init(Syscalls syscalls, Core core, Runtime runtime)
		{
			runtime.RegisterCleaner(delegate()
			{
				foreach (KeyValuePair<int, Connection> p in mConnections)
				{
					p.Value.close();
				}
				mConnections.Clear();
			});

			mResultHandler = delegate(int handle, int connOp, int result)
			{
				Memory evt = new Memory(4 * 4);
				evt.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_CONN);
				evt.WriteInt32(MoSync.Struct.MAEvent.conn.handle, handle);
				evt.WriteInt32(MoSync.Struct.MAEvent.conn.opType, connOp);
				evt.WriteInt32(MoSync.Struct.MAEvent.conn.result, result);
				runtime.PostEvent(new Event(evt));
			};

			syscalls.maConnect = delegate(int _url)
			{
				String url = core.GetDataMemory().ReadStringAtAddress(_url);
				//Util.Log("maConnect(" + url + ")\n");
				if (url.StartsWith("btspp"))
					return MoSync.Constants.CONNERR_UNAVAILABLE;
				Uri uri = new Uri(url);
				if (uri.Host.Length == 0)
				{
					//throw new Exception("Server connections not supported on Windows Phone 7.");
					return MoSync.Constants.CONNERR_UNAVAILABLE;
				}
				Connection c;
				if (uri.Scheme.Equals("socket"))
				{
					c = new SocketConnection(uri, mNextConnHandle, SocketType.Stream, ProtocolType.Tcp);
				}
				else if (uri.Scheme.Equals("datagram"))
				{
					c = new SocketConnection(uri, mNextConnHandle, SocketType.Dgram, ProtocolType.Udp);
				}
				else if (uri.Scheme.Equals("http") || uri.Scheme.Equals("https"))
				{
					c = new WebRequestConnection(uri, mNextConnHandle, MoSync.Constants.HTTP_GET);
				}
				else
				{
					return MoSync.Constants.CONNERR_GENERIC;
				}

				c.connect(mResultHandler);
				mConnections.Add(mNextConnHandle, c);
				return mNextConnHandle++;
			};

			syscalls.maConnClose = delegate(int _conn)
			{
				Connection c = mConnections[_conn];
				c.close();
				mConnections.Remove(_conn);
			};

			syscalls.maConnGetAddr = delegate(int _conn, int _addr)
			{
				if (_conn == MoSync.Constants.HANDLE_LOCAL) // unavailable
					return -1;
				Connection c = mConnections[_conn];
                return c.getAddr(core.GetDataMemory(), _addr);
			};

			syscalls.maConnRead = delegate(int _conn, int _dst, int _size)
			{
                Connection c = mConnections[_conn];
#if !LIB
                c.recv(core.GetDataMemory().GetData(), _dst, _size, mResultHandler);
#else
				//Create an empty byte array of size _size and give it as a buffer to the recv function.
				Byte[] bytes = new Byte[_size];
				c.recv(bytes, 0, _size, mResultHandler);

				//write the buffer into memory at the _dst address
				core.GetDataMemory().WriteBytes(_dst, bytes, _size);
#endif
			};

			DataDelegate dataDelegate = delegate(int _conn, int _data,
					CommDelegate cd)
			{
				Connection c = mConnections[_conn];
				Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
				Stream s = (Stream)res.GetInternalObject();
				runtime.SetResourceRaw(_data, Resource.Flux);
				MemoryStream mem = null;
				if (s.GetType() == typeof(MemoryStream))
				{
					mem = (MemoryStream)s;
				}
				else
				{
					MoSync.Util.CriticalError("Only binaries (non-ubins) are allowed for maConn(Read/Write)(To/From)Data");
				}

				cd(c, mem.GetBuffer(),
						delegate(int handle, int connOp, int result)
						{
							runtime.SetResourceRaw(_data, res);
							mResultHandler(handle, connOp, result);
						});
			};

			syscalls.maConnReadToData = delegate(int _conn, int _data, int _offset, int _size)
			{
				dataDelegate(_conn, _data,
						delegate(Connection c, byte[] buf, ResultHandler rh)
						{
							c.recv(buf, _offset, _size, rh);
						});
			};

			syscalls.maConnWrite = delegate(int _conn, int _src, int _size)
			{
				Connection c = mConnections[_conn];
#if !LIB
				c.write(core.GetDataMemory().GetData(), _src, _size, mResultHandler);
#else
				Byte[] bytes = new Byte[_size];
				core.GetDataMemory().ReadBytes(bytes, _src, _size);
				c.write(bytes, 0, _size, mResultHandler);
#endif
			};

			syscalls.maConnWriteFromData = delegate(int _conn, int _data, int _offset, int _size)
			{
				dataDelegate(_conn, _data,
						delegate(Connection c, byte[] buf, ResultHandler rh)
						{
							c.write(buf, _offset, _size, rh);
						});
			};

			syscalls.maHttpCreate = delegate(int _url, int _method)
			{
				String url = core.GetDataMemory().ReadStringAtAddress(_url);
				//Util.Log("maHttpCreate(" + url + ")\n");
				Uri uri = new Uri(url);
				WebRequestConnection c = new WebRequestConnection(uri, mNextConnHandle, _method);
				mConnections.Add(mNextConnHandle, c);
				return mNextConnHandle++;
			};

			syscalls.maHttpFinish = delegate(int _conn)
			{
				WebRequestConnection c = (WebRequestConnection)mConnections[_conn];
				c.connect(delegate(int handle, int connOp, int result)
				{
					mResultHandler(handle, MoSync.Constants.CONNOP_FINISH, result);
				});
			};

			syscalls.maHttpSetRequestHeader = delegate(int _conn, int _key, int _value)
			{
				WebRequestConnection c = (WebRequestConnection)mConnections[_conn];
				String key = core.GetDataMemory().ReadStringAtAddress(_key);
				String value = core.GetDataMemory().ReadStringAtAddress(_value);
				if (value.Length > 0)
					c.setRequestHeader(key, value);
			};

			syscalls.maHttpGetResponseHeader = delegate(int _conn, int _key, int _buffer, int _bufSize)
			{
				WebRequestConnection c = (WebRequestConnection)mConnections[_conn];
				String key = core.GetDataMemory().ReadStringAtAddress(_key);
				String value = c.getResponseHeader(key);
				if (value == null)
					return MoSync.Constants.CONNERR_NOHEADER;
				if (value.Length + 1 <= _bufSize)
					core.GetDataMemory().WriteStringAtAddress(_buffer, value, _bufSize);
				return value.Length;
			};
		}

		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{
			ioctls.maSendTextSMS = delegate(int dst, int msg)
			{
				String number = core.GetDataMemory().ReadStringAtAddress(dst);
				String message = core.GetDataMemory().ReadStringAtAddress(msg);

				SmsComposeTask task = new SmsComposeTask();
				task.Body = message;
				task.To = number;
				task.Show();
				return 0;
			};
		}
	}
}
