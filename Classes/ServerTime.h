#pragma once

#include <time.h>

// 服务器时间
#include <ctime>

class ServerTime
{
private:
	static int time0_Offset;
	static bool inited;
public:

	static void setUninited();
	static bool isInited();

	static void setCurrentServerTime(unsigned long _s_time);
	// 获取时间的字符串表示
	static char* getTimeInString(unsigned long _s_time);
	// 获取大致的服务器时间
	static unsigned long getTime();
	// 
	static tm* getTime_tm();

	static unsigned long getDistance(unsigned long t1,unsigned long t2);

	ServerTime(void);
	~ServerTime(void);
};

