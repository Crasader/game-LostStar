#include "Biguangka.h"
#include "Debug.h"
#include "VersionControl.h"
#include "font.h"
#include "LFStrings.h"
#include "ResManager.h"
#include "UserDataManager.h"
#include "GameInforEditer.h"
#include "LevelManager.h"
#include "Myobject.h"
#include "Uimsg.h"
#include "GameLayerFactory.h"
#include "Starseneui.h"
#include "Starmain.h"
#include "Shieldingmanger.h"
#include "AppMacros.h"
#include "LFUtile.h"
#include "Music.h"

#include "WrapperManager.h"

enum Buttontag
{

	start_tag=100,
	fah_maintag,

	Tag_friendProgressPicture_bg = 1000,
	Tag_friendProgressPicture = 1100,
};

enum Bigtype{

	 sprite_tag=10,
	 menu_tag,
	 menu_butoontag,
	 scal9_tag,
	 scale_tag,
	 startlable_tag,
	 numstarlable_tag,
	 numstaricon_tag,
	 plableonetag,
	 plabletwotag,

	 bigleft_tag,
	 bigright_tag,

	 Progress_tag,
};

Biguangka::Biguangka(void)
{

}

Biguangka::Biguangka(int season)
{
	this->curren_season=season;
}

Biguangka::~Biguangka(void)
{
 
}

bool Biguangka::init(){
	if(!CCLayer::init()){
		return false;
	}
	itemleft = NULL;
	itemright = NULL;
	back_ph = NULL;

	int iCount = GameLayerFactory::getInstance()->getValidSeasonNum();
	for(int i=1;i<=iCount;i++){
		int index = i;
		string back_str=CCString::createWithFormat("ZJ_bg_season%d.png",index)->getCString();
		back_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
		CCImage *pImage = new CCImage();
		pImage->initWithImageFile(back_str.c_str(),CCImage::kFmtPng);
		getImagecolor4Bpos(pImage,ccp(1,384),index);
	}

	m_iLastChangBgPage = 1;
	datainint();
	drawPublicback();
	//_hasSnsLayerCreated = false;

	//if(CmdHelper::getInstance()->isLogin())
	{
		drawsnsPicture();
	}
	
	return true;
}

void Biguangka::onEnter(){

	CCLayer::onEnter();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Biguangka::refreshlaguage),MSG_LAGUAGEBig, NULL);

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Biguangka::updateMsglayer), NetMessage::MSG_SyncUserDataEnd, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Biguangka::updateMsglayer), NetMessage::MSG_GotSeasonSocialInfo, NULL);

}

void Biguangka::onExit(){
	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Biguangka::updateMsglayer(CCObject* object)
{
	int lastId = LevelManager::getManager()->getLastSeasonId();
	this->setCurrentseason(lastId);
	getBiglisttable()->scrollToSeason(lastId);

	refreshLockButton(object);
	refreshSnsProgress();
}

void Biguangka::refreshLockButton(CCObject* object)
{
	GameInforEditer* infor=GameInforEditer::getGameInfor();

	for( int i = 0; i < maxseson; i++)
	{
		int id = i + 1;
		SeasonInfor* seasonInfor = infor->getSeasonInfor(id);
		bool locked = (seasonInfor == NULL || seasonInfor->locked) && !GameInforEditer::getGameInfor()->isIgnoreAllLock();
		CCMenuItemImage* item		= _startButtons.at(i);
		CCMenuItemImage* itemLock	= _lockButtons.at(i);
		item->setVisible(!locked);
		itemLock->setVisible(locked);

		_levelNums.at(i)->setVisible(!locked);
		_levelLocks.at(i)->setVisible(locked);

#if Version_Free != 0
		if (i == (maxseson-1))
		{
			item->setVisible(false);
			itemLock->setVisible(false);
			_levelNums.at(i)->setVisible(false);
			_levelLocks.at(i)->setVisible(false);
		}
#endif

	}
	
}

void Biguangka::refreshlaguage(CCObject* object){

	if (back_ph)
	{
		if (CCLabelTTF* pLabel = (CCLabelTTF*)back_ph->getChildByTag(Progress_tag))
		{
			pLabel->setString(LFStrings::getValue("Pro").c_str());
		}
	}

	for(int i=1;i<=GameLayerFactory::getInstance()->getValidSeasonNum();i++){
		vector<CCSprite*> spvs=sprite_map.at(i);
		for(size_t j=0;j<spvs.size();j++){
			if(spvs.at(j)->getTag()==startlable_tag){
				CCLabelTTF* p=(CCLabelTTF*) spvs.at(j);
				p->setString(LFStrings::getValue("start").c_str());

#if Show_End_BigGuanQia != 0
	if (i == maxseson)
	{
		p->setVisible(false);
	}
#endif
			}else if(spvs.at(j)->getTag()==numstarlable_tag){
				CCLabelTTF* p=(CCLabelTTF*) spvs.at(j);
				string num_str=CCString::createWithFormat(" %d",GameInforEditer::getGameInfor()->getTotalFreeScore())->getCString();
				p->setString(num_str.c_str());

				CCSprite* pIcon = (CCSprite*)p->getParent()->getChildByTag(numstaricon_tag);
				pIcon->setPositionX(size.width/2 - pIcon->getContentSize().width/2-p->getContentSize().width/2 + 5.0f);
				p->setPosition(ccp(pIcon->getPositionX()+pIcon->getContentSize().width/2 + 5, pIcon->getPositionY()));
			}
		}
	}
	biglist->refreshLanguge();
}

Biguangka*  Biguangka::creteByseason(int season){

	Biguangka* pRet=new Biguangka(season);
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}
}

int Biguangka::getByseasoncheck(int season){

	//GameInforEditer
	int num=GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(biglist->getCurntId());
	return num;

}



void Biguangka::netDatainint(int current_season){
    
	 
	 //const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(current_season,i);

}

void Biguangka::drawsnsPicture(){

// 	if(_hasSnsLayerCreated)
// 	{
// 		return;
// 	}
	 string pictures[2]={ResManager::getManager()->getFilePath(g_public+"ZJ_haoyou_bg.png"),ResManager::getManager()->getFilePath(g_public+"GQ_touxiang_bg.png")};

	 back_ph=CCSprite::create(pictures[0].c_str());
	 back_ph->setPosition(CCPoint(back_ph->getContentSize().width/2,size.height/2));
	 addChild(back_ph);

	 string bt_str=LFStrings::getValue("Pro");
	 CCLabelTTF* bT = CCLabelTTF::create(bt_str.c_str(),fontStr_KlavikaBold,35);
	 bT->setAnchorPoint(CCPoint(0.0f,0.0f));
	 bT->setPosition(CCPoint(0,back_ph->getContentSize().height*0.85f));
	 back_ph->addChild(bT, 0, Progress_tag);

	 for(int i=1;i<=GameLayerFactory::getInstance()->getValidSeasonNum();i++){
		 CCLayer* layer=CCLayer::create();
		 layer->setContentSize(back_ph->getContentSize());
		 drawUrlpiture(i,layer);
		 back_ph->addChild(layer);
		 layers_map.insert(make_pair(i,layer));
	 }

	 //_hasSnsLayerCreated = true;
}

void Biguangka::drawUrlpiture(int k,CCLayer* layer){

	vector<urlSprite*> sps;
	float hight_p=0.25f;

	map<string,unsigned int> progressMap = SocialInfoManager::getInstance()->getFriendProgressBySeason(k);
	int idx=0;
	for(map<string,unsigned int>::iterator itor = progressMap.begin(); itor != progressMap.end() && idx < 3; ++itor, ++idx){

		CCSprite* back_s=CCSprite::create(ResManager::getManager()->getFilePath(g_public+"GQ_touxiang_bg.png").c_str());
		back_s->setPosition(CCPoint(back_ph->getContentSize().width/2,back_ph->getContentSize().height*(1-(idx+1)*hight_p)));
		layer->addChild(back_s,100,Tag_friendProgressPicture_bg+idx);

		urlSprite*  m=urlSprite::create(ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png").c_str(),
			SocialInfoManager::getInstance()->getUserInfor(itor->first)->pictrue.c_str());
		m->setPosition(CCPoint(back_ph->getContentSize().width/2,back_ph->getContentSize().height*(1-(idx+1)*hight_p)));
		layer->addChild(m,1000,Tag_friendProgressPicture+idx);
		sps.push_back(m);

		//float scale=1.2f;
		if(back_s->getContentSize().width>m->getContentSize().width){
			//back_s->setScale(1.2f);
		}else{
			back_s->setScale(m->getContentSize().width/back_s->getContentSize().width+0.1f);
		}

		if (k != curren_season)
		{
			back_s->setVisible(false);
			m->setVisible(false);
		}
	}

    frinedprogress_maps.insert(make_pair(k,sps));//更新用
}

void Biguangka::refreshSnsProgress()
{
	frinedprogress_maps.clear();
	for (map<int,CCLayer*>::iterator it = layers_map.begin(); it != layers_map.end(); ++it)
	{
		it->second->removeAllChildren();
		drawUrlpiture(it->first, it->second);
	}
}


void Biguangka::datainint(){
	size=CCDirector::sharedDirector()->getWinSize();
	maxseson=GameLayerFactory::getInstance()->getValidSeasonNum();
}

void Biguangka::drawPublicback(){
	 
	_startButtons.clear();
	_lockButtons.clear();
	_levelNums.clear();
	_levelLocks.clear();
	 for(int i=1;i<=maxseson;i++){
		 CCLayer* layer=CCLayer::create();
		 layer_map.insert(make_pair(i,layer));
		 drawLayer(i);
		 if(i!=curren_season){
			 Fadeoutbig(i);
			
		 }
	 }
	 this->scheduleUpdate();
	 biglist=Biglisttable::create();
	 biglist->ignoreAnchorPointForPosition(false);
	 biglist->setAnchorPoint(CCPoint(0.5f,0.5f));
	 biglist->setPosition(CCPoint(size.width*0.5f,size.height*0.55f));
	 biglist->setBiguangka(this);
	 addChild(biglist);
	 //动态
	 string fahui_img="ZJ_anniu_fanhui.png";
	 fahui_img=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+fahui_img);
	 CCMenuItemImage* button_fh=CCMenuItemImage::create(
		 fahui_img.c_str(),
		 fahui_img.c_str(),
		 this,
		 menu_selector(Biguangka::allCallback));
	 button_fh->setTag(fah_maintag);
	 button_fh->setPosition(ccp(size.width*(buton_jiangju),button_fh->getContentSize().height/2+5));
	 fillOuty(button_fh,fahui_img);
	 CCMenu* pMenu = CCMenu::create(button_fh,NULL);
	 pMenu->setPosition(CCPointZero);
	 addChild(pMenu,1);
	 //ZJ_fenye_biaoshi.png ZJ_fenye_biaoshi_bg.png
	 //黑色底
	 heseSp=CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_fenye_biaoshi_bg.png").c_str());
	 addChild(heseSp);
	 for(int i=0;i<maxseson;i++){
		 CCSprite* sp0=CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_fenye_biaoshi.png").c_str()); 
		 float start_spx=(size.width-(sp0->getContentSize().width*maxseson+(maxseson-1)*Big_sp_juli))/2;
		 sp0->setPosition(CCPoint(start_spx+sp0->getContentSize().width/2+(i)*(sp0->getContentSize().width+Big_sp_juli),button_fh->getPositionY()));
		 addChild(sp0);
		 backs_v.push_back(sp0);
		 if(curren_season==i+1){
			 heseSp->setPosition(sp0->getPosition());
		 }else{
			 sp0->setOpacity(num_Fade/2);
		 }
		 if(i<maxseson-1){
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

	 //左右滑动列表
	 //选折箭头
	 string left_str="ZJ_anniu_fanye.png";
	 left_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+left_str);

	 itemleft=CCMenuItemImage::create(
		 left_str.c_str(),
		 left_str.c_str(),
		 this,
		 menu_selector(Biguangka::allCallback));
	 itemleft->setTag(bigleft_tag);
	 itemleft->setRotation(135.0f);
	 itemleft->setPosition(ccp(
		 size.width*0.2f
		 ,size.height*0.5f));

	 itemright=CCMenuItemImage::create(
		 left_str.c_str(),
		 left_str.c_str(),
		 this,
		 menu_selector(Biguangka::allCallback));
	 itemright->setTag(bigright_tag);
	 itemright->setRotation(-45.0f);
	 itemright->setPosition(ccp(
		 size.width*0.8f
		 ,size.height*0.5f));

	 pMenu->addChild(itemleft);
	 pMenu->addChild(itemright);

}

void  Biguangka::getImagecolor4Bpos(CCImage *p,CCPoint point,int idx)
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

void Biguangka::drawLayer(int id){
	 
	 vector<CCSprite*> spvs;
	 vector<CCMenuItemSprite*> menusps;
	 CCLayer* layer=layer_map.at(id);
	 layer->setContentSize(size);
	 addChild(layer);
	 //主背景
	 string back_str=CCString::createWithFormat("ZJ_bg_season%d.png",id)->getCString();
	 back_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
	 CCScale9Sprite* back_s=CCScale9Sprite::create(back_str.c_str());
	 CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	 back_s->setAnchorPoint(ccp(0.5,0.5));
	 back_s->ignoreAnchorPointForPosition(false);
	 back_s->setPosition(ccp(size.width/2,size.height/2));
	 back_s->setContentSize(size);
	 back_s->setTag(scal9_tag);
	 layer->addChild(back_s);
	 scla9_map.insert(make_pair(id,back_s));


	 /*
	 //底框
	 //背景框
	 string db_str=CCString::createWithFormat("ZJ_bg_season%d_kuang.png",id)->getCString();
	 db_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+db_str);

	 CCSprite* tempspr = CCSprite::create(db_str.c_str());

	 CCScale9Sprite* d_ba= CCScale9Sprite::create(CCRectMake(1, tempspr->getContentSize().height, 1, tempspr->getContentSize().height), db_str.c_str());
	 d_ba->setTag(sprite_tag);
	 d_ba->setContentSize(CCSizeMake(CCDirector::sharedDirector()->getWinSize().width - 300, tempspr->getContentSize().height));
	 d_ba->setPosition(CCPoint(size.width/2,size.height/2));
	 back_s->addChild(d_ba);
	 dk_width=d_ba->getContentSize().width;
	 scla9_kuang_map.insert(make_pair(id,d_ba));
	 */
//	 spvs.push_back(d_ba);

//	 d_ba->setVisible(false);

	 string xing=ResManager::getManager()->getFilePath(g_xiaogk+"YX_start.png");
	 CCSprite* pIcon = CCSprite::create(xing.c_str());
	 back_s->addChild(pIcon, 1025, numstaricon_tag);
	 pIcon->setPosition(ccp(size.width/2 - pIcon->getContentSize().width/2 + 5.0f, size.height-pIcon->getContentSize().height/2-10));
	 spvs.push_back(pIcon);

	 //总星数
	 string num_str=CCString::createWithFormat(" %d",GameInforEditer::getGameInfor()->getTotalFreeScore())->getCString();
	 CCLabelTTF* star_l = CCLabelTTF::create(num_str.c_str(),fontStr_KlavikaBold,40);
	 star_l->setAnchorPoint(ccp(0.0f, 0.5f));
	 star_l->setTag(numstarlable_tag);
	 //star_l->setTag(scale_tag);
	 back_s->addChild(star_l);
	 spvs.push_back(star_l);

	 pIcon->setPositionX(pIcon->getPositionX()-star_l->getContentSize().width/2);
	 star_l->setPosition(ccp(pIcon->getPositionX()+pIcon->getContentSize().width/2 + 5, pIcon->getPositionY()));

	 string yuan_img=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_shumu_bg.png");
	 CCSprite* yuan_sp=CCSprite::create(yuan_img.c_str());
	 yuan_sp->setTag(scale_tag);
	 spvs.push_back(yuan_sp);

	 /*
	 //season
	 CCLabelTTF* sean_l = CCLabelTTF::create(LFStrings::getValue("gk").c_str(),fontStr_KozukaGothicProEL,35);
	 //sean_l->setTag(scale_tag);
	 sean_l->setPosition(ccp(d_ba->getPositionX()+d_ba->getContentSize().width/2-yuan_sp->getContentSize().width/2-sean_l->getContentSize().width/2-10,
		 d_ba->getContentSize().height/2+d_ba->getPositionY()+sean_l->getContentSize().height/2));
	 back_s->addChild(sean_l);
	 spvs.push_back(sean_l);
	 */

	 //圆
	 yuan_sp->setPosition(ccp(size.width*(1-buton_jiangju),yuan_sp->getContentSize().height/2+5));
	 back_s->addChild(yuan_sp);
	 //是否开启
	 bool locked=false;
	 GameInforEditer* infor=GameInforEditer::getGameInfor();
	 SeasonInfor* seasonInfor = infor->getSeasonInfor(id);
	 if ( (seasonInfor == NULL || seasonInfor->locked) && !GameInforEditer::getGameInfor()->isIgnoreAllLock() )
	 {
		 locked = true;
	 }else
	 {
		 locked = false;
	 }

		 ccColor3B color = colors.at(id); 
	 //if(locked==false){
		 CCLabelTTF* gk_ls = CCLabelTTF::create(CCString::createWithFormat("%d",id)->getCString(),fontStr_KlavikaBold,60);
		 gk_ls->setColor(color);
		 gk_ls->setPosition(ccp(yuan_sp->getContentSize().width/2,yuan_sp->getContentSize().height/2));
		 yuan_sp->addChild(gk_ls);
		 gk_ls->setTag(scale_tag);

		 //左下角锁的按钮现在需要取消，一直显示seasonid
		 //gk_ls->setVisible(!locked);
		 spvs.push_back(gk_ls);
		 _levelNums.push_back(gk_ls);
	// }else if(locked==true){
		 string suo=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_shumu_suoding.png");
		 CCSprite* suo_sp=CCSprite::create(suo.c_str());
		 suo_sp->setTag(scale_tag);
		 suo_sp->setVisible(false);
		 //suo_sp->setVisible(locked);
		 suo_sp->setColor(color);
		 suo_sp->setPosition(ccp(yuan_sp->getContentSize().width/2,yuan_sp->getContentSize().height/2));
		 yuan_sp->addChild(suo_sp);
		 spvs.push_back(suo_sp);
		 _levelLocks.push_back(suo_sp);
	 //}

		 string st_str=ResManager::getManager()->getFilePath(g_xiaogk+"GQ_anniu_start.png");

		  CCMenuItemImage* st_bt=CCMenuItemImage::create(
			 st_str.c_str(),
			 st_str.c_str(),
			 this,
			 menu_selector(Biguangka::allCallback));
		 st_bt->setPosition(CCPoint(size.width/2,size.height*0.25f));
		 st_bt->setTag(start_tag);
		 menusps.push_back(st_bt);

		 CCLabelTTF* st_text=CCLabelTTF::create(LFStrings::getValue("start").c_str(),fontStr_KlavikaBold,50);
		 st_text->setColor(color);
		 st_text->setTag(startlable_tag);
		 st_text->setPosition(CCPoint(st_bt->getContentSize()/2));
		 spvs.push_back(st_text);

		 st_bt->addChild(st_text);
		 st_bt->setUserObject(st_text);
		 st_bt->setVisible(!locked);
		 _startButtons.push_back(st_bt);
		 
		string st_str_lock = ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_icon_suoding.png");
		 CCMenuItemImage* st_bt_lock=CCMenuItemImage::create(
			 st_str_lock.c_str(),
			 st_str_lock.c_str(),
			 this,
			 menu_selector(Biguangka::allCallback));
		 st_bt_lock->setPosition(CCPoint(size.width/2,size.height*0.25f));
		 st_bt_lock->setTag(kCCNodeTagInvalid);
		 st_bt_lock->setVisible(locked);
		 menusps.push_back(st_bt_lock);
		
#if Show_End_BigGuanQia != 0
		 if (id == (maxseson))
		 {
			 yuan_sp->setVisible(false);
			 st_bt->setVisible(false);
			 st_text->setVisible(false);
			 st_bt_lock->setVisible(false);
		 }
#endif

		 _lockButtons.push_back(st_bt_lock);

		 CCMenu* menu=CCMenu::create(st_bt, st_bt_lock, NULL);
		 menu->setPosition(CCPointZero);
		 menu->setTag(menu_tag);
		 back_s->addChild(menu);

	 sprite_map.insert(make_pair(id,spvs));
	 menusprite_map.insert(make_pair(id,menusps));

}

bool Biguangka::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	
	return this->biglist->ccTouchBegan(pTouch, pEvent);

	
}
void Biguangka::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	
	return this->biglist->ccTouchMoved(pTouch, pEvent);
}
void Biguangka::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	return this->biglist->ccTouchEnded(pTouch, pEvent);
}

void Biguangka::update(float delta){
	
	/*
	if(mRenderStarts.size()==mBGs.size()==labeles.size()){
		for(int i=0;i<labeles.size();i++){
			LF::cutoutText(mRenderStarts.at(i),mBGs.at(i),labeles.at(i));
		}
	}
	*/
}

int Biguangka::getCurrentseason(){

	if(biglist){
		return biglist->getCurntId();
	}
	return curren_season;
}

void Biguangka::setCurrentseason(int tag){

	 this->curren_season=tag;
}

void Biguangka::moveOver()
{
	curren_season = getCurrentseason();
	if (maxseson <= 1)
	{
		if (itemleft)
		{
			itemleft->setVisible(false);
		}

		if (itemright)
		{
			itemright->setVisible(false);
		}
		return;
	}
	if (curren_season <= 1)
	{
		if (itemleft)
		{
			itemleft->setVisible(false);
		}

		if (itemright)
		{
			itemright->setVisible(true);
		}
	}
	else if (curren_season >= maxseson)
	{
		if (itemleft)
		{
			itemleft->setVisible(true);
		}

		if (itemright)
		{
			itemright->setVisible(false);
		}
	}
	else
	{
		if (itemleft)
		{
			itemleft->setVisible(true);
		}

		if (itemright)
		{
			itemright->setVisible(true);
		}
	}
}

//集体变化
void Biguangka::BigjuliOut(float changeOut,int current,int xlpage){
    
	changeOut=abs(changeOut);
	if(current<1 || current>maxseson){
		return;
	}
// 	if(0.0f==abs(changeOut)-num_Fade||changeOut==(num_Fade-num_Fade)){
// 		this->biglist->two_check=false;
// 	}
     
	//heseSp sps_juli
	if(current>=1&&current<=maxseson&&xlpage>=1&&xlpage<=maxseson){
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

	for(int i = 1;i<= maxseson;i++)
	{
		scla9_map[i]->setVisible(false);
	}

	if(current>=1&&current<=maxseson){
		//scla9_kuang_map.at(current)->setOpacity(changeOut);
		
		
		if(changeOut < 1.0f)
		{
			scla9_map.at(current)->setVisible(false);
		}
		else
		{
			scla9_map.at(current)->setVisible(true);
			scla9_map.at(current)->setOpacity((GLubyte)changeOut);
		}

		vector<CCSprite*> sps=sprite_map.at(current);
		for(size_t i=0;i<sps.size();i++){
			sps.at(i)->setOpacity(changeOut);
			if(sps.at(i)->getTag()==scale_tag){
				sps.at(i)->setScale(changeOut/num_Fade);
			}
		}
		vector<CCMenuItemSprite*> mes=menusprite_map.at(current);
		for(size_t j=0;j<mes.size();j++){
			mes.at(j)->setOpacity(changeOut);
		}	 
	}
	if(xlpage>=1&&xlpage<=maxseson){
		//scla9_kuang_map.at(xlpage)->setOpacity(num_Fade-changeOut);
		if(changeOut > 254.0)
		{
			scla9_map.at(xlpage)->setVisible(false);
		}
		else
		{
			scla9_map.at(xlpage)->setVisible(true);
			scla9_map.at(xlpage)->setOpacity((GLubyte)(num_Fade-changeOut));
		}
		vector<CCSprite*> sps=sprite_map.at(xlpage);
		for(size_t i=0;i<sps.size();i++){
			sps.at(i)->setOpacity(num_Fade-changeOut);
			if(sps.at(i)->getTag()==scale_tag){
				sps.at(i)->setScale((num_Fade-changeOut)/num_Fade);
			}
		}
		vector<CCMenuItemSprite*> mes=menusprite_map.at(xlpage);
		for(size_t j=0;j<mes.size();j++){
			mes.at(j)->setOpacity(num_Fade-changeOut);
		}	 

		//当下一章节的背景要出现时（根据背景的透明度来判断），发送更换背景的消息，
		//这里设置为245，就表示出现了，
		//主要是为了解决玩家在章节选择界面和其他界面滑动太快导致其他界面背景无法刷新的问题
		if(num_Fade-changeOut >= 200.0f){
			if (m_iLastChangBgPage != getCurrentseason())
			{
				m_iLastChangBgPage = getCurrentseason();
				CCInteger id = CCInteger(m_iLastChangBgPage);
				CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE_BG_COLOR, &id);
			}
		}
	}
	//scf
	//好友头像 -- 进度
	for (map<int,CCLayer*>::iterator it = layers_map.begin(); it!=layers_map.end(); ++it)
	{
		CCLayer* layer = it->second; 

		for (int i=0; i<3; ++i)
		{
			CCSprite *pic_bg = dynamic_cast<CCSprite*>(layer->getChildByTag(Tag_friendProgressPicture_bg+i));
			CCSprite *pic = dynamic_cast<CCSprite*>(layer->getChildByTag(Tag_friendProgressPicture+i));
			if (pic_bg && pic)
			{
				if (layer == layers_map.at(current))
				{
					pic_bg->setVisible(true);
					pic_bg->setOpacity(changeOut);
					pic->setVisible(true);
					pic->setOpacity(changeOut);
				}
				else if (xlpage>=1 && xlpage<=maxseson && layer==layers_map.at(xlpage)) //下一个
				{
					pic_bg->setVisible(true);
					pic_bg->setOpacity(abs(num_Fade-changeOut));
					pic->setVisible(true);
					pic->setOpacity(abs(num_Fade-changeOut));
				}
				else
				{
					pic_bg->setVisible(false);
					pic->setVisible(false);
				}
			}
		}
	}
}

//

//初始化为0
void Biguangka::Fadeoutbig(int curent){
    
	//scla9_kuang_map.at(curent)->setOpacity(0);
	scla9_map.at(curent)->setOpacity(0);
	scla9_map.at(curent)->setVisible(false);
	vector<CCSprite*> sps=sprite_map.at(curent);
	for(size_t i=0;i<sps.size();i++){
		sps.at(i)->setOpacity(0);
		if(sps.at(i)->getTag()==scale_tag){
			sps.at(i)->setScale(0.0f);
		}
	}
	vector<CCMenuItemSprite*> mes=menusprite_map.at(curent);
	for(size_t j=0;j<mes.size();j++){
		mes.at(j)->setOpacity(0);
	}	 
}


//集体out

void Biguangka::AllOut(int current){

	//scla9_kuang_map.at(current)->setOpacity(0);
	scla9_map.at(current)->setOpacity(0);
	scla9_map.at(current)->setVisible(false);
	vector<CCSprite*> sps=sprite_map.at(current);
	for(size_t i=0;i<sps.size();i++){
		sps.at(i)->setOpacity(0);
	}
	vector<CCMenuItemSprite*> mes=menusprite_map.at(current);
	for(size_t j=0;j<mes.size();j++){
		mes.at(j)->setOpacity(0);
	}	 
}
//集体in
void Biguangka::Allin(int current){

	//scla9_kuang_map.at(current)->setOpacity(num_Fade);
	scla9_map.at(current)->setOpacity(num_Fade);
	scla9_map.at(current)->setVisible(true);
	vector<CCSprite*> sps=sprite_map.at(current);
	for(size_t i=0;i<sps.size();i++){
		sps.at(i)->setOpacity(num_Fade);
	}
	vector<CCMenuItemSprite*> mes=menusprite_map.at(current);
	for(size_t j=0;j<mes.size();j++){
		mes.at(j)->setOpacity(num_Fade);
	}	 
}


void Biguangka::allCallback(CCObject* pSender){
	if (!Starseneui::checkIsCanTouched(this))
	{
		return;
	}
	Music::sharedMusicPlayer()->buttonClicked();
	 CCMenuItemImage* p=(CCMenuItemImage*)pSender;
	 int tag=p->getTag();
	 if(tag==start_tag){

		  curren_season = biglist->getCurntId();
		  LevelManager::getManager()->setLastSeasonId(curren_season);
		  Myobject* object=new Myobject();
		  object->parent_id=Bigcheckpoint_tag;
		  object->current_id=Smalllayer_tag;
		  object->season=curren_season;
		  object->mark=true;
		  object->pCurNode = this;
		  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE,(CCObject*)object);
		  
	  }else if(tag==fah_maintag){

		  
		  Myobject* object=new Myobject();
		  object->mark=false;
		  object->parent_id = Starmain_tag;
		  object->current_id = Bigcheckpoint_tag;
		  object->pCurNode = this;
		  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE,(CCObject*)object);
		  
	 }
	  else if (tag==bigleft_tag)
	  {
		  if (biglist)
		  {
			  biglist->scrollToSeason(curren_season-1, true);
		  }
	  }
	  else if (tag==bigright_tag)
	  {
		  if (biglist)
		  {
			  biglist->scrollToSeason(curren_season+1, true);
		  }
	  }
}

void Biguangka::fillOuty(CCMenuItemImage* p,string imgfile){

	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.10f);
	p->setSelectedImage(s);
	CCSprite* n=CCSprite::create(imgfile.c_str());
	CCSprite* layer=CCSprite::create();
	layer->setContentSize(CCSize(p->getContentSize()*1.10f));
	n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	layer->addChild(n);
	p->setNormalImage(layer);
}

Biglisttable::Biglisttable(void):
mGradientClipShader(NULL)
{
  

}

Biglisttable::~Biglisttable(void){


}

 bool Biglisttable::init(){
	  if(!CCLayer::init()){
		  return false;
	  }
      isTouch = false;
	  size=CCDirector::sharedDirector()->getWinSizeInPixels();
	  maxseson=GameLayerFactory::getInstance()->getValidSeasonNum();
	  current_page=LevelManager::getManager()->getLastSeasonId();
	  old_page=current_page;
	  moveold=movecurent=CCPoint(0,0);
	  changePage=false;//没有变化
	  direction=-1;//默认向左
	  ch=ch_check=0.0f;
	  kuanlist = NULL;
	  //two_check=false;
	  drawBiglistscroview();
  
	  return true;
 }


 void Biglisttable::onEnter(){

	 CCLayer::onEnter();
	 //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,bigtouchtype,false);
 }

 void Biglisttable::onExit(){
	 CCLayer::onExit();
	 //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	 
 }

 void Biglisttable::createGradientClipShader(float leftX, float rightX)
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
							float x = gl_FragCoord.x - 0.5;										\n\
							float alpha = 0.0;													\n\
							if(x <= left + grad_w && x >= left)									\n\
							{																	\n\
								alpha = 1.0 - (left + grad_w - x) / grad_w;						\n\
							}																	\n\
							else if(x >= right - grad_w && x <= right)							\n\
							{																	\n\
								alpha = 1.0 - (x - right + grad_w) / grad_w;					\n\
							}																	\n\
							else if (x > left + grad_w && left + grad_w < right - grad_w)		\n\
							{                                                                   \n\
								alpha = 1.0;													\n\
							}																	\n\
							alpha = alpha * alpha;																	\n\
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
	 shader->setUniformLocationWith1f(wloc, 200.0f * (wWith / dWidth));


	 mGradientClipShader = shader;
 }


 void Biglisttable::drawBiglistscroview(){

	 CCSize winsize=CCDirector::sharedDirector()->getWinSize();
	 string db_str=CCString::createWithFormat("ZJ_bg_season%d_kuang.png",LevelManager::getManager()->getLastSeasonId())->getCString();
	 db_str=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+db_str);

	 CCSprite* tempspr = CCSprite::create(db_str.c_str());

	 scrow_size=CCSize(CCDirector::sharedDirector()->getWinSize().width - 300, tempspr->getContentSize().height + 50);

	 createGradientClipShader((winsize.width - scrow_size.width)/2, (winsize.width + scrow_size.width)/2);

	 scroll_layer = CCLayer::create();  
	 scroll_layer->setPosition(CCPoint(0,0));  
	 scroll_layer->setAnchorPoint(CCPoint(0.0f,0.0f));  
	 scroll_layer->setContentSize(CCSize((scrow_size.width)*maxseson,scrow_size.height));

	 scrollView = CCScrollView::create(scrow_size,scroll_layer);
	 scrollView->setDelegate(this);
	 scrollView->setContentOffset(CCPoint(0,0));
	 scrollView->setTouchEnabled(false);
	 scrollView->setDirection(kCCScrollViewDirectionHorizontal);
	 scrollView->ignoreAnchorPointForPosition(false);
	 scrollView->setAnchorPoint(CCPoint(0.5,0.5));
	 scrollView->setPosition(CCPoint(size.width/2, size.height/2));  
	 addChild(scrollView,20);  
	 for(int i=0;i<maxseson;i++){
		 CCLayer* layer=CCLayer::create();
		 layer_map.insert(make_pair(i+1,layer));
		 drowList(CCPoint((scrow_size.width)*i+(scrow_size.width)/2,scrow_size.height/2),i);
		 if(i+1!=current_page){
			 diguisetOut(layer);
		 }
	 }

	 CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width),0);  
	 scrollView->setContentOffset(adjustPos,true);
	 moveold_fade=movecurent_fade=scrollView->getContentOffset();
 }



 void Biglisttable::scrollViewDidScroll(CCScrollView* view){

	  if(layer_map.size()==maxseson){
          movecurent_fade=scrollView->getContentOffset();
		  if(movecurent_fade.x<scroview_point.x){
			  direction=0;
		  }else if(movecurent_fade.x>scroview_point.x){
              direction=1;
		  }
          int old_p=old_page;
		  float m_x=movecurent_fade.x;
          m_x=abs(m_x);
		  m_x=m_x-(old_page-1)*scrow_size.width;
		  m_x=abs(m_x);

		  float fadescale = m_x/(scrow_size.width);
		  float fadescale1 = m_x/(scrow_size.width);


		  ch=num_Fade-num_Fade*fadescale;
		  ch_check=ch;
		 
          //diguibyjuliOut(layer_map.at(old_page),ch);
		  
		
		 
		 


		  if(kuanlist){
			  
			  if(direction==0){
				  if(old_p+1<=maxseson){
					  old_p=old_p+1;
				  }else{
					  old_p=-1;
				  }
// 				  if(old_p>=1&&old_p<=maxseson&&direction==0){
// 					  float in_f=num_Fade*fadescale1;
// 					  diguibyjuliOut(layer_map.at(old_p),in_f);
// 				  }
				  this->kuanlist->BigjuliOut(ch,old_page,old_p);

			  }else if(direction==1){
				  if(old_p-1>=1){
					  old_p=old_p-1;
				  }else{
					  old_p=-1;
				  }
// 				  if(old_p<=maxseson&&old_p>=1&&direction==1){
// 					  float in_f=num_Fade*fadescale1;
// 					  diguibyjuliOut(layer_map.at(old_p),in_f);
// 				  }
				  this->kuanlist->BigjuliOut(ch,old_page,old_p);
			  }
		  }
		  moveold_fade=movecurent_fade;
	  }

	  if (kuanlist)
	  {
		  kuanlist->moveOver();
	  }
 }


 void Biglisttable::scrollViewDidZoom(CCScrollView* view){


 }


 bool Biglisttable::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	 if(Shieldingmanger::getSielding()->getCureentlayertag()!=Bigcheckpoint_tag){
		return false;
	 }
//      if(isTouch==true){
//          return false;
//      }
	

// 	 if(two_check==true){
// 
// 		 return false;
// 	 }
	 
	 old_page=current_page;
	 start_ponit=pTouch->getLocation();
	 scroview_point=scrollView->getContentOffset();
	 mark_firstmove=0;
	 

	 return true;
 }


 void Biglisttable::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	 if(!Shieldingmanger::getSielding()->isHorizontalActive())
	 {
		 return;
	 }
	 
	 
// 	 if(two_check==true){
// 		 return;
// 	 }
	
	 isTouch=true;
	 movecurent=pTouch->getLocation();
	 scrollView->setContentOffset(scroview_point + ccp(movecurent.x - start_ponit.x, 0));
//  	 if(mark_firstmove==0){
//  		 scrollView->setContentOffset(scrollView->getContentOffset()+CCPoint(((movecurent.x-start_ponit.x)),0),false);
//  		 mark_firstmove=1;
//  	 }else if(mark_firstmove==1){
//  		 scrollView->setContentOffset(scrollView->getContentOffset()+CCPoint(((movecurent.x-moveold.x)),0),false);
//  	 }
//  	 moveold=movecurent; 

	
 }

 void Biglisttable::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	 	 
	 end_point=pTouch->getLocation();
	  
#if Show_End_BigGuanQia != 0
	 if (current_page == maxseson && ccpDistance(start_ponit, end_point) <= 20)
	 {
		 getFullVersion();
	 }
#endif

	 if(!Shieldingmanger::getSielding()->isHorizontalActive())
	 {
		 return;
	 }
// 	 if(two_check==true){
// 		 return;
// 	 }
// 	 two_check=true;
	 isTouch=false;
	 float distance = end_point.x - start_ponit.x; 
	 float prent_qie=size.width*(20.0f/1024.0f); 
	 if(fabs(distance)>=prent_qie){

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
		 if (current_page > maxseson)  
		 {  
			 current_page = maxseson;  
		 }
	 }
	 CCPoint adjustPos =CCPoint( - (current_page - 1) * (scrow_size.width),0);  
	 scrollView->setContentOffset( CCEaseExponentialOut::create(CCMoveTo::create(0.4f, adjustPos)));
	 LevelManager::getManager()->setLastSeasonId(current_page);
 }

 void Biglisttable::setBigcurent(float time){
	 float m=scrollView->getContentOffset().x;
	 m=abs(m);
	 float y=scrow_size.width;
 }

 void Biglisttable::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
	 ccTouchEnded(pTouch, pEvent);
 }



 void Biglisttable::ontouchCilick(CCTouch *pTouch, CCEvent *pEvent){

 }

 bool Biglisttable::SpisContaintPoint(CCSprite* ps,CCPoint p){
	  return true;
 }
 
 void Biglisttable::inintData(){

 }

 void Biglisttable::drowList(CCPoint p,int page){

	 vector<CCLabelTTF*> lables;
	 auto layer=layer_map.at(page+1);
	 layer->setContentSize(scrow_size);// 
	 layer->ignoreAnchorPointForPosition(false);
	 layer->setAnchorPoint(CCPoint(0.5,0.5));
	 layer->setPosition(p);

	 scroll_layer->addChild(layer,1);

	 ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
#if Show_End_BigGuanQia != 0
	 if (page == (maxseson)-1)
	 {
		 CCSprite* tb_sprite= CCSprite::create(ResManager::getManager()->getFilePath(g_mainlayerPath+"ZJ_biaoshi_free.png").c_str());
		 tb_sprite->setShaderProgram(mGradientClipShader);
		 tb_sprite->setBlendFunc(blend);
		 tb_sprite->setPosition(ccp(scrow_size.width/2, scrow_size.height*0.35f));
		 layer->addChild(tb_sprite);
		 
		 CCLabelTTF* pLabel = CCLabelTTF::create(LFStrings::getValue("fullversion").c_str(),fontStr_KlavikaBold, 80);
         
         pLabel->setPosition(ccp(scrow_size.width/2,scrow_size.height*0.78f));
         if (ResManager::getManager()->getLanguageType() == kLanguageGerman)
         {
             pLabel->setFontSize(60);
             pLabel->setPosition(ccp(scrow_size.width/2,scrow_size.height*0.85f));
         }
		 layer->addChild(pLabel);
		 pLabel->setShaderProgram(mGradientClipShader);
		 pLabel->setBlendFunc(blend);
		 pLabel->setTag(plableonetag);
		 lables.push_back(pLabel);

		 CCLabelTTF* pLabel2 = CCLabelTTF::create("",fontStr_KlavikaBold,40);
		 pLabel2->setPosition(ccp(scrow_size.width/2,scrow_size.height*0.25f));
		 layer->addChild(pLabel2); 
		 pLabel2->setShaderProgram(mGradientClipShader);
		 pLabel2->setBlendFunc(blend);
		 pLabel2->setTag(plabletwotag);
		 lables.push_back(pLabel2);
		 lable_maps.insert(make_pair(page+1,lables));
		 return;
	 }
#endif

	 string tb_imgfile=CCString::createWithFormat("ZJ_biaoshi_season%d.png",page+1)->getCString();
	 tb_imgfile=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+tb_imgfile);
	 string str[3]={tb_imgfile,
		 LFStrings::getValue(CCString::createWithFormat("sd%d",page+1)->getCString()),
		 LFStrings::getValue(CCString::createWithFormat("sx%d",page+1)->getCString())};

	 CCSprite* tb_sprite= CCSprite::create(str[0].c_str());
	 tb_sprite->setShaderProgram(mGradientClipShader);
	 tb_sprite->setBlendFunc(blend);
	 //ccBlendFunc blend = tb_sprite->getBlendFunc();
	 CCLabelTTF* pLabel = CCLabelTTF::create(str[1].c_str(),fontStr_KlavikaBold,80);
	 ccBlendFunc func = pLabel->getBlendFunc();
	 //float x_b=(scrow_size.width-(tb_sprite->getContentSize().width+pLabel->getContentSize().width))/2;

	 tb_sprite->setPosition(ccp(scrow_size.width/2,scrow_size.height*0.68f));
	 layer->addChild(tb_sprite);
	 pLabel->setPosition(ccp(scrow_size.width/2,scrow_size.height*0.42f));
	 layer->addChild(pLabel);
	 pLabel->setShaderProgram(mGradientClipShader);
	 pLabel->setBlendFunc(blend);
	 pLabel->setTag(plableonetag);
	 lables.push_back(pLabel);

	 CCLabelTTF* pLabel2 = CCLabelTTF::create(str[2].c_str(),fontStr_KlavikaBold,40);
	 pLabel2->setPosition(ccp(scrow_size.width/2,scrow_size.height*0.25f));
	 layer->addChild(pLabel2); 
	 pLabel2->setShaderProgram(mGradientClipShader);
	 pLabel2->setBlendFunc(blend);
	 pLabel2->setTag(plabletwotag);
	 lables.push_back(pLabel2);
	 lable_maps.insert(make_pair(page+1,lables));
 }

 void Biglisttable::getFullVersion()
 {
	 Music::sharedMusicPlayer()->buttonClicked();
	 string urlString = "https://itunes.apple.com/us/app/lost-star-landing/id980188049?l=zh&ls=1&mt=8";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	 urlString = "http://www.camellia-network.com";
#endif
     WrapperManager::getInstance()->openUrl(urlString.c_str());
 }

 void Biglisttable::refreshLanguge(){


	  for(int i=1;i<=GameLayerFactory::getInstance()->getValidSeasonNum();i++){

	      vector<CCLabelTTF*> lables=lable_maps.at(i);

		  for(size_t j=0;j<lables.size();j++){

			  if(lables.at(j)->getTag()==plableonetag){
#if Show_End_BigGuanQia != 0
				  if (i != maxseson)
#endif
				  {
					 string onelable=LFStrings::getValue(CCString::createWithFormat("sd%d",i)->getCString());
					 lables.at(j)->setString(onelable.c_str());
				  }
#if Version_Free != 0 && Show_End_BigGuanQia != 0
				  if (i == maxseson)
				  {
					  lables.at(j)->setString(LFStrings::getValue("fullversion").c_str());
				  }
#endif
			  }else if(lables.at(j)->getTag()==plabletwotag){

                  string twolable=LFStrings::getValue(CCString::createWithFormat("sx%d",i)->getCString());
				  lables.at(j)->setString(twolable.c_str());

			  }

			  lables.at(j)->setShaderProgram(mGradientClipShader);
			  ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
			  lables.at(j)->setBlendFunc(blend);

#if Show_End_BigGuanQia != 0
			  if (i == maxseson)
			  {
				  if (lables.at(j)->getTag() != plableonetag)
				  {
					   lables.at(j)->setVisible(false);
				  }
			  }
#endif
		  }

	  }

 }

 void Biglisttable::scrollToSeason(int seasonId, bool bAnimation/* = false*/)
 {
	 if (seasonId == current_page) return;

	 int old_page = current_page;

	 if (bAnimation)
	 {
		 Shieldingmanger::getSielding()->setHorizontalActive(true);
		 float x = 100.0f;
		 CCTouch pTouch;
		 pTouch.setTouchInfo(0, x, 0);
		 CCEvent pEvent;
		 this->ccTouchBegan(&pTouch, &pEvent);

		 if (old_page > seasonId)
		 {
			 pTouch.setTouchInfo(0, x+50, 0);
		 }
		 else if (old_page < seasonId)
		 {
			 pTouch.setTouchInfo(0, x-50, 0);
		 }
		 this->ccTouchEnded(&pTouch, &pEvent);
	 }
	 else
	 {
		 current_page = seasonId;

		 if (current_page < 1)
		 {
			 current_page = 1;
		 }
		 else if (current_page > maxseson)
		 {
			 current_page = maxseson;
		 }
		  CCPoint adjustPos = CCPoint( - (seasonId - 1) * (scrow_size.width),0);
		 scrollView->setContentOffset(adjustPos);
		 this->kuanlist->BigjuliOut(255, current_page, old_page);
	 }
 }

 void Biglisttable::diguisetOut(CCNode* node){
#if 0
	 CCArray* arry= node->getChildren();
	 for(size_t i=0;i<arry->count();i++)
	 {
		 //确保所有元素为sprite 或者继承sprite
		 CCSprite* p=((CCSprite*)arry->objectAtIndex(i)); 
		 p->setOpacity(0.0f);// runAction(CCFadeOut::create(gkfadeout));
		 if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			 diguisetOut(p);
		 }
	 } 
#endif
 }


 void Biglisttable::diguibyjuliOut(CCNode* node,float changeOut){

#if 0
	 if (changeOut > 255)
	 {
		 changeOut = 255;
	 }

	 if (changeOut < 0)
	 {
		 changeOut = 0;
	 }
	 CCArray* arry= node->getChildren();
	 for(size_t i=0;i<arry->count();i++)
	 {
		 //确保所有元素为sprite 或者继承sprite
		 CCSprite* p=((CCSprite*)arry->objectAtIndex(i)); 
		 p->setOpacity(changeOut);// runAction(CCFadeOut::create(gkfadeout));
		 if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			 diguibyjuliOut(p,changeOut);
		 }
	 } 
#endif
 }

 void Biglisttable::diguibyjuliIn(CCNode* node,float changeOut){
#if 0
	 CCArray* arry= node->getChildren();
	 for(size_t i=0;i<arry->count();i++)
	 {
		 //确保所有元素为sprite 或者继承sprite
		 CCSprite* p=((CCSprite*)arry->objectAtIndex(i)); 
		 p->setOpacity(changeOut);// runAction(CCFadeOut::create(gkfadeout));
		 if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			 diguibyjuliIn(p,changeOut);
		 }
	 }
#endif
 }

 void Biglisttable::setBiguangka(Biguangka* kuan){
	  this->kuanlist=kuan; 
 }

 int Biglisttable::getCurntId(){
	 return current_page;
 }