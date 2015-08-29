#include "Chaonengdaoju.h"
#include "font.h"
#include "MenuLayer.h"
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
#include "Baselisttable.h"
#include "LevelManager.h"
#include "LFStrings.h"
#include "GameInforEditer.h"


enum Chaonengdaojunum
{
	useButton_tag=1025,
};

Chaonengdaoju::Chaonengdaoju(void){


}
Chaonengdaoju::Chaonengdaoju(bool mark){
	this->mark_prarent=mark;
}
Chaonengdaoju::~Chaonengdaoju(void){

}


Chaonengdaoju*  Chaonengdaoju::creteBytag(bool mark){

	Chaonengdaoju* pRet=new Chaonengdaoju(mark);
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}
}



bool Chaonengdaoju::init(){

	if(!CCLayer::init()){
		return false;
	}
	this->setZOrder(1025);
	size=CCDirector::sharedDirector()->getWinSize();
	drawBuju();
	updatenumberUI();
	return true;
}

void Chaonengdaoju::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,getTouchPriority(),true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Chaonengdaoju::refreshChaondjnumber), MSG_CHAONDJBUYN, NULL);

}
void Chaonengdaoju::onExit(){

	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


bool Chaonengdaoju::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

	CCPoint point=pTouch->getLocation();
	CCSize back_size=back->getContentSize();
	CCPoint back_point=back->getPosition();
	CCRect rect = CCRectMake(0, 0, back->boundingBox().size.width, back->boundingBox().size.height);
	if (!rect.containsPoint(back->convertToNodeSpace(point)))
	{
// 	bool p_x=(point.x<=back_point.x+back_size.width/2&&point.x>=back_point.x-back_size.width/2);
// 	bool p_y=(point.y<=back_point.y+back_size.height/2&&point.y>=back_point.y-back_size.height/2);
// 	if(p_x==false||p_y==false){
		Fadeoutnodes(list);
		Fadeoutnodes(layer_back);
		back->runAction(
			CCSequence::create(
			CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			CCCallFunc::create(this,callfunc_selector( Chaonengdaoju::actionFaihui)),
			NULL
			)
			);
	}
	return true;
}




void Chaonengdaoju::drawBuju(){


	layer_back=CCLayer::create();
	layer_back->setContentSize(size);
	addChild(layer_back);

	string bg=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang.png");
	CCSprite* tmp1 = CCSprite::create(bg.c_str());
	CCSize size2 = tmp1->getContentSize();
	CCRect fullRect1 = CCRectMake(0,0, size2.width, size2.height);
	CCRect insetRect1 =CCRectMake(size2.width/2-1,size2.height*0.8f, 2, 2);

	//740 360
	string back_file="ZT_tanchukuang.png";
	back_file=ResManager::getManager()->getFilePath(g_stop+back_file);
	back=CCScale9Sprite::create(back_file.c_str(),fullRect1,insetRect1);
	back->setContentSize(CCSize(740,360+84));
	back->setPosition(CCPoint(size.width/2,size.height/2));
	back->setScale(chaonengdj_scale);
	layer_back->addChild(back);
	//标题 270 61

	string biao_t=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_biaoti.png"); 
	CCSprite* tmp = CCSprite::create(biao_t.c_str());
	CCSize size = tmp->getContentSize();
	//创建原始的图像对应大小矩形区域。
	CCRect fullRect = CCRectMake(0,0, size.width, size.height);
	CCRect insetRect = CCRectMake(20, 0,20, size.height);
	CCScale9Sprite* bt_scale=CCScale9Sprite::create(biao_t.c_str(),fullRect,insetRect);
	bt_scale->setContentSize(CCSize(270, size.height));
	bt_scale->setAnchorPoint(ccp(0.0f, 0.5f));
	bt_scale->setPosition(CCPoint(-20, back->getContentSize().height-bt_scale->getContentSize().height/2 - 2));
	back->addChild(bt_scale);

	string bt_text=LFStrings::getValue("Cndj");
	CCLabelTTF* bt_label = CCLabelTTF::create(bt_text.c_str(),fontStr_KlavikaBold,40);
	bt_label->setPosition(CCPoint(bt_scale->getContentSize().width/2,bt_scale->getContentSize().height/2));
	bt_scale->addChild(bt_label);



	list=Baselisttable::create();
	list->setMtabletype(Chaondaoju,CCSize(650,83),2,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,20);
	list->ignoreAnchorPointForPosition(false);
	list->setAnchorPoint(CCPoint(0.5f,0.5f));
	list->setPosition(CCPoint(back->getContentSize().width/2,back->getContentSize().height*0.55));
	back->addChild(list);


	string use_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_goumai.png");
	use_ccbt=CCMenuItemImage::create(use_bt.c_str(),use_bt.c_str(),this,
		menu_selector(Chaonengdaoju::usebuton));
	use_ccbt->setTag(useButton_tag);
	use_ccbt->setPosition(CCPoint(back->getContentSize().width-50-use_ccbt->getContentSize().width, list->getPositionY()-list->getContentSize().height/2-20-   use_ccbt->getContentSize().height/2));
	fillOuty(use_ccbt,use_bt);
    
	CCMenu* pMenu = CCMenu::create(use_ccbt,NULL);
	pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
	pMenu->setPosition(CCPointZero);
	back->addChild(pMenu, 1);

	string file = ResManager::getManager()->getFilePath(g_stop+"ZT_chaonengli.png");
	CCSprite* icon = CCSprite::create(file.c_str());
	back->addChild(icon);
	icon->setPosition(CCPoint(icon->getContentSize().width + 40, list->getPositionY()-list->getContentSize().height/2-20-use_ccbt->getContentSize().height/2));
    
	string dj_str=CCString::createWithFormat(" x %d",GameInforEditer::getGameInfor()->getSuperToolCount())->getCString();
	cn_lable =CCLabelTTF::create(dj_str.c_str(),fontStr_KlavikaBold, 40);  
	cn_lable->setAnchorPoint(ccp(0.0f, 0.5f));
	cn_lable->setPosition(ccp(icon->boundingBox().size.width + 15, icon->getContentSize().height/2));

	icon->addChild(cn_lable);

	back->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,1.0f,1.0f),NULL
		)
		);
	Fadeinnodes(list);
	Fadeinnodes(layer_back);


}

void Chaonengdaoju::usebuton(CCObject* object){

	 if(GameInforEditer::getGameInfor()->getSuperToolCount()<=0){
		 return;
	 }else{
		Fadeoutnodes(list);
		Fadeoutnodes(layer_back);
		back->runAction(
			CCSequence::create(
			CCScaleTo::create(chaonengdj_time,chaonengdj_scale,chaonengdj_scale),
			CCCallFunc::create(this,callfunc_selector( Chaonengdaoju::actionFaihuiW)),
			NULL
			)
			);
	 }
	 


}

void Chaonengdaoju::faihui(CCObject* pSender){

	back->runAction(
		CCSequence::create(
		CCScaleTo::create(chaonengdj_time,0.0f,0.0f),
		CCCallFunc::create(this,callfunc_selector( Chaonengdaoju::actionFaihui)),
		NULL
		)
		);



}

void Chaonengdaoju::Fadeinnodes(CCNode* node){

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

void Chaonengdaoju::changeaptity(CCNode* node,float aptityvalue){
	 
    
	CCArray* ay=node->getChildren();
	if(ay){
		for(size_t i=0;i<ay->count();i++){
			CCNode* node=(CCNode*)ay->objectAtIndex(i);
			node->runAction(CCFadeTo::create(0.1f,aptityvalue));
			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				changeaptity(node,aptityvalue);
			}
		}
	}
}

void Chaonengdaoju::Fadeoutnodes(CCNode* node){

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

void Chaonengdaoju::setSizewidth(float stopwidth){

	this->stopwidth=stopwidth;

}

void Chaonengdaoju::baselayerNode2(CCNode* node){

	node->setVisible(true);

}

void Chaonengdaoju::actionFaihui(){


	if(this->mark_prarent==false){//stop layer

		int tag=Gameother_tag;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stopright,(CCObject*)tag);
	}else if(this->mark_prarent==true){

		BaseScense::getCurrentScene()->getMenuLayer()->resume(true);

	}

	this->removeFromParent();	 
}

void Chaonengdaoju::actionFaihuiW(){

	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USE_SUPERTOOL);

	int tag_msg=Gamestopxiaoshi_tag;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stopright,(CCObject*)tag_msg);
	int tag_msg_z=Gamestopxiaoshi_xiaosi;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_Stop,(CCObject*)tag_msg_z);
	this->removeFromParent();	 
}



void Chaonengdaoju::fillOuty(CCMenuItemImage* p,string imgfile){


	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.10f);
	p->setSelectedImage(s);
	CCSprite* n=CCSprite::create(imgfile.c_str());
	CCLayer* layer=CCLayer::create();
	layer->setContentSize(CCSize(p->getContentSize()*1.10f));
	n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	layer->addChild(n);
	p->setNormalImage(layer);
	if(p->getTag()==useButton_tag){

		string st_str=LFStrings::getValue("use");
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
void Chaonengdaoju::refreshChaondjnumber(CCObject* obj){
	 updatenumberUI();
}

void Chaonengdaoju::updatenumberUI(){
     	 
	 
	 if(GameInforEditer::getGameInfor()->getSuperToolCount()<=0){
		 //changeaptity(use_ccbt,num_Fade*0.5f);
		 use_ccbt->setVisible(false);
	 }else{
         //changeaptity(use_ccbt,num_Fade);
         use_ccbt->setVisible(true);
	 }

	 string number_str=CCString::createWithFormat(" %d",GameInforEditer::getGameInfor()->getSuperToolCount())->getCString();
     cn_lable->setString(number_str.c_str());
}