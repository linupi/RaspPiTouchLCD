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

int step=0;
unsigned short int adc_x1, adc_y1,adc_x2,adc_y2,adc_x3,adc_y3,adc_x4,adc_y4;

void recalcTouchCalib(void)
{
    printf("ADC-Values: x1=%i y1=%i x2=%i y2=%i x3=%i y3=%i x4=%i y4=%i\n",adc_x1,adc_y1,adc_x2,adc_y2,adc_x3,adc_y3,adc_x4,adc_y4);
    float adc_xA,adc_xB,adc_yA,adc_yB,mx,my,bx,by;
    adc_xA=((float)adc_x1+(float)adc_x3)/(float)2;
    adc_xB=((float)adc_x2+(float)adc_x4)/(float)2;
    adc_yA=((float)adc_y1+(float)adc_y2)/(float)2;
    adc_yB=((float)adc_y3+(float)adc_y4)/(float)2;
    printf("ADC-Mean-Values: adc_xA=%f, adc_xB=%f, adc_yA=%f, adc_yB=%f \n",adc_xA,adc_xB,adc_yA,adc_yB);
    
    mx=((float)(180-60))/(adc_xB-adc_xA);
    bx=(((float)60*adc_xB)-(float)180*adc_xA)/(adc_xB-adc_xA);

    my=((float)(240-80))/(adc_yB-adc_yA);
    by=(((float)80*adc_yB)-(float)240*adc_yA)/(adc_yB-adc_yA);
    
    printf("Calibration-Parameters: mx=%f, bx=%f, my=%f, by=%f \n",mx,bx,my,by);
    
    printf("******************************************************\n**** Please copy the following line just after init_touch(); in your main routine ****\n");
    printf("\n\nset_touch_calib(%f, %f, %f, %f);\n\n",mx, bx, my, by);
    printf("******************************************************\nExit program with ctrl c !\n");
    
    
    set_touch_calib(mx, bx, my, by);

}

void getPos(void)
{
    unsigned short int x,y,adc_x,adc_y;
    deactivate_irq();
    read_touch_ADC(&adc_x,&adc_y);
    convert_ADC_to_xy(adc_x,adc_y,&x,&y);
    if((adc_x!=0) & (adc_y!=0))
    {
	if (step <= 3 )
	{
	    lcd_draw_dot(CYAN,x,y);
	    lcd_draw_circle(CYAN,x,y,20);
	}
	else
	{
	    lcd_draw_dot(PURPLE,x,y);
	    lcd_draw_circle(PURPLE,x,y,20);
	}
	if (step==0)
	{
	    adc_x1=adc_x;
	    adc_y1=adc_y;
	    step++;
	    lcd_draw_thin_line(GREEN,150,80,210,80);
	    lcd_draw_thin_line(GREEN,180,50,180,110);

	    char str[31]=" Press center of green cross! ";
	    lcd_display_string_px(str,WHITE,BLACK,0,156);
	}
	else if (step==1)
	{
	    adc_x2=adc_x;
	    adc_y2=adc_y;
	    step++;
	    lcd_draw_thin_line(BLUE,30,240,90,240);
	    lcd_draw_thin_line(BLUE,60,210,60,270);

	    char str[31]="  Press center of blue cross! ";
	    lcd_display_string_px(str,WHITE,BLACK,0,156);
	}
	else if (step==2)
	{
	    adc_x3=adc_x;
	    adc_y3=adc_y;
	    step++;
	    lcd_draw_thin_line(ORANGE,150,240,210,240);
	    lcd_draw_thin_line(ORANGE,180,210,180,270);

	    char str[31]=" Press center of orange cross!";
	    lcd_display_string_px(str,WHITE,BLACK,0,156);
	}
	else if (step==3)
	{
	    adc_x4=adc_x;
	    adc_y4=adc_y;
	    
	    recalcTouchCalib();
	    
	    char str[31]="Calibration Completed! Try now";
	    lcd_display_string_px(str,WHITE,BLACK,0,156);
	    
	    step++;
	    
	    lcd_draw_thin_line(RED,30,80,90,80);
	    lcd_draw_thin_line(RED,60,50,60,110);
	    lcd_draw_thin_line(GREEN,150,80,210,80);
	    lcd_draw_thin_line(GREEN,180,50,180,110);
	    lcd_draw_thin_line(BLUE,30,240,90,240);
	    lcd_draw_thin_line(BLUE,60,210,60,270);
	    lcd_draw_thin_line(ORANGE,150,240,210,240);
	    lcd_draw_thin_line(ORANGE,180,210,180,270);
	}
    }
    reactivate_irq();
}

void main()
{

    lcd_init(WHITE);
    init_touch();
    
    
    lcd_draw_line(BLACK,120,0,120,320);
    lcd_draw_line(BLACK,0,160,240,160);
    
    lcd_draw_thin_line(RED,30,80,90,80);
    lcd_draw_thin_line(RED,60,50,60,110);

    char str[31]="  Press center of red cross!  ";
    lcd_display_string_px(str,WHITE,BLACK,0,156);
    
    
    if(register_touch_isr(&getPos)<0)
        {printf("Unable to register touch isr\n");}
    else{
        printf("exit program with ctrl c !\n");
        for (;;)
            {;}
    }

    deactivate_irq();

}