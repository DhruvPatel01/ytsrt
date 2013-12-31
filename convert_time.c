/*
	Copyright 2013 Dhruv Patel.

	This file is part of ytsrt.

    ytsrt is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    ytsrt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include "structures.h"

/* convert google time from xml file to srt time format and store it in 
 * srt time structurtue passed in argument.
 * \param xmlTime in seconds.
 * \param srtTime structure to store converted time.
 * \return 0==OK 1==!OK
 */

int convert_time(double xmlTime,srt_time_t *srtTime)
{
	int temp;
	int msec,sec,min,hour;

	/*	xmlTime may have miliseconds like 1234.78
	 *	so store digits after decimal points in to
	 *	msec.
	 */

	temp = (int)(xmlTime * 1000); //remove decimal point
	msec = temp%1000; //store last three digits as msec

	srtTime->msec = msec;

	sec = (int)(temp/1000);//remove last three digits(msec)

	/*	Check the seconds left after removing miliseconds
	*		-> if seconds are less than 60 then store them as it
	*		is and make HH:MM = 00:00
	* 	-> but if greater than 60 then convert them in to minutes.
	*		if minutes are greater than 60 then convert them into hours
	*			else store minutes as it is and make HH = 00
	*/ 
	if(sec<60){

		srtTime->sec = sec;
		srtTime->min = 0;
		srtTime->hour = 0;

		return 0;
	} else {
		min = (int)(sec/60);//convert seconds into int minutes
		sec -= min*60;//make those minutes into seconds and retrive left seconds

		srtTime->sec = sec;

		if(min<60){

			srtTime->min = min;
			srtTime->hour = 0;

			return 0;
		}else {
			hour = (int)(min/60);//convert minutes into int hours
			min -= hour*60;//retrive left minutes 

			srtTime->min = min;
			srtTime->hour = hour;
			return 0;
		}//end if-else (min<60)
	}//end if-else (sec<60)
	return 1;
}