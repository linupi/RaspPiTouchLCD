/*
 *	2013 Linus Pithan 
 *	names and structure of functions and structs inspired by example codes given at
 *	http://www.xbdev.net/image_formats/bmp/index.php
 */

#include "bmpLoad.h"
#include <stdio.h>


void get_bmp_info(FILE* fp, struct stBMFH* BMFH, struct stBMIF* BMINFO)
{


	fread(BMFH,   1, sizeof(*BMFH), fp);
	fread(BMINFO, 1, sizeof(*BMINFO), fp);

	if((*BMINFO).iHeight<0)
		(*BMINFO).iHeight*=-1;
	if((*BMINFO).iWidth<0)
		(*BMINFO).iWidth*=-1;

	printf("in bmpLoad w:%d h:%d \n",(*BMINFO).iWidth,(*BMINFO).iHeight);
}

void bmp_load_rgb(FILE* fp, unsigned int* bmpRGB, struct stBMFH* BMFH, struct stBMIF* BMINFO)
{

	
    fseek( fp, (*BMFH).iOffsetBits, SEEK_SET);


	int iNumPaddedBytes = ((*BMINFO).iWidth * 3) % 4;

	for(unsigned int  h=0; h<(*BMINFO).iHeight; h++)
	{
		for(unsigned int  w=0; w<(*BMINFO).iWidth; w++)
		{
			
			unsigned char r,g,b;
			fread(&b, 1, 1, fp);
			fread(&g, 1, 1, fp);
			fread(&r, 1, 1, fp);
			
			bmpRGB[ w + h*(*BMINFO).iWidth ] = (r<<16 | g<<8 | b);

		}// End of for loop w

		//If there are any padded bytes - we skip over them here
		if( iNumPaddedBytes != 0 )
		{
			unsigned char skip[4];
			fread(skip, 1, 4 - iNumPaddedBytes, fp);
		}// End of if reading padded bytes

	}// End of for loop h
}