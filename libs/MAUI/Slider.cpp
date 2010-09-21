#include "Slider.h"
#include "Engine.h"
#include <MAUtil/Graphics.h>

namespace MAUI {

Slider::Slider(int x, int y, int width, int height, Widget* parent, Orientation ori, double minValue, double maxValue, double defaultValue) :
	Widget(x, y, width, height, parent), mPressed(false), mOrientation(ori), mMinValue(minValue), mMaxValue(maxValue), mValueChanged(false) {
	setValue(defaultValue);
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
	if(mOrientation == HORIZONTAL && p.x>(mBounds.x+sliderPos-mSliderGripWidth) && (p.x<mBounds.x+sliderPos+mSliderGripWidth*2)) {
		mPressed = true;
	}
	else if(mOrientation == VERTICAL  && p.y>(mBounds.y+sliderPos-mSliderGripHeight) && p.y<(mBounds.x+sliderPos+mSliderGripHeight*2)) {
		mPressed = true;
	}
	else {
		mPressed = false;
		return false;
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
	fireTriggered();
	requestRepaint();
	return false;
}

void Slider::drawWidget() {
	int sliderPos = getSliderPos();
	if(mOrientation == HORIZONTAL) {
		mBkgSkin->drawDirect(sliderPos+(mSliderGripWidth>>1), (mPaddedBounds.height>>1)-(mSliderWeight>>1),
				mPaddedBounds.width-(sliderPos+(mSliderGripWidth>>1)), mSliderWeight);
		mAmountSkin->drawDirect(mPaddedBounds.x, (mPaddedBounds.height>>1)-(mSliderWeight>>1), sliderPos+(mSliderGripWidth>>1), mSliderWeight);
		Gfx_drawImage(mGripImage, sliderPos, (mPaddedBounds.height>>1)-(mSliderGripHeight>>1));
	} else {
		mBkgSkin->drawDirect((mPaddedBounds.width>>1)-(mSliderWeight>>1), sliderPos+(mSliderGripHeight>>1),
				mSliderWeight, mPaddedBounds.height-(sliderPos+(mSliderGripHeight>>1)));
		mAmountSkin->drawDirect( (mPaddedBounds.width>>1)-(mSliderWeight>>1), mPaddedBounds.y, mSliderWeight, sliderPos+(mSliderGripHeight>>1));
		Gfx_drawImage(mGripImage, (mPaddedBounds.width>>1)-(mSliderGripWidth>>1), sliderPos);
	}
}

void Slider::restyle() {
	if(getStyle() == NULL) {
		setStyle(Engine::getSingleton().getDefaultStyle("Slider"));
	}
	Widget::restyle();

	const SliderStyle* style = (const SliderStyle*)getStyle();

	mAmountSkin = style->getSafe<SkinProperty>("sliderAmountSkin");
	mBkgSkin = style->getSafe<SkinProperty>("sliderSkin");
	mGripImage = style->getSafe<ImageProperty>("gripImage")->mHandle;

	mSliderGripWidth = EXTENT_X(maGetImageSize(mGripImage));
	mSliderGripHeight = EXTENT_Y(maGetImageSize(mGripImage));

	if(mOrientation == HORIZONTAL)
		mSliderWeight = mBkgSkin->getEndY()-mBkgSkin->getStartY();
	else
		mSliderWeight = mBkgSkin->getEndX()-mBkgSkin->getStartX();
}

bool Slider::isTransparent() const {
	return true;
}

void Slider::setFocused(bool focused) {
	mPressed = focused;
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
	requestRepaint();
}

void Slider::update() {
	Widget::update();
	if(mValueChanged) {
		fireOnValueChange();
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

void Slider::fireOnValueChange() {
	Vector_each(SliderListener*, wl, mSliderListeners) {
		(*wl)->onValueChange(this, mValue);
	}
}

SliderStyle::SliderStyle(SkinProperty* slider_amt, SkinProperty* slider_bkg, ImageProperty* grip) : Style(0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL)
{
	this->mProperties["sliderSkin"] = slider_bkg;
	this->mProperties["sliderAmountSkin"] = slider_amt;
	this->mProperties["gripImage"] = grip;
}

} // namespace MAUI
