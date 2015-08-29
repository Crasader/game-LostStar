#include "ServerTime.h"
#include "cocos2d.h"

using namespace cocos2d;

int ServerTime::time0_Offset = 0;
bool ServerTime::inited = false;

ServerTime::ServerTime(void)
{
}

ServerTime::~ServerTime(void)
{
}

void ServerTime::setUninited()
{
	inited = false;
}
bool ServerTime::isInited()
{
	return inited;
}

void ServerTime::setCurrentServerTime(unsigned long _s_time)
{
	unsigned long clientTime = time(0);
	if (_s_time > clientTime)
	{
		time0_Offset = int(_s_time - clientTime);
	}else
	{
		time0_Offset -= int(clientTime - _s_time);
	}
	inited = true;
}

char* ServerTime::getTimeInString(unsigned long _s_time)
{
	time_t timep = _s_time; 
	/*用gmtime将time_t类型的时间转换为struct tm类型的时间按， 
	 然后再用asctime转换为我们常见的格式 Fri Jan 11 17:25:24 2008 
	*/ 
	return asctime(gmtime(&timep));
}

// 获取大致的服务器时间
unsigned long ServerTime::getTime()
{
	return time(0); // 测试

	//CCAssert(inited,"");
	//unsigned long clientTime = time(0);
	//return clientTime + time0_Offset;
}

tm* ServerTime::getTime_tm()
{
	time_t timep = getTime(); 
	return gmtime(&timep);
}
unsigned long ServerTime::getDistance(unsigned long t1,unsigned long t2)
{
	return t1>t2 ? t1-t2 : t2-t1;
}