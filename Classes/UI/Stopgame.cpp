#include "Stopgame.h"
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
#include "urlSprite.h"
#include "MenuLayer.h"
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
#include "Shieldingmanger.h"


enum Stoptag{


	start_tag=10,
	faihui_zhuye,
	xiao_gkselect,
	yinxiao_tag,
	yinyue_tag,
};

Stopgame::Stopgame(void)
{
}
Stopgame::Stopgame(bool ismove){
	this->isMove=ismove;
}
Stopgame::~Stopgame(void)
{
	menuitems.clear();
}




bool Stopgame::init(){

	if(!CCLayer::init()){
		return false;
	}
	this->setZOrder(100);
	mark=true;
	isjixu=false;
	button=false;
	size=CCDirector::sharedDirector()->getWinSize();
	ccColor4B mr;
	mr.r=0;mr.g=0;mr.b=0;mr.a=num_Fade*0.4f;
	CCLayerColor* colorLayer1 = CCLayerColor::create(mr);
	colorLayer1->setContentSize(CCSize(180,size.height));
	addChild(colorLayer1);
	drawStop();
	
	//this->setTouchEnabled(true);
	//this->setTouchEnabled(true);
	//this->setTouchPriority(-129);
	//this->setTouchMode(kCCTouchesOneByOne);
	//this->registerWithTouchDispatcher();
	return true;
} 
void Stopgame::onEnter(){

	CCLayer::onEnter();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Stopgame::Stopmsg), MSG_GAME_Stop, NULL);

}

void Stopgame::onExit(){
	CCLayer::onExit();
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


void    Stopgame::Stopmsg(CCObject* object){

        int tag=(int)(long long)object;

		if(tag==Gamecontinu_tag){


		}else if(tag==Gameother_tag){
			this->setVisible(true);
			Fadeinnodes(layer_stop);

		}else if(tag==Gamestopxiaoshi_tag){

			actionxiaoshi();
		}else if(tag==Gamestopxiaoshi_xiaosi){

			isjixu=true;
			actionQux();
		}

}
void Stopgame::actionxiaoshi(){

	layer_stop->runAction(
		CCSequence::create(
		CCFadeOut::create(time_action),
		CCCallFunc::create(this,callfunc_selector(Stopgame::end)),
		nullptr
		)
		);
	Fadeoutnodes(layer_stop);
}

void Stopgame::end(){

	this->setVisible(false);
}

Stopgame* Stopgame::createBymark(bool isMove=true)
{

	Stopgame* pRet=new Stopgame(isMove);
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}
}

void Stopgame::drawStop(){

	 
	 this->setContentSize(CCSize(180,size.height));
	 if(isMove==true){
	   this->setPosition(CCPoint(getPosition().x-this->getContentSize().width,getPositionY()));
	 }

	 
	 layer_stop=CCLayer::create();
	 layer_stop->setContentSize(CCSize(180,size.height));
	 addChild(layer_stop);

     string hese="ZT_bg_touying.png";
	 hese=ResManager::getManager()->getFilePath(g_stop+hese);
	 sprite=CCScale9Sprite::create(hese.c_str());
	 sprite->setContentSize(CCSize(11,size.height));
	 sprite->setAnchorPoint(ccp(0.5,0.5));
	 sprite->ignoreAnchorPointForPosition(false);
	 sprite->setPosition(CCPoint(getContentSize().width-sprite->getContentSize().width/2,size.height/2));
	 layer_stop->addChild(sprite,1025);


	


	 float startPoint=0.8f;
	 float jg=0.15f;
	 string start="ZT_anniu_jixu.png";
	 start=ResManager::getManager()->getFilePath(g_stop+start);


	 CCMenuItemImage* button_st=CCMenuItemImage::create(
		 start.c_str(),
		 start.c_str(),
		 this,
		 menu_selector(Stopgame::allCallback));
	 button_st->setTag(start_tag);
	 button_st->setPosition(ccp(getContentSize().width/2,size.height*(startPoint)));
	 fillOuty(button_st,start);
     menuitems.push_back(button_st);
     
	 string fhui="ZT_anniu_zhuye.png";
	 fhui=ResManager::getManager()->getFilePath(g_stop+fhui);
	 CCMenuItemImage* fhzy_st=CCMenuItemImage::create(
		 fhui.c_str(),
		 fhui.c_str(),
		 this,
		 menu_selector(Stopgame::allCallback));
	 fhzy_st->setTag(faihui_zhuye);
	 fhzy_st->setPosition(ccp(getContentSize().width/2,size.height*(startPoint-jg)));
	 fillOuty(fhzy_st,fhui);
	 menuitems.push_back(fhzy_st);

	 string xiao_gk="ZT_anniu_guanqiaxuanze.png";
	 xiao_gk=ResManager::getManager()->getFilePath(g_stop+xiao_gk);
	 CCMenuItemImage* xiao_st=CCMenuItemImage::create(
		 xiao_gk.c_str(),
		 xiao_gk.c_str(),
		 this,
		 menu_selector(Stopgame::allCallback));
	 xiao_st->setTag(xiao_gkselect);
	 xiao_st->setPosition(ccp(getContentSize().width/2,size.height*(startPoint-jg*2)));
	 fillOuty(xiao_st,xiao_gk);
	 menuitems.push_back(xiao_st);

	 //音效
	 string yinxiao="";//"ZT_anniu_yinxiao.png";
	 string yinxiao_select="";//"ZT_anniu_yinxiao_guanbi.png";

	 bool kg_yx= Music::sharedMusicPlayer()->isEffectiveMute();
	 if(kg_yx==true){
		 yinxiao=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinxiao_guanbi.png");
		 yinxiao_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinxiao.png");
	 }else{
		 yinxiao=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinxiao.png");
		 yinxiao_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinxiao_guanbi.png");
	 }
	 
	 CCMenuItemImage* yinx_bt=CCMenuItemImage::create(
		 yinxiao.c_str(),
		 yinxiao.c_str(),
		 this,
		 menu_selector(Stopgame::allCallback));
	 yinx_bt->setTag( yinxiao_tag);
	 yinx_bt->setPosition(ccp(getContentSize().width/2,size.height*(startPoint-jg*3)));
	 fillOuty(yinx_bt,yinxiao);
	 menuitems.push_back(yinx_bt);

	 //音乐
	 string yiny="";//"ZT_anniu_yinyue.png";
	 string yiny_select="";//"ZT_anniu_yinyueguanbi.png";
     
	 bool yiny_y=Music::sharedMusicPlayer()->isBackgroundMute();
	 if(yiny_y==true){
		 yiny_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinyue.png");
		 yiny=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinyueguanbi.png");
	 }else if(yiny_y==false){
		 yiny_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinyueguanbi.png");
		 yiny=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinyue.png");
	 }


	 CCMenuItemImage* yy_bt=CCMenuItemImage::create(
		 yiny.c_str(),
		 yiny.c_str(),
		 this,
		 menu_selector(Stopgame::allCallback));
	 yy_bt->setTag( yinyue_tag);
	 yy_bt->setPosition(ccp(getContentSize().width/2,size.height*(startPoint-jg*4)));
	 fillOuty(yy_bt,yiny);
	 menuitems.push_back(yy_bt);
     
	 CCMenu* pMenu = CCMenu::create(button_st,fhzy_st,xiao_st,yinx_bt,yy_bt,NULL);
	 pMenu->setPosition(CCPointZero);
	 layer_stop->addChild(pMenu, 1);
	 runactionDh();
	 
}

void Stopgame::allCallback(CCObject* pSender){


	if(Shieldingmanger::getSielding()->isPinbingbutton()==true){

		return;
	}

	if(button==true){

		return;
	}
	button=true; 
	Music::sharedMusicPlayer()->buttonClicked();
	 CCMenuItemImage* p=(CCMenuItemImage*)pSender;
	 int tag=p->getTag();


	 if(tag==start_tag){

		 isjixu=true;
		 actionQux();
		 int tag_msg=Gamestopxiaoshi_tag;
		 CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stopright,(CCObject*)tag_msg);

	 }else if(tag==faihui_zhuye){

		 LevelManager::getManager()->GameInToMainLayer();

	 }else if(tag==xiao_gkselect){
		
		 LevelManager::getManager()->goToSectionUI();

	 }else if(tag== yinxiao_tag){
		  button=false;
		 //音效
		 string yinxiao=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinxiao.png");
		 string yinxiao_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinxiao_guanbi.png");

		 bool kg_yx= Music::sharedMusicPlayer()->isEffectiveMute();

		 if(kg_yx==true){
			 p->setNormalImage(CCSprite::create(yinxiao.c_str()));
			 fillOuty(p,yinxiao);
			 Music::sharedMusicPlayer()->setEffectiveMute(false);
			 GameInforEditer::getGameInfor()->saveSessonInforList();
		 }else{
			 p->setNormalImage(CCSprite::create(yinxiao_select.c_str()));
			 fillOuty(p,yinxiao_select);
			 Music::sharedMusicPlayer()->setEffectiveMute(true);
			 GameInforEditer::getGameInfor()->saveSessonInforList();
		 }

	 }else if(tag==yinyue_tag){
		  button=false;
         string yiny=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinyue.png");
		 string yiny_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yinyueguanbi.png");

		 bool yiny_y=Music::sharedMusicPlayer()->isBackgroundMute();
		 if(yiny_y==true){
			 p->setNormalImage(CCSprite::create(yiny.c_str()));
			 fillOuty(p,yiny);
			 Music::sharedMusicPlayer()->setBackgroundMute(false);
			 GameInforEditer::getGameInfor()->saveSessonInforList();
		 }else{
			 p->setNormalImage(CCSprite::create(yiny_select.c_str()));
			 fillOuty(p,yiny_select);
			 Music::sharedMusicPlayer()->setBackgroundMute(true);
			 GameInforEditer::getGameInfor()->saveSessonInforList();
		 }


	 }

}


void Stopgame::fillOuty(CCMenuItemImage* p,string imgfile){

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

void Stopgame::runactionDh(){
	 
	 if(isMove==true){
	    this->runAction(CCMoveTo::create(time_action,CCPoint(getContentSize().width+getPositionX(),getPositionY())));
	 }
     sprite->runAction(CCFadeIn::create(time_action));
	 Fadeinnodes(layer_stop);

}

void Stopgame::Fadeinnodes(CCNode* node){
	 
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

void Stopgame::Fadeoutnodes(CCNode* node){

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






void Stopgame::actionQux(){

	
	Fadeoutnodes(layer_stop);
	if(isjixu==true){
		sprite->runAction(
			CCSequence::create(
			CCFadeOut::create(time_action),
			CCCallFunc::create(this,callfunc_selector(Stopgame::dealWith)),
			NULL
			)
			);
		CCPoint p=CCPoint(getPositionX()-getContentSize().width,getPositionY());
		this->runAction(CCMoveTo::create(chaonengdj_time,p)); 
	}else{

		sprite->runAction(
			CCSequence::create(
			CCFadeOut::create(time_action),
			CCCallFunc::create(this,callfunc_selector(Stopgame::dealWith)),
			NULL
			)
			);
	}
	
}

void Stopgame::dealWith(){
	  button=false;
	 if(isjixu==true){//显示游戏图层
		 int tag=Gamecontinu_tag;
		 CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_NEI,(CCObject*)tag);
		 baselayerOprate();
		 this->removeFromParent();
	 }else{
		 this->setVisible(false);
	 }
	 
}

void Stopgame::baselayerNode(CCNode* node){
	 
	 node->setVisible(true);
	  
}


void Stopgame::baselayerOprate(){
	
			BaseScense::getCurrentScene()->getMenuLayer()->setVisible(true);
			if (BaseScense::getCurrentScene()->getGuideLayer())
			{
				BaseScense::getCurrentScene()->getGuideLayer()->setVisible(true);
			}
			
			SpriteHelper::VisibleSubNodeAndChildrenInParentNodeByTag(BaseContentLayer::shareBaseContentLayer(), Tag_Fragment, true);
			BaseScense::getCurrentScene()->setCollisionEnable(true);
			BaseScense::getCurrentScene()->getMenuLayer()->resume(true);
			//得到齿轮的背景
			nodesallFade(BaseScense::getCurrentScene()->getBackgroudCoverLayer(),false);
			if(BaseScense::getCurrentScene()->getBackgroudCoverLayer()){
				BaseScense::getCurrentScene()->getBackgroudCoverLayer()->runAction(CCFadeIn::create(time_action));
			}
			//得到所有的元素（木块，星星）
			nodesallFade(BaseContentLayer::shareBaseContentLayer(),false);
			if(BaseContentLayer::shareBaseContentLayer()){
				BaseContentLayer::shareBaseContentLayer()->runAction(CCFadeIn::create(time_action));
			}

			BaseContentLayer::shareBaseContentLayer()->setInfoVisible(true);
			BaseContentLayer::shareBaseContentLayer()->setRoleVisible(true);
}

void Stopgame::nodesallFade(CCNode* node,bool mark){
	 if(node){
		 CCArray* nodes=node->getChildren();

		 if(nodes){

		 for(size_t i=0;i<nodes->count();i++){
			 CCNode* node= (CCNode*)nodes->objectAtIndex(i);
			 //冰冻炸弹效果:点击了之后才显示，不然会有冰冻炸弹和效果同时出现的情况
			 if (node->getTag() == Tag_Box_Frost)
			 {
				 if(((TimerDec*)node)->getIsDecing())
				 {
					 node->setVisible(true);
				 }
				 continue;
			 }	
			 else if (node->getTag() == Tag_Box_Frost_Par)
			 {
				 continue;
			 }
			 else if (node->getTag() == Tag_Box_Bomb_DarkWhole)
			 {
				 //黑洞点击了后会隐藏黑洞图片，所以点击了之后，就不要再显示了，不然会有黑洞和黑洞效果同时出现的情况
				 //黑洞的粒子也是要在点击了黑洞了之后才能显示
				 if(((DarkHoleBomb*)node)->getIsBombed())
				 {
					 node->setVisible(false);
					 if (CCNode* pNode = node->getChildByTag(Tag_Box_DarkHoleBomb_Par))
					 {
						 pNode->setVisible(true);
					 }
				 }
				 continue;
			 }
			 else if (node->getTag() == Tag_Box_DarkHoleBomb_Par)
			 {
				 continue;
			 }
			 else if (node->getTag() == Tag_Anchor_Mator)
			 {
				 //可点击的转动马达，需要单独处理
				 MotorSprite* psp = (MotorSprite*)node;
				 psp->setVisible(true);
				 psp->setOpacity(255);
				 if (((MotorSprite*)node)->getIsStop())
				 {
					 ((MotorSprite*)node)->stopRotation();
				 }
				 else
				 {
					 ((MotorSprite*)node)->remuseRotation();
				 }
				 continue;
			 }
             CCActionInterval* action;
			 if(mark==false){
				 action=CCFadeIn::create(time_action);
			 }else{
				 action=CCFadeOut::create(time_action);
			 }
			 if(mark==false){
				 node->runAction(
					 CCSequence::create(
					 action,
					 CCCallFuncN::create(this,callfuncN_selector(Stopgame::baselayerNode)),
					 NULL
					 )
					 );
			 }else{
				 node->runAction(action);
			 }

			 if (node->getTag() == Tag_Box_RedStar_Mask)
			 {
				 if (!mark)
				 {
					 node->runAction(CCRepeat::create(CCSequence::create(CCFadeTo::create(0.9f,0),CCFadeTo::create(0.9f,180),NULL),-1));
				 }
			 }

			 if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				 nodesallFade(node,mark);
			 }

			 if (i == nodes->count()-1)
			 {
				 BaseContentLayer::shareBaseContentLayer()->setVisible(!mark);
			 }
			 
		  }
		 }
	 }
}







