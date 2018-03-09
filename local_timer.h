#ifndef LOCAL_TIMER_H
#define LOCAL_TIMER_H

#include <time.h>

#ifdef LINUX
#include <sys/time.h>
#else
#include <windows.h>
#endif

class local_timer
{
private:
#ifdef LINUX
    struct timeval ts, te;
#else
    __int64 ts, te;
#endif
	double  frq, ms_elapse, prev, dead_line;
	int count;

public:
	void start();
	void start(int time_out);
	void stop();
	
	void out(const char * str);
    void tick_tok(const char * str);
	
	void get_frame_rate(const char *str);
	void get_frame_rate(const char *str,int* counter);
	void cur_time();
	double ms_get();

	virtual ~local_timer();
	local_timer();
	int check_vanish();
};
#endif
