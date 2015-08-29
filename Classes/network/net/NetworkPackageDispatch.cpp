#include "NetworkPackageDispatch.h"
#include "NetworkThreadProc.h"
#include "TimeoutMonitor.h"


NetworkPackageDispatch* NetworkPackageDispatch::m_packageDispatch = nullptr;

NetworkPackageDispatch::NetworkPackageDispatch()
:m_msgProc(nullptr)
{
}

NetworkPackageDispatch::~NetworkPackageDispatch()
{

}

NetworkPackageDispatch* NetworkPackageDispatch::getInstance()
{
	if (! m_packageDispatch) {
		m_packageDispatch = new NetworkPackageDispatch;
		m_packageDispatch->initNode();
	}
	return m_packageDispatch;
}

void NetworkPackageDispatch::setMsgProc(NetworkMessageListener* func)
{
	m_msgProc = func;
}

void NetworkPackageDispatch::initNode()
{
	CCNode::init();

// 	EventListenerCustom *listener = EventListenerCustom::create(MSG_WAIT_TIME_OUT, std::bind(&NetworkPackageDispatch::onTimeOut, this, std::placeholders::_1));
// 	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	CCNotificationCenter *_sharedCenter = CCNotificationCenter::sharedNotificationCenter();
	_sharedCenter->addObserver(this, callfuncO_selector(NetworkPackageDispatch::onTimeOut), MSG_WAIT_TIME_OUT, NULL);

	// Let's running
	this->onEnter();
	this->onEnterTransitionDidFinish();
}

void NetworkPackageDispatch::onEnter()
{
	CCNode::onEnter();
	this->scheduleUpdate();
}

void NetworkPackageDispatch::onExit()
{
	CCNode::onExit();
}

void NetworkPackageDispatch::update(float dt)
{
	RespondPackage* data = NetworkThreadProc::getInstance()->getRecvData(); //remember delete
	if (! data) return;

	switch (data->getType())
	{
		// none --- recv err
		// 全当作超时处理
	case RespondPackage::Type_None:
		break;
	case RespondPackage::Type_Conncet_Suc:
		CCLOG("%s ---> connect success", __FUNCTION__);
		break;
	case RespondPackage::Type_Conncet_Err:
		CCLOG("%s ---> connect error", __FUNCTION__);
		messageDispatch(NetError);
		break;
	case RespondPackage::Type_Send_Err:
		CCLOG("%s ---> send error", __FUNCTION__);
		messageDispatch(NetError);
		break;
	case RespondPackage::Type_Recv_Err:
		CCLOG("%s ---> recv error", __FUNCTION__);
		//服务器长时间未收到客户端消息后，会自动断开，此时客户端也会收到recv error！该消息不用处理
		messageDispatch(NetDataError);
		break;
	case RespondPackage::Type_Recv_OK:
		messageDispatch(NetSuccess,data);
		break;
	default:
		CCLOG("%s ---> error branch", __FUNCTION__);
		break;
	}

	delete data;
}

void NetworkPackageDispatch::messageDispatch(NetErrorCode errCode, RespondPackage* package)
{
	TimeoutMonitor::getInstance()->cancleMonitor();

	std::unique_lock<std::mutex> lock;  //析构会unlock
	std::queue<RequestPackage*>* alreadySendQueue = NetworkThreadProc::getInstance()->getAlreadySendQueue(lock);

	if (m_msgProc)
	{
		if (errCode == NetSuccess)
		{
			SPHead* pHead = (SPHead*)package->getHead();
			//bug: 2015/03/13 success的时候可能会出现alreadySendQueue为empty (unresolved) 
			if (alreadySendQueue && !alreadySendQueue->empty())
			{
				RequestPackage* temp = alreadySendQueue->front();
				alreadySendQueue->pop();
				temp->release();
				//现在不再检测已发队列，因为server是基于功能回复的，所以接收包可能乱序，alreadySendQueue作用只用来在error是通知所有的 网络请求ui
				//MessageCmd rspcmd = getRspcmdByReqcmd(temp->getCmdType());
				//CCAssert(pHead->cmd == rspcmd, "乱序");
			}
			m_msgProc->onMessageProcess(NetSuccess, (MessageCmd)pHead->cmd, package->getBody(), pHead->data_len-sizeof(unsigned int));  // 除去校验码
		}
		else
		{
			while(! alreadySendQueue->empty())
			{
				RequestPackage* temp = alreadySendQueue->front();
				alreadySendQueue->pop();
				m_msgProc->onMessageProcess(errCode, getRspcmdByReqcmd(temp->getCmdType()), 0, 0);
				temp->release();
			}
		}
	}
}

//void NetworkPackageDispatch::onTimeOut(EventCustom* event)
//{
//	//网络请求错误
//	m_msgProc(1, 0, 0, 0);
//}
void NetworkPackageDispatch::onTimeOut(CCObject* obj)
{
	//20150325
	//超时会强制关闭连接， 此时会有recv error消息发错， 那么这里就不用发送超时的消息了。

// 	if (m_msgProc){
// 		MessageCmd cmd = RspCmdEnd;
// 		if (dynamic_cast<CCNode*>(obj)) cmd = (MessageCmd)dynamic_cast<CCNode*>(obj)->getTag();
// 		m_msgProc->onMessageProcess(NetTimeOut, cmd, 0, 0);
// 	}
}