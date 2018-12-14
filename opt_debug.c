#include "./common.h"
#include <time.h>
#include <sys/time.h>

static struct timeval ori_time;
static struct timeval pre_time; 

void init_op_debug()
{
	/* get time */
	gettimeofday(&ori_time, NULL);
	gettimeofday(&pre_time, NULL);
}

void op_debug(const char *prompt)
{
	struct timeval cur_time;
	/* get time */
	gettimeofday(&cur_time, NULL);


	long sec_interval = cur_time.tv_sec  - pre_time.tv_sec;
	long usec_interval = cur_time.tv_usec - pre_time.tv_usec;
	

	if(usec_interval < 0)
	{
		sec_interval -= 1;
		usec_interval += 1000000;
	}

	pre_time = cur_time;

	printf("%s, consume %ld.%06ld\n", prompt, sec_interval, usec_interval);
}