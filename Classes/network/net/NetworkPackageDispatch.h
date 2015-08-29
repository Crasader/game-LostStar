#pragma once

#include "cocos2d.h"
#include "NetworkThreadProc.h"
using namespace cocos2d;

enum NetErrorCode
{
	NetSuccess = 0,
	NetError = 1,
	NetTimeOut = 2,
	NetDataError = 3,
};

class NetworkMessageListener
{
public:
	virtual void onMessageProcess(NetErrorCode, MessageCmd, void*, unsigned int) = 0;
};



class NetworkPackageDispatch : public CCNode
{
private:
	static NetworkPackageDispatch* m_packageDispatch;
	NetworkPackageDispatch();
	void initNode();
public:
	static NetworkPackageDispatch* getInstance();
	~NetworkPackageDispatch();
	void setMsgProc(NetworkMessageListener* listener);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void update(float dt) override;
	void messageDispatch(NetErrorCode errCode, RespondPackage* package=nullptr);
	//void onTimeOut(EventCustom* event);
	void onTimeOut(CCObject*);

private:
	NetworkMessageListener* m_msgProc;
};