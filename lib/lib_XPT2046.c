/*
 * lib_XPT2046.c:
 *	Adding XPT2046 support to Raspberry Pi
 *	Copyright (c) 2013 Linus Pithan 
 *
 *
 ***********************************************************************
 * This file is part of lib_XPT2046:
 *	http://projects.pithan.net/lcd-touch-module/
 *
 *    lib_XPT2046 is free software: you can redistribute it and/or modify
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
 *    License along with lib_XPT2046.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "lib_XPT2046.h"

float mx=0.1364;
float bx=-18.8864;
float my=0.1810;
float by=-21.8100;


int init_touch(void)
{
     wiringPiSPISetup (TOUCH_SPI_CHANNEL, 500000);
     unsigned char data [] = {0x80,0x00,0x00};
     wiringPiSPIDataRW (TOUCH_SPI_CHANNEL, data, 3) ;
}

int deactivate_irq(void)
{
    char command[40]="gpio edge 22 none";
    return system(command);
}
int reactivate_irq(void)
{
    char command[40]="gpio edge 22 falling";
    return system(command);
}

void set_touch_calib(float new_mx, float new_bx, float new_my, float new_by)
{
	mx=new_mx;
	bx=new_bx;
	my=new_my;
	by=new_by;
}

int register_touch_isr(void (*func)(void))
{
    return wiringPiISR (TOUCH_IRQ_PIN, INT_EDGE_FALLING , func) ;
}

void read_touch_ADC(unsigned short int* x, unsigned short int* y)
{
    unsigned char data_x [] = {0xD0,0x00,0x00};
    unsigned char data_y [] = {0x90,0x00,0x00};
    wiringPiSPIDataRW (TOUCH_SPI_CHANNEL, data_x, 3);
    wiringPiSPIDataRW (TOUCH_SPI_CHANNEL, data_y, 3);
  //  printf("x %x %x y %x %x\n",data_x[1],data_x[2],data_y[1],data_y[2]);
    *x = data_x[1];
    *x = ((*x)<<8)+data_x[2];
    *x>>=4;
    *y = data_y[1];
    *y = ((*y)<<8)+data_y[2];
    *y>>=4;
   // printf("X %i Y %i\n",*x ,*y);
}

void convert_ADC_to_xy(unsigned short int adc_x, unsigned short int adc_y ,unsigned short int* px_x, unsigned short int* px_y)
{  
	
    *px_x=(mx*(float)adc_x+bx);
    *px_y=(my*(float)adc_y+by);
}

int get_touch_position(unsigned short int* x, unsigned short int* y)
{
    unsigned short int adc_x, adc_y;
    read_touch_ADC(&adc_x, &adc_y);
    convert_ADC_to_xy(adc_x,adc_y,x,y);
    if(adc_x==0 | adc_y==0)
        return 0;
    else
        return 1;
}