using System;
using System.Net;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;

namespace MoSync
{
    public class FileModule : IIoctlModule
    {
        public class File
        {
            public String Path
            {
                get 
                {
                    return mPath;
                }
            }

            public bool Exists
            {
                get
                {
                    if (mIsDirectory)
                        return mIsolatedStorage.DirectoryExists(mPath);
                    else
                        return mIsolatedStorage.FileExists(mPath);
                }
            }

            public IsolatedStorageFileStream FileStream
            {
                get
                {
                    return mFileStream;
                }

                set
                {
                    mFileStream = value;
                }
            }

            public bool IsDirectory
            {
                get
                {
                    return mIsDirectory;
                }
            }

            public File(String path, FileAccess fileAccess)
            {
                mPath = path;
                mIsDirectory = mPath.Length==0 || mPath.EndsWith("\\");
                if (mIsDirectory && mPath.EndsWith("\\"))
                    mPath = mPath.Substring(0, mPath.Length - 1);

                mFileStream = null;
                mIsolatedStorage = IsolatedStorageFile.GetUserStoreForApplication();
                mFileAccess = fileAccess;
            }

            public void TryOpen()
            {
                if (!mIsDirectory)
                {
                    try
                    {
                        mFileStream = mIsolatedStorage.OpenFile(mPath, FileMode.Open, mFileAccess);
                    }
                    catch (IsolatedStorageException e)
                    {
                        MoSync.Util.Log(e.ToString());
                    }
                }
            }

            public void Create()
            {
                if (mIsDirectory)
                {
                    mIsolatedStorage.CreateDirectory(mPath);
                }
                else
                {
                    mFileStream = mIsolatedStorage.OpenFile(mPath, FileMode.Create, mFileAccess);
                }
            }

            public void Close()
            {
                if (mIsDirectory)
                {
                }
                else
                {
                    if(mFileStream != null)
                        mFileStream.Close();
                }
            }

            protected String mPath;
            protected FileAccess mFileAccess;
            protected bool mIsDirectory;
            protected IsolatedStorageFile mIsolatedStorage;
            protected IsolatedStorageFileStream mFileStream;

        };

        protected List<File> mFileHandles = new List<File>();

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

                File file = null;
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

                file = new File(path, access);

                if (file.IsDirectory == false)
                {
                    file.TryOpen();
                }

                mFileHandles.Add(file);
                return mFileHandles.Count - 1;
            };

            ioctls.maFileClose = delegate(int _file)
            {
                File file = mFileHandles[_file];
                file.Close();
                return 0;
            };

            ioctls.maFileRead = delegate(int _file, int _dst, int _len)
            {
                File file = mFileHandles[_file];
                if (file.IsDirectory)
                    return MoSync.Constants.MA_FERR_WRONG_TYPE;
                IsolatedStorageFileStream fileStream = file.FileStream;
                if (fileStream == null)
                    return MoSync.Constants.MA_FERR_GENERIC;
                core.GetDataMemory().WriteFromStream(_dst, fileStream, _len);
                return 0;
            };

            ioctls.maFileReadToData = delegate(int _file, int _data, int _offset, int _len)
            {
                File file = mFileHandles[_file];
                if (file.IsDirectory)
                    return MoSync.Constants.MA_FERR_WRONG_TYPE;
                IsolatedStorageFileStream fileStream = file.FileStream;
                if (fileStream == null)
                    return MoSync.Constants.MA_FERR_GENERIC;
                Resource dataRes = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory data = (Memory)dataRes.GetInternalObject();
                data.WriteFromStream(_offset, fileStream, _len);
                return 0;
            };

            ioctls.maFileWriteFromData = delegate(int _file, int _data, int _offset, int _len)
            {
                File file = mFileHandles[_file];
                if (file.IsDirectory)
                    return MoSync.Constants.MA_FERR_WRONG_TYPE;
                IsolatedStorageFileStream fileStream = file.FileStream;
                if (fileStream == null)
                    return MoSync.Constants.MA_FERR_GENERIC;
                Resource dataRes = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory data = (Memory)dataRes.GetInternalObject(); 
                byte[] bytes = new byte[_len];
                data.ReadBytes(bytes, _offset, _len);
                fileStream.Write(bytes, 0, _len);
                return 0;
            };

            ioctls.maFileWrite = delegate(int _file, int _src, int _len)
            {
                File file = mFileHandles[_file];
                if (file.IsDirectory)
                    return MoSync.Constants.MA_FERR_WRONG_TYPE;
                IsolatedStorageFileStream fileStream = file.FileStream;
                if (fileStream == null)
                    return MoSync.Constants.MA_FERR_GENERIC;
                byte[] bytes = new byte[_len];
                core.GetDataMemory().ReadBytes(bytes, _src, _len);
                fileStream.Write(bytes, 0, _len);
                return 0;
            };

            ioctls.maFileSeek = delegate(int _file, int _offset, int _whence)
            {
                File file = mFileHandles[_file];
                if (file.IsDirectory)
                    return MoSync.Constants.MA_FERR_WRONG_TYPE;
                IsolatedStorageFileStream fileStream = file.FileStream;
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
                File file = mFileHandles[_file];
                if (file.IsDirectory)
                    return MoSync.Constants.MA_FERR_WRONG_TYPE;
                IsolatedStorageFileStream fileStream = file.FileStream;
                return (int)fileStream.Position;
            };

            ioctls.maFileExists = delegate(int _file)
            {
                File file = mFileHandles[_file];
                return file.Exists?1:0;
            };

            ioctls.maFileCreate = delegate(int _file)
            {
                File file = mFileHandles[_file];
                if (file.Exists)
                    return MoSync.Constants.MA_FERR_FORBIDDEN;
                file.Create();
                return 0;
            };
        }
    }
}
