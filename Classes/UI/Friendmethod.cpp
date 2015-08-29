#include "Friendmethod.h"

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
#include "BaseScense.h"
#include "Baselisttable.h"
#include "Music.h"
#include "BaseContentLayer.h"
#include "BaseContentWrapLayer.h"
#include "Myobject.h"

#include "MenuLayer.h"
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
#include "Baselisttable.h"
#include "MessageBar.h"


Friendmethod::Friendmethod(void){


}

Friendmethod::~Friendmethod(void){

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool Friendmethod::init(){

	if(!CCLayer::init()){
		return false;
	}
	freind_solution=nullptr;
	is_answer=false;
	mark=0;
	isclickZ=false;
	//默认是好友列变界面 1是播放界面
	who_layer=0;
	mark_sns=false;
	this->setZOrder(1025);
	size=CCDirector::sharedDirector()->getWinSize();
	drawfriendlayer();
	snsgetData();

	return true;
}  

void Friendmethod::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Friendmethod::son_praent), MSG_FRIND_PLAY, NULL);//点击tableview时 播放好友解法
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Friendmethod::refreshSolution),NetMessage::MSG_DownloadSolution, NULL);//下载好友解法回调
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Friendmethod::refreshClickz),MSG_FRIND_CLICKZ, NULL);//点赞过后栓新页面
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Friendmethod::clickisz),MSG_GAME_CLICKIS_Z, NULL);//是否可以点击赞 播放一次才能点击一次

}
void Friendmethod::onExit(){

	CCLayer::onExit();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128,true);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	if(mark_sns==true){
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NetMessage::MSG_GotSeasonSocialInfo); // NetMessage::MSG_GotSeasonSocialInfo, NULL);
	}
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	
}

void Friendmethod::clickisz(CCObject* obj){
     isclickZ=true;
}

void Friendmethod::refreshClickz(CCObject* object){
	 
	 char* num_c=(char*)object;
	 z_label->setString(num_c);

}
void Friendmethod::refreshSolution(CCObject* object){

	if(freind_solution){
		son_praent(freind_solution);

		if(freind_solution){
		   Myobject* ob=(Myobject*)freind_solution; 
		   list_friend->refresheplay_num(ob->data.solutionId);
		}
	}
}

bool Friendmethod::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	if(mark==1) return true;
	if(who_layer==0){


		CCPoint p=pTouch->getLocation();
		bool x_m=(p.x<=size.width/2+back->getContentSize().width/2&&p.x>=size.width/2-back->getContentSize().width/2);
		bool y_m=(p.y<=size.height/2+back->getContentSize().height/2&&p.y>=size.height/2-back->getContentSize().height/2);
		if(x_m==false||y_m==false){
			mark=1;

			Fadeoutnodes(back);
			back->runAction(
				CCSequence::create(
				CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
				CCCallFunc::create(this,callfunc_selector(Friendmethod::actionFaihui)),
				NULL
				)
				);
		}
	}else if(who_layer==1){

		CCPoint p=pTouch->getLocation();
		bool x_m=(p.x<=size.width/2+back->getContentSize().width/2&&p.x>=size.width/2-back->getContentSize().width/2);
		bool y_m=(p.y<=size.height/2+back->getContentSize().height/2&&p.y>=size.height/2-back->getContentSize().height/2);
		if(x_m==false||y_m==false){

			answer->removeChildByTag(Tag_Layer_PlayRecord);
			mark=1;
			Fadeoutnodes(answer);
			answer->runAction(
				CCSequence::create(
				CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
				CCCallFunc::create(this,callfunc_selector(Friendmethod::playFh)),
				NULL
				)
				);
		}
	}
	return true;
}


void Friendmethod::drawfriendlayer(){



	string biao_t1=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang.png"); 
	CCSprite* tmp1 = CCSprite::create(biao_t1.c_str());
	CCSize size1 = tmp1->getContentSize();
	//创建原始的图像对应大小矩形区域。
	CCRect fullRect1 = CCRectMake(0,0, size1.width, size1.height);
	CCRect insetRect1 = CCRectMake(size1.width/2-1,size1.height*0.8f, 2, 2);

	//740 360
	string back_file="ZT_tanchukuang.png";
	back_file=ResManager::getManager()->getFilePath(g_stop+back_file);
	back=CCScale9Sprite::create(back_file.c_str(),fullRect1,insetRect1);
	back->setContentSize(CCSize(740,450));
	back->setPosition(CCPoint(size.width/2,size.height/2));
	addChild(back);
	back->setScale(chaonengdj_scale);

	//friendanswer
	string bt_text=LFStrings::getValue("friendanswer");
	CCLabelTTF* bt_label = CCLabelTTF::create(bt_text.c_str(),fontStr_KlavikaBold,40);
	bt_label->setColor(ccc3(138,207,255));
	//bt_label->setPosition(CCPoint(bt_scale->getContentSize().width/2,bt_scale->getContentSize().height/2));


	string biao_t=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_biaoti.png"); 
	CCSprite* tmp = CCSprite::create(biao_t.c_str());
	CCSize size = tmp->getContentSize();
	//创建原始的图像对应大小矩形区域。
	CCRect fullRect = CCRectMake(0,0, size.width, size.height);

	CCRect insetRect = CCRectMake(size.width/2-1, size.height/2,1, 1);


	//标题 270 61

	CCScale9Sprite* bt_scale=CCScale9Sprite::create(insetRect, biao_t.c_str());
	bt_scale->setContentSize(CCSize(50+bt_label->getContentSize().width,fullRect.size.height));
	bt_scale->setPosition(CCPoint(bt_scale->getContentSize().width/2-10,back->getContentSize().height-5-bt_scale->getContentSize().height/2));
	back->addChild(bt_scale);

	bt_label->setPosition(CCPoint(bt_scale->getContentSize().width/2,bt_scale->getContentSize().height/2));
	bt_scale->addChild(bt_label);

	//好友列表选折 Baselisttable*
	list_friend=Baselisttable::create();
	list_friend->setMtabletype(Friendlist,CCSize(650,83),3,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,20);
	list_friend->ignoreAnchorPointForPosition(false);
	list_friend->setAnchorPoint(CCPoint(0.5f,0.5f));
	list_friend->setPosition(CCPoint(back->getContentSize().width/2,back->getContentSize().height*0.50));

	back->addChild(list_friend,1025);
	back->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,1.0f,1.0f),NULL
		)
		);
	Fadeinnodes(back);

}


void Friendmethod::drawPlaylayer(msgSnsdata data){

	if(is_answer==false){

		is_answer=true;
		answer=CCLayer::create();
		answer->setContentSize(size);
		answer->setVisible(false);
		addChild(answer);
		//头像 名字 被点次数
		//图标

		string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
		toux_sp=urlSprite::create(tb_file.c_str(),data.pictrue.c_str());
		answer->addChild(toux_sp);
        
		string name=data.name;
		name_label = CCLabelTTF::create(name.c_str(),fontStr_KlavikaBold,30);
		answer->addChild(name_label);


		float x=(size.width-(toux_sp->getContentSize().width+name_label->getContentSize().width+20))/2;
		toux_sp->setPosition(CCPoint(x+toux_sp->boundingBox().size.width/2, getContentSize().height*0.15f));
		name_label->setPosition(CCPoint(toux_sp->getPositionX()+toux_sp->getContentSize().width/2+20+name_label->getContentSize().width/2,toux_sp->getPositionY()));



		string z_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan.png");
		string z_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan_select.png");
		z_ccbt=CCMenuItemImage::create(z_bt.c_str(),z_select.c_str(),this,
			menu_selector(Friendmethod::zan_button));
		z_ccbt->setUserData((void*)data.solutionId.c_str());

		string num_st=CCString::createWithFormat("%d",data.upTimes)->getCString();
		z_label=CCLabelTTF::create(num_st.c_str(),fontStr_KlavikaBold,30);
		float z_x=(size.width-(z_ccbt->getContentSize().width+z_label->getContentSize().width+20))/2;
		z_ccbt->setPosition(CCPoint(z_x+z_ccbt->getContentSize().width/2,back->getContentSize().height*0.15));
		z_label->setPosition(CCPoint(z_ccbt->getPositionX()+z_ccbt->getContentSize().width/2+20+z_label->getContentSize().width/2,z_ccbt->getPositionY()));
		answer->addChild(z_label);


		CCMenu* pMenu = CCMenu::create(z_ccbt,NULL);
		pMenu->setPosition(CCPointZero);
		answer->addChild(pMenu, 1);
		//
	}else{


		string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
		toux_sp->init(tb_file.c_str(),data.pictrue.c_str());
		name_label->setString(data.name.c_str());
		z_label->setString(CCString::createWithFormat("%d",data.upTimes)->getCString());
		z_ccbt->setUserData((void*)data.userId.c_str());

	}
	answer->setScale(chaonengdj_scale);
	CCScrollView* pPlayRecordLayer = GameScenseDefine::SharedGameScenseDefine()->playRecord(data.seasonId, data.sectionId, &m_allRecordData, false);
	pPlayRecordLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height - pPlayRecordLayer->getContentSize().height/2*pPlayRecordLayer->getScale() - 50));
	answer->addChild(pPlayRecordLayer, 1, Tag_Layer_PlayRecord);

	list_friend->getTaleView()->setTouchEnabled(false);

	if (m_allRecordData.size() <= 0)
	{
		CCLOG("%s----------->m_allRecordData size is 0 !!!!", __FUNCTION__);

		CCLabelTTF* pLabel = CCLabelTTF::create(LFStrings::getValue("nofriendsolution").c_str(), fontStr_KlavikaBold, 40);
		pLabel->setScale(1.5f);
		pPlayRecordLayer->addChild(pLabel, 1);
		pLabel->setPosition(ccp(pPlayRecordLayer->getViewSize().width/2 - pLabel->getContentSize().width*pLabel->getScale()/2, pPlayRecordLayer->getViewSize().height/2 + 200));
	}
}

void Friendmethod::playFh(){
	mark=0;
	who_layer=0;
	answer->setVisible(false);
	list_friend->getTaleView()->setTouchEnabled(true);
	back->setVisible(true);
	Fadeinnodes(back);
	back->runAction(
		CCSequence::create(
		CCScaleTo::create(time_action,1.0f,1.0f),
		NULL
		)
		);
}

void Friendmethod::actionFaihui(){
	mark=0;
	if(who_layer==0){
		int tag=Gameother_tag;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stopright,(CCObject*)tag);
		this->removeFromParent();
	}else if(who_layer==1){
		back->setVisible(false);
		list_friend->getTaleView()->setTouchEnabled(false);
		answer->setVisible(true);
		Fadeinnodes(answer);
		answer->runAction(
			CCSequence::create(
			CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
			NULL
			)
			);
	}
}

void Friendmethod::zan_button(CCObject* obj){

	 if(isclickZ==true){

		 isclickZ=false;
		 CCMenuItemImage* p=(CCMenuItemImage*)obj;
		 Myobject* ob=(Myobject*)freind_solution; 
		 list_friend->clickZ_idx(ob->data.solutionId);
	     
	 }
}


void Friendmethod::son_praent(CCObject *object){

	if(who_layer==1){
		return;
	}
    
	Myobject* ob=(Myobject*)object; 
	//string datastring = SocialInfoManager::getInstance()->getSolutionData(ob->data.solutionId);
	string datastring = SocialInfoManager::getInstance()->getSolutionData(ob->data.seasonId, ob->data.sectionId, ob->data.userId);
	freind_solution=object;

	if (datastring.empty())
	{
		//CCMessageBox("local is not found data!", "Error");
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
		return;
	}
	who_layer=1;
	drawPlaylayer(ob->data);

	Fadeoutnodes(back);
	back->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
		CCCallFunc::create(this,callfunc_selector(Friendmethod::actionFaihui)),
		NULL
		)
		);	  
		
}


void Friendmethod::snsgetData(){


	const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	if(info){
		this->info_layer=info;
		shuaixinData();
	}else{
	    mark_sns=true;
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Friendmethod::snsData), NetMessage::MSG_GotSeasonSocialInfo, NULL);
		CmdHelper::getInstance()->getSeasonSocialInfo(LevelManager::getManager()->getLastSeasonId(), false);
	}
}

void Friendmethod::snsData(CCObject* obj){
    
	const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	if(info){
		this->info_layer=info;
		shuaixinData();
	}
}
void Friendmethod::shuaixinData(){
	this->list_friend->friendSolutionlist(info_layer,true);
}


void Friendmethod::Fadeinnodes(CCNode* node){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCFadeIn::create(chaonengdj_time));
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Fadeinnodes(node);
			}
		}
	}
}

void Friendmethod::Fadeoutnodes(CCNode* node){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCFadeOut::create(chaonengdj_time));
			if(node->getChildren()!=nullptr&&node->getChildren()->count()>0){
				Fadeoutnodes(node);
			}
		}
	}
}

//



