/*
 Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

#include <ma.h>
#include <mavsprintf.h>
#include <maassert.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>
#include "MAHeaders.h"
#include <MAFS/File.h>

using namespace MAUtil;
using namespace NativeUI;

String getLocalPath() {
	char buffer[1024];
	int size = maGetSystemProperty("mosync.path.local", buffer, 1024);
	MAASSERT(size > 0);
	return String(buffer, size-1);
}

void initFiles() {
	setCurrentFileSystem(R_LOCAL_FILES, 0);
	MAFS_extractCurrentFileSystem(getLocalPath().c_str());
	freeCurrentFileSystem();
}

// this is a test program for the new audio api
// it has options to play sounds directly from urls, from disk or from resources.

enum SoundType {
	RESOURCE, URL
};

struct SoundSource {
	SoundSource() : audioData(-1), audioInstance(-1) {
		name = NULL;
	}

	explicit SoundSource(const char* name, const String& url, const char* mime = NULL) :
		name(name), type(URL), url(url), mime(mime), audioData(-1), audioInstance(-1)
	{
	}

	explicit SoundSource(const char* name, MAHandle handle, const char* mime = NULL) :
		name(name), type(RESOURCE), resource(handle), mime(mime), audioData(-1), audioInstance(-1)
	{
	}

	const char* name;
	SoundType type;

	String url;
	MAHandle resource;

	const char* mime;

	MAAudioData audioData;
	MAAudioInstance audioInstance;
};

// two screens
// one screen to select source
// one screen to set options and play the sound

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet: public Moblet,
	public ListViewListener,
	public ButtonListener,
	public SliderListener,
	public EditBoxListener
{
private:
	Vector<SoundSource> mSoundSources;
	SoundSource* mCurrentSoundSource;
	Button* mPlayButton;
	Button* mPauseButton;
	Button* mStopButton;
	Button* mCreateInstanceButton;
	Button* mDestroyInstanceButton;
	Slider* mVolumeSlider;
	Slider* mPositionSlider;
	EditBox* mNumLoopsEditBox;

public:
	void setupSoundSources() {
		initFiles();

		mSoundSources.add(
			SoundSource("WAV url web",
				"http://www.galaxyfaraway.com/Sounds/EWOK.WAV"));
		mSoundSources.add(
			SoundSource("MP3 url web",
				"http://www.mosync.com/fred/mobilesorcery2.mp3"));
		mSoundSources.add(
			SoundSource("music file mp3 local",
				getLocalPath() + "mobilesorcery2.mp3", "audio/mpeg"));
		mSoundSources.add(
			SoundSource("sound file wav local",
				getLocalPath() + "mobilesorcery2.wav", "audio/x-wav"));
		mSoundSources.add(SoundSource("music mp3 ubin", R_MP3, "audio/mpeg"));
		mSoundSources.add(SoundSource("sound wav bin", R_WAV));

		for (int i = 0; i < mSoundSources.size(); i++) {
			SoundSource& s(mSoundSources[i]);
			if(!s.name)
				maPanic(0, "NULL!!!");
			if (s.type == RESOURCE) {
				if ((s.audioData = maAudioDataCreateFromResource(
					s.mime, s.resource, 0,
					maGetDataSize(s.resource), 0)) < 0)
				{
					char buf[128];
					sprintf(buf, "Failed to initialize sound source %i (%s)!", i, s.name);
					maMessageBox("Error", buf);
				}
			} else if (s.type == URL) {
				if ((s.audioData = maAudioDataCreateFromURL(s.mime,
					s.url.c_str(), s.mime ? 0 : MA_AUDIO_DATA_STREAM))
					< 0)
				{
					char buf[128];
					sprintf(buf, "Failed to initialize sound source %i (%s)!", i, s.name);
					maMessageBox("Error", buf);
				}
			}
		}
	}

	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet() {

		setupSoundSources();
		createUI();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet() {

		// All the children will be deleted.
		delete mSourceSelection;
		delete mSourceOptions;
	}

	void setupSoundSource(SoundSource& soundSource) {
		mCurrentSoundSource = &soundSource;
	}

	void listViewItemClicked(ListView* listView, ListViewItem* listViewItem) {

		int index = (int) listViewItem->getData();
		SoundSource& soundSource = mSoundSources[index];

		setupSoundSource(soundSource);
		mStackScreen->push(mSourceOptions);
	}

	virtual void buttonClicked(Widget* button) {
		if (mPlayButton == button) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox("Error",
					"Invalid audio instance (try to create it first)");
			else
				maAudioPlay(mCurrentSoundSource->audioInstance);
		} else if (mPauseButton == button) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox("Error",
					"Invalid audio instance (try to create it first)");
			else
				maAudioPause(mCurrentSoundSource->audioInstance);
		} else if (mStopButton == button) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox("Error",
					"Invalid audio instance (try to create it first)");
			else
				maAudioStop(mCurrentSoundSource->audioInstance);
		} else if (mCreateInstanceButton == button) {
			if (mCurrentSoundSource->audioInstance >= 0) {
				maMessageBox("Error", "Audio instance is already created.");
			} else {
				mCurrentSoundSource->audioInstance = maAudioInstanceCreate(
					mCurrentSoundSource->audioData);
				if (mCurrentSoundSource->audioInstance < 0) {
					if (mCurrentSoundSource->type == URL) {
						maMessageBox(
							"Error",
							("Failed to create audio instance! Url: "
								+ mCurrentSoundSource->url).c_str());
					} else
						maMessageBox("Error",
							"Failed to create audio instance!");
				}
			}
		} else if (mDestroyInstanceButton == button) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox(
					"Error",
					"Audio instance is already destroyed or never created.");
			else {
				maAudioInstanceDestroy(mCurrentSoundSource->audioInstance);
				mCurrentSoundSource->audioInstance = -1;
			}
		}
	}

	virtual void sliderValueChanged(Slider* slider, const int sliderValue) {
		if (slider == mVolumeSlider) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox(
					"Error",
					"Audio instance is already destroyed or never created.");
			else {
				maAudioSetVolume(mCurrentSoundSource->audioInstance,
					(float) sliderValue / 100.0f);
			}
		} else if (slider == mPositionSlider) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox(
					"Error",
					"Audio instance is already destroyed or never created.");
			else {
				int length = maAudioGetLength(
					mCurrentSoundSource->audioInstance);
				maAudioSetPosition(mCurrentSoundSource->audioInstance,
					length * sliderValue / 100);
			}
		}
	}

	virtual void editBoxReturn(EditBox* editBox) {
		if (editBox == mNumLoopsEditBox) {
			if (mCurrentSoundSource->audioInstance < 0)
				maMessageBox(
					"Error",
					"Audio instance is already destroyed or never created.");
			else {
				int numLoops = stringToInteger(mNumLoopsEditBox->getText());
				mNumLoopsEditBox->setText(integerToString(numLoops));
				maAudioSetNumberOfLoops(mCurrentSoundSource->audioInstance,
					numLoops);
			}

			mNumLoopsEditBox->hideKeyboard();
		}
	}

	void createSourceSelectionScreen(Screen* screen) {
		ListView* listView = new ListView();
		listView->fillSpaceHorizontally();
		listView->fillSpaceVertically();

		listView->addListViewListener(this);

		for (int i = 0; i < mSoundSources.size(); i++) {
			ListViewItem* item = new ListViewItem();
			item->setText(mSoundSources[i].name);
			item->setData((void*) i);
			listView->addChild(item);
		}

		screen->setMainWidget(listView);
	}

	HorizontalLayout* createLabeledHorizontalLayout(const char* string,
			Widget* w) {
		HorizontalLayout* hl = new HorizontalLayout();
		hl->fillSpaceHorizontally();
		hl->wrapContentVertically();

		Label* label = new Label();
		label->setText(string);
		label->wrapContentVertically();
		label->wrapContentHorizontally();

		hl->addChild(label);
		hl->addChild(w);
		return hl;
	}

	void createSourceOptionsScreen(Screen* screen) {
		ListView* listView = new ListView();
		listView->fillSpaceHorizontally();
		listView->fillSpaceVertically();

		mCreateInstanceButton = new Button();
		mCreateInstanceButton->setText("create instance");
		mCreateInstanceButton->wrapContentVertically();
		mCreateInstanceButton->wrapContentHorizontally();
		mCreateInstanceButton->addButtonListener(this);

		mDestroyInstanceButton = new Button();
		mDestroyInstanceButton->setText("destroy instance");
		mDestroyInstanceButton->wrapContentVertically();
		mDestroyInstanceButton->wrapContentHorizontally();
		mDestroyInstanceButton->addButtonListener(this);

		mPlayButton = new Button();
		mPlayButton->setText("play");
		mPlayButton->wrapContentVertically();
		mPlayButton->wrapContentHorizontally();
		mPlayButton->addButtonListener(this);

		mPauseButton = new Button();
		mPauseButton->setText("pause");
		mPauseButton->wrapContentVertically();
		mPauseButton->wrapContentHorizontally();
		mPauseButton->addButtonListener(this);

		mStopButton = new Button();
		mStopButton->setText("stop");
		mStopButton->wrapContentVertically();
		mStopButton->wrapContentHorizontally();
		mStopButton->addButtonListener(this);

		mVolumeSlider = new Slider();
		mVolumeSlider->setMaximumValue(100);
		mVolumeSlider->setValue(100);
		mVolumeSlider->wrapContentVertically();
		mVolumeSlider->fillSpaceHorizontally();
		mVolumeSlider->addSliderListener(this);

		mPositionSlider = new Slider();
		mPositionSlider->setMaximumValue(100);
		mPositionSlider->setValue(100);
		mPositionSlider->wrapContentVertically();
		mPositionSlider->fillSpaceHorizontally();
		mPositionSlider->addSliderListener(this);

		mNumLoopsEditBox = new EditBox();
		mNumLoopsEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		mNumLoopsEditBox->setText("0");
		mNumLoopsEditBox->wrapContentVertically();
		mNumLoopsEditBox->fillSpaceHorizontally();
		mNumLoopsEditBox->addEditBoxListener(this);

		listView->addChild(mCreateInstanceButton);
		listView->addChild(mDestroyInstanceButton);
		listView->addChild(
				createLabeledHorizontalLayout("num loops: ", mNumLoopsEditBox));
		listView->addChild(mPlayButton);
		listView->addChild(mPauseButton);
		listView->addChild(mStopButton);
		listView->addChild(
				createLabeledHorizontalLayout("volume: ", mVolumeSlider));
		listView->addChild(
				createLabeledHorizontalLayout("position: ", mPositionSlider));

		screen->setMainWidget(listView);
	}

	/**
	 * Create the user interface.
	 */
	void createUI() {
		// Create a NativeUI screen that will hold layout and widgets.
		mSourceSelection = new Screen();
		mSourceSelection->setTitle("Sources");
		mSourceOptions = new Screen();
		mSourceOptions->setTitle("Options");
		mStackScreen = new StackScreen();

		createSourceSelectionScreen(mSourceSelection);
		createSourceOptionsScreen(mSourceOptions);

		mStackScreen->push(mSourceSelection);
		//Show the screen
		mStackScreen->show();
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			// Call close to exit the application.
			close();
		}
	}

private:
	StackScreen* mStackScreen;
	Screen* mSourceSelection; //A Native UI screen
	Screen* mSourceOptions; //A Native UI screen
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain() {
	Moblet::run(new NativeUIMoblet());
	return 0;
}
