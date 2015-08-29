#include "Starmain.h"
#include "LFStrings.h"
#include "ResManager.h"
#include "font.h"
#include "Uimsg.h"
#include "Myobject.h"
#include "Stopgame.h"
#include "Baselisttable.h"
#include "Starseneui.h"
#include "LevelManager.h"
#include "CmdHelper.h"
#include "LFUtile.h"
#include "GameLayerFactory.h"
#include "AchievementManager.h"
#include "MessageBar.h"
#include "Music.h"
#include "VersionControl.h"

//#define UI_mainaramite_name "STAR_FALL_UI_zhuye.ExportJson"   
//#define UI_animation_data  "STAR_FALL_UI_zhuye"

using std::string;

enum Maintag{

	armrate_chuxiantag=1000,
	back_tag,

	new_achiev_tag,
	connect_tag,
};

Starmain::Starmain(void)
{
}


Starmain::~Starmain(void)
{
//	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ResManager::getManager()->getFilePath(UI_mainaramite_name).c_str());
}


bool Starmain::init(){
	 
	 if(!CCLayer::init()){
		 return false;
	 }
	 facebooklogin = NULL;
	 facebooklogoutbutton = NULL;
	 pLabel = NULL;
	 pLabelm = NULL;

	 if (LevelManager::getManager()->getStartApp())
	 {
		 LevelManager::getManager()->setStartApp(false);
		 m_iTempSeaonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
		 LevelManager::getManager()->setLastSeasonId(m_iTempSeaonId);
	 }
	 else
	 {
		 m_iTempSeaonId = LevelManager::getManager()->getLastSeasonId();
	 }
	 int iCount = GameLayerFactory::getInstance()->getValidSeasonNum();
	 for(int i=1;i<=iCount;i++){
		 int index = i;
		 string back_str=CCString::createWithFormat("ZJ_bg_season%d.png",index)->getCString();
		 back_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
		 CCImage *pImage = new CCImage();
		 pImage->initWithImageFile(back_str.c_str(),CCImage::kFmtPng);
		 getImagecolor4Bpos(pImage,ccp(1,384),index);
	 }
	 /*
	 ResManager* resManager = ResManager::getManager();
	 CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	 size=winSize;
	 width=winSize.width;
	 hight=winSize.height;
	 string back_str = CCString::createWithFormat("ZY_bg_%d.png", m_iTempSeaonId)->getCString();
	 back_str = resManager->getSharedFilePath(g_mainlayerPath+back_str);
	 mBG = CCScale9Sprite::create(back_str.c_str());
	 mBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
	 mBG->setPosition(CCDirector::sharedDirector()->getWinSize()/2);
	 this->addChild(mBG, -1, back_tag);
	 string imgname = resManager->getSharedFilePath(g_buttonPath+"ZY_anniu_kaishiyouxi.png");
	 mStartItem = CCMenuItemImage::create(
		 imgname.c_str(),
		 imgname.c_str(),
		 this,SEL_MenuHandler(&Starmain::startItemClicked));
//	 mStartItem->setScale(2.0f);
	 mStartItem->setPosition(ccp(winSize.width/2, winSize.height*0.48f));
	 mStartItem->setOpacity(0);

	 CCMenu* starMenu = CCMenu::create(mStartItem,NULL);
	 starMenu->setPosition(CCPointZero);
	 addChild(starMenu,129);
	 //facebook
	 //Buttonfadein();
	 //动画播放
	 CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ResManager::getManager()->getFilePath(UI_mainaramite_name).c_str());
	 CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(ResManager::getManager()->getFilePath(UI_mainaramite_name).c_str()); //,this, schedule_selector(Starmain::dataLoaded));
	 dataLoaded(1);
	 */
	 CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	 size=winSize;
	 width=winSize.width;
	 hight=winSize.height;

	 string back_str = CCString::createWithFormat("ZY_bg_%d.png", m_iTempSeaonId)->getCString();
	 back_str = ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
	 mBG = CCScale9Sprite::create(back_str.c_str());
	 mBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
	 mBG->setPosition(CCDirector::sharedDirector()->getWinSize()/2);
	 this->addChild(mBG, -1, back_tag);

	 //this->schedule(schedule_selector(Starmain::playaction),2.0f);
	 drawupdatelayer();
	 actionlayer();

	 refreshlog();

	 return true;
}  
void Starmain::onEnter(){
	CCLayer::onEnter();	 
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Starmain::changeBGColor), MSG_UI_ChANGE_BG_COLOR, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Starmain::facebookloginshuangxin),NetMessage::MSG_LoginResult, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Starmain::refreshBgColor),NetMessage::MSG_SyncUserDataEnd, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Starmain::refreshLogoutButton),NetMessage::MSG_GotSeasonSocialInfo, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Starmain::refreshLuaguge),MSG_LAGUAGEMAIN, NULL);

	
}
void Starmain::onExit(){
	 
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	CCLayer::onExit();

}

void Starmain::getImagecolor4Bpos(CCImage *p,CCPoint point,int idx){

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

void Starmain::refreshLuaguge(CCObject* object){

	if (facebooklogin)
	{
		if (CCLabelTTF* pTTF = (CCLabelTTF*)facebooklogin->getChildByTag(connect_tag))
		{
			pTTF->setString(LFStrings::getValue("connect").c_str());

			if (ResManager::getManager()->getLanguageType() == kLanguageGerman)
			{
				pTTF->setFontSize(40);
			}
			else
			{
				pTTF->setFontSize(50);
			}
		}
	}
	 
	 if(pLabel){
	   pLabel->setString(LFStrings::getValue("play").c_str());
	   pLabel->setColor(colors.at(LevelManager::getManager()->getLastSeasonId()));
	 }
	 if(pLabelm){

	   pLabelm->setString(LFStrings::getValue("miaoshu").c_str());
	   float hight_l=0.0f;
	   if(LFStrings::getValue("miaoshu1").size()>0){
		   hight_l=pLabelm->getContentSize().height/2;
	   }
	   pLabelm->setPosition(ccp(pLabelm->getPositionX(),hight_l));

	   pLabelm_x->setString(LFStrings::getValue("miaoshu1").c_str());
	   pLabelm_x->setPosition(ccp(pLabelm_x->getPositionX(),-10.0f));


	 }

	 if(spbt){

		 string byluangue_spstr=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_biaoti_fubiaoti.png", true);
		 spbt->setTexture(CCSprite::create(byluangue_spstr.c_str())->getTexture());
	 }
}

bool Starmain::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void Starmain::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	
}
void Starmain::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Starmain::fillOuty(CCMenuItemImage* p,string imgfile){


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

void Starmain::setClicked(CCObject* obj){

	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}
	Music::sharedMusicPlayer()->buttonClicked();
	Myobject* object=new Myobject();
	object->mark=true;
	object->parent_id=Starmain_tag;
	object->current_id=Gamesetlayer_tag;
	object->pCurNode = this;
	object->autorelease();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE, (CCObject*)object);

}
void Starmain::achiveClicked(CCObject* obj){
	
	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}
	Music::sharedMusicPlayer()->buttonClicked();
	AchievementManager::getInstance()->setHaveNew(false);
	removeNewChengjiuAimation((CCNode*)obj);
	Myobject* object=new Myobject();
	object->mark=true;
	object->parent_id=Starmain_tag;
	object->current_id=Achievelayer_tag;
	object->pCurNode = this;
	object->autorelease();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE, (CCObject*)object);

}
void Starmain::gamecenterClicked(CCObject* obj){

	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}
	Music::sharedMusicPlayer()->buttonClicked();
}

void Starmain::facebookClicked(CCObject* obj){

	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}
	CmdHelper::getInstance()->login();
	Music::sharedMusicPlayer()->buttonClicked();
}


void Starmain::startItemClicked(CCObject* obj){

	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}

	Music::sharedMusicPlayer()->buttonClicked();
	Myobject* object=new Myobject();
	object->mark=true;
	object->parent_id=Starmain_tag;
	object->current_id=Bigcheckpoint_tag;
	object->pCurNode = this;
	object->autorelease();

	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE, (CCObject*)object);
}

void Starmain::xinUi(){




}

void Starmain::yuntuoXian(){

	string imgfile=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_xiaoguo_yunduo.png");

	CCPoint pleft=CCPoint(0,hight*0.6);
	CCPoint pright=CCPoint(width,hight*0.625);

	for(int i=1;i<=6;i++){

		CCSprite* p= CCSprite::create(imgfile.c_str());
		p->setOpacity(0);
		p->setRotation(45.0f);
		if(i<=3){
			float scalef=0.8f- 1.0*(i-1)/10;
			p->setScale(scalef);
			p->setPosition(pleft);
			p->runAction( CCMoveTo::create(0.0f,CCPoint(p->getPositionX()+(i-1)*50,p->getPositionY())));

		}else{

			float scalef=0.5f- 1.0*(i-4)/10;
			p->setScale(scalef);
			p->setPosition(pright);
			p->runAction(CCMoveTo::create(0.0f,CCPoint(p->getPositionX()-((i-3)-1)*25,p->getPositionY())));
		}
		p->runAction(CCFadeTo::create(1.2f, 50));
		addChild(p);
	}

	Buttonfadein();


}


void Starmain::Buttonfadein(){

	//facebook
	string facebookimg=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_facebook.png");
	facebooklogin = CCMenuItemImage::create(facebookimg.c_str(),facebookimg.c_str(),
		this,SEL_MenuHandler(&Starmain::facebookClicked));


	facebooklogin->setPosition(ccp(mStartItem->getPositionX(), mStartItem->getPositionY() - mStartItem->getContentSize().height/2 - 27 - facebooklogin->getContentSize().height/2)); 
//	fillOuty(facebooklogin,facebookimg);
	facebooklogin->setOpacity(0);

	//gamecenter
	string gameceter=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_gamecenter.png");
	CCMenuItemImage* gameceterbutton = CCMenuItemImage::create(gameceter.c_str(),gameceter.c_str(),
		this,SEL_MenuHandler(&Starmain::gamecenterClicked));

	gameceterbutton->setPosition(CCPoint(20+gameceterbutton->getContentSize().width/2,gameceterbutton->getContentSize().height/2+5));
	fillOuty(gameceterbutton,gameceter);
	gameceterbutton->setOpacity(0);
	gameceterbutton->runAction(CCFadeIn::create(1.0f));


	//设置
	string setflie=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_shezhi.png");
	CCMenuItemImage* setbutton = CCMenuItemImage::create(setflie.c_str(),setflie.c_str(),
		this,SEL_MenuHandler(&Starmain::setClicked));

	setbutton->setPosition(CCPoint(width*buton_jiangju,setbutton->getContentSize().height/2+5));
	fillOuty(setbutton,setflie);
	setbutton->setOpacity(0);
	setbutton->runAction(CCFadeIn::create(1.0f));


	//成就
	string aciveflie=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_chengjiu.png");
	CCMenuItemImage* acivebutton = CCMenuItemImage::create(aciveflie.c_str(),aciveflie.c_str(),
		this,SEL_MenuHandler(&Starmain::achiveClicked));
	acivebutton->setPosition(CCPoint(setbutton->getPositionX()-setbutton->getContentSize().width/2-20-acivebutton->getContentSize().width/2,acivebutton->getContentSize().height/2+5));
	fillOuty(acivebutton,aciveflie);
	acivebutton->setOpacity(0);
	acivebutton->runAction(CCFadeIn::create(1.0f));

	//logout
	string log_str=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_facebook_log_bg.png");
	facebooklogoutbutton=CCMenuItemImage::create(log_str.c_str(),log_str.c_str(),
		this,SEL_MenuHandler(&Starmain::facebooklogOut));
	facebooklogoutbutton->setPosition(CCPoint(gameceterbutton->getPositionX()+gameceterbutton->getContentSize().width/2+20+facebooklogoutbutton->getContentSize().width/2,gameceterbutton->getPositionY()));
	fillOuty(facebooklogoutbutton,log_str);

	if (CmdHelper::getInstance()->isAlreadyGetSocialInfor()){
		CCLabelTTF* label =CCLabelTTF::create(LFStrings::getValue("logout").c_str(),fontStr_KlavikaBold, 30);
		label->setPosition(CCPoint(facebooklogoutbutton->getContentSize().width/2 + 45,  facebooklogoutbutton->getContentSize().height/2));
		facebooklogoutbutton->addChild(label, 0, 100);
	}else {
		CCSprite* sbg = CCSprite::create(ResManager::getManager()->getSharedFilePath("UI_zairu_bg.png").c_str());
		facebooklogoutbutton->addChild(sbg, 0, 100);
		sbg->setPosition(ccp(facebooklogoutbutton->getContentSize().width*0.67, facebooklogoutbutton->getContentSize().height/2));
		CCSprite* s = CCSprite::create(ResManager::getManager()->getSharedFilePath("UI_zairu_zhuandong.png").c_str());
		sbg->addChild(s);
		s->setPosition(ccp(sbg->getContentSize().width/2, sbg->getContentSize().height/2));
		s->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
	}


	CCMenu* fmenu = CCMenu::create(facebooklogin,setbutton,acivebutton,gameceterbutton,facebooklogoutbutton,NULL);
	fmenu->setPosition(CCPointZero);
	addChild(fmenu,129);

	if(CmdHelper::getInstance()->isLogin()){
		facebooklogin->setVisible(false);
		facebooklogoutbutton->setVisible(true);
	}else{
		facebooklogin->setVisible(true);
		facebooklogoutbutton->setVisible(false);
	}
	//MSG_LoginResult

}

void Starmain::facebooklogOut(CCObject* object){

	//analytics
	CmdHelper::getInstance()->getAnalyticsInstance()->onBegin("logout");
	CmdHelper::getInstance()->getAnalyticsInstance()->onCompleted("logout");

	 CmdHelper::getInstance()->logout();
	 refreshlog();
}


void Starmain::showNewChengjiuAimation(CCNode* pParent)
{
	CCSprite* pXin = CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_xin.png").c_str());
	pParent->addChild(pXin, 1, new_achiev_tag);
	pXin->setOpacity(0);
	pXin->setPosition(pParent->getContentSize()/2);

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
			CCCallFuncN::create(this, callfuncN_selector(Starmain::showNewChengjiuWaitingAimation)),
		NULL));
}

void Starmain::showNewChengjiuWaitingAimation(CCNode* pParent)
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

void Starmain::removeNewChengjiuAimation(CCNode* pParent)
{
	if (!pParent)
	{
		return;
	}
	pParent->removeChildByTag(new_achiev_tag);
}

void Starmain::update(float delta)
{
	//LF::cutoutText(mRenderStart, mBG, (CCLabelTTF*)this->getChildByTag(100));
}



void Starmain::onFrameEvent(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex)
{
	string name = evt;

	if (name.compare("yunduo") == 0)
	{
		if (originFrameIndex == 40)
		{
			yuntuoXian();
		}
	}
	else if (name.compare("wenzi")== 0)
	{
		if (originFrameIndex == 40)
		{
			pLabel = CCLabelTTF::create(LFStrings::getValue("play").c_str(),fontStr_KlavikaBold, 60);
			pLabel->setTag(100);
			pLabel->setColor(colors.at(LevelManager::getManager()->getLastSeasonId()));
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			pLabel->setPosition(CCPoint(winSize.width/2,winSize.height*0.465f));
			pLabel->setZOrder(1025);
			addChild(pLabel);
		}
		else if (originFrameIndex == 50)
		{
			pLabelm = CCLabelTTF::create(LFStrings::getValue("miaoshu").c_str(),fontStr_KlavikaBold, 28);
			pLabelm->setPosition(ccp(width/2,hight*0.63));
			pLabelm->setZOrder(1025);
			addChild(pLabelm);
		}
	}
}

void Starmain::animationEvent(CCArmature *armature, MovementEventType movementType, const char *movementID)
{
	string ID = string(movementID);
	if (movementType == COMPLETE)
	{
	}
}

void Starmain::playDengDaiAnimation()
{
//	armature->getAnimation()->play("dengdai");
}

void Starmain::playerAnimation(CCNode* node,string animationame,CCPoint point,bool isloop){

	//chengjiu_chuxian 新成就
	//chengjiu_dengdai 成就领取

// 	armature=CCArmature::create(UI_animation_data);
// 	if(isloop==true){
// 		armature->getAnimation()->play(animationame.c_str(),-1,1);
// 	}else{
// 		armature->getAnimation()->play(animationame.c_str());
// 		armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Starmain::animationEvent));
// 		armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(Starmain::onFrameEvent));
// 	}
// 	armature->setZOrder(1025);
// 	armature->setPosition(point);
// 	node->addChild(armature);
}

void Starmain::dataLoaded(float percent){

	if (percent < 1) return;
	playerAnimation(this,"chuxian",CCPoint(width/2,hight/2),false);
}

void Starmain::refreshBgColor(CCObject* obj)
{
	int lastId = LevelManager::getManager()->getLastSeasonId();
	CCInteger *iniValue = new CCInteger(lastId);
	changeBGColor(iniValue);
	CC_SAFE_DELETE(iniValue);
}
void Starmain::changeBGColor(CCObject* pObj)
{
	CCInteger* pId = (CCInteger*)pObj;
	m_iTempSeaonId = pId->getValue();
	Starseneui::changeBg(pId->getValue(), mBG);

	//LF::cutoutText(mRenderStart, mBG, (CCLabelTTF*)this->getChildByTag(100));

	if(CmdHelper::getInstance()->isLogin())
	{
		if (facebooklogin)
		{
			facebooklogin->setVisible(false);
		}
		if (facebooklogoutbutton)
		{
			facebooklogoutbutton->setVisible(true);
		}
	}
	else
	{
		if (facebooklogin)
		{
			facebooklogin->setVisible(true);
		}
		if (facebooklogoutbutton)
		{
			facebooklogoutbutton->setVisible(false);
		}
	}

// 	if (pLabelm)
// 	{
// 		pLabelm->setColor(colors.at(LevelManager::getManager()->getLastSeasonId()));
// 	}

	if (pLabel)
	{
		pLabel->setColor(colors.at(LevelManager::getManager()->getLastSeasonId()));
	}
}
void Starmain::refreshlog(){

	if(CmdHelper::getInstance()->isLogin()){
		facebooklogin->setVisible(false);
		facebooklogoutbutton->setVisible(true);
	}else{
		facebooklogin->setVisible(true);
		facebooklogoutbutton->setVisible(false);
	}

//	facebooklogin->setVisible(true);
}

void Starmain::facebookloginshuangxin(CCObject* obj){

	MsgData *data = dynamic_cast<MsgData*>(obj);
	if (data->errCode == MsgData::eSuccess)
		MessageBar::show(LFStrings::getValue("Loginsuccessfully").c_str());
	else 
		MessageBar::show(LFStrings::getValue("Loginfailed").c_str());

	refreshlog();
}


void Starmain::drawupdatelayer(){

	 float top_f=size.width*(60.0f/1024.0f);
     float right_y=size.width*(50.0f/1024.0f);
	 float top_s=size.height*(82.0f/768.0f);
	 float left_x=size.height*(20.0f/768.0f);
	 float right_x=size.height*(50.0f/768.0f);
	 //top----------------------->layer
	 top=CCLayer::create();
	 top->ignoreAnchorPointForPosition(false);
	 top->setAnchorPoint(CCPoint(0.0f,1.0f));
	 addChild(top);
	 
	 CCSprite* sp=CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_biaoti_zhubiaoti.png", true).c_str());
	 top->setContentSize(CCSize(sp->getContentSize().width+300,sp->getContentSize().height));
	 top_start=CCPoint(top_f,size.height-top_s+size.height);
	 top_end=CCPoint(top_f,size.height-top_s);
	 top->setPosition(top_start);
	 
	 sp->setPosition(CCPoint(sp->getContentSize().width/2,top->getContentSize().height*0.5f));
	 top->addChild(sp);

	 string ck_str="intelligence";
	 CCSize ck_size= CCLabelTTF::create(ck_str.c_str(),fontStr_KlavikaBold,50)->getContentSize();

	 //intelligence
	 pLabelm = CCLabelTTF::create(LFStrings::getValue("miaoshu").c_str(),fontStr_KlavikaBold,50,CCSize(ck_size.width,0),kCCTextAlignmentLeft);
	 pLabelm->setAnchorPoint(CCPoint(0.0f,0.0f));
	 float hight_l=0.0f;
	 if(LFStrings::getValue("miaoshu1").size()>0){
		 hight_l=pLabelm->getContentSize().height/2;
	 }
	 pLabelm->setPosition(ccp(sp->getPositionX()+sp->getContentSize().width/2,hight_l));
	 pLabelm->setZOrder(1025);
	 pLabelm->setVisible(false);
	 top->addChild(pLabelm);

	 pLabelm_x = CCLabelTTF::create(LFStrings::getValue("miaoshu1").c_str(),fontStr_KlavikaBold,50,CCSize(ck_size.width,0),kCCTextAlignmentLeft);
	 pLabelm_x->setAnchorPoint(CCPoint(0.0f,0.0f));
	 pLabelm_x->setVisible(false);
	 pLabelm_x->setPosition(ccp(sp->getPositionX()+sp->getContentSize().width/2,-10.0f));
	 top->addChild(pLabelm_x);

	 string byluangue_spstr=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_biaoti_fubiaoti.png", true);
	 spbt=CCSprite::create(byluangue_spstr.c_str());
	 spbt->setAnchorPoint(CCPoint(0.0f,0.0f));
	 spbt->setPosition(CCPoint(sp->getPositionX()+sp->getContentSize().width/2+2,0));
	 top->addChild(spbt);

#if Version_Free != 0
	 byluangue_spstr=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_biaoti_zhubiaoti_free.png", true);
	 CCSprite* free = CCSprite::create(byluangue_spstr.c_str());
	 free->setAnchorPoint(CCPoint(0.0f,0.0f));
	 free->setPosition(CCPoint(0, spbt->getContentSize().height));
	 spbt->addChild(free);
#endif
	 //暂时采用图片，不替换
//	 spbt->setVisible(false);
	 
	 //left------------------------>layer
	 left=CCLayer::create();
	 left->ignoreAnchorPointForPosition(false);
	 left->setAnchorPoint(CCPoint(0.0f,0.0f));
	 addChild(left);
	 left_start=CCPoint(top_f,left_x-size.height);
	 left_end=CCPoint(top_f,left_x);

	 float scale_f=size.width*(10.0f/1024.0f);
	 //gamecenter
	 string gameceter=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_gamecenter.png");
	 gameceterbutton = CCMenuItemImage::create(gameceter.c_str(),gameceter.c_str(),
		 this,SEL_MenuHandler(&Starmain::gamecenterClicked));
	 left->setContentSize(CCSize(gameceterbutton->getContentSize().width*4+3*scale_f,gameceterbutton->getContentSize().height));
	 left->setPosition(left_start);
	 gameceterbutton->setPosition(CCPoint(gameceterbutton->getContentSize().width/2,left->getContentSize().height/2));
	 gameceterbutton->setVisible(false);
	 fillOuty(gameceterbutton,gameceter);

 //设置
	 string setflie=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_shezhi.png");
	 setbutton = CCMenuItemImage::create(setflie.c_str(),setflie.c_str(),
		 this,SEL_MenuHandler(&Starmain::setClicked));
     setbutton->setPosition(CCPoint(setbutton->getContentSize().width/2,setbutton->getContentSize().height/2));
	// setbutton->setPosition(CCPoint(acivebutton->getPositionX()+acivebutton->getContentSize().width+scale_f,left->getContentSize().height/2));
	 fillOuty(setbutton,setflie);
	
     
     //成就
	 string aciveflie=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_chengjiu.png");
	 acivebutton = CCMenuItemImage::create(aciveflie.c_str(),aciveflie.c_str(),
		 this,SEL_MenuHandler(&Starmain::achiveClicked));
	// acivebutton->setPosition(CCPoint(gameceterbutton->getPositionX()+gameceterbutton->getContentSize().width+scale_f,left->getContentSize().height/2));
	 acivebutton->setAnchorPoint(CCPoint(0.0f,0.5f));
	 acivebutton->setPosition(CCPoint(setbutton->getPositionX()+setbutton->getContentSize().width/2+scale_f,acivebutton->getContentSize().height/2));
	 fillOuty(acivebutton,aciveflie);
	 if (AchievementManager::getInstance()->getHaveNew())
	 {
		 showNewChengjiuAimation(acivebutton);
	 }

	 //logout
	 string log_str=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_facebook_log_bg.png");
	 facebooklogoutbutton=CCMenuItemImage::create(log_str.c_str(),log_str.c_str(),
		 this,SEL_MenuHandler(&Starmain::facebooklogOut));
	 facebooklogoutbutton->setPosition(CCPoint(acivebutton->getPositionX()+acivebutton->getContentSize().width+scale_f+facebooklogoutbutton->getContentSize().width/2,left->getContentSize().height/2));
	 fillOuty(facebooklogoutbutton,log_str);

	 if (CmdHelper::getInstance()->isAlreadyGetSocialInfor()){
		 CCLabelTTF* label =CCLabelTTF::create(LFStrings::getValue("logout").c_str(),fontStr_KlavikaBold, 30);
		 label->setPosition(CCPoint(facebooklogoutbutton->getContentSize().width/2 + 45,  facebooklogoutbutton->getContentSize().height/2));
		 facebooklogoutbutton->addChild(label, 0, 100);
	 }else {
		 CCSprite* sbg = CCSprite::create(ResManager::getManager()->getSharedFilePath("UI_zairu_bg.png").c_str());
		 facebooklogoutbutton->addChild(sbg, 0, 100);
		 sbg->setPosition(ccp(facebooklogoutbutton->getContentSize().width*0.67, facebooklogoutbutton->getContentSize().height/2));
		 CCSprite* s = CCSprite::create(ResManager::getManager()->getSharedFilePath("UI_zairu_zhuandong.png").c_str());
		 sbg->addChild(s);
		 s->setPosition(ccp(sbg->getContentSize().width/2, sbg->getContentSize().height/2));
		 s->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
	 }

     
	 CCMenu* fmenu = CCMenu::create(setbutton,acivebutton,gameceterbutton,facebooklogoutbutton,NULL);
	 fmenu->setPosition(CCPointZero);
	 left->addChild(fmenu,129);


	 //right------------------------->layer
	 right=CCLayer::create();
	 right->ignoreAnchorPointForPosition(false);
	 right->setAnchorPoint(CCPoint(1.0f,0.0f));
	 right_start=CCPoint(size.width-right_x,right_y-size.height);
	 right_end=CCPoint(size.width-right_x,right_y);
	 addChild(right);

	 float right_sc=size.height*(15.0f/1024.0f);
	 string facebookimg=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_facebook.png");
	 facebooklogin = CCMenuItemImage::create(facebookimg.c_str(),facebookimg.c_str(),
		 this,SEL_MenuHandler(&Starmain::facebookClicked));
	 right->setContentSize(CCSize(facebooklogin->getContentSize().width,facebooklogin->getContentSize().height*2+right_sc));
	 right->setPosition(right_start);
	 
	 facebooklogin->setPosition(CCPoint(facebooklogin->getContentSize().width/2,facebooklogin->getContentSize().height/2));
     
	 CCLabelTTF* label1 =CCLabelTTF::create(LFStrings::getValue("connect").c_str(),fontStr_KlavikaBold, 50);
	 label1->setPosition(CCPoint(facebooklogin->getContentSize().width/2 + 50,  facebooklogin->getContentSize().height/2));
	 facebooklogin->addChild(label1, 1, connect_tag);

	 if (ResManager::getManager()->getLanguageType() == kLanguageGerman)
	 {
		 label1->setFontSize(40);
	 }

	 string imgname = ResManager::getManager()->getSharedFilePath(g_buttonPath+"ZY_anniu_kaishiyouxi.png");
	 mStartItem = CCMenuItemImage::create(
		 imgname.c_str(),
		 imgname.c_str(),
		 this,SEL_MenuHandler(&Starmain::startItemClicked));
	 mStartItem->setAnchorPoint(CCPoint(0.0f,1.0f));
	 mStartItem->setPosition(CCPoint(0,right->getContentSize().height));
     

	 pLabel = CCLabelTTF::create(LFStrings::getValue("play").c_str(),fontStr_KlavikaBold, 60);
	 pLabel->setTag(100);
	 pLabel->setColor(colors.at(LevelManager::getManager()->getLastSeasonId()));
	 //this->scheduleUpdate();
	 CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	 pLabel->setPosition(CCPoint(mStartItem->getContentSize().width/2,mStartItem->getContentSize().height/2));
	 //pLabel->setPosition(nd->getContentSize()/2);
	 pLabel->setZOrder(1025);
	 mStartItem->addChild(pLabel);

	 CCMenu* rmenu = CCMenu::create(facebooklogin,mStartItem,NULL);
	 rmenu->setPosition(CCPointZero);
	 right->addChild(rmenu,129);

	 if(CmdHelper::getInstance()->isLogin()){
		 facebooklogin->setVisible(false);
		 facebooklogoutbutton->setVisible(true);
	 }else{
		 facebooklogin->setVisible(true);
		 facebooklogoutbutton->setVisible(false);
	 }
}

void Starmain::actionlayer(){
	 
	 float time=25.0f/60.0f;
	 float scald=360.0f;
	 top->runAction(CCEaseExponentialOut::create(CCMoveTo::create(time,top_end)));
	 right->runAction(CCEaseExponentialOut::create(CCMoveTo::create(time,right_end)));
	 left->runAction(CCEaseExponentialOut::create(CCMoveTo::create(time,left_end)));
	 

}

void Starmain::playaction(float dt){
	
}

void Starmain::refreshLogoutButton(CCObject* obj)
{
	if (CmdHelper::getInstance()->isAlreadyGetSocialInfor())
	{
		facebooklogoutbutton->removeChildByTag(100);
		CCLabelTTF* label =CCLabelTTF::create(LFStrings::getValue("logout").c_str(),fontStr_KlavikaBold, 30);
		label->setPosition(CCPoint(facebooklogoutbutton->getContentSize().width/2 + 45,  facebooklogoutbutton->getContentSize().height/2));
		facebooklogoutbutton->addChild(label, 0, 100);
	}
}