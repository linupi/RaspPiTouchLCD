/*
 * lib_PD161704A.h:
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

//IO Ports
#define     LCD_RST_PIN     17
#define     LCD_RS_PIN      27
#define     LCD_SPI_CHANNEL  0

//color define
#define     CYAN	 0x07FF
#define     PURPLE	 0xF81F
#define     RED          0XF800	  
#define     GREEN        0X07E0	 
#define     BLUE         0X001F	  
#define     WHITE        0XFFFF	  
#define     BLACK        0X0000	  
#define     YELLOW       0XFFE0	  
#define     ORANGE       0XFC08	 
#define     GRAY         0X8430  
#define     LGRAY        0XC618	  
#define     DARKGRAY     0X8410	  
#define     PORPO        0X801F	  
#define     PINK         0XF81F	 
#define     GRAYBLUE     0X5458   
#define     LGRAYBLUE    0XA651   
#define     DARKBLUE     0X01CF	 
#define     LIGHTBLUE    0X7D7C

/*******************************************************************************
 * Function Name  : LCD_WRITE_REG
 * Description    : write 16 bit to LCD Register
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LCD_WRITE_REG(unsigned short int index);

/******************************************************************************
 * Function Name  : LCD_WRITE_COMMAND
 * Description    : send command and data to LCD
 * Input          : index, data
 * Output         : None
 * Return         : None
 ******************************************************************************/
void LCD_WRITE_COMMAND(unsigned short int index,unsigned short int data);

/*******************************************************************************
* Function Name  : LCD_WRITE_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WRITE_DATA(unsigned short int data);

/******************************************************************************
* Function Name  : lcd_init
* Description    : LCD initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void lcd_init(unsigned short int bgcolor);

/******************************************************************************
* Function Name  : lcd_clear_screen
* Description    : clear screen
* Input          : color_background: refresh background color
* Output         : None
* Return         : None
******************************************************************************/
void lcd_clear_screen(unsigned short int color_background);


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
                    unsigned height);


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
void lcd_display_char_px ( unsigned char ch_asc, 
					  unsigned short int color_front,
					  unsigned short int color_background, 
					  unsigned short int postion_x, 
					  unsigned short int postion_y);

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
void lcd_display_char_col_row(  unsigned char ch_asc, 
					  unsigned short int color_front,
					  unsigned short int color_background, 
					  unsigned short int postion_x, 
					  unsigned short int postion_y);

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
								unsigned short int color_front, 
								unsigned short int color_background, 
								unsigned char x, 
								unsigned char y );

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
void lcd_display_string_px(unsigned char *str, 
						   unsigned short int color_front, 
						   unsigned short int color_background, 
						   unsigned short int x, 
						   unsigned short int y );

/******************************************************************************
* Function Name  : lcd_draw_pixel
* Description    : draw pixel
* Input          : color, x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_pixel(unsigned int color_front,
                  unsigned short int x,
                  unsigned short int y);
/******************************************************************************
* Function Name  : lcd_draw_dot
* Description    : draw dot
* Input          : color, x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_dot(unsigned int color_front,
                  unsigned short int x,
                  unsigned short int y);


/******************************************************************************
* Function Name  : lcd_draw_line
* Description    : draw line,1 pix.
* Input          : color_frong, x, y , begin x1,y1 , end x2,y2
* Output         : None
* Return         : None
******************************************************************************/
unsigned char lcd_draw_line(  
			unsigned int line_color,
                          short int x1,
                          short int y1,
                          short int x2,
                          short int y2 );

unsigned char lcd_draw_thin_line(  
								 unsigned short int line_color,
								 short int x1,
								 short int y1,
								 short int x2,
								 short int y2 );

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
                          short int r );


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
					unsigned short int height );

void lcd_display_image(unsigned int* RGB, 
					unsigned short int x, 
					unsigned short int y, 
					unsigned short int width, 
					unsigned short int height );
