#include "Xiaocheckpoint.h"
#include "Debug.h"
#include "VersionControl.h"
#include "font.h"
#include "LFStrings.h"
#include "ResManager.h"
#include "UserDataManager.h"
#include "GameInforEditer.h"
#include "LevelManager.h"
#include "Myobject.h"
#include "Tanchuk.h"
#include "Baselisttable.h"
#include "GameLayerFactory.h"
#include "Starseneui.h"
#include "BaseScense.h"
#include "urlSprite.h" 
#include "LFUtile.h"
#include "Shieldingmanger.h"
#include "AppMacros.h"
#include "CmdHelper.h"
#include "History.h"
#include "MessageBar.h"
#include "Music.h"
#include "network/net/NetworkWaitingLayer.h"

#include "SocialSeasonInfor.h"
enum Xiaogkenum{

	 xiaoleft_tag=20,
	 xiaoright_tag,
};


enum Statetag{

	 lockstate=100, //解锁通过
	 unlockstate,   //解锁未通过
	 hidden,        //隐藏
	 no_state,     //未解锁
	 yinchnag_suoding,
	 yinchnag_jiesuo,
	 locklayertag,
	 unlocklayertag,
	 star_tag,
	 optical_cardlabel,
	 net_spritetag,
	 netback_spritetag,
	 start_colortag,

	 star_icon_tag,

	 yinchanglable_tag,
	 yinchanglockstate_tag,
	 yinchnaglocklable_tag,
	 yinchnaglocklable_shuzi_tag,
	 yinchnaglocklablesps_tag,
	 yinchnagxing_tag,


	 tag_start = 1234234,

}; 


Xiaocheckpoint::Xiaocheckpoint(void)
{
	
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Xiaocheckpoint::snsData), NetMessage::MSG_GotSeasonSocialInfo, NULL);
}

Xiaocheckpoint::~Xiaocheckpoint(void)
{
	
}





bool Xiaocheckpoint::init(){
	 
	 if(!CCLayer::init()){
		 return false;
	 }
	 int last=LevelManager::getManager()->getLastSectionId();
	 if(last%8==0){
		 current_page=last/8;
	 }else{
		 current_page=last/8+1;
	 }
	 moveold=movecurent=CCPoint(0,0);
	 size=CCDirector::sharedDirector()->getWinSize();
	 return true;
}  

//
void Xiaocheckpoint::setcurrentId(int season){

	this->current_season=season;
	drawLayer();
	list=Xiaogktablelist::create();
    list->setXiaogk(this);
	list->ignoreAnchorPointForPosition(false);
	list->setAnchorPoint(CCPoint(0.5f,0.5f));
	list->setPosition(CCPoint(size.width/2,size.height*0.5f));
	list->setcurrentId(season);
	addChild(list);
	list->dingwei();
}

void Xiaocheckpoint::onEnter(){

	CCLayer::onEnter();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Xiaocheckpoint::changeBGColor), MSG_UI_ChANGE_BG_COLOR, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Xiaocheckpoint::updateMsglayer), NetMessage::MSG_SyncUserDataEnd, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Xiaocheckpoint::updateMsglayer), NetMessage::MSG_GotSeasonSocialInfo, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Xiaocheckpoint::refreshLanguge),MSG_LAGUAGESmall, NULL);

}
void Xiaocheckpoint::onExit(){

	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NetMessage::MSG_SyncUserDataEnd);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NetMessage::MSG_GotSeasonSocialInfo);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_LAGUAGESmall);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

}

void Xiaocheckpoint::refreshLanguge(CCObject* object){

	 
	labelRefresh();
	list->setSpriteClip(star_l);
	list->refreshPagelable();


}

void Xiaocheckpoint::updateMsglayer(CCObject* object){

	 
	CCLOG("refresh--------------------------------->");
	//总星数
	labelRefresh();
	list->shuangxin(LevelManager::getManager()->getLastSeasonId());
    
	int lastId = LevelManager::getManager()->getLastSeasonId();
	CCInteger * intValue = new CCInteger(lastId);
	changeBGColor(intValue);
	CC_SAFE_DELETE(intValue);
}

void Xiaocheckpoint::changeBGColor(CCObject* pObj)
{
	CCInteger* pId = (CCInteger*)pObj;
	string back_str=CCString::createWithFormat("background_s%d.png",pId->getValue())->getCString();
	CCTextureCache::sharedTextureCache()->addImage(back_str.c_str());
	mBG->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(back_str.c_str()));
	mBG->setZOrder(-1);

	refreshUI(pId->getValue());
	
}

void Xiaocheckpoint::refreshUI(int seasonid)
{
	//总星数
	labelRefresh();
	list->shuangxin(seasonid);
	list->setSpriteClip(star_l);
	list->dingwei(); //这个是刷新函数 比如 内容是 第一张的第一关 第二张的第三关  设计的时候只考虑了自然数的连续性 而3-1大于1 所以只有从新初始化
}


bool Xiaocheckpoint::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return this->list->ccTouchBegan(pTouch, pEvent);
}
void Xiaocheckpoint::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	return this->list->ccTouchMoved(pTouch, pEvent);
}
void Xiaocheckpoint::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	return this->list->ccTouchEnded(pTouch, pEvent);
}

//
void Xiaocheckpoint::drawLayer(){

     //背景
	string back_str=CCString::createWithFormat("background_s%d.png",current_season)->getCString();
	mBG=CCSprite::create(back_str.c_str());
	mBG->setAnchorPoint(ccp(0.5f, 1.0f));
	mBG->setPosition(CCPoint(size.width/2, size.height));
	addChild(mBG);

	//
	string xing=ResManager::getManager()->getFilePath(g_xiaogk+"YX_start.png");
	CCSprite* pIcon = CCSprite::create(xing.c_str());
	addChild(pIcon, 1025, star_icon_tag);
	pIcon->setPosition(ccp(size.width/2 - pIcon->getContentSize().width/2 + 5.0f, size.height-pIcon->getContentSize().height/2-10));

	//总星数
	string num_str=CCString::createWithFormat(" %d",GameInforEditer::getGameInfor()->getTotalFreeScore())->getCString();
	int font_st=0;
	star_l = CCLabelTTF::create(num_str.c_str(),fontStr_KlavikaBold,30);
	star_l->setAnchorPoint(ccp(0.0f, 0.5f));
	star_l->setPosition(ccp(pIcon->getPositionX()+pIcon->getContentSize().width/2 + 5, pIcon->getPositionY()));
	star_l->setZOrder(pIcon->getZOrder());
	addChild(star_l);

	pIcon->setPositionX(size.width/2 - pIcon->getContentSize().width/2-star_l->getContentSize().width/2);
	star_l->setPosition(ccp(pIcon->getPositionX()+pIcon->getContentSize().width/2 + 5, pIcon->getPositionY()));


	//返回按钮
	string fhui_anniufile="ZJ_anniu_fanhui.png";
	fhui_anniufile=ResManager::getManager()->getFilePath(g_mainlayerPath+fhui_anniufile);
	CCMenuItemImage* p_fh=CCMenuItemImage::create(fhui_anniufile.c_str(),fhui_anniufile.c_str(),this,
		menu_selector(Xiaocheckpoint::faihuiDa));
    p_fh->setPosition(CCPoint(size.width*buton_jiangju,p_fh->getContentSize().height/2));
	fillOuty(p_fh,fhui_anniufile);

    
	
	//左右滑动列表
	//选折箭头
	string left_str="ZJ_anniu_fanye.png";
	left_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+left_str);

	itemleft=CCMenuItemImage::create(
		left_str.c_str(),
		left_str.c_str(),
		this,
		menu_selector(Xiaocheckpoint::allCallback));
	itemleft->setTag(xiaoleft_tag);
	itemleft->setRotation(135.0f);
	itemleft->setPosition(ccp(
		 size.width*0.1f
		,size.height*0.5f));
	

	itemright=CCMenuItemImage::create(
		left_str.c_str(),
		left_str.c_str(),
		this,
		menu_selector(Xiaocheckpoint::allCallback));
	itemright->setTag(xiaoright_tag);
	itemright->setRotation(-45.0f);
	itemright->setPosition(ccp(
		size.width*0.9f
		,size.height*0.5f));
	setIsyingchang();



	CCMenu* pMenu = CCMenu::create(p_fh,itemleft,itemright,NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	
	//ZJ_fenye_biaoshi.png ZJ_fenye_biaoshi_bg.png
	//黑色底
	heseSp=CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_fenye_biaoshi_bg.png").c_str());
	addChild(heseSp);
	for(int i=0;i<getYeshu();i++){
		CCSprite* sp0=CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_fenye_biaoshi.png").c_str()); 
		float start_spx=(size.width-(sp0->getContentSize().width*getYeshu()+(getYeshu()-1)*Big_sp_juli))/2;
		sp0->setPosition(CCPoint(start_spx+sp0->getContentSize().width/2+(i)*(sp0->getContentSize().width+Big_sp_juli),p_fh->getContentSize().height/2+5));
		addChild(sp0);
		backs_v.push_back(sp0);
		if(current_page==i+1){
			heseSp->setPosition(sp0->getPosition());
		}else{
			sp0->setOpacity(num_Fade/2);
		}
		if(i<getYeshu()-1){
			CCSprite* sp1=CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_fenye_biaoshi.png").c_str()); 
			sp1->setScale(Big_xiao_backscale);
			sp1->setPosition(CCPoint(sp0->getPositionX()+sp0->getContentSize().width/2+sp1->getContentSize().width*Big_xiao_backscale/2-1,sp0->getPositionY()));
			sp1->setOpacity(num_Fade/2);
			addChild(sp1);
		}
	}

	if(backs_v.size()>1){
		sps_juli=backs_v.at(1)->getPositionX()-backs_v.at(0)->getPositionX();
	}

	
	//heseSp->setPosition(sp0->getPosition());
	int lastsectionid=LevelManager::getManager()->getLastSectionId();
	int idx=0;
	
}

void Xiaocheckpoint::setHesepoint(int page){

	heseSp->setPosition(CCPoint(backs_v.at(page-1)->getPositionX(),heseSp->getPositionY()));

}

void Xiaocheckpoint::jianxiao(int page){

	//从新初始化
	//heseSp->setPosition(CCPoint(backs_v.at(page-1)->getPositionX(),heseSp->getPositionY()))
	for(int i=0;i<getYeshu();i++){
		if(i+1==page){
			heseSp->setPosition(backs_v.at(i)->getPosition());
			backs_v.at(i)->setOpacity(num_Fade);
		}else{
			backs_v.at(i)->setOpacity(num_Fade/2);
		}
	}
}

void Xiaocheckpoint::labelRefresh(){

	string num_str=CCString::createWithFormat(" %d",GameInforEditer::getGameInfor()->getTotalFreeScore())->getCString();
	star_l->setString(num_str.c_str());

	CCSprite* pIcon = (CCSprite*)this->getChildByTag(star_icon_tag);
	pIcon->setPositionX(size.width/2 - pIcon->getContentSize().width/2-star_l->getContentSize().width/2 + 5.0f);
	star_l->setPosition(ccp(pIcon->getPositionX()+pIcon->getContentSize().width/2 + 5, pIcon->getPositionY()));
}

int Xiaocheckpoint::getByseasoncheck(int season){


	//GameInforEditer
	int num=GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(current_season);
	return num;
}

int Xiaocheckpoint::getYeshu(){


	int yeshu=0;
	if(getByseasoncheck(current_season)%8==0){
		yeshu=getByseasoncheck(current_season)/8;
	}else{
		yeshu=getByseasoncheck(current_season)/8+1;
	}
	return yeshu;
}

void Xiaocheckpoint::allCallback(CCObject* ob){


}






int  Xiaocheckpoint::getcurrentId(){
	 
     return current_season;
}




int Xiaocheckpoint::getNumscore(){

	int num=0;
	for(size_t i=0;i<msg_setion.size();i++){
		num+=msg_setion.at(i)->iStarNum;
	}
	//是否加上以买星星数
	num+=0;
	return num;

}


void Xiaocheckpoint::faihuiDa(CCObject* object){
	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}
	Music::sharedMusicPlayer()->buttonClicked();
	Myobject* ob=new Myobject();
	ob->mark=false;
	ob->parent_id = Bigcheckpoint_tag;
	ob->current_id = Smalllayer_tag;
	ob->pCurNode = this;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE,(CCObject*)ob);

}

void Xiaocheckpoint::fillOuty(CCMenuItemImage* p,string imgfile){
    
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

bool Xiaocheckpoint::isetLeftbutton(){

	return true;
}

bool Xiaocheckpoint::isrightbutton(){

	return true;
}

CCSprite* Xiaocheckpoint::getBacksprite(){

	 return mBG;

}

void Xiaocheckpoint::setIsyingchang(){

	if(isetLeftbutton()){
		itemleft->setVisible(false);
	}else{
		itemleft->setVisible(true);
	}

	if(isrightbutton()){
		itemright->setVisible(false);
	}else{
		itemright->setVisible(true);
	}
}

void Xiaocheckpoint::BigjuliOut(float changeOut,int current,int xlpage){


	
	

		changeOut=abs(changeOut);
		if(changeOut>=0&&changeOut<=num_Fade){
			//heseSp sps_juli
			if(current>=1&&current<=getYeshu()&&xlpage>=1&&xlpage<=getYeshu()){
				if(current>xlpage){//向左滑
					float change_x=sps_juli*((num_Fade-changeOut)/num_Fade);
					heseSp->setPosition(CCPoint(backs_v.at(current-1)->getPositionX()-change_x,heseSp->getPositionY()));
				}else if(current<xlpage){//向右滑
					float change_x=sps_juli*((num_Fade-changeOut)/num_Fade);
					heseSp->setPosition(CCPoint(backs_v.at(current-1)->getPositionX()+change_x,heseSp->getPositionY()));
				}
				float num_z=num_Fade/2;
				backs_v.at(current-1)->setOpacity(num_Fade-num_z*((num_Fade-changeOut)/num_Fade));
				backs_v.at(xlpage-1)->setOpacity(num_Fade/2+num_z*((num_Fade-changeOut)/num_Fade));
			}
		}

	
}



/*
===============================================================================================
===============================================================================================
===============================================================================================
*/


//xiaocheck_point
int Xiaogktablelist::layernum=8;
Xiaogktablelist::Xiaogktablelist(void){

}

Xiaogktablelist::~Xiaogktablelist(void){

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

 bool Xiaogktablelist::init(){

	 if(!CCLayer::init()){

		 return false;
	 }

	 CCSpriteBatchNode* spriteTexturelock = CCSpriteBatchNode::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_jiesuo_1.png").c_str()); 
	 batchnodemaps.insert(make_pair(lockstate,spriteTexturelock));
	 addChild(spriteTexturelock);
	 CCSpriteBatchNode* spriteTextureunlock = CCSpriteBatchNode::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_jiesuo_xin.png").c_str()); 
	 batchnodemaps.insert(make_pair(unlockstate,spriteTextureunlock));
	 addChild(spriteTextureunlock);
	 CCSpriteBatchNode* spriteTexturehidden = CCSpriteBatchNode::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_yincang_suoding.png").c_str()); 
	 batchnodemaps.insert(make_pair(hidden,spriteTexturehidden));
	 addChild(spriteTexturehidden);

	 CCSpriteBatchNode* spriteTextureno_state = CCSpriteBatchNode::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_suoding.png").c_str()); 
	 batchnodemaps.insert(make_pair(no_state,spriteTextureno_state));
	 addChild(spriteTextureno_state);

	 // yinchnag_suoding, yinchnag_jiesuo,

	 CCSpriteBatchNode* yspriteTextureno_state = CCSpriteBatchNode::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_yincang_suoding.png").c_str()); 
	 batchnodemaps.insert(make_pair(yinchnag_suoding,yspriteTextureno_state));
	 addChild(yspriteTextureno_state);


	 CCSpriteBatchNode* yjspriteTextureno_state = CCSpriteBatchNode::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_yincang_jiesuo.png").c_str()); 
	 batchnodemaps.insert(make_pair(yinchnag_jiesuo,yjspriteTextureno_state));
	 addChild(yjspriteTextureno_state);


	 
	 int iCount = GameLayerFactory::getInstance()->getValidSeasonNum();
	 for(int i=1;i<=iCount;i++){
		 int index = i;
		 string back_str=CCString::createWithFormat("ZJ_bg_season%d.png",index)->getCString();
		 back_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
		 CCImage *pImage = new CCImage();
		 pImage->initWithImageFile(back_str.c_str(),CCImage::kFmtPng);
		 getImagecolor4Bpos(pImage,ccp(1,384),index);
	 }
      
	 mLastTouchTime = 0;
	 direction=-1;
	 current_page=1;
	 old_page=current_page;
	 moveold=movecurent=CCPoint(0,0);
	 size=CCDirector::sharedDirector()->getWinSize();
	 button=false;
	 isTouch = false;
	 return true;
 }

 void Xiaogktablelist::shuangxin(int curentseason){

	 this->current_season=curentseason;
	 msg_setion.clear();

	 SeasonInfor* s_msg=GameInforEditer::getGameInfor()->getSeasonInfor(curentseason);
	 if(s_msg){
		 list<SectionInfor*> lsitseinfo= s_msg->getAllSectionInfors();
		 list<SectionInfor*>::iterator it;
		 for (it = lsitseinfo.begin(); it!=lsitseinfo.end(); it++) {
			 msg_setion.push_back(*it);
		 }
	 }
	 for(int i=0;i<getYeshu();i++){
		 refreshLayer(i);
	 }
	gk_txvs.clear();
	netDatainint();

	//
	refresh3rdLayer();
 }

 void Xiaogktablelist::refresh3rdLayer()
 {
	 if(lockstate_layers.size()==3){
		 CCLayer* layer=lockstate_layers.at(3);
		 //yinc_l->setTag(yinchanglable_tag);
		 CCLabelTTF* p=(CCLabelTTF*)layer->getChildByTag(yinchanglable_tag);
		 string str_yc=LFStrings::getValue("lockbiaoT");
		 p->setString(str_yc.c_str());

		 string lock_msglable=LFStrings::getValue("lockmsg"); //CCString::createWithFormat("%d",page*8+i)->getCString" "+();
		 string current_stars=CCString::createWithFormat("%d", GameInforEditer::getGameInfor()->getTotalScoreBeforeSeasonId(current_season))->getCString();
		 const ConfigSeasonInfo* info=GameLayerFactory::getInstance()->getConfigSeasonInfo(current_season);
		 string need_stars=CCString::createWithFormat("%d", info->iDeblockNum)->getCString();
//		lock_msglable+=current_stars+"/"+need_stars;

		 CCLabelTTF* l_msg_shuzi=(CCLabelTTF*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklable_shuzi_tag);
		 l_msg_shuzi->setString((" "+current_stars+"/"+need_stars).c_str());

		 CCLabelTTF* l_msg=(CCLabelTTF*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklable_tag);
		 CCSprite* sp_locksprite = (CCSprite*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklablesps_tag);
		 CCSprite* sp_xing = (CCSprite*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnagxing_tag);

		 l_msg_shuzi->setPosition(ccp(l_msg->getPositionX(), l_msg->getPositionY() - l_msg->getContentSize().height/2 - l_msg_shuzi->getContentSize().height/2 - 10));
		 sp_locksprite->setPosition(ccp(l_msg_shuzi->getPositionX()+l_msg_shuzi->getContentSize().width/2+sp_locksprite->getContentSize().width/2 , l_msg_shuzi->getPositionY()));
		 sp_xing->setPosition(ccp(l_msg_shuzi->getPositionX()-l_msg_shuzi->getContentSize().width/2-sp_xing->getContentSize().width/2 , l_msg_shuzi->getPositionY()));

//
//		CCSprite* sps=(CCSprite*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklablesps_tag);
//		float x=(width_layerbt-(sps->getContentSize().width+l_msg->getContentSize().width))/2;
//		l_msg->setPosition(CCPoint(x+l_msg->getContentSize().width/2, sps->getContentSize().height/2));

//		sps->setPosition(CCPoint(l_msg->getPositionX()+l_msg->getContentSize().width/2+sps->getContentSize().width/2 + 5, l_msg->getPositionY()));

		 if(GameInforEditer::getGameInfor()->getTotalScoreBeforeSeasonId(current_season)>=info->iDeblockNum){
			 layer->getChildByTag(yinchanglockstate_tag)->setVisible(false);
		 }else{
			 layer->getChildByTag(yinchanglockstate_tag)->setVisible(true);
		 }
	 }
 }

 void Xiaogktablelist::getMarkbackstr(string &bg_file,int &mark,int page,int i){
	  
	
	 if(page*layernum+i<17){
		 //已解锁
		 if(isJiesuo(page*8+i,true)){

			 bg_file="GQ_jiesuo_1.png";
			 if(isJiesuo(page*8+i,false)){
				 bg_file="GQ_jiesuo_1.png";
				 mark=lockstate;//解锁通过
			 }else{
				 bg_file="GQ_jiesuo_xin.png";
				 mark=unlockstate;//解锁未通过
			 }
		 }else{//未解锁
			 bg_file="GQ_suoding.png";
			 mark=no_state;//未解锁
		 }
	 }else{
		 bg_file="GQ_yincang_suoding.png";
		 int getNum_star=getNumscore();
		 
		 
		 const ConfigSeasonInfo* info=GameLayerFactory::getInstance()->getConfigSeasonInfo(current_season);

		 if(getNum_star>=info->iDeblockNum || GameInforEditer::getGameInfor()->isIgnoreAllLock()){

			 bg_file="GQ_yincang_suoding.png";
			 if(isJiesuo(page*8+i,true)){

				 if(isJiesuo(page*8+i,false)){
					 bg_file="GQ_yincang_jiesuo.png";
					 mark=lockstate;
					 
				 }else{
					 bg_file="GQ_yincang_jiesuo.png";
					 mark=unlockstate;
				 }
			 }else{
				 bg_file="GQ_yincang_suoding.png";
				 mark=hidden;//隐场
			 }
		 }else{
			 bg_file="GQ_yincang_suoding.png";
			 mark=hidden;//隐场
		 }
	 }
	 bg_file=ResManager::getManager()->getFilePath(g_xiaogk+bg_file);
 }



 void Xiaogktablelist::refreshPagelable(){

	 for(int i=0;i<getYeshu();i++){
		 refreshlable(i);
	 }

	 refresh3rdLayer();

 }
 void Xiaogktablelist::refreshlable(int page){

	 vector<CCSprite*> sps=msg_mapsps.at(page+1);
	 int mark=0;
	 for(int i=1;i<=layernum;i++){
		 if(page*8+i>getByseasoncheck(current_season)){
			 break;
		 }
		 string bg_file="";
		 mark=0;
		 getMarkbackstr(bg_file,mark,page,i);
		 if(page*8+i<=getByseasoncheck(current_season)){
			  if(mark==unlockstate){
				  string start=LFStrings::getValue("start");
				  CCLabelTTF* startlable=(CCLabelTTF*) sps.at(i-1)->getChildByTag(unlocklayertag)->getChildByTag(start_colortag);
				  
				  startlable->setString(start.c_str());
				  setSpriteClip(startlable);
			 }
		 }
	 }

 }

 /*

 lockstate=100, //解锁通过
 unlockstate,   //解锁未通过
 hidden,        //隐藏
 no_state,     //未解锁

 */


 void Xiaogktablelist::refreshLayer(int page){

	 vector<CCSprite*> sps=msg_mapsps.at(page+1);
	 int mark=0;
	 for(int i=1;i<=layernum;i++){

		 if(page*8+i>getByseasoncheck(current_season)){
			 break;
		 }
		 string bg_file="";
		 mark=0;
		 getMarkbackstr(bg_file,mark,page,i);

		 // yinchnag_suoding,
		 //yinchnag_jiesuo,

		 if(page*8+i<=16){
		     sps.at(i-1)->setTexture(batchnodemaps.at(mark)->getTexture());
		 }else{
			 if(mark==hidden){
			   sps.at(i-1)->setTexture(batchnodemaps.at(yinchnag_suoding)->getTexture());
			 }else{
			   sps.at(i-1)->setTexture(batchnodemaps.at(yinchnag_jiesuo)->getTexture());
			 }
		 }
		
		 setSpriteClip(sps.at(i-1));

		 if(page*8+i<=getByseasoncheck(current_season)){
			 if( sps.at(i-1)->getChildByTag(net_spritetag)){
			     sps.at(i-1)->removeChildByTag(net_spritetag);
			 }
			 if( sps.at(i-1)->getChildByTag(netback_spritetag)){
				 sps.at(i-1)->removeChildByTag(netback_spritetag);
			 }

			 if(mark==lockstate){
				 sps.at(i-1)->getChildByTag(optical_cardlabel)->setVisible(true);
				 CCLabelTTF* pl=(CCLabelTTF*)sps.at(i-1)->getChildByTag(optical_cardlabel);
				 pl->setColor(colors.at(current_season));
				 sps.at(i-1)->getChildByTag(locklayertag)->setVisible(true);
				 sps.at(i-1)->getChildByTag(unlocklayertag)->setVisible(false);
				 int num_fenshu= 0;
				 size_t index = page*8+i;
				 //if (index <= msg_setion.size())
				 //{
					 for(size_t m=0;m<msg_setion.size();m++){
						 if(msg_setion.at(m)->sectionId==index&& msg_setion.at(m)->isSuccessed==true){
							 num_fenshu = msg_setion.at(m)->iStarNum;
							 break;
						 }
					 }
					 
				 //}
				 for(int j=0;j<3;j++){
					 if(j<num_fenshu){
						 sps.at(i-1)->getChildByTag(locklayertag)->getChildByTag(star_tag+j)->setVisible(true);
					 }else{
						 sps.at(i-1)->getChildByTag(locklayertag)->getChildByTag(star_tag+j)->setVisible(false);
					 }
				 }
			 }else if(mark==unlockstate){
				 sps.at(i-1)->getChildByTag(optical_cardlabel)->setVisible(true);
				 CCLabelTTF* pl=(CCLabelTTF*)sps.at(i-1)->getChildByTag(optical_cardlabel);
				 pl->setColor(colors.at(current_season));
				 //CCLabelTTF* startlable=(CCLabelTTF*) sps.at(i-1)->getChildByTag(unlocklayertag)->getChildByTag(start_colortag);
				 //startlable->setColor(colors.at(current_season));

				 sps.at(i-1)->getChildByTag(locklayertag)->setVisible(false);
				 sps.at(i-1)->getChildByTag(unlocklayertag)->setVisible(true);

				 if(page*8+i>=17){
					 sps.at(i-1)->getChildByTag(unlocklayertag)->setVisible(false);
				 }

			 }else if(mark==no_state){
				 sps.at(i-1)->getChildByTag(locklayertag)->setVisible(false);
				 sps.at(i-1)->getChildByTag(unlocklayertag)->setVisible(false);
				 sps.at(i-1)->getChildByTag(optical_cardlabel)->setVisible(false);
			 }else if(mark==hidden){
				 sps.at(i-1)->getChildByTag(locklayertag)->setVisible(false);
				 sps.at(i-1)->getChildByTag(unlocklayertag)->setVisible(false);
				 sps.at(i-1)->getChildByTag(optical_cardlabel)->setVisible(false);
			 }

			 
		 }
		 
	 }
 }


 /*
 //方法五：利用纹理，  
 //适合于需要频繁使用的图片  
 CCSpriteBatchNode* spriteTexture = CCSpriteBatchNode::create("iocn.png");  
 spriteTexture->setPosition(CCPointZero);  
 addChild(spriteTexture);  
 CCSprite* sprite = CCSprite::createWithTexture(spriteTexture->getTexture());  
 sprite->setPosition(ccp(visiblesize.width/2, 100));  
 spriteTexture->addChild(sprite, 2);</span></strong> 
 */


 void Xiaogktablelist::onEnter(){

	 CCLayer::onEnter();
	 /*
	 static const char*  MSG_SyncUserDataEnd = "NetMessage:MSG_SyncUserDataEnd"; //同步用户数据到服务器结束
	 static const char*  MSG_GotSeasonSocialInfo = "NetMessage:MSG_GotSeasonSocialInfo"; //获取一个season的社交信息（好友分数、排行、关卡全服平均分）
	 */
	 //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,smalltouchtype,false);
	 


 }





 void Xiaogktablelist::onExit(){


	 CCLayer::onExit();
	 //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	 msg_setion.clear();
	 msg_mapsps.clear();
	 batchnodemaps.clear();
	 gk_txvs.clear();
	 layer_map.clear();
	 colors.clear();
 }
 void Xiaogktablelist::setSpriteClip(CCSprite* sprite)
 {
	 ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
	 sprite->setBlendFunc(blend);

	 sprite->setShaderProgram(mGradientClipShader);
 }
 void Xiaogktablelist::createGradientClipShader(float leftX, float rightX)
 {
	 CCGLProgram* shader = new CCGLProgram();
	 const GLchar* FSH = "																		\n\
						 #ifdef GL_ES															\n\
						 precision lowp float;													\n\
						 #endif																	\n\
						 \n\
						 varying vec4 v_fragmentColor;											\n\
						 varying vec2 v_texCoord;												\n\
						 uniform float left;													\n\
						 uniform float right;													\n\
						 uniform float grad_w;													\n\
						 uniform sampler2D CC_Texture0;											\n\
						 \n\
						 void main()															\n\
						 {																		\n\
						 vec4 color = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);	\n\
						 float x = gl_FragCoord.x + 0.5;											\n\
						 float alpha = 1.0;													\n\
						 if(x <= left + grad_w && x >= left)									\n\
						 {																	\n\
						 alpha = 1.0 - (left + grad_w - x) / grad_w;						\n\
						 }																	\n\
						 else if(x >= right - grad_w && x <= right)							\n\
						 {																	\n\
						 alpha = 1.0 - (x - right + grad_w) / grad_w;					\n\
						 }																	\n\
						 alpha = alpha * alpha * alpha;																	\n\
						 gl_FragColor = vec4(color.r / color.a, color.g / color.a, color.b / color.a, color.a * alpha);	\n\
						 }																		\n\
						 ";
	 shader->initWithVertexShaderByteArray(ccPositionTextureColor_vert, FSH);
	 shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	 shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	 shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

	 shader->link();
	 shader->updateUniforms();

	 int left  = shader->getUniformLocationForName("left");
	 int right = shader->getUniformLocationForName("right");
	 int wloc  = shader->getUniformLocationForName("grad_w");

	 float vWith = CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width;
	 float wWith = CCDirector::sharedDirector()->getWinSize().width;
	 float dWidth = designResolutionSize.width;
	 float factor = vWith / wWith;
	 shader->setUniformLocationWith1f(left, leftX * factor);
	 shader->setUniformLocationWith1f(right, rightX * factor);
	 shader->setUniformLocationWith1f(wloc, 120.0f * (wWith / dWidth));


	 mGradientClipShader = shader;
 }

 void Xiaogktablelist::diguibyjuliOut(CCNode* node,float changeOut){

#if 0
 if(changeOut>=0&&changeOut<=num_Fade){
	 CCArray* arry= node->getChildren();
	 for(size_t i=0;i<arry->count();i++)
	 {
		 CCSprite* p=((CCSprite*)arry->objectAtIndex(i)); 
		 p->setOpacity(changeOut);// runAction(CCFadeOut::create(gkfadeout));
		 if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			 diguibyjuliOut(p,changeOut);
		 }
	 } 
  }
#endif

 }

 void Xiaogktablelist::scrollViewDidScroll(CCScrollView* view){


	 bool mo= scrollView->isSingleReference();
	 if(mo==true){

		
	 }
	 if(!msg_mapsps.empty() && !layer_map.empty())
	 {
		 movecurent_fade=scrollView->getContentOffset();
		 if(movecurent_fade.x<scroview_point.x){
			 direction=0;
		 }else if(movecurent_fade.x>scroview_point.x){
			 direction=1;
		 }
		 int old_p=old_page;
		 float m_x=movecurent_fade.x;
		 m_x=abs(m_x);
		 m_x=m_x-(old_page-1)*ju_li;
		 m_x=abs(m_x);
		 float ch=num_Fade-num_Fade*(m_x/ju_li);
		 diguibyjuliOut(layer_map.at(old_page),ch);


		 if(prent){

			 if(direction==0){
				 if(old_p+1<=getYeshu()){
					 old_p=old_p+1;
				 }else{
					 old_p=-1;
				 }
				 if(old_p>=1&&old_p<=getYeshu()&&direction==0){
					 float in_f=num_Fade*(m_x/ju_li);
					 diguibyjuliOut(layer_map.at(old_p),in_f);
				 }
				 this->prent->BigjuliOut(ch,old_page,old_p);

			 }else if(direction==1){
				 if(old_p-1>=1){
					 old_p=old_p-1;
				 }else{
					 old_p=-1;
				 }
				 if(old_p<=getYeshu()&&old_p>=1&&direction==1){
					 float in_f=num_Fade*(m_x/ju_li);
					 diguibyjuliOut(layer_map.at(old_p),in_f);
				 }
				 this->prent->BigjuliOut(ch,old_page,old_p);
			 }
		 }

		 moveold_fade=movecurent_fade;
	 }
 }

 void Xiaogktablelist::scrollViewDidZoom(CCScrollView* view){

 }

 bool Xiaogktablelist::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

	 if(Shieldingmanger::getSielding()->getCureentlayertag()!=Smalllayer_tag){
		 return false;
	 }
	 if(isTouch==true){
		 return false;
	 }
	 struct cc_timeval now;
	 CCTime::gettimeofdayCocos2d(&now, NULL);
	 mLastTouchTime = (double)now.tv_sec + (double)now.tv_usec / 1000000.0f;

	 old_page=current_page;
	 start_ponit=pTouch->getLocation();
	 mark_firstmove=0;
	 CCPoint po=this->getPosition();
	 scroview_point=scrollView->getContentOffset();
// 	 if(start_ponit.x<=po.x+scrow_size.width/2&&start_ponit.x>=po.x-scrow_size.width/2
// 		 &&start_ponit.y<=po.y+scrow_size.height/2&&start_ponit.y>=po.y-scrow_size.height/2
// 		 ){
// 		 old_page=current_page;
// 		 scroview_point=scrollView->getContentOffset();
// 		 return true;
//          
// 	 }
	 return true;
 }
 void Xiaogktablelist::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

	 if(!Shieldingmanger::getSielding()->isHorizontalActive())
	 {
		 return;
	 }



	 isTouch=true;
	 dianjinum++;

	 movecurent=pTouch->getLocation();
	 if(mark_firstmove==0){
		 scrollView->setContentOffset( scrollView->getContentOffset()+CCPoint(((movecurent.x-start_ponit.x)),0),false);
		 mark_firstmove=1;
	 }else if(mark_firstmove==1){
		 scrollView->setContentOffset( scrollView->getContentOffset()+CCPoint(((movecurent.x-moveold.x)),0),false);
	 }
	 moveold=movecurent;

 }
 void Xiaogktablelist::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

	 end_point=pTouch->getLocation();
	 
	 if(ccpDistance(start_ponit, end_point) <= 30){
		 if (ontouchCilick(pTouch,pEvent))
		 {
			 return;
		 }
	 }

	 if(!Shieldingmanger::getSielding()->isHorizontalActive())
	 {
		 return;
	 }



	 isTouch=false;
	 CCPoint deltaPoint = end_point - start_ponit;
	 float dis = end_point.getDistance(start_ponit);

	 struct cc_timeval now;
	 CCTime::gettimeofdayCocos2d(&now, NULL);
	 double nowTime = (double)now.tv_sec + (double)now.tv_usec / 1000000.0f;
	 double deltaTime = (double)(nowTime - mLastTouchTime);
	 float vel = dis / (float)deltaTime * 1.0f; //TODO: 1.0f替换成（PPI / 标准PPI）
	 const float MOVE_SPEED_MIN = 900.0f;
	 //CCLog("vel=%f, dis=%f, deltaTime=%f nowTime=%f, mLastTouchTime=%f\n", vel, dis, deltaTime, nowTime, mLastTouchTime);
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	 const float MOVE_DIS_MIN = size.height/2;

	if(dis > 10 && (vel > MOVE_SPEED_MIN || abs(deltaPoint.y) > MOVE_DIS_MIN))
	{
		if (deltaPoint.x < 0)
		{
			current_page++;  
		}
		else
		{  
			current_page--;  
		}  

		if (current_page < 1)  
		{  
			current_page = 1;  
		}  
		if (current_page > getYeshu())  
		{  
			current_page = getYeshu();  
		}

		if(current_page*layernum<20){
			GameInforEditer::getGameInfor()->setLastClickId(current_season,current_page*layernum);


		}else{
			GameInforEditer::getGameInfor()->setLastClickId(current_season,17);
		}


	}
	CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0); 
	scrollView->setContentOffset(CCEaseExponentialOut::create(CCMoveTo::create(0.6f, adjustPos)));
	//CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0);  
	//scrollView->setContentOffset(adjustPos, true);
	//this->prent->setHesepoint(current_page);
#if 0
	 float distance = end_point.x - start_ponit.x;  
	 //float prent_qie=size.width*(100.0f/1024.0f);
	 float prent_qie=50.0f;
	 if(fabs(distance)>=prent_qie)
	{
		 if (distance < 0)  
		 {  
			 current_page++;  
		 }else{  
			 current_page--;  
		 }  
		 if (current_page < 1)  
		 {  
			 current_page = 1;  
		 }  
		 if (current_page > getYeshu())  
		 {  
			 current_page = getYeshu();  
		 }
		 CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0);  
		 scrollView->setContentOffset(adjustPos, true);
	 }else{
		 CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0);  
		 scrollView->setContentOffset(adjustPos, true);
		 scrollView->setContentOffset( CCEaseExponentialOut::create(CCMoveTo::create(0.4f, adjustPos)));
	 }
#endif
	 dianjinum=0;
 }

 void Xiaogktablelist::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

	ccTouchEnded(pTouch, pEvent);
 }

bool Xiaogktablelist::ontouchCilick(CCTouch *pTouch, CCEvent *pEvent){

	if(button==true){
		return false;
	}

	Music::sharedMusicPlayer()->buttonClicked();
	CCPoint p=pTouch->getLocation();
	vector<CCSprite*> sps = msg_mapsps.at(current_page);

	for(size_t i = 0; i < sps.size(); i++)
	{
		CCSprite* pTmep = sps.at(i);
		CCRect size = CCRectMake(0, 0, pTmep->boundingBox().size.width, pTmep->boundingBox().size.height);
		if (size.containsPoint(pTmep->convertToNodeSpace(p)))
		{
			select_gkid=(current_page-1)*layernum+i+1;

			//增加关卡点击判断，只有在关卡全开和关卡解锁的才能点进去
			SectionInfor* pInfo = GameInforEditer::getGameInfor()->getSectionInfor(current_season, select_gkid);
			if (!GameInforEditer::getGameInfor()->isIgnoreAllLock())
			{
				if (!pInfo)
				{
					if (GameInforEditer::getGameInfor()->isChallengeSection(current_season, select_gkid))
					{
						playLockInfoAnimation();
					}
					else
					{
						MessageBar::show(LFStrings::getValue("Thelevelislocked").c_str());
						//CCMessageBox("The current level is locked", "Tip");
					}
					return false;
				}
				else
				{
					if (GameInforEditer::getGameInfor()->isChallengeSection(pInfo->seasonId, pInfo->sectionId))
					{
						if (!GameInforEditer::getGameInfor()->getSecretStageCanUnLocked(pInfo->seasonId))
						{
							//CCMessageBox("score is not enough!", "Tip");
							playLockInfoAnimation();
							return false;
						}
					}
				}
			}
			this->addChild(NetworkWaitingLayer::create(true), 12000);
			LevelManager::getManager()->setSlideToPage3(false);
            button = true;
			pTmep->runAction(
				CCSequence::create(CCFadeOut::create(0.05f),CCFadeIn::create(0.05f),
				CCCallFunc::create(this,callfunc_selector(Xiaogktablelist::startGame)),
				NULL));
			return true;
		}
	}
	return false;
#if 0
	 bool is_x=(p.x<=size.width/2+scrow_size.width/2&&p.x>=size.width/2-scrow_size.width/2);
	 bool is_y=(p.y<=size.height/2+scrow_size.height/2&&p.y>=size.height/2-scrow_size.height/2);
	 if(is_x==true&&is_y==true){
		 vector<CCSprite*> sps=msg_mapsps.at(current_page);
		 CCPoint juyuoN=CCPoint(p.x-(size.width/2-scrow_size.width/2),p.y-(size.height/2-scrow_size.height/2));
		 for(size_t i=0;i<sps.size();i++){
			 if(SpisContaintPoint(sps.at(i),juyuoN)){
				 select_gkid=(current_page-1)*layernum+i+1;

				 //增加关卡点击判断，只有在关卡全开和关卡解锁的才能点进去
				 SectionInfor* pInfo = GameInforEditer::getGameInfor()->getSectionInfor(current_season, select_gkid);
				 if ((!pInfo || pInfo->locked || pInfo->isPlayed) && !GameInforEditer::getGameInfor()->isIgnoreAllLock())
				 {
					 return;
				 }

				 
				 sps.at(i)->runAction(
					 CCSequence::create(CCFadeOut::create(0.05f),CCFadeIn::create(0.05f),
					 CCCallFunc::create(this,callfunc_selector(Xiaogktablelist::startGame)),
					 NULL));
				 break;
			 }
		 }
	 }
#endif
 }

 void Xiaogktablelist::setXiaogk(Xiaocheckpoint* p){

	  this->prent=p;
 }

 void Xiaogktablelist::startGame(){
	 
	 CmdHelper::getInstance()->preloadSectionSocialInfo(current_season,select_gkid);
	 LevelManager::getManager()->setLastSectionId(select_gkid);
	 ResManager::getManager()->loadAllArmatureInfo();
	 LevelManager::getManager()->retry(true);
	 button=false;
	 return;
	 //LevelManager::getManager()->setLastSectionId();
	 Myobject* object=new Myobject();
	 object->mark=true;
	 object->parent_id=Smalllayer_tag;
	 object->current_id=Guangkalayer_tag;
	 object->pCurNode = this;
	 //游戏信息
	 object->season=current_season;
	 object->gk_id=select_gkid;
	 object->autorelease();
	 CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE, (CCObject*)object);

 }

 bool Xiaogktablelist::SpisContaintPoint(CCSprite* ps,CCPoint p){

	 bool iscontaint=false;
	 if(p.x<=ps->getPositionX()+ps->getContentSize().width/2&&p.x>=ps->getPositionX()-ps->getContentSize().width/2
		 &&p.y<=ps->getPositionY()+ps->getContentSize().height/2&&p.y>=ps->getPositionY()-ps->getContentSize().height/2
		 ){
			 iscontaint=true;
	 }
	 return iscontaint;
 }


 void  Xiaogktablelist::snsData(CCObject* obj){

	 /* 2015/3/11 scf
	  * 不用在每一个小关卡上都显示第一名了，只在关卡上显示玩家进度（即刚好打通这关才显示）
	  */
	 addNodelabel();
 }

 void Xiaogktablelist::netDatainint(){

	 /* 2015/3/11 scf
	  * 不用在每一个小关卡上都显示第一名了，只在关卡上显示玩家进度（即刚好打通这关才显示）
	  */
	 const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(current_season,1);
	 if(info==nullptr){
		 CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Xiaogktablelist::snsData), NetMessage::MSG_GotSeasonSocialInfo, NULL);
	 }else {
		addNodelabel();
	 }
 }


 CCSprite* Xiaogktablelist::getByyshuandid(int key){

	 CCSprite* sp=nullptr;
	 for(int i=0;i<getYeshu();i++){
		 vector<CCSprite*> sps=msg_mapsps.at(i+1);
		 for(int j=0;j<layernum;j++){
			 if(i*layernum+j==key-1){
				 sp= sps.at(j);
				 return sp;
			 }
		 }
	 }
	 return sp;

 }
 void Xiaogktablelist::addNodelabel(){

	 /* 2015/3/11 scf
	  * 不用在每一个小关卡上都显示第一名了，只在关卡上显示玩家进度（即刚好打通这关才显示）
	  */
	 for (int sectionId=1,count=getByseasoncheck(current_season); sectionId<=count; ++sectionId)
	 {
		 vector<string> userList = SocialInfoManager::getInstance()->getFriendProgressBySection(current_season, sectionId);
		 if (userList.size() > 0)
		 {
			 //显示第一名
			 const TaskUser* pUser = SocialInfoManager::getInstance()->getUserInfor(userList[0]);
			 if (! pUser) continue;
			 TaskUser value = *pUser;
			 CCSprite* sp=getByyshuandid(sectionId);
			 if(sp){
				 CCSprite* back_s=CCSprite::create(ResManager::getManager()->getFilePath(g_public+"GQ_touxiang_bg.png").c_str());
				 back_s->setPosition(CCPoint(sp->getContentSize().width/2,sp->getContentSize().height*1.05f));
				 back_s->setTag(netback_spritetag);
				 sp->addChild(back_s);

				 string default_str=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
				 urlSprite* sp_s=urlSprite::create(default_str.c_str(),value.pictrue.c_str());
				 sp_s->setTag(net_spritetag);
				 sp_s->setPosition(CCPoint(sp->getContentSize().width/2,sp->getContentSize().height*1.05f));
				 sp_s->setZOrder(100);
				 sp->addChild(sp_s);
				 setSpriteClip(sp);
				 setSpriteClip(sp_s);
				 setSpriteClip(back_s);

				 //float scale=1.2f;
				 if(back_s->getContentSize().width>sp_s->getContentSize().width){
					 //back_s->setScale(1.2f);
				 }else{
					 back_s->setScale(sp_s->getContentSize().width/back_s->getContentSize().width+0.1f);
				 }

			 }
		 }else {
			 CCSprite* sp=getByyshuandid(sectionId);
			 if(sp){
				 sp->removeChildByTag(netback_spritetag);
				 sp->removeChildByTag(net_spritetag);
			 }
		 }
	 }
 }



 void Xiaogktablelist::setcurrentId(int season){

	 this->current_season=season;
	 SeasonInfor* s_msg=GameInforEditer::getGameInfor()->getSeasonInfor(season);
	 if(s_msg){
		 list<SectionInfor*> lsitseinfo= s_msg->getAllSectionInfors();
		 list<SectionInfor*>::iterator it;
		 for (it = lsitseinfo.begin(); it!=lsitseinfo.end(); it++) {
			 msg_setion.push_back(*it);
		 }
	 }
	 drawLayer();
	 netDatainint();
	 this->prent->setHesepoint(current_page);
 }

 //初始化
 void Xiaogktablelist::drawLayer(){

	 CCSize m_size=CCDirector::sharedDirector()->getWinSize();
	 scrow_size=CCSize(m_size.width, 700);

	 createGradientClipShader((m_size.width - scrow_size.width)/2, (m_size.width + scrow_size.width)/2);
	 scroll_layer = CCLayer::create();  
	 scroll_layer->setPosition(CCPoint(0,0));  
	 scroll_layer->setAnchorPoint(CCPoint(0.0f,0.0f));  
	 scroll_layer->setContentSize(CCSize((scrow_size.width)*getYeshu()+(getYeshu()-1)*xiaogk_jg,scrow_size.height));
	 scrollView = CCScrollView::create(scrow_size,scroll_layer);
	 scrollView->setDelegate(this);
	 scrollView->setContentOffset(CCPoint(0,0));
	 scrollView->setTouchEnabled(false);
	 scrollView->setDirection(kCCScrollViewDirectionHorizontal);
	 scrollView->ignoreAnchorPointForPosition(false);
	 scrollView->setAnchorPoint(CCPoint(0.5,0.5));
	 scrollView->setPosition(ccp(size.width/2, size.height/2));  
	 addChild(scrollView,20);  
	 for(int i=0;i<getYeshu();i++){
		
		  drowList(CCPoint((scrow_size.width+xiaogk_jg)*i+(scrow_size.width)/2,scrow_size.height/2),i);
		
	 }
	 int last=LevelManager::getManager()->getLastSectionId();
	 if(last%layernum==0){
		 current_page=last/layernum;
	 }else{
		 current_page=last/layernum+1;
	 }
	 {
		 CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0);  
		 scrollView->setContentOffset(adjustPos,true);
	 }
	 old_page=current_page;
	 ju_li=scrow_size.width+xiaogk_jg;

	 refreshPagelable();
 }

 void Xiaogktablelist::dingwei(){

	 //refresh_page=current_page;

	 int last=GameInforEditer::getGameInfor()->getLastClickSectionId(current_season);
	 int page=0;
	 if(last%layernum==0){
		 current_page=last/layernum;
	 }else{
		 current_page=last/layernum+1;
	 }
	 CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0);  
	 scrollView->setContentOffset(CCEaseExponentialOut::create(CCMoveTo::create(0.1f, adjustPos)));
	 this->schedule(schedule_selector(Xiaogktablelist::updateCs),0.15f);//之所以是0.65f是因为上面的动作需要0.6f 小于0.6 会破坏curent_page
 }

 void Xiaogktablelist::updateCs(float dt){
     
	 this->prent->jianxiao(current_page);
	 this->unschedule(schedule_selector(Xiaogktablelist::updateCs));
	 if (LevelManager::getManager()->getSlideToPage3())
	 {
		 if (LevelManager::getManager()->getShowUnlockAni())
		 {
			 vector<CCSprite*> sps=msg_mapsps.at(current_page+1);
			 for (size_t i = 0; i < sps.size(); ++i)
			 {
				 CCSprite* p = sps.at(i);
				 p->setOpacity(0);
				 p->setVisible(false);

				 CCSprite* pSpr = CCSprite::createWithTexture(batchnodemaps.at(yinchnag_suoding)->getTexture());
				 p->getParent()->addChild(pSpr, 1, tag_start+i);
				 pSpr->setPosition(p->getPosition());
			 }
		 }
	 }

	 this->scheduleOnce(schedule_selector(Xiaogktablelist::slideToPage3), 1.0f);
 }

 void Xiaogktablelist::slideToPage3(float dt)
 {
	 if (LevelManager::getManager()->getSlideToPage3())
	 {
		 LevelManager::getManager()->setSlideToPage3(false);
		 int last=GameInforEditer::getGameInfor()->getLastClickSectionId(current_season)+1;
		 int page=0;
		 if(last%layernum==0){
			 current_page=last/layernum;
		 }else{
			 current_page=last/layernum+1;
		 }

		 CCPoint adjustPos = CCPoint( - (current_page - 1) * (scrow_size.width+xiaogk_jg),0);  
		 scrollView->setContentOffset(CCSequence::create(
			 CCEaseExponentialOut::create(CCMoveTo::create(0.8f, adjustPos)),
			 CCCallFunc::create(this, callfunc_selector(Xiaogktablelist::slideToPage3Over)),
			 NULL));
	 }
 }
 

 void Xiaogktablelist::slideToPage3Over()
 {
	 if (!LevelManager::getManager()->getShowUnlockAni())
	 {
		 playLockInfoAnimation();
		 return;
	 }
	 LevelManager::getManager()->setShowUnlockAni(false);
	 float time = 0.8f;
	  vector<CCSprite*> sps=msg_mapsps.at(current_page);
	  for (size_t i = 0; i < sps.size(); ++i)
	  {
		  CCSprite* p = sps.at(i);
		  p->setVisible(true);

		  p->runAction(CCFadeIn::create(time));

		  CCSprite* pSpr = (CCSprite*)p->getParent()->getChildByTag(tag_start+i);
		  pSpr->runAction(CCSequence::create(CCFadeOut::create(time), CCRemoveSelf::create(), NULL));
	  }
	  CCUserDefault::sharedUserDefault()->setBoolForKey(Show_Unlock_Animation, true);
	  CCUserDefault::sharedUserDefault()->flush();

	  CCLayer* layer=lockstate_layers.at(current_page);
	  layer->getChildByTag(yinchanglockstate_tag)->setVisible(false);
 }

 void Xiaogktablelist::playLockInfoAnimation()
 {
	 if (lockstate_layers.size() >= 3)
	 {
		 CCLayer* layer = lockstate_layers.at(3);

		 CCLabelTTF* l_msg=(CCLabelTTF*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklable_tag);
		 if (l_msg)
		 {
			 l_msg->stopAllActions();
			 l_msg->setScale(1.0f);
			 l_msg->runAction(CCSequence::create(
				 CCDelayTime::create(0.1f),
				 CCRepeat::create(CCSequence::create(
				 CCScaleTo::create(0.15f, 0.5f),
				 CCScaleTo::create(0.15f, 1.0f),
				 NULL), 2), NULL));
		 }
		 CCLabelTTF* l_msg_shuzi=(CCLabelTTF*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklable_shuzi_tag);
		 if (l_msg_shuzi)
		 {
			 l_msg_shuzi->stopAllActions();
			 l_msg->setRotation(0.0f);
			 l_msg_shuzi->runAction(CCSequence::create(
				 CCDelayTime::create(0.1f),
				 CCRepeat::create(CCSequence::create(
				 CCRotateTo::create(0.05f, 20),
				 CCRotateTo::create(0.05f, 0),
				 CCRotateTo::create(0.05f, -20),
				 CCRotateTo::create(0.05f, 0),
				 NULL), 3), NULL));
		 }
		 //
		 CCSprite* sps=(CCSprite*)layer->getChildByTag(yinchanglockstate_tag)->getChildByTag(yinchnaglocklablesps_tag);
		 if (sps)
		 { 
			 sps->stopAllActions();
			 sps->setRotation(0.0f);
			 sps->runAction(CCSequence::create(
				 CCDelayTime::create(0.1f),
				 CCRepeat::create(CCSequence::create(
				 CCRotateTo::create(0.05f, 20),
				 CCRotateTo::create(0.05f, 0),
				 CCRotateTo::create(0.05f, -20),
				 CCRotateTo::create(0.05f, 0),
				 NULL), 3), NULL));
		 }
	 }
 }

 int Xiaogktablelist::getYeshu(){

	 int yeshu=0;
	 if(getByseasoncheck(current_season)%layernum==0){
		 yeshu=getByseasoncheck(current_season)/layernum;
	 }else{
		 yeshu=getByseasoncheck(current_season)/layernum+1;
	 }
	 return yeshu;

 }

 int Xiaogktablelist::getByseasoncheck(int season){

	 //GameInforEditer
	 int num=GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(current_season);
	 return num;

 }


 /*
 lockstate=100, //解锁通过
	 unlockstate,   //解锁未通过
	 hidden,        //隐藏
	 no_state,     //未解锁
*/
 void Xiaogktablelist::drowList(CCPoint p,int page){

	 auto layer=CCLayer::create();
	 layer->setContentSize(scrow_size);// 
	 layer->ignoreAnchorPointForPosition(false);
	 layer->setAnchorPoint(CCPoint(0.5,0.5));
	 layer->setPosition(p);
	 scroll_layer->addChild(layer,1);

	 lockstate_layers.insert(make_pair(page+1,layer));

	 if(page==2){

		 CCSprite* sp=CCSprite::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_yincang_biaoti.png").c_str());
		 sp->setPosition(CCPoint(layer->getContentSize().width/2, layer->getContentSize().height*0.75f));
		 layer->addChild(sp);

		 //yinchanglable_tag
		 string str_yc=LFStrings::getValue("lockbiaoT");
		 CCLabelTTF* yinc_l = CCLabelTTF::create(str_yc.c_str(),fontStr_KlavikaBold,80);
		 yinc_l->setPosition(CCPoint(layer->getContentSize().width/2, sp->getPositionY()));
		 yinc_l->setColor(ccc3(59,1,117));
		 yinc_l->setTag(yinchanglable_tag);
		 layer->addChild(yinc_l);

		 //yinchanglockstate_tag,
		 //yinchnaglocklable_tag,
		 CCNode* layer_st=CCNode::create();
		 layer_st->setTag(yinchanglockstate_tag);
		 layer_st->setPosition(CCPoint(sp->getPositionX(), sp->getPositionY() + sp->getContentSize().height*0.75f));
		 layer->addChild(layer_st);
		 string lock_msglable=LFStrings::getValue("lockmsg"); //CCString::createWithFormat("%d",page*8+i)->getCString();
		 string current_stars=CCString::createWithFormat("%d", GameInforEditer::getGameInfor()->getTotalScoreBeforeSeasonId(current_season))->getCString();
		 const ConfigSeasonInfo* info=GameLayerFactory::getInstance()->getConfigSeasonInfo(current_season);
		 string need_stars=CCString::createWithFormat("%d", info->iDeblockNum)->getCString();
		 //lock_msglable+=current_stars+"/"+need_stars;

		 CCSprite* sp_locksprite=CCSprite::create(ResManager::getManager()->getFilePath(g_xiaogk+"GQ_yincang_biaoshi.png").c_str());
		 sp_locksprite->setAnchorPoint(CCPoint(0.0f, 0.5f));
		 sp_locksprite->setTag(yinchnaglocklablesps_tag);
		 layer_st->addChild(sp_locksprite);

		 CCSprite* sp_xing=CCSprite::create(ResManager::getManager()->getFilePath(g_xiaogk+"YX_start.png").c_str());
		 sp_xing->setTag(yinchnagxing_tag);
		 layer_st->addChild(sp_xing);

		 CCLabelTTF* l_msg = CCLabelTTF::create(lock_msglable.c_str(),fontStr_KlavikaBold,30);
		 l_msg->setColor(ccc3(237,229,180));
		 l_msg->setTag(yinchnaglocklable_tag);
		 layer_st->addChild(l_msg);
		 l_msg->setVisible(false);

		 CCLabelTTF* l_msg_shuzi = CCLabelTTF::create((" "+current_stars+"/"+need_stars).c_str(),fontStr_KlavikaBold,30);
		 l_msg_shuzi->setColor(l_msg->getColor());
		 l_msg_shuzi->setTag(yinchnaglocklable_shuzi_tag);
		 layer_st->addChild(l_msg_shuzi);

		 float x=(layer_st->getContentSize().width-(sp_locksprite->getContentSize().width+l_msg->getContentSize().width))/2;

		 l_msg->setPosition(ccp(x+l_msg->getContentSize().width/2, sp_locksprite->getContentSize().height));
		 l_msg_shuzi->setPosition(ccp(l_msg->getPositionX(), l_msg->getPositionY() - l_msg->getContentSize().height/2 - l_msg_shuzi->getContentSize().height/2 - 10));
		 sp_locksprite->setPosition(ccp(l_msg_shuzi->getPositionX()+l_msg_shuzi->getContentSize().width/2+sp_locksprite->getContentSize().width/2 , l_msg_shuzi->getPositionY()));
		 sp_xing->setPosition(ccp(l_msg_shuzi->getPositionX()-l_msg_shuzi->getContentSize().width/2-sp_xing->getContentSize().width/2 , l_msg_shuzi->getPositionY()));
	 }


	 layer_map.insert(make_pair(page+1,layer));
	 string kongx=ResManager::getManager()->getFilePath(g_xiaogk+"YX_start_kong.png");
	 vector<CCSprite*> sps;
	 int mark=0;
	 for(int i=1;i<=layernum;i++){

		 if(page*8+i>getByseasoncheck(current_season)){
			 break;
		 }
		 string bg_file="";
		 mark=0;
		 getMarkbackstr(bg_file,mark,page,i);
		 if(page*8+i<=getByseasoncheck(current_season)){

			 CCSprite* bg=CCSprite::create(bg_file.c_str());
			 sps.push_back(bg);
			 float p_x=0;
			 float p_y=0;
			 p_x= bg->getContentSize().width + 100;//(scrow_size.width-(bg->getContentSize().width*4))/3;
             float x_offset = 50 + CCDirector::sharedDirector()->getWinSize().width/2 - designResolutionSize.width/2;


			 if(page==0&&i==1){
				 sp_quyu.sprite_size=bg->getContentSize();
				 sp_quyu.distance_one=p_x;
			 }
			 if(i<=4){

				 if(page*8+i<=16){
				   p_y=scrow_size.height/2+bg->getContentSize().height/2+ 50;
				   bg->setPosition(CCPoint(bg->getContentSize().width + x_offset + (p_x)*(i-1),p_y));
				 }else if(page*8+i==17){

				   bg->setPosition(CCPoint(layer->getContentSize().width*0.3f,layer->getContentSize().height/2));

				 }else if(page*8+i==18){

					bg->setPosition(CCPoint(layer->getContentSize().width*0.425f,layer->getContentSize().height*0.5f-bg->getContentSize().height*0.425f));

				 }else if(page*8+i==19){

					 bg->setPosition(CCPoint(layer->getContentSize().width*0.575f,layer->getContentSize().height/2));

				 }else if(page*8+i==20){
					 bg->setPosition(CCPoint(layer->getContentSize().width*0.7f,layer->getContentSize().height*0.5f-bg->getContentSize().height*0.425f));


				 }


			 }else{
				 p_y=scrow_size.height/2 - bg->getContentSize().height/2 - 50;
				 bg->setPosition(CCPoint(bg->getContentSize().width + x_offset + (p_x)*((i-4)-1),p_y));
			 }
			 layer->addChild(bg);
			 setSpriteClip(bg);

			 string start=CCString::createWithFormat("%d",page*8+i)->getCString();
			 CCLabelTTF* st = CCLabelTTF::create(start.c_str(),fontStr_KlavikaBold,60);
			 setSpriteClip(st);
			 st->setPosition(CCPoint(bg->getContentSize().width/2,bg->getContentSize().height*0.6));
			 st->setColor(colors.at(current_season));
			 st->setTag(optical_cardlabel);
			 bg->addChild(st);
			 //CCRenderTexture* render = LF::createRenderTextureFromLabel(st);
			 //LF::cutoutText(render, prent->getBacksprite(), st);
			 st->setVisible(false);

			 if(mark==lockstate){
				st->setVisible(true);
			 }else if(mark==unlockstate){
				st->setVisible(true);
			 }else if(mark==no_state){

			 }else if(mark==hidden){

			 }

			 CCLayer* locklayer=CCLayer::create();
			 locklayer->setTag(locklayertag);
			 locklayer->setContentSize(bg->getContentSize());
			

			 for(int j=0;j<3;j++){
				 CCSprite* x_daoju=CCSprite::create(kongx.c_str());
				 setSpriteClip(x_daoju);
				 float jg_x=(bg->getContentSize().width-(x_daoju->getContentSize().width*3+(3-1)*5))/2;


				 if(page*8+i<=16){
				      x_daoju->setPosition(CCPoint(jg_x+x_daoju->getContentSize().width/2+(x_daoju->getContentSize().width+5)*j,bg->getContentSize().height*0.125));
				 }else{
					 float chushigaodu=bg->getContentSize().height*0.48f;
					 float end_hight=chushigaodu-(j)*x_daoju->getContentSize().height-x_daoju->getContentSize().height/2;
					 x_daoju->setPosition(CCPoint(x_daoju->getContentSize().width/2+(j)*(x_daoju->getContentSize().width-5),end_hight+x_daoju->getContentSize().height/2));

				 }
				 locklayer->addChild(x_daoju);
			 }
			 if(page*8+i>=17){
				 locklayer->ignoreAnchorPointForPosition(false);
				 locklayer->setAnchorPoint(CCPoint(0.5f,0.5f));
				 locklayer->setPosition(CCPoint(bg->getContentSize().width/2-20,bg->getContentSize().height/2-25));
			 }
			 drowlockstate(locklayer,page,i);
			 bg->addChild(locklayer);
			

			 CCLayer* unlocklayer=CCLayer::create();
			 unlocklayer->setTag(unlocklayertag);
			 unlocklayer->setContentSize(bg->getContentSize());
			 drowunlockstate(unlocklayer);
			 bg->addChild(unlocklayer);
			  if(mark==lockstate){
				  unlocklayer->setVisible(false);
			  }else if(mark==unlockstate){
				  locklayer->setVisible(false);
			  }else if(mark==no_state){
				  unlocklayer->setVisible(false);
				  locklayer->setVisible(false);
			  }else if(mark==hidden){
				  unlocklayer->setVisible(false);
				  locklayer->setVisible(false);
			  }

			  if(page*8+i>=17){
				  unlocklayer->setVisible(false);
			  }


		 }
	 }
	 msg_mapsps.insert(make_pair(page+1,sps));
 }



 void Xiaogktablelist::drowunlockstate(CCLayer* bg){

	 string start=LFStrings::getValue("start");
	 CCLabelTTF* st = CCLabelTTF::create(start.c_str(),fontStr_KlavikaBold,30);
	 st->setPosition(CCPoint(bg->getContentSize().width/2,bg->getContentSize().height*0.125));
	 //st->setColor(colors.at(current_season));
	 st->setTag(start_colortag);
	 bg->addChild(st);
	 setSpriteClip(st);
 }

 void Xiaogktablelist::drowlockstate(CCLayer* bg,int page,int i){

	 string xinxi=ResManager::getManager()->getFilePath(g_xiaogk+"YX_start.png");
	 int num_fenshu= 0;
	 size_t index = page*8+i;
	 //if (index <= msg_setion.size())
	 //{
		 for(size_t m=0;m<msg_setion.size();m++){
			 if(msg_setion.at(m)->sectionId==index&& msg_setion.at(m)->isSuccessed==true){
				 num_fenshu = msg_setion.at(m)->iStarNum;
				 break;
			 }
		 }
	 //}
	 
	 for(int j=0;j<3;j++){
		 string true_str=xinxi;
		 CCSprite* x_daoju=CCSprite::create(true_str.c_str());
		 x_daoju->setTag(star_tag+j);
		 x_daoju->setVisible(false);
		 setSpriteClip(x_daoju);
		 float jg_x=(bg->getContentSize().width-(x_daoju->getContentSize().width*3+(3-1)*5))/2;

		 if(page*8+i<=16){
			 x_daoju->setPosition(CCPoint(jg_x+x_daoju->getContentSize().width/2+(x_daoju->getContentSize().width+5)*j,bg->getContentSize().height*0.125));
		 }else{
			 float chushigaodu=bg->getContentSize().height*0.48f;
			 float end_hight=chushigaodu-(j)*x_daoju->getContentSize().height-x_daoju->getContentSize().height/2;
			 x_daoju->setPosition(CCPoint(x_daoju->getContentSize().width/2+(j)*(x_daoju->getContentSize().width-5),end_hight+x_daoju->getContentSize().height/2));

		 }
		 bg->addChild(x_daoju);
		 if(j<num_fenshu){
			 x_daoju->setVisible(true);
		 }else{
			 x_daoju->setVisible(false);
		 }

	 }

	

 }

 bool Xiaogktablelist::isJiesuo(int key,bool mark){

	 bool cs_mark=false;
	 if(mark==true){
		 if (GameInforEditer::getGameInfor()->isIgnoreAllLock())
		 {
			 cs_mark=true;
		 }
		 for(size_t i=0;i<msg_setion.size();i++){
			 if(msg_setion.at(i)->sectionId==key){
				 cs_mark=true;
				 break;
			 }
		 }
	 }
	 if(mark==false){
		 for(size_t i=0;i<msg_setion.size();i++){
			 if(msg_setion.at(i)->sectionId==key&& msg_setion.at(i)->isSuccessed==true){
				 cs_mark=true;
				 break;
			 }
		 }
	 }
	 return cs_mark;
 }

 int Xiaogktablelist::getNumscore(){

	 vector<SectionInfor*> msg_setionnum;
	 for(int i=1;i<=current_season;i++){
		 SeasonInfor* s_msg=GameInforEditer::getGameInfor()->getSeasonInfor(i);
		 if(s_msg){
			 list<SectionInfor*> lsitseinfo= s_msg->getAllSectionInfors();
			 list<SectionInfor*>::iterator it;
			 for (it = lsitseinfo.begin(); it!=lsitseinfo.end(); it++) {
				 msg_setionnum.push_back(*it);
			 }
		 }
	 }
	 int num=0;
	 for(size_t i=0;i<msg_setionnum.size();i++){
		 num+=msg_setionnum.at(i)->iStarNum;
	 }
	 //是否加上以买星星数
	 num+=0;
	 return num;
 }


 void  Xiaogktablelist::getImagecolor4Bpos(CCImage *p,CCPoint point,int idx)
 {


	
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

 /*
 //代码示例：
 CCImage *pImage = new CCImage();
 pImage->initWithImageFile("images.png",CCImage::kFmtPng);
 ccColor4B colorOfPos = getImageColorOfPos(pImage, ccp(0,0));
 pImage->release();
 */