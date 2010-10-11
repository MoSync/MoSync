#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/Button.h>
#include <MAUI/Slider.h>
#include <MAUI/Engine.h>
#include <MAUI/GridLayout.h>
#include <MAUI/StackLayout.h>

#include <MAUI/ListBox.h>

#include <MAUtil/Stack.h>

#include "MAHeaders.h"

#include <conprint.h>
#include <mastdlib.h>

#include <MAUtil/Graphics.h>

#include "MAUI/NativeEditBox.h"

using namespace MAUtil;
using namespace MAUI;

template <typename T>
class Queue {
public:
	void put(const T& t) {
		mQ.add(t);
	}

	const T& peek() const {
		if(mQ.size()==0) maPanic(1, "Empty queue!");
		return mQ[0];
	}

	void pop() {
		if(mQ.size()==0) maPanic(1, "Empty queue!");
		mQ.erase(0);
	}

	void clear() {
		mQ.clear();
	}

	Vector<T> mQ;
};

struct Value {
	enum Type {
		FLOAT,
		INT,
		NONE
	};

	Value() : mType(NONE), mIntValue(0) {}

	Value(int value) : mType(INT), mIntValue(value) {
	}
	Value(float value) : mType(FLOAT), mFloatValue(value) {
	}



	Type mType;
	union {
		int mIntValue;
		float mFloatValue;
	};
};

struct RpnNode {
	enum Type {
		NUMBER,
		ADD,
		SUB,
		MUL,
		DIV,
		VAR,
		NONE
	};

	RpnNode() : mType(NONE), mValue((int)0) {}
	RpnNode(const Value& value) : mType(NUMBER), mValue(value) {}
	RpnNode(Type type) : mType(type), mValue((int)0) {}

	Type mType;
	Value mValue;
};



class Expression {
public:
	void parse(const char *expression) {
		Stack<RpnNode> operatorStack;


		char temp[128];
		const char *exp = expression;
		RpnNode tempNode;
		outputQueue.clear();
		while(*exp) {
			int len;
			while(*exp && *exp==' ') exp++;
			if((*exp) == 0) break;

			if(len=isNum(tempNode, exp)) {
				exp+=len;
				outputQueue.put(tempNode);
			} else
			if(len=isOp(tempNode, exp)) {
				exp+=len;
				while(operatorStack.size()) {
					const RpnNode& cmp = operatorStack.peek();
					int p1 = getPrecedence(tempNode);
					int p2 = getPrecedence(cmp);
					if(p1<=p2) {
						outputQueue.put(cmp);
						operatorStack.pop();
					} else {
						break;
					}
				}
				operatorStack.push(tempNode);
			} else {
				maPanic(1, "Invalid token!");
			}
		}

		while(operatorStack.size()) {
			const RpnNode& op = operatorStack.peek();
			outputQueue.put(op);
			operatorStack.pop();
		}

		for(int i = 0; i < outputQueue.mQ.size(); i++) {
			switch(outputQueue.mQ[i].mType) {
				case RpnNode::NUMBER: lprintfln("%d", outputQueue.mQ[i].mValue.mIntValue); break;
				case RpnNode::VAR: lprintfln("x"); break;
				case RpnNode::ADD: lprintfln("+"); break;
				case RpnNode::SUB: lprintfln("-"); break;
				case RpnNode::MUL: lprintfln("*"); break;
				case RpnNode::DIV: lprintfln("/"); break;
			}
		}

	}

	void setVariable(Value value) {
		mVariableValue = value;
	}

	Value evaluate() {
		Stack<Value> stack;

		for(int i = 0; i < outputQueue.mQ.size(); i++) {
			switch(outputQueue.mQ[i].mType) {
			case RpnNode::NUMBER:
				stack.push(outputQueue.mQ[i].mValue);
				break;
			case RpnNode::VAR:
				stack.push(mVariableValue);
				break;
			case RpnNode::ADD:
				{
				Value v2 = stack.peek(); stack.pop();
				Value v1 = stack.peek(); stack.pop();
				stack.push(Value(v1.mIntValue + v2.mIntValue));
				}
				break;
			case RpnNode::SUB:
				{
				Value v2 = stack.peek(); stack.pop();
				Value v1 = stack.peek(); stack.pop();
				stack.push(Value(v1.mIntValue - v2.mIntValue));
				}
				break;
			case RpnNode::MUL:
				{
				Value v2 = stack.peek(); stack.pop();
				Value v1 = stack.peek(); stack.pop();
				stack.push(Value(v1.mIntValue * v2.mIntValue));
				}
				break;
			case RpnNode::DIV:
				{
				Value v2 = stack.peek(); stack.pop();
				Value v1 = stack.peek(); stack.pop();
				stack.push(Value(v1.mIntValue / v2.mIntValue));
				}
				break;
			}
		}

		return stack.peek();
	}

private:

#ifdef _WIN32
	int isdigit(char c) {
		return (c>='0')&&(c<='9');
	}
#endif

	int getPrecedence(const RpnNode& node) {
		switch(node.mType) {
		case RpnNode::ADD:
		case RpnNode::SUB: return 1;
		case RpnNode::MUL:
		case RpnNode::DIV: return 2;
		default: maPanic(1, "Precedence not applicable!!");
		}
		return -1;
	}

	int isNum(RpnNode& output, const char *str) {
		const char *s = str;
		int length = 0;
		if(*str && *str=='x') {
			output = RpnNode(RpnNode::VAR);
			return 1;
		}
		while(*str) {
			if(!isdigit(*str)) {
				break;
			}
			str++;
			length++;
		}
		if(length==0) return 0;
		output = RpnNode(Value(atoi(s)));
		return length;
	}

	int isOp(RpnNode &output, const char *str) {
		RpnNode::Type type;
		switch(*str) {
		case '+': type = RpnNode::ADD; break;
		case '-': type = RpnNode::SUB; break;
		case '*': type = RpnNode::MUL; break;
		case '/': type = RpnNode::DIV; break;
		default: return 0;
		}
		output = RpnNode(type);
		return 1;
	}

	Value mVariableValue;
	Queue<RpnNode> outputQueue;
};

class GraphWidget : public Widget {
public:
	GraphWidget(int x=0, int y=0, int w=0, int h=0) : Widget(x, y, w, h),
	mAmplitude(1.0), mFrequency(1.0) {

	}

	void setExpression(Expression *exp) {
		mExp = exp;
	}

	void setRange(int startX, int endX, int startY, int endY) {
		mStartX = startX;
		mEndX = endX;
		mStartY = startY;
		mEndY = endY;
		requestRepaint();
	}

	void setAmplitude(double a) {
		mAmplitude = a;
		requestRepaint();
	}

	void setFrequency(double f) {
		mFrequency = f;
		requestRepaint();
	}

	void setPhase(double p) {
		mPhase = p;
		requestRepaint();
	}

	void drawWidget() {
		//if(!mExp) return;
		//maSetColor(0);
		//Gfx_fillRect(0, 0, mBounds.width, mBounds.height);
		maSetColor(0xffffff);
		/*
		int x = mStartX;
		int xDelta = ((mEndX-mStartX)<<16)/mBounds.width;
		for(int i = 0; i < mBounds.width; i++) {
			mExp->setVariable(Value(x>>16));
			int value = mBounds.height-mExp->evaluate().mIntValue;
			int y = value*mBounds.height/(mEndY-mStartY);
			Gfx_plot(x>>16, y);
			x+=xDelta;
		}*/

		double ang = mPhase;
		double angDelta = (2.0*3.14159)/(double)mPaddedBounds.width;
		double height2 = (double)mPaddedBounds.height/2.0;
		for(int i = 0; i<  mPaddedBounds.width; i++) {
			double sinValue = sin(ang*mFrequency)*height2*mAmplitude;
			Gfx_plot(i, (int)(height2-sinValue));
			ang+=angDelta;
		}
	}

	bool isTransparent() const {
		return false;
	}

private:
	double mAmplitude, mFrequency;
	double mPhase;
	int mStartX, mEndX, mStartY, mEndY;
	Expression *mExp;
};

static Screen *sGraphScreen;
static Screen *sCalculatorScreen;


class GraphScreen : public Screen, public ButtonListener, public SliderListener {
public:
	void onValueChange(Slider* slider, double value) {
		if(slider == mFrequencySlider) {
			mGraphWidget->setFrequency(value);
		}
		else if(slider == mAmplitudeSlider) {
			mGraphWidget->setAmplitude(value);
		}
		else if(slider == mPhaseSlider) {
			mGraphWidget->setPhase(value);
		}
	}

	void onButtonEvent(Button* b, bool pressed) {
			const String& str = b->getCaption();
			if(pressed==true && str == "calculator") {
				sCalculatorScreen->show();
			}
	}

	GraphScreen() {
		StackLayout *mainLayout = new StackLayout(0, 0, 240, 320);

		mGraphWidget = new GraphWidget(0, 0, 0, 100);
		mainLayout->add(mGraphWidget);

		Slider *slider;
		mFrequencySlider = new Slider(0, 0, 0, mScreenHeight/6, Slider::HORIZONTAL, 0.0, 8, 1.0);
		mFrequencySlider->addSliderListener(this);
		mAmplitudeSlider = new Slider(0, 0, 0, mScreenHeight/6, Slider::HORIZONTAL, -1, 1, 1.0);
		mAmplitudeSlider->addSliderListener(this);
		mPhaseSlider = new Slider(0, 0, 0, mScreenHeight/6, Slider::HORIZONTAL, 0.0, 2*3.14159, 0.0);
		mPhaseSlider->addSliderListener(this);
		mPhaseSlider->setMoveToPoint(true);
		mainLayout->add(mFrequencySlider);
		mainLayout->add(mAmplitudeSlider);
		mainLayout->add(mPhaseSlider);

		Button *button = new Button(0, 0, 50, mScreenHeight/6, "calculator");
		button->addButtonListener(this);
		mainLayout->add(button);
		setMain(mainLayout);

	}

private:
	GraphWidget* mGraphWidget;
	Slider *mFrequencySlider;
	Slider *mAmplitudeSlider;
	Slider *mPhaseSlider;
};

class CalculatorScreen : public Screen, public ButtonListener {
public:
	Label *mainLabel;

	void onButtonEvent(Button* b, bool pressed) {
		if(pressed) return;
		String exp = mainLabel->getCaption();
		const String& str = b->getCaption();
		if(str == "exit") {
			maExit(0);
		} else if(str == "graph") {
			sGraphScreen->show();
		} else if(str=="C") {
			mainLabel->setCaption("");
		} else if(str=="=") {
			if(exp=="") return;
			mExp->parse(exp.c_str());
			Value v = mExp->evaluate();
			char temp[128];
			sprintf(temp, "%d", v.mIntValue);
			mainLabel->setCaption(temp);
			return;
		} else {
			exp+=str;
			mainLabel->setCaption(exp);
		}
	}

	CalculatorScreen() {
			ListBox *mainListbox = new ListBox(0, 0, 240, 320, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true);
			mainListbox->setAutoSize(true);

			mainLabel = new Label(0, 0, 50, 80, "");
			mainLabel->setVerticalAlignment(Label::VA_CENTER);

			mainListbox->add(mainLabel);

			GridLayout *calculatorPane = new GridLayout(0, 0, 240, 240, 4, 4);
			calculatorPane->setAutoSizeX(true);
			calculatorPane->setAutoSizeY(true);

			mainListbox->add(calculatorPane);

			const char* buttons1[] = {
					"1","2","3","+",
					"4","5","6","-",
					"7","8","9","*",
					"C","0","=","/"
			};

			Button *button;
			for(int i = 0; i < 16; i++) {
				button = new Button(0, 0, 50, 50, buttons1[i]);
				button->addButtonListener(this);
				calculatorPane->add(button);
			}

			for(int i = 0; i < 2; i++) {
				button = new Button(0, 0, 50, 100, "graph");
				button->addButtonListener(this);
				mainListbox->add(button);
			}

			ListBox *testListbox = new ListBox(0, 0, 240, 320, ListBox::LBO_HORIZONTAL, ListBox::LBA_LINEAR, true);
			testListbox->setAutoSize(true);
			mainListbox->add(testListbox);
			for(int i = 0; i < 16; i++) {
				button = new Button(0, 0, 100, 50, "graph");
				button->addButtonListener(this);
				testListbox->add(button);
			}

			ListBox *testListbox2 = new ListBox(0, 0, 240, 320, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true);
			testListbox2->setAutoSize(true);
			testListbox->add(testListbox2);
			for(int i = 0; i < 16; i++) {
				button = new Button(0, 0, 100, 50, "graph2");
				button->addButtonListener(this);
				testListbox2->add(button);
			}

			NativeEditBox *native = new NativeEditBox(0, 0, 50, 50);
			mainListbox->add(native);

			button = new Button(0, 0, 50, 100, "exit");
			button->addButtonListener(this);
			mainListbox->add(button);


			mExp = new Expression();

			/*
			GraphWidget *graph = new GraphWidget(0, 0, 0, 120, mainListbox);
			mExp->parse("2*x/3");
			graph->setExpression(mExp);
			graph->setRange(-100, 100, -100, 100);
			*/

			setMain(mainListbox);
	}
	
	~CalculatorScreen() {
	}
private:
	Expression *mExp;
};

class MenuScreen : public Screen {
public:
	MenuScreen() {
	}
	~MenuScreen() {
	}
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {

		gConsoleForceDisplay = 0;
		gConsoleDrawBackground = 0;

		Engine::getSingleton().setDisplayConsole(true);

		//SkinProperty* selectedWidgetSkin = new SkinProperty(RES_SELECTED2, 16, 32, 16, 32, true);
		//SkinProperty* unselectedWidgetSkin = new SkinProperty(RES_UNSELECTED2, 16, 32, 16, 32, true);
		//SkinProperty* focusedUnselectedWidgetSkin = new SkinProperty(RES_FOCUSED_UNSELECTED2, 16, 32, 16, 32, true);

			FontProperty* font = new FontProperty(RES_FONT);

		Style* widgetStyle = new Style(0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL);
		LabelStyle* labelStyle = new LabelStyle(font, 10,10,10,10,0,0,0,0, NULL, NULL);
		//ButtonStyle* buttonStyle = new ButtonStyle(selectedWidgetSkin, focusedUnselectedWidgetSkin, unselectedWidgetSkin, font);
	
		SkinProperty* sliderAmountSkin = new SkinProperty(RES_SLIDER_AMT, 10, 12, 0, 18, true);
		SkinProperty* sliderBackgroundSkin = new SkinProperty(RES_SLIDER_BKG, 10, 12, 0, 18, true);
		ImageProperty* sliderGripImage = new ImageProperty(RES_SLIDER_GRIP);
		SliderStyle* sliderStyle = new SliderStyle(sliderAmountSkin, sliderBackgroundSkin, sliderGripImage);
		
		Engine::getSingleton().setDefaultStyle("Widget", widgetStyle);
		Engine::getSingleton().setDefaultStyle("Label", labelStyle);
		//Engine::getSingleton().setDefaultStyle("Button", buttonStyle);
		Engine::getSingleton().setDefaultStyle("Slider", sliderStyle);

		// For some strange reason this is now broken??.
		//WidgetSkin::setCacheEnabled(true);

		sCalculatorScreen = new CalculatorScreen();
		sGraphScreen = new GraphScreen();
		sCalculatorScreen->show();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}


	~MAUIMoblet() {
		delete sCalculatorScreen;
		delete sGraphScreen;
	}
	
};

extern "C" int MAMain() {
	Moblet::run(new MAUIMoblet());
	return 0;
};
