package com.mosync.nativeui.ui.custom;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.widget.ArrayAdapter;

import com.mosync.internal.android.MoSyncThread;

/*
 * Helpers for maOptionDialog syscalls.
 */
public class MoSyncOptionsDialog
{

	//-------------------------- IMPLEMENTATION --------------------------//

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncOptionsDialog(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mOptionItems = new ArrayList<CharSequence>();
	}

	/**
	 * Create and display the dialog.
	 */
	public void showDialog(String title, String destructiveBtnTitle, String cancelBtnText, String[] options)
	{
		// Initialize the selected option index.
		mSelectedOption = -1;
		mOptionItems.clear();

		if ( destructiveBtnTitle.length() > 0 )
		{
			mOptionItems.add(destructiveBtnTitle);
		}

		for (int i=0; i < options.length; i++)
		{
			mOptionItems.add(options[i]);
		}

		mAdapter = new ArrayAdapter<CharSequence>(getActivity(), android.R.layout.select_dialog_item, mOptionItems);
		// or: select dialog single choice items and set dialogbuilder.setSingleChoiceItems

		// Create the dialog.
		AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(getActivity());
		dialogBuilder.setTitle(title);

		if ( cancelBtnText.length() > 0 )
		{
			dialogBuilder.setCancelable(true);
			dialogBuilder.setNegativeButton( cancelBtnText ,
			        new DialogInterface.OnClickListener() {

				        @Override
				        public void onClick(DialogInterface dialog, int index)
				        {
							// Compute the index: cancel gets the last position.
							postOptionDialogEvent(mOptionItems.size()+1);
				        }
			        });
		}
		else
		{
			dialogBuilder.setCancelable(false);
		}

		// Set the list adapter.
		dialogBuilder.setAdapter(mAdapter, new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int index) {
				mSelectedOption = index;
				postOptionDialogEvent(mSelectedOption);
			}
		});

		// Create the dialog and show it.
		AlertDialog dialog = dialogBuilder.create();
		// Display the dialog until user provides a selection, or cancels the dialog.
		dialog.show();
	}

    //--------------------------   Utility functions to parse the options array from memory  --------------------------//

	/**
	 * Parse buffer from memory and fill the string array.
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	public String[] parseStringFromMemory(int buffPointer, int buffSize)
	{
		char[] buffer = readBufferFromMemory(buffPointer, buffSize >> 1);
		return readStringArray(buffer);
	}

	/**
	 * Read string from system memory.
	 */
	char[] readBufferFromMemory(int address, int length)
	{
		char[] buffer = new char[length];
		getMemDataSection().position(address);
		for (int i=0; i<length; i++)
		{
			buffer[i] = getMemDataSection().getChar();
		}
		return buffer;
	}

	/**
	 * @return The MoSync data section memory buffer.
	 */
	public ByteBuffer getMemDataSection()
	{
		return mMoSyncThread.mMemDataSection;
	}

	/**
	 * Parse a buffer into an array.
	 * @param buffer
	 * @return
	 */
	String[] readStringArray(char[] buffer)
	{
		int buffIndex = 0;
		int length = readInt(buffer, buffIndex);
		buffIndex += 2;
		String[] val = new String[length];
		for (int i = 0; i < length; i++)
		{
			int crtIndex = buffIndex;
			while (buffer[crtIndex++] != 0)
				;
			crtIndex--;
			int len = crtIndex - buffIndex;
			char[] tmp = new char[len];
			System.arraycopy(buffer, buffIndex, tmp, 0, len);
			val[i] = new String(tmp);
			tmp = null;
			buffIndex += val[i].length();
			buffIndex++;
		}

		return val;
	}

	int readInt(char[] buffer, int index)
	{
		return (buffer[index] | (buffer[index + 1] << 16));
	}

	/**
	 * Add one option to the list.
	 * @param option
	 */
	public void addOption(String option)
	{
		mAdapter.add(option);
		mAdapter.notifyDataSetChanged();
	}

	public void removeLastOption()
	{
		// If we have at least 1opt
		mAdapter.remove( mAdapter.getItem(mAdapter.getCount()-1) );
		mAdapter.notifyDataSetChanged();
	}

	/**
	 * Post a message to the MoSync event queue.
	 * Send the index of the clicked option item.
	 */
	private void postOptionDialogEvent(int index)
	{
		int[] event = new int[2];
		event[0] = EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED;
		event[1] = index;

		mMoSyncThread.postEvent(event);
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

    //--------------------------   Members   --------------------------//

	/**
	 * The selected option in the dialog.
	 */
	private int mSelectedOption = -1;

	/**
	 * The available list of options.
	 */
	private List<CharSequence> mOptionItems;

	/**
	 * The adapter that holds the array of option values.
	 */
	private ArrayAdapter<CharSequence> mAdapter;

	/**
	 * The MoSync thread object.
	 */
    private MoSyncThread mMoSyncThread;
}
