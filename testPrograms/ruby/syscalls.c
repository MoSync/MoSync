#include "src/ruby.h"
#include <maassert.h>

VALUE MoSync = Qnil;
VALUE MAStd = Qnil;
VALUE MAEvent = Qnil;
VALUE MAExtent = Qnil;

VALUE method_MAStdFreeze(VALUE self) {
	FREEZE;
}

VALUE method_maWait(VALUE self, VALUE milliSecs) {
	int x = NUM2INT(milliSecs);
	maWait(x);
	return Qnil;
}

VALUE method_maGetMilliSecondCount(VALUE self) {
	return INT2NUM(maGetMilliSecondCount());
}

VALUE method_maSetColor(VALUE self, VALUE color) {
	//printf("maSetColor");
	int x = NUM2INT(color);
	//printf("worked");
	maSetColor(x);
	return Qnil;
}

VALUE method_maFillRect(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
	//printf("maSetColor");
	int a = NUM2INT(x);
	int b = NUM2INT(y);	
	int c = NUM2INT(w);
	int d = NUM2INT(h);	
	//printf("worked");
	maFillRect(a,b,c,d);
	return Qnil;
}

VALUE method_maPlot(VALUE self, VALUE x, VALUE y) {
	//printf("maSetColor");
	int a = NUM2INT(x);
	int b = NUM2INT(y);
	//printf("worked");
	maPlot(a, b);
	return Qnil;
}


VALUE method_maExit(VALUE self, VALUE num) {
	maExit(NUM2INT(num));
}

VALUE method_MAEvent_type(VALUE self) {
	EVENT *e;
	Data_Get_Struct(self, EVENT, e);
	return INT2NUM(e->type);
}

VALUE method_MAEvent_key(VALUE self) {
	EVENT *e;
	Data_Get_Struct(self, EVENT, e);
	return INT2NUM(e->key);
}

VALUE method_maGetEvent(VALUE self) {
	//printf("maSetColor");
	//printf("worked");
	EVENT *e;
	VALUE info;
	
	e = ALLOC(EVENT);
	int res = maGetEvent(e);
	
	if(res == 0) {
		return Qnil;
	}
		
	info = Data_Wrap_Struct(MAEvent, 0, free, e);
	//rb_iv_set(info, "@_type", INT2NUM(e->type));
	//rb_iv_set(info, "@key", INT2NUM(e->key));

	return info;
}

VALUE method_maUpdateScreen(VALUE self) {
	maUpdateScreen();
	return Qnil;
}

typedef struct {
	int w, h;
} MAExtentStruct;

VALUE method_MAExtent_width(VALUE self) {
//	return rb_iv_get(self, "@width");
	MAExtentStruct *e;
	Data_Get_Struct(self, MAExtentStruct, e);
	return INT2NUM(e->w);
}

VALUE method_MAExtent_height(VALUE self) {
//	return rb_iv_get(self, "@height");
	MAExtentStruct *e;
	Data_Get_Struct(self, MAExtentStruct, e);
	return INT2NUM(e->h);
}

VALUE method_maGetScrSize(VALUE self) {
//	VALUE obj = rb_class_new_instance(0, 0, MAExtent);
//	Extent e = maGetScrSize();
//	rb_iv_set(obj, "@width", INT2NUM(EXTENT_X(e)));
//	rb_iv_set(obj, "@height", INT2NUM(EXTENT_Y(e)));
	MAExtentStruct *e = ALLOC(MAExtentStruct);
	Extent ext = maGetScrSize();
	e->w = EXTENT_X(ext);
	e->h = EXTENT_Y(ext);
	VALUE obj = Data_Wrap_Struct(MAExtent, 0, free, e);
	return obj;
}

VALUE method_maLine(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2) {
	//printf("maLine");
	int a = NUM2INT(x1);
	int b = NUM2INT(y1);
	int c = NUM2INT(x2);
	int d = NUM2INT(y2);
	maLine(a, b, c, d);
	return Qnil;
}

void Init_MAEvent() {
	MAExtent = rb_define_class_under(MoSync, "Extent", rb_cObject);

	MAEvent = rb_define_class_under(MoSync, "Event", rb_cObject);
	
	/*
		rb_define_const(VALUE class, const char* name, VALUE value)
	*/
	rb_define_const(MoSync, "EVENT_TYPE_KEY_PRESSED", INT2NUM(EVENT_TYPE_KEY_PRESSED));
	rb_define_const(MoSync, "EVENT_TYPE_KEY_RELEASED", INT2NUM(EVENT_TYPE_KEY_RELEASED));
	rb_define_const(MoSync, "EVENT_TYPE_CLOSE", INT2NUM(EVENT_TYPE_CLOSE));
	
	rb_define_const(MoSync, "MAK_LEFT", INT2NUM(MAK_LEFT));
	rb_define_const(MoSync, "MAK_UP", INT2NUM(MAK_UP));
	rb_define_const(MoSync, "MAK_RIGHT", INT2NUM(MAK_RIGHT));
	rb_define_const(MoSync, "MAK_DOWN", INT2NUM(MAK_DOWN));
	rb_define_const(MoSync, "MAK_FIRE", INT2NUM(MAK_FIRE));
	rb_define_const(MoSync, "MAK_SOFTLEFT", INT2NUM(MAK_SOFTLEFT));
	rb_define_const(MoSync, "MAK_SOFTRIGHT", INT2NUM(MAK_SOFTRIGHT));
	
	rb_define_method(MAEvent, "key?", method_MAEvent_key, 0);	
	rb_define_method(MAEvent, "type?", method_MAEvent_type, 0);	
}

void Init_MoSync() {
	MoSync = rb_define_module("MoSync");
	rb_define_method(MoSync, "SetColor", method_maSetColor, 1);
	rb_define_method(MoSync, "Line", method_maLine, 4);
	rb_define_method(MoSync, "UpdateScreen", method_maUpdateScreen, 0);
	rb_define_method(MoSync, "FillRect", method_maFillRect, 4);	
	rb_define_method(MoSync, "Plot", method_maPlot, 2);	
	rb_define_method(MoSync, "Exit", method_maExit, 1);	
	rb_define_method(MoSync, "GetEvent", method_maGetEvent, 0);		
	rb_define_method(MoSync, "Wait", method_maWait, 1);
	rb_define_method(MoSync, "GetScrSize", method_maGetScrSize, 0);
	rb_define_method(MoSync, "GetMilliSecondCount", method_maGetMilliSecondCount, 0);
	
	MAStd = rb_define_module("MAStd");
	rb_define_method(MAStd, "Freeze", method_MAStdFreeze, 0);
	
	MAExtent = rb_define_class_under(MoSync, "Extent", rb_cObject);
	rb_define_method(MAExtent, "width", method_MAExtent_width, 0);
	rb_define_method(MAExtent, "height", method_MAExtent_height, 0);
		
	
	Init_MAEvent();
}
