#include "Stopgameright.h"


#include "font.h"
#include "MenuLayer.h"
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
#include "Baselisttable.h"
#include "LevelManager.h"
#include "LFStrings.h"
#include "Chaonengdaoju.h"
#include "Goluetu.h"
#include "Friendmethod.h"
#include "Shieldingmanger.h"
#include "MessageBar.h"
#include "Music.h"


Stopgameright::Stopgameright(void){


}
Stopgameright::~Stopgameright(void){

	sps.clear();
	ccbuttons.clear();

}


bool Stopgameright::init(){
	if(!CCLayer::init()){
		return false;
	}
	
	//this->setTouchEnabled(true);
	//this->setTouchPriority(-127);
	//this->setTouchMode(kCCTouchesOneByOne);
	//this->registerWithTouchDispatcher();

	
		string back_str=CCString::createWithFormat("ZJ_bg_season%d.png",LevelManager::getManager()->getLastSeasonId())->getCString();
		back_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
		CCImage *pImage = new CCImage();
		pImage->initWithImageFile(back_str.c_str(),CCImage::kFmtPng);
		getImagecolor4Bpos(pImage,ccp(1,384),LevelManager::getManager()->getLastSeasonId());
	this->setZOrder(100);
	stopAction();
	isGb=true;
	butoon=false;
	return true;
}



void Stopgameright::onEnter(){

	CCLayer::onEnter();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Stopgameright::Stopgamerightmsg), MSG_GAME_Stopright, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Stopgameright::refreshChaondj), MSG_CHAONDJBUY, NULL);

}
void Stopgameright::onExit(){

	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Stopgameright::refreshChaondj(CCObject* obj){

	//MSG_CHAONDJBUY
	if (GameInforEditer::getGameInfor()->getSuperToolCount()<=0){
		pXin->setVisible(false);
	}else if(GameInforEditer::getGameInfor()->getSuperToolCount()>0){
        pXin->setVisible(true);
        num_cndj->setString(CCString::createWithFormat("%d",GameInforEditer::getGameInfor()->getSuperToolCount())->getCString());
	}
}

void Stopgameright::Stopgamerightmsg(CCObject* object){

	 int tag=(int)(long long)object;
	 if(tag==Gamecontinu_tag){


	 }else if(tag==Gameother_tag){

		 this->setVisible(true);
		 Fadeinnodes(layer_bt);
	 }else if(tag==Gamestopxiaoshi_tag){
		 
		 layer_bt->runAction(
			   CCSequence::create(
			    CCFadeOut::create(time_action),
				CCCallFunc::create(this,callfunc_selector(Stopgameright::actionend)),
				nullptr
			 ));
		 Fadeoutnodes(layer_bt);
	 }

}
void Stopgameright::actionend(){
	 
	 this->removeFromParent();
}

void Stopgameright::fillOuty(CCMenuItemImage* p,string imgfile){

	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.10f);
	p->setSelectedImage(s);
	CCSprite* n=CCSprite::create(imgfile.c_str());
	CCLayer* layer=CCLayer::create();
	layer->setContentSize(CCSize(p->getContentSize()*1.10f));
	n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	layer->addChild(n);

	p->setNormalImage(layer);
}

void Stopgameright::stopAction(){

	//float dt=0.5f;
	//this->runAction(CCEaseBackIn::create(CCMoveTo::create(dt,CCPoint(getContentSize().width+getPositionX(),getPositionY()))));
	//



	layer_bt=CCLayer::create();
	addChild(layer_bt);

	size=CCDirector::sharedDirector()->getWinSize();
	int currentid=LevelManager::getManager()->getLastSeasonId();
	int gkId=LevelManager::getManager()->getLastSectionId();
	//ZJ_biaoshi_season2.png
	//size.width-yinx_bt->getContentSize().width/2-10
	//音效
	/*
	chaonengdaojulayer_tag,
	friendlayer_tag,
	golutulayer_tag,
	*/
	string yinxiao="ZT_anniu_chaonengdaoju.png";
	string yinxiao_select="ZT_anniu_chaonengdaoju.png";
	yinxiao=ResManager::getManager()->getFilePath(g_stop+yinxiao);
	yinxiao_select=ResManager::getManager()->getFilePath(g_stop+yinxiao_select);

	yinx_bt=CCMenuItemImage::create(
		yinxiao.c_str(),
		yinxiao_select.c_str(),
		this,
		menu_selector(Stopgameright::allCallback));
	yinx_bt->setTag(chaonengdaojulayer_tag);
	yinx_bt->setPosition(ccp(size.width*0.95f,size.height*0.28));
	ccbuttons.push_back(yinx_bt);
	fillOuty(yinx_bt,yinxiao);
    showNewChengjiuAimation(yinx_bt);

	if (GameInforEditer::getGameInfor()->getSuperToolCount()<=0){
		pXin->setVisible(false);
	}
	//音乐
	string yiny="ZT_anniu_gongluetu.png";
	yiny=ResManager::getManager()->getFilePath(g_stop+yiny);
	string yiny_select="ZT_anniu_gongluetu.png";
	yiny_select=ResManager::getManager()->getFilePath(g_stop+yiny_select);
	CCMenuItemImage* yy_bt=CCMenuItemImage::create(
		yiny.c_str(),
		yiny_select.c_str(),
		this,
		menu_selector(Stopgameright::allCallback));
	yy_bt->setTag(golutulayer_tag);
	yy_bt->setPosition(ccp(size.width*0.95f,size.height*0.15));
	ccbuttons.push_back(yy_bt);
	fillOuty(yy_bt,yiny);

	//ZT_anniu_haoyoujiefa.png
	string hyjf=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_haoyoujiefa.png");
	CCMenuItemImage* hy_bt=CCMenuItemImage::create(
		hyjf.c_str(),
		hyjf.c_str(),
		this,
		menu_selector(Stopgameright::allCallback));
	hy_bt->setTag(friendlayer_tag);
	hy_bt->setPosition(ccp(size.width*0.85f,size.height*0.215));
	ccbuttons.push_back(hy_bt);
	fillOuty(hy_bt,hyjf);
	hy_bt->setVisible(false);

	CCMenu* pMenu = CCMenu::create(yinx_bt,yy_bt,hy_bt,NULL);
	pMenu->setPosition(CCPointZero);
	layer_bt->addChild(pMenu, 1);
    
    if (BaseContentLayer::shareBaseContentLayer()->isUsingSuperTool())
    {
        yinx_bt->setVisible(false);
        yy_bt->setVisible(false);
    }

	string backcontend_str=CCString::createWithFormat("ZJ_biaoshi_season%d.png",currentid)->getCString();
	string img=ResManager::getManager()->getFilePath(g_mainlayerPath+backcontend_str);
	CCSprite* p=CCSprite::create(img.c_str());
	p->setPosition(CCPoint(getContentSize().width*0.4f,getContentSize().height*0.5f));
	layer_bt->addChild(p);
	sps.push_back(p);
	poits=p->getPosition();
    
	string Gk=CCString::createWithFormat("sd%d",currentid)->getCString();
	string gk_ms=CCString::createWithFormat(" %d-%d",currentid,gkId)->getCString();
	string msg_text=LFStrings::getValue(Gk)+gk_ms;

	CCLabelTTF* star_l = CCLabelTTF::create(msg_text.c_str(),fontStr_KlavikaBold,50);
	star_l->setPosition(CCPoint(p->getPositionX()+p->getContentSize().width/2+star_l->getContentSize().width/2,p->getPositionY()));
	layer_bt->addChild(star_l);
	sps.push_back(star_l);

	/*
	float move=(size.width-(p->getContentSize().width+star_l->getContentSize().width))/2-p->getPositionX()+(p->getContentSize().width+star_l->getContentSize().width)/2;
	for(size_t i=0;i<sps.size();i++){
		sps.at(i)->setOpacity(0);
		sps.at(i)->runAction(CCFadeIn::create(time_action));
	}*/
	Fadeinnodes(layer_bt);

	if (LevelManager::getManager()->getLastSeasonId() == 1 && LevelManager::getManager()->getLastSectionId() == 1)
	{
		yinx_bt->setVisible(false);
		yy_bt->setVisible(false);
	}

}

void Stopgameright::getImagecolor4Bpos(CCImage *p,CCPoint point,int idx){

	ccColor4B c={0,0,0,0};
	unsigned int x,y;
	x=point.x;
	y=p->getHeight()-point.y;
	int width=p->getWidth();

	unsigned char *data=p->getData();
	unsigned int *pixel=(unsigned int*)data;
	pixel=pixel+(y*width)+x;
	c.a=((*pixel)>>24)& 0xff;


	float scale =  (c.a + 1.0f) / 256 ;
	c.r = *pixel & 0xff;
	c.r = c.r / scale ;
	c.g = (*pixel >> 8) & 0xff;
	c.g = c.g / scale ;
	c.b = (*pixel >> 16) & 0xff;
	c.b = c.b / scale ;

	ccColor3B c3;
	c3.r=c.r;
	c3.g=c.g;
	c3.b=c.b;
	colors.insert(make_pair(idx,c3));

}

void Stopgameright::showNewChengjiuAimation(CCNode* pParent)
{
	pXin = CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_icon_shuzi_bg.png").c_str());
	pParent->addChild(pXin);
	pXin->setOpacity(0);
	pXin->setPosition(pParent->getContentSize()/2);
    
	num_cndj=CCLabelTTF::create(CCString::createWithFormat("%d",GameInforEditer::getGameInfor()->getSuperToolCount())->getCString(),fontStr_KlavikaBold,30);
	num_cndj->setColor(colors.at(LevelManager::getManager()->getLastSeasonId()));
	num_cndj->setPosition(CCPoint(pXin->getContentSize().width/2,pXin->getContentSize().height/2));
	pXin->addChild(num_cndj);
      
	float time = 0.16f;
	pXin->runAction(CCSequence::create(
		CCSpawn::create(
		CCRotateBy::create(time, 180), 
		CCFadeTo::create(time, 50), 
		CCMoveBy::create(time, ccp(13, 13)),
		NULL), 
		CCSpawn::create(
		CCRotateBy::create(time*2, 180), 
		CCFadeTo::create(time*2, 255), 
		CCMoveTo::create(time*2, ccp(pParent->getContentSize().width-pXin->getContentSize().width/2+3, pParent->getContentSize().height-4)),
		NULL),
		CCDelayTime::create(0.5f),
		CCCallFuncN::create(this, callfuncN_selector(Stopgameright::showNewChengjiuWaitingAimation)),
		NULL));
}


void Stopgameright::showNewChengjiuWaitingAimation(CCNode* pParent)
{
	CCSprite* pXiaoGuo = CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_xin_xiaoguo.png").c_str());
	pParent->addChild(pXiaoGuo);
	pXiaoGuo->setScale(1.2f);
	pXiaoGuo->setOpacity(125);
	pXiaoGuo->setPosition(ccp(pParent->getContentSize().width/2, pParent->getContentSize().height/2+1));

	float time = 0.8f;
	pXiaoGuo->runAction(
		CCRepeatForever::create(
		CCSequence::create(
		CCSpawn::create(
		CCFadeTo::create(time, 0),
		CCScaleTo::create(time, 1.8f),
		NULL),
		CCDelayTime::create(0.3f),
		CCSpawn::create(
		CCFadeTo::create(0, 125),
		CCScaleTo::create(0, 1.2f),
		NULL),
		NULL))
		);
}


void Stopgameright::Fadeinnodes(CCNode* node){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCFadeIn::create(time_action));
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Fadeinnodes(node);
			}
		}
	}
}

void Stopgameright::Fadeoutnodes(CCNode* node){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCFadeOut::create(time_action));
			if(node->getChildren()!=nullptr&&node->getChildren()->count()>0){
				Fadeoutnodes(node);
			}
		}
	}
}


void Stopgameright::setIsgb(bool mark){
	this->isGb=mark;
}

void Stopgameright::actionxiaoshi(){
	 
	 layer_bt->runAction(
		 CCSequence::create(
		      CCFadeOut::create(time_action),
			  CCCallFunc::create(this,callfunc_selector(Stopgameright::end)),
			  nullptr
		   )
		 );
	 Fadeoutnodes(layer_bt);
}

void Stopgameright::end(){
	 
	 butoon=false;
	 this->setVisible(false);
}
void Stopgameright::allCallback(CCObject* pSender){

	
	if(butoon==true){
		return;
	}
	butoon=true;
	Music::sharedMusicPlayer()->buttonClicked();
	CCMenuItemImage* p=(CCMenuItemImage*)pSender;
	int tag=p->getTag();

	if(tag==chaonengdaojulayer_tag){//超能道具

		//得到超能道具的数量 有就直接播放
		if (GameInforEditer::getGameInfor()->getSuperToolCount() <= 0)
		{
			//没有购买
			actionxiaoshi();
			Chaonengdaoju* daoju= Chaonengdaoju::creteBytag(false);
			CCDirector::sharedDirector()->getRunningScene()->addChild(daoju,1000);
		}
		else
		{
			//回到游戏界面，并使用超能道具
			CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USE_SUPERTOOL, NULL);
			
			int tag_msg=Gamestopxiaoshi_tag;
			CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stopright,(CCObject*)tag_msg);
			int tag_msg_z=Gamestopxiaoshi_xiaosi;
			CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag_msg_z);	
		}
	}else if(tag==golutulayer_tag){//攻略图

		/*
		if (!GameInforEditer::getGameInfor()->getHasPayOfficialSolution())
		{
			CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Stopgameright::payResult), NetMessage::MSG_PayResult, NULL);
			CmdHelper::getInstance()->buyProduct(ProductID_OfficialSolution);
			Shieldingmanger::getSielding()->setPingbingbutton(false);
			return;
		}
		int index = GameLayerFactory::getInstance()->getSectionIndex(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
		vector<vector<record> > allRecordData;
		if (!Recorder::getInstance()->getOneSectionRecord(index, &allRecordData))
		{  
			butoon=false;
			Shieldingmanger::getSielding()->setPingbingbutton(false);
			CCMessageBox("not found the OfficialSolution", "Error");
			return;
		}
		*/
		
		actionxiaoshi();
		Goluetu* gt_layer=Goluetu::creteBytag(false);
		gt_layer->drawGongluetu();
		CCDirector::sharedDirector()->getRunningScene()->addChild(gt_layer,1000);
	}else if(tag==friendlayer_tag){
		actionxiaoshi();
		Friendmethod* flayer=Friendmethod::create();
		CCDirector::sharedDirector()->getRunningScene()->addChild(flayer,1000);
	}

	if (tag==chaonengdaojulayer_tag&&GameInforEditer::getGameInfor()->getSuperToolCount() <= 0)
	{
		int tag_msg=Gamestopxiaoshi_tag;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag_msg);
		 
	}else{

		int tag_msg=Gamestopxiaoshi_tag;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag_msg);
	}
}

void Stopgameright::payResult(CCObject* pMsg)
{
	butoon = false;
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NetMessage::MSG_PayResult);
	MsgData* pdata = (MsgData*)pMsg;
	if (pdata && pdata->errCode == MsgData::eSuccess)
	{
		CCMenuItemImage iamg;
		ProductID id = *(ProductID*)pdata->data;
		if (id == ProductID_OfficialSolution)
		{
			iamg.setTag(golutulayer_tag);
		}
		allCallback(&iamg);
	}
	else
	{
		MessageBar::show(LFStrings::getValue("Purchasefailed").c_str());
		//CCMessageBox("Purchase failed", "Tip");
	}
}

void Stopgameright::actionFaihui(){

	Fadeoutnodes(layer_bt);
	for(size_t i=0;i<sps.size();i++){	
		if(i==sps.size()-1){
			sps.at(i)->runAction(
				CCSequence::create(
				CCFadeOut::create(time_action),
				CCCallFunc::create(this,callfunc_selector(Stopgameright::dealWith)),
				NULL
				));
		}else{
			sps.at(i)->runAction(CCFadeOut::create(time_action));
		}
	}
}

void Stopgameright::dealWith(){

	if(this->isGb==true){
		this->removeFromParent();
	}else{
		this->setVisible(false);
	}

}



