#include "TimeoutMonitor.h"


TimeoutMonitor* TimeoutMonitor::mTimeMonitor = nullptr;

TimeoutMonitor::TimeoutMonitor() 
{
	this->init();
	this->onEnter();
	this->onEnterTransitionDidFinish();
}

TimeoutMonitor* TimeoutMonitor::getInstance()
{
	if (mTimeMonitor == nullptr)
	{
		//MessageBox("new time monitor", "");
		mTimeMonitor = new TimeoutMonitor();
	}
	return mTimeMonitor;
}

void TimeoutMonitor::startMonitor(int msgCmd, float time_wait)
{
	m_curCmd = msgCmd;
	CCLOG("%s:time_wait = %f",__FUNCTION__,time_wait);
	unschedule(schedule_selector(TimeoutMonitor::timeOutCallback));
	schedule(schedule_selector(TimeoutMonitor::timeOutCallback),time_wait,0,0);
}

void TimeoutMonitor::cancleMonitor()
{
	CCLOG("%s",__FUNCTION__);
	unschedule(schedule_selector(TimeoutMonitor::timeOutCallback));
}

void TimeoutMonitor::timeOutCallback(float t)
{
	CCLOG("TimeoutMonitor::---------------TimeOut-----------");
	unschedule(schedule_selector(TimeoutMonitor::timeOutCallback));
	NetworkThreadProc::getInstance()->closeNetwork();

	//CCDirector::getInstance()->getEventDispatcher()->dispatchCustomEvent(MSG_WAIT_TIME_OUT, nullptr);

	CCNode *obj = new CCNode();
	obj->setTag(m_curCmd);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WAIT_TIME_OUT,obj);
	delete obj;
}