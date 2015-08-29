#pragma once
#include "cocos2d.h"
#include "NetworkThreadProc.h"

using namespace cocos2d;

// 会发送这个消息通知已经超时了
#define  MSG_WAIT_TIME_OUT	"WAIT_TIME_OUT"

// 最多等20秒钟
#define		MAX_TIME_WAIT	30.0f

class TimeoutMonitor : public CCNode
{
private:
	TimeoutMonitor();

	static TimeoutMonitor* mTimeMonitor;
	int m_curCmd;


public:
	static TimeoutMonitor* getInstance();

	// 开始监视，超时之后自动断连接
	void startMonitor(int msgCmd, float time_wait = MAX_TIME_WAIT);

	void cancleMonitor();

	void timeOutCallback(float t);
};
