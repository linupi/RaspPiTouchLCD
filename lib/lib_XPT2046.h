/*
 * lib_XPT2046.h:
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

#define TOUCH_IRQ_PIN 22
#define TOUCH_SPI_CHANNEL 1

int init_touch(void);

int register_touch_isr(void (*func)(void));

void read_touch_ADC(unsigned short int* x, unsigned short int* y);

void convert_ADC_to_xy(unsigned short int adc_x, unsigned short int adc_y ,unsigned short int* px_x, unsigned short int* px_y);

int get_touch_position(unsigned short int* x, unsigned short int* y);

int reactivate_irq(void);

int deactivate_irq(void);


