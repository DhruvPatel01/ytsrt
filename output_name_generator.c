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
#include <string.h>
#include "constants.h"

/** output_name_generator generates name of srt file based on input file name
 * \param pointer to input name string
 * \param pointer to output name array
 * \param pointer to lang_code string
 */

void output_name_generator(const char *input,char *output, char *lang)
{
	char tempname[MAX_SIZE];
	strcpy(tempname,input);

	if (lang[0] != '\0') {
		strcat(tempname,"_");
		strcat(tempname,lang);
	}

	/* if input is from convert section it should have .xml suffix.
	 * in this case remove that .xml from tempname.
	 * incase ther is .srt than remove it also.	 
	 */
	char *j;
	if ((j = strstr(tempname,".xml")) || (j = strstr(tempname,".srt"))) {
		if ((j-tempname) == strlen(tempname)-4) {
			tempname[j-tempname] = '\0';
		}
	}

	/* append .srt at end  of tempname */
	strcat(tempname,".srt");

	strcpy(output,tempname);
	return;
}/* end output_name_generator */