#include "PCXLoader.h"

PCXImg::PCXImg()
 {
  pImage=pPalette=pData=NULL;
  iWidth=iHeight=iBPP=iPlanes=iBPL=bEnc=0;
 }


PCXImg::~PCXImg()
 {
  if(pImage)
   {
    delete [] pImage;
    pImage=NULL;
   }

  if(pPalette)
   {
    delete [] pPalette;
    pPalette=NULL;
   }

  if(pData)
   {
    delete [] pData;
    pData=NULL;
   }
 }


int PCXImg::Load(unsigned char *data, int sizeOfData)
{
  //using namespace std;
  //ifstream fIn;
  unsigned long ulSize = sizeOfData;
  int iRet;

  // Clear out any existing image and palette
   if(pImage)
    {
     delete [] pImage;
     pImage=NULL;
    }

   if(pPalette)
    {
     delete [] pPalette;
     pPalette=NULL;
    }

   /*
  // Open the specified file
  fIn.open(szFilename,ios::binary);

   if(fIn==NULL)
    return IMG_ERR_NO_FILE;

  // Get file size
  fIn.seekg(0,ios_base::end);
  ulSize=fIn.tellg();
  fIn.seekg(0,ios_base::beg);
*/

  // Allocate some space
  // Check and clear pDat, just in case
   if(pData)
    delete [] pData;

  //pData=new unsigned char[ulSize];

   pData = data;

   if(pData==NULL)
    {
     //fIn.close();
     return IMG_ERR_MEM_FAIL;
    }

  // Read the file into memory
  //fIn.read((char*)pData,ulSize);

 // fIn.close();

  // Process the header
  iRet=ReadHeader();

   if(iRet!=IMG_OK)
    return iRet;

   if(iBPP!=8) // We'll only bother with 8 bit indexed and 24 bit RGB images
    return IMG_ERR_UNSUPPORTED;

   if(bEnc!=1) // We only know about RLE compressed images
    return IMG_ERR_UNSUPPORTED;

  // Get the image data
  iRet=LoadRLEData();

   if(iRet!=IMG_OK)
    return iRet;

  // Load palette if present
  iRet=LoadPalette(ulSize);

   if(iRet!=IMG_OK)
    return iRet;

  // Free the file data
  //delete [] pData;
  pData=NULL;

  // Update the BPP value to reflect the image format
  iBPP*=iPlanes;

  return IMG_OK;
 }


int PCXImg::ReadHeader()
 {
  unsigned short x1,x2,y1,y2;

   if(pData==NULL)
    return IMG_ERR_NO_FILE;

   if(pData[0]!=0xA) // PCX ID Byte, should be 0xA
    return IMG_ERR_BAD_FORMAT_ID;

   if(pData[1]>5)    // Version, we don't know about anything after v5
    return IMG_ERR_UNSUPPORTED;

  bEnc=pData[2];     // Encode flag 1 = RLE Compression

   if(pData[3]==1 || pData[3]==2 || pData[3]==4 || pData[3]==8)   // BPP value
    iBPP=pData[3];
   else
    return IMG_ERR_BAD_FORMAT_BPP;

  // Get image window and produce width & height values
  memcpy(&x1,&pData[4],2);
  memcpy(&y1,&pData[6],2);
  memcpy(&x2,&pData[8],2);
  memcpy(&y2,&pData[10],2);

  iWidth=(x2-x1)+1;
  iHeight=(y2-y1)+1;

   if(iWidth<1 || iHeight<1)
    return IMG_ERR_BAD_FORMAT_DIM;

  // Planes byte.  1 = Indexed,  3 = RGB
  iPlanes=pData[65];

  // Bits per line for decoding purposes,
  memcpy(&iBPL,&pData[66],2);

  return IMG_OK;
 }


int PCXImg::LoadRLEData()
 {
  int iLineCount,iBufferLineLen,iImageLineLen;
  long lLinePos=0;
  unsigned char bRunLen;
  unsigned char *pCur,*pLine,*pInterLine;

  // Set our pointer to the beginning of the image data
  pCur=&pData[128];

  // Calculate line lengths for image and buffer, Allocate the buffer scan line
  iBufferLineLen=iBPL*iPlanes;
  iImageLineLen =iWidth*iPlanes;
  pLine=new unsigned char[iBufferLineLen];

   if(pLine==NULL)
    return IMG_ERR_MEM_FAIL;

  // Allocate space for the image data
   if(pImage!=NULL)
    delete [] pImage;

  pImage=new unsigned char[(iImageLineLen * iHeight)+1];

   if(pImage==NULL)
    return IMG_ERR_MEM_FAIL;

   // Decode each scanline
   for(iLineCount=0;iLineCount<iHeight;++iLineCount)
    {
     lLinePos=0;
      while(lLinePos<iBufferLineLen)
       {
         if(*pCur > 0xC0) // First 2 bits indicate run of next byte value
          {
           bRunLen=*pCur & 0x3F; // Remaining 6 bits indicate run length
           ++pCur;  // Repeated value
            for( ;bRunLen!=0;bRunLen--,lLinePos++)
             pLine[lLinePos]=*pCur;

           ++pCur;
          }
         else
          {
           pLine[lLinePos]=*pCur; // Other bytes are directly copied
           ++lLinePos;
           ++pCur;
          }
       }

     // Once we've decoded a line, copy it to the image.
     // This disregards any end-of-line padding inserted during the compression

      if(iPlanes==1) // 8 bit images, straight copy
       {
        memcpy(&pImage[iLineCount*iImageLineLen],pLine,iImageLineLen);
       }
      else if(iPlanes==3) // for 24 bit, We have to interleave the RGB values
       {
        pInterLine=&pImage[iLineCount*iImageLineLen];
         for(lLinePos=0;lLinePos!=iWidth;++lLinePos,pInterLine+=3)
          {
           pInterLine[0]=pLine[lLinePos];
           pInterLine[1]=pLine[lLinePos+iWidth];
           pInterLine[2]=pLine[lLinePos+(iWidth*2)];
          }
       }

    }

  return IMG_OK;
 }


int PCXImg::LoadPalette(unsigned long ulDataSize)
 {
  // Load a 256 color palette

   if(pPalette)
    {
     delete [] pPalette;
     pPalette=NULL;
    }

   if(iPlanes==3) // NULL Palette for RGB images
    return IMG_OK;

  // Create space for palette
  pPalette=new unsigned char[768];

   if(pPalette==NULL)
    return IMG_ERR_MEM_FAIL;

  // Start of palette entries should be 769 bytes back from the end of the file
  // First byte is 0x0C
   if(pData[ulDataSize-769]!=0x0C)
    return IMG_ERR_BAD_FORMAT_PAL;

  memcpy(pPalette,&pData[ulDataSize-768],768);

  return IMG_OK;
 }


int PCXImg::GetBPP()
 {
  return iBPP;
 }


int PCXImg::GetWidth()
 {
  return iWidth;
 }


int PCXImg::GetHeight()
 {
  return iHeight;
 }


unsigned char* PCXImg::GetImg()
 {
  return pImage;
 }


unsigned char* PCXImg::GetPalette()
 {
  return pPalette;
 }
