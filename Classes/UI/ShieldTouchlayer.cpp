#include "ShieldTouchlayer.h"


ShieldTouchlayer::ShieldTouchlayer(void)
{
}


ShieldTouchlayer::~ShieldTouchlayer(void)
{
}


bool ShieldTouchlayer::init(){

	if(!CCLayer::init()){
		return false;
	}

	CCSize size=CCDirector::sharedDirector()->getWinSize();
	this->setContentSize(size);
	return true;
}
void ShieldTouchlayer::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-127,true);
}
void ShieldTouchlayer::onExit(){

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();

}

void ShieldTouchlayer::setTouchlevel(int level){

	 this->setTouchPriority(level);
}


bool ShieldTouchlayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;

}