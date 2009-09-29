/* Copyright (C) 2009 Mobile Sorcery AB

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

#include <config_platform.h>
#include "ImageLoad.h"

#include <Imaging.h>
#include <initguid.h>
#include <imgguids.h>

#include <windows.h>
#include <MemStream.h>

#include "wce_errors.h"
using namespace MoSyncError;

namespace Base {
	extern Image* backBuffer;
	extern uint redMask;
	extern uint redShift;
	extern uint redBits;
	extern uint greenMask;
	extern uint greenShift;
	extern uint greenBits;
	extern uint blueMask;
	extern uint blueShift;
	extern uint blueBits;
	extern Image::PixelFormat pixelFormat;
}

using namespace Base;

#if 0	//unused

#ifndef DecompressImageInfo

class IImageRender;
typedef void (CALLBACK *PROGRESSFUNC)(IImageRender *pRender, BOOL bComplete, LPARAM lParam);
typedef DWORD (CALLBACK *GETDATAFUNC)(LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam);


typedef struct tagDecompressImageInfo {
   DWORD dwSize;                                 
   LPBYTE pbBuffer;
   DWORD dwBufferMax;
   DWORD dwBufferCurrent;
   HBITMAP* phBM;                                   
   IImageRender **ppImageRender;                 
   int iBitDepth;     
   LPARAM lParam;                 
   HDC hdc;           
   int iScale;         
   int iMaxWidth;     
   int iMaxHeight;     
   GETDATAFUNC pfnGetData;       
   PROGRESSFUNC pfnImageProgress;         
   COLORREF crTransparentOverride;
} DecompressImageInfo;

#endif 

struct MemInfo {
	const void *data;
	int len;
	int pos;
};

static DWORD CALLBACK GetImageData(LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam)
{
	MemInfo* memInfo = (MemInfo*)lParam;
	size_t c = dwBufferMax;
	int bytesToRead;

	bytesToRead = MIN(memInfo->len-memInfo->pos, (int)c);

	if(bytesToRead == 0) { 
		return 0;
	}

	memcpy(szBuffer, memInfo->data, bytesToRead);

	memInfo->pos += bytesToRead;

	return bytesToRead;
}

Image* loadImageFromStreamWithImgdecmp(const void* data, int len) {
	HBITMAP retBitmap;

	const int bufferSize = 4096;
	BYTE buffer[bufferSize];

	DecompressImageInfo decInfo;
	decInfo.dwSize = sizeof(DecompressImageInfo);
	decInfo.dwBufferCurrent = 0;
	
	decInfo.pbBuffer = buffer;
	decInfo.dwBufferMax = bufferSize;
	
	decInfo.phBM = &retBitmap;
	decInfo.ppImageRender = NULL;
  
	HDC dc = GetDC(NULL);
	decInfo.iBitDepth = GetDeviceCaps(dc, BITSPIXEL);
	ReleaseDC(NULL, dc);

	decInfo.hdc = 0;                              
	decInfo.iScale = 100;                              
	decInfo.iMaxWidth = 10000;                        
	decInfo.iMaxHeight = 10000;                        
	decInfo.pfnGetData = GetImageData;            
	decInfo.pfnImageProgress = 0;            
	decInfo.crTransparentOverride = (UINT) -1;   

	MemInfo memInfo = {data, len, 0};
	decInfo.lParam = LPARAM((MemInfo*)&memInfo);

	HINSTANCE hDll = LoadLibrary(TEXT("imgdecmp.dll"));
	if (!hDll) {
		LOG("Could not load imgdecmp.dll for image loading\n");
		return 0;
	}
	typedef HRESULT (*DecompressImageIndirect_t)(DecompressImageInfo *pParams);
	DecompressImageIndirect_t proc = (DecompressImageIndirect_t)GetProcAddress(hDll, TEXT("DecompressImageIndirect"));
	if (! proc) {
		LOG("Could not find DecompressImageIndirect proc in dll\n");
		FreeLibrary(hDll);
		return NULL;
	}
	HRESULT hr = proc(&decInfo);

	FreeLibrary(hDll);

	if (FAILED(hr)) {
		LOG("DecompressImageIndirect failed: 0x%x\n", hr);
		return NULL;
	} else {
		//Image* image = new Image(
		return NULL;//image;
	}

	return NULL;
}
#endif	//0

HRESULT loadImageFromStreamWithImagingApi(Image** image, const void* data, int len) {
	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;
	//HBITMAP hResult = 0;
	HRESULT hresult;
	
	if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IImagingFactory,
		(void **)&pImgFactory)))
	{

		ImageInfo imageInfo;
		if (SUCCEEDED(hresult=pImgFactory->CreateImageFromBuffer(data, len, BufferDisposalFlagNone,&pImage))
			&& SUCCEEDED(pImage->GetImageInfo(&imageInfo)))
		{
			/*
			HDC dc = GetDC(NULL);
			HDC dcBitmap = CreateCompatibleDC(dc);
			hResult = CreateCompatibleBitmap(dc, imageInfo.Width, imageInfo.Height);
			if (hResult) {
			HGDIOBJ hOldBitmap = SelectObject(dcBitmap, hResult);
			RECT r = {0, 0, imageInfo.Width, imageInfo.Height};
			pImage->Draw(dcBitmap, &r, NULL);
			SelectObject(dcBitmap, hOldBitmap);

			}
			*/

			PixelFormatID fmt;
			switch(pixelFormat) {
				case Image::PIXELFORMAT_RGB555: fmt = PixelFormat16bppRGB555; break;
				case Image::PIXELFORMAT_RGB565: fmt = PixelFormat16bppRGB565; break;
				case Image::PIXELFORMAT_RGB888: fmt = PixelFormat24bppRGB; break;
			}
			IBitmapImage *imageComponent;
			IBitmapImage *alphaComponent;
			if(SUCCEEDED(hresult=pImgFactory->CreateBitmapFromImage(pImage, 
				0, 0, 
				fmt, InterpolationHintDefault, &imageComponent))) {

					BitmapData imageData;
					BitmapData alphaData;
					memset(&imageData, 0, sizeof(BitmapData));
					memset(&alphaData, 0, sizeof(BitmapData));
					unsigned char *pixels = 0;
					unsigned char *alpha = 0;

					RECT r = {0, 0, imageInfo.Width, imageInfo.Height};
					if(SUCCEEDED(imageComponent->LockBits(&r, ImageLockModeRead, fmt, &imageData))) {
						pixels = new unsigned char[imageData.Stride*imageInfo.Height];
						memcpy(pixels, imageData.Scan0, imageData.Stride*imageInfo.Height);
						imageComponent->UnlockBits(&imageData);

						bool alphaSucceeded = false;
						if(imageInfo.Flags&ImageFlagsHasAlpha) {
							if(SUCCEEDED(pImgFactory->CreateBitmapFromImage(pImage, 
								0, 0, 
								PixelFormat32bppARGB, InterpolationHintDefault, &alphaComponent))) {

									if(SUCCEEDED(alphaComponent->LockBits(&r, ImageLockModeRead, PixelFormat32bppARGB, &alphaData))) {
										alpha = new unsigned char[imageInfo.Width*imageInfo.Height];
										unsigned char *dst = alpha;
										unsigned char *src = (unsigned char*)alphaData.Scan0;
										for(unsigned int y = 0; y < imageInfo.Height; y++) {
											unsigned int *srcScan = (unsigned int*) src;
											for(unsigned int x = 0; x < imageInfo.Width; x++) {
												*dst++ = ((*srcScan++)&0xff000000)>>24;
											}
											src+=alphaData.Stride;
										}
										alphaComponent->UnlockBits(&alphaData);						
									}

									alphaComponent->Release();
							}
						}


						*image = new Image(
							pixels,
							alpha, 
							imageInfo.Width,
							imageInfo.Height,
							imageData.Stride,
							pixelFormat, 
							false,
							true);

						if(!*image) hresult = E_OUTOFMEMORY;

					}

					imageComponent->Release();
			}


			pImage->Release();
			//		DeleteDC(dcBitmap);
			//		ReleaseDC(NULL, dc);
		}
		pImgFactory->Release();
	}


	/*

	DIBSECTION info;
	GetObject( hResult, sizeof(DIBSECTION), &info );

	HDC dc = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(dc);
	ReleaseDC(NULL, dc);
	HGDIOBJ oldBitmap;
	oldBitmap = SelectObject(hdc, hResult);

	unsigned int bytesPerPixel = backBuffer->bytesPerPixel;
	unsigned char *pixels = new unsigned char[info.dsBm.bmWidth*info.dsBm.bmHeight*bytesPerPixel];
	unsigned char *dst = pixels;

	for(int y = 0; y < info.dsBm.bmHeight; y++) {
		for(int x = 0; x < info.dsBm.bmWidth; x++) {
			COLORREF c = GetPixel(hdc, x, y);
			switch(bytesPerPixel) {
				case 1:
					*((unsigned char*)dst) = c&0xff;						
					break;
				case 2:
					*((unsigned short*)dst) = c&0xffff; 
					break;
				case 3:
				case 4:
					*((unsigned int*)dst) = c;
					break;
			}
			dst+=bytesPerPixel;
		}
	}

	Image *image = new Image(
		pixels, 
		0, 
		info.dsBm.bmWidth, 
		info.dsBm.bmHeight, 
		info.dsBm.bmWidth*backBuffer->bytesPerPixel,
		pixelFormat, 
		false);

	DeleteObject(hResult);
	DeleteDC(hdc);
*/
	return hresult;
}

unsigned int calcShift(int a) {
	unsigned int s = 0;
	while(!(a&0x1)) {s++; a>>=1;}
	return s;
}

unsigned int calcBits(int a) {
	unsigned int s = 0;
	while(!(a&0x1)) {a>>=1;}
	while(a) {s++; a>>=1;}
	return s;
}

HRESULT loadImageFromStream(Image** image, const void* data, int len) {
	if(!len) return NULL;
	OSVERSIONINFO vi;
	memset(&vi, 0, sizeof(vi));
	vi.dwOSVersionInfoSize = sizeof(vi);
	VERIFY(GetVersionEx(&vi));
	if (vi.dwMajorVersion>=4) {
		return loadImageFromStreamWithImagingApi(image, data, len);
	} else {
		//return loadImageFromStreamWithImgdecmp(data, len);
		BIG_PHAT_ERROR(WCEERR_UNSUPPORTED_OS_VERSION);
	}
}
