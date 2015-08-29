#include "Tanchuk.h"
#include "LFStrings.h"
#include "ResManager.h"
#include "font.h"
#include "Uimsg.h"

enum Tanchknum{

	 Close_tag=100,
};

Tanchuk::Tanchuk(void)
{
}


Tanchuk::~Tanchuk(void)
{

}

Tanchuk*  Tanchuk::creteTag(int tag){

	Tanchuk* pRet=new Tanchuk();
	pRet->tag_type=tag;
	if(pRet&&pRet->init()){
		pRet->autorelease(); 
		return pRet; 
	}else{
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}

}

bool Tanchuk::init(){

	if(!CCLayer::init()){
		return false;
	}
	size=CCDirector::sharedDirector()->getWinSize();
	if(tag_type==NonetWork_tag){
		drawNonetWork();
	}
	return true;
}
void Tanchuk::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-129,true);
}
void Tanchuk::onExit(){

	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



void Tanchuk::drawNonetWork(){

	 
	ccColor4B mr;
	mr.r=0;mr.g=0;mr.b=0;mr.a=250*0.3f;
	CCLayerColor* color = CCLayerColor::create(mr);
	color->setContentSize(size);
	color->ignoreAnchorPointForPosition(false);
	color->setPosition(CCPoint(size.width/2,size.height/2));
	color->setZOrder(10);
	addChild(color); 

	CCLayer* layer=CCLayer::create();
    layer->setContentSize(size);
	color->addChild(layer);


	//needwork
	string str_net=LFStrings::getValue("needwork");
	CCLabelTTF* tishi = CCLabelTTF::create(str_net.c_str(),fontStr_KlavikaBold,60);
	


	string bg=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang.png");
	CCSprite* tmp = CCSprite::create(bg.c_str());
	CCSize size2 = tmp->getContentSize();
	CCRect fullRect = CCRectMake(0,0, size2.width, size2.height);
	CCRect insetRect = CCRectMake(size2.width/2-1,size2.height*0.8f, 2, 2);
	CCScale9Sprite* bg_sc=CCScale9Sprite::create(bg.c_str(),fullRect,insetRect);

	bg_sc->setContentSize(CCSize(size.width*0.5,size.height*0.5f));
	bg_sc->setPosition(CCPoint(size.width/2,size.height/2));

	layer->addChild(bg_sc);
	string xius_backs=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_biaoti.png");
	CCSprite* tmp1 = CCSprite::create(xius_backs.c_str());
	CCSize si = tmp1->getContentSize();
	CCRect fullRect1 = CCRectMake(0,0, si.width, si.height);
	CCRect insetRect1 = CCRectMake(si.width/2-1, si.height*0.8f,2, 2);
    
	CCScale9Sprite* xs_sp=CCScale9Sprite::create(xius_backs.c_str(),fullRect1,insetRect1);
	xs_sp->setContentSize(CCSize(tishi->getContentSize().width+40,tishi->getContentSize().height+10));
	xs_sp->setPosition(CCPoint(xs_sp->getContentSize().width/2,bg_sc->getContentSize().height-xs_sp->getContentSize().height/2-10));
	bg_sc->addChild(xs_sp);

	
	tishi->setPosition(CCPoint(xs_sp->getContentSize().width/2,xs_sp->getContentSize().height/2));
	xs_sp->addChild(tishi);

	
	string close_str=ResManager::getManager()->getFilePath(g_public+"ZT_anniu_guanbi.png");
	CCMenuItemImage* close_bt=CCMenuItemImage::create(close_str.c_str(),close_str.c_str(),this,
		menu_selector(Tanchuk::allcallback));
	close_bt->setTag(Close_tag);
	close_bt->setPosition(CCPoint(bg_sc->getContentSize().width-close_bt->getContentSize().width,bg_sc->getContentSize().height-close_bt->getContentSize().height/2));
	fillOuty(close_bt,close_str);

	
	CCMenu* menu=CCMenu::create(close_bt,NULL);
	menu->setPosition(CCPointZero);
	menu->setTouchPriority(-130);
	bg_sc->addChild(menu);
	
}


void Tanchuk::allcallback(CCObject* object){


	CCMenuItemImage* p=(CCMenuItemImage*)object;
	int tag=p->getTag();
	if(tag==Close_tag){
	   this->removeFromParent();
	}

}

//公用函数
void Tanchuk::fillOuty(CCMenuItemImage* p,string imgfile){

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

bool Tanchuk::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

	return true;
}