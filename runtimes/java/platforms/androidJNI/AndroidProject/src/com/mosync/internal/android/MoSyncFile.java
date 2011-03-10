/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

package com.mosync.internal.android;


import java.io.File;
import java.io.FilenameFilter;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Hashtable;

import android.os.Environment;
import android.os.StatFs;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_SEEK_SET;
import static com.mosync.internal.generated.MAAPI_consts.MA_SEEK_CUR;
import static com.mosync.internal.generated.MAAPI_consts.MA_SEEK_END;

import static com.mosync.internal.generated.MAAPI_consts.MA_ACCESS_READ;
import static com.mosync.internal.generated.MAAPI_consts.MA_ACCESS_READ_WRITE;

import static com.mosync.internal.generated.MAAPI_consts.MA_FERR_GENERIC;
import static com.mosync.internal.generated.MAAPI_consts.MA_FERR_NOTFOUND;
import static com.mosync.internal.generated.MAAPI_consts.MA_FERR_FORBIDDEN;
import static com.mosync.internal.generated.MAAPI_consts.MA_FERR_RENAME_FILESYSTEM;
import static com.mosync.internal.generated.MAAPI_consts.MA_FERR_RENAME_DIRECTORY;

/**
 * MoSync File API
 * 
 * The documentation in this file is more or less
 * copied from the IDL declaration
 * 
 */
public class MoSyncFile {

	MoSyncThread mMoSyncThread;
	
	int mNumFileHandles = 0;
	
	/**
	 * Internal file for handling files
	 */
	class MoSyncFileHandle
	{
		/**
		 * Constructor
		 * @param path The path to this file
		 * @param accessMode The access mode either read or read_write
		 * @throws Throwable
		 */
		MoSyncFileHandle(String path, int accessMode) throws Throwable
		{
			mFile = new File(path);
			mAccessMode = accessMode;
			
			if(path.endsWith("/"))
				mIsAFile = false;
			else
				mIsAFile = true;
			
			mFileChannel = null;
			mCurrentPosition = 0;
		}
		
		/**
		 * Opens the file channel
		 */
		private int openChannel()
		{
			try
			{
				mRandomAccessFile = new RandomAccessFile(mFile, "rw");
				mFileChannel = mRandomAccessFile.getChannel();
				
				if(mCurrentPosition != 0)
					mFileChannel.position(mCurrentPosition);
			}
			catch(Throwable t)
			{
				Log.e("MoSync File API","(Exception) File open : " + t);
				return MA_FERR_GENERIC;
			}
			return 0;
		}
		
		/**
		 * Opens the file
		 */
		public int open()
		{
			if(mIsAFile)
			{
				if(mFileChannel == null || (!mFileChannel.isOpen()))
				{
					return openChannel();
				}
			}
			
			// OOPS.. a directory can't be a file channel
			return MA_FERR_GENERIC;
		}
		
		/**
		 * Closes the file
		 */
		public int close()
		{
			try
			{
				mFileChannel.close();
				mRandomAccessFile.close();
			}
			catch(Throwable t)
			{
				return MA_FERR_GENERIC;
			}
			
			return 0;
		}
		
		public File mFile;
		public int mAccessMode;
		public boolean mIsAFile;
		public FileChannel mFileChannel;
		public int mCurrentPosition;
		private RandomAccessFile mRandomAccessFile;
	}
	
	/**
	 * Hashtable which stores all the active file handles
	 */
	Hashtable<Integer, MoSyncFileHandle> mFileHandles = 
		new Hashtable<Integer, MoSyncFileHandle>();
	
	int mNumFileListings = 0;
	
	/**
	 * Internal class for file listings
	 */
	class MoSyncFileListing
	{
		MoSyncFileListing()
		{
			mIndex = 0;
		}
		
		File[] mFiles;
		int mIndex;
	}
	
	/**
	 * Hashtable which keeps all the active file listings
	 */
	Hashtable<Integer, MoSyncFileListing> mFileListings = 
		new Hashtable<Integer, MoSyncFileListing>();
	
	/**
	 * Constructor File API
	 * @param thread The underlaying MoSync thread
	 */
	public MoSyncFile(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}
	
	/**
	* Opens a file handle.
	* If the file exists, the file will be accessible according to 
	* the mode specified.
	* If the file does not exist, a handle will still be returned and
	* maFileExists() will return false.
	* The file may then be created by calling maFileCreate().
	*
	* Directories may be opened with this function.
	* They cannot be read or written,
	* but they can be created and deleted, and their existence can be queried.
	* Directory paths must end with a slash('/').
	*
	* \note To avoid memory leaks, all opened handles should be closed 
	* using maFileClose().
	*
	* \note To find valid file paths, use maFileListStart().
	*
	* \param path The absolute path to the file.
	* \param mode The access mode. 
	* Either #MA_ACCESS_READ or #MA_ACCESS_READ_WRITE.
	* Directories also use these access modes. #MA_ACCESS_READ lets you 
	* see if the directory exists.
	* #MA_ACCESS_READ_WRITE also lets you create or delete it.
	* \returns The new file handle, or \< 0 on error.
	*
	* \note If any maFile function other than maFileOpen() fails,
	* the state of the file handle becomes boundedly undefined.
	* This means that any operation on it, other than maFileClose(), may fail.
	*/
	int maFileOpen(String path, int mode)
	{
		mNumFileHandles++;
		
		try
		{
			mFileHandles.put(mNumFileHandles, 
							new MoSyncFileHandle(path, mode));
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","(Exception) maOpenFile : " + t);
			return MA_FERR_GENERIC;
		}
		
		return mNumFileHandles;
	}

	/**
	* Returns 1 if the file exists, 0 if it doesn't, or \< 0 on error.
	*/
	int maFileExists(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		if(fileHandle.mFile.exists()) return 1;
		return 0;
	}

	/**
	* Closes a file handle. Returns 0. Panics on error.
	*/
	int maFileClose(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		int ret = fileHandle.close();
		
		mFileHandles.remove(file);
		return ret;
	}

	//see JSR 75 for docs
	/**
	* Creates a file or directory, given an open handle.
	* The file must not exist prior to calling this function.
	* After creation, the file will be accessible according to the mode
	* specified when its handle was opened.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileCreate(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		if(fileHandle.mAccessMode == MA_ACCESS_READ)
			return MA_FERR_FORBIDDEN;
		
		try
		{
			if(fileHandle.mIsAFile)
			{
				fileHandle.mFile.createNewFile();
				return 0;
			}
			else
			{
				if(!fileHandle.mFile.mkdirs())
					return MA_FERR_GENERIC;
				return 0;
			}
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","Got exception:" + t.toString());
			return MA_FERR_GENERIC;
		}
		
	}

	/**
	* Deletes a file or directory.
	* The file must exist prior to calling this function.
	* If it's a directory, it must be empty.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileDelete(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		if(!fileHandle.mFile.exists())
			return MA_FERR_GENERIC;
		
		if(fileHandle.mAccessMode == MA_ACCESS_READ)
			return MA_FERR_FORBIDDEN;
		
		// If this is a directory, it should be empty!
		if(!fileHandle.mIsAFile)
		{
			String[] strings = fileHandle.mFile.list();
			
			// Directory is not empty, won't be able to delete
			if(strings.length != 0)
				return MA_FERR_GENERIC;	
		}
		
		try
		{
			fileHandle.mFile.delete();
		}
		catch(SecurityException se)
		{
			return MA_FERR_FORBIDDEN;
		}
		
		return 0;
	}

	/**
	* Returns the size of the file, or \< 0 on error.
	* \note MoSync cannot handle files larger than 2 GiB (2^31 - 1 bytes).
	* \param file A file handle. The file must exist and must not be a dir.
	*/
	int maFileSize(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		if( !fileHandle.mIsAFile ) return MA_FERR_GENERIC;
		
		return (int)fileHandle.mFile.length();
	}

	/**
	* Returns the number of free bytes on the file system on which \a file 
	* resides.
	* Note that due to file system overhead, it is not possible to use all
	* of this space for file data.
	* Returns \< 0 on error.
	*/
	int maFileAvailableSpace(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;

		// The space function in java.io.File wasn't available on Android
		StatFs fs = new StatFs(fileHandle.mFile.getAbsolutePath()); 

		return (fs.getAvailableBlocks() * fs.getBlockSize());
	}

	/**
	* Returns the total size, in bytes, of the file system on which \a 
	* file resides.
	* Returns \< 0 on error.
	*/
	int maFileTotalSpace(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;

		// The space function in java.io.File wasn't available on Android
		StatFs fs = new StatFs(fileHandle.mFile.getAbsolutePath()); 

		return (fs.getBlockCount() * fs.getBlockSize());
	}

	/**
	* Returns the date/time when the file was last modified, or \< 0 on error.
	* The time format is Unix UTC.
	* \see maTime()
	*/
	int maFileDate(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		return (int)(fileHandle.mFile.lastModified() / 1000);
	}

	/**
	* Renames a file.
	*
	* If \a newName does not contain a slash,
	* the file stays in its original directory.
	*
	* If \a newName does contain a slash, it must be a complete pathname,
	* and the file is moved to that path. The new path must be on the same
	* file system as the original path. If it is not, this function fails 
	* with #MA_FERR_RENAME_FILESYSTEM.
	* Moving a file to another directory is not supported on Java ME. If 
	* it is attempted, this function fails with #MA_FERR_RENAME_DIRECTORY.
	*
	* \returns 0 on success, or \< 0 on error.
	*/
	int maFileRename(int file, int newName)
	{
		
		return MA_FERR_GENERIC;
	}

	/**
	* Truncates the file.
	*
	* If offset is less than the file's size,
	* the file's new size is equal to the offset.
	* All data after the offset is discarded.
	*
	* If the offset is equal to the file's size, this function does nothing.
	*
	* If the offset is greater than the file's current size,
	* this function MAY increase the size of the file.
	* You can determine if it did so by calling maFileSeek() with #MA_SEEK_END.
	*
	* If the file's position is greater than the offset, it is reset
	* to be equal to the offset.
	*
	* The file must exist prior to calling this function.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileTruncate(int file, int offset)
	{
		
		return MA_FERR_GENERIC;
	}

	/**
	 * Sends a ByteBuffer to the given file
	 */
	private int writeByteBufferToFile(
		MoSyncFileHandle fileHandle,
		ByteBuffer byteBuffer)
	{
		try
		{
			int ret = fileHandle.mFileChannel.write(byteBuffer);
			fileHandle.mCurrentPosition =
				(int)fileHandle.mFileChannel.position();
			
			return ret;
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","(Exception) maWriteFile - " + t);
			return MA_FERR_GENERIC;
		}
	}
	
	/**
	 * Stores the data in the given file in a ByteBuffer
	 */
	private int readFileToByteBuffer(
		MoSyncFileHandle fileHandle, 
		ByteBuffer byteBuffer)
	{
		try
		{
			int ret = fileHandle.mFileChannel.read(byteBuffer);
			fileHandle.mCurrentPosition = 
				(int)fileHandle.mFileChannel.position();
			
			return ret;
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","(Exception) maWriteFile - " + t);
			return MA_FERR_GENERIC;
		}
	}
	
	/**
	* Write from memory to file.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileWrite(int file, int src, int len)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		if(fileHandle.mAccessMode == MA_ACCESS_READ)
			return MA_FERR_FORBIDDEN;
		
		int ret = fileHandle.open();
		if(ret != 0)
			return ret;
	
		// Create a sliced buffer which we can send to file
		mMoSyncThread.mMemDataSection.position(src);
		ByteBuffer slicedBuffer = mMoSyncThread.mMemDataSection.slice();
		slicedBuffer.limit(len);
	
		int writtenBytes = writeByteBufferToFile(fileHandle, slicedBuffer);
		if(len != writtenBytes)
			return MA_FERR_GENERIC;
		
		return 0;
	}
	
	/**
	* Write from data object to file.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileWriteFromData(int file, int data, int offset, int len)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		if(fileHandle.mAccessMode == MA_ACCESS_READ)
			return MA_FERR_FORBIDDEN;
		
		ByteBuffer byteBuffer = mMoSyncThread.getBinaryResource(data);
		if (null == byteBuffer)
		{
			Log.e("MoSync File API","(Error) No such data resource");
			return MA_FERR_GENERIC;
		}

		int ret = fileHandle.open();
		if(ret != 0)
			return ret;
	
		// If the whole object should be sent, just send it
		if((offset == 0) && (len == byteBuffer.capacity()))
		{
			writeByteBufferToFile(fileHandle, byteBuffer);
			return 0;
		}
		// Create a sliced buffer which we can send to file
		byteBuffer.position(offset);
		ByteBuffer slicedBuffer = byteBuffer.slice();
		slicedBuffer.limit(len);
	
		int writtenBytes = writeByteBufferToFile(fileHandle, slicedBuffer);
		if(len != writtenBytes)
			return MA_FERR_GENERIC;
		
		return 0;
	}
	
	/**
	* Read from file to memory.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileRead(int file, int dst, int len)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		int ret = fileHandle.open();
		if(ret != 0)
			return ret;
	
		// Create a sliced buffer which we can send to file
		mMoSyncThread.mMemDataSection.position(dst);
		ByteBuffer slicedBuffer = mMoSyncThread.mMemDataSection.slice();
		slicedBuffer.limit(len);
	
		readFileToByteBuffer(fileHandle, slicedBuffer);
		
		return 0;
	}
	
	/**
	* Read from file to data object.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileReadToData(int file, int data, int offset, int len)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		ByteBuffer byteBuffer = mMoSyncThread.getBinaryResource(data);
		if (null == byteBuffer)
		{
			Log.e("MoSync File API","(Error) No such data resource");
			return MA_FERR_GENERIC;
		}

		int ret = fileHandle.open();
		if(ret != 0)
			return ret;
	
		// If the whole object should be sent, just send it
		if((offset == 0) && (len == byteBuffer.capacity()))
		{
			readFileToByteBuffer(fileHandle, byteBuffer);
			return 0;
		}
		// Create a sliced buffer which we can send to file
		byteBuffer.position(offset);
		ByteBuffer slicedBuffer = byteBuffer.slice();
		slicedBuffer.limit(len);
	
		readFileToByteBuffer(fileHandle, slicedBuffer);
		
		return 0;
	}

	/**
	* Returns the file's current position, or \< 0 on error.
	* \param file A file handle. The file must exist and must not be a dir.
	*/
	int maFileTell(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		int ret = fileHandle.open();
		if(ret != 0)
			return ret;
		
		try
		{
			int position = (int)fileHandle.mFileChannel.position();
			return position;
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","(Exception) maFileTell : " + t);
			return MA_FERR_GENERIC;
		}
	}

	/**
	* Sets the file's position.
	* \param file A file handle. The file must exist and must not be a dir.
	* \param offset The offset of the new position,
	* relative to the position specified by \a whence.
	* \param whence One of the \link #MA_SEEK_SET MA_SEEK \endlink constants.
	* \returns The new position, or \< 0 on error.
	*/
	int maFileSeek(int file, int offset, int whence)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if(fileHandle == null)
			return MA_FERR_GENERIC;
		
		int ret = fileHandle.open();
		if(ret != 0)
			return ret;
		
		if(whence == MA_SEEK_SET)
		{
			offset = 0;
		}
		else if(whence == MA_SEEK_END)
		{
			offset = (int)fileHandle.mFile.length();
			if(offset == 0)
				return MA_FERR_GENERIC;
		}
		else if(whence != MA_SEEK_CUR)
		{
			return MA_FERR_GENERIC;
		}
		
		try
		{
			fileHandle.mFileChannel.position(offset);
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","(Exception) maFileSeek : " + t.toString());
			return MA_FERR_GENERIC;
		}
		
		return offset;
	}
	
	/**
	* Creates a listing of names of files and directories, or file systems.
	* Call maFileListNext() repeatedly to retrieve the names. 
	* Call maFileListClose() to free the resources used.
	*
	* It is often useful to begin by listing the file systems.
	*
	* \param path The full path to a directory, or the empty string,
	* which specifies that the root file systems should be listed.
	* \param filter A string to match names with. May include an asterisk ('*')
	*  wildcard to represent 0 or more characters. Ignored if \a path is empty.
	*
	* \returns A File Listing handle, or \< 0 on error.
	*/
	int maFileListStart(String path, String filter)
	{
		mNumFileListings++;
		
		try
		{
			MoSyncFileListing fileListing = new MoSyncFileListing();
			
			if(path.equals(""))
			{
				Log.i("MoSync File API","Getting roots...");
			
				// Only support for sdcard at this point
			
				fileListing.mFiles = new File[1];
				fileListing.mFiles[0] = 
					Environment.getExternalStorageDirectory();
			
			}
			else
			{
				// get a folder listing ( wildcards... not at this point )
				Log.i("MoSync File API","Getting directory...");
				
				File f = new File(path);
				fileListing.mFiles = f.listFiles();
			}
			
			for(int i = 0; i != fileListing.mFiles.length; i++)
				Log.i("MoSync File API", " dir: " 
						+ fileListing.mFiles[i].getPath());
			
			mFileListings.put(mNumFileListings, fileListing);
		}
		catch(SecurityException se)
		{
			Log.e("MoSync File API"," ** SECURITY BREACH!!! ** ");
			return MA_FERR_FORBIDDEN;
		}
		catch(Throwable t)
		{
			Log.e("MoSync File API","(Exception) maFileListStart : " + t);
			return MA_FERR_GENERIC;
		}
		
		return mNumFileListings;
	}

	/**
	* Writes the name of the next file in the list to the specified buffer.
	*
	* The buffer may be too small to contain the name and the terminating zero;
	* in that case, the internal list pointer remains unchanged,
	* the buffer will not be filled,
	* and you should allocate a bigger buffer and call this function again.
	* You may call this function with NULL and 0 to simply retrieve the length
	* of the name.
	*
	* If the name ends with a slash('/'), it is a directory.
	*
	* \returns The length of the name, excluding the terminating zero,
	* or 0 if there are no more files, or \< 0 on error.
	*/
	int maFileListNext(int list, int nameBuf, int bufSize)
	{
		MoSyncFileListing fileListing = mFileListings.get(list);
		if(fileListing == null)
			return MA_FERR_GENERIC;
		
		if(fileListing.mFiles.length == fileListing.mIndex) return 0;
		
		File file = fileListing.mFiles[fileListing.mIndex];
		String name = file.getName();
		
		Log.i("MoSync File API","current path: " + name);
		
		int len = name.length()+1;
		
		if(file.isDirectory())
			len++;
		
		if(bufSize == 0) return len;
		if(len > bufSize) return len;

		byte[] nameChars = name.getBytes();
		
		mMoSyncThread.mMemDataSection.position(nameBuf);	
		mMoSyncThread.mMemDataSection.put(nameChars);
		if(file.isDirectory())
			mMoSyncThread.mMemDataSection.putChar('/');
		mMoSyncThread.mMemDataSection.put((byte)0);
	
		fileListing.mIndex++;
		
		return len-1;
	}

	/**
	* Closes a file listing, freeing its resources.
	*
	* \returns 0.
	*/
	int maFileListClose(int list)
	{
		mFileListings.remove(list);		
		
		return 0;
	}
	
}
