#include "Shieldingmanger.h"

#include "Uimsg.h"
#include "BaseScense.h"

USING_NS_CC;

Shieldingmanger* Shieldingmanger::manager = nullptr;
Shieldingmanger::Shieldingmanger(void):
	mHorizontalActive(true)
{
	this->hot=false;
	this->tag=100;
	this->continuousClick_mark=true;
	this->onethetime=0.0f;
	this->wcPingbing_button=false;
	

}

Shieldingmanger::~Shieldingmanger(void)
{
}

Shieldingmanger* Shieldingmanger::getSielding()
{
	if (! manager){
		manager = new Shieldingmanger();

	}
	return manager;
}

bool Shieldingmanger::isHorizontalActive()
{
	return mHorizontalActive;
}

void Shieldingmanger::setHorizontalActive(bool h)
{
	mHorizontalActive = h;
}

void Shieldingmanger::setCurrentlayertag(int tag){
	  this->tag=tag;
	  if(tag==Bigcheckpoint_tag){
		   CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LAGUAGEBig,(CCObject*)"0");
	  }else if(tag==Smalllayer_tag){
		  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LAGUAGESmall,(CCObject*)"0");
	  }
}
int Shieldingmanger::getCureentlayertag(){
	return tag;
}

bool Shieldingmanger::isNomainhot(){
	 return hot;
}
void Shieldingmanger::setNomainhot(bool hot){
	 this->hot=hot;
}
void Shieldingmanger::setPingbingbutton(bool wcPingbing){
	this->wcPingbing_button=wcPingbing;
}
bool Shieldingmanger::isPinbingbutton(){
	return wcPingbing_button;
}


void Shieldingmanger::Fadeoutnodes(CCNode* node){

	
	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			if (node->getTag() == Tag_Layer_PlayRecord)
			{
				continue;
			}
			node->runAction(CCFadeOut::create(chaonengdj_time));
			if(node->getChildren()!=nullptr&&node->getChildren()->count()>0){
				Fadeoutnodes(node);
			}
		}
	}
}

void Shieldingmanger::Fadeinnodes(CCNode* node){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			if (node->getTag() == Tag_Layer_PlayRecord)
			{
				continue;
			}
			node->runAction(CCFadeIn::create(chaonengdj_time));
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Fadeinnodes(node);
			}
		}
	}
}

bool Shieldingmanger::continuousThetimer(){

	 if(this->continuousClick_mark==true){//可点击 计时开始
		 this->continuousClick_mark=false;
		 return true;
	 }else{
		 return false;
	 }
}
void Shieldingmanger::setcontinuousThetimermark(bool mark=true){
	 
	 this->continuousClick_mark=mark;
}

void Shieldingmanger::continuousThetimer_schedule(){
	 
		 this->onethetime=0.0f;
		 this->numtime=0.0f;
		 this->continuousClick_mark=true;
	 
}

int Shieldingmanger::DaySearch(){

	//获得年月日
	struct tm *tm;  
	time_t timep; 

	//time(null);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	time(&timep); 
#else
	struct timeval now;    
	gettimeofday(&now, NULL); 
	timep=now.tv_sec; 
#endif

	tm = localtime(&timep); 

	int year = tm->tm_year + 1900; 
	int month = tm->tm_mon + 1; 
	int day = tm->tm_mday; 
	int hour=tm->tm_hour; 
	int min=tm->tm_min; 
	int second=tm->tm_sec; 

	return 0;
}

long Shieldingmanger::getCurrentTime()  
{   
	struct cc_timeval tv;   
	CCTime::gettimeofdayCocos2d(&tv, NULL);
	long time=tv.tv_sec * 1000 + tv.tv_usec / 1000;   
	return time;
} 
