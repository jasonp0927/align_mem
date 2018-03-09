/*
***********************************************************************************************************************
* File       : local_timer.cpp
* Designed by: ChengHaibo
* Version    : V3.00
***********************************************************************************************************************
* History:
*     1. 2017.11.17 detach from Qt Frame
*/
#include <string.h>
#include <stdio.h>
#include "local_timer.h"

#define  TOTAL_FRAME 100

local_timer gTick;

local_timer::local_timer()
{
	// TODO Auto-generated constructor stub
	ms_elapse = 0;
	memset(&ts, 0, sizeof(ts));
	memset(&te, 0, sizeof(te));

	count = TOTAL_FRAME;
    prev = .0;

#ifdef LINUX
	frq = CLOCKS_PER_SEC;
#else
	LARGE_INTEGER  large_interger;
	QueryPerformanceFrequency(&large_interger);
	frq = (double)large_interger.QuadPart;
#endif
}


local_timer::~local_timer()
{
	// TODO Auto-generated destructor stub
}


void local_timer::start(void)
{
#ifdef LINUX
	gettimeofday(&ts,NULL);
#else
	LARGE_INTEGER  large_interger;
	QueryPerformanceCounter(&large_interger);
	ts = large_interger.QuadPart;
#endif
}


void local_timer::start(int time_out)
{
	start();
	dead_line = time_out;
}


void local_timer::stop(void)
{
#ifdef LINUX
	gettimeofday(&te,NULL);
	double tt = 1000000 * (te.tv_sec - ts.tv_sec) + te.tv_usec - ts.tv_usec;
	ms_elapse = tt / 1000.;
#else
	LARGE_INTEGER  large_interger;
	QueryPerformanceCounter(&large_interger);
	te = large_interger.QuadPart;
	double tt = (double)(te - ts);
	ms_elapse = tt*1000.0 / frq;
#endif
}


double local_timer::ms_get(void)
{
	return ms_elapse;
}


void local_timer::tick_tok(const char *str)
{
    stop();
    printf("%s used:%f\r\n", str, ms_elapse - prev);
    prev = ms_elapse;
}


void local_timer::get_frame_rate(const char *str)
{
	float frame_rate = 0.f;
	count -= 1;

	if ( count <= 0 ) {
		stop();
		frame_rate = 1000.f / (ms_elapse / TOTAL_FRAME);
		count = TOTAL_FRAME;
		start();
		printf("%s frame rate:%f\r\n", str, frame_rate);
	}
}


void local_timer::get_frame_rate(const char *str,int* counter)
{
	float frame_rate = 0.f;
	*counter -= 1;

	if ( *counter <= 0 ) {
		stop();
		frame_rate = 1000.f / (ms_elapse / TOTAL_FRAME);
		*counter = TOTAL_FRAME;
		start();
		printf("%s frame rate:%f\r\n", str, frame_rate);
	}
}


void local_timer::out(const char * str)
{
	stop();
	printf("%s used:%f\r\n", str, ms_elapse);
}


void local_timer::cur_time()
{
	timeval tmp;
	gettimeofday(&tmp, NULL);

	int total_sec = (int)tmp.tv_sec;
	int hour   = (total_sec / 3600) % 24;
	int minite = (total_sec % 3600) / 60;
	int sec = total_sec % 60;
	int ms  = (int)(tmp.tv_usec / 1000);

	printf("%d:%d:%d:%d", hour, minite, sec, ms);
	return;
}


int local_timer::check_vanish()
{
	stop();
	return int(ms_elapse > dead_line);
}

