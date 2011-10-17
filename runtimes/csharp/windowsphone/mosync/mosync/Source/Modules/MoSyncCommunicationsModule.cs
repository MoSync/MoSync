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
        protected List<Socket> mSockets = new List<Socket>();

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            syscalls.maConnect = delegate(int _url)
            {
                String url = core.GetDataMemory().ReadStringAtAddress(_url);
                Uri uri = new Uri(url);
                if (uri.Scheme.Equals("socket") == false) return MoSync.Constants.CONNERR_GENERIC;

                DnsEndPoint hostEntry = new DnsEndPoint(uri.Host, uri.Port);
                Socket _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                SocketAsyncEventArgs socketEventArg = new SocketAsyncEventArgs();
                socketEventArg.RemoteEndPoint = hostEntry;
                ManualResetEvent _clientDone = new ManualResetEvent(false);

                SocketError socketError = SocketError.Success;
                socketEventArg.Completed += new EventHandler<SocketAsyncEventArgs>(delegate(object s, SocketAsyncEventArgs e)
                {
                    socketError = e.SocketError;
                    _clientDone.Set();
                });

                _socket.ConnectAsync(socketEventArg);
                _clientDone.WaitOne();
                if(socketError == SocketError.Success)
                {
                    mSockets.Add(_socket);
                    return mSockets.Count-1;
                }
                else
                {
                    return MoSync.Constants.CONNERR_GENERIC;
                }
            };

            syscalls.maConnClose = delegate(int _conn)
            {
                Socket socket = mSockets[_conn];
                socket.Close();
            };

            syscalls.maConnGetAddr = delegate(int _conn, int _addr)
            {
                Socket socket = mSockets[_conn];
                EndPoint endPoint = socket.RemoteEndPoint;
                SocketAddress socketAddress = endPoint.Serialize();
                String socketAddressStr = socketAddress.ToString();
                core.GetDataMemory().WriteStringAtAddress(_addr, socketAddressStr, socketAddressStr.Length + 1);
                return 0;
            };

            syscalls.maConnRead = delegate(int _conn, int _dst, int _size)
            {
            };

            syscalls.maConnReadToData = delegate(int _conn, int _data, int _offset, int _size)
            {
            };

            syscalls.maConnWrite = delegate(int _conn, int _src, int _size)
            {
            };

            syscalls.maConnWriteFromData = delegate(int _conn, int _data, int _offset, int _size)
            {
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
