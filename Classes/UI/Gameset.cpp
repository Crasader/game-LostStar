#include "Gameset.h"

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
#include "Starseneui.h"
#include "Starmain.h"

enum SetEnum
{
	luaguge_tag=10,
	yinyue_tag,
	yinxiao_tag,
	kuangzhnag_tag,
	guangyu_tag,
	faihui_button_tag,
	faihui2_tag,
	faihui3_tag,
	//left_tag,
	//right_tag,

};

Gameset::Gameset(void)
{
}


Gameset::~Gameset(void)
{
}

 /*
 kLanguageEnglish = 0,
 kLanguageChinese,
 kLanguageFrench,
 kLanguageItalian,
 kLanguageGerman,
 kLanguageSpanish,
 kLanguageDutch,
 kLanguageRussian,
 kLanguageKorean,
 kLanguageJapanese,
 kLanguageHungarian,
 kLanguagePortuguese,
 kLanguageArabic
 */

const ccLanguageType Gameset::SUPPORTED_LANGUAGE[] = 
{
	kLanguageEnglish,
	kLanguageGerman
};
const char* Gameset::SUPPORTED_LANGUAGE_NAME[] = 
{
	"English",
	"Deutsch"
};
 bool Gameset::init(){

	 if(!CCLayer::init()){
		 return false;
	 }
	 size=CCDirector::sharedDirector()->getWinSize();
	 mark_l=0;

	 _supportedLanguageNum = sizeof(SUPPORTED_LANGUAGE) / sizeof(LanguageType);
	 for( int i = 0; i < _supportedLanguageNum; i++)
	 {
		 if(ResManager::getManager()->getLanguageType() == SUPPORTED_LANGUAGE[i])
		 {
			 mark_idx=i;
			 cz_idx=i;
		 } 
	 }

	 drawSet();

	 return true;
 }  
 void Gameset::onEnter(){

	 CCLayer::onEnter();
	 CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Gameset::changeBGColor), MSG_UI_ChANGE_BG_COLOR, NULL);
 }
 void Gameset::onExit(){

	 CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	 CCLayer::onExit();
 }

 void Gameset::changeBGColor(CCObject* pObj)
 {
	 CCInteger* pId = (CCInteger*)pObj;
	 Starseneui::changeBg(pId->getValue(), mBG);
 }

 void Gameset::drawSet(){

	 set_layer=CCLayer::create();
	 set_layer->setContentSize(size);
	 addChild(set_layer);


	 int id = LevelManager::getManager()->getLastSeasonId();
	 if (Starmain* pMainLayer = (Starmain*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Starseneui_tag)->getChildByTag(Starmain_tag))
	 {
		 id = pMainLayer->getTempSeaonId();
	 }
	 string back_str = CCString::createWithFormat("ZY_bg_%d.png", id)->getCString();
	 back_str = ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);

	 mBG = CCScale9Sprite::create(back_str.c_str());
	 mBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
	 mBG->setPosition(CCDirector::sharedDirector()->getWinSize()/2);
	 this->addChild(mBG, -2);

	 const float startY = size.height * 0.52f;

	 string set_laguage=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yuyanqiehuan.png");
	 CCMenuItemImage* yuyao_set=CCMenuItemImage::create(
		 set_laguage.c_str(),
		 set_laguage.c_str(),
		 this,
		 menu_selector(Gameset::allCallback));
	 yuyao_set->setTag(luaguge_tag);
	 yuyao_set->setPosition(CCPoint(size.width * 0.2f, startY));
	 fillOuty( yuyao_set,set_laguage,false,0);
     
	 //音乐
	 string yiny="";//"ZT_anniu_yinyue.png";
	 string yiny_select="";//"ZT_anniu_yinyueguanbi.png";
	 bool yiny_y=Music::sharedMusicPlayer()->isBackgroundMute();
	 if(yiny_y==true){
		 yiny_select=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue.png");
		 yiny=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue_guan.png");
	 }else if(yiny_y==false){
		 yiny_select=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue_guan.png");
		 yiny=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue.png");
	 }
	 //string yinyue=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue.png");
	 //string yinyue_s=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue_guan.png");
	 CCMenuItemImage* yy_set=CCMenuItemImage::create(
		 yiny.c_str(),
		 yiny.c_str(),
		 this,
		 menu_selector(Gameset::allCallback));
	 yy_set->setTag(yinyue_tag);
	 yy_set->setPosition(CCPoint(size.width * 0.4f, startY));
	 fillOuty(yy_set,yiny,false,0);


	 //音效
	 string yinxiao="";//"ZT_anniu_yinxiao.png";
	 string yinxiao_select="";//"ZT_anniu_yinxiao_guanbi.png";
	 bool kg_yx= Music::sharedMusicPlayer()->isEffectiveMute();
	 if(kg_yx==true){
		 yinxiao=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao_guan.png");
		 yinxiao_select=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao.png");
	 }else{
		 yinxiao=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao.png");
		 yinxiao_select=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao_guan.png");
	 }
	 //string yinxiao=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao.png");
	 //string yinxiao_s=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao_guan.png");
	 CCMenuItemImage* yinxiao_b=CCMenuItemImage::create(
		 yinxiao.c_str(),
		 yinxiao.c_str(),
		 this,
		 menu_selector(Gameset::allCallback));
	 yinxiao_b->setPosition(CCPoint(size.width * 0.6f, startY));
	 yinxiao_b->setTag(yinxiao_tag);
	 fillOuty(yinxiao_b,yinxiao,false,0);
	 string kz_file=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_kuozhan.png");
#if 0
	 //kz
	 CCMenuItemImage* kz_bt=CCMenuItemImage::create( 
		 kz_file.c_str(),
		 kz_file.c_str(),
		 this,
		 menu_selector(Gameset::allCallback));
	 kz_bt->setTag(kuangzhnag_tag);
	 kz_bt->setPosition(CCPoint(size.width*0.5f-size.width*0.13f,size.height*0.40));
	 fillOuty(kz_bt,kz_file,false,0);
#endif
	 //guangyu 关于
	 string gy_file=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_info.png");
	 CCMenuItemImage* gy_bt=CCMenuItemImage::create( 
		 gy_file.c_str(),
		 gy_file.c_str(),
		 this,
		 menu_selector(Gameset::allCallback));
	 gy_bt->setTag(guangyu_tag);
	 gy_bt->setPosition(CCPoint(size.width*0.8f, startY));
	 fillOuty(gy_bt,gy_file,false,0);
	 //返回按钮

	 string f_hui_file=ResManager::getManager()->getFilePath(g_mainlayerPath+"SZ_yuyanqiehuan_guanbi.png");
	 CCMenuItemImage* ret_bt=CCMenuItemImage::create( 
		 f_hui_file.c_str(),
		 f_hui_file.c_str(),
		 this,
		 menu_selector(Gameset::allCallback));

	 ret_bt->setTag(faihui_button_tag);
	 ret_bt->setPosition(CCPoint(getContentSize().width*buton_jiangju,ret_bt->getContentSize().height/2+5));
	 fillOuty(ret_bt,f_hui_file,false,0);

     
	 CCMenu* pMenu = CCMenu::create(yuyao_set,yy_set,yinxiao_b,gy_bt,ret_bt,NULL);
	 pMenu->setPosition(CCPointZero);
	 set_layer->addChild(pMenu, 1);



	  
 }

 /*

 {
 "key":"setyuyao",
 "value":"语言设置"
 },
 {
 "key":"setype",
 "value":"中文"
 }

 */

 void Gameset::drawLuaguage(){

	  if(mark_l==0){

		  lag_layer=CCLayer::create();
		  lag_layer->setContentSize(size);
		  addChild(lag_layer);
          
		  string tubiao=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yuyanqiehuan.png");
		  CCSprite* t_p=CCSprite::create(tubiao.c_str());
		  t_p->setPosition(CCPoint(size.width/2,size.height*0.85f));
		  lag_layer->addChild(t_p);


		  string fahui_img=ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"SZ_yuyanqiehuan_guanbi.png");

		  CCMenuItemImage* p_faihui=CCMenuItemImage::create(
			                fahui_img.c_str(),
							fahui_img.c_str(),
							this, menu_selector(Gameset::allCallback));
		  p_faihui->setTag(faihui2_tag);
		  p_faihui->setPosition(CCPoint(getContentSize().width*buton_jiangju,p_faihui->getContentSize().height/2+5));
		  fillOuty(p_faihui,fahui_img,false,0);

		  string fy_str=LFStrings::getValue("setyuyao");
		  fy_label = CCLabelTTF::create(fy_str.c_str(),fontStr_KlavikaBold,30);
		  fy_label->setPosition(CCPoint(size.width*0.5,size.height*0.75f));
		  fy_label->setOpacity(128);
		  lag_layer->addChild(fy_label);
		  fy_label->setColor(ccc3(134,181,208));

		  fy_label->setVisible(false);//屏蔽

		  _lanItemMarker = CCSprite::create(ResManager::getManager()->getFilePath(g_public+"SZ_yuyan_kuang_select_icon.png").c_str());
		  lag_layer->addChild(_lanItemMarker);

		  float startY = fy_label->getPosition().y - fy_label->getContentSize().height * 2;
		  CCArray* arr = CCArray::create();
		  _curSelectLanguageItem = NULL;
		  for( int i = 0; i < _supportedLanguageNum; i++)
		  {

			  //语言框
			  string yuyao=ResManager::getManager()->getFilePath(g_public+"SZ_yuyan_kuang.png");

			  CCScale9Sprite* spscale9=CCScale9Sprite::create(yuyao.c_str());
			  
			  type_label = CCLabelTTF::create(SUPPORTED_LANGUAGE_NAME[i],fontStr_KlavikaBold, 30);
			  type_label->setPosition(CCPoint(spscale9->getContentSize().width/2,spscale9->getContentSize().height/2));
			  spscale9->addChild(type_label);
			  type_label->setOpacity(90.0f);
			  spscale9->setOpacity(90.0f);

			  CCScale9Sprite* spscale9_s=CCScale9Sprite::create(yuyao.c_str());
			  type_label = CCLabelTTF::create(SUPPORTED_LANGUAGE_NAME[i],fontStr_KlavikaBold, 30);
			  type_label->setPosition(CCPoint(spscale9_s->getContentSize().width/2,spscale9_s->getContentSize().height/2));
			  spscale9_s->addChild(type_label);


			  CCMenuItemImage* item = CCMenuItemImage::create();
			  item->initWithNormalSprite(spscale9, spscale9_s, NULL, this, menu_selector(Gameset::onSelectLanguage));
			  item->setPosition(CCPoint(size.width/2, startY));
			  item->setTag(i);
			  arr->addObject(item);

			  if(i == cz_idx)
			  {
				  item->selected();
				  _curSelectLanguageItem = item;
				  focusLanguageBar(item);
				 // changeLanguage(i);
			  } 
			  
			  startY -= spscale9->getContentSize().height + 5.0f;
		  }
		  arr->addObject(p_faihui);
		  CCMenu* pMenu = CCMenu::createWithArray(arr);
		  pMenu->setPosition(CCPointZero);
		  lag_layer->addChild(pMenu);

	  }else{
         


	  }
	  
	  lag_layer->setVisible(true);
	  

 }

 void Gameset::focusLanguageBar(CCMenuItemImage* bar)
 {
	 CCSize barSize = bar->getContentSize();
	 CCPoint pos = bar->getPosition() - ccp(barSize.width/2 + _lanItemMarker->getContentSize().width/2 + 5, 0);
	 _lanItemMarker->setPosition(pos);
 }

 void Gameset::changeLanguage(int idx)
 {
	 ResManager::getManager()->setLanguageType(SUPPORTED_LANGUAGE[idx]);
	 ResManager::getManager()->reloadResource();

	 string fy_str=LFStrings::getValue("setyuyao");
	 fy_label->setString(fy_str.c_str());
 }

 void Gameset::onSelectLanguage(CCObject* object)
 {
	 if(object == _curSelectLanguageItem || object == NULL)
	 {
		 if(_curSelectLanguageItem != NULL)
		 {
			 _curSelectLanguageItem->selected();
		 }
		 return;
	 }

	 if(_curSelectLanguageItem)
	 {
		 _curSelectLanguageItem->unselected();
	 }

	 _curSelectLanguageItem = (CCMenuItemImage*)object;
	 _curSelectLanguageItem->selected();
	 focusLanguageBar(_curSelectLanguageItem);

	 //设置选择的语言索引，不然会导致每次进语言设置界面默认选择的是English的问题
	 cz_idx = _curSelectLanguageItem->getTag();
	 changeLanguage(cz_idx);

	 Music::sharedMusicPlayer()->buttonClicked();
 }

 void Gameset::drawAbout(){

	  if(mark_l==0){

		  about_layer=CCLayer::create();
		  about_layer->setContentSize(size);
		  addChild(about_layer);


		  string tubiao=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_biaoti_zhubiaoti.png", true);
		  CCSprite* t_p=CCSprite::create(tubiao.c_str());
		  t_p->setPosition(CCPoint(size.width/2,size.height*0.8f));
		  t_p->setScale(0.5);
		  about_layer->addChild(t_p);

		  string yy_str=LFStrings::getValue("producedby");
		  type_label = CCLabelTTF::create(yy_str.c_str(),fontStr_KlavikaBold,30);
		  type_label->setPosition(CCPoint(about_layer->getContentSize().width/2,about_layer->getContentSize().height * 0.6f));
		  about_layer->addChild(type_label);

		  yy_str=LFStrings::getValue("companyname");
		  type_label = CCLabelTTF::create(yy_str.c_str(),fontStr_KlavikaBold,60);
		  type_label->setPosition(CCPoint(about_layer->getContentSize().width/2,about_layer->getContentSize().height * 0.5f));
		  about_layer->addChild(type_label);
		  
		  yy_str=LFStrings::getValue("copyrightinfo");
		  type_label = CCLabelTTF::create(yy_str.c_str(),fontStr_KlavikaBold,22);
		  type_label->setPosition(CCPoint(about_layer->getContentSize().width/2,about_layer->getContentSize().height * 0.42f));
		  about_layer->addChild(type_label);


		 string fahui_img =ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"SZ_yuyanqiehuan_guanbi.png");

		  CCMenuItemImage* p_faihui=CCMenuItemImage::create(
			  fahui_img.c_str(),
			  fahui_img.c_str(),
			  this,
			  menu_selector(Gameset::allCallback));
		  p_faihui->setTag(faihui3_tag);
		  p_faihui->setPosition(CCPoint(getContentSize().width*buton_jiangju,p_faihui->getContentSize().height/2+5));
		  fillOuty(p_faihui,fahui_img,false,0);
		  CCMenu* pMenu = CCMenu::create(p_faihui, NULL);
		  pMenu->setPosition(CCPointZero);
		  about_layer->addChild(pMenu, 1);
	  }else{
         


	  }

	  about_layer->setVisible(true);
	  

 }

 void Gameset::allCallback(CCObject* object){

	 /*
	 yinxiao_tag,
	 kuangzhnag_tag,
	 guangyu_tag,
	 */
	  Music::sharedMusicPlayer()->buttonClicked();
	  CCMenuItemImage* p=(CCMenuItemImage*)object;
 	  int tag=p->getTag();

	  if(tag==luaguge_tag){

		  set_layer->setVisible(false);
		  drawLuaguage();

	  }else if(tag==yinyue_tag){

		  string yiny=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue.png");
		  string yiny_select=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinyue_guan.png");
		  bool yiny_y=Music::sharedMusicPlayer()->isBackgroundMute();
		  if(yiny_y==true){
			  p->setNormalImage(CCSprite::create(yiny.c_str()));
			  fillOuty(p,yiny,false,0);
			  Music::sharedMusicPlayer()->setBackgroundMute(false);
			  GameInforEditer::getGameInfor()->saveSessonInforList();
		  }else{
			  p->setNormalImage(CCSprite::create(yiny_select.c_str()));
			  fillOuty(p,yiny_select,false,0);
			  Music::sharedMusicPlayer()->setBackgroundMute(true);
			  GameInforEditer::getGameInfor()->saveSessonInforList();

		  }
	  }else if(tag==yinxiao_tag){

		  //音效
		  string yinxiao=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao.png");
		  string yinxiao_select=ResManager::getManager()->getFilePath(g_public+"SZ_anniu_yinxiao_guan.png");
		  bool kg_yx= Music::sharedMusicPlayer()->isEffectiveMute();
		  if(kg_yx==true){
			  p->setNormalImage(CCSprite::create(yinxiao.c_str()));
			  fillOuty(p,yinxiao,false,0);
			  Music::sharedMusicPlayer()->setEffectiveMute(false);
			   GameInforEditer::getGameInfor()->saveSessonInforList();
		  }else{
			  p->setNormalImage(CCSprite::create(yinxiao_select.c_str()));
			  fillOuty(p,yinxiao_select,false,0);
			  Music::sharedMusicPlayer()->setEffectiveMute(true);
			   GameInforEditer::getGameInfor()->saveSessonInforList();
		  }
	  }else if(tag==kuangzhnag_tag){


	  }else if(tag==guangyu_tag){
		  set_layer->setVisible(false);
		  drawAbout();
	  }else if(tag==faihui_button_tag){
		  Myobject* object=new Myobject();
		  object->mark=false;
		  object->pCurNode = this;
		  object->parent_id = Starmain_tag;
		  object->current_id = Gamesetlayer_tag;
		  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ChANGE,(CCObject*)object);
		  //从新刷新语言
		  //MSG_LAGUAGEMAIN
		  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LAGUAGEMAIN,(CCObject*)"0");
		  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LAGUAGEAchive,(CCObject*)"0");

	  }else if(tag==faihui2_tag){

		  lag_layer->setVisible(false);
		  set_layer->setVisible(true);
	  }
	  else if(tag == faihui3_tag)
	  {
		  about_layer->setVisible(false);
		  set_layer->setVisible(true);
	  }
#if 0
	  else if(tag==left_tag){

		  p->runAction(CCSequence::create(
			           CCScaleTo::create(0.02f,1.1f,1.1f),
					   CCScaleTo::create(0.02f,1.0f,1.0f),
					   NULL
			  ));
		 cz_idx=cz_idx-1;
		 setLuangugeByid(SUPPORTED_LANGUAGE[cz_idx]);

		 ResManager::getManager()->reloadResource();

		 string fy_str=LFStrings::getValue("setyuyao");
		 string yy_str=LFStrings::getValue("setype");
		 type_label->setString(yy_str.c_str());
		 fy_label->setString(fy_str.c_str());

		 setIsyingchang();

           
	  }else if(tag==right_tag){
		  
		  p->runAction(CCSequence::create(
			  CCScaleTo::create(0.02f,1.1f,1.1f),
			  CCScaleTo::create(0.02f,1.0f,1.0f),
			  NULL
			  ));
		  cz_idx=cz_idx+1;
		  
		  setLuangugeByid(SUPPORTED_LANGUAGE[cz_idx]);
		  ResManager::getManager()->reloadResource();
		  string fy_str=LFStrings::getValue("setyuyao");
		  string yy_str=LFStrings::getValue("setype");
		  type_label->setString(yy_str.c_str());
		  fy_label->setString(fy_str.c_str());
		  setIsyingchang();
	  }
#endif

 }

 void Gameset::fillOuty(CCMenuItemImage* p,string imgfile,bool scale=false,float dt=0.0f){


	 CCSprite* s=CCSprite::create(imgfile.c_str());
	 s->setScale(1.15f);
	 p->setSelectedImage(s);
	 CCSprite* n=CCSprite::create(imgfile.c_str());
	 CCLayer* layer=CCLayer::create();
	 layer->setContentSize(CCSize(p->getContentSize()*1.15f));
	 n->setPosition(CCPoint(layer->getContentSize().width/2,layer->getContentSize().height/2));
	 layer->addChild(n);
	 p->setNormalImage(layer);
	 if(scale==true){
		 s->setRotation(dt);
		 n->setRotation(dt);
	 }
 }


#if 0
 //
 //得到当前语言的位置是否在极值处

 bool Gameset::isetLeftbutton(){

	 if(cz_idx == 0){
		 return true;
	 }   
	 return false;
 }

 bool Gameset::isrightbutton(){

	 if(cz_idx == _supportedLanguageNum - 1){
		 return true;
	 }
	 return false;
 }


 void Gameset::setIsyingchang(){

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

 void Gameset::setLuangugeByid(int id){

	  

	 switch (id)
	 {
	 case kLanguageEnglish:
		  ResManager::getManager()->setLanguageType(kLanguageEnglish);
		 break;
	 case kLanguageChinese:
		 ResManager::getManager()->setLanguageType(kLanguageChinese);
		 break;
	 case kLanguageFrench:
		 ResManager::getManager()->setLanguageType(kLanguageFrench);
		 break;
	 case kLanguageGerman:
		 ResManager::getManager()->setLanguageType(kLanguageGerman);
		 break;
	 case kLanguageItalian:
		 ResManager::getManager()->setLanguageType(kLanguageItalian);
		 break;
	 case kLanguageRussian:
		  ResManager::getManager()->setLanguageType(kLanguageRussian);
		 break;
	 case kLanguageSpanish:
		ResManager::getManager()->setLanguageType(kLanguageSpanish);
		 break;
	 case kLanguageJapanese:
		 ResManager::getManager()->setLanguageType(kLanguageJapanese);
		 break;
	 case kLanguageKorean:
		ResManager::getManager()->setLanguageType(kLanguageKorean);
		 break;
	 case kLanguagePortuguese:
		 ResManager::getManager()->setLanguageType(kLanguagePortuguese);
		 break;
	 case kLanguageHungarian:
		 ResManager::getManager()->setLanguageType(kLanguageHungarian);
		 break;
	 case kLanguageArabic:
		ResManager::getManager()->setLanguageType(kLanguageArabic);
		 break;
	 }
 }
#endif