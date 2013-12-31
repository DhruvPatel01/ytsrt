#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct srt_time {/**< time in HH:MM:SS:mmm format*/
	short hour;	/**<  HH between 0 and 23*/
	short min;	/**<  MM between 0 and 59*/
	short sec;	/**<  HH between 0 and 59*/
	short msec;	/**<  mmm between 0 and 999*/
}srt_time_t;

struct memory_struct {
	char *memory;
	size_t size;
};

#endif
