using System;
using System.Net;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;

namespace MoSync
{
    public class FileModule : IIoctlModule
    {
        protected List<IsolatedStorageFileStream> mFileHandles = new List<IsolatedStorageFileStream>();

        private String ConvertPath(String path)
        {
            return path.Replace('/', '\\');
        }

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {

            IsolatedStorageFile isolatedStorage = IsolatedStorageFile.GetUserStoreForApplication();

            MoSync.SystemPropertyManager.RegisterSystemPropertyProvider("mosync.path.local",
                delegate(String key)
                {
                    return "\\";
                }
            );

            ioctls.maFileOpen = delegate(int _path, int _mode)
            {
                String path = core.GetDataMemory().ReadStringAtAddress(_path);
                path = ConvertPath(path);
                FileMode mode = FileMode.Open;
                FileAccess access = 0;

                if (_mode == MoSync.Constants.MA_ACCESS_READ)
                {
                    access = FileAccess.Read;
                }
                else if (_mode == MoSync.Constants.MA_ACCESS_READ_WRITE)
                {
                    access = FileAccess.ReadWrite;
                }
                else
                {
                    return MoSync.Constants.MA_FERR_GENERIC;
                }

                IsolatedStorageFileStream fileStream = isolatedStorage.OpenFile(path, mode, access);
                mFileHandles.Add(fileStream);
                return mFileHandles.Count - 1;
            };

            ioctls.maFileClose = delegate(int _file)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                fileStream.Close();
                return 0;
            };

            ioctls.maFileRead = delegate(int _file, int _dst, int _len)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                core.GetDataMemory().WriteFromStream(_dst, fileStream, _len);
                return 0;
            };

            ioctls.maFileReadToData = delegate(int _file, int _data, int _offset, int _len)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                Resource dataRes = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory data = (Memory)dataRes.GetInternalObject();
                data.WriteFromStream(_offset, fileStream, _len);
                return 0;
            };

            ioctls.maFileWriteFromData = delegate(int _file, int _data, int _offset, int _len)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                Resource dataRes = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory data = (Memory)dataRes.GetInternalObject(); 
                byte[] bytes = new byte[_len];
                data.ReadBytes(bytes, _offset, _len);
                fileStream.Write(bytes, 0, _len);
                return 0;
            };

            ioctls.maFileWrite = delegate(int _file, int _src, int _len)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                byte[] bytes = new byte[_len];
                core.GetDataMemory().ReadBytes(bytes, _src, _len);
                fileStream.Write(bytes, 0, _len);
                return 0;
            };

            ioctls.maFileSeek = delegate(int _file, int _offset, int _whence)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                SeekOrigin origin;
                switch (_whence)
                {
                    case MoSync.Constants.MA_SEEK_SET:
                        origin = SeekOrigin.Begin;
                        break;
                    case MoSync.Constants.MA_SEEK_CUR:
                        origin = SeekOrigin.Current;
                        break;
                    case MoSync.Constants.MA_SEEK_END:
                        origin = SeekOrigin.End;
                        break;
                    default:
                        return MoSync.Constants.MA_FERR_GENERIC;
                }

                return (int)fileStream.Seek(_offset, origin);
            };

            ioctls.maFileTell = delegate(int _file)
            {
                IsolatedStorageFileStream fileStream = mFileHandles[_file];
                return (int)fileStream.Position;
            };
        }
    }
}
