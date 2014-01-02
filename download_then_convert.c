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
#include <curl/curl.h>
#include "ezxml.h"
#include "constants.h"
#include "structures.h"
#include "funct_declarations.h"

/* download_then_convert takes void * to target which may be url(string) or file(FILE),
 * then loops through each url if it is list(file) , it also desides the 
 * output filename if it is not given.
 * \param pointer to input (URL or FILE)
 * \param source type, SRC_STRING(url) OR SRC_FILE
 * \param void pointer to FILE which helds names or name itself
 * \param rename type RN_NO,RN_SIN,RN_MUL OR RN_FILE
 * \param interactive type IA_ALL OR IA_ONCE OR IA_NO
 * \param pointer to two character default lang code if IA_NO
 * \return 0==OK 1==ERROR
 */

int download_then_convert(void *input, int src_type, void *output, int rn_type, int ia_type, char *lang)
{
	int skip = FALSE; /* bool var to skip language selection or not */
	unsigned int nod = 2;/* no of digits tobe appended after output RN_MUL*/
	char lang_code[3];
	char lang_name[20];
	char url[100];/* will be used to store url to pass curl */
	char *url_api = "https://gdata.youtube.com/feeds/api/videos/";
	char *url_timedtext = "https://video.google.com/timedtext?type=track&v=";
	char *url_list = "https://video.google.com/timedtext?type=list&v=";

	if (ia_type == IA_NO) {
		skip = TRUE;
		if (lang != NULL)
			strcpy(lang_code, lang);
		else
			strcpy(lang_code, "en");
	}



	/* set up curl */
	char *ua = "Mozilla/5.0 (X11; Linux i686; rv:26.0) Gecko/20100101 Firefox/26.0";
	CURL *curl_handle;
	CURLcode result;

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	/* set curl_handle options which will NOT reassigned */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, ua);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);

	FILE *inListFile;
	FILE *outListFile;
	char targetLocation[MAX_SIZE];
	char outputFileName[MAX_SIZE];

	if (rn_type == RN_SIN)
		strcpy(outputFileName,output);

	if (src_type == SRC_STRING)
		strcpy(targetLocation,(char *)input);
	else
		inListFile = (FILE *)input;

	if (rn_type == RN_FILE)
		outListFile = (FILE *)output;

	unsigned short i = 0;/* index of current url */
	while (1) {
		if (src_type == SRC_FILE) {
			int exit = fscanf(inListFile,"%[^\n]\n",targetLocation);
			++i;
			if (exit == EOF)
				break;
		}

		/* parse video id */
		char video_id[12];
		if (extract_video_id(targetLocation,video_id) != 0) {
			/* i.e. that is error retriving video id */
			fprintf(stderr, "There is some problem with %s\n", targetLocation);
			if (src_type == SRC_FILE)
				continue;
			else if (src_type == SRC_STRING)
				break;
		}

		/* now retrive list of available subtitles */
		struct memory_struct available_list; /*downloaded xml page contains list*/
		available_list.memory = malloc(1);
		available_list.size = 0;

		/* generate available_list url */
		strcpy(url,url_list);
		strcat(url,video_id);

		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&available_list);

		result = curl_easy_perform(curl_handle);
		if (result != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		} else {
			ezxml_t head;/* pointer to first root tag of xml. 'transcript_list' here. */
			ezxml_t next;/* used for traversint head */

			head = ezxml_parse_str(available_list.memory,available_list.size);
			
			if (head == NULL) {
				fprintf(stderr, "\nSorry! unable to retrive list of Subtitles for %s\n",targetLocation);
				if (src_type == SRC_FILE)
					continue;
				else if (src_type == SRC_STRING)
					break;
			}

			/* make sure that parsed xml is really xml and not any html error page */
			if (strcmp(head->name,"transcript_list") != 0) {
				fprintf(stderr, "\nSorry, but something is not right, Please report "
					"about me to my owner, it seems like bug. OR link %s is wrong\n",targetLocation);
				fprintf(stderr, "\n\nFor DEBUG : URL failed to download === %s\n\n",url);
				if (src_type == SRC_FILE)
					continue;
				else if (src_type == SRC_STRING)
					break;	
			}
			
			/* check if there are any subtitles or not */
			if(head->child == NULL) {
				fprintf(stderr, "\nSorry No subtitles found for %s\n", targetLocation);
				if (src_type == SRC_FILE)
					continue;
				else if (src_type == SRC_STRING)
					break;
			}

			int i=1;
			int option;
			char line[5];

			ezxml_t temp_node = ezxml_child(head, "track");
			if (ia_type == IA_NO) {
				while (1) {
					if (temp_node == NULL) {
						fprintf(stderr, "\nSorry No Subtitles found for default lang '%s', "
							"switching to interactive once mode.\n", lang);
						skip = FALSE;
						ia_type = IA_ONCE;
						break;
					}
					const char *tmp = ezxml_attr(temp_node,"lang_code");
					if (tmp != NULL && strcmp(tmp,lang_code) == 0) {
						strcpy(lang_name,ezxml_attr(temp_node,"name"));
						break;
					}
					temp_node = temp_node->next;
				}/* end while*/
			}/* end if ia_type == IA_NO */

			skip_mode_changed:
			temp_node = ezxml_child(head, "track");
			if (skip == FALSE) {
				printf("\nFound following subtitles. Please select code to download.\n\n");
				printf("CODE\tLanguage\n");
				for (next=ezxml_child(head,"track");next;next=next->next,i++)
					printf("%d\t%s\n", i, ezxml_attr(next,"lang_translated"));

				printf("\nYour choice = ");
				fgets(line, sizeof(line), stdin);
				sscanf(line, "%d", &option);
				while (option >= i && option <= 0) {
					printf("Invalid choice. choose again = ");
					fgets(line, sizeof(line), stdin);
					sscanf(line, "%d", &option);
				}/*end while (option >= i) */

				/* get lang_code and lang_name for option or default lang*/
				i = 0;
				for(i=0;i<option-1;i++,temp_node = temp_node->next);
				const char *temp_code = ezxml_attr(temp_node,"lang_code");
				strcpy(lang_code,temp_code);
			}

			/*even if skip is true we will have to get name each time*/
			if (skip == TRUE) {
				while (1) {
					if (temp_node == NULL) {
						fprintf(stderr, "\nNo subtitles found in guessed language"
							", switching to interactive all mode.\n");
						skip = FALSE;
						goto skip_mode_changed;
					}
					const char *tmp = ezxml_attr(temp_node,"lang_code");
					if (tmp != NULL && strcmp(tmp,lang_code) == 0) {
						strcpy(lang_name,ezxml_attr(temp_node,"name"));
						break;
					}
					temp_node = temp_node->next;
				}/* end while */
			}

			/* remove spaces from name */
			const char *temp_name_ptr = ezxml_attr(temp_node,"name");
			char orig_name[MAX_SIZE];
			strcpy(orig_name,temp_name_ptr);
			char temp[MAX_SIZE];/* buffer to avoid overlapping*/
			char new[MAX_SIZE] = "";/* after removed spaces */
			char *ptr = orig_name;
		    while ((ptr = strchr(ptr,' '))!= NULL) {
		    	*ptr = '\0';
		        strcat(new,orig_name);
		        strcat(new,"%20");
		        ptr = ptr+1;
		        strcpy(temp,ptr);
		        strcpy(orig_name,temp);
		    }
		    strcat(new,orig_name);
			strcpy(lang_name,new);

			/* set for interactive once */
			if (ia_type == IA_ONCE)
				skip = TRUE;

			/* free memory */
			ezxml_free(head);
			if (available_list.memory)
				free(available_list.memory);
		
		}/* end if-else result != CURLE_OK */

		/* if output file name is not given then generate it */
		short rn_file_shortage = FALSE;/* if rn_file has less names than reqire */
		if (rn_type == RN_FILE && !rn_file_shortage) {
			int exit = fscanf(outListFile, "%[^\n]\n", outputFileName);
			output_name_generator(outputFileName,outputFileName, "");
			if (exit == EOF)
				rn_file_shortage = TRUE;
		}
		if (rn_type == RN_NO || rn_file_shortage) {
			ezxml_t vid_feed_xml; /* head of parsed xml nodes */
			struct memory_struct feed; /* downloaded xml file */
			feed.memory = malloc(1);
			feed.size = 0;

			/* generate video_feed url */
			strcpy(url,url_api);
		    strcat(url,video_id);
		    strcat(url,"?v=2");

		    /* call curl to download video feed in feed */
		    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&feed);
		    result = curl_easy_perform(curl_handle);

		    if (result != CURLE_OK) {
			    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			    if (src_type == SRC_FILE)
					continue;
				else if (src_type == SRC_STRING)
					break;
			} else {
				vid_feed_xml = ezxml_parse_str(feed.memory, feed.size);
				if (strcmp(vid_feed_xml->name,"entry") != 0) {
					fprintf(stderr, "\nSorry, but something is not right, Please report "
						"about me to my owner, it seems like bug. OR link %s is wrong\n",targetLocation);
					if (feed.memory)
						free(feed.memory);
					if (src_type == SRC_FILE)
						continue;
					else if (src_type == SRC_STRING)
						break;	
				} else {
					const char *title = ezxml_get(vid_feed_xml,"media:group",0,"media:title",-1)->txt;
					char tempname[MAX_SIZE];
					strcpy(tempname,title);
					
					int i;
					#ifdef __linux__
					for (i=0;tempname[i] != '\0';++i) {
						if (tempname[i] == 47)
							tempname[i] = '|';
					}
					#endif

					#ifdef __MING32__
					for (i=0;tempname[i] != '\0';++i) {
						if(!(isalnum(tempname[i]) || tempname[i] == ' ' || tempname[i] == '_' || tempname[i] == '-'))
							tempname[i] = '_';
					}
					#endif

					#ifdef __MING64__
					for (i=0;tempname[i] != '\0';++i) {
						if(!(isalnum(tempname[i]) || tempname[i] == ' ' || tempname[i] == '_' || tempname[i] == '-'))
							tempname[i] = '_';
					}
					#endif

					output_name_generator(tempname, outputFileName, lang_code);
					ezxml_free(vid_feed_xml);
				}
			}

			if (feed.memory)
				free(feed.memory);
		} else if (rn_type == RN_MUL) {
			strcpy(outputFileName,(char *)output);
			strtok(outputFileName, "@"); /* @ is string and no. of digit seprator.*/
			char temp1[5];
			char *temp;
			temp = strtok(NULL, "@");
			if (temp != NULL)
				nod = atoi(temp);
			else 
				fprintf(stderr, "\nYou have not provided no. of digits for"
				" --rename-multi option, so I am using 2.\n");
			sprintf(temp1,"%0*u", nod, i);
			strcat(outputFileName,temp1);
			output_name_generator(outputFileName, outputFileName, "");
		} 
		
		/* generate url to timedtext.xml */
		strcpy(url,url_timedtext);
		strcat(url,video_id);
		strcat(url,"&name=");
		strcat(url,lang_name);
		strcat(url,"&lang=");
		strcat(url,lang_code);

		/* set curl to download timedtext.xml */
		struct memory_struct timedtext;
		timedtext.memory = malloc(1);
		timedtext.size = 0;

		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &(timedtext.memory));
		result = curl_easy_perform(curl_handle);
		if (result != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
		} else {
			/* finally convert to srt */
			int success = convert_to_srt(timedtext.memory, SRC_STRING, timedtext.size, outputFileName);
			if (success) printf("\nDownloaded %s\n", outputFileName);
			if (timedtext.memory)
				free(timedtext.memory);
		}

		if(src_type == SRC_STRING)
			break;
		
	}/* end while */
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
	return 0;
}