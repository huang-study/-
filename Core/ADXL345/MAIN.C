/********************************************************************************

 Author        : Neil.Zhao  

 Date          : Sep 25th 2009

 File          : main.c

 Hardware      : ADXL345 EVALUATION BOARD (Based on M Board)`ADUC7026+ADXL345
		
********************************************************************************/

#include <Common.h>
#include "ADXL345.H"	

unsigned char buf[4] = {0,0,0,0};

int ia(void) 
{

/*
	buf[0]=0X03;
	WriteToADXL345ViaSpi(XL345_OFSX,1,buf);
	buf[0]=0XFF;
	ReadFromADXL345ViaSpi(XL345_OFSX,1,buf);
	putchar(buf[0]);
*/
	ReadFromADXL345ViaSpi(XL345_DEVID,1,buf);
	putchar(buf[0]);

	while(1)
    {;}
}
