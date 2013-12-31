#ifndef FUNCT_DECLARATIONS
#define FUNCT_DECLARATIONS

int extract_video_id(char *target, char *dest);
int convert_time(double xmlTime,srt_time_t *srtTime);
int convert_to_srt(char *input, int type, int size, const char *output);
void output_name_generator(const char *input, char *output, char *lang);
int extract_video_id(char *target,char *dest);
int download_then_convert(void *input, int src_type, void *output, int rn_type, int ia_type, char *lang);
size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp);
int make_playlist(char *url,char *name_of_file);
void view_help();

#endif
