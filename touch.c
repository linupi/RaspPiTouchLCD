/*
 * touch.c:
 *	Simple example how to use lib_XPT2046.h
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

//includes
#include "lib/lib_PD161704A.h"
#include "lib/lib_XPT2046.h"

#include <stdio.h>

void printPos(void)
{
    unsigned short int x,y;
    deactivate_irq();
    if(get_touch_position(&x, &y)==1)
    { 
      
        printf("Touch event at postion x=%i y=%i \n",x,y);
        //draw dot
        lcd_draw_dot(RED,x,y);
		if(x<130 & x>60 & y<20)
        {
            lcd_clear_screen(GRAY);
            lcd_draw_rect(BLACK,60, 0, 130, 20);    
	    //write string to display
	    unsigned char str[16] = "press to clear!";
	    lcd_display_string_col_row(str,RED,BLACK,8,0);
	    unsigned char str2[11] = "draw here!";
	    lcd_display_string_col_row(str2,BLUE,GRAY,8,10);
        }
    }
    reactivate_irq();
}

void main()
{

    lcd_init(LIGHTBLUE);
    init_touch();
	set_touch_calib(0.133630, -18.240534, 0.178472, -22.353598); // To find the right parameter for your screen please use the touch_calibration app
    
    
    lcd_draw_rect(BLACK,60, 0, 130, 20);    
    //write string to display
    unsigned char str[16] = "press to clear!";
    lcd_display_string_col_row(str,RED,BLACK,8,0);
    unsigned char str2[11] = "draw here!";
    lcd_display_string_col_row(str2,GREEN,BLUE,8,10);
    
    
    if(register_touch_isr(&printPos)<0)
        {printf("Unable to register touch isr\n");}
    else{
        printf("exit program with ctrl c !\n");
        for (;;)
            {;}
    }

    deactivate_irq();

}