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
                mIsDirectory = mPath.Length == 0 || mPath.EndsWith("\\");
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
                    if (mFileStream != null)
                        mFileStream.Close();
                }
            }

            protected String mPath;
            protected FileAccess mFileAccess;
            protected bool mIsDirectory;
            protected IsolatedStorageFile mIsolatedStorage;
            protected IsolatedStorageFileStream mFileStream;

        };

        public class FileList
        {
            public String[] dirs, files;
            // range: 0 => dirs.Length + files.Length.
            // read from files if(pos >= dirs.Length).
            public int pos;
        }

        protected Dictionary<int, File> mFileHandles = new Dictionary<int, File>();
        protected int mNextFileHandle = 1;

        protected Dictionary<int, FileList> mFileListHandles = new Dictionary<int, FileList>();
        protected int mNextFileListHandle = 1;

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
                    // The isolated storage becomes the "root"
                    return "/";
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

                mFileHandles.Add(mNextFileHandle, file);
                return mNextFileHandle++;
            };

            ioctls.maFileClose = delegate(int _file)
            {
                File file = mFileHandles[_file];
                file.Close();
                mFileHandles.Remove(_file);
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
                        throw new Exception("maFileSeek whence");
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
                return file.Exists ? 1 : 0;
            };

            ioctls.maFileCreate = delegate(int _file)
            {
                File file = mFileHandles[_file];
                if (file.Exists)
                    return MoSync.Constants.MA_FERR_GENERIC;
                file.Create();
                return 0;
            };

            ioctls.maFileListStart = delegate(int _path, int _filter, int _sorting)
            {
                // todo: respect _sorting.
                String path = core.GetDataMemory().ReadStringAtAddress(_path);
                path = ConvertPath(path);
                String filter = core.GetDataMemory().ReadStringAtAddress(_filter);
                String pattern = path + filter;
                IsolatedStorageFile isf = IsolatedStorageFile.GetUserStoreForApplication();
                FileList fl = new FileList();
                fl.dirs = isf.GetDirectoryNames(pattern);
                fl.files = isf.GetFileNames(pattern);
                fl.pos = 0;

                mFileListHandles.Add(mNextFileListHandle, fl);
                return mNextFileListHandle++;
            };

            ioctls.maFileListNext = delegate(int _list, int _nameBuf, int _bufSize)
            {
                FileList fl = mFileListHandles[_list];
                String name;
                if (fl.pos < fl.dirs.Length)
                    name = fl.dirs[fl.pos] + "/";
                else if (fl.pos < fl.dirs.Length + fl.files.Length)
                    name = fl.files[fl.pos - fl.dirs.Length];
                else
                    return 0;
                if (name.Length >= _bufSize)
                    return name.Length;
                core.GetDataMemory().WriteStringAtAddress(_nameBuf,
                    name, _bufSize);
                fl.pos++;
                return name.Length;
            };

            ioctls.maFileListClose = delegate(int _list)
            {
                FileList fl = mFileListHandles[_list];
                mFileListHandles.Remove(_list);
                return 0;
            };
        }
    }
}
