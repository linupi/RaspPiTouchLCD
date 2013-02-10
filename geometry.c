/*
 * geometry.c:
 *	Simple example to use graphical functions in lib_PD161704A
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
    lcd_init(BLUE);

    //draw pixel
    lcd_draw_pixel(GREEN,110,20);
    //draw dot
    lcd_draw_dot(RED,130,20);
    
    //draw line
    lcd_draw_line(YELLOW,0,50,240,50);
    lcd_draw_thin_line(YELLOW,0,60,240,60);
    
    //draw circle
    lcd_draw_circle(WHITE,120,160,20);
    
    //draw rectangle
    lcd_draw_rect(BLACK,95, 200, 50, 80);
}