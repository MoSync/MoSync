#include "config_platform.h"
#include "Syscall.h"

void MoSyncErrorExit(int errorCode)
{

}

SYSCALL(int,  maSetColor(int rgb))
{
	return -1;
}

SYSCALL(void,  maSetClipRect(int left, int top, int width, int height))
{

}

SYSCALL(void,  maGetClipRect(MARect* out))
{

}

SYSCALL(void,  maPlot(int posX, int posY))
{

}

SYSCALL(void,  maLine(int startX, int startY, int endX, int endY))
{

}

SYSCALL(void,  maFillRect(int left, int top, int width, int height))
{

}

SYSCALL(void,  maFillTriangleStrip(const MAPoint2d* points, int count))
{

}

SYSCALL(void,  maFillTriangleFan(const MAPoint2d* points, int count))
{

}

SYSCALL(MAExtent,  maGetTextSize(const char* str))
{
	return -1;
}

SYSCALL(MAExtent,  maGetTextSizeW(const wchar* str))
{
	return -1;
}

SYSCALL(void,  maDrawText(int left, int top, const char* str))
{

}

SYSCALL(void,  maDrawTextW(int left, int top, const wchar* str))
{

}

SYSCALL(void,  maUpdateScreen(void))
{

}

SYSCALL(void,  maResetBacklight(void))
{

}

SYSCALL(MAExtent,  maGetScrSize(void))
{
	return -1;
}

SYSCALL(void,  maDrawImage(MAHandle image, int left, int top))
{

}

SYSCALL(void,  maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect, int scanlength))
{

}

SYSCALL(void,  maDrawImageRegion(MAHandle image, const MARect* srcRect, const MAPoint2d* dstPoint, int transformMode))
{

}

SYSCALL(MAExtent,  maGetImageSize(MAHandle image))
{
	return -1;
}

SYSCALL(void,  maGetImageData(MAHandle image, void* dst, const MARect* srcRect, int scanlength))
{

}

SYSCALL(MAHandle,  maSetDrawTarget(MAHandle image))
{
	return -1;
}

SYSCALL(int,  maFindLabel(const char* name))
{
	return -1;
}

SYSCALL(int,  maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size))
{
	return -1;
}

SYSCALL(int,  maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int alpha))
{
	return -1;
}

SYSCALL(int,  maCreateDrawableImage(MAHandle placeholder, int width, int height))
{
	return -1;
}

SYSCALL(int,  maCreateData(MAHandle placeholder, int size))
{
	return -1;
}

SYSCALL(MAHandle,  maCreatePlaceholder(void))
{
	return -1;
}

SYSCALL(void,  maDestroyObject(MAHandle handle))
{

}

SYSCALL(int,  maGetDataSize(MAHandle data))
{
	return -1;
}

SYSCALL(void,  maReadData(MAHandle data, void* dst, int offset, int size))
{

}

SYSCALL(void,  maWriteData(MAHandle data, const void* src, int offset, int size))
{

}

SYSCALL(void,  maCopyData(const MACopyData* params))
{

}

SYSCALL(MAHandle,  maOpenStore(const char* name, int flags))
{
	return -1;
}

SYSCALL(int,  maWriteStore(MAHandle store, MAHandle data))
{
	return -1;
}

SYSCALL(int,  maReadStore(MAHandle store, MAHandle placeholder))
{
	return -1;
}

SYSCALL(void,  maCloseStore(MAHandle store, int remove))
{

}

SYSCALL(MAHandle,  maConnect(const char* url))
{
	return -1;
}

SYSCALL(void,  maConnClose(MAHandle conn))
{

}

SYSCALL(void,  maConnRead(MAHandle conn, void* dst, int size))
{

}

SYSCALL(void,  maConnWrite(MAHandle conn, const void* src, int size))
{

}

SYSCALL(void,  maConnReadToData(MAHandle conn, MAHandle data, int offset, int size))
{

}

SYSCALL(void,  maConnWriteFromData(MAHandle conn, MAHandle data, int offset, int size))
{

}

SYSCALL(int,  maConnGetAddr(MAHandle conn, MAConnAddr* addr))
{
	return -1;
}

SYSCALL(MAHandle,  maHttpCreate(const char* url, int method))
{
	return -1;
}

SYSCALL(void,  maHttpSetRequestHeader(MAHandle conn, const char* key, const char* value))
{

}

SYSCALL(int,  maHttpGetResponseHeader(MAHandle conn, const char* key, char* buffer, int bufSize))
{
	return -1;
}

SYSCALL(void,  maHttpFinish(MAHandle conn))
{

}

SYSCALL(int,  maLoadResources(MAHandle data))
{
	return -1;
}

SYSCALL(void,  maLoadProgram(MAHandle data, int reload))
{

}

SYSCALL(int,  maGetKeys(void))
{
	return -1;
}

SYSCALL(int,  maGetEvent(MAEvent* event))
{
	return -1;
}

SYSCALL(void,  maWait(int timeout))
{

}

SYSCALL(int,  maTime(void))
{
	return -1;
}

SYSCALL(int,  maLocalTime(void))
{
	return -1;
}

SYSCALL(int,  maGetMilliSecondCount(void))
{
	return -1;
}

SYSCALL(int,  maFreeObjectMemory(void))
{
	return -1;
}

SYSCALL(int,  maTotalObjectMemory(void))
{
	return -1;
}

SYSCALL(int,  maVibrate(int ms))
{
	return -1;
}

SYSCALL(void, maExit(int result))
{

}

SYSCALL(void, maPanic(int result, const char* message))
{

}

SYSCALL(int,  maSoundPlay(MAHandle sound_res, int offset, int size))
{
	return -1;
}

SYSCALL(void,  maSoundStop(void))
{

}

SYSCALL(int,  maSoundIsPlaying(void))
{
	return -1;
}

SYSCALL(int,  maSoundGetVolume(void))
{
	return -1;
}

SYSCALL(void,  maSoundSetVolume(int vol))
{

}

SYSCALL(int,  maInvokeExtension(int function, int a, int b, int c))
{
	return -1;
}

SYSCALL(int,  maIOCtl(int function, int a, int b, int c))
{
	return -1;
}
