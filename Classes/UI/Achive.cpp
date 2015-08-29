#include "Achive.h"
#include "ResManager.h"
#include "font.h"
#include "LFStrings.h"
#include "GameLayerFactory.h"
#include "LevelManager.h"
#include "Uimsg.h"
#include "Myobject.h"
#include "Starmain.h"
#include "Starseneui.h"
#include "Shieldingmanger.h"
#include "Music.h"

enum Marknum{
	 
	 One_wctag=0,    //一级菜单完成
	 Two_wctag=1,    //2级菜单完成
	 One_weiwctag=2, //一级菜单未完成
	 Two_weiwctag=3, //2级菜单未完成

};

Achive::Achive(void)
{

}



Achive::~Achive(void)
{

}

bool Achive::init(){
	if(!CCLayer::init()){
		return false;
	}
	//iscontaint=true;
	m_pBg = NULL;
	size=CCDirector::sharedDirector()->getWinSize();
	int max=GameLayerFactory::getInstance()->getValidSeasonNum();
	//返回配置的成就列表
	allahives=AchievementManager::getInstance()->getDefaultAchievementList();
	//返回成就列表
	wc_achives=AchievementManager::getInstance()->getAchievementList();
	/*
	for(int i=1;i<=max;i++){

		vector<ccColor4B> clors;
		if(i==1){
           
		}else if(i==2){
		}else if(i==3){
		}else if(i==4){
		}else if(i==5){
		}
		color_maps.insert(mark_pair());

	}
	*/
	moveold=movecurent=CCPoint(0,0);
	drawAchive();
	return true;
}


void Achive::onEnter(){
	
	CCLayer::onEnter();
	setTouchEnabled(true);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Achive::changeBGColor), MSG_UI_ChANGE_BG_COLOR, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Achive::refreshlanguge), MSG_LAGUAGEAchive, NULL);
}


void Achive::onExit(){
	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Achive::refreshlanguge(CCObject* object){
	  
	  string text=LFStrings::getValue("achive");
	  data1->setString(text.c_str());
	  list->refreshlanguge();
}

void Achive::changeBGColor(CCObject* pObj)
{
	CCInteger* pId = (CCInteger*)pObj;
	Starseneui::changeBg(pId->getValue(), m_pBg);
}


void Achive::drawAchive(){
	 
	 //CCLayerGradient* layer=CCLayerGradient::create();
     //layer->setContentSize(size);

	 int id = LevelManager::getManager()->getLastSeasonId();
	 if (Starmain* pMainLayer = (Starmain*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Starseneui_tag)->getChildByTag(Starmain_tag))
	 {
		 id = pMainLayer->getTempSeaonId();
	 }
	 string back_str = CCString::createWithFormat("ZY_bg_%d.png", id)->getCString();
	 back_str = ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);

	 m_pBg=CCScale9Sprite::create(back_str.c_str());
	 m_pBg->setContentSize(size);
	 m_pBg->setPosition(CCPoint(size.width/2,size.height/2));
	 addChild(m_pBg);
     
	 string dw_bg=ResManager::getManager()->getFilePath(g_public+"CJ_bg.png");
	 CCSprite* sps=CCSprite::create(dw_bg.c_str());
	 
	 sps->setPosition(CCPoint(this->getContentSize().width/2,this->getContentSize().height*0.425f));
	 this->addChild(sps);


	 string tiao_str=ResManager::getManager()->getFilePath(g_public+"CJ_biaoti.png");
	 CCSprite* ti_sp=CCSprite::create(tiao_str.c_str());
	 ti_sp->setOpacity(150);
	 ti_sp->setPosition(CCPoint(this->getContentSize().width/2,this->getContentSize().height*0.9f));
	 this->addChild(ti_sp);

	 //achive
	 string text=LFStrings::getValue("achive");
	 data1 = CCLabelTTF::create(text.c_str(),fontStr_KlavikaBold,50);
	 data1->setPosition(CCPoint(ti_sp->getContentSize().width/2,ti_sp->getContentSize().height*0.5f));
	 ti_sp->addChild(data1);

	 float jd_ft=0.0f; 
	 if(allahives.size()>0){
		 jd_ft=wc_achives.size()/(float)allahives.size();
	 }
	 string jd_str=CCString::createWithFormat("%.0f%%",jd_ft*100)->getCString();
	 string cicle[3]={ResManager::getManager()->getFilePath(g_public+"CJ_wanchengdu_bg.png"),ResManager::getManager()->getFilePath(g_public+"CJ_wanchengdu.png")
	 ,ResManager::getManager()->getFilePath(g_public+"prent.png")};
	 CCControlPotentiometer* cs_potetion=CCControlPotentiometer::create(cicle[0].c_str(), cicle[1].c_str(),cicle[2].c_str());
	 cs_potetion->setPosition(CCPoint(this->getContentSize().width*0.1f,this->getContentSize().height*0.75f));
	 //cs_potetion->addTargetWithActionForControlEvents(this, cccontrol_selector(Achive::valueChanged), CCControlEventValueChanged);
	 cs_potetion->setValue(jd_ft);
	 cs_potetion->setTouchEnabled(false);
	 this->addChild(cs_potetion);

	 string wc_sp=ResManager::getManager()->getFilePath(g_public+"CJ_wanchengdu_zhishi.png"); 
	 CCSprite* wc_st=CCSprite::create(wc_sp.c_str());
	 wc_st->setPosition(CCPoint(cs_potetion->getContentSize().width*0.3f+cs_potetion->getPositionX()+wc_st->getContentSize().width/2,
		    cs_potetion->getPositionY()-cs_potetion->getContentSize().height/2+wc_st->getContentSize().height/2
		 ));
	 this->addChild(wc_st);

	

	 CCLabelTTF* data2 = CCLabelTTF::create(jd_str.c_str(),fontStr_KlavikaBold,40);
	 data2->setPosition(CCPoint(wc_st->getPositionX()+wc_st->getContentSize().width/2+15,cs_potetion->getPositionY()-cs_potetion->getContentSize().height/2));
	 data2->setAnchorPoint(CCPoint(0.0f,0.0f));
	 this->addChild(data2);

	 string jiaobei_str=ResManager::getManager()->getFilePath(g_public+"CJ_icon_jiangbei.png");
	 CCSprite* j_sp=CCSprite::create(jiaobei_str.c_str());
	 j_sp->setAnchorPoint(CCPoint(0.0f,0.0f));
	 j_sp->setPosition(CCPoint(data2->getPositionX()+data2->getContentSize().width+10,cs_potetion->getPositionY()-cs_potetion->getContentSize().height/2));
	 this->addChild(j_sp);


	 string zs_str=ResManager::getManager()->getFilePath(g_public+"CJ_zhuangshi.png");
	 CCSprite* top_s=CCSprite::create(zs_str.c_str());
	 top_s->setPosition(CCPoint(size.width/2,cs_potetion->getPositionY()-cs_potetion->getContentSize().height/2-18-top_s->getContentSize().height/2));
	 this->addChild(top_s);


	 CCSprite* top_x=CCSprite::create(zs_str.c_str());
	 top_x->setPosition(CCPoint(size.width/2,m_pBg->getContentSize().height*0.095f));
	 this->addChild(top_x);

	 

	 //CJ_gundongtiao_bg.png
	 string gd_sps=ResManager::getManager()->getFilePath(g_public+"CJ_gundongtiao_bg.png");
	 gd_spsrite=CCSprite::create(gd_sps.c_str());
	 gd_spsrite->setPosition(CCPoint(this->getContentSize().width/2+712/2+gd_spsrite->getContentSize().width/2+5,this->getContentSize().height*0.38f));
	 this->addChild(gd_spsrite);
     
	 string kz_str=ResManager::getManager()->getFilePath(g_public+"CJ_gundongtiao.png");
	 sp_kongzhi=CCSprite::create(kz_str.c_str());
	 sp_kongzhi->setAnchorPoint(CCPoint(0.5f,0.5f));
	 sp_kongzhi->setPosition(CCPoint(gd_spsrite->getPositionX(),gd_spsrite->getPositionY()+gd_spsrite->getContentSize().height/2-sp_kongzhi->getContentSize().height/2));
	 this->addChild(sp_kongzhi);
	 spsize=sp_kongzhi->getContentSize();
	 scaleBeishu=1.0f;
	 shangxiang_y=gd_spsrite->getPositionY()+gd_spsrite->getContentSize().height/2-spsize.height/2;
	 xianxian_y=gd_spsrite->getPositionY()-gd_spsrite->getContentSize().height/2+spsize.height/2;
	 zongjuli=shangxiang_y-xianxian_y;


	 list=Achivetablelist::create();
	 list->setMtabletype(100,CCSize(712,70),6,kCCScrollViewDirectionVertical,kCCTableViewFillTopDown,achivelistjuli);
	 list->ignoreAnchorPointForPosition(false);
	 list->setAnchorPoint(CCPoint(0.5f,0.5f));
	 list->setPosition(CCPoint(this->getContentSize().width/2,this->getContentSize().height*0.38f));
	 this->addChild(list);
	 list->setAchivelayer(this);


	 string fahui=ResManager::getManager()->getFilePath(g_xiaogk+"GQ_anniu_fanhui.png");
	 CCMenuItemImage* fh_bt=CCMenuItemImage::create(fahui.c_str(),fahui.c_str(),this,menu_selector(Achive::faihuiCallback));
	 fh_bt->setPosition(CCPoint(getContentSize().width*buton_jiangju,fh_bt->getContentSize().height/2+5));
	 
	 fillOuty(fh_bt,fahui);

	 CCMenu* menu=CCMenu::create(fh_bt,NULL);
	 menu->setPosition(CCPointZero);
	 this->addChild(menu);

}


void Achive::fillOuty(CCMenuItemImage* p,string imgfile){

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

void Achive::faihuiCallback(CCObject* ob){

	Music::sharedMusicPlayer()->buttonClicked();

	Myobject* object=new Myobject();
	object->mark=false;
	object->parent_id=Starmain_tag;
	object->current_id=Achievelayer_tag;
	object->pCurNode = this;
	object->autorelease();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE, (CCObject*)object);
}


void Achive::setSpritepostion(float num,float dex1,bool mark){
	 CCPoint p=list->getTableconsetoff();
	 float prentpoint=1.0f-abs(p.y/list->gettableHight());
	 sp_kongzhi->setPosition(CCPoint(sp_kongzhi->getPositionX(),shangxiang_y-zongjuli*prentpoint));
}
void Achive::change_sx(float oldhight,float currenthight){

	scaleBeishu=list->gettablestartHight()/currenthight;
	sp_kongzhi->setScaleY(scaleBeishu);
	shangxiang_y=gd_spsrite->getPositionY()+gd_spsrite->getContentSize().height/2-(spsize.height*scaleBeishu)/2;
	xianxian_y=gd_spsrite->getPositionY()-gd_spsrite->getContentSize().height/2+(spsize.height*scaleBeishu)/2;
	zongjuli=shangxiang_y-xianxian_y;
	CCPoint p=list->getTableconsetoff();
	float prentpoint=1.0f-abs(p.y/list->gettableHight());
	sp_kongzhi->setPosition(CCPoint(sp_kongzhi->getPositionX(),shangxiang_y-zongjuli*prentpoint));
}


bool Achive::iscontainPoint(CCPoint po){

	if(po.x<=(sp_kongzhi->getPositionX()+spsize.width/2)&&po.x>=(sp_kongzhi->getPositionX()-spsize.width/2)
		&&po.y<=(sp_kongzhi->getPositionY()+spsize.height/2)&&po.y>=(sp_kongzhi->getPositionY()-spsize.height/2)) 
	{
		return true;
	}
	return false;
}

bool Achive::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	 
	 if(Shieldingmanger::getSielding()->getCureentlayertag()!=Achievelayer_tag){
		 return false;
	 }
	 moveold=movecurent=list->getTableconsetoff();

	 return true;

}

void Achive::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

	 
	 movecurent=list->getTableconsetoff();
	 /*
	 if(iscontainPoint(pTouch->getLocation())&&moveold.y!=0){
        float dex=movecurent.y-moveold.y;
		if(dex<0){//向下
			if(sp_kongzhi->getPositionY()+dex>=xianxian_y){
				sp_kongzhi->setPosition(CCPoint(sp_kongzhi->getPositionX(),sp_kongzhi->getPositionY()+dex));
				list->setPrentlist(shangxiang_y-xianxian_y,dex,false);
			}
		}else if(dex>0){//向上
			if(sp_kongzhi->getPositionY()+dex<=shangxiang_y){
				sp_kongzhi->setPosition(CCPoint(sp_kongzhi->getPositionX(),sp_kongzhi->getPositionY()+dex));
				list->setPrentlist(shangxiang_y-xianxian_y,dex,true);
			}
		}
	 }else{
		 //moveold=movecurent=CCPoint(0,0);
	 }
	 
	 bool mark=false;//false 向上拖动  true 向下拖动
	 if(movecurent.y-moveold.y>0){
		 mark==false;
         setSpritepostion(list->gettableHight(),abs(movecurent.y-moveold.y),mark);
	 }else if(movecurent.y-moveold.y<0){
		 mark==true;
		 setSpritepostion(list->gettableHight(),abs(movecurent.y-moveold.y),mark);
	 }
*/
     moveold=movecurent;
	 
}

void Achive::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){


}

void Achive::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

     
}

void Achive::registerWithTouchDispatcher(void){
	 CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128, false);
}


void Achive::valueChanged(CCObject *sender, CCControlEvent controlEvent){

	CCControlPotentiometer* pControl = (CCControlPotentiometer*)sender;

}

void Achive::setAchivetablelist(Achivetablelist* p){
	 this->list=p;
}

//achivetablelsit

#include "ResManager.h"
Achivetablelist::Achivetablelist(void){


}

Achivetablelist::~Achivetablelist(void){

	
}

bool Achivetablelist::init(){
	 
	 if(!CCLayer::init()){
		 return false;
	 }
	 main_a=NULL;
	 //返回配置的成就列表
	 allahives=AchievementManager::getInstance()->getDefaultAchievementList();
	 //返回成就列表
	 wc_achives=AchievementManager::getInstance()->getAchievementList();
	 bg_strs[0]=ResManager::getManager()->getFilePath(g_public+"CJ_wancheng_bg.png");
	 bg_strs[1]=ResManager::getManager()->getFilePath(g_public+"CJ_wancheng_zhankai.png");
	 bg_strs[2]=ResManager::getManager()->getFilePath(g_public+"CJ_weiwancheng_bg.png");
	 bg_strs[3]=ResManager::getManager()->getFilePath(g_public+"CJ_weiwancheng_zhankai.png");
	 inintData();
	 return true;
}

void Achivetablelist::refreshlanguge(){

	mTableView->reloadData();
}

void Achivetablelist::inintData(){


	//返回配置的成就列表
	//vector<Achievement> allahives;
	//返回成就列表
	//vector<Achievement> wc_achives;

	vector<Achievement> allcs;

	for(size_t i=0;i<wc_achives.size();i++){
		allcs.push_back(wc_achives.at(i));
	}

	for(size_t i=0;i<allahives.size();i++){

		bool isnoachive=false;
		for(size_t j=0;j<wc_achives.size();j++){
			if(allahives.at(i).name.compare(wc_achives.at(j).name)==0){
				isnoachive=true;
				break;
			}
		}
		if(isnoachive==false){
			allcs.push_back(allahives.at(i));
		}
	}
	allahives.clear();
	for(size_t i=0;i<allcs.size();i++){
		allahives.push_back(allcs.at(i));
	}
	allcs.clear();
	//allahives.erase(allahives.at(0));
	 for(size_t i=0;i<allahives.size();i++){
		 for(size_t j=0;j<2;j++){
		    Changelist mlist;
			if(j==0){
                mlist.mark=true;//一级菜单
				mlist.iszk=false;
				mlist.name=allahives.at(i).name;
				mlist.describe=allahives.at(i).describe;
				mlist.filename=allahives.at(i).filename;
				tablelist.push_back(mlist);
			}else if(j==1){
				mlist.mark=false;//二级菜单
				mlist.iszk=false;
				mlist.name=allahives.at(i).name;
				mlist.describe=allahives.at(i).describe;
				mlist.filename=allahives.at(i).filename;
				tablelist.push_back(mlist);
			}
		 }
	 }


	 //
	 for(size_t i=0;i<tablelist.size();i++){
		 if(tablelist.at(i).mark==true||(tablelist.at(i).mark==false&&tablelist.at(i).iszk==true)){
			 changelist.push_back(tablelist.at(i));
		 }
	 }

}

void Achivetablelist::onEnter(){
	
	 
	 CCLayer::onEnter();
	 
}
void Achivetablelist::onExit(){
	 
	 CCLayer::onExit();
	
}


void Achivetablelist::scrollViewDidScroll(CCScrollView* view) {
	 
	
	 if(main_a&&mTableView->getContentOffset().y<=0&&mTableView->getContentOffset().y>=-hight_m){
         movecurent=mTableView->getContentOffset();
		 bool mark=false;//false 向上拖动  true 向下拖动
		 if(movecurent.y-moveold.y>0){
			 mark=false;
			 main_a->setSpritepostion(hight_m,abs(movecurent.y-moveold.y),mark);
		 }else if(movecurent.y-moveold.y<0){
			 mark=true;
			 main_a->setSpritepostion(hight_m,abs(movecurent.y-moveold.y),mark);
		 }
		 moveold=movecurent;
	 }
}

void Achivetablelist::scrollViewDidZoom(CCScrollView* view){

	

}

void Achivetablelist::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	
	int idx=(int)(long long)cell->getUserData();
	CCLOG("idx--->%d",idx);
	changeIdxbutton(idx);
}

CCSize Achivetablelist::tableCellSizeForIndex(CCTableView *table, unsigned int idx){


	if(this->isVetical==false){
		return CCSize(size.width+layerspace,size.height);
	}else{
		return CCSize(size.width,size.height+layerspace);
	}
	return size;
}

CCTableViewCell* Achivetablelist::tableCellAtIndex(CCTableView *table, unsigned int idx){


	CCTableViewCell *cell = table->dequeueCell();
	if (cell != NULL) 
	{
		cell->removeAllChildren();
	}
	else			  
	{
		cell = new CCTableViewCell();
		cell->autorelease();
	}
	addDataimg(cell,idx);
	return cell;
}

unsigned int Achivetablelist::numberOfCellsInTableView(CCTableView *table){

	int num=changelist.size();
	return num;
}

void Achivetablelist::setDataType(int type){

	//数据初始化
	this->type=type;
	

}
void Achivetablelist::setMtabletype(int type,CCSize size,int itemnum,CCScrollViewDirection direction,CCTableViewVerticalFillOrder order,int space){


	this->size=size;
	this->itemnum=itemnum;
	this->layerspace=space;
	this->setDataType(type);
	CCSize layersize;
	if(direction==kCCScrollViewDirectionVertical){
		this->isVetical=true;
		layersize.width=size.width;
		layersize.height=size.height*itemnum+space*itemnum;
	}else{
		this->isVetical=false;
		layersize.width=size.width*itemnum+space*itemnum;
		layersize.height=size.height;
	}
	this->setContentSize(layersize);
	mTableView = CCTableView::create(this,layersize);
	mTableView->setDirection(direction);
	mTableView->setPosition(CCPoint(0,0));
	mTableView->setDelegate(this);
	mTableView->setVerticalFillOrder(order);
	this->addChild(mTableView,2);
	hight_m=mTableView->getContentSize().height;
	hight_m=hight_m-(size.height+achivelistjuli)*6;
	hight_sm=hight_m;
	moveold=movecurent=mTableView->getContentOffset();
    
}

void Achivetablelist::addDataimg(CCTableViewCell *cell, unsigned int idx){
	drawAchivelist(cell,idx);
}

void Achivetablelist::drawAchivelist(CCTableViewCell *cell, unsigned int idx){

	 
	 string back_str=bg_strs[0];
	 string star_str="";
	 //ccColor4B color=ccColor4B(255,255,255,255);
	 ccColor3B color=ccc3(155,164,181); //一级菜单 默认颜色

	 ccColor3B color2=ccc3(155,164,181); //二级菜单 默认颜色
	 int mark=0;
	 if(changelist.at(idx).mark==true){
		 if(isAchivewc(changelist.at(idx).name)){
			 back_str=bg_strs[0];
			 mark=One_wctag;
			 star_str="CJ_icon_wancheng.png"; 
			 color=ccc3(255,214,35);
		 }else{
			 back_str=bg_strs[2];
			 mark=One_weiwctag;
			 star_str="CJ_icon_weiwancheng.png";
		 }
	 }else if(changelist.at(idx).mark==false){
		 if(isAchivewc(changelist.at(idx).name)){
			 back_str=bg_strs[1];
			 mark=Two_wctag;
			 color2=ccc3(234,129,238);
		 }else{
			 back_str=bg_strs[3];
			 mark=Two_weiwctag;
		 }
	 }
	 star_str=ResManager::getManager()->getFilePath(g_public+star_str);
	 if(mark==One_wctag||mark==One_weiwctag){
         

		 cell->setUserData((void*)idx);
		 CCSprite* back=CCSprite::create(back_str.c_str());
		 back->setPosition(CCPoint(size.width/2,size.height/2));
		 cell->addChild(back);

		 string ku_str="";
		 if(changelist.at(idx).iszk==false){
              ku_str="YX_jiaoxue_fanye.png";
		 }else if(changelist.at(idx).iszk==true){
			  ku_str="shou.png";
		 }
		 ku_str=ResManager::getManager()->getFilePath(g_public+ku_str);
		 CCMenuItemImage* p_kg=CCMenuItemImage::create(ku_str.c_str(),ku_str.c_str(),this,
                                             menu_selector(Achivetablelist::allCallback)
			 );
		 p_kg->setPosition(CCPoint(back->getContentSize().width*0.95f,back->getContentSize().height/2));
		 fillOuty(p_kg,ku_str);
		 p_kg->setUserData((void*)idx);

		 CCSprite* star_sps=CCSprite::create(star_str.c_str());
		 star_sps->setPosition(CCPoint(back->getContentSize().width*0.04f,back->getContentSize().height*0.5f));
		 back->addChild(star_sps);


		 //name
		 CCLabelTTF* name_achive=CCLabelTTF::create(moquduoyustring("_",changelist.at(idx).name).c_str(),fontStr_KlavikaBold,35);
		 name_achive->setColor(color);
		 name_achive->setAnchorPoint(CCPoint(0.0f,0.5f));
		 name_achive->setPosition(CCPoint(star_sps->getContentSize().width/2+5+star_sps->getPositionX(),back->getContentSize().height/2));
		 back->addChild(name_achive);


		 CCMenu* mu=CCMenu::create(p_kg,NULL);
		 mu->setPosition(CCPointZero);
		 back->addChild(mu);
         
	 }else if(mark==Two_wctag||mark==Two_weiwctag){

		 CCSprite* back=CCSprite::create(back_str.c_str());
		 back->setPosition(CCPoint(size.width-back->getContentSize().width/2,size.height/2));
		 cell->addChild(back);

		 //name
		 CCLabelTTF* name_achive=CCLabelTTF::create(changelist.at(idx).describe.c_str(),fontStr_KlavikaBold,30);
		 name_achive->setHorizontalAlignment(kCCTextAlignmentLeft);
		 name_achive->setColor(color2);
		 name_achive->setAnchorPoint(CCPoint(0.0f,0.5f));
		 name_achive->setPosition(CCPoint(back->getContentSize().width*0.08f,back->getContentSize().height/2));
		 back->addChild(name_achive);
	 }
}

void Achivetablelist::fillOuty(CCMenuItemImage* p,string imgfile){

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


void Achivetablelist::changeIdxbutton(int idx){

	bool isz=false;
	for(size_t i=0;i<tablelist.size()-1;i++){
		if(changelist.at(idx).name.compare(tablelist.at(i).name)==0&&(tablelist.at(i).mark==true)){
			if(tablelist.at(i).iszk==false){
				tablelist.at(i).iszk=true;
				tablelist.at(i+1).iszk=true;
				isz=true;
			}else if(tablelist.at(i).iszk==true){
				tablelist.at(i).iszk=false;
				tablelist.at(i+1).iszk=false;
				isz=false;
			}
			break;
		}
	}
	changelist.clear();
	for(size_t i=0;i<tablelist.size();i++){
		if(tablelist.at(i).mark==true||(tablelist.at(i).mark==false&&tablelist.at(i).iszk==true)){
			changelist.push_back(tablelist.at(i));
		}
	} 
	CCPoint point=mTableView->getContentOffset();
	int diwei=idx;
	if(isz){ //增加一项
		if(idx==changelist.size()-2){

		}else{
			point=point-CCPoint(0,size.height+achivelistjuli);
		} 
	}else{   //减少一项
		if(idx==changelist.size()-1){

		}else{
			point=point+CCPoint(0,size.height+achivelistjuli); 
		}
	}
	float oldhight=hight_m;
	mTableView->reloadData();
	mTableView->setContentOffset(point);
	hight_m=mTableView->getContentSize().height;
	hight_m=hight_m-(size.height+achivelistjuli)*6;
	main_a->change_sx(oldhight,hight_m);
}

void Achivetablelist::allCallback(CCObject* ob){
    
	 CCMenuItemImage* p_bt=(CCMenuItemImage*)ob;
	 int idx=(int)(long long)p_bt->getUserData();
	 CCLOG("idx--->%d",idx);
	 changeIdxbutton(idx);

}


void Achivetablelist::setAchivelayer(Achive* p){
	this->main_a=p;
}

bool Achivetablelist::isAchivewc(string name){


	 bool iswc=false;
	 for(size_t i=0;i<wc_achives.size();i++){
		 if(name.compare(wc_achives.at(i).name)==0){
			 iswc=true;
			 break;
		 }
	 }
	 return iswc;
}

void Achivetablelist::setPrentlist(float num,float dex,bool mark){

	 float juli=hight_m*(dex/num);
	 if(mark==false){ //向下 向上滑
		 mTableView->setContentOffset( mTableView->getContentOffset()+CCPoint(0,-juli));
	 }else if(mark==true){//向上 向上滑
		 mTableView->setContentOffset( mTableView->getContentOffset()+CCPoint(0,-juli));
	 }
	 //int m=0;
}


string Achivetablelist::moquduoyustring(string moqu,string src){

	int start=src.find(moqu); 
	while(start>=0){
			int maxlength=src.size();
			string  s2=src.substr(0,start);
			string  s3=src.substr(start+1,maxlength-start);
			string nstr=s2+s3;
			src=nstr;
			start=src.find(moqu);
	}
	return src;
}

CCPoint Achivetablelist::getTableconsetoff(){
	    return mTableView->getContentOffset();
}
float  Achivetablelist::gettableHight(){
	    return hight_m;
}
float   Achivetablelist::gettablestartHight(){
	    return hight_sm;
}


/*
void Achivetablelist::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}
*/

