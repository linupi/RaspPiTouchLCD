/*
 *	2013 Linus Pithan 
 *	names and structure of functions and structs inspired by example codes given at
 *	http://www.xbdev.net/image_formats/bmp/index.php
 */

#include <stdio.h>

#pragma pack(1)
struct stBMFH // BitmapFileHeader
{
	char         bmtype[2];     // 2 bytes - 'B' 'M'
	unsigned int iFileSize;     // 4 bytes
	short int    reserved1;     // 2 bytes
	short int    reserved2;     // 2 bytes
	unsigned int iOffsetBits;   // 4 bytes

};// End of stBMFH structure - size of 18 bytes
#pragma pack()

#pragma pack(1)
struct stBMIF // BitmapInfoHeader
{
	unsigned int iSizeHeader;    // 4 bytes - 40
	int iWidth;         // 4 bytes
	int iHeight;        // 4 bytes
	short int    iPlanes;        // 2 bytes
	short int    iBitCount;      // 2 bytes
	unsigned int Compression;    // 4 bytes
	unsigned int iSizeImage;     // 4 bytes
	unsigned int iXPelsPerMeter; // 4 bytes
	unsigned int iYPelsPerMeter; // 4 bytes
	unsigned int iClrUsed;       // 4 bytes
	unsigned int iClrImportant;  // 4 bytes

};// End of stBMIF structure - size 40 bytes
#pragma pack()

//struct stImage
//{
//	unsigned int iWidth;
//	unsigned int iHeight;
//
//	unsigned int* pARGB; // Alpha Red Green Blue

//};// End of stImage

void get_bmp_info(FILE* fp, struct stBMFH* BMFH, struct stBMIF* BMINFO);
void bmp_load_rgb(FILE* fp, unsigned int* bmpRGB, struct stBMFH* BMFH, struct stBMIF* BMINFO);

