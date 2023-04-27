/*
 Name:		JalaliDateLib.h
 Created:	8/3/2018 2:32:14 PM
 Author:	ghasem poorahar
 
*/

#ifndef _JalaliDateLib_h
#define _JalaliDateLib_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Date
{
public:
	Date(int iYear, int iMonth, int iDay);
	String JDate();
	int JYear();
	int JMonth();
	int JDay();
private:
	int _iYear;
	int _iMonth;
	int _iDay;
};

#endif



