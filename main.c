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
#include <stdlib.h>
#include <getopt.h>
#include "constants.h"
#include "structures.h"
#include "funct_declarations.h"

int main(int argc, char *argv[])
{
	static int download=0, convert=0;/*< Flags to download or convert*/
	short fromURL=0, fromFile=0, fromPlayList=0;/*< Flags for download or convert source*/
	short rename=RN_NO;
	short interactive=IA_NO;

	char *default_lang = NULL;/*< default-language of subtitle */

	char targetLocation[MAX_SIZE];/*< url given by -u or scanned from list file if -f */

	char outputFileName[MAX_SIZE] = "";/*< output file name */

	char *pl_id;/*pointer to playlist id*/

	FILE *inListFile; /*< incase of -f option */
	FILE *outListFile; /*< incase of -x option (--rename-file)*/


	int c; /*< temporary variable to parse cli options  */
	/* parse input arguments and set flags */
	while (1) {
		static struct option long_options[] = {
			{"convert", 		no_argument, 0, 'c'},
			{"download",		no_argument, 0, 'd'},
			{"from-file",		required_argument, 0, 'f'},
			{"from-url",		required_argument, 0, 'u'},
			{"interactive",		required_argument, 0, 'i'},
			{"rename-single",	required_argument, 0, 'r'},
			{"rename-multi",	required_argument, 0, 'R'},
			{"rename-file",		required_argument, 0, 'x'},
			{"default-lang",	required_argument, 0, 'l'},
			{"from-playlist",	required_argument, 0, 'p'},
			{"help",			no_argument,	   0, 'h'},
			{0, 0, 0, 0}
		};

		int opt_index = 0;
		c = getopt_long(argc, argv, "cdhf:u:i:r:R:x:l:p:", long_options, &opt_index);

		if (c == -1)
			break;

		switch (c) {
		case 'd' :
			download = 1;
			break;

		case 'c' :
			convert = 1;
			break;

		case 'f' :
			fromFile = 1;
			inListFile = fopen(optarg, "r");
			if (inListFile == NULL) {
				fprintf(stderr, "\nERROR: Can't read %s file.\n", optarg);
				exit(EXIT_FAILURE);
			}
			break;

		case 'u' :
			fromURL = 1;
			strcpy(targetLocation,optarg);
			break;

		case 'i' :
			if (strcasecmp(optarg,"all") == 0)
				interactive = IA_ALL;
			else if (strcasecmp(optarg,"once") == 0)
				interactive = IA_ONCE;
			else if (strcasecmp(optarg,"no") == 0)
				interactive = IA_NO;
			else {
				fprintf(stderr, "\nInvalid argument passed %s to -%c\n", optarg, 'i');
				fprintf(stderr, "Usage -i (all | once | none)\n");
				exit(EXIT_FAILURE);
			}
			break;

		case 'r' :
			rename = RN_SIN;
			strcpy(outputFileName,optarg);
			break;

		case 'R' :
			rename = RN_MUL;
			strcpy(outputFileName,optarg);
			break;

		case 'x' :
			rename = RN_FILE;
			outListFile = fopen(optarg,"r");
			if (outListFile == NULL) {
				fprintf(stderr, "\nERROR: Reading %s file.\n", optarg);
				exit(EXIT_FAILURE);
			}
			break;

		case 'l' :
			if (strlen(optarg)!=2) {
				fprintf(stderr, "\nERROR: Argument for -l (--default_lang) should"
					" be two character wide. 'en','hi'...\n");
				exit(EXIT_FAILURE);
			} else {
				default_lang = optarg;
			}
			break;

		case 'p':
			fromPlayList = 1;
			pl_id = optarg;
			break;

		case 'h':
			view_help();
			return 0;
			break;

		default:
			fprintf(stderr, "\n\nInvalid option. type %s --help for help\n", argv[0]);
			abort();
		}/*end switch*/
	}/*end while for parsing argv*/

	if (fromURL && fromFile) {
		fprintf(stderr, "\nSORRY!! I am confused. Where I should look for URLs"
			"you are saying from both file and url, please tell only one thing.\n");
		exit(EXIT_FAILURE);
	}

	if (!fromURL && !fromFile && !fromPlayList) {
		fprintf(stderr, "\nWhoa!! From where I download or convert, please"
			" look at my documention and tell me source from whrere"
			" I can pickup file to convert or download.\n");
		exit(EXIT_FAILURE);
	}

	/* if playlist is TRUE then download playlist to file and then make
	 * download true
	 */

	 if (fromPlayList) {
	 	if(make_playlist(pl_id, "playlist.lst") == 0){
	 		inListFile = fopen("playlist.lst","r");
	 		download = TRUE;
	 		fromFile = TRUE;
		}
	 }

	/*  if convert is true then convert
	 *	else if download is true then download
	 *	if nither is true print error.
	 *	if both are true print error.
	 */

	if (download && convert) {
		fprintf(stderr, "\nSorry!! I can't download and convert simultaneously.\n");
		return 1;
	} else if (download) {

		if (fromFile) {
			if (rename == RN_FILE)
				download_then_convert(inListFile, SRC_FILE, outListFile, RN_FILE, interactive, default_lang);
			else if (rename == RN_MUL)
				download_then_convert(inListFile, SRC_FILE, outputFileName, RN_MUL, interactive, default_lang);
			else if (rename == RN_SIN)
				fprintf(stderr, "Sorry --rename-single / -r is not supported with --from-file/-f\n");
			else if (rename == RN_NO)
				download_then_convert(inListFile, SRC_FILE, outputFileName, RN_NO, interactive, default_lang);
		} else if (fromURL) {
			if(rename == RN_FILE || rename == RN_MUL) {
				fprintf(stderr, "\nSorry --rename-multi(-R) or --rename-file(-x)"
					" are not supported in --from-url(-u) mode\n");
				return 1;
			}
			if (rename == RN_SIN)
				download_then_convert(targetLocation, SRC_STRING, outputFileName, RN_SIN, interactive, default_lang);
			else if (rename == RN_NO)
				download_then_convert(targetLocation, SRC_STRING, outputFileName, RN_NO, interactive, default_lang);
		}

	} else if (convert) {
		if (fromFile && rename == RN_SIN) {
			fprintf(stderr, "\nSorry: you can't give single name in --from-file(-f) mode\n");
			return 1;
		} else if (rename == RN_MUL) {
			fprintf(stderr, "\nSorry: --rename-multi(-R) is not supported in --convert(-c) mode\n");
			return 1;
		}
		
		while (1) {
			if (fromFile) {
				short exit = fscanf(inListFile, "%[^\n]\n", targetLocation);
				if (exit == EOF) 
					break;
			}

			short rn_file_shortage = FALSE;/* are there less name then required? */
			if (rename == RN_FILE && !rn_file_shortage) {
				short exit = fscanf(outListFile, "%[^\n]\n", outputFileName);
				if (exit == EOF)
					rn_file_shortage = TRUE;
			} else {
				output_name_generator(targetLocation, outputFileName, "");
			}

			if (convert_to_srt(targetLocation, SRC_FILE, 0, outputFileName)) {
				fprintf(stderr, "\nConverted %s to %s successfully.\n", targetLocation, outputFileName);
			}

			if (fromFile)
				continue;
			else
				break;
		} 

	} else {
		fprintf(stderr, "\nSORRY !! I am confused, why you called me"
			" If you don't want to download or convert anything.."
			"\nIf you are confused see my documention.\n");
		return 1;
	}

	if (fromFile)
		fclose(inListFile);

	if (rename == RN_FILE)
		fclose(outListFile);

	return 0;
}/* end main */

void view_help() {
	fputs(("\
\n\n\
ytsrt is used to download subtitles from YouTube\n\
videos and YouTube playlists. You can use this  tool\n\
to download subtitle for single  video  as  well  as\n\
to downlaod subtitles for whole playlist.\n\n\
NOTE: This help is not very detailed. for more details\n\
You should read README document came with this tool\n\n\
"), stdout);

	fputs(("\
Syntax:    ytsrt [OPTIONS]\n\n\
Options:\n\
  [-d | -c],\n\
  [--download | --convert]\n\
     select -d if you want to download subtitle(s) or\n\
     select -c if you want to convert XMLs into SRTs.\n\
     --download is equivalent to -d and --convert to -c\n\n\
"), stdout);

	fputs(("\
  [-f | -u | -p] argument,\n\
  [--from-file | --from-url | --from-playlist] argument\n\
     select -f(--from-file) if you want to download \n\
     more than subtitles. make the list of URLs to \n\
     videos one URL per line and then provide path \n\
     to that list file via argument\n\
     e.g. ytsrt -d -f \"\\home\\dhruv\\videolist.txt\"\n\
\n\
     select -u(--from-url) if you want to download just one\n\
     subtitle. give that URL as an argument to -u option\n\
     e.g. ytsrt -d -u \"http://www.youtube.com/watch?v=eRsGyueVLvQ\"\n\
\n\
     select -p(--from-playlist) if you want to download\n\
     more than subtitles of single playlist. give just \n\
     an id of a playlist as an argument to -p option\n\
     e.g. ytsrt -d -p PLcC6hSpO26Ks0yj86cpHBTchQqV9_b19E\n\
          ytsrt -d -p PL8B24C31197EC371C\n\n\
"), stdout);

	fputs(("\
  -i [all | once | no],\n\
  --interactive [all | once | no]\n\
     -i(--interactive) tells ytsrt to 'how to ask \n\
     which subtitle' to download if there are more \n\
     than one languages available.\n\
\n\
     if argument 'all' is given then ytsrt \n\
     will ask each time before downloading \n\
     subtitle from input list(-f) or playlist(-p)\n\
\n\
     if argument 'once' is given then ytsrt will\n\
     ask once for which language to download. \n\
     in subsequent downloads from playlist(-p) and\n\
     input list(-f) it will apply previous choice.\n\
     if no subtitle is available for previous selection\n\
     then it will ask once again.\n\
\n\
     if argument 'no' is given then ytsrt will \n\
     try to find subtitle for language set by \n\
     --default-lang(-l) option. and if found it will\n\
     download that subtitle. if not found then \n\
     it will go on '-i once' modeif -l(--default-lang)\n\
     is not set then ytsrt will download english\n\
     all time\n\
\n"), stdout);


	fputs(("\
  --rename-single, -r\n\
  --rename-multi, -R\n\
  --rename-file, -x\n\n\
     An argument given to --rename-single(-r) \n\
     will be the output name for single download.\n\
     This option will have effect on -u(--from-url)option.\n\
     e.g. ytsrt -c -u \\path\\to\\timedtext.xml\" -r \"\\path\\to\\timedtext.srt\"\n\
\n\
     In case you are downloading subtitles from\n\
     playlist or list file you should choose \n\
     -R(--rename-multi). the argument of this\n\
     option has format:\n\
\n\
     \\path\\prefix@no.of.digits\n\
\n\
     e.g if you are downloading subtitles for \n\
     video course having 28 lectures\n\
     ytsrt -d -p PL8B24C31197EC371C -R \\path\\to\\direcory\\lec@2\n\
\n\
     NOTE: You must NOT give .srt here. \n\
           lec@3.srt is invalid.\n\
\n\
     -x(--rename-file) is similar to --from-file(-f)\n\
     option. If you have created list of outputs \n\
     (as one name per line and in number as --from-file\n\
     or playlist has. provide path to that file as \n\
     an argument.\n\
\n\
     If none of above is selected then the output\n\
     name will be same as YouTube video title.\n\
     If title contains invalid characters then\n\
     they will be replaced by _\n\
\n"), stdout);

	fputs(("\
   -l,\n\
   --default-lang\n\
     -l takes two characters wide language code\n\
     as it's argument. This option will be helpful\n\
     in '-i no' mode.\n\
\n\
     if -l is set but ytsrt doesn't finds any\n\
     subtitle for your given language then it\n\
 	 will be switched to '-i once' mode.\n\
 	 and if it is not set then en is used\n\
 	 as default language.\n\
\n"), stdout);

	fputs(("\
Examples:\
\n"), stdout);
	
	fputs(("\
* To download single subtitle from Youtube.com \n\
  ytsrt -d -u \"www.youtube.com/watch?v=eRsGyueVLvQ\" -i once\n\
  ytsrt -d -u \"http://www.youtube.com/watch?v=eRsGyueVLvQ\"\n\
  ytsrt -d -u \"youtube.com/watch?v=eRsGyueVLvQ\" -i no -l en\n\
  ytsrt -d -u \"youtu.be/eRsGyueVLvQ\" -r sintel_en.srt\n\
\n\
* To download all subtitles from video course\n\
  ytsrt -p PL8B24C31197EC371C -R lec@2 -i once\n\
  #Automatic name\n\
  ytsrt -p PL8B24C31197EC371C -i once\n\
\n\
* To download subtitles from my own created list\n\
  #Autorename\n\
  ytsrt -d -f list.lst -i all\n\
  #Rename from file list\n\
  ytsrt -d -f list.lst -i no -x output.lst\n\
\n\
* To convert XML files locally\n\
  #output name will be generated from input\n\
  ytsrt -c -f local.lst\n\
\n"), stdout);

	fputs("Report bugs to dhruv.nanosoft@gmail.com\n", stdout);
}
