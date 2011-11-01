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

namespace MoSync
{
    public class CommunicationsModule : IIoctlModule, ISyscallModule
    {
        // event struct offsets
        // todo: generate from IDL.
        const int MAEvent_type = 0;
        const int MAConnEventData_handle = 4;
        const int MAConnEventData_opType = 8;
        const int MAConnEventData_result = 12;

        delegate void ResultHandler(int handle, int connOp, int result);

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
            public abstract int getAddr(int _addr);
        }

        class SocketConnection : Connection
        {
            protected Socket mSocket;
            DnsEndPoint mHostEntry;

            public SocketConnection(Uri uri, int handle)
            {
                mHandle = handle;

                mSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
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
                 ResultHandler rh)
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
                            result = e.BytesTransferred;
                        }
                        else
                        {
                            result = MoSync.Constants.CONNERR_GENERIC;
                        }
                        rh(mHandle, MoSync.Constants.CONNOP_READ, result);
                    });
                return socketEventArg;
            }

            public override void recv(byte[] buffer, int offset, int size,
                 ResultHandler rh)
            {
                mSocket.ReceiveAsync(setupRW(buffer, offset, size, rh));
            }
            public override void write(byte[] buffer, int offset, int size,
                 ResultHandler rh)
            {
                mSocket.SendAsync(setupRW(buffer, offset, size, rh));
            }

            // immediate
            public override void close()
            {
                mSocket.Close();
            }
            public override int getAddr(int _addr)
            {
                EndPoint endPoint = mSocket.RemoteEndPoint;
                SocketAddress socketAddress = endPoint.Serialize();
                return -1;
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
                mRequest = WebRequest.Create(uri);
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
                mRequest.BeginGetResponse(new AsyncCallback(RespCallback), rh);
            }
            protected void RespCallback(IAsyncResult ar)
            {
                ResultHandler rh = (ResultHandler)ar.AsyncState;
                mResponse = mRequest.EndGetResponse(ar);
                mStream = mResponse.GetResponseStream();
                int result;
                if (mResponse is HttpWebResponse)
                    result = (int)((HttpWebResponse)mResponse).StatusCode;
                else
                    result = 1;
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
                //mStream.BeginWrite(buffer, offset, size, new AsyncCallback(WriteCallback), rh);
                throw new Exception("HTTP write");
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
                if (mResponse != null)
                    mResponse.Close();
                if (mRequest != null)
                    mRequest.Abort();
            }
            public override int getAddr(int _addr)
            {
                return -1;
            }
            public String getResponseHeader(String key)
            {
                return mResponse.Headers[key];
            }
        }

        Dictionary<int, Connection> mConnections = new Dictionary<int, Connection>();
        int mNextConnHandle = 1;
        ResultHandler mResultHandler;
        delegate void CommDelegate(Connection c, byte[] buf, ResultHandler rh);
        delegate void DataDelegate(int _conn, int _data, CommDelegate rh);

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            mResultHandler = delegate(int handle, int connOp, int result)
            {
                Memory evt = new Memory(4 * 4);
                evt.WriteInt32(MAEvent_type, MoSync.Constants.EVENT_TYPE_CONN);
                evt.WriteInt32(MAConnEventData_handle, handle);
                evt.WriteInt32(MAConnEventData_opType, connOp);
                evt.WriteInt32(MAConnEventData_result, result);
                runtime.PostEvent(new Event(evt));
            };

            syscalls.maConnect = delegate(int _url)
            {
                String url = core.GetDataMemory().ReadStringAtAddress(_url);
                Uri uri = new Uri(url);
                Connection c;
                if (uri.Scheme.Equals("socket"))
                {
                    c = new SocketConnection(uri, mNextConnHandle);
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
                Connection c = mConnections[_conn];
                return c.getAddr(_addr);
            };

            syscalls.maConnRead = delegate(int _conn, int _dst, int _size)
            {
                Connection c = mConnections[_conn];
                c.recv(core.GetDataMemory().GetData(), _dst, _size, mResultHandler);
            };

            DataDelegate dataDelegate = delegate(int _conn, int _data,
                CommDelegate cd)
            {
                Connection c = mConnections[_conn];
                Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory mem = (Memory)res.GetInternalObject();
                runtime.SetResource(_data, Resource.Flux);
                cd(c, mem.GetData(),
                    delegate(int handle, int connOp, int result)
                    {
                        runtime.SetResource(_data, res);
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
                c.write(core.GetDataMemory().GetData(), _src, _size, mResultHandler);
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
