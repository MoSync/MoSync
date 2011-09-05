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
import java.io.IOException;
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
import static com.mosync.internal.generated.MAAPI_consts.MA_FERR_WRONG_TYPE;

/**
 * MoSync File API
 *
 * The documentation in this file is more or less
 * copied from the IDL declaration
 *
 */
public class MoSyncFile {

	MoSyncThread mMoSyncThread;

	int mFileHandleNext = 1;

	void log(String s)
	{
		Log.i("MoSyncFile", s);
	}

	void logerr(String s)
	{
		Log.e("MoSyncFile ERROR", s);
	}

	/**
	 * Internal file for handling files
	 */
	class MoSyncFileHandle
	{
		private final int mAccessMode;
		private final boolean mIsAFile;
		private FileChannel mFileChannel;
		private int mCurrentPosition;
		private File mFile;
		private RandomAccessFile mRandomAccessFile;

		/**
		 * Constructor
		 * @param path The path to this file
		 * @param accessMode The access mode either read or read_write
		 * @throws Throwable
		 */
		MoSyncFileHandle(String path, int accessMode) throws Throwable
		{
			mAccessMode = accessMode;
			if(mAccessMode != MA_ACCESS_READ_WRITE &&
				mAccessMode != MA_ACCESS_READ)
			{
				throw new Error("Invalid access mode!");
			}
			mFile = new File(path);

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
				mRandomAccessFile = new RandomAccessFile(mFile,
					mAccessMode == MA_ACCESS_READ_WRITE ? "rw" : "r");
				mFileChannel = mRandomAccessFile.getChannel();

				if(mCurrentPosition != 0)
					mFileChannel.position(mCurrentPosition);
			}
			catch(Throwable t)
			{
				logerr("openChannel Exception : " + t);
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
				if(mFileChannel != null)
					mFileChannel.close();
				if(mRandomAccessFile != null)
					mRandomAccessFile.close();
			}
			catch(Throwable t)
			{
				t.printStackTrace();
			}

			return 0;
		}
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
	 * @param thread The underlying MoSync thread
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
		log("maFileOpen ("+path+", "+mode+"): "+mFileHandleNext);
		try
		{
			MoSyncFileHandle fileHandle = new MoSyncFileHandle(path, mode);
			if(fileHandle.mFile.exists())
			{
				if(fileHandle.mIsAFile != fileHandle.mFile.isFile())
				{
					return MA_FERR_WRONG_TYPE;
				}

				if(fileHandle.mIsAFile)
				{
					int res = fileHandle.open();
					if(res < 0)
					{
						return res;
					}
				}
			}
			mFileHandles.put(mFileHandleNext, fileHandle);
		}
		catch(Throwable t)
		{
			logerr("maOpenFile Exception: " + t);
			return MA_FERR_GENERIC;
		}

		return mFileHandleNext++;
	}

	/**
	* Returns 1 if the file exists, 0 if it doesn't, or \< 0 on error.
	*/
	int maFileExists(int file)
	{
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		log("maFileExists ("+file+"): "+fileHandle.mFile.exists());
		if(fileHandle.mFile.exists()) { return 1; }
		return 0;
	}

	/**
	* Closes a file handle. Returns 0. Panics on error.
	*/
	int maFileClose(int file)
	{
		log("maFileClose ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			mMoSyncThread.threadPanic(
				0,
				"maFileClose - File does not exist: " + file
			);
		}

		fileHandle.close();
		mFileHandles.remove(file);
		return 0;
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
		log("maFileCreate ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

		if (fileHandle.mAccessMode == MA_ACCESS_READ)
		{
			return MA_FERR_FORBIDDEN;
		}

		try
		{
			if (fileHandle.mIsAFile)
			{
				fileHandle.mFile.createNewFile();
				return fileHandle.open();
			}
			else
			{
				if(!fileHandle.mFile.mkdirs())
				{
					return MA_FERR_GENERIC;
				}
				return 0;
			}
		}
		catch (Throwable t)
		{
			logerr("Got exception:" + t.toString());
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
		log("maFileDelete ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

		if (!fileHandle.mFile.exists())
		{
			return MA_FERR_GENERIC;
		}

		if (fileHandle.mAccessMode == MA_ACCESS_READ)
		{
			return MA_FERR_FORBIDDEN;
		}

		// If this is a directory, it should be empty!
		if (!fileHandle.mIsAFile)
		{
			String[] strings = fileHandle.mFile.list();

			// Directory is not empty, won't be able to delete
			if(strings.length != 0)
			{
				return MA_FERR_GENERIC;
			}
		}

		try
		{
			fileHandle.mFile.delete();
		}
		catch(SecurityException se)
		{
			return MA_FERR_FORBIDDEN;
		}

		log("maFileDelete success ("+file+")");

		return 0;
	}

	/**
	* Returns the size of the file, or \< 0 on error.
	* \note MoSync cannot handle files larger than 2 GiB (2^31 - 1 bytes).
	* \param file A file handle. The file must exist and must not be a dir.
	*/
	int maFileSize(int file)
	{
		log("maFileSize ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

		if( !fileHandle.mIsAFile )
		{
			return MA_FERR_GENERIC;
		}

		try
		{
			return (int)fileHandle.mFileChannel.size();
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return MA_FERR_GENERIC;
		}
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
		log("maFileAvailableSpace ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

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
		log("maFileTotalSpace ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

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
		log("maFileDate ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

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
	int maFileRename(int file, String newName)
	{
		log("maFileRename ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

		String newFilePath;

		// Does the file name have a path?
		if (newName.contains("/"))
		{
			// Yes, it does, use the name as it is.
			newFilePath = newName;
		}
		else
		{
			// No, it does not, assume the same location
			// and build a full path name.
			newFilePath = fileHandle.mFile.getParent() + "/" + newName;
		}

		// Create the new file object.
		File newFile = new File(newFilePath);
		boolean success = fileHandle.mFile.renameTo(newFile);
		if (success)
		{
			log("SUCCESS maFileRename from: " +
				fileHandle.mFile.getAbsolutePath() +
				" to: " + newFile.getAbsolutePath());

			// Rebind the mFile variable to the new file object.
			// The old file object does not point to a valid file anymore.
			fileHandle.mFile = newFile;

			return 0;
		}
		else
		{
			log("FAIL maFileRename from: " +
				fileHandle.mFile.getAbsolutePath() +
				" to: " + newFile.getAbsolutePath());

			return -1;
		}
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
		log("maFileTruncate ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

		try
		{
			fileHandle.mFileChannel.truncate(offset);
			return 0;
		}
		catch (Throwable error)
		{
			logerr("(Exception) maFileTruncate - " + error);
			return MA_FERR_GENERIC;
		}
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

			log("writeByteBufferToFile bytes written: "  + ret);

			fileHandle.mCurrentPosition =
				(int)fileHandle.mFileChannel.position();

			return ret;
		}
		catch (Throwable t)
		{
			logerr("writeByteBufferToFile Exception - " + t);
			t.printStackTrace();
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

			log("readFileToByteBuffer bytes read: "  + ret);

			fileHandle.mCurrentPosition =
				(int)fileHandle.mFileChannel.position();

			return ret;
		}
		catch(Throwable t)
		{
			logerr("readFileToByteBuffer Exception - " + t);
			return MA_FERR_GENERIC;
		}
	}

	/**
	* Write from memory to file.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileWrite(int file, int src, int len)
	{
		log("maFileWrite ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			return MA_FERR_NOTFOUND;
		}

		if(fileHandle.mAccessMode == MA_ACCESS_READ)
		{
			return MA_FERR_FORBIDDEN;
		}

		// Create a sliced buffer which we can send to file
		mMoSyncThread.mMemDataSection.position(src);
		ByteBuffer slicedBuffer = mMoSyncThread.mMemDataSection.slice();
		slicedBuffer.limit(len);

		int writtenBytes = writeByteBufferToFile(fileHandle, slicedBuffer);
		if (len != writtenBytes)
		{
			return MA_FERR_GENERIC;
		}

		return 0;
	}

	/**
	* Write from data object to file.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileWriteFromData(int file, int data, int offset, int len)
	{
		log("maFileWriteFromData ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			logerr("maFileWriteFromData: MA_FERR_NOTFOUND file handle not found");
			return MA_FERR_NOTFOUND;
		}

		if (fileHandle.mAccessMode == MA_ACCESS_READ)
		{
			logerr("maFileWriteFromData: MA_FERR_FORBIDDEN writing forbidden");
			return MA_FERR_FORBIDDEN;
		}

		ByteBuffer byteBuffer = mMoSyncThread.getBinaryResource(data);
		if (null == byteBuffer)
		{
			logerr("maFileWriteFromData: MA_FERR_GENERIC No such data resource");
			return MA_FERR_GENERIC;
		}

		// Set buffer position.
		byteBuffer.position(offset);

		// If the whole object should be written, just write it.
		if ((offset == 0) && (len == byteBuffer.capacity()))
		{
			log("maFileWriteFromData: writing whole buffer length: " + len);

			int writtenBytes = writeByteBufferToFile(fileHandle, byteBuffer);
			if (len != writtenBytes)
			{
				logerr("maFileWriteFromData: MA_FERR_GENERIC error: len != writtenBytes");
				return MA_FERR_GENERIC;
			}
			else
			{
				// Success.
				log("maFileWriteFromData: success");
				return 0;
			}
		}
		else
		{
			log("maFileWriteFromData: writing buffer slice length: " + len);

			// Otherwise create a sliced buffer and write it to file.
			ByteBuffer slicedBuffer = byteBuffer.slice();
			slicedBuffer.limit(len);

			int writtenBytes = writeByteBufferToFile(fileHandle, slicedBuffer);
			if (len != writtenBytes)
			{
				logerr("maFileWriteFromData: MA_FERR_GENERIC error: len != writtenBytes");
				return MA_FERR_GENERIC;
			}
			else
			{
				// Success.
				log("maFileWriteFromData: success");
				return 0;
			}
		}
	}

	/**
	* Read from file to memory.
	* Returns 0 on success, or \< 0 on error.
	*/
	int maFileRead(int file, int dst, int len)
	{
		log("maFileRead ("+file+")");

		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			logerr("maFileRead Error: MA_FERR_NOTFOUND ("+file+")");
			return MA_FERR_NOTFOUND;
		}

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
		log("maFileReadToData ("+file+")");

		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			logerr("maFileReadToData MA_FERR_NOTFOUND ("+file+")");
			return MA_FERR_NOTFOUND;
		}

		ByteBuffer byteBuffer = mMoSyncThread.getBinaryResource(data);
		if (null == byteBuffer)
		{
			logerr("maFileReadToData MA_FERR_GENERIC No such data resource ("+file+")");
			return MA_FERR_GENERIC;
		}

		// Set buffer position.
		byteBuffer.position(offset);

		// If the whole buffer should be read, just read it.
		if ((offset == 0) && (len == byteBuffer.capacity()))
		{
			int readBytes = readFileToByteBuffer(fileHandle, byteBuffer);
			if (len != readBytes)
			{
				logerr("maFileReadToData: MA_FERR_GENERIC error: len != readBytes");
				return MA_FERR_GENERIC;
			}
			else
			{
				// Success.
				log("maFileReadToData: success");
				return 0;
			}
		}
		else
		{
			// Read a slice, create a sliced buffer we read into.
			ByteBuffer slicedBuffer = byteBuffer.slice();
			slicedBuffer.limit(len);
			int readBytes = readFileToByteBuffer(fileHandle, slicedBuffer);
			if (len != readBytes)
			{
				logerr("maFileReadToData: MA_FERR_GENERIC error: len != readBytes");
				return MA_FERR_GENERIC;
			}
			else
			{
				// Success.
				log("maFileReadToData: success");
				return 0;
			}
		}
	}

	/**
	* Returns the file's current position, or \< 0 on error.
	* \param file A file handle. The file must exist and must not be a dir.
	*/
	int maFileTell(int file)
	{
		log("maFileTell ("+file+")");
		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			logerr("maFileTell: MA_FERR_NOTFOUND");
			return MA_FERR_NOTFOUND;
		}

		try
		{
			int position = (int)fileHandle.mFileChannel.position();
			return position;
		}
		catch(Throwable t)
		{
			logerr("maFileTell MA_FERR_GENERIC Exception: " + t);
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
		log("maFileSeek ("+file+")");

		MoSyncFileHandle fileHandle = mFileHandles.get(file);
		if (null == fileHandle)
		{
			logerr("maFileSeek: MA_FERR_NOTFOUND");
			return MA_FERR_NOTFOUND;
		}

		switch(whence) {
		case MA_SEEK_SET:
			break;
		case MA_SEEK_END:
			try {
				offset = (int)fileHandle.mFileChannel.size() + offset;
			} catch(Throwable t) {
				logerr("maFileSeek MA_FERR_GENERIC Exception: " + t.toString());
				return MA_FERR_GENERIC;
			}
			break;
		case MA_SEEK_CUR:
			try {
				offset = (int)fileHandle.mFileChannel.position() + offset;
			} catch(Throwable t) {
				logerr("maFileSeek MA_FERR_GENERIC Exception: " + t.toString());
				return MA_FERR_GENERIC;
			}
			break;
		default:
			throw new Error("maFileSeek: Invalid whence! Throwing Error");
		}

		try
		{
			fileHandle.mFileChannel.position(offset);
		}
		catch(Throwable t)
		{
			logerr("maFileSeek MA_FERR_GENERIC Exception: " + t.toString());
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
		log("maFileListStart ");

		mNumFileListings++;

		try
		{
			MoSyncFileListing fileListing = new MoSyncFileListing();

			if(path.equals(""))
			{
				log("maFileListStart Getting roots...");

				// Only support for sdcard at this point

				fileListing.mFiles = new File[1];
				fileListing.mFiles[0] =
					Environment.getExternalStorageDirectory();

			}
			else
			{
				// get a folder listing ( wildcards... not at this point )
				log("maFileListStart Getting directory...");

				File f = new File(path);
				fileListing.mFiles = f.listFiles();
			}

			for(int i = 0; i != fileListing.mFiles.length; i++)
			{
				log("maFileListStart dir: " + fileListing.mFiles[i].getPath());
			}

			mFileListings.put(mNumFileListings, fileListing);
		}
		catch(SecurityException se)
		{
			logerr("maFileListStart ** SECURITY BREACH!!! ** MA_FERR_FORBIDDEN");
			return MA_FERR_FORBIDDEN;
		}
		catch(Throwable t)
		{
			logerr("maFileListStart MA_FERR_GENERIC Exception: " + t);
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
		log("maFileListNext");

		MoSyncFileListing fileListing = mFileListings.get(list);

		if(fileListing.mFiles.length == fileListing.mIndex) return 0;

		File file = fileListing.mFiles[fileListing.mIndex];
		String name = file.getName();

		log("maFileListNext current path: " + name);

		byte[] nameChars = name.getBytes();
		int len = nameChars.length+1;

		if(file.isDirectory())
		{
			len++;
		}

		if(bufSize == 0) { return len-1; }
		if(len > bufSize) { return len-1; }

		mMoSyncThread.mMemDataSection.position(nameBuf);
		mMoSyncThread.mMemDataSection.put(nameChars);
		if(file.isDirectory())
		{
			mMoSyncThread.mMemDataSection.putChar('/');
		}
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
		log("maFileListClose");

		mFileListings.remove(list);
		return 0;
	}

}
