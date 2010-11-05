// PCX Loader - Codehead 06/11/04
// Modified by: Niklas Nummelin & Patrick Broman

//#include <iostream>
//#include <fstream>
//#include <memory.h>
#include <ma.h>

#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT_ID  0x8
#define IMG_ERR_BAD_FORMAT_BPP  0x10
#define IMG_ERR_BAD_FORMAT_DIM  0x20
#define IMG_ERR_BAD_FORMAT_PAL  0x40

#define IMG_ERR_UNSUPPORTED 0x80

class PCXImg
 {
  public:
   PCXImg();
   ~PCXImg();
   int Load(unsigned char *data, int sizeOfData);
   int GetBPP();
   int GetWidth();
   int GetHeight();
   unsigned char* GetImg();       // Return a pointer to image data
   unsigned char* GetPalette();   // Return a pointer to VGA palette

  private:
   short int iWidth,iHeight,iBPP,iPlanes,iBPL;
   long lImageSize;
   char bEnc;
   unsigned char *pImage, *pPalette, *pData;

   // Internal workers
   int ReadHeader();
   int LoadRLEData();
   int LoadPalette(unsigned long ulDataSize);
 };
