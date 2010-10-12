#include "Slider.h"
#include "Engine.h"
#include <MAUtil/Graphics.h>

namespace MAUI {

Slider::Slider(int x, int y, int width, int height,
	Orientation ori, double minValue, double maxValue, double defaultValue)
: Widget(x, y, width, height),
mPressed(false), mOrientation(ori), mMinValue(minValue), mMaxValue(maxValue),
mValueChanged(false), mMoveToPoint(false), mSliderListeners(false)
{
	setValue(defaultValue);
}

void Slider::setMoveToPoint(bool moveToPoint) {
	mMoveToPoint = moveToPoint;
}

int Slider::getSliderPos() const {
	int sliderPos;
	double normalizedValue = (mValue-mMinValue)/(mMaxValue-mMinValue);
	if(mOrientation == HORIZONTAL) {
		sliderPos = (int) ((double)(mPaddedBounds.width-mSliderGripWidth)*normalizedValue);
	} else {
		sliderPos = (int) ((double)(mPaddedBounds.height-mSliderGripHeight)*normalizedValue);
	}
	return sliderPos;
}

bool Slider::pointerPressed(MAPoint2d p, int id) {
	int sliderPos = getSliderPos();

	if(mMoveToPoint || !mGripImage) {
		double newValue;
		if(mOrientation == HORIZONTAL)
			newValue = mMinValue + (((double)(p.x-(mPaddedBounds.x+(mSliderGripWidth>>1))))*(mMaxValue-mMinValue))/(double)(mPaddedBounds.width-mSliderGripWidth);
		else
			newValue = mMinValue + (((double)(p.y-(mPaddedBounds.y+(mSliderGripHeight>>1))))*(mMaxValue-mMinValue))/(double)(mPaddedBounds.height-mSliderGripHeight);
		setValue(newValue);
		mPressed = true;
	} else {
		if(mOrientation == HORIZONTAL && p.x>(mPaddedBounds.x+sliderPos-mSliderGripWidth) && (p.x<mPaddedBounds.x+sliderPos+mSliderGripWidth*2)) {
			mPressed = true;
		}
		else if(mOrientation == VERTICAL  && p.y>(mPaddedBounds.y+sliderPos-mSliderGripHeight) && p.y<(mPaddedBounds.x+sliderPos+mSliderGripHeight*2)) {
			mPressed = true;
		}
		else {
			mPressed = false;
			return false;
		}
	}

	mStartValue = mValue;
	mStartX = p.x;
	mStartY = p.y;

	return true;
}

bool Slider::pointerMoved(MAPoint2d p, int id) {
	if(mPressed) {
		int relX = p.x-mStartX;
		int relY = p.y-mStartY;
		double newValue;
		if(mOrientation == HORIZONTAL)
			newValue = mStartValue + (((double)relX)*(mMaxValue-mMinValue))/(double)(mPaddedBounds.width-mSliderGripWidth);
		else
			newValue = mStartValue + (((double)relY)*(mMaxValue-mMinValue))/(double)(mPaddedBounds.height-mSliderGripHeight);

		setValue(newValue);
		requestRepaint();
		return true;
	}

	return false;
}

bool Slider::pointerReleased(MAPoint2d p, int id) {
	mPressed = false;
	requestRepaint();
	return false;
}

void Slider::drawWidget() {
	int sliderPos = getSliderPos();
	if(mOrientation == HORIZONTAL) {

		if(mBkgSkin)
			mBkgSkin->draw(sliderPos+(mSliderGripWidth>>1), (mPaddedBounds.height>>1)-(mSliderWeight>>1),
					mPaddedBounds.width-(sliderPos+(mSliderGripWidth>>1)), mSliderWeight);

		if(mAmountSkin)
			mAmountSkin->draw(0, (mPaddedBounds.height>>1)-(mSliderWeight>>1), sliderPos+(mSliderGripWidth>>1), mSliderWeight);

		if(mGripImage)
			Gfx_drawImage(mGripImage, sliderPos, (mPaddedBounds.height>>1)-(mSliderGripHeight>>1));
	} else {
		if(mBkgSkin)
			mBkgSkin->draw((mPaddedBounds.width>>1)-(mSliderWeight>>1), sliderPos+(mSliderGripHeight>>1),
					mSliderWeight, mPaddedBounds.height-(sliderPos+(mSliderGripHeight>>1)));

		if(mAmountSkin)
			mAmountSkin->draw( (mPaddedBounds.width>>1)-(mSliderWeight>>1), 0, mSliderWeight, sliderPos+(mSliderGripHeight>>1));

		if(mGripImage)
			Gfx_drawImage(mGripImage, (mPaddedBounds.width>>1)-(mSliderGripWidth>>1), sliderPos);
	}
}

void Slider::restyle() {
	if(getStyle() == NULL) {
		setStyle(Engine::getSingleton().getDefaultStyle("Slider"));
	}
	Widget::restyle();

	const SliderStyle* style = (const SliderStyle*)getStyle();

	mAmountSkin = style->getSafe<DrawableProperty>("sliderAmountSkin")->mDrawable;
	mBkgSkin = style->getSafe<DrawableProperty>("sliderSkin")->mDrawable;
	ImageProperty* prop = style->get<ImageProperty>("gripImage");

	mAmountSkin->setUseCaching(false);
	mBkgSkin->setUseCaching(false);

	mGripImage = 0;
	mSliderGripWidth = 0;
	mSliderGripHeight = 0;

	if(prop) {
		mGripImage = prop->mHandle;
		mSliderGripWidth = EXTENT_X(maGetImageSize(mGripImage));
		mSliderGripHeight = EXTENT_Y(maGetImageSize(mGripImage));
	} else {

	}

	if(mOrientation == HORIZONTAL)
		mSliderWeight = mSliderGripHeight; //mBkgSkin->getEndY()-mBkgSkin->getStartY();
	else
		mSliderWeight = mSliderGripWidth; //mBkgSkin->getEndX()-mBkgSkin->getStartX();
}

bool Slider::isTransparent() const {
	return true;
}

void Slider::setFocused(bool focused) {
	Widget::setFocused(focused);
	if(mPressed==true && focused == false)
		mPressed = false;
	requestRepaint();
}

void Slider::setMinValue(double min) {
	mMinValue = min;
	requestRepaint();
}

void Slider::setMaxValue(double max) {
	mMaxValue = max;
	requestRepaint();
}

void Slider::setValue(double val) {
	mValue = val;
	if(mValue<mMinValue) mValue=mMinValue;
	else if(mValue>mMaxValue) mValue=mMaxValue;
	mValueChanged = true;
	requestUpdate();
	requestRepaint();
}

void Slider::updateInternal() {
	if(mValueChanged) {
		ListenerSet_fire(SliderListener, mSliderListeners, onValueChange(this, mValue));
		mValueChanged = false;
	}
}

double Slider::getMinValue() const {
	return mMinValue;
}

double Slider::getMaxValue() const {
	return mMaxValue;
}

double Slider::getValue() const {
	return mValue;
}

void Slider::addSliderListener(SliderListener* sl) {
	mSliderListeners.add(sl);
}

void Slider::removeSliderListener(SliderListener* sl) {
	mSliderListeners.remove(sl);
}

SliderStyle::SliderStyle(DrawableProperty* slider_amt, DrawableProperty* slider_bkg, ImageProperty* grip) : Style(0, 0, 0, 0, NULL, NULL)
{
	this->mProperties["sliderSkin"] = slider_bkg;
	this->mProperties["sliderAmountSkin"] = slider_amt;
	this->mProperties["gripImage"] = grip;
}

} // namespace MAUI
