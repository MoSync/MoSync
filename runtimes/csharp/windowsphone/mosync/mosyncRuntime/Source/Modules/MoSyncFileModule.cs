using System;
using System.Net;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;

namespace MoSync
{
    public class FileModule : IIoctlModule, ISyscallModule
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
                if (!mIsDirectory && mIsolatedStorage.FileExists(mPath))
                {
                    mFileStream = mIsolatedStorage.OpenFile(mPath, FileMode.Open,
                        mFileAccess, FileShare.ReadWrite);
                }
            }

            public void Create()
            {
                if (mIsDirectory)
                {
                    if (mIsolatedStorage.DirectoryExists(mPath))
                        throw new Exception("DirectoryExists");
                    mIsolatedStorage.CreateDirectory(mPath);
                }
                else
                {
                    mFileStream = mIsolatedStorage.OpenFile(mPath, FileMode.CreateNew, mFileAccess);
                }
                if (!Exists)
                    throw new Exception("Create");
            }

            public void Delete()
            {
                Close();
                if (mIsDirectory)
                {
                    mIsolatedStorage.DeleteDirectory(mPath);
                }
                else
                {
                    mIsolatedStorage.DeleteFile(mPath);
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
                    {
                        mFileStream.Close();
                        mFileStream = null;
                    }
                }
            }

            public long Size()
            {
                return mFileStream.Length;
            }

            public long AvailableSpace()
            {
                return mIsolatedStorage.AvailableFreeSpace;
            }

            public long TotalSpace()
            {
                return mIsolatedStorage.Quota;
            }

            public DateTimeOffset Date()
            {
                return mIsolatedStorage.GetLastWriteTime(mPath);
            }

            public void Rename(String newName)
            {
                if (mIsDirectory)
                {
                    mIsolatedStorage.MoveDirectory(mPath, newName);
                }
                else
                {
                    mIsolatedStorage.MoveFile(mPath, newName);
                }
                mPath = newName;
            }

            public void Truncate(int offset)
            {
                mFileStream.SetLength(offset);
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

        protected Dictionary<int, File> mStoreHandles = new Dictionary<int, File>();
        protected int mNextStoreHandle = 1;

        protected Dictionary<int, FileList> mFileListHandles = new Dictionary<int, FileList>();
        protected int mNextFileListHandle = 1;

        private String ConvertPath(String path)
        {
            return path.Replace('/', '\\');
        }

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            // todo: store "stores" in a separate location from the filesystem,
            // to avoid clashes.
            syscalls.maOpenStore = delegate(int _name, int _flags)
            {
                String name = core.GetDataMemory().ReadStringAtAddress(_name);
                name = ConvertPath(name);
                File file = new File(name, FileAccess.ReadWrite);
                if (file.IsDirectory)
                {
                    throw new Exception("Invalid store name");
                }
                if (file.Exists)
                    file.TryOpen();
                else if ((_flags & MoSync.Constants.MAS_CREATE_IF_NECESSARY) != 0)
                {
                    file.Create();
                }
                else
                    return MoSync.Constants.STERR_NONEXISTENT;
                if (file.FileStream == null)
                    return MoSync.Constants.STERR_GENERIC;
                mStoreHandles.Add(mNextStoreHandle, file);
                return mNextStoreHandle++;
            };

            syscalls.maWriteStore = delegate(int _store, int _data)
            {
                File file = mStoreHandles[_store];
                IsolatedStorageFileStream fileStream = file.FileStream;
                fileStream.SetLength(0);
                Resource dataRes = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory data = (Memory)dataRes.GetInternalObject();
                fileStream.Write(data.GetData(), 0, data.GetData().Length);
                return 1;
            };

            syscalls.maReadStore = delegate(int _store, int _placeholder)
            {
                File file = mStoreHandles[_store];
                IsolatedStorageFileStream fileStream = file.FileStream;
                Memory mem = new Memory((int)fileStream.Length);
                fileStream.Seek(0, SeekOrigin.Begin);
                fileStream.Read(mem.GetData(), 0, (int)fileStream.Length);
                runtime.SetResource(_placeholder, new Resource(mem, MoSync.Constants.RT_BINARY));
                return MoSync.Constants.RES_OK;
            };

            syscalls.maCloseStore = delegate(int _store, int _delete)
            {
                File file = mStoreHandles[_store];
                file.Close();
                if (_delete != 0)
                    file.Delete();
                mStoreHandles.Remove(_store);
            };
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
                    throw new Exception("Invalid file access mode");
                }

                file = new File(path, access);

                if (file.IsDirectory)
                {
                    if (isolatedStorage.FileExists(path))
                        return MoSync.Constants.MA_FERR_WRONG_TYPE;
                }
                else
                {
                    if (isolatedStorage.DirectoryExists(path))
                        return MoSync.Constants.MA_FERR_WRONG_TYPE;
                    try
                    {
                        file.TryOpen();
                    }
                    catch (IsolatedStorageException e)
                    {
                        MoSync.Util.Log(e);
                        return MoSync.Constants.MA_FERR_GENERIC;
                    }
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

                try
                {
                    return (int)fileStream.Seek(_offset, origin);
                }
                catch (IOException e)
                {
                    MoSync.Util.Log(e);
                    return MoSync.Constants.MA_FERR_GENERIC;
                }
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

            ioctls.maFileDelete = delegate(int _file)
            {
                File file = mFileHandles[_file];
                try
                {
                    file.Delete();
                }
                catch (IsolatedStorageException e)
                {
                    MoSync.Util.Log(e);
                    return MoSync.Constants.MA_FERR_GENERIC;
                }
                return 0;
            };

            ioctls.maFileSize = delegate(int _file)
            {
                File file = mFileHandles[_file];
                return file.Size();
            };

            ioctls.maFileAvailableSpace = delegate(int _file)
            {
                File file = mFileHandles[_file];
                return file.AvailableSpace();
            };

            ioctls.maFileTotalSpace = delegate(int _file)
            {
                File file = mFileHandles[_file];
                return file.TotalSpace();
            };

            ioctls.maFileDate = delegate(int _file)
            {
                File file = mFileHandles[_file];
                return Util.ToUnixTimeUtc(file.Date().ToFileTime());
            };

            ioctls.maFileRename = delegate(int _file, int _newName)
            {
                File file = mFileHandles[_file];
                String newName = core.GetDataMemory().ReadStringAtAddress(_newName);
                newName = ConvertPath(newName);
                if (newName.Contains("\\"))
                {
                    if (newName[0] != '\\')
                        throw new Exception("Invalid newName");
                }
                else
                {   // add directory of old file.
                    newName = Path.GetDirectoryName(file.Path) + "\\" + newName;
                }
                file.Rename(newName);
                return 0;
            };

            ioctls.maFileTruncate = delegate(int _file, int _offset)
            {
                File file = mFileHandles[_file];
                file.Truncate(_offset);
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
