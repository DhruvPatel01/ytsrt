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
#include "ezxml.h"
#include "constants.h"
#include "structures.h"
#include "funct_declarations.h"

/* convert function converts input xml file or xml string to srt file
 * \param pointer to ["input file" string or input xml string]
 * \param type of input SRC_STRING or SRC_FILE
 * \param if input is SRC_STRING then size of string else anything.
 * \param pointer to string containing output file may NOT NULL.
 * \return 0!=OK 1==OK
 */

int convert_to_srt(char *input, int type, int size, const char *output)
{
  ezxml_t transcript; /**< Pointer to head of parsed data_structure */
  ezxml_t text; /**< pointer to text node(currently being proceesed) of xml file */

  FILE *outputfile = fopen(output,"w");
  if (outputfile == NULL) {
    fprintf(stderr, "Unable to create %s, please check you have enough permissions.\n", output);
    return 0;
  }

  double xtime; /*< xml time */
  double xml_dur; /*< xml duration */
  srt_time_t stime; /*< srt time */

  /* Parse xml file or string and store it at transcript */
  if (type == SRC_FILE)
    transcript = ezxml_parse_file(input);
  else if (type == SRC_STRING)
    transcript = ezxml_parse_str(input, (size_t)size);

  int i = 1;
  for (text = ezxml_child(transcript,"text");text;text = text->next,++i) {
    xtime = atof(ezxml_attr(text,"start"));
    
    const char *temp = ezxml_attr(text,"dur");
    if (temp == NULL) {
      ezxml_t next_text = text->next;
      if (next_text == NULL)
        xml_dur = 10;
      else
        xml_dur = atof(ezxml_attr(next_text,"start")) - xtime;
    } else {
      xml_dur = atof(ezxml_attr(text,"dur"));
    }

    /*convert xml start time to srt start time structure*/
    convert_time(xtime,&stime);

    /*print the converted xml to srt file*/
    fprintf(outputfile, "%d\n", i);
    fprintf(outputfile, "%02d:%02d:%02d,%03d", stime.hour,stime.min,stime.sec,stime.msec);
    fprintf(outputfile, " --> ");

    #ifdef DEBUG
    fprintf(stdout, "%d\n", i);
    fprintf(stdout, "%02d:%02d:%02d,%03d", stime.hour,stime.min,stime.sec,stime.msec);
    fprintf(stdout, " --> ");
    #endif

    /*convert xml stop time to srt stop time structure*/
    convert_time(xtime+xml_dur,&stime);

    /*print the converted xml to srt file*/
    fprintf(outputfile, "%02d:%02d:%02d,%03d\n", stime.hour,stime.min,stime.sec,stime.msec);

    #ifdef DEBUG
    fprintf(outputfile, "%02d:%02d:%02d,%03d\n", stime.hour,stime.min,stime.sec,stime.msec);
    #endif

    /*print content of "text" tag */
    fprintf(outputfile, "%s\n", text->txt);

    fprintf(outputfile, "\n");

    #ifdef DEBUG
    fprintf(outputfile, "%s\n", text->txt);
    fprintf(outputfile, "\n");
    #endif

  }/*end for*/
  ezxml_free(transcript);
  fclose(outputfile);
  return 1;
}
