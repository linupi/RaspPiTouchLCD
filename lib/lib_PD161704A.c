/*
 * lib_PD161704A.c:
 *	SDriver Library for a 240x320 Touch Display with uPD161704A and XPT2046 chipset
 *	Copyright (c) 2013 Linus Pithan 
 *
 *
 ***********************************************************************
 * This file is part of lib_PD161704A:
 *	http://projects.pithan.net/lcd-touch-module/
 *
 *    lib_PD161704A is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    lib_PD161704A is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with lib_PD161704A.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 *
 *	names and structure of functions inspired by example codes given at
 *	http://www.wvshare.com/product/2.2inch-320x240-Touch-LCD-A.htm
 */

//includes
#include "lib_PD161704A.h"
#include "simple_ascii.h"
#include <unistd.h> // for micro second sleep timer
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>




// Providing access to RPi SPI port
unsigned char SPI_communication(unsigned char *data, int len)
{
    wiringPiSPIDataRW (LCD_SPI_CHANNEL, data, len);
}

// Display Reset via IO Pin
void LCD_RST_H()
{
    digitalWrite (LCD_RST_PIN, HIGH);
}
void LCD_RST_L()
{
    digitalWrite (LCD_RST_PIN, LOW);
}

// Display RS via IO Pin
void LCD_RS_H()
{
    digitalWrite (LCD_RS_PIN, HIGH);
}
void LCD_RS_L()
{
    digitalWrite (LCD_RS_PIN, LOW);
}

/*******************************************************************************
 * Function Name  : LCD_WRITE_REG
 * Description    : write 16 bit to LCD Register
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LCD_WRITE_REG(unsigned short int index)
{
    unsigned short int value_index;
    unsigned char comData [2];
	
    LCD_RS_L();
    value_index=index;
    value_index =value_index>>8;
    comData [0]=(unsigned char)(value_index);
	
    value_index=index;
    value_index &=0x00ff;
    comData [1]=(unsigned char)(index);
    SPI_communication(comData, 2);

    LCD_RS_H();
}

/******************************************************************************
 * Function Name  : LCD_WRITE_COMMAND
 * Description    : send command and data to LCD
 * Input          : index, data
 * Output         : None
 * Return         : None
 ******************************************************************************/
void LCD_WRITE_COMMAND(unsigned short int index,unsigned short int data)
{
	unsigned char comIndex [2];
	unsigned char comData [2];
	
	comIndex[0]=(unsigned char)(index>>8);
	comIndex[1]=(unsigned char)(index);
	comData[0]=(unsigned char)(data>>8);
	comData[1]=(unsigned char)(data);
	

	//select command register
	LCD_RS_L();
	SPI_communication(comIndex, 2);
	//send data
	LCD_RS_H();
	SPI_communication(comData, 2);
}

/*******************************************************************************
* Function Name  : LCD_WRITE_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WRITE_DATA(unsigned short int data)
{   unsigned char comData [2];

    comData[0]=(unsigned char)(data>>8);
    comData[1]=(unsigned char)(data);
    SPI_communication(comData, 2);    //00000000 000000000   
}

/******************************************************************************
* Function Name  : lcd_init
* Description    : LCD initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void lcd_init(void)
{
    	if (wiringPiSetupGpio () == -1)
                exit (1) ;
	
	pinMode (LCD_RST_PIN, OUTPUT) ;
	pinMode (LCD_RS_PIN, OUTPUT) ;
	//wiringPiSPISetup (LCD_SPI_CHANNEL, 1000000);
         wiringPiSPISetup (LCD_SPI_CHANNEL, 10000000);
	LCD_RST_L();

	usleep(500);
	LCD_RST_H();
	usleep(500);

	//register reset
	LCD_WRITE_COMMAND(0x0003,0x0001);	//Soft reset (reset pulse occurs by writing in "1")

	//oscillator start
	LCD_WRITE_COMMAND(0x003A,0x0001);	//Oscillator control (0:oscillator stop, 1: oscillator operation)
	usleep(100);

	//y-setting
	LCD_WRITE_COMMAND(0x0024,0x007B);	//amplitude setting
	usleep(10);
	LCD_WRITE_COMMAND(0x0025,0x003B);	//amplitude setting	
	LCD_WRITE_COMMAND(0x0026,0x0034);	//amplitude setting
	usleep(10);
	LCD_WRITE_COMMAND(0x0027,0x0004);	//amplitude setting	
	LCD_WRITE_COMMAND(0x0052,0x0025);	//circuit setting 1
	usleep(10);
	LCD_WRITE_COMMAND(0x0053,0x0033);	//circuit setting 2	
	LCD_WRITE_COMMAND(0x0061,0x001C);	//adjustment V10 positive polarity
	usleep(10);
	LCD_WRITE_COMMAND(0x0062,0x002C);	//adjustment V9 negative polarity
	LCD_WRITE_COMMAND(0x0063,0x0022);	//adjustment V34 positive polarity
	usleep(10);
	LCD_WRITE_COMMAND(0x0064,0x0027);	//adjustment V31 negative polarity
	usleep(10);
	LCD_WRITE_COMMAND(0x0065,0x0014);	//adjustment V61 negative polarity
	usleep(10);
	LCD_WRITE_COMMAND(0x0066,0x0010);	//adjustment V61 negative polarity
	
	//Basical clock for 1 line (BASECOUNT[7:0]) number specified
	LCD_WRITE_COMMAND(0x002E,0x002D);
	
	//Power supply setting
	LCD_WRITE_COMMAND(0x0019,0x0000);	//DC/DC output setting
	usleep(200);
	LCD_WRITE_COMMAND(0x001A,0x1000);	//DC/DC frequency setting
	LCD_WRITE_COMMAND(0x001B,0x0023);	//DC/DC rising setting
	LCD_WRITE_COMMAND(0x001C,0x0C01);	//Regulator voltage setting
	LCD_WRITE_COMMAND(0x001D,0x0000);	//Regulator current setting
	LCD_WRITE_COMMAND(0x001E,0x0009);	//VCOM output setting
	LCD_WRITE_COMMAND(0x001F,0x0035);	//VCOM amplitude setting	
	LCD_WRITE_COMMAND(0x0020,0x0015);	//VCOMM cencter setting	
	LCD_WRITE_COMMAND(0x0018,0x1E7B);	//DC/DC operation setting

	//windows setting
	LCD_WRITE_COMMAND(0x0008,0x0000);	//Minimum X address in window access mode
	LCD_WRITE_COMMAND(0x0009,0x00EF);	//Maximum X address in window access mode
	LCD_WRITE_COMMAND(0x000a,0x0000);	//Minimum Y address in window access mode
	LCD_WRITE_COMMAND(0x000b,0x013F);	//Maximum Y address in window access mode

	//LCD display area setting
	LCD_WRITE_COMMAND(0x0029,0x0000);	//[LCDSIZE]  X MIN. size set
	LCD_WRITE_COMMAND(0x002A,0x0000);	//[LCDSIZE]  Y MIN. size set
	LCD_WRITE_COMMAND(0x002B,0x00EF);	//[LCDSIZE]  X MAX. size set
	LCD_WRITE_COMMAND(0x002C,0x013F);	//[LCDSIZE]  Y MAX. size set	

	//Gate scan setting
	LCD_WRITE_COMMAND(0x0032,0x0002);
	
	//n line inversion line number
	LCD_WRITE_COMMAND(0x0033,0x0000);

	//Line inversion/frame inversion/interlace setting
	LCD_WRITE_COMMAND(0x0037,0x0000);
	
	//Gate scan operation setting register
	LCD_WRITE_COMMAND(0x003B,0x0001);
	
	//Color mode
	LCD_WRITE_COMMAND(0x0004,0x0000);	//GS = 0: 260-k color (64 gray scale), GS = 1: 8 color (2 gray scale)

	//RAM control register
	LCD_WRITE_COMMAND(0x0005,0x0010);	//Window access control (0: Normal access, 1: Window access)
//	LCD_WRITE_COMMAND(0x0005,0x0000);	//Window access control (0: Normal access, 1: Window access)
	

	//Display setting register 2
	LCD_WRITE_COMMAND(0x0001,0x0000);

	//display setting	
	LCD_WRITE_COMMAND(0x0000,0x0000);	//display on

	usleep(20);
	
	lcd_clear_screen(BLUE);
}

/******************************************************************************
* Function Name  : lcd_clear_screen
* Description    : clear screen
* Input          : color_background: refresh background color
* Output         : None
* Return         : None
******************************************************************************/
void lcd_clear_screen(unsigned short int color_background)
{
	unsigned int i,j;
	unsigned char comData [480];

	LCD_WRITE_COMMAND(0x08,0x0000);
	LCD_WRITE_COMMAND(0x0a,0x0000);
	LCD_WRITE_COMMAND(0x09,0x00EF);
	LCD_WRITE_COMMAND(0x0b,0x013F);

	LCD_WRITE_COMMAND(0x06,0x0000);
	LCD_WRITE_COMMAND(0x07,0x0000);
	
	LCD_WRITE_REG(0x0E);	//RAM Write index

	LCD_RS_H();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
		  comData[j*2]=(unsigned char)(color_background>>8);
		  comData[j*2+1]=(unsigned char)(color_background);
		 
		}
		SPI_communication(comData, 480);
	}
	LCD_RS_L();
}

/******************************************************************************
* Function Name  : lcd_draw_rect
* Description    : clear area,fill
* Input          : color_front:draw a rectangle, fill with color_front
                   x:
                   y:
                   width: rectangle width
                   height:rectangle height
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_rect(unsigned int color_front, 
                    unsigned char x, 
                    unsigned int y, 
                    unsigned int width, 
                    unsigned height)
{
	unsigned int i, j;

	LCD_WRITE_COMMAND( 0x08, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x0a, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x09, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x0b, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );

	LCD_WRITE_REG( 0x0E );

	//LCD_CS_L();
	LCD_RS_H();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			LCD_WRITE_DATA( color_front );

		}
	}
	//LCD_CS_H();
}


/******************************************************************************
 * Function Name  : lcd_display_char_px
 * Description    : ch_asc: ascii code of data. position_x in px, position_y in px.
 color_front, color_background.
 * Input          : None
 * Output         : None
 * Return         : None
 * REMARKS:         max value of position_x: 232
 *                  max value of position_y: 304
 ******************************************************************************/
void lcd_display_char_px (  unsigned char ch_asc, 
					  unsigned short int color_front,
					  unsigned short int color_background, 
					  unsigned short int postion_x, 
					  unsigned short int postion_y)
{
	unsigned char i, j, b;
	const unsigned char *p = 0;
	unsigned int n=0;
	unsigned char comData [256];
	
	LCD_WRITE_COMMAND(0x08,postion_x); 	//x start point
	LCD_WRITE_COMMAND(0x0a,postion_y); 	//y start point
	LCD_WRITE_COMMAND(0x09,postion_x+7);	//x end point
	LCD_WRITE_COMMAND(0x0b,postion_y+15);	//y end point
	
	LCD_WRITE_COMMAND(0x06,postion_x);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_WRITE_COMMAND(0x07,postion_y);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
	LCD_WRITE_REG(0x0E);	//RAM Write index(prepare to write data)
	
	LCD_RS_H();
	
	p = ascii;
	p += ch_asc*16;
	for(j=0;j<16;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if( b & 0x80 )
			{
				comData[n]=(unsigned char)(color_front>>8);
				comData[n+1]=(unsigned char)(color_front);
			}
			else
			{
				comData[n]=(unsigned char)(color_background>>8);
				comData[n+1]=(unsigned char)(color_background);
			}
			b=b<<1;			
			n=n+2;
		}
	}
	SPI_communication(comData, 256);
}

/******************************************************************************
 * Function Name  : lcd_display_char_col_row
 * Description    : ch_asc: ascii code of data. position_x in columns, position_y in rows.
 color_front, color_background.
 * Input          : None
 * Output         : None
 * Return         : None
 * REMARKS:         max value of position_x: 29
 *                  max value of position_y: 19
 ******************************************************************************/
void lcd_display_char_col_row  (unsigned char ch_asc, 
					  unsigned short int color_front,
					  unsigned short int color_background, 
					  unsigned short int postion_x, 
					  unsigned short int postion_y)
{
 lcd_display_char_px( ch_asc, color_front,color_background, postion_x*8, postion_y*16);
}

/******************************************************************************
* Function Name  : lcd_display_string_col_row
* Description    : *str: address of string data. 
					x: the xth row(0~30).
					y: the yth column(0~20).
					color_front, color_background.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void lcd_display_string_col_row(unsigned char *str, 
						unsigned int color_front, 
						unsigned int color_background, 
						unsigned char x, 
						unsigned char y )
{
	while (*str) 
	{ 
		lcd_display_char_col_row( *str, color_front, color_background, x, y);
		if(++x>=30)
		{
			x=0;
			if(++y>=20)
			{
				y=0;
			}
		}
		str ++;
    }
}

/******************************************************************************
* Function Name  : lcd_draw_dot
* Description    : draw dot
* Input          : color, x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_pixel(unsigned int color_front,
                  unsigned short int x,
                  unsigned short int y)
{
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );
	LCD_WRITE_COMMAND( 0x0E, color_front );
}


/******************************************************************************
* Function Name  : lcd_draw_bigdot
* Description    : draw big dot,9 pix.
* Input          : color_frong, x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_dot(unsigned int   color_front,
                     unsigned short int  x,
                     unsigned short int  y )
{
    lcd_draw_pixel(color_front,x,y);
    lcd_draw_pixel(color_front,x,y+1);
    lcd_draw_pixel(color_front,x,y-1);

    lcd_draw_pixel(color_front,x+1,y);
    lcd_draw_pixel(color_front,x+1,y+1);
    lcd_draw_pixel(color_front,x+1,y-1);
    
    lcd_draw_pixel(color_front,x-1,y);    
    lcd_draw_pixel(color_front,x-1,y+1);
    lcd_draw_pixel(color_front,x-1,y-1);
    
    lcd_draw_pixel(color_front,x-2,y);    
    lcd_draw_pixel(color_front,x-2,y+1);
    lcd_draw_pixel(color_front,x-2,y-1);
    
    lcd_draw_pixel(color_front,x+2,y);
    lcd_draw_pixel(color_front,x+2,y+1);
    lcd_draw_pixel(color_front,x+2,y-1);
    
    lcd_draw_pixel(color_front,x+1,y+2);
    lcd_draw_pixel(color_front,x+1,y-2);
    
    lcd_draw_pixel(color_front,x-1,y+2);
    lcd_draw_pixel(color_front,x-1,y-2);
}

/******************************************************************************
* Function Name  : lcd_draw_line
* Description    : draw line,1 pix.
* Input          : color_frong, x, y , begin x1,y1 , end x2,y2
* Output         : None
* Return         : None
******************************************************************************/
void Swap(unsigned short int *a , unsigned short int *b)	//for BresenhamLine
{
	unsigned short int tmp;
	tmp = *a ;
	*a = *b ;
	*b = tmp ;
}

unsigned char lcd_draw_line(  
			unsigned int line_color,
                          short int x1,
                          short int y1,
                          short int x2,
                          short int y2 )
{
	  short int dx , dy;
	  short int tx , ty;
	  short int inc1 , inc2;
	  short int d , iTag;
	  short int x , y;
	lcd_draw_dot( line_color , x1 , y1 );
	if( x1 == x2 && y1 == y2 )	
	{
		return 1;
	}
	iTag = 0;
	dx = ( x2 - x1 );
        if(dx < 0) dx=-dx;
	dy = ( y2 - y1 );
        if(dy < 0) dy=-dy;
	if( dx < dy )	
	{
		iTag = 1 ;
		Swap ( &x1, &y1 );
		Swap ( &x2, &y2 );
		Swap ( &dx, &dy );
	}
	tx = ( x2 - x1 ) > 0 ? 1 : -1;	   
	ty = ( y2 - y1 ) > 0 ? 1 : -1;
	x = x1;
	y = y1;
	inc1 = 2 * dy;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while( x != x2 )	 
	{
		if( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if( iTag )
		{
			lcd_draw_dot ( line_color, y, x ) ;
		}
		else
		{
			lcd_draw_dot ( line_color, x, y ) ;
		}
		x += tx ;
	}
	return 0;
}


/******************************************************************************
* Function Name  : lcd_draw_circle
* Description    : draw line,1 pix.
* Input          : color_frong, x, y , r
* Output         : None
* Return         : None
******************************************************************************/

unsigned char lcd_draw_circle(  
			unsigned int line_color,
                          short int x0,
                          short int y0,
                          short int r )
{         short int f=1-r;
          short int ddF_x=1;
          short int ddF_y=-2*r;
          short int x=0;
          short int y=r;
          
          lcd_draw_dot ( line_color, x0, y0+r ) ;
          lcd_draw_dot ( line_color, x0, y0-r ) ;
          lcd_draw_dot ( line_color, x0+r, y0 ) ;
          lcd_draw_dot ( line_color, x0-r, y0 ) ;
        while(x<y)
       {
         if(f >=0)
         {y--;
          ddF_y +=2;
          f += ddF_y;
         }
         x++;
         ddF_x += 2;
         f +=ddF_x;
         lcd_draw_dot ( line_color, x0+x, y0+y ) ;
         lcd_draw_dot ( line_color, x0-x, y0+y ) ;
         lcd_draw_dot ( line_color, x0+x, y0-y ) ;
         lcd_draw_dot ( line_color, x0-x, y0-y ) ;
         lcd_draw_dot ( line_color, x0+y, y0+x ) ;
         lcd_draw_dot ( line_color, x0-y, y0+x ) ;
         lcd_draw_dot ( line_color, x0+y, y0-x ) ;
         lcd_draw_dot ( line_color, x0-y, y0-x ) ;
	}
	return 0;
}

/******************************************************************************
* Function Name  : lcd_display_pxarray
* Description    : Draw image
* Input          : x, y: image start at x, y. width, length, *img.
* Output         : None
* Return         : None
******************************************************************************/
void lcd_display_pxarray( const unsigned char *img, 
					unsigned short int x, 
					unsigned short int y, 
					unsigned short int width, 
					unsigned short int height )
{
	unsigned short int i, j;
	unsigned short int data16;


	LCD_WRITE_COMMAND( 0x08, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x0a, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x09, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x0b, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );

	LCD_WRITE_REG( 0x0E );

	//LCD_CS_L();
	LCD_RS_H();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			//Be carful of MCU type, big endian or little endian
			//little endian
			data16 = ( *(img + 1) << 8 ) | (*img);
			LCD_WRITE_DATA( data16 );
			img += 2;
		}
	}
	//LCD_CS_H();
}


void lcd_display_image(unsigned int* RGB, 
					unsigned short int x, 
					unsigned short int y, 
					unsigned short int width, 
					unsigned short int height )
{
	unsigned short int highColorRGB[width];
        
	LCD_WRITE_COMMAND( 0x08, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x0a, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x09, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x0b, y + height - 1 );	//y end point
	
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );
	
	LCD_WRITE_REG( 0x0E );
	
	//LCD_CS_L();
	LCD_RS_H();
	
	char buf;
        
	for(unsigned int h=0; h<height; h++)
	{
		for(unsigned int  w=0; w<width; w++)
		{
			unsigned char r = (RGB[ w + h*width ] >> 16) & 0xff;
			unsigned char g = (RGB[ w + h*width ] >>  8) & 0xff;
			unsigned char b = (RGB[ w + h*width ] >>  0) & 0xff;
			
			highColorRGB[w]=((unsigned short int)(r & (~0x07)))<<8;
			highColorRGB[w]+=((unsigned short int)(g & (~0x03)))<<3;
			highColorRGB[w]+=((unsigned short int)(b & (~0x07)))>>3;
			
			buf=((unsigned short int)(highColorRGB[w] & (~0x00FF)))>>8;
			highColorRGB[w]=(highColorRGB[w] & (~0xFF00))<<8;
		    highColorRGB[w]+=buf;
									 
		//	printf( "%.2x%.2x%.2x %.4x \t ", r,g,b,highColorRGB[n]);
		//	debug(buf);
			
		}// End of for loop w
		SPI_communication((char*)highColorRGB, width*2);
	//	debug("\n");
		//printf("\n");
	}// End of for loop h

}


	