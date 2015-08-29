#include "Goluetu.h"
#include "Uimsg.h"
#include "LevelManager.h"
#include "GameLayerFactory.h"
#include "BaseScense.h"
#include  "Shieldingmanger.h"
#include "Uimsg.h"
#include "Myobject.h"
#include "MessageBar.h"
#include "Music.h"



Goluetu::Goluetu(bool mark){

	this->isStoplayer=mark;

}

Goluetu::Goluetu(void){


}


Goluetu::~Goluetu(void){


}

Goluetu*  Goluetu::creteBytag(bool mark){

	Goluetu* pRet=new Goluetu(mark);
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}
}

bool Goluetu::init(){

	if(!CCLayer::init()){
		return false;
	}
	freind_solution=nullptr;
	isclickZ=false;
	isgfjiefa=false;
	current_layermark=glutulayer_tag;
	iscloseactiondone=true;
	isgolutuactiondone=true;
	is_answer=false;
	mark_sns=false;
	isliandian=true;
	pPlayRecordLayer_Official = NULL;
	size=CCDirector::sharedDirector()->getWinSize();  
	playersize=CCSize(size*0.7f);
	return true;
}  
void Goluetu::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Goluetu::son_praent), MSG_FRIND_PLAY, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Goluetu::refreshSolution),NetMessage::MSG_DownloadSolution, NULL);//下载好友解法回调 NetMessage::MSG_PraiseSolution
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Goluetu::click_zan), NetMessage::MSG_PraiseSolution, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Goluetu::clickisz),MSG_GAME_CLICKIS_Z, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Goluetu::logincallback),NetMessage::MSG_GotSeasonSocialInfo, NULL);
	
	
}
void Goluetu::onExit(){
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
void Goluetu::clickisz(CCObject* obj){
	isclickZ=true;
}

bool Goluetu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){





	CCPoint p=pTouch->getLocation();
	bool x_m=(p.x<=size.width/2+playersize.width/2&&p.x>=size.width/2-playersize.width/2);
	bool y_m=(p.y<=size.height/2+playersize.height/2&&p.y>=size.height/2-playersize.height/2);
	if((x_m==false||y_m==false)&&(current_layermark==glutulayer_tag)){

		if(isgolutuactiondone==false){

			return true;
		}
		isgolutuactiondone=false;
		goltu_layer->runAction(
			CCSequence::create(
			CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			CCCallFunc::create(this,callfunc_selector(Goluetu::gLuetFh)),
			NULL
			)
			);
		//Fadeoutnodes(pPlayRecordLayer);
	}
	return true;
}


void Goluetu::gLuetFh(){

	isgolutuactiondone=true;
	if(isStoplayer==false){
		int tag=Gameother_tag;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stopright,(CCObject*)tag);
	}else if(isStoplayer==true){
		BaseScense::getCurrentScene()->setCollisionEnable(true);
		BaseScense::getCurrentScene()->getMenuLayer()->resume(true);
		BaseScense::getCurrentScene()->getMenuLayer()->setVisible(true);
	}
	this->removeFromParent();
}

void Goluetu::drawGongluetu(){

	/*
	if (m_allRecordData.size() <= 0)
	{
		CCAssert(false, "m_allRecordData is not should be 0 ! \nYou should be call setAllRecordData first!");
		return;
	}
		
	bool bAutoPlay = true;
	if (pPlayRecordLayer)
	{
		bAutoPlay = false;
	}
	int seasonid = LevelManager::getManager()->getLastSeasonId();
	int sectionid = LevelManager::getManager()->getLastSectionId();
	//播放
	pPlayRecordLayer = GameScenseDefine::SharedGameScenseDefine()->playRecord(seasonid, sectionid, &m_allRecordData, bAutoPlay);
	pPlayRecordLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));// - pPlayRecordLayer->getContentSize().height/2*pPlayRecordLayer->getScale() - 50));
	addChild(pPlayRecordLayer, 1, Tag_Layer_PlayRecord);

	pPlayRecordLayer->setScale(chaonengdj_scale-0.2f);
	pPlayRecordLayer->runAction(CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale));
	if (bAutoPlay)
	{
		Fadeinnodes(pPlayRecordLayer);
	}
	*/

	Golutugailayer();
	goltu_layer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,1.0f,1.0f),NULL
		)
		);
	Fadeinnodes(goltu_layer);
	snsgetData();
	Shieldingmanger::getSielding()->setPingbingbutton(false);

}


void  Goluetu::Golutugailayer(){

	  goltu_layer=CCLayer::create();
	  goltu_layer->setContentSize(CCSize(750,600));
	  addChild(goltu_layer);
	  
	  string bg=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang.png");
	  CCSprite* tmp1 = CCSprite::create(bg.c_str());
	  CCSize size2 = tmp1->getContentSize();
	  CCRect fullRect1 = CCRectMake(0,0, size2.width, size2.height);
	  CCRect insetRect1 =CCRectMake(size2.width/2-1,size2.height*0.8f, 2, 2);
	  //740 640
	  string back_file="ZT_tanchukuang.png";
	  back_file=ResManager::getManager()->getFilePath(g_stop+back_file);
	  back=CCScale9Sprite::create(back_file.c_str(),fullRect1,insetRect1);
	  back->setContentSize(CCSize(750,600));
	  back->setPosition(CCPoint(size.width/2,size.height/2));
	  goltu_layer->addChild(back);
	  
	  //标题 270 61
	  string biao_t=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_biaoti_1.png"); 
	  CCSprite* tmp = CCSprite::create(biao_t.c_str());
	  CCSize size = tmp->getContentSize();
	  //创建原始的图像对应大小矩形区域。
	  CCRect fullRect= CCRectMake(0,0, size.width, size.height);
	  CCRect insetRect=CCRectMake(20, 0,20, size.height);
	  CCScale9Sprite* bt_scale=CCScale9Sprite::create(biao_t.c_str(),fullRect,insetRect);
	  bt_scale->setContentSize(CCSize(270, size.height));
	  back->addChild(bt_scale);

	  bt_scale->setAnchorPoint(ccp(0.0f, 0.5f));

	  string bt_text=LFStrings::getValue("gsolution");
	  CCLabelTTF* g_label = CCLabelTTF::create(bt_text.c_str(),fontStr_KlavikaBold,40);
	  g_label->setAnchorPoint(CCPoint(0.0f,0.5f));
	  bt_scale->setContentSize(CCSize(g_label->getContentSize().width+100, size.height));
	  bt_scale->setPosition(CCPoint(-20, back->getContentSize().height-bt_scale->getContentSize().height/2 - 1));
	  g_label->setPosition(CCPoint(30,bt_scale->getContentSize().height/2));
	  bt_scale->addChild(g_label);

	   
	  // 官方解法
	  //ZT_tanchukuang_3.png
	  string back_filestr=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_2.png"); 
      back_gfhave=CCMenuItemImage::create(back_filestr.c_str(),back_filestr.c_str(),this,
		  menu_selector(Goluetu::chankangfjieFa));
	  back_gfhave->setTag(chankanguanfangjiefa_tag);
	  back_gfhave->setPosition(CCPoint(back->getContentSize().width/2,back->getContentSize().height*0.8f));

	  CCMenu* b_me=CCMenu::create(back_gfhave,nullptr);
	  b_me->setTouchPriority(kCCMenuHandlerPriority-1);
	  b_me->setPosition(CCPointZero);
      back->addChild(b_me);

	  //ZT_gongluetu.png
	  string glt_str=ResManager::getManager()->getFilePath(g_stop+"ZT_gongluetu.png");
	  CCSprite* glT=CCSprite::create(glt_str.c_str()); 
	  glT->setAnchorPoint(CCPoint(0.0f,0.5f));
	  glT->setPosition(CCPoint(-glT->getContentSize().width/2,back_gfhave->getContentSize().height/2));
	  back_gfhave->addChild(glT);

	  



	  //是哪关 msg
	  string Gk=CCString::createWithFormat("sd%d",LevelManager::getManager()->getLastSeasonId())->getCString();
	  string gk_ms=CCString::createWithFormat(" %d-%d",LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId())->getCString();
	  string msg_text=LFStrings::getValue(Gk)+gk_ms;
	  CCLabelTTF* gk_season = CCLabelTTF::create(msg_text.c_str(),fontStr_KlavikaBold,30);
	  gk_season->setAnchorPoint(CCPoint(0.0f,0.5f));
	  gk_season->setPosition(CCPoint(back_gfhave->getContentSize().width*0.08f,back_gfhave->getContentSize().height/2));
	  back_gfhave->addChild(gk_season);

	  //查看按钮 ZT_anniu_chakan.png chankanguanfangjiefa_tag
	  string ck_str=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_chakan.png");
	  CCMenuItemImage* ckgfjiefa=CCMenuItemImage::create(ck_str.c_str(),ck_str.c_str(),this,
		  menu_selector(Goluetu::chankangfjieFa));
	  ckgfjiefa->setTag(chankanguanfangjiefa_tag);
	  ckgfjiefa->setPosition(CCPoint(back_gfhave->getContentSize().width - ckgfjiefa->boundingBox().size.width/2 - 10, back_gfhave->getContentSize().height/2));
	  fillOuty(ckgfjiefa,ck_str);

	  CCMenu* cmenu=CCMenu::create(ckgfjiefa,nullptr);
	  cmenu->setTouchPriority(kCCMenuHandlerPriority-1);
	  cmenu->setPosition(CCPointZero);
	  back_gfhave->addChild(cmenu);

      
	  {	  
		  back_nogfslution=CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_3.png").c_str());
		  back_nogfslution->setPosition(CCPoint(back->getContentSize().width/2,back->getContentSize().height*0.8f));
		  back->addChild(back_nogfslution);

		  CCSprite* glTn=CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_gongluetu.png").c_str()); 
		  glTn->setAnchorPoint(CCPoint(0.0f,0.5f));
		  glTn->setPosition(CCPoint(-glTn->getContentSize().width/2,back_nogfslution->getContentSize().height/2));
		  back_nogfslution->addChild(glTn);

		  //是哪关 msg
		  string Gk=CCString::createWithFormat("sd%d",LevelManager::getManager()->getLastSeasonId())->getCString();
		  string gk_ms=CCString::createWithFormat(" %d-%d",LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId())->getCString();
		  string msg_text=LFStrings::getValue("allgk");
		  CCLabelTTF* gk_season = CCLabelTTF::create(msg_text.c_str(),fontStr_KlavikaBold,30);
		  gk_season->setAnchorPoint(CCPoint(0.0f,0.5f));
		  gk_season->setPosition(CCPoint(back_nogfslution->getContentSize().width*0.08f,back_nogfslution->getContentSize().height/2));
		  back_nogfslution->addChild(gk_season);


		  string buy_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_goumai.png");
		  CCMenuItemImage* buy_ccbt=CCMenuItemImage::create(buy_bt.c_str(),buy_bt.c_str(),this,
			  menu_selector(Goluetu::allCallback_buy));
		  buy_ccbt->setPosition(CCPoint(back_nogfslution->getContentSize().width-buy_ccbt->getContentSize().width*0.8f,back_nogfslution->getContentSize().height*0.5));

		  fillOuty(buy_ccbt,buy_bt);
		  CCMenu* pMenu = CCMenu::create(buy_ccbt,NULL);
		  pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
		  pMenu->setPosition(CCPointZero);
		  back_nogfslution->addChild(pMenu, 1);

		  //price
		  string price="$ 2.99";
		  CCLabelTTF* labelttf=CCLabelTTF::create(price.c_str(),fontStr_KlavikaBold,30);
		  labelttf->setPosition(CCPoint(buy_ccbt->getContentSize().width*0.5f+buy_ccbt->getPositionX()+labelttf->getContentSize().width/2+5,back_nogfslution->getContentSize().height/2));
		  back_nogfslution->addChild(labelttf);
      }

	  refreshgfjiefa(); 
	  //标题 2
	  string biao_th=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_biaoti.png"); 
	  CCSprite* tmph = CCSprite::create(biao_th.c_str());
	  CCSize sizeh = tmph->getContentSize();

	  CCRect fullRecth= CCRectMake(0,0, size.width, size.height);
	  CCRect insetRecth=CCRectMake(20, 0,20, size.height);
	  CCScale9Sprite* bt_scaleh=CCScale9Sprite::create(biao_th.c_str(),fullRect,insetRect);
	  bt_scaleh->setContentSize(CCSize(270, size.height));
	  back->addChild(bt_scaleh);
	  bt_scaleh->setAnchorPoint(ccp(0.0f, 0.5f));


	  string f_text=LFStrings::getValue("fsolution");
	  CCLabelTTF* fg_label = CCLabelTTF::create(f_text.c_str(),fontStr_KlavikaBold,40);
	  fg_label->setAnchorPoint(CCPoint(0.0f,0.5f));
	  bt_scaleh->setContentSize(CCSize(g_label->getContentSize().width+100, size.height));
	  bt_scaleh->setPosition(CCPoint(-20, back->getContentSize().height*0.65f));
	  fg_label->setPosition(CCPoint(30, bt_scaleh->getContentSize().height/2));
	  bt_scaleh->addChild(fg_label);

	  //好友列表解法
	  //好友列表选折 Baselisttable*
	  list_friend=Baselisttable::create();
	  list_friend->setMtabletype(Friendlist,CCSize(650,83),3,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,20);
	  list_friend->ignoreAnchorPointForPosition(false);
	  list_friend->setAnchorPoint(CCPoint(0.5f,0.5f));
	  list_friend->setPosition(CCPoint(back->getContentSize().width/2,back->getContentSize().height*0.3f));
	  back->addChild(list_friend,1025);
	  drawlogin();
	  refreshloginlayer();
	  goltu_layer->setScale(chaonengdj_scale);

	  //_inviteButton = createInviteButton(back);
	
}
#if 0
CCMenu* Goluetu::createInviteButton(CCNode* parent)
{
	string yao_str=ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_yaoqing.png");
	string st_str=LFStrings::getValue("yaoqin");

	CCSprite* itemSpr = CCSprite::create(yao_str.c_str());
	CCLabelTTF* label =CCLabelTTF::create(st_str.c_str(),fontStr_KozukaGothicProEL,40);
	label->setPosition(CCPoint(itemSpr->getContentSize().width/2, itemSpr->getContentSize().height/2));
	itemSpr->addChild(label);

	CCSprite* itemSpr2 = CCSprite::create(yao_str.c_str());
	CCLabelTTF* label2 =CCLabelTTF::create(st_str.c_str(),fontStr_KozukaGothicProEL,40);
	label2->setPosition(CCPoint(itemSpr2->getContentSize().width/2, itemSpr2->getContentSize().height/2));
	itemSpr2->addChild(label2);
	itemSpr2->setAnchorPoint(ccp(0.5f, 0.5f));
	itemSpr2->setColor(ccc3(200, 200, 200));
	

	CCMenuItemImage* item = CCMenuItemImage::create();
	item->setScale(0.7f);
	item->initWithNormalSprite(itemSpr, itemSpr2, NULL, this, menu_selector(Goluetu::onInviteClick));
	CCMenu* menu = CCMenu::create(item, NULL);

	menu->setTouchPriority(kCCMenuHandlerPriority-1);
	 menu->setPosition(ccp(parent->getContentSize().width/2, itemSpr->getContentSize().height/2 + 3));
	 parent->addChild(menu, list_friend->getZOrder() + 1);

	return menu;
}

void Goluetu::onInviteClick(CCObject* obj)
{
	if (!CmdHelper::getInstance()->isLogin())
	{
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Goluetu::loginResult), NetMessage::MSG_LoginResult, NULL);
		CmdHelper::getInstance()->login();
	}
	else
	{
		CmdHelper::getInstance()->inviteFriends();
	}
}

void Goluetu::loginResult(CCObject* pMsg)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NetMessage::MSG_LoginResult);

	MsgData* pdata = (MsgData*)pMsg;
	if (pdata)
	{
		if(pdata->errCode == MsgData::eSuccess)
		{
			CCMessageBox("Login Success!", "Tip");
			LevelManager::getManager()->GameInToMainLayer();
			return;
		}
	}
	CCMessageBox("Login Faild!", "Tip");
}
#endif
void Goluetu::refreshloginlayer(){

//f(CmdHelper::getInstance()->isLogin()){
		list_friend->setVisible(true);
		loginlayer->setVisible(false);
//else{
	//ist_friend->setVisible(false);
//loginlayer->setVisible(true);
//

}

void Goluetu::drawlogin(){

	 loginlayer=CCLayer::create();
	 loginlayer->ignoreAnchorPointForPosition(false);
	 loginlayer->setAnchorPoint(CCPoint(0.5f,0.5f));
	 loginlayer->setContentSize(CCSize(740,360));
     
	 //login
	 string ck_str=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_facebook_log_bg.png"); 
	 CCMenuItemImage* logo_facebook=CCMenuItemImage::create(ck_str.c_str(),ck_str.c_str(),this,
		 menu_selector(Goluetu::loginfaceBook));
	 logo_facebook->setPosition(CCPoint(loginlayer->getContentSize().width/2,loginlayer->getContentSize().height/2));
	 fillOuty(logo_facebook,ck_str);

	 CCLabelTTF* label =CCLabelTTF::create(LFStrings::getValue("login").c_str(),fontStr_KlavikaBold, 30);
	 label->setPosition(CCPoint(logo_facebook->getContentSize().width/2 + 45,  logo_facebook->getContentSize().height/2));
	 logo_facebook->addChild(label);


	 CCMenu* cmenu=CCMenu::create(logo_facebook,nullptr);
	 cmenu->setTouchPriority(kCCMenuHandlerPriority-1);
	 cmenu->setPosition(CCPointZero);
	 loginlayer->addChild(cmenu);


	 //frsolu
	 string fsoltion_str=LFStrings::getValue("frsolu");
	 CCLabelTTF* lable=CCLabelTTF::create(fsoltion_str.c_str(),fontStr_KlavikaBold,30);
	 lable->setPosition(CCPoint(loginlayer->getContentSize().width/2,loginlayer->getContentSize().height*0.25f));
	 loginlayer->addChild(lable);
	 loginlayer->setPosition(CCPoint(back->getContentSize().width/2,back->getContentSize().height*0.4f));
	 back->addChild(loginlayer);


}


void Goluetu::loginfaceBook(CCObject* ob){

	 Music::sharedMusicPlayer()->buttonClicked();
	 CmdHelper::getInstance()->login(); 
	 
	 

}

void Goluetu::logincallback(CCObject* ob){

	 MsgData* pdata = (MsgData*)ob;
	 if(pdata->errCode==MsgData::eSuccess){
		 refreshloginlayer();
		 snsgetData();
	 }
}

void Goluetu::refreshgfjiefa(){

	if (GameInforEditer::getGameInfor()->getHasPayOfficialSolution()==false)
	{
		back_gfhave->setVisible(false);
		back_nogfslution->setVisible(true);
	}else if(GameInforEditer::getGameInfor()->getHasPayOfficialSolution()==true){
		back_gfhave->setVisible(true);
		back_nogfslution->setVisible(false);
	}
}

void Goluetu::allCallback_buy(CCObject* object){
	Music::sharedMusicPlayer()->buttonClicked();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Goluetu::payResult), NetMessage::MSG_PayResult, NULL);
	CmdHelper::getInstance()->buyProduct(ProductID_OfficialSolution);
	
}


void Goluetu::chankangfjieFa(CCObject *object){
	
	if(current_layermark==glutulayer_tag){

	}else{
		return;
	}
    Music::sharedMusicPlayer()->buttonClicked();
	int index = GameLayerFactory::getInstance()->getSectionIndex(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
	
	vector<vector<record> > allRecordData;
	if(!Recorder::getInstance()->getOfficialRecord(index, &allRecordData))
	{  
		Shieldingmanger::getSielding()->setPingbingbutton(false);
		CCLOG("not found the OfficialSolution");
		return;
	}
	setAllRecordData(&allRecordData);

	drawguanfalayer();
	Fadeoutnodes(goltu_layer);
	goltu_layer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
		CCCallFunc::create(this,callfunc_selector(Goluetu::actiongf)),
		NULL
		)
		);	   
    //gflayer

}

void Goluetu::drawguanfalayer(){
	 
	 if(isgfjiefa==false){

		 isgfjiefa=true;
		 gflayer=CCLayer::create();
		 gflayer->setTag(gflayertag);
		 gflayer->setContentSize(size);
		 addChild(gflayer);
		 gflayer->ignoreAnchorPointForPosition(false);
		 gflayer->setPosition(CCPoint(size.width/2,size.height/2));
		 gflayer->setVisible(false);

	 }else{
	 }

	 if (!pPlayRecordLayer_Official)
	 {
		 addplaygf();
	 }

	 gflayer->setScale(chaonengdj_scale);
}

void Goluetu::addplaygf()
{
	if (m_allRecordData.size() <= 0)
	{
		CCLOG("%s----------->m_allRecordData size is 0 !!!!", __FUNCTION__);
//		CCAssert(false, "m_allRecordData is not should be 0 ! \nYou should be call setAllRecordData first!");
//		return;
	}

	bool bAutoPlay = true;
//	if (pPlayRecordLayer)
	{
		bAutoPlay = false;
	}
	int seasonid = LevelManager::getManager()->getLastSeasonId();
	int sectionid = LevelManager::getManager()->getLastSectionId();
	//播放
	pPlayRecordLayer_Official = GameScenseDefine::SharedGameScenseDefine()->playRecord(seasonid, sectionid, &m_allRecordData, bAutoPlay);
	pPlayRecordLayer_Official->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));// - pPlayRecordLayer->getContentSize().height/2*pPlayRecordLayer->getScale() - 50));
	
	
	CCSprite* recordBG = CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_bg.png").c_str());
	gflayer->addChild(recordBG, pPlayRecordLayer_Official->getZOrder() - 1);
	recordBG->setPosition(pPlayRecordLayer_Official->getPosition());

	CCSprite* recordTitleBG = CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_biaoti_guanfang.png").c_str());
	recordBG->addChild(recordTitleBG);
	recordTitleBG->setPosition( ccp(recordTitleBG->getContentSize().width/2, recordBG->getContentSize().height +recordTitleBG->getContentSize().height/2 ) );

	string start_strl=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_guanbi.png");
	startmenu_l=CCMenuItemImage::create(start_strl.c_str(),start_strl.c_str(),this,menu_selector(Goluetu::allCallback));
	startmenu_l->setTag(close_gfgltutag);
	CCSize sizepl=CCSize(500,500);//pPlayRecordLayer->getContentSize();
	startmenu_l->setPosition(CCPoint(size.width/2+sizepl.width/2,size.height/2+sizepl.height/2));
	CCMenu* pMenu = CCMenu::create(startmenu_l,NULL);
	pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
	pMenu->setPosition(CCPointZero);
	gflayer->addChild(pMenu,1000);

	string bt_text=LFStrings::getValue("gsolution");
	CCLabelTTF* recordTitle = CCLabelTTF::create(bt_text.c_str(),fontStr_KlavikaBold,30);
	recordTitle->setAnchorPoint(CCPoint(1.0f,0.5f));
	recordTitle->setPosition(ccp(recordTitleBG->getContentSize().width - startmenu_l->getContentSize().width, recordTitleBG->getContentSize().height/2));
	recordTitleBG->addChild(recordTitle);

	gflayer->addChild(pPlayRecordLayer_Official, 1, Tag_Layer_PlayRecord);
	startmenu_l->setPosition(CCPoint(recordBG->getPositionX() + recordBG->getContentSize().width/2 - startmenu_l->getContentSize().width/2 + 10,
									 recordBG->getPositionY() + recordBG->getContentSize().height/2 + startmenu_l->getContentSize().height/2 - 9));

}


void Goluetu::actiongf(){

	list_friend->getTaleView()->setTouchEnabled(false);
	 goltu_layer->setVisible(false);
	 current_layermark=guanfalayer_tag;
	 gflayer->setVisible(true);

	 Fadeinnodes(gflayer);
	 gflayer->runAction(
		 CCSequence::create(
		 CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
		 NULL
		 )
		 );	   

}





void Goluetu::payResult(CCObject* pMsg)
{
	
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NetMessage::MSG_PayResult);
	MsgData* pdata = (MsgData*)pMsg;
	if (pdata && pdata->errCode == MsgData::eSuccess)
	{
		MessageBar::show(LFStrings::getValue("Purchasesuccessfully").c_str());
		//CCMessageBox("Buy successfully", "Tip"); 
		CCMenuItemImage iamg;
		ProductID id = *(ProductID*)pdata->data;
		if (id == ProductID_OfficialSolution)
		{
		   refreshgfjiefa();
		}
	}
	else
	{
		MessageBar::show(LFStrings::getValue("Purchasefailed").c_str());
		//CCMessageBox("Purchase failed", "Tip");
	}
}



void Goluetu::snsgetData(){


	const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	if(info){
		this->info_layer=info;
		shuaixinData();
	}else{
		mark_sns=true;
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Goluetu::snsData), NetMessage::MSG_GotSeasonSocialInfo, NULL);
		CmdHelper::getInstance()->getSeasonSocialInfo(LevelManager::getManager()->getLastSeasonId(), false);
	}
}



void Goluetu::snsData(CCObject* obj){

	const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	if(info){
		this->info_layer=info;
		shuaixinData();
	}
}




void Goluetu::shuaixinData(){

     this->list_friend->friendSolutionlist(info_layer,true);
	 
	// _inviteButton->setVisible(info_layer == NULL || info_layer->users.empty());
}

void  Goluetu::refreshSolution(CCObject* obj){

	Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
	MsgData* data = (MsgData*)obj;
	if (data->errCode == MsgData::eSuccess)
	{
		if(freind_solution){
			son_praent(freind_solution);
		}
	}
	else 
	{
		MessageBar::show(LFStrings::getValue("nofriendsolution").c_str());
		//CCMessageBox(LFStrings::getValue("nofriendsolution").c_str(), "Error");

		//analytics
		//统计查看好友解法事件  --- end (error)
		const char* pEventInfo = CCString::createWithFormat("SeeFriendsSolution%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();	
		CmdHelper::getInstance()->getAnalyticsInstance()->onFailed(pEventInfo, "down load solution failed");
	}
}

void Goluetu::son_praent(CCObject *object){

    
	if(current_layermark==glutulayer_tag){
        
	}else{
		return;
	}
    
	//连续点击事件屏蔽
	if(Shieldingmanger::getSielding()->continuousThetimer()==false){

		return;
	}

	Myobject* ob=(Myobject*)object; 
	//analytics
	//统计查看好友解法事件  --- begin
	const char* pEventInfo = CCString::createWithFormat("SeeFriendsSolution%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();	
	CmdHelper::getInstance()->getAnalyticsInstance()->onBegin(pEventInfo);

	//string datastring = SocialInfoManager::getInstance()->getSolutionData(ob->data.solutionId);
	string datastring = SocialInfoManager::getInstance()->getSolutionData(ob->data.seasonId, ob->data.sectionId, ob->data.userId);
	freind_solution=object;
	if (datastring.empty())
	{
		CCLOG("local is not found data!");
		// NetMessage::MSG_DownloadSolution  succese  string datastring = SocialInfoManager::getInstance()->getSolutionData(ob->data.solutionId);
		CmdHelper::getInstance()->downloadTaskSolution(ob->data.solutionId, true);
		return;
	}
	string curUserId;
	bool hasAcount = CmdHelper::getInstance()->getCurrentUserID(curUserId);
	// 是查看自己，且本地没有数据才直接弹框返回
	if (!Recorder::getInstance()->getRecordFromString(datastring, &m_allRecordData) && hasAcount && ob->data.userId == curUserId)
	{
		MessageBar::show(LFStrings::getValue("NODataFound").c_str());
		//CCMessageBox("NO Data Found","Warning");
		//analytics
		//统计查看好友解法事件  --- end (error)
		const char* pEventInfo = CCString::createWithFormat("SeeFriendsSolution%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();	
		CmdHelper::getInstance()->getAnalyticsInstance()->onFailed(pEventInfo, "get record from string error");
		Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
		return;
	}
	if(isliandian==false){

		return;
	}
	isliandian=false;

	Fadeoutnodes(goltu_layer);
	goltu_layer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
		CCCallFunc::create(this,callfunc_selector(Goluetu::actionF)),
		NULL
		)
		);

	recorld_data=ob->data;
	drawPlaylayer(ob->data);


	{	
		//analytics
		//统计查看好友解法事件  --- end (success)
		const char* pEventInfo = CCString::createWithFormat("SeeFriendsSolution%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();	
		CmdHelper::getInstance()->getAnalyticsInstance()->onCompleted(pEventInfo);
	}
}


void Goluetu::actionF(){


	Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
	isliandian=true;
	list_friend->getTaleView()->setTouchEnabled(false);
	goltu_layer->setVisible(false);
	answer->setVisible(true);
	current_layermark=freindlayer_tag;
	Fadeinnodes(answer);
	answer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
		NULL
		)
		);	   
}


void Goluetu::drawPlaylayer(msgSnsdata data){


	current_layermark=freindlayer_tag;

	if(!is_answer){

		answer=ShieldTouchlayer::create();
		answer->setContentSize(size);
		answer->setVisible(false);
		addChild(answer);
	}
	
	answer->setScale(chaonengdj_scale);
	CCScrollView* pPlayRecordLayer = GameScenseDefine::SharedGameScenseDefine()->playRecord(data.seasonId, data.sectionId, &m_allRecordData, false);
	pPlayRecordLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height / 2));
	answer->addChild(pPlayRecordLayer, 1, Tag_Layer_PlayRecord);

	if (m_allRecordData.size() <= 0)
	{
		CCLOG("%s----------->m_allRecordData size is 0 !!!!", __FUNCTION__);

		CCLabelTTF* pLabel = CCLabelTTF::create(LFStrings::getValue("nofriendsolution").c_str(), fontStr_KlavikaBold, 40);
		pLabel->setScale(1.5f);
		pPlayRecordLayer->addChild(pLabel, 1);
		pLabel->setPosition(ccp(pPlayRecordLayer->getViewSize().width/2 - pLabel->getContentSize().width*pLabel->getScale()/2, pPlayRecordLayer->getViewSize().height/2 + 200));
	}

	list_friend->getTaleView()->setTouchEnabled(false);

	CCSprite* recordBG = CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_bg.png").c_str());
	answer->addChild(recordBG, pPlayRecordLayer->getZOrder() - 1);
	recordBG->setPosition(pPlayRecordLayer->getPosition());


	if(is_answer==false){
		

		//标题栏和标题
		string titleBGPath = ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_biaoti_haoyou.png");
		CCSprite* titleBG = CCSprite::create(titleBGPath.c_str());
		titleBG->setPosition(ccp(titleBG->getContentSize().width/2, recordBG->getContentSize().height + titleBG->getContentSize().height/2));
		recordBG->addChild(titleBG);

		//底部条
		string bottomPath = ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_haoyouxinxi_bg.png");
		CCSprite* infoBG = CCSprite::create(bottomPath.c_str());
		infoBG->setPosition(ccp(titleBG->getContentSize().width/2, -infoBG->getContentSize().height/2));
		recordBG->addChild(infoBG);

		//头像
		string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
		toux_sp=urlSprite::create(tb_file.c_str(),data.pictrue.c_str());
        toux_sp->setScaleX(62.0f/toux_sp->getContentSize().width);
		toux_sp->setScaleY(62.0f/toux_sp->getContentSize().height);
		recordBG->addChild(toux_sp);
		toux_sp->setPosition(ccp(87 + toux_sp->boundingBox().size.width/2, -toux_sp->boundingBox().size.height/2));

		//名字
		string name=data.name;
		name_label = CCLabelTTF::create(name.c_str(),fontStr_KlavikaBold,30);
		name_label->setAnchorPoint(ccp(0, 0.5f));
		name_label->setPosition(toux_sp->getPosition() + ccp(toux_sp->boundingBox().size.width/2 + 10, 0));
		recordBG->addChild(name_label);

		//点赞图标
		string z_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan_2.png");
		string z_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan_2_select.png");
		z_ccbt=CCMenuItemImage::create(z_bt.c_str(),z_select.c_str(),this,
			menu_selector(Goluetu::zan_button));
		z_ccbt->setUserData((void*)data.solutionId.c_str());

		CCMenu* pMenu = CCMenu::create(z_ccbt,NULL);
		pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
		pMenu->setPosition(ccp(recordBG->getContentSize().width - z_ccbt->getContentSize().width/2 + 10, - z_ccbt->getContentSize().height/2 + 10));
		recordBG->addChild(pMenu, 1);

		//点赞次数
		string num_st=CCString::createWithFormat("%d",data.upTimes)->getCString();
		z_label=CCLabelTTF::create(num_st.c_str(),fontStr_KlavikaBold,30);
		z_label->setPosition(pMenu->getPosition() - CCPoint(z_label->getContentSize().width/2 + z_ccbt->getContentSize().width/2 - 5, 0));
		recordBG->addChild(z_label);
		//
	}else{
		string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
		toux_sp->init(tb_file.c_str(),data.pictrue.c_str());
		name_label->setString(data.name.c_str());
		z_label->setString(CCString::createWithFormat("%d",data.upTimes)->getCString());
		z_ccbt->setUserData((void*)data.userId.c_str());
	}
    
	if(is_answer==false){
		is_answer=true;
		string start_strl=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_guanbi.png");
		CCMenuItemImage*  startmenu_2=CCMenuItemImage::create(start_strl.c_str(),start_strl.c_str(),this,menu_selector(Goluetu::allCallback));
		startmenu_2->setTag(close_gltutag);
		//startmenu_2->setPosition(CCPoint(pPlayRecordLayer->getPositionX() +pPlayRecordLayer->getContentSize().width/2*pPlayRecordLayer->getScale(),pPlayRecordLayer->getPositionY()+pPlayRecordLayer->getContentSize().height/2*pPlayRecordLayer->getScale()));
		//fillOuty(startmenu_2,start_strl);
		CCMenu* pMenu = CCMenu::create(startmenu_2,NULL);
		pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
		pMenu->setPosition(ccp(recordBG->getContentSize().width - startmenu_2->getContentSize().width/2 + 10, recordBG->getContentSize().height + startmenu_2->getContentSize().height/2 - 9));
		recordBG->addChild(pMenu,1000);

		string bt_text=LFStrings::getValue("fsolution");
		CCLabelTTF* recordTitle = CCLabelTTF::create(bt_text.c_str(),fontStr_KlavikaBold,30);
		recordTitle->setAnchorPoint(CCPoint(1.0f,0.5f));
		recordTitle->setPosition(pMenu->getPosition() - ccp(startmenu_2->getContentSize().width/2 + 5, 0));
		recordBG->addChild(recordTitle);
	}else{
      
	}

}

void Goluetu::fillOuty(CCMenuItemImage* p,string imgfile){

	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.10f);
	p->setSelectedImage(s);
	CCSprite* n=CCSprite::create(imgfile.c_str());
	CCSprite* layer=CCSprite::create();
	layer->setContentSize(CCSize(p->getContentSize()*1.10f));
	n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	layer->addChild(n);
	p->setNormalImage(layer);

	if(imgfile.compare(ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_goumai.png"))==0){

		string st_str=LFStrings::getValue("buy");
		CCLabelTTF* inN=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		inN->setPosition(CCPoint(s->getContentSize().width*0.5f,s->getContentSize().height/2));
		inN->setColor(ccc3(3,79,132));
		s->addChild(inN);

		CCLabelTTF* inS=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		inS->setPosition(CCPoint(layer->getContentSize().width*0.5f,layer->getContentSize().height/2));
		inS->setColor(ccc3(3,79,132));
		layer->addChild(inS);

	}
}


void Goluetu::allCallback(CCObject *object){

	if(iscloseactiondone==false){
		return;
	}
	 Music::sharedMusicPlayer()->buttonClicked();
	 CCMenuItemImage* p=(CCMenuItemImage*)object;
	 if(p->getTag()==close_gltutag){

         current_layermark=glutulayer_tag;
		 Fadeoutnodes(answer);
		 iscloseactiondone=false;
		 answer->runAction(
			 CCSequence::create(
			 CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			 CCCallFunc::create(this,callfunc_selector(Goluetu::answerchangeyuan)),
			 NULL
			 )
			 );	   
	 }else if(p->getTag()==close_gfgltutag){
		 if(isclickZ==false){
			 //去掉点赞后才能关闭
//			 return;
		 }
		 isclickZ=false;
		 current_layermark=glutulayer_tag;
		 Fadeoutnodes(gflayer);
		 iscloseactiondone=false;
		 gflayer->runAction(
			 CCSequence::create(
			 CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			 CCCallFunc::create(this,callfunc_selector(Goluetu::gfchangemain)),
			 NULL
			 )
			 );	   
	 }
}
void Goluetu::gfchangemain(){

	gflayer->removeAllChildren();
	pPlayRecordLayer_Official = NULL;
	list_friend->getTaleView()->setTouchEnabled(true);
	gflayer->setVisible(false);
	goltu_layer->setVisible(true);
	iscloseactiondone=true;
	Fadeinnodes(goltu_layer);
	goltu_layer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
		NULL
		)
		);	   
	refreshgfjiefa();

}

void Goluetu::answerchangeyuan(){

	list_friend->getTaleView()->setTouchEnabled(true);
	answer->removeFromParent();
	answer=nullptr;
    is_answer=false;
	goltu_layer->setVisible(true);
	iscloseactiondone=true;
	Fadeinnodes(goltu_layer);
	goltu_layer->runAction(
		    CCSequence::create(
		     CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
		    NULL
		  )
		);	   
}

void Goluetu::zan_button(CCObject* object){
	 

	if(isclickZ==true){
		isclickZ=false;
		Myobject* ob=(Myobject*)freind_solution;
		string solutionid=ob->data.solutionId;
		if(solutionid.size()>0){
			list_friend->clickZ_idx(solutionid);
		}
	}
}


void Goluetu::click_zan(CCObject* object){
   
	MsgData* data=(MsgData*)object;
	if(data->errCode == MsgData::eSuccess&&is_answer==true){
		//
		const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
		if(info){
			dataUnion.clear();
			vector<TaskSolution> us=info->users;
			for(size_t i=0;i<us.size();i++){
				msgSnsdata mdata;
				mdata.name="";
				mdata.pictrue=""; //url
				mdata.userId="";
				mdata.seasonId=us.at(i).seasonId;
				mdata.sectionId=us.at(i).sectionId;
				mdata.solutionId=us.at(i).solutionId; //解法id，由服务器分配
				mdata.star=us.at(i).star;   //解法评星
				mdata.score=us.at(i).score; //解法分数
				mdata.downloadTimes=us.at(i).downloadTimes; //下载次数
				mdata.upTimes=us.at(i).upTimes; //赞次数
				mdata.solutionData=us.at(i).solutionData;//解法数据， 没有取时 为空
				const TaskUser*  p_user=SocialInfoManager::getInstance()->getUserInfor(us.at(i).userId);
				if(p_user){
					mdata.name=p_user->name;
					mdata.pictrue=p_user->pictrue; //url
					mdata.userId=p_user->userId;
				}
				dataUnion.push_back(mdata);
			}
			for(size_t i=0;i<dataUnion.size();i++){
				if(dataUnion.at(i).solutionId.compare(recorld_data.solutionId)==0){
					string z_num=CCString::createWithFormat("%d",dataUnion.at(i).upTimes)->getCString();
					z_label->setString(z_num.c_str());
					break;
				}
			}
		}
	}
	else
	{
		isclickZ = false;
	}

	
}

//
void Goluetu::Fadeinnodes(CCNode* node){

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

void Goluetu::Fadeoutnodes(CCNode* node){

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


