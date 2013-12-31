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

/* extract_video_id extracts 11 char. youtube id from url
 *  \param target is pointer to url
 *  \param dest is pointer where id will be stored
 *  \return 0=OK 1=ERROR
 */

int extract_video_id(char *target,char *dest)
{
  int i,count=0;
  if (strstr(target,"http://"))
    count += 7;

  else if (strstr(target,"https://"))
    count += 8;

  if (strstr(target,"www."))
    count += 4;

  if (strstr(target,"youtube.com")) {
    if (!strstr(target,"watch?v=")) {
      fprintf(stderr, "Invalid url %s\n", target);
    } else {
      count += 11;//for youtube.com
      count += 9; //for /watch?v=
    }
  } else if (strstr(target,"youtu.be")) {
    count += 9; //for youtu.be/
  } else {
    fprintf(stderr, "Invalid URL %s\n", target);
    return 1;
  }

  for(i=0;i<11;i++,count++)
    dest[i]=target[count];
  dest[11] = '\0';
  return 0;
}