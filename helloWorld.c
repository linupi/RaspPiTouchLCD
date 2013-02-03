/*
 * helloWorld.c:
 *	Simple example to use lib_PD161704A.h
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
 */

#include "lib/lib_PD161704A.h"


void main()
{
    lcd_init();

       // display two characters at positions given through row and column number	
    unsigned char ch_asc = 'A';
    lcd_display_char_col_row(ch_asc,WHITE,BLACK,0,0);
    ch_asc = 'Z';
    lcd_display_char_col_row(ch_asc,BLACK,WHITE,29,19);
	
       // display two characters at positions given through pixel position	
    ch_asc = 'a';
    lcd_display_char_px(ch_asc,GREEN,RED,0,320-16);
    ch_asc = 'b';
    lcd_display_char_px(ch_asc,RED,GREEN,240-8,0);
    
        //display string
    char mystring[15]="Hello World!";   
    lcd_display_string_col_row(mystring, YELLOW, RED, 8, 5);
}