#include "Guankamsg.h"
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
#include "GameInforEditer.h"

#include "BaseContentLayer.h"
#include "BaseScense.h"
#include "Shieldingmanger.h"
#include "MessageBar.h"
#include "Music.h"


enum ChangeTag
{
	 friendlayertag=1020,
	 tag_recoder_BG,
	 Zannum_tag,

};

Guankamsg::Guankamsg(void)
{


}

Guankamsg::Guankamsg(int season,int gkid){

	this->currnt_season=season;
	this->currnt_gk=gkid;

	LevelManager::getManager()->setLastSeasonId(season);
	LevelManager::getManager()->setLastSectionId(gkid);



}



Guankamsg::~Guankamsg(void)
{

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

}

Guankamsg* Guankamsg::createByid(int season,int gkid){

	Guankamsg* pRet=new Guankamsg(season,gkid);
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
	    return NULL; 
	}

}

void  Guankamsg::snsData(CCObject* obj){

	 const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(currnt_season,currnt_gk);
	 if(info){
			this->info_layer=info;
			shuaixinData();
	 }
}

void Guankamsg::snsgetData(){

		 const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(currnt_season,currnt_gk);
		 if(info){
			 this->info_layer=info;
             shuaixinData();
		 }else{
             CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Guankamsg::snsData), NetMessage::MSG_GotSeasonSocialInfo, NULL);
		 }
	
}





//好友解法列表
void  Guankamsg::friendSolutionlist(){

	dataUnion.clear();
	vector<TaskSolution> us=info_layer->users;
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

}

void Guankamsg::shuaixinData(){

	  /*
	  int maxScore;
	  int avgScore;
	  int playerCount;
	  lablen1->setString(CCString::createWithFormat("%d",info_layer->avgScore)->getCString());
	  lablen2->setString(CCString::createWithFormat("%d",info_layer->maxScore)->getCString());
	  lablen3->setString(CCString::createWithFormat("%d",info_layer->playerCount)->getCString());
	  */
	  vector<TaskSolution> users=info_layer->users;
	  list->setGkmsglayeruser(users);
	  friendSolutionlist();

}


bool Guankamsg::init(){

	if(!CCLayer::init()){
		return false;
	}
	m_bIsPlayRecord = false;
	freind_solution=nullptr;
	isgklayer=true;
	isclickZ=true;
	is_solution=false;
	size=CCDirector::sharedDirector()->getWinSize();
	maxsocore=0;
	SeasonInfor* s_msg=GameInforEditer::getGameInfor()->getSeasonInfor(currnt_season);
	if(s_msg){

		if(s_msg->getAllSectionInfors().size()>0){

			std::list<SectionInfor*> lsitseinfo= s_msg->getAllSectionInfors();
			std::list<SectionInfor*>::iterator it;
			for (it = lsitseinfo.begin(); it!=lsitseinfo.end(); it++) {
				msg_setion.push_back(*it);
			} 
		}

	}

	for(size_t i=0;i<msg_setion.size();i++){

		if(msg_setion.at(i)->sectionId==currnt_gk){
			if(msg_setion.at(i)->isSuccessed==true){
				maxsocore=msg_setion.at(i)->iStarNum;
			}else{
				maxsocore=0;
			}
		   break;
		}
	}
	//drawGklayer();
	//gkmsg_layer=CCLayer::create();



	center_time=0.5f;
	left_time=0.5f;
	rght_time=0.5f;

	num_time=0;
	all_time=145;
	back_time=20;
	bt_time=30;
	first_actiontime=45;
	xinxin_time=50;
	text_time=60;
	butoon_time=75;
	dom_buttontime=75;
	newcorlds_time=120;
    float ever_time=0.01f;

	ccColor4B mr;
	mr.r=0;mr.g=0;mr.b=0;mr.a=0;
	color = CCLayerColor::create(mr);
	addChild(color);
	color->setVisible(false);


	layerinit();
    this->schedule(schedule_selector(Guankamsg::actionupdate),ever_time);
    snsgetData();
	//centeraction();
	
	
	return true;
}

bool Guankamsg::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

	return true;
}


void Guankamsg::drawfriendlayer(msgSnsdata data){

	    data_play=data;
		ShieldTouchlayer* friend_solutionlayer=ShieldTouchlayer::create();
		friend_solutionlayer->setTag(friendlayertag);
		friend_solutionlayer->setContentSize(size);
		friend_solutionlayer->setScale(chaonengdj_scale);
		friend_solutionlayer->setVisible(false);
		addChild(friend_solutionlayer);

		CCScrollView* pPlayRecordLayer = (CCScrollView*)friend_solutionlayer->getChildByTag(Tag_Layer_PlayRecord);
		if (!pPlayRecordLayer)
		{
			m_bIsPlayRecord = true;
			pPlayRecordLayer = GameScenseDefine::SharedGameScenseDefine()->playRecord(data.seasonId, data.sectionId, &m_allRecordData, false);
			pPlayRecordLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height / 2));
			friend_solutionlayer->addChild(pPlayRecordLayer, 1, Tag_Layer_PlayRecord);

			if (m_allRecordData.size() <= 0)
			{
				CCLOG("%s----------->m_allRecordData size is 0 !!!!", __FUNCTION__);

				CCLabelTTF* pLabel = CCLabelTTF::create(LFStrings::getValue("nofriendsolution").c_str(), fontStr_KlavikaBold, 40);
				pLabel->setScale(1.5f);
				pPlayRecordLayer->addChild(pLabel, 1);
				pLabel->setPosition(ccp(pPlayRecordLayer->getViewSize().width/2 - pLabel->getContentSize().width*pLabel->getScale()/2, pPlayRecordLayer->getViewSize().height/2 + 200));
			}
		}

		list->getTaleView()->setTouchEnabled(false);

		CCSprite* recordBG = CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_bg.png").c_str());
		friend_solutionlayer->addChild(recordBG, pPlayRecordLayer->getZOrder() - 1, tag_recoder_BG);
		recordBG->setPosition(pPlayRecordLayer->getPosition());

		string start_strl=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_guanbi.png");
		CCMenuItemImage*  startmenu_l=CCMenuItemImage::create(start_strl.c_str(),start_strl.c_str(),this,menu_selector(Guankamsg::allCallback));
		startmenu_l->setTag(close_friendsolutiontag);

		CCMenu* pMenu = CCMenu::create(startmenu_l,NULL);
		pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
		pMenu->setPosition(ccp(recordBG->getContentSize().width - startmenu_l->getContentSize().width/2 + 10, recordBG->getContentSize().height + startmenu_l->getContentSize().height/2 - 9));
		recordBG->addChild(pMenu,1000);



		//标题栏和标题
		string titleBGPath = ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_biaoti_haoyou.png");
		CCSprite* titleBG = CCSprite::create(titleBGPath.c_str());
		titleBG->setPosition(ccp(titleBG->getContentSize().width/2, recordBG->getContentSize().height + titleBG->getContentSize().height/2));
		recordBG->addChild(titleBG);

		string bt_text=LFStrings::getValue("fsolution");
		CCLabelTTF* recordTitle = CCLabelTTF::create(bt_text.c_str(),fontStr_KlavikaBold,30);
		recordTitle->setAnchorPoint(CCPoint(1.0f,0.5f));
		recordTitle->setPosition(pMenu->getPosition() - ccp(startmenu_l->getContentSize().width/2 + 5, 0));
		recordBG->addChild(recordTitle);

		//底部条
		string bottomPath = ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_haoyouxinxi_bg.png");
		CCSprite* infoBG = CCSprite::create(bottomPath.c_str());
		infoBG->setPosition(ccp(titleBG->getContentSize().width/2, -infoBG->getContentSize().height/2));
		recordBG->addChild(infoBG);

		//头像
		string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
		urlSprite* toux_sp=urlSprite::create(tb_file.c_str(),data.pictrue.c_str());
		//toux_sp->setScale(1.24f);
		recordBG->addChild(toux_sp);
		toux_sp->setPosition(ccp(87 + toux_sp->getContentSize().width/2, -toux_sp->getContentSize().height/2-1));

		//名字
		string name=data.name;
		CCLabelTTF* name_label = CCLabelTTF::create(name.c_str(),fontStr_KlavikaBold,30);
		name_label->setAnchorPoint(ccp(0, 0.5f));
		name_label->setPosition(toux_sp->getPosition() + ccp(toux_sp->getContentSize().width/2 + 10, 0));
		recordBG->addChild(name_label);

		//点赞图标
		string z_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan_2.png");
		string z_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan_2_select.png");
		CCMenuItemImage* z_ccbt=CCMenuItemImage::create(z_bt.c_str(),z_select.c_str(),this,
			menu_selector(Guankamsg::zan_button));
		z_ccbt->setUserData((void*)data.solutionId.c_str());

		CCMenu* pMenu2 = CCMenu::create(z_ccbt,NULL);
		pMenu2->setTouchPriority(kCCMenuHandlerPriority-1);
		pMenu2->setPosition(ccp(recordBG->getContentSize().width - z_ccbt->getContentSize().width/2 + 10, - z_ccbt->getContentSize().height/2 + 10));
		recordBG->addChild(pMenu2, 1);

		//点赞次数
		string num_st=CCString::createWithFormat("%d",data.upTimes)->getCString();
		CCLabelTTF* z_label=CCLabelTTF::create(num_st.c_str(),fontStr_KlavikaBold,30);
		z_label->setPosition(pMenu2->getPosition() - CCPoint(z_label->getContentSize().width/2 + z_ccbt->getContentSize().width/2 - 5, 0));
		recordBG->addChild(z_label, 1, Zannum_tag);

}





void Guankamsg::zan_button(CCObject* obj){

	if(!isclickZ){
		isclickZ=true;
		CCMenuItemImage* p=(CCMenuItemImage*)obj;
		string solutionid=data_play.solutionId;
		CmdHelper::getInstance()->praiseSolution(solutionid,true, true);

	}	 
}





void Guankamsg::clickisz(CCObject* obj){

	isclickZ=false;

}


void Guankamsg::layerinit(){

	

	gkmsg_layer=CCLayer::create();
	gkmsg_layer->setContentSize(size);
	addChild(gkmsg_layer);
	


    //GQ_shu_bg.png center
	string center_str=ResManager::getManager()->getFilePath(g_gkjies+"GQ_shu_bg.png");
	center_sp=CCSprite::create(center_str.c_str());
	center_sp->setOpacity(0.0f);

	center_layer=CCLayer::create();
	center_layer->ignoreAnchorPointForPosition(false);
	center_layer->setAnchorPoint(CCPoint(0.5f,0.5f));
	center_layer->setContentSize(center_sp->getContentSize());
	center_layer->setPosition(CCPoint(size.width*0.55f,size.height*0.65f));
	center_point=CCPoint(size.width*0.55f,size.height*0.65f);
	centerstart_point=CCPoint(size.width+1024,center_layer->getPositionY());

	center_layer->setPosition(centerstart_point);
	gkmsg_layer->addChild(center_layer,1025);

	gkmsg_layer->addChild(center_sp,999);
	center_sp->setPosition(center_point);
	




	string Gk=CCString::createWithFormat("sd%d",currnt_season)->getCString();
	string gk_ms=CCString::createWithFormat(" %d-%d",currnt_season,currnt_gk)->getCString();
	string msg_text=LFStrings::getValue(Gk)+gk_ms;
	gk_season = CCLabelTTF::create(msg_text.c_str(),fontStr_KlavikaBold,50);
	gk_season->setPosition(CCPoint(center_layer->getContentSize().width/2,center_layer->getContentSize().height/2));
	center_layer->addChild(gk_season);
	//left layer
	float duideng=0.18f;
	//JS_bg_xinxilan.png


	CCSprite* left_sp=CCSprite::create(ResManager::getManager()->getFilePath(g_gkjies+"JS_bg_xinxilan.png").c_str());
	left_layer=CCLayer::create();
	left_layer->ignoreAnchorPointForPosition(false);
	left_layer->setAnchorPoint(CCPoint(0.5f,0.5f));
	left_layer->setContentSize(left_sp->getContentSize());
	left_layer->setPosition(CCPoint(left_sp->getContentSize().width/2,size.height/2));
	left_point=CCPoint(left_sp->getContentSize().width/2,size.height/2);
	leftstart_point=CCPoint(-1024,size.height*0.5f);
	left_layer->setPosition(left_point);
	gkmsg_layer->addChild(left_layer);
	left_sp->setPosition(CCPoint(left_sp->getContentSize().width/2,left_layer->getContentSize().height/2));
	left_layer->addChild(left_sp);
	/*
	leftcontent_layer=CCLayer::create();
	leftcontent_layer->ignoreAnchorPointForPosition(false);
	leftcontent_layer->setAnchorPoint(CCPoint(0.5f,0.5f));
	leftcontent_layer->setContentSize(left_sp->getContentSize());
	leftcontent_layer->setPosition(leftstart_point);
	addChild(leftcontent_layer);
	*/
	list=Baselisttable::create();
	list->setMtabletype(Xiaogkfriendlist,CCSize(220,150),3,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,0);
	list->ignoreAnchorPointForPosition(false);
	list->setAnchorPoint(CCPoint(0.5f,0.5f));
	list->setPosition(CCPoint(left_layer->getContentSize().width/2,left_layer->getContentSize().height*0.55f));
	left_layer->addChild(list, 0, list_tag);

	//left buton
	string start_strl=ResManager::getManager()->getFilePath(g_gkjies+"GQ_anniu_facebook.png");
	CCMenuItemImage*  startmenu_l=CCMenuItemImage::create(start_strl.c_str(),start_strl.c_str(),this,menu_selector(Guankamsg::allCallback));
	startmenu_l->setTag(LianjieFb_tag);
	fillOuty(startmenu_l,start_strl);

	startmenu_l->setPosition(CCPoint(left_layer->getContentSize().width/2,left_layer->getContentSize().height*0.145f));
	CCMenu* menul=CCMenu::create(startmenu_l,nullptr);
	menul->setTouchPriority(kCCMenuHandlerPriority-1);
	menul->setPosition(CCPointZero);
	left_layer->addChild(menul);

	/*
	left_layerm=CCLayer::create();
	left_layerm->setContentSize(startmenu_l->getContentSize());
	left_layerm->ignoreAnchorPointForPosition(false);
	left_layerm->setAnchorPoint(CCPoint(0.5f,0.5f));
	left_layerm->setPosition(CCPoint(startmenu_l->getContentSize().width/2-15,size.height*duideng));
	left_layermpoint=CCPoint(startmenu_l->getContentSize().width/2-15,size.height*duideng);
	leftstart_layermpoint=CCPoint(-(startmenu_l->getContentSize().width/2-15),size.height*duideng);
	left_layerm->setPosition(leftstart_layermpoint);
	addChild(left_layerm);
	startmenu_l->setPosition(CCPoint(left_layerm->getContentSize().width/2,left_layerm->getContentSize().height/2));
	CCMenu* menul=CCMenu::create(startmenu_l,nullptr);
	menul->setPosition(CCPointZero);
	menul->setTouchPriority(-129);
	left_layerm->addChild(menul);
	*/
     //right butoon
	string start_str=ResManager::getManager()->getFilePath(g_gkjies+"GQ_anniu_start.png");
	CCMenuItemImage* startmenu=CCMenuItemImage::create(start_str.c_str(),start_str.c_str(),this,menu_selector(Guankamsg::allCallback));
	startmenu->setTag(Start_gametag);
	fillOuty(startmenu,start_str);
	right_layer=CCLayer::create();
	right_layer->setContentSize(startmenu->getContentSize());
	right_layer->ignoreAnchorPointForPosition(false);
	right_layer->setAnchorPoint(CCPoint(0.5f,0.5f));
    right_point=CCPoint(size.width-right_layer->getContentSize().width/2+20,size.height*duideng);
	right_layer->setPosition(right_point);
	rightstart_point=CCPoint(size.width+1024,size.height*duideng);
	right_layer->setPosition(rightstart_point);
    gkmsg_layer->addChild(right_layer);
	startmenu->setPosition(CCPoint(right_layer->getContentSize().width/2,right_layer->getContentSize().height/2));
    
	CCMenu* menur=CCMenu::create(startmenu,nullptr);
	menur->setTouchPriority(kCCMenuHandlerPriority-1);
	menur->setPosition(CCPointZero);
	menur->setTouchPriority(-129);
	right_layer->addChild(menur);

}

void Guankamsg::actionupdate(float time){
	if(num_time==0){
	   color->runAction(CCEaseExponentialOut::create(CCFadeTo::create(0.0f,num_Fade*0.8f)));
//	}else if(num_time==25){
		center_sp->runAction(CCEaseExponentialOut::create(CCFadeIn::create(0.0f)));
       center_layer->runAction(
			  CCEaseExponentialOut::create(CCMoveTo::create(0.0f,center_point)));
		Fadeinnodes(center_layer,0.0f);
		left_layer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.0f,left_point)));
		Fadeinnodes(left_layer,0.0f);
		right_layer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.0f,right_point)));
		Fadeinnodes(right_layer,0.0f);
	}
	num_time=num_time+1;

}

void Guankamsg::centeraction(){

     center_layer->runAction(CCSequence::create(
		           CCEaseExponentialOut::create(CCMoveTo::create(center_time,center_point)),
				   CCCallFunc::create(this,callfunc_selector(Guankamsg::leftaction)),
				   nullptr
			   )
		 );
	 Fadeinnodes(center_layer,centeraction_tag);
	 //sps.at(current_sensonID)->runAction(CCSequence::create(CCFadeOut::create(time),CCCallFunc::create(this,callfunc_selector(Bigcheckpoint::bianAn)),NULL));
}


void Guankamsg::leftaction(){

	left_layer->runAction(CCSequence::create(
		CCEaseExponentialOut::create(CCMoveTo::create(left_time,left_point)),
		CCCallFunc::create(this,callfunc_selector(Guankamsg::endaction)),
		nullptr
		)
		);
	Fadeinnodes(left_layer,leftaction_tag);
}


void Guankamsg::endaction(){
     
	 right_layer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(rght_time,right_point)));
	 Fadeinnodes(right_layer,rightaction_tag);

	 left_layerm->runAction(CCEaseExponentialOut::create(CCMoveTo::create(left_time,left_layermpoint)));
	 Fadeinnodes(left_layerm,leftaction_tag);

}

//反向运动

void Guankamsg::Fadeinnodes(CCNode* node,float time){

	CCArray* ay=node->getChildren();
	if(ay){

		for(size_t i=0;i<ay->count();i++){
			CCNode* p=(CCNode*)ay->objectAtIndex(i);
			p->runAction(CCEaseExponentialOut::create(CCFadeIn::create(time)));
			if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
				Fadeinnodes(p,time);
			}
		}

	}
}


void Guankamsg::Fadeinnodes(CCNode* node,int tag){


	
	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
            if(tag==centeraction_tag){
			   node->runAction(CCEaseExponentialOut::create(CCFadeIn::create(center_time)));
			}else if(tag==leftaction_tag){
				 node->runAction(CCEaseExponentialOut::create(CCFadeIn::create(left_time)));
			}else if(tag==rightaction_tag){
				 node->runAction(CCEaseExponentialOut::create(CCFadeIn::create(rght_time)));
			}
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Fadeinnodes(node,tag);
			}
		}
	}
}

void Guankamsg::Fadeoutnodes(CCNode* node,int tag){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			if(tag==centeraction_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeOut::create(center_time)));
			}else if(tag==leftaction_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeOut::create(left_time)));
			}else if(tag==rightaction_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeOut::create(rght_time)));
			}
			if(node->getChildren()!=nullptr&&node->getChildren()->count()>0){
				Fadeoutnodes(node,tag);
			}
		}
	}
}




void Guankamsg::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,getZOrder(),true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Guankamsg::clickisz),MSG_GAME_CLICKIS_Z, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Guankamsg::solutionplay),MSG_FRINDGKMSG_PLAY, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Guankamsg::refreshSolution),NetMessage::MSG_DownloadSolution, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Guankamsg::click_zan), NetMessage::MSG_PraiseSolution, NULL);
}

void Guankamsg::onExit(){

	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Guankamsg::refreshSolution(CCObject* object){

	Shieldingmanger::getSielding()->setcontinuousThetimermark(true);

	MsgData* data=(MsgData*)object;
	if(freind_solution&&data->errCode==MsgData::eSuccess){
		solutionplay(freind_solution);
	}else{
		MessageBar::show(LFStrings::getValue("nofriendsolution").c_str());
		//CCMessageBox(LFStrings::getValue("nofriendsolution").c_str(), "Error");
	}
}

void Guankamsg::solutionplay(CCObject* obj){
	 
	 freind_solution=obj;
	 Myobject* ob=(Myobject*)obj;
	 int idx=ob->current_id;
	 result_idx=idx;
	 CCLOG("--------------------------------->hehe");
	 //连续点击事件屏蔽
	 if(Shieldingmanger::getSielding()->continuousThetimer()==false){
		 return;
	 }
	 if(idx>=dataUnion.size()){
		 Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
		 return;
	 }
	 //string datastring = SocialInfoManager::getInstance()->getSolutionData(dataUnion.at(idx).solutionId);
	 string datastring = SocialInfoManager::getInstance()->getSolutionData(currnt_season, currnt_gk, dataUnion.at(idx).userId);
	 if (datastring.empty())
	 {
		 //CCMessageBox("local is not found data!", "Error");
		 // NetMessage::MSG_DownloadSolution  succese  string datastring = SocialInfoManager::getInstance()->getSolutionData(ob->data.solutionId);
		 CmdHelper::getInstance()->downloadTaskSolution(dataUnion.at(idx).solutionId, true); 
		 return;
	 }
	 string curUserId;
	 bool hasAcount = CmdHelper::getInstance()->getCurrentUserID(curUserId);
	 // 是查看自己，且本地没有数据才直接弹框返回
	 if (!Recorder::getInstance()->getRecordFromString(datastring, &m_allRecordData) && hasAcount && dataUnion.at(idx).userId == curUserId)
	 {
		 MessageBar::show(LFStrings::getValue("NODataFound").c_str());
		 //CCMessageBox("NO Data Found","Warning");
		 Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
		 return;
	 }
	 drawfriendlayer(dataUnion.at(idx));
	 Fadeoutnodes(gkmsg_layer);
	 gkmsg_layer->runAction(
		 CCSequence::create(
		 CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
		 CCCallFunc::create(this,callfunc_selector(Guankamsg::actionFaihui)),
		 NULL
		 )
		 );	  


}

void Guankamsg::actionFaihui(){
	    
	   
		gkmsg_layer->setVisible(false);
		CCLayer* friend_solutionlayer=(CCLayer*)this->getChildByTag(friendlayertag);
		if(friend_solutionlayer){
		  friend_solutionlayer->setVisible(true);
		  Fadeinnodes(friend_solutionlayer);
		  friend_solutionlayer->runAction(
			  CCSequence::create(
			  CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
			  NULL
			  )
			  );
		}
		Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
}


void Guankamsg::fillOuty(CCMenuItemImage* p,string imgfile){

	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.10f);

	p->setSelectedImage(s);
	CCSprite* n=CCSprite::create(imgfile.c_str());
	CCSprite* layer=CCSprite::create();
	layer->setContentSize(CCSize(p->getContentSize()*1.10f));
	n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	layer->addChild(n);
	p->setNormalImage(layer);
    

	if(p->getTag()==Start_gametag){
		string st_str=LFStrings::getValue("start");
		label_startN=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		label_startN->setPosition(CCPoint(layer->getContentSize().width/2-40,layer->getContentSize().height/2));
		label_startN->setColor(ccc3(3,79,132));
		s->addChild(label_startN);
        
		label_startS=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		label_startS->setPosition(CCPoint(n->getPositionX()-30,n->getPositionY()));
		label_startS->setColor(ccc3(3,79,132));
		layer->addChild(label_startS);
	}
}
void Guankamsg::allCallback(CCObject* ob){

	Music::sharedMusicPlayer()->buttonClicked();
	CCMenuItemImage* p=(CCMenuItemImage*)ob;
	int tag=p->getTag();
	/*
	LianjieFb_tag=100,
	Start_gametag=101,
	*/
	if(tag==LianjieFb_tag){
         
	}else if(tag==Start_gametag){
                
		if (CCNode* p = BaseScense::getCurrentScene()->getChildByTag(this->getTag() + 100))
		{
			p->removeFromParent();
		}
		this->removeFromParent();
		BaseContentLayer::shareBaseContentLayer()->reallyStart(NULL);

	}else if(tag==Dingwei_tag){
		list->dinweiMyself(15);
	}else if(tag==close_friendsolutiontag){

		//gkmsg_layer->setVisible(false);
		//friend_solutionlayer->setVisible(true);
		CCLOG("moveqian------------------------------>%d",this->getChildren()->count());
		CCLayer* friend_solutionlayer=(CCLayer*)this->getChildByTag(friendlayertag);
		Fadeoutnodes(friend_solutionlayer);
		friend_solutionlayer->runAction(
			CCSequence::create(
			CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			CCCallFuncN::create(this,callfuncN_selector(Guankamsg::actionFaihuimain)),
			NULL
			)
			);

	}
}

void Guankamsg::actionFaihuimain(CCNode* node){
	
	 list->getTaleView()->setTouchEnabled(true);
	 this->removeChildByTag(friendlayertag);
CCLOG("movehou------------------------------>%d",this->getChildren()->count());
	 is_solution=false;
	 m_bIsPlayRecord = false;
	 isclickZ=true;
	 gkmsg_layer->setVisible(true);
	 Fadeinnodes(gkmsg_layer);
	 gkmsg_layer->runAction(
		   CCSequence::create(
		   CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
		   NULL
		 )
		 );
}
void Guankamsg::Fadeoutnodes(CCNode* node){
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

void Guankamsg::Fadeinnodes(CCNode* node){
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

void Guankamsg::click_zan(CCObject* object){

	MsgData* data=(MsgData*)object;
	if(data->errCode == MsgData::eSuccess){
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
			if(result_idx<dataUnion.size()){
			   string z_num=CCString::createWithFormat("%d",dataUnion.at(result_idx).upTimes)->getCString();
			   CCLabelTTF* z_label=(CCLabelTTF*)getChildByTag(friendlayertag)->getChildByTag(tag_recoder_BG)->getChildByTag(Zannum_tag);
			   if (z_label) z_label->setString(z_num.c_str());
			}
		}
	}
	else
	{
		isclickZ = false;
	}
}