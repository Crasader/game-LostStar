#include "Acount.h"
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
#include "Baselisttable.h"
#include "GameLayerFactory.h"
#include "Music.h"
#include "Tanchuk.h"
#include "Guankamsg.h"
#include "Runtime.h"
#include  "net/NetworkWaitingLayer.h"
#include "Recorder.h"
#include "BaseContentLayer.h"
#include "BaseScense.h"
#include "Shieldingmanger.h"
#include "History.h"
#include "MessageBar.h"

enum Acounttrue{

	 layer1_tag=0,
	 layer2_tag,

};

enum Acounttag{

	Dw_tag=100,
	Yaoqing_tag,
	Yaoqing_tag2,
	Fenxiang_tag,
	Zaiwan_tag,
	Xiaogk_tag,
	Xiaoyiguan_tag,
	centeraction_tag,
	leftaction_tag,
	rightaction_tag,
	bitaoTandstar_tag,
	endTime_tag,
	close_friendacountsolutiontag,


};

enum Listnumtag{

	 layer1_list=125,
	 layer2_list=126,
	 layer1_backtag,
};

Acount::Acount(void)
{
}


Acount::~Acount(void)
{
	nodes1.clear();
	nodes2.clear();
	vsps.clear();
	vspsstar.clear();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);


}

void Acount::shuaixinData(){
      
	  vector<TaskSolution> users=info_layer->users;

	  //超越好友的动画
	  if (this->m_playBeyondAnimate){
		  m_playBeyondAnimate = false;
		  string userId;
		  if (CmdHelper::getInstance()->getCurrentUserID(userId))
		  {
			  for (int i=0,count=users.size(); i<count; ++i)
			  {
				  if (users[i].userId == userId){
					  users[i].score = this->acount_score;
					  users[i].star = this->curent_starnum;
					  break;
				  }
			  }
		  }
	  }


	  list->setGkmsglayeruser(users);
	  friendSolutionlist();
	  //list2->yaoqingfriends(allusers);
}

void  Acount::friendSolutionlist(){


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


void Acount::snsgetData(){

	 
	const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	allusers=SocialInfoManager::getInstance()->getFriendsExceptMe();
    
    this->info_layer=info;
	if(info){
		shuaixinData();
	}else{
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Guankamsg::snsData), NetMessage::MSG_GotSeasonSocialInfo, NULL);
	}

}

void Acount::snsData(CCObject* obj){

	const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	if(info){
		this->info_layer=info;
		shuaixinData();
	}

}


bool Acount::init(){

	 if(!CCLayer::init()){
		 return false;
	 }

	 m_playBeyondAnimate = false;

	 button_mark=true;
	 isclickZ=true;
	 isfriendVideolayerinit=false;
	 mark=layer1_tag;
	 num=0;
	 size=CCDirector::sharedDirector()->getWinSize();
	 
	 ccColor4B mr;
	 mr.r=0;mr.g=0;mr.b=0;mr.a=0;
	 color = CCLayerColor::create(mr);
	 color->setContentSize(size);
	 color->ignoreAnchorPointForPosition(false);
	 color->setPosition(CCPoint(size.width/2,size.height/2));
	 color->setZOrder(-10);
	 addChild(color);
     
	 num_time=0;
	 center_time=25.0f/60.0f;// 0.25f;
	 left_time=25.0f/60.0f;
	 left_time = 0;
	 rght_time=1.0f;// 0.25f;
	 btandst_time=25.0f/60.0f;// 0.25f/2;
	 starS_time=25.0f/60.0f;//0.25f;// 0.25f/2;
	 starS_timeMan=50.0f/60.0f;
	 end_time=1.0f;// 0.25f/2;
	 jijiaqi=0;
     
	 all_time=205;
	 back_time=200;
	 bt_time=30;
	 first_actiontime=45;
	 xinxin_time=50;
	 text_time=60;
	 butoon_time=75;
	 dom_buttontime=75;
	 newcorlds_time=120;

	 Music::sharedMusicPlayer()->loadEffectSound("star1.wav");	
	 Music::sharedMusicPlayer()->loadEffectSound("star2.wav");	
	 Music::sharedMusicPlayer()->loadEffectSound("star3.wav");	


	 //savedata();  //由BaseScene::congratulation来保存
	 layerinint();
	 //drawfriendlist();
	 float ever_time=0.01f;
	 this->schedule(schedule_selector(Acount::actionupdate),ever_time);
	 snsgetData();
	 return true;
}


void Acount::savedata(){

	//this->currnt_season=season; this->currnt_gk=gkid;
	//记录本关时间
/*
	GameScore runtime = Runtime::getRuntime()->getGameScore();
	SectionInfor* info = GameInforEditer::getGameInfor()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
	if (info)
	{
		if (!info->isSuccessed)
		{
			info->fFirstCost = runtime.timeCost;
		}
		//保存更优的数据
		if (info->isBetterThanMe(runtime.timeCost, runtime.effectiveClickedNum, runtime.clickStar))
		{

			if (Recorder::getInstance()->setOneSectionRecord(GameLayerFactory::getInstance()->getSectionIndex(
				LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId()), 
				BaseContentLayer::shareBaseContentLayer()->getAllFramesData()))
			{
				info->fTimeCost = runtime.timeCost;
				info->effectiveClickNum = runtime.effectiveClickedNum;
				info->iStarNum = runtime.clickStar;
				info->iRankingScore = SectionInfor::calculateRankScore(runtime.timeCost, runtime.effectiveClickedNum, runtime.clickStar);
				GameInforEditer::getGameInfor()->saveSessonInforList();

				CCLog("record OK");
				vector<std::pair<int,int> > sectionList;
				sectionList.push_back(std::make_pair(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId()));
				CmdHelper::getInstance()->uploadTaskSolution(sectionList);
				NetworkWaitingLayer::hideWaiting();//静默上传
			}
			else
			{
				CCLog("record Error");
			}
		}
	}*/
}

void Acount::layerinint(){

	acount_layer=CCLayer::create();
	acount_layer->setContentSize(size);
	addChild(acount_layer);
	
	
	float duideng=0.18f;
	CCSprite* left_sp=CCSprite::create(ResManager::getManager()->getFilePath(g_gkjies+"JS_bg_xinxilan.png").c_str());
	left_layer=CCLayer::create();
	left_layer->ignoreAnchorPointForPosition(false);
	left_layer->setAnchorPoint(CCPoint(0.5f,0.5f));
	left_layer->setContentSize(left_sp->getContentSize());

	float left_juli=-725;

    leftstart_point=CCPoint(-1024,size.height/2);//(-left_sp->getContentSize().width/2+left_juli,size.height/2);
	leftend_point=CCPoint(left_sp->getContentSize().width/2,size.height/2);
	left_layer->setPosition(leftend_point);
    
	acount_layer->addChild(left_layer);
	left_sp->setPosition(CCPoint(left_sp->getContentSize().width/2,left_layer->getContentSize().height/2));
	left_layer->addChild(left_sp);
	list=Baselisttable::create();
	list->setParentequaltasg(ACountlayer_tag);
	list->setMtabletype(Xiaogkfriendlist,CCSize(220,150),3,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,0);
	list->ignoreAnchorPointForPosition(false);
	list->setAnchorPoint(CCPoint(0.5f,0.5f));
	list->setPosition(CCPoint(left_layer->getContentSize().width/2,left_layer->getContentSize().height*0.55f));
	left_layer->addChild(list);
	//left button


	

	//JS_anniu_yaoqing.png
	string yao_str=ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_yaoqing.png");
	CCMenuItemImage* yq_bt=CCMenuItemImage::create(yao_str.c_str(),yao_str.c_str(),this,menu_selector(Acount::allCallback));
	yq_bt->setTag(Yaoqing_tag);
	fillOuty(yq_bt,yao_str);

	/*
	leftlayer_button=CCLayer::create();
	leftlayer_button->setContentSize(yq_bt->getContentSize());
	leftlayer_button->ignoreAnchorPointForPosition(false);
	leftlayer_button->setAnchorPoint(CCPoint(0.0f,0.0f));
	leftstart_pointb=leftend_pointb=CCPoint(-15.0f,size.height*duideng-leftlayer_button->getContentSize().height/2);
	leftlayer_button->setPosition(leftend_pointb);
	acount_layer->addChild(leftlayer_button);
    yq_bt->setPosition(CCPoint(leftlayer_button->getContentSize().width/2,leftlayer_button->getContentSize().height/2));

    CCMenu* lf_bmenu=CCMenu::create(yq_bt,nullptr);
	lf_bmenu->setPosition(CCPointZero);
	//leftlayer_button->addChild(lf_bmenu)
	*/
	yq_bt->setPosition(CCPoint(yq_bt->getContentSize().width/2-15,left_layer->getContentSize().height*0.145f));
	CCMenu* lf_bmenu=CCMenu::create(yq_bt,nullptr);
	lf_bmenu->setPosition(CCPointZero);
	//leftlayer_button->addChild(lf_bmenu);
	left_layer->addChild(lf_bmenu);
    

 	//centerlayer
	string center_str=ResManager::getManager()->getFilePath(g_gkjies+"JS_bg.png");
	CCSprite* cen_sp=CCSprite::create(center_str.c_str());

	float cent_pointx=left_sp->getContentSize().width+(size.width-left_sp->getContentSize().width)/2;
	float bianju=1024;//(size.width-left_sp->getContentSize().width)/2-cen_sp->getContentSize().width/2+400;
	centerlayer=CCLayer::create();
	centerlayer->setContentSize(cen_sp->getContentSize());
	centerlayer->ignoreAnchorPointForPosition(false);
	centerlayer->setAnchorPoint(CCPoint(0.5f,0.5f));
	center_point=CCPoint(size.width+bianju,size.height/2-30);
	centerend_point=CCPoint(cent_pointx,size.height/2-30);
	centerlayer->setPosition(centerend_point);
	acount_layer->addChild(centerlayer);
	cen_sp->setPosition(CCPoint(centerlayer->getContentSize().width/2,centerlayer->getContentSize().height/2));
	centerlayer->addChild(cen_sp);


	
	//biaoti 第二步
	string bT_str=ResManager::getManager()->getFilePath(g_gkjies+"JS_hengfu.png");
	CCSprite* bt_sp=CCSprite::create(bT_str.c_str());

	layer_bT=CCLayer::create();
	layer_bT->setContentSize(bt_sp->getContentSize());
	layer_bT->ignoreAnchorPointForPosition(false);
	layer_bT->setAnchorPoint(CCPoint(0.5f,0.5f));
	layer_bT->setZOrder(1025);
   // bt_startP=CCPoint(center_point.x,centerend_point.y+centerlayer->getContentSize().height/2+20);//(centerend_point.x,centerend_point.y+centerlayer->getContentSize().height/2+bt_sp->getContentSize().width);
//	bt_endP=CCPoint(centerend_point.x,centerend_point.y+centerlayer->getContentSize().height/2+20);
	layer_bT->setPosition(CCPoint(centerlayer->getContentSize().width/2,centerlayer->getContentSize().height+20));
	//acount_layer->addChild(layer_bT);
	bt_sp->setPosition(CCPoint(layer_bT->getContentSize().width/2,layer_bT->getContentSize().height/2));
	layer_bT->addChild(bt_sp);
	centerlayer->addChild(layer_bT);


	string text=LFStrings::getValue("bengpj");
	CCLabelTTF* shuom = CCLabelTTF::create(text.c_str(),fontStr_KlavikaBold,60);
	shuom->setColor(ccc3(115,51,0));
	shuom->setPosition(CCPoint(bt_sp->getContentSize().width/2,bt_sp->getContentSize().height/2));
	bt_sp->addChild(shuom);

	
	//starlayer
	string star_str=ResManager::getManager()->getFilePath(g_gkjies+"JS_star_kong.png");
	CCSprite* st_sp=CCSprite::create(star_str.c_str());
	layer_anStar=CCLayer::create();
	layer_anStar->setContentSize(CCSize(cen_sp->getContentSize().width,st_sp->getContentSize().height));
	layer_anStar->ignoreAnchorPointForPosition(false);
	layer_anStar->setAnchorPoint(CCPoint(0.5f,0.5f));
	layer_anStar->setPosition(CCPoint(centerlayer->getContentSize().width/2,centerlayer->getContentSize().height*0.75f));
	centerlayer->addChild(layer_anStar);
	

	float star_xs=(layer_anStar->getContentSize().width-(st_sp->getContentSize().width*3+25*2))/2;
	for(size_t i=0;i<3;i++){
		CCSprite* p=CCSprite::create(star_str.c_str());
		p->setOpacity(num_Fade*0.5f);
		p->setPosition(CCPoint(star_xs+p->getContentSize().width/2+i*(p->getContentSize().width+25),layer_anStar->getContentSize().height/2));
		an_P.push_back(p->getPosition());
		layer_anStar->addChild(p);
	}



	//share 
	layer_share=CCLayer::create();
	layer_share->ignoreAnchorPointForPosition(false);
	layer_share->setAnchorPoint(CCPoint(0.5f,0.5f));
	layer_share->setContentSize(CCSize(centerlayer->getContentSize().width,100));
	layer_share->setPosition(CCPoint(centerend_point.x,centerend_point.y-55));
	acount_layer->addChild(layer_share);

	string js_backstr=ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_fenxiang_bg.png");
	CCSprite* js_b=CCSprite::create(js_backstr.c_str());
	js_b->setPosition(CCPoint(layer_share->getContentSize().width/2,layer_share->getContentSize().height/2 - 52));
	layer_share->addChild(js_b);
	js_b->setOpacity(0.0f);

	string share_bustr=ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_fenxiang.png");
	CCMenuItemImage* sh_bt=CCMenuItemImage::create(share_bustr.c_str(),share_bustr.c_str(),this,menu_selector(Acount::allCallback));
	
	layer_shareB=CCLayer::create();
	layer_shareB->setVisible(false);
	
	layer_shareB->ignoreAnchorPointForPosition(false);
	layer_shareB->setAnchorPoint(CCPoint(0.5f,0.5f));
	layer_shareB->setContentSize(sh_bt->getContentSize());
	layer_shareB->setPosition(CCPoint(centerend_point.x,centerend_point.y-55));
	acount_layer->addChild(layer_shareB);
	
	
	sh_bt->setTag(Fenxiang_tag);
	sh_bt->setPosition(CCPoint(layer_shareB->getContentSize().width/2,layer_shareB->getContentSize().height/2 - 52));
	sh_bt->setOpacity(0.0f);

	CCMenu* sh_me=CCMenu::create(sh_bt,nullptr);
	sh_me->setOpacity(0.0f);
	sh_me->setPosition(CCPointZero);
	layer_shareB->addChild(sh_me);
	fillOuty(sh_bt,share_bustr);
	
	Outall(layer_shareB,0.01f);
	//button

	layer_gameB=CCLayer::create();
	layer_gameB->setVisible(false);
	layer_gameB->setScale(0.8f);
	layer_gameB->setContentSize(CCSize(centerlayer->getContentSize().width,215));
	layer_gameB->ignoreAnchorPointForPosition(false);
	layer_gameB->setAnchorPoint(CCPoint(0.5f,0.5f));
	layer_gameB->setPosition(CCPoint(centerend_point.x,centerend_point.y-220));
	acount_layer->addChild(layer_gameB);
	
	
	//JS_anniu_chongzhi.png JS_anniu_guanqiaxuanze.png JS_anniu_xiayiguan.png

	string button_zu[3]={ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_chongzhi.png"),
		ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_guanqiaxuanze.png"),
		ResManager::getManager()->getFilePath(g_gkjies+"JS_anniu_xiayiguan.png")
	};
	//Zaiwan_tag, Xiaogk_tag, Xiaoyiguan_tag,
	CCMenuItemImage* cong_w=CCMenuItemImage::create(button_zu[0].c_str(),button_zu[0].c_str(),this,menu_selector(Acount::allCallback));
	cong_w->setTag(Zaiwan_tag);
	cong_w->setOpacity(0.0f);
	cong_w->setPosition(CCPoint(layer_gameB->getContentSize().width*0.15f,layer_gameB->getContentSize().height/2 - 30));
	fillOuty(cong_w,button_zu[0]);

	CCMenuItemImage* xiao_m=CCMenuItemImage::create(button_zu[1].c_str(),button_zu[1].c_str(),this,menu_selector(Acount::allCallback));
	xiao_m->setTag(Xiaogk_tag);
	xiao_m->setOpacity(0.0f);
	xiao_m->setPosition(CCPoint(layer_gameB->getContentSize().width*0.5f,layer_gameB->getContentSize().height/2 - 30));
	fillOuty(xiao_m,button_zu[1]);

	CCMenuItemImage* xy_g=CCMenuItemImage::create(button_zu[2].c_str(),button_zu[2].c_str(),this,menu_selector(Acount::allCallback));
	xy_g->setTag(Xiaoyiguan_tag);
	xy_g->setOpacity(0.0f);
	xy_g->setPosition(CCPoint(layer_gameB->getContentSize().width*0.85f,layer_gameB->getContentSize().height/2 - 30));
	fillOuty(xy_g,button_zu[2]);

	
	

	CCMenu* game_me=CCMenu::create(cong_w,xiao_m,xy_g,nullptr);
	game_me->setPosition(CCPointZero);
	game_me->setOpacity(0);
	layer_gameB->addChild(game_me);

    Outall(layer_gameB,0.01f);

	if(curent_starnum>history_numstar&&history_numstar>0){
		string tg_str=ResManager::getManager()->getFilePath(g_public+"JS_chengjitigao_bg.png");
		tg_sp=CCSprite::create(tg_str.c_str());

		cjlayer=CCLayer::create();
		cjlayer->setZOrder(100);
		cjlayer->ignoreAnchorPointForPosition(false);
		cjlayer->setAnchorPoint(CCPoint(0.5f,0.5f));
		cjlayer->setContentSize(CCSize(tg_sp->getContentSize()));
		
		cjlayer_startpoint=CCPoint(centerlayer->getContentSize().width/2,centerlayer->getContentSize().height/2+262+70);
		cjlayer_endpoint=CCPoint(centerlayer->getContentSize().width/2,centerlayer->getContentSize().height/2+262);
		cjlayer->setPosition(cjlayer_startpoint);
		centerlayer->addChild(cjlayer);

		tg_sp->setPosition(CCPoint(cjlayer->getContentSize().width/2,cjlayer->getContentSize().height/2));
		cjlayer->addChild(tg_sp);

		string cj_str=ResManager::getManager()->getFilePath(g_public+"JS_icon_chengjitigao.png");
		CCSprite* cj_sp=CCSprite::create(cj_str.c_str());
		cj_sp->setPosition(CCPoint(tg_sp->getContentSize().width*0.15f,tg_sp->getContentSize().height*0.6));
		tg_sp->addChild(cj_sp);
		//chenz
		string cz_str=LFStrings::getValue("chenz");
		CCLabelTTF* cz_label = CCLabelTTF::create(cz_str.c_str(),fontStr_KlavikaBold,30);
		cz_label->setPosition(CCPoint(cj_sp->getPositionX()+cj_sp->getContentSize().width*0.5f+cz_label->getContentSize().width/2+10,tg_sp->getContentSize().height*0.6f));
		tg_sp->addChild(cz_label);
	}
	this->runAction(CCSequence::create(
		CCDelayTime::create(1.0f),
		CCCallFunc::create(this, callfunc_selector(Acount::playBeyondFriendAnimate)),
		nullptr));
    
	string text1=CCString::createWithFormat("%d",acount_score)->getCString();
	score_l=CCLabelTTF::create(text1.c_str(),fontStr_KlavikaBold,40);
	score_l->setPosition(CCPoint(centerend_point.x,centerend_point.y+50));
	acount_layer->addChild(score_l);
	score_l->setScale(0.0f);
	score_l->setOpacity(0);

}

void Acount::actionStart(){

	 left_layer->setVisible(true);
	 left_layer->runAction(CCMoveTo::create(left_time,leftend_point));
	 Fadeinnodes(left_layer,leftaction_tag);

	 centerlayer->runAction(
		     CCSequence::create(
		     CCEaseExponentialOut::create(CCMoveTo::create(center_time,centerend_point)),
			 CCCallFunc::create(this,callfunc_selector(Acount::btandanStar)),
			 nullptr
			 ));
	 Fadeinnodes(centerlayer,centeraction_tag);
}

void Acount::btandanStar(){
     
	
	 layer_bT->runAction(
		 CCSequence::create(

		 CCEaseExponentialOut::create(CCMoveTo::create(btandst_time,bt_endP)),
		 CCEaseExponentialOut::create(CCFadeIn::create(btandst_time)),


		 CCCallFunc::create(this,callfunc_selector(Acount::starSaction)),
		 nullptr
		 ));
	 Fadeinnodes(layer_bT,bitaoTandstar_tag);


	 layer_anStar->setVisible(true);
	 Fadeinnodes(layer_anStar,bitaoTandstar_tag);



}



void Acount::starSaction(){

	  
	 if(curent_starnum>0&&jijiaqi<curent_starnum){

		 string star=ResManager::getManager()->getFilePath(g_gkjies+"JS_star.png");
		 CCSprite* p=CCSprite::create(star.c_str());
		 p->setPosition(an_P.at(jijiaqi));
		 layer_anStar->addChild(p);
		 p->setScale(3.0f);
		 p->setOpacity(0.0f);
		 jijiaqi+=1;

		 p->runAction(
			    CCSequence::create(
			          CCSpawn::create(
					      CCEaseExponentialOut::create(CCFadeIn::create(starS_time)),
						  CCEaseExponentialOut::create(CCScaleTo::create(starS_time,1.0f,1.0f)),
						 
						  nullptr),
				      CCCallFunc::create(this,callfunc_selector(Acount::starSaction)),
				      nullptr
				)
			 );
	 }else{

		 
		 //所有见隐见现
		 layer_share->setVisible(true);
		 layer_share->runAction(
			    CCSequence::create(
				  CCEaseExponentialOut::create(CCScaleTo::create(end_time,1.0f,1.0f)),
				  CCCallFunc::create(this,callfunc_selector(Acount::endAction)),
				  nullptr)
			 );
		 Fadeinnodes(layer_share,endTime_tag);
		 layer_gameB->setVisible(true);
		 Fadeinnodes(layer_gameB,endTime_tag);
		 /*
		 leftlayer_button->runAction(
			   CCEaseExponentialOut::create(CCScaleTo::create(end_time,1.0f,1.0f))
			 );
		 Fadeinnodes(leftlayer_button,endTime_tag);
		 */

		 score_l->setVisible(true);
		 score_l->runAction(
			  CCEaseExponentialOut::create(CCFadeIn::create(end_time))
			 );
			
	 } 
}


void Acount::endAction(){
     
	 if(curent_starnum>history_numstar){
		 cjlayer->setVisible(true);
		 cjlayer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(left_time,cjlayer_endpoint)));
		 Fadeinnodes(cjlayer,leftaction_tag);
	 }
}


void Acount::starSactionMan(){

	 
}

void Acount::starSactionMan(CCNode* pSender, void* data){

	//pSender->setScale(1.0f);
	
	pSender->runAction(
		CCSequence::create(
		CCSpawn::create(
		CCEaseExponentialOut::create(CCFadeIn::create(starS_timeMan)),
		CCEaseExponentialOut::create(CCScaleTo::create(starS_timeMan,1.0f,1.0f)),
	    nullptr),
		nullptr));
		
}

void Acount::actionupdate(float time){

    

	if(num_time==0){
		color->setVisible(false);
		color->runAction(CCEaseExponentialOut::create(CCFadeTo::create(0.0f,num_Fade*0.8f)));
		left_layer->setVisible(true);
		left_layer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(left_time,leftend_point)));
		//Fadeinnodes(left_layer,leftaction_tag);
		centerlayer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(center_time,centerend_point)));
		//Fadeinnodes(centerlayer,centeraction_tag);
		//layer_bT->runAction(CCEaseExponentialOut::create(CCMoveTo::create(center_time,bt_endP)));
		//Fadeinnodes(layer_bT,bitaoTandstar_tag);
		

	}else if(num_time==50){

			if(curent_starnum>=1){
					float star_scale=3.77f;
					string star=ResManager::getManager()->getFilePath(g_gkjies+"JS_star.png");
					CCSprite* p=CCSprite::create(star.c_str());
					starS_time=15.0f/60.0f;
					p->setPosition(an_P.at(0));
					layer_anStar->addChild(p);
					p->setOpacity(0.0f);
					p->setScale(star_scale);
					p->runAction(
						CCSequence::create(
						CCSpawn::create(
						CCFadeIn::create(starS_time),
						CCScaleTo::create(starS_time,1.0f,1.0f),
						//CCEaseExponentialOut::create(CCFadeIn::create(starS_time)),
						//CCEaseExponentialOut::create(CCScaleTo::create(starS_time,1.0f,1.0f)),
						nullptr),
						CCCallFuncN::create(this,callfuncN_selector(Acount::btandanStar2)),
						nullptr
						)
						);
					Music::sharedMusicPlayer()->playEffectSound("star1.wav");	
			}


	}else if(num_time==65){

		if(curent_starnum>=2){
					float star_scale=3.77f;
					string star=ResManager::getManager()->getFilePath(g_gkjies+"JS_star.png");
					CCSprite* p=CCSprite::create(star.c_str());
					starS_time=15.0f/60.0f;
					p->setPosition(an_P.at(1));
					layer_anStar->addChild(p);
					p->setOpacity(0.0f);
					p->setScale(star_scale);
					p->runAction(
						CCSequence::create(
						CCSpawn::create(
						CCFadeIn::create(starS_time),
						CCScaleTo::create(starS_time,1.0f,1.0f),
						//CCEaseExponentialOut::create(CCFadeIn::create(starS_time)),
						//CCEaseExponentialOut::create(CCScaleTo::create(starS_time,1.0f,1.0f)),
						nullptr),
						CCCallFuncN::create(this,callfuncN_selector(Acount::btandanStar2)),
						nullptr
						)
						);
					Music::sharedMusicPlayer()->playEffectSound("star2.wav");	
		
		}

	}else if(num_time==80){

		if(curent_starnum>=3){
			float star_scale=3.77f;
			string star=ResManager::getManager()->getFilePath(g_gkjies+"JS_star.png");
			CCSprite* p=CCSprite::create(star.c_str());
			starS_time=15.0f/60.0f;
			p->setPosition(an_P.at(2));
			layer_anStar->addChild(p);
			p->setOpacity(0.0f);
			p->setScale(star_scale);
			p->runAction(
				CCSequence::create(
				CCSpawn::create(
				CCFadeIn::create(starS_time),
				CCScaleTo::create(starS_time,1.0f,1.0f),
				//CCEaseExponentialOut::create(CCFadeIn::create(starS_time)),
				//CCEaseExponentialOut::create(CCScaleTo::create(starS_time,1.0f,1.0f)),
				nullptr),
				CCCallFuncN::create(this,callfuncN_selector(Acount::btandanStar2)),
				nullptr
				)
				);
			Music::sharedMusicPlayer()->playEffectSound("star3.wav");	
		}
	}else if(num_time==105){

		
		float text_actiontime=20.0f/60.0f;
		score_l->runAction(
			CCSequence::create(
				CCSpawn::create(
					CCEaseExponentialOut::create(CCScaleTo::create(text_actiontime,1.0f,1.0f)),
					CCEaseExponentialOut::create(CCFadeIn::create(text_actiontime)),
					NULL), 
				CCCallFuncN::create(this, callfuncN_selector(Acount::showTime)),
				NULL)
			);

	}else if(num_time==125){

		if(curent_starnum>history_numstar&&history_numstar>0){
			float cj_time=25.0f/60.0f;
			cjlayer->runAction(CCMoveTo::create(cj_time,cjlayer_endpoint));//  CCEaseExponentialOut::create(CCMoveTo::create(cj_time,cjlayer_endpoint)));
			Fadeinnodes(cjlayer,cj_time);
		}
		float gameb_trme=25.0f/60.0f; 
		layer_gameB->setVisible(true);
		Fadeinnodes(layer_gameB,gameb_trme);

		layer_gameB->runAction(
			CCSpawn::create(
			CCEaseExponentialOut::create(CCScaleTo::create(gameb_trme,1.0f,1.0f)),
			nullptr
			) 
			);
		layer_shareB->setVisible(true);
		Fadeinnodes(layer_shareB,gameb_trme);

		float buton_actiontime=25.0f/60.0f;
		Fadeinnodes(layer_share,buton_actiontime);
	}else if(num_time==xinxin_time){

		  /*
		  float star_scale=2.77f;
          layer_anStar->setVisible(true);
		  Fadeinnodes(layer_anStar,starS_time);

		  float first_scale=0.5f;

		  for(int i=0;i<curent_starnum;i++){
			  string star=ResManager::getManager()->getFilePath(g_gkjies+"JS_star.png");
			  CCSprite* p=CCSprite::create(star.c_str());
			  p->setPosition(an_P.at(i));
			  layer_anStar->addChild(p);
              p->setOpacity(0.0f);
			  p->setScale(star_scale);
			  p->runAction(
				  CCSequence::create(
				  CCSpawn::create(
				  //CCFadeIn::create(starS_time),
				  //CCScaleTo::create(starS_time,1.0f,1.0f),
				  CCEaseExponentialOut::create(CCFadeIn::create(starS_time)),
				  CCEaseExponentialOut::create(CCScaleTo::create(starS_time,1.0f,1.0f)),
				  //CCEaseExponentialOut::create(CCFadeTo::create(starS_time,num_Fade*first_scale)),
				  //CCEaseExponentialOut::create(CCScaleTo::create(starS_time,star_scale*first_scale,star_scale*first_scale)),
				  nullptr),
				  //CCCallFuncND::create(this, callfuncND_selector(Acount::starSactionMan),(void*)"1"),
				  nullptr
				  )
				  );
		  }

		  float buton_actiontime=25.0f/60.0f;
		  Fadeinnodes(layer_share,buton_actiontime);
		  */
	 }else if(num_time==text_time){

		 /*
		 float text_actiontime=15.0f/60.0f;
		 score_l->runAction(
			    CCSpawn::create(
				CCEaseExponentialOut::create(CCScaleTo::create(text_actiontime,1.0f,1.0f)),
			    CCEaseExponentialOut::create(CCFadeIn::create(text_actiontime)),
				nullptr
				)
			 );
			 */
		


	 }else if(num_time==butoon_time){

		
		/*
		 float gameb_trme=25.0f/60.0f; 
		  layer_gameB->setVisible(true);
		 Fadeinnodes(layer_gameB,gameb_trme);

		 layer_gameB->runAction(
			 CCSpawn::create(
			 CCEaseExponentialOut::create(CCScaleTo::create(gameb_trme,1.0f,1.0f)),
			 nullptr
			 ) 
			 );
		 layer_shareB->setVisible(true);
		 Fadeinnodes(layer_shareB,gameb_trme);
		 

		 if(curent_starnum>history_numstar){
			 float cj_time=25.0f/60.0f;
			 cjlayer->setVisible(true);
			 cjlayer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(cj_time,cjlayer_endpoint)));
			 Fadeinnodes(cjlayer,cj_time);
		 }
		 */

	 }else if(num_time==dom_buttontime){
	 /*
		 float gameb_trme=0.25f; 
		 Fadeinnodes(layer_gameB,gameb_trme);
		 layer_gameB->runAction(
			 CCSpawn::create(
			   CCEaseExponentialOut::create(CCScaleTo::create(gameb_trme,1.0f,1.0f)),
			   CCEaseExponentialOut::create(CCFadeIn::create(gameb_trme)),
			   nullptr
			 ) 
			 );
			 */
	 }else if(num_time==newcorlds_time){
		 /*
		 if(curent_starnum>history_numstar){
			 float cj_time=0.25f;
			  cjlayer->setVisible(true);
			  cjlayer->runAction(CCEaseExponentialOut::create(CCMoveTo::create(cj_time,cjlayer_endpoint)));
			  Fadeinnodes(cjlayer,cj_time);
		 }
         */
	 }else if(num_time>=all_time){
		 this->unschedule(schedule_selector(Acount::actionupdate));
	 }
	 num_time=num_time+1;
	
}

void Acount::showTime(CCNode* pNode)
{
	CCSprite* pClock = CCSprite::create(ResManager::getManager()->getFilePath(g_gkjies+"JS_icon_shijian.png").c_str());
	pNode->getParent()->addChild(pClock);

	GameScore score = Runtime::getRuntime()->getGameScore();
	int min = 59;
	int sed = 59;
	if (score.timeCost < 3600.0f)
	{
		min = score.timeCost/60.0f;
		sed = (int)score.timeCost%60; 
	}

	CCLabelTTF* pTimeLabel = CCLabelTTF::create(CCString::createWithFormat("%02d:%02d", min, sed)->getCString(), fontStr_KlavikaBold, 36);
	pTimeLabel->setAnchorPoint(ccp(0.0f, 0.5f));
	pClock->addChild(pTimeLabel);
	pTimeLabel->setPosition(ccp(pClock->getContentSize().width + 3, pClock->getContentSize().height/2));

	pClock->setPosition(ccp(pNode->getPositionX() - pTimeLabel->getContentSize().width/2 - 2.0f, pNode->getPositionY() - pNode->getContentSize().height/2 - pClock->getContentSize().height/2 - 10));

}

void Acount::btandanStar2(CCNode* node){

	float time=5.0f/60.0f;
	node->runAction(CCScaleTo::create(time,1.2f,1.2f));
}


void Acount::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128,true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Acount::freindvideoPlay),MSG_FRINDGKMSG_ACOUNTPLAY, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Acount::beyondFriendAnimateEnd_refreshData),MSG_FRIND_BEYOND_ACOUNT_REFRESH, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Acount::refreshSolution),NetMessage::MSG_DownloadSolution, NULL);

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Acount::click_zan), NetMessage::MSG_PraiseSolution, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Acount::clickisz),MSG_GAME_CLICKIS_Z, NULL);

}

void Acount::onExit(){
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Acount::clickisz(CCObject* obj){

	isclickZ=false;

}

void Acount::refreshSolution(CCObject* object){

	Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
	MsgData* data=(MsgData*)object;
	if(freind_solution&&data->errCode==MsgData::eSuccess){
		freindvideoPlay(freind_solution);
	}else{
		MessageBar::show(LFStrings::getValue("nofriendsolution").c_str());
		//CCMessageBox(LFStrings::getValue("nofriendsolution").c_str(), "Error");
	}
}

void Acount::click_zan(CCObject* object){

	MsgData* data=(MsgData*)object;
	if(data->errCode == MsgData::eSuccess&&isfriendVideolayerinit==true){
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
			if(freind_idx<dataUnion.size()){
				string z_num=CCString::createWithFormat("%d",dataUnion.at(freind_idx).upTimes)->getCString();
				z_label->setString(z_num.c_str());
			}
		}
	}
	else
	{
		isclickZ = false;
	}
}


Acount::Acount(int current_score,int curent_star, int history_score,int history_star)
{
	this->acount_score=current_score;
	this->curent_starnum=curent_star;
	this->history_score=history_score;
	this->history_numstar=history_star;
}

Acount* Acount::createByid(int current_score,int curent_star, int history_score,int history_star)
{

	Acount* pRet=new Acount(current_score,curent_star,history_score, history_star);
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}
}

void Acount::setTruelayer(){

	 
	 if(mark==layer1_tag){
		 if(num==0){
			 layer1->setVisible(true);
			 layer2->setVisible(false);
		 }else{
			 actionFp();
		 }
     }else if(mark==layer2_tag){
		 if(num==0){
			 layer1->setVisible(false);
			 layer2->setVisible(true);
		 }else{
			 actionFp();
		 }
	 }
	 num=1;
}







void Acount::drawAcountlayer(){


	
	
}

void Acount::fillOuty(CCMenuItemImage* p,string imgfile){


	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.10f);
	p->setSelectedImage(s);
	CCSprite* n=CCSprite::create(imgfile.c_str());
	CCLayer* layer=CCLayer::create();
	layer->setContentSize(CCSize(p->getContentSize()*1.10f));

	n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	layer->addChild(n);
	p->setNormalImage(layer);

	if(p->getTag()==Yaoqing_tag){

		string st_str=LFStrings::getValue("yaoqin");
		inN=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		inN->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
		//inN->setColor(ccc3(3,79,132));
		s->addChild(inN);

		inS=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		inS->setPosition(CCPoint(n->getPositionX(),n->getPositionY()));
		//inS->setColor(ccc3(3,79,132));
		layer->addChild(inS);

	}else if(p->getTag()==Yaoqing_tag2){

		string st_str=LFStrings::getValue("yaoqin");
		inN2=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		inN2->setPosition(CCPoint(layer->getContentSize().width/2-15,layer->getContentSize().height/2));
		s->addChild(inN2);

		inS2=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		inS2->setPosition(CCPoint(n->getPositionX(),n->getPositionY()));
		layer->addChild(inS2);

	}else if(p->getTag()==Fenxiang_tag){

		
		string st_str=LFStrings::getValue("share");
		shN=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		shN->setPosition(CCPoint(layer->getContentSize().width/2+10,layer->getContentSize().height*0.415f));
		//inN->setColor(ccc3(3,79,132));
		s->addChild(shN);

		shS=CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,40);
		shS->setPosition(CCPoint(n->getPositionX()+20,layer->getContentSize().height*0.45f));
		//inS->setColor(ccc3(3,79,132));
		layer->addChild(shS);
		
	}
}

void Acount::drawfriendlist(){

	 
	 layer2=CCLayer::create();
	 layer2->setVisible(false);
	 layer2->setScale(chaonengdj_scale);
	 layer2->setContentSize(size);
	 
	 addChild(layer2);
	 string bg=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang.png");
	 CCSprite* tmp = CCSprite::create(bg.c_str());
	 CCSize size2 = tmp->getContentSize();
	 CCRect fullRect = CCRectMake(0,0, size2.width, size2.height);
	 CCRect insetRect = CCRectMake(size2.width/2-2,size2.height*0.8f,2,2);
     

	 CCScale9Sprite* bg_sc=CCScale9Sprite::create(bg.c_str(),fullRect,insetRect);
	 bg_sc->setContentSize(CCSize(755,535));
	 bg_sc->setPosition(CCPoint(size.width/2,size.height/2));
	 layer2->addChild(bg_sc);
     
	 
	 touchsize=bg_sc->getContentSize();
	 string tiao=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_biaoti.png");
	 CCSprite* tmp1 = CCSprite::create(tiao.c_str());
	 CCSize size3 = tmp1->getContentSize();
	 CCRect fullRect3 = CCRectMake(0,0, size3.width, size3.height);
	 CCRect insetRect3 = CCRectMake(20, 0,20, size3.height);
	 
	 string your_text=LFStrings::getValue("yourf");
	 CCLabelTTF* data1 = CCLabelTTF::create(your_text.c_str(),fontStr_KlavikaBold,60);
	 


	 CCScale9Sprite* tiao_sp=CCScale9Sprite::create(tiao.c_str(),fullRect3,insetRect3);
     
	 tiao_sp->setContentSize(CCSize(70+data1->getContentSize().width,10+data1->getContentSize().height));
	 tiao_sp->setPosition(CCPoint(tiao_sp->getContentSize().width/2,bg_sc->getContentSize().height*0.885));
	 bg_sc->addChild(tiao_sp);
	 data1->setPosition(CCPoint(tiao_sp->getContentSize().width/2,tiao_sp->getContentSize().height/2));
	 tiao_sp->addChild(data1);


	
	 list2=Baselisttable::create();
	 list2->setMtabletype(Yaoqingfriendlist,CCSize(bg_sc->getContentSize().width,83),4,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,10);
	 list2->ignoreAnchorPointForPosition(false);
	 list2->setAnchorPoint(CCPoint(0.5f,0.5f));
	 list2->setPosition(CCPoint(bg_sc->getContentSize().width/2,bg_sc->getContentSize().height*0.45));
	 list2->setTag(layer2_list);
	 bg_sc->addChild(list2,1025);

	
	 string fxs[2]={ResManager::getManager()->getFilePath(g_public+"JS_fenxiang_haoyou_anniu_fengxiang_zhuangshi.png"),
	                 ResManager::getManager()->getFilePath(g_public+"JS_fenxiang_haoyou_anniu_fengxiang.png")
	                }; 
	 CCSprite* left_yy=CCSprite::create(fxs[0].c_str());
     left_yy->setPosition(CCPoint(left_yy->getContentSize().width/2,left_yy->getContentSize().height/2+5));
	 bg_sc->addChild(left_yy);

	 
	 CCSprite* right_yy=CCSprite::create(fxs[0].c_str());
	 right_yy->setPosition(CCPoint(bg_sc->getContentSize().width-right_yy->getContentSize().width/2-2,right_yy->getContentSize().height/2+5));
	 right_yy->setRotation(270.0f);
	 bg_sc->addChild(right_yy);
	 

	 
     //Yaoqing_tag2
	 CCMenuItemImage* fx_spt=CCMenuItemImage::create(fxs[1].c_str(),fxs[1].c_str(),this,menu_selector(Acount::allCallback2));
	 fx_spt->setTag(Yaoqing_tag2);
	 fillOuty(fx_spt,fxs[1]);
	 fx_spt->setPosition(CCPoint(bg_sc->getPositionX(),bg_sc->getPositionY()-bg_sc->getContentSize().height/2-fx_spt->getContentSize().height/2+10));

	 CCMenu* y_m=CCMenu::create(fx_spt,nullptr);
	 y_m->setPosition(CCPointZero);
	 layer2->addChild(y_m);
}


void Acount::allCallback2(CCObject* ob){
 

	Music::sharedMusicPlayer()->buttonClicked();
	CCMenuItemImage* p=(CCMenuItemImage*)ob;
	int tag=p->getTag();
	if(tag==Yaoqing_tag2){//邀请回调
		//是否联网 
		//登录fb         
		//
		/*
		if(mark==layer1_tag){
			mark=layer2_tag;
			setTruelayer();
			//CCDirector::sharedDirector()->getRunningScene()->addChild(Tanchuk::creteTag(NonetWork_tag));

		}else if(mark==layer2_tag){


		}
		
		*/
		CCLOG("heheh------------------->");

		list2->fxves;
		allusers;


	}else if(tag==Fenxiang_tag){//分享回调

	}
	
}

bool Acount::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){


	if(mark==layer2_tag){

		CCPoint p=pTouch->getLocation();
		bool x_m=(p.x<=size.width/2+touchsize.width/2&&p.x>=size.width/2-touchsize.width/2);
		bool y_m=(p.y<=size.height/2+touchsize.height/2&&p.y>=size.height/2-touchsize.height/2);
		if(x_m==false||y_m==false){
			mark=layer1_tag;
			//是否联网
			layer2->runAction(
				CCSequence::create(
				CCScaleTo::create(left_time,chaonengdj_scale,chaonengdj_scale),
				CCCallFunc::create(this,callfunc_selector(Acount::yaoqing_changeacount)),
				nullptr
				)
				);
			Outall(layer2,leftaction_tag);

			CCLOG("ii--->");

		}
	}
	return true;
}


void Acount::acount_chnageYaoqing(){
	 
	 mark=layer2_tag;
	 acount_layer->setVisible(false);
	 layer2->setVisible(true);
	 Fadeinnodes(layer2,leftaction_tag);
	 layer2->runAction(CCScaleTo::create(left_time,1.0f,1.0f));
	 button_mark=true;

}

void Acount::yaoqing_changeacount(){

	mark=layer1_tag;
	acount_layer->setVisible(true);
	layer2->setVisible(false);

	Fadeinnodes(acount_layer,leftaction_tag);
	acount_layer->runAction(CCScaleTo::create(left_time,1.0f,1.0f));
	 

}


void Acount::sectionSwitchaction(){
	 /*
	 layer1_tag=0,
	 layer2_tag,
	 acount_layer
	 layer2
	 */
	

		 acount_layer->runAction(
			 CCSequence::create(
			 CCFadeOut::create(0.25f),
			 CCCallFunc::create(this,callfunc_selector(Acount::sectionActionend)),
			 nullptr
			 )
			 );
		 Outall(acount_layer,0.25f);

	 

}

void Acount::sectionActionend(){


	LevelManager* pLevelManger = LevelManager::getManager();
	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();

	bool bChallengeSection = false;
	if (GameInforEditer::getGameInfor()->isChallengeSection(pLevelManger->getLastSeasonId(),pLevelManger->getLastSectionId()))
	{
		bChallengeSection = true;
	}

	int nextSeasonId = layerFactory->getNextSeasonId(pLevelManger->getLastSeasonId(),pLevelManger->getLastSectionId(), bChallengeSection);
	int nextSectionId = layerFactory->getNextSectionId(pLevelManger->getLastSeasonId(),pLevelManger->getLastSectionId(), bChallengeSection);

	//打到16关，跳出去，并且判断是否显示隐藏关卡的解锁动画
	if ((!bChallengeSection && nextSeasonId != pLevelManger->getLastSeasonId() && nextSectionId == 1))
	{
		pLevelManger->setLastSectionId(pLevelManger->getLastSectionId());
#if Version_Free != 0
		LevelManager::getManager()->setSlideToPage3(false);
#else
		LevelManager::getManager()->setSlideToPage3(true);
		//如果没有播放过解锁动画，并且星星数足够,才设置播放解锁动画
		if (!CCUserDefault::sharedUserDefault()->getBoolForKey(Show_Unlock_Animation, false)
			&& GameInforEditer::getGameInfor()->getSecretStageCanUnLocked(pLevelManger->getLastSeasonId()))
		{
			LevelManager::getManager()->setShowUnlockAni(true);
		}
#endif
		pLevelManger->GameInToSectionLayer();
		return;
	}
	//打到20关，跳到当前小关卡选择界面
	else if (nextSeasonId >= GameLayerFactory::getInstance()->getConfigSeasonNum() || (bChallengeSection && nextSeasonId != pLevelManger->getLastSeasonId()))
	{
		LevelManager::getManager()->setSlideToPage3(false);
		LevelManager::getManager()->setShowUnlockAni(false);
		pLevelManger->GameInToSectionLayer();
		return;
	}

	if (CCNode* p = BaseScense::getCurrentScene()->getChildByTag(this->getTag() + 100))
	{
		p->removeFromParent();
	}
	this->removeFromParent();
	LevelManager::getManager()->setNext(true);
	LevelManager::getManager()->setShowLoadingAnimation(false);
	GameScenseDefine::SharedGameScenseDefine()->createBaseContentLayer(nextSeasonId, nextSectionId);
	  

}

void Acount::Fadeinnodes(CCNode* node,float time){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCEaseExponentialIn::create(CCFadeIn::create(time)));
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Fadeinnodes(node,time);
			}
		}
	}
}


void Acount::Outall(CCNode* node,float time){

	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCEaseExponentialOut::create(CCFadeOut::create(time)));
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Outall(node,time);
			}
		}
	}

}

//donghua
void Acount::Fadeinnodes(CCNode* node,int tag){

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
			}else if(tag==bitaoTandstar_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeIn::create(btandst_time)));
			}else if(tag==endTime_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeIn::create(end_time)));
			}
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				Fadeinnodes(node,tag);
			}
		}
	}
}

void Acount::Outall(CCNode* node,int tag){

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
			}else if(tag==bitaoTandstar_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeOut::create(btandst_time)));
			}else if(tag==endTime_tag){
				node->runAction(CCEaseExponentialOut::create(CCFadeOut::create(end_time)));
			}
			if(node->getChildren()!=nullptr&&node->getChildren()->count()>0){
				Outall(node,tag);
			}
		}
	}
}





 /*
	CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 180, 0); 
	layer2->runAction(orbitFront);
		 CCOrbitCamera* orbitFront1 = CCOrbitCamera::create(orbitTime, 1, 0, 90, 180, 0, 0); 
		 layer1->runAction(orbitFront1);
          CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 0, 0); 
		  layer1->runAction(orbitFront);
		  CCOrbitCamera* orbitFront1 = CCOrbitCamera::create(orbitTime, 1, 0, 0, 0, 90, 0); 
		  layer2->runAction(orbitFront1);
		  //layer1->setVisible(true);
		  //layer2->setVisible(false);
*/

void Acount::actionFp(){

	float orbitTime = 1;  
	if(mark==layer1_tag){
		action2_1();
	}else{
		action1_2();
	}
}


void Acount::action2_1(){

	nodesOut(list2);
	for(size_t i=0;i<nodes2.size();i++){
		if(i==nodes2.size()-1){
			nodes2.at(i)->runAction(
				CCSequence::create(
				CCFadeOut::create(jesuantime_action),
				CCCallFunc::create(this,callfunc_selector(Acount::action2_1hd)),
				NULL
				)
				);
		}else{
			nodes2.at(i)->runAction(CCFadeOut::create(jesuantime_action));
		}
	}
}

void Acount::action2_1hd(){


	nodesIn(list);
	layer1->setVisible(true);
	layer2->setVisible(false);
	for(size_t i=0;i<nodes1.size();i++){
		nodes1.at(i)->runAction(CCFadeIn::create(jesuantime_action));
	}

}




void Acount::action1_2(){

	nodesOut(list);
	for(size_t i=0;i<nodes1.size();i++){
		if(i==nodes1.size()-1){
			nodes1.at(i)->runAction(
				CCSequence::create(
				CCFadeOut::create(jesuantime_action),
				CCCallFunc::create(this,callfunc_selector(Acount::action1_2hd)),
				NULL
				)
				);
		}else{
			nodes1.at(i)->runAction(CCFadeOut::create(jesuantime_action));
		}	
	}


}

void Acount::action1_2hd(){

	nodesIn(list2);
	layer1->setVisible(false);
	layer2->setVisible(true);

	for(size_t i=0;i<nodes2.size();i++){
		nodes2.at(i)->runAction(CCFadeIn::create(jesuantime_action));
	}



}

void Acount::hd1(CCNode* p){

	CCArray* arry= p->getChildren();
	for(size_t i=0;i<arry->count();i++)
	{
		CCNode* p=((CCNode*)arry->objectAtIndex(i)); 
		if(p->getTag()==layer1_list){
			continue;
		}

		nodes1.push_back(p);

		if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			hd1(p);
		}
	}
}



void Acount::nodesOut(CCNode* node){

	CCArray* arry= node->getChildren();
	for(size_t i=0;i<arry->count();i++)
	{
		CCNode* p=((CCNode*)arry->objectAtIndex(i)); 
		p->runAction(CCFadeOut::create(jesuantime_action));
		if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			nodesOut(p);
		}
	}
}

void Acount::nodesIn(CCNode* node){


	CCArray* arry= node->getChildren();
	for(size_t i=0;i<arry->count();i++)
	{
		CCNode* p=((CCNode*)arry->objectAtIndex(i)); 
		p->runAction(CCFadeIn::create(jesuantime_action));
		if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			nodesIn(p);
		}

	}

}


void Acount::hd2(CCNode* p){

	CCArray* arry= p->getChildren();
	for(size_t i=0;i<arry->count();i++)
	{
		CCNode* p=((CCNode*)arry->objectAtIndex(i)); 
		if(p->getTag()==layer2_list){
			continue;
		}
		nodes2.push_back(p);
		if(p->getChildren()!=NULL&&p->getChildren()->count()>0){
			if(p->getTag()==layer2_list){
				continue;
			}
			hd2(p);
		}
	}
	int m=0;
}


void Acount::firstAction(){

	for(size_t j=0;j<vsps.size();j++){
		vsps.at(j)->setVisible(false);
	}
	for(size_t i=0;i<vspsstar.size();i++){
		vspsstar.at(i)->setScale(0.0f);
	}
	num_star=0;

	if(vspsstar.size()==1){
		index=0;
	}else if(vspsstar.size()==2){
		index=1;
	}else if(vspsstar.size()==3){
		index=1;
	}
	actionode(vspsstar.at(index));
}


void Acount::actionode(CCNode* node){



	if(num_star==vspsstar.size()-1){
		vspsstar.at(index)->runAction(
			CCSequence::create(
			CCScaleTo::create(jesuantime_action,1.0f,1.0f)
			,
			CCCallFunc::create(this,callfunc_selector(Acount::starEnd)),
			NULL
			)

			);
	}else if(num_star<(int)vspsstar.size()-1){

		vspsstar.at(index)->runAction(
			CCSequence::create(
			CCScaleTo::create(jesuantime_action,1.0f,1.0f)
			,
			CCCallFunc::create(this,callfunc_selector(Acount::starNoend)),
			NULL
			)

			);

	}

}


void Acount::starNoend(){

	num_star+=1;

	if(num_star==1&&vspsstar.size()==2){
		index=0;
	}else if(num_star==1&&vspsstar.size()==3){
		index=0;
	}else if(num_star==2&&vspsstar.size()==3){
		index=2;
	}

	actionode(vspsstar.at(index));

}


void Acount::starEnd(){

	for(size_t j=0;j<vsps.size();j++){
		vsps.at(j)->setVisible(true);
		vsps.at(j)->runAction(CCFadeIn::create(jesuantime_action));
	}

	if(curent_starnum>history_numstar){
		tg_sp->setVisible(true);
		tg_sp->runAction(CCFadeIn::create(jesuantime_action));

	}
}



void Acount::freindvideoPlay(CCObject* obj){

	//连续点击事件屏蔽
	if(Shieldingmanger::getSielding()->continuousThetimer()==false){
		return;
	}

	
	freind_solution=obj;
	Myobject* ob=(Myobject*)obj;
	int idx=ob->current_id;
	freind_idx=idx;
	//通过计时来屏蔽连续点击事件
    
    
	if(idx>=dataUnion.size()){
		Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
		return;
	}
	//string datastring = SocialInfoManager::getInstance()->getSolutionData(dataUnion.at(idx).solutionId);
	string datastring = SocialInfoManager::getInstance()->getSolutionData(dataUnion.at(idx).seasonId, dataUnion.at(idx).sectionId, dataUnion.at(idx).userId);
	if (datastring.empty())
	{
		//CCMessageBox("local is not found data!", "Error");
		// video 视屏下载
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
		//CCMessageBox("NO Data Found","Error");
		Shieldingmanger::getSielding()->setcontinuousThetimermark(true);
		return;
	}

    
	drawfriendVideo(dataUnion.at(idx));
	Shieldingmanger::getSielding()->Fadeoutnodes(acount_layer);
	acount_layer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
		CCCallFuncN::create(this,callfuncN_selector(Acount::acouTofriendvideo_actiondone)),
		NULL
		)
		);	  
	

}

void Acount::acouTofriendvideo_actiondone(CCNode* node){

	 node->setVisible(false);
	 friendvideoplay_layer->setVisible(true);
	 Shieldingmanger::getSielding()->Fadeinnodes(friendvideoplay_layer);
	 friendvideoplay_layer->runAction(CCScaleTo::create(chaonengdj_time,1.0f,1.0f));	  
	 Shieldingmanger::getSielding()->setcontinuousThetimermark(true);

}

//从该页到视屏解法页 动作回调函数




void Acount::drawfriendVideo(msgSnsdata data){


	data_play=data;
	if(isfriendVideolayerinit==false){

		friendvideoplay_layer=ShieldTouchlayer::create();
		friendvideoplay_layer->setContentSize(size);
		friendvideoplay_layer->setVisible(false);
		friendvideoplay_layer->setScale(chaonengdj_scale);
		addChild(friendvideoplay_layer);

		//	int index = GameLayerFactory::getInstance()->getSectionIndex(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
		CCScrollView* pPlayRecordLayer = (CCScrollView*)friendvideoplay_layer->getChildByTag(Tag_Layer_PlayRecord);
		if (!pPlayRecordLayer)
		{
			m_bIsPlayRecord = true;
			pPlayRecordLayer = GameScenseDefine::SharedGameScenseDefine()->playRecord(data.seasonId, data.sectionId, &m_allRecordData, false);
			pPlayRecordLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
			friendvideoplay_layer->addChild(pPlayRecordLayer, 1, Tag_Layer_PlayRecord);

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
		isfriendVideolayerinit=true;

		CCSprite* recordBG = CCSprite::create(ResManager::getManager()->getFilePath(g_stop+"ZT_jiefa_bg.png").c_str());
		friendvideoplay_layer->addChild(recordBG, pPlayRecordLayer->getZOrder() - 1);
		recordBG->setPosition(pPlayRecordLayer->getPosition());

		string start_strl=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_guanbi.png");
		CCMenuItemImage*  startmenu_l=CCMenuItemImage::create(start_strl.c_str(),start_strl.c_str(),this,menu_selector(Acount::allCallback));
		startmenu_l->setTag(close_friendacountsolutiontag);

		CCMenu* pMenu = CCMenu::create(startmenu_l,NULL);
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
			menu_selector(Acount::zan_button));
		z_ccbt->setUserData((void*)data.solutionId.c_str());

		CCMenu* pMenu2 = CCMenu::create(z_ccbt,NULL);
		pMenu2->setTouchPriority(kCCMenuHandlerPriority-1);
		pMenu2->setPosition(ccp(recordBG->getContentSize().width - z_ccbt->getContentSize().width/2 + 10, - z_ccbt->getContentSize().height/2 + 10));
		recordBG->addChild(pMenu2, 1);

		//点赞次数
		string num_st=CCString::createWithFormat("%d",data.upTimes)->getCString();
		z_label=CCLabelTTF::create(num_st.c_str(),fontStr_KlavikaBold,30);
		z_label->setPosition(pMenu2->getPosition() - CCPoint(z_label->getContentSize().width/2 + z_ccbt->getContentSize().width/2 - 5, 0));
		recordBG->addChild(z_label);

	}else{

		string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
		toux_sp->init(tb_file.c_str(),data.pictrue.c_str());
		name_label->setString(data.name.c_str());
		z_label->setString(CCString::createWithFormat("%d",data.upTimes)->getCString());
		z_ccbt->setUserData((void*)data.userId.c_str());

	}
	

	
	

}

void Acount::allCallback(CCObject* ob){


	if(button_mark==false){
		return;
	}
	Music::sharedMusicPlayer()->buttonClicked();
	button_mark=false;
	LevelManager* pLevelManger = LevelManager::getManager();
	CCMenuItemImage* p=(CCMenuItemImage*)ob;
	int tag=p->getTag();
	if(tag==Dw_tag){

	}else if(tag==Yaoqing_tag){
		if (!CmdHelper::getInstance()->isLogin())
		{
			CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Acount::loginResult), NetMessage::MSG_LoginResult, NULL);
			CmdHelper::getInstance()->login();
		}
		else
		{
			CmdHelper::getInstance()->inviteFriends();
		}
		button_mark = true;
		//是否联网
		// 		 acount_layer->runAction(
		// 			     CCSequence::create(
		// 			        CCScaleTo::create(left_time,chaonengdj_scale,chaonengdj_scale),
		// 					CCCallFunc::create(this,callfunc_selector(Acount::acount_chnageYaoqing)),
		// 					nullptr
		// 				 )
		// 			 );
		//		 Outall(acount_layer,leftaction_tag);
	}else if(tag==Xiaogk_tag){
		pLevelManger->GameInToSectionLayer();
	}else if(tag==Xiaoyiguan_tag){
		sectionSwitchaction();
	}
	else if (tag == Zaiwan_tag)
	{
		LevelManager::getManager()->retry(false);
	}else if(tag==Fenxiang_tag){
		button_mark=true;
		if (!CmdHelper::getInstance()->isLogin())
		{
			CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Acount::loginResult), NetMessage::MSG_LoginResult, NULL);
			CmdHelper::getInstance()->login();
		}
		else
		{
			CmdHelper::getInstance()->share();
		}
	}else if(tag==close_friendacountsolutiontag){


		Shieldingmanger::getSielding()->Fadeoutnodes(friendvideoplay_layer);
		friendvideoplay_layer->runAction(
			CCSequence::create(
			CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			CCCallFuncN::create(this,callfuncN_selector(Acount::actionFaihuimain)),
			NULL
			)
			);
	}
}

void Acount::loginResult(CCObject* pMsg)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NetMessage::MSG_LoginResult);

	MsgData* pdata = (MsgData*)pMsg;
	if (pdata)
	{
		if(pdata->errCode == MsgData::eSuccess)
		{
			MessageBar::show(LFStrings::getValue("Loginsuccessfully").c_str());
			//CCMessageBox("Login successfully", "Tip");
			LevelManager::getManager()->GameInToMainLayer();
			return;
		}
	}
	MessageBar::show(LFStrings::getValue("Loginfailed").c_str());
	//CCMessageBox("Login failed", "Tip");
}

void Acount::actionFaihuimain(CCNode* node){

	list->getTaleView()->setTouchEnabled(true);
	button_mark = true;
	friendvideoplay_layer->removeFromParent();
	friendvideoplay_layer=nullptr;

	isfriendVideolayerinit=false;
	m_bIsPlayRecord = false;
	isclickZ=true;

	acount_layer->setVisible(true);
	Shieldingmanger::getSielding()->Fadeinnodes(acount_layer);
	acount_layer->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,1.0f,1.0f),
		NULL
		)
		);
}


void Acount::zan_button(CCObject* obj){

	if(!isclickZ){
		isclickZ=true;
		CCMenuItemImage* p=(CCMenuItemImage*)obj;
		string solutionid=data_play.solutionId;
		CmdHelper::getInstance()->praiseSolution(solutionid,true, true);

	}	 

}

void Acount::Videofriendlayerinit(){


	 if(isfriendVideolayerinit==false){

		 friendvideoplay_layer=ShieldTouchlayer::create();

	 }

}


void Acount::playBeyondFriendAnimate()
{
	CCLOG("%s", __FUNCTION__);
	string userId;
	if (! CmdHelper::getInstance()->getCurrentUserID(userId))
		return;

	int seasonId = LevelManager::getManager()->getLastSeasonId();
	int sectionId = LevelManager::getManager()->getLastSectionId();
	//如果以前没有自己的数据，然后就不显示超越动画
	{
		bool bHasOldTaskInfor = false;
		const TaskInfor* pTaskInfor = SocialInfoManager::getInstance()->getSectionInfor(seasonId, sectionId);
		if (pTaskInfor)
		{
			for (vector<TaskSolution>::const_iterator it = pTaskInfor->users.begin(); it!=pTaskInfor->users.end(); ++it)
			{
				if (it->userId == userId){
					bHasOldTaskInfor = true;
					break;
				}
			}
		}
		if (!bHasOldTaskInfor || this->info_layer == nullptr)
		{
			SocialInfoManager::getInstance()->setSelfSectionScore(seasonId, sectionId, this->acount_score, this->curent_starnum);
			const TaskInfor* info = SocialInfoManager::getInstance()->getSectionInfor(seasonId, sectionId);
			if(info){
				this->info_layer=info;
				shuaixinData();
			}
			//list->setGkmsglayeruser(vec);
			return;
		}
	}
	
	if (this->acount_score <= this->history_score){
		return;
	}

	bool bNotFoundSelf = true;
	int selfIdx = 0, willIdx = 10000;
	for (int i=0,count=info_layer->users.size(); i<count; ++i)
	{
		if(info_layer->users.at(i).userId == userId) {
			selfIdx = i;
			bNotFoundSelf = false;
		}

		if (this->acount_score > info_layer->users[i].score && willIdx > i){
			willIdx = i;
		}
	}
	//CCAssert(!bNotFoundSelf, "");
	if (bNotFoundSelf || willIdx >= selfIdx){
		const TaskInfor* info = SocialInfoManager::getInstance()->getSectionInfor(seasonId, sectionId);;
		if(info){
			this->info_layer=info;
			shuaixinData();
		}
		return;
	}

	CCTableView* pTable = list->getTaleView();
	if (! pTable) return;
	CCTableViewCell* pCell = pTable->cellAtIndex(selfIdx);
	CCTableViewCell* pWillCell = pTable->cellAtIndex(willIdx);
	if (! pCell || ! pWillCell) return;
	list->moveTableCellAt(pCell, willIdx);
}

void Acount::beyondFriendAnimateEnd_refreshData(CCObject* obj)
{
	int seasonId = LevelManager::getManager()->getLastSeasonId();
	int sectionId = LevelManager::getManager()->getLastSectionId();
	const TaskInfor* info = SocialInfoManager::getInstance()->getSectionInfor(seasonId, sectionId);;
	if(info){
		this->info_layer=info;
		shuaixinData();
	}
}