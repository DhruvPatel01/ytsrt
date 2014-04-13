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
#include <curl/curl.h>
#include "ezxml.h"
#include "structures.h"
#include "constants.h"
#include "funct_declarations.h"

/* make_playlist : creates playlist file from id and saves file on harddisk 
 * \param id: unique playlist id
 * \param name_of_file: name of the playlist file created 
 * \return 1==!OK 0==OK
*/

int make_playlist(char *id,char *name_of_file)
{
	short is_next = 0;/*true if there is next page*/
	char pl_id[50];
	strcpy(pl_id,id);

	FILE *outFile = fopen(name_of_file,"w");
	if (outFile == NULL) {
		fprintf(stderr, "\nERROR: Failed to create file with name %s\n", name_of_file);
		return 1;
	}

	char src[MAX_SIZE] = "https://gdata.youtube.com/feeds/api/playlists/";
	strcat(src,pl_id);
	strcat(src,"?v=2");

	curl_global_init(CURL_GLOBAL_ALL);
	CURLcode result;

	CURL* handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, FALSE);

	struct memory_struct downloaded_data;
	downloaded_data.memory = malloc(1);
	downloaded_data.size = 0;

	ezxml_t head, link, entry;

	while (1) {
		curl_easy_setopt(handle, CURLOPT_URL, src);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&downloaded_data);
		
		result = curl_easy_perform(handle);
		if (result != CURLE_OK) {
			fprintf(stderr, "\nERROR: Failed to download %s, curl_easy_perform() failed\n", curl_easy_strerror(result));
			return 1;		
		}

		head = ezxml_parse_str(downloaded_data.memory, downloaded_data.size); 

		/* check if retrived/parsed file is valid */
		if (head == NULL) {
			fprintf(stderr, "\nERROR: Something went wrong. Make sure %s is OK.\n", src);
			return 1;
		}
		if (strcmp(head->name,"feed") !=0 ) {
			fprintf(stderr, "\nERROR: Something went wrong. Make sure %s is OK.\n", src);
			return 1;	
		}

		/* retrive link to next page */
		link = ezxml_child(head, "link");
		for (;link; link = link->next) {
			const char *attr = ezxml_attr(link, "rel");
			if (strcmp(attr,"next") == 0) {
				const char *next_link = ezxml_attr(link, "href");
				strcpy(src, next_link);
				is_next = 1;
				break;
			}
			is_next = 0;
		}/*end for*/

		/* parse each entry tag */
		for (entry = ezxml_child(head, "entry");entry;entry = entry->next) {
			link = ezxml_child(entry,"link");
			for (;link; link = link->next) {
				const char *tmp = ezxml_attr(link,"rel");
				if (strcmp(tmp,"alternate") == 0) {
					const char *temp = ezxml_attr(link,"href");
					if (temp != NULL)
						fprintf(outFile, "%s\n", temp);
					break;
				}
			}/*end for*/
		}/*end for -for parsing entry */

		/* now free the memory downloaded_data */
		if (downloaded_data.memory)
			free(downloaded_data.memory);
		downloaded_data.memory = malloc(1);
		downloaded_data.size = 0;

		if (head)
			ezxml_free(head);

		if (!is_next)
			break;
	}
	curl_easy_cleanup(handle);
	curl_global_cleanup();
	fclose(outFile);
	printf("\nSUCCESS: Playlist created successfully.\n");
	return 0;
}
