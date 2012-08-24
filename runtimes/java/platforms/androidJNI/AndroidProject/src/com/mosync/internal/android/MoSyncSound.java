/* Copyright (C) 2010 MoSync AB

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

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Hashtable;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.MediaPlayer;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread.UBinData;

/**
 * Class that implements sound syscalls.
 */
public class MoSyncSound
{
	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	private MediaPlayer mMediaPlayer;

	private int mSoundVolume;

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncSound(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mSoundVolume = 50;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/************************ Syscalls ************************/

	/**
	 * Play a sound.
	 *
	 * @param handle Handle to data object that contains sound data.
	 * @param offset Offset from the beginning of the data object
	 * to the start of the sound data (mime type + audio data).
	 * @param length Length of sound data (including mime header).
	 */
	int maSoundPlay(int handle, int offset, int length)
	{
		Log.i("@@@@@@@", "maSoundPlay handle: "
			+ handle + " offset: " + offset + " length: " + length);
		try
		{
			// Free current MediaPlayer instance.
			if (null != mMediaPlayer)
			{
				mMediaPlayer.release();
				mMediaPlayer = null;
			}

			// Create new MediaPlayer instance.
			mMediaPlayer = new MediaPlayer();

			SYSLOG("MoSyncSound.maSoundPlay()" +
				"Reading audio resource:" + handle);

			// Get audio information.
			AudioStore audioStore = obtainAudioStoreObject(handle, offset);
			if (audioStore == null)
			{
				Log.e("MoSyncSound.maSoundPlay",
					"No audio resource with handle: "
					+ handle + " found!");
				return -1;
			}

			// Open the audio file.
			FileInputStream stream = getActivity().openFileInput(
				audioStore.mTemporaryFileName);

			// Find beginning of audio data (starts after mime header).
			if (offset > 0)
			{
				long skipped = stream.skip(offset);
				if (skipped != offset)
				{
					Log.e("MoSyncSound.maSoundPlay",
						"Could not locate start of sound data " +
						"for handle: " + handle + " at offset: " + offset);
					return -1;
				}
			}

			// Read past the mime header (mime type ends with zero byte).
			int mimeLength = 0;
			int b;
			do
			{
				b = stream.read();
				++mimeLength;
			}
			while (0 != b);

			// Log.i("@@@@@@@", "maSoundPlay mimeLength: " + mimeLength);

			// Get file descriptor for audio file.
			FileDescriptor fileDesc = stream.getFD();
			if (null == fileDesc)
			{
				//Log.e("MoSyncSound.maSoundPlay", "File Descriptor was null!");
				return -1;
			}

			// Play sound.
			mMediaPlayer.setDataSource(
				fileDesc,
				offset + mimeLength,
				length - mimeLength);
			mMediaPlayer.prepare();
			mMediaPlayer.start();

		}
		catch (Exception ex)
		{
			//Log.e("MoSyncAudio.maSoundPlay", "Exception:" + ex.toString());
			ex.printStackTrace();
			return -1;
		}

		return 1;
	}

	// FIXME: throws MediaException - Is this still the case?!
	void maSoundStop()
	{
		if (null == mMediaPlayer)
		{
			return;
		}

		if (mMediaPlayer.isPlaying())
		{
			mMediaPlayer.stop();
		}
	}

	/**
	 * Set the sound volume.
	 * @param vol Volume in the range 0 - 100.
	 */
	void maSoundSetVolume(int vol)
	{
		if (null == mMediaPlayer)
		{
			return;
		}

		if (vol < 0)
		{
			vol = 0;
		}
		else if (vol > 100)
		{
			vol = 100;
		}

		mSoundVolume = vol;

		// This sets the volume within the span 0 : current system volume.
		// This is the specified behaviour of maSoundSetVolume in the MoSync API.
		float volume = (float)mSoundVolume / 100.0f;
		mMediaPlayer.setVolume(volume, volume);

		/*
		// This code sets the system volume.
		// Not used, as this behaviour is not the specified one in MoSync.
		try
		{
			// Use audio manager to set the volume or the stream.
			// getStreamMaxVolume is typically a quite low number, e.g. 15.
			AudioManager audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
			int maxVolume = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
			int newVolume = (int) (((float)mSoundVolume / 100f) * (float)maxVolume);
			audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, newVolume, 0);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		*/
	}

	int maSoundGetVolume()
	{
		return mSoundVolume;
	}

	/**
	 * @return 1 if the sound is playing, otherwise 0
	 */
	int maSoundIsPlaying()
	{
		if (null == mMediaPlayer)
		{
			return 0;
		}

		if (mMediaPlayer.isPlaying())
		{
			return 1;
		}

		return 0;
	}

	/************************ Implementation Support ************************/

	/**
	 * Counter for number of temporary audio files.
	 * On Android the sound data must be played from a file,
	 * you cannot play sound from memory. This is why we store
	 * audio data objects in temporary files.
	 */
	int mNumTempAudioFiles = 1;

	/**
	 * Table with AudioStore objects that keep track of audio data.
	 */
	Hashtable<Integer, AudioStore> mAudioStores =
		new Hashtable<Integer, AudioStore>();

	/**
	 * This class is used for storing the filename
	 * of an audio object.
	 */
	class AudioStore
	{
		/**
		 * The file name of the audio object. Android needs
		 * sound to be in a file to play it?
		 * TODO: Perhaps a memory file would work? Is that an option?
		 * TODO: Another thing, is the temporary file ever deleted?
		 */
		public String mTemporaryFileName;

		/**
		 * Constructor.
		 * @param mimeType The mime string, currently not used.
		 * @param size The size of the audio data, currently not used.
		 * @param offset Offset into data buffer.
		 * @param temporaryFileName The name of the audio file.
		 */
		AudioStore(String temporaryFileName)
		{
			SYSLOG("AudioStore temp file savec :" + temporaryFileName);
			mTemporaryFileName = temporaryFileName;
		}
	};

	/**
	 * If the given resource is an audio file, store it as a file.
	 * This is done because we need an audio file to play the sound.
	 * @param soundHandle The handle to the sound object. The audio object
	 * @param dataOffset Offset into data object where mime type + audio data begins.
	 * begins with a null terminated mime string, then follows audio data.
	 */
	void storeIfBinaryAudioResource(int soundHandle, int offset)
	{
		ByteBuffer audioData = mMoSyncThread.getBinaryResource(soundHandle);
		if (null == audioData)
		{
			SYSLOG("MoSyncAudio.storeIfBinaryAudioResource: "
				+ "Sound data object not found");
			// TODO: What about returning an error code?
			return;
		}

		// Is there a mime type at the offset?
		if (!checkIfMimeAudioType(audioData, offset))
		{
			// No it was not.
			SYSLOG("MoSyncAudio.storeIfBinaryAudioResource: "
				+ "Not an audio object!");
			// TODO: What about returning an error code?
			return;
		}

		// Check that we can read the mime string.
		String mimeType = readMimeString(audioData, offset);
		if (mimeType == null)
		{
			SYSLOG("MoSyncAudio.storeIfBinaryAudioResource: No mime type!");
			// TODO: What about returning an error code?
			return;
		}
/*
		// Determine the length of the audio data.
		int mimeStringLength = mimeType.length() + 1;
*/
		// Write the entire data object to file.
		try
		{
			// Create file name.
			String fileName = "MOSYNCTEMP:audio" + soundHandle + ".tmp";

			// Create a temporary audio file.
			FileOutputStream audioFileOutputStream =
				getActivity().openFileOutput(
					fileName,
					Context.MODE_WORLD_READABLE | Context.MODE_WORLD_WRITEABLE);

			// Open a file channel to write audio data to.
			FileChannel channel = audioFileOutputStream.getChannel();

			// Set position to start of audio data.
			audioData.position(0);

			// Write data.
			channel.write(audioData);

			// Close the channel, also closes the output stream.
			channel.close();

			// Add entry to audio resource table.
			mAudioStores.put(soundHandle, new AudioStore(fileName));
		}
		catch (Exception ex)
		{
			Log.e("MoSyncSound.storeIfBinaryAudioResource",
				"Unable to save temporary audio file.");
			ex.printStackTrace();
			// TODO: What about returning an error code?
		}
	}

	/**
	 * If this is an audio ubin, we store the resource as a file that
	 * can be played with MediaPlayer.
	 * @param ubinData An object holding information about this resource.
	 * @param resHandle The id of the audio resource.
	 */
	public void storeIfAudioUBin(UBinData ubinData, int resHandle)
	{
		SYSLOG("MoSyncSound.storeIfAudioUBin - ubinData.getSize(): " +
			ubinData.getSize() + "bytes");

		if (!checkIfMimeAudioType(ubinData))
		{
			// This is not an audio resource.
			return;
		}

		// Write content of entire ubin to file (including the mime header).
		try
		{
			// Open the resource file.
			AssetManager assetManager = getActivity().getAssets();
			// RESOURCE_FILE = "resources.mp3"
			AssetFileDescriptor fileDescriptor =
				assetManager.openFd(MoSyncThread.RESOURCE_FILE);
			FileInputStream inputStream = fileDescriptor.createInputStream();

			// Jump to beginning of resource.
			inputStream.skip(
				ubinData.getOffset() - mMoSyncThread.getResourceStartOffset());

			// Create buffer to hold audio data (mime type plus sound data).
			byte[] buffer = new byte[ubinData.getSize()];

			// Read data.
			inputStream.read(buffer);

			// Close input stream.
			inputStream.close();

			// Create a temporary file.
			String fileName = "MOSYNCTEMP:audio" + resHandle + ".tmp";
			FileOutputStream outputStream = getActivity().openFileOutput(
				fileName,
				Context.MODE_WORLD_READABLE | Context.MODE_WORLD_WRITEABLE);

			// Write audio data.
			outputStream.write(buffer);

			// Close output stream.
			outputStream.close();

			// TODO: Unify AudioStore with UBinData ?
			// Store sound data in audio table.
			mAudioStores.put(resHandle, new AudioStore(fileName));
		}
		catch(Exception ex)
		{
			Log.e("MoSyncAudio.storeIfAudioUBin", "Unable to save temporary audio file.");
			ex.printStackTrace();
		}
	}

	/**
	 * Get the AudioStore object for the given audio handle and write an
	 * audio file and create an AudioStore object if it does not exist.
	 * @param soundHandle The audio handle.
	 * @param offset The offset into the data where the mime type
	 * and sound is located. The mime type is a null-terminated string.
	 * @return An AudioStore object for the sound or null if there is an error.
	 */
	AudioStore obtainAudioStoreObject(int soundHandle, int offset)
	{
		// Does the sound already exist in the AudioStore table?
		AudioStore audioStore =
			(AudioStore) mAudioStores.get(new Integer(soundHandle));
		if (audioStore != null)
		{
			// Yes it exists.
			return audioStore;
		}
		else
		{
			// The sound did not exist. Store it.
			storeIfBinaryAudioResource(soundHandle, offset);

			// If this handle contained proper sound data there
			// should now be an audio object in the audio store.
			// If not, null will be returned.
			return (AudioStore) mAudioStores.get(new Integer(soundHandle));
		}
	}

	/**
	 * Checks if the header is an audio mime type.
	 * @param header The first five bytes of an audio data object.
	 * @return true if the header is of type "audio", false if not.
	 */
	final boolean checkIfMimeAudioType(byte[] header)
	{
		return
			(byte)'a' == header[0] &&
			(byte)'u' == header[1] &&
			(byte)'d' == header[2] &&
			(byte)'i' == header[3] &&
			(byte)'o' == header[4];
	}

	/**
	 * Checks if audio data is of audio mime type.
	 * @param audioData A buffer containing audio data.
	 * @param offset The offset into the data where the mime type
	 * and sound is located. The mime type is a null-terminated string.
	 * @return true if the audioData is of type "audio", false if not.
	 */
	final boolean checkIfMimeAudioType(ByteBuffer audioData, int offset)
	{
		// No point in checking audio type unless we have at least five bytes.
		if (audioData.limit() < 5)
		{
			//Log.e("MoSyncAudio.checkIfMimeAudioType",
			//	"Resource was smaller than 5 bytes");
			return false;
		}

		// Save current position on the byte buffer.
		int savedPosition = audioData.position();

		// Read first five bytes of audio data.
		byte header[] = new byte[5];
		audioData.position(offset);
		audioData.get(header);

		// Restore buffer position.
		audioData.position(savedPosition);

		// Check mime type.
		return checkIfMimeAudioType(header);
	}

	/**
	 * Checks if the header of a resource is of audio mime type.
	 * @param ubinData A description of a ubin resource.
	 * @return true if the header is of type "audio", false if not.
	 */
	final boolean checkIfMimeAudioType(UBinData ubinData)
	{
		try
		{
			// Open the resource file.
			AssetManager assetManager = getActivity().getAssets();
			// RESOURCE_FILE = "resources.mp3"
			AssetFileDescriptor fileDescriptor =
				assetManager.openFd(MoSyncThread.RESOURCE_FILE);
			FileInputStream stream = fileDescriptor.createInputStream();

			// Jump to start of this audio resource.
			stream.skip(
				ubinData.getOffset() - mMoSyncThread.getResourceStartOffset());

			// Determine if this is an audio file by examining the header bytes.
			byte header[] = new byte[5];
			stream.read(header);
			stream.close();
			return checkIfMimeAudioType(header);
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return false;
		}
	}

	/**
	 * Reads the MIME string from a file. Note that the method
	 * advances the file pointer to the byte after the mime string,
	 * that is, to the start of the audio data.
	 * @param input The input file stream.
	 * @return The mime type string.
	 */
	final String readMimeStringFromFile(FileInputStream input)
		throws Exception
	{
		StringBuffer mimeType = new StringBuffer();

		int b;

		while ((b = input.read()) != 0)
		{
			if (b == -1)
			{
				throw new BigPhatError(
					"MoSyncSound.readMimeStringFromFile: End of input");
			}
			mimeType.append((char)b);
		}

		return new String(mimeType);
	}

	/**
	 * Reads the MIME string at the beginning of the data buffer and returns it.
	 * @param buffer The ByteBuffer that contains audio data.
	 *   Format is <mime string><zero byte><audio data>.
	 * @return The string containing the mime type.
	 *   Returns null if there was a an error.
	 */
	final String readMimeString(ByteBuffer audioBuffer, int offset)
	{
		// Mime type string data.
		StringBuffer mimeType = new StringBuffer();

		// Save current buffer position.
		int savedBufferPosition = audioBuffer.position();

		// Set position to the start of the buffer.
		audioBuffer.position(offset);

		try
		{
			byte b;
			while (true)
			{
				// Get next byte and check if it is the
				// zero termination byte.
				b = audioBuffer.get();
				if (b == 0)
				{
					break;
				}

				// Add byte to mime type string.
				mimeType.append((char)b);
			}
		}
		catch (BufferUnderflowException ex)
		{
			ex.printStackTrace();
			return null;
		}
		finally
		{
			// Restore the buffer position.
			audioBuffer.position(savedBufferPosition);
		}

		return new String(mimeType);
	}
}
