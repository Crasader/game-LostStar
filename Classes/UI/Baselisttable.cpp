#include "Baselisttable.h"
#include "Debug.h"
#include "VersionControl.h"
#include "font.h"
#include "LFStrings.h"
#include "ResManager.h"
#include "UserDataManager.h"
#include "GameLayerFactory.h"
#include "LevelManager.h"
#include "Myobject.h"
#include "urlSprite.h" 
#include "History.h"
#include "Guankamsg.h"
#include "MessageBar.h"
#include "Music.h"

Baselisttable::Baselisttable(void)
{
}


Baselisttable::~Baselisttable(void)
{
}


bool Baselisttable::init(){

	if(!CCLayer::init()){
		 return false;
	}
	return true;
}
void Baselisttable::onEnter(){

	CCLayer::onEnter();
}
void Baselisttable::onExit(){

	CCLayer::onExit();
	if(this->type==Friendlist){
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NetMessage::MSG_PraiseSolution);
	}
	if(this->type==Friendlist){
		if(mark_sns==true){
			CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NetMessage::MSG_GotSeasonSocialInfo);
		}
	}
}


void Baselisttable::scrollViewDidScroll(CCScrollView* view) {


}

void Baselisttable::scrollViewDidZoom(CCScrollView* view){
	CCPoint p=view->getPosition();
}

 void Baselisttable::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	  CCLOG("%s------>>touch", __FUNCTION__);
// 	  if (this->getTag() == Guankamsg::list_tag)
// 	  {
// 		  if (Guankamsg* pguankamsg = dynamic_cast<Guankamsg*>(this->getParent()->getParent()->getParent()))
// 		  { 
// 			  if (pguankamsg->getIsPlayRecord())
// 			  {
// 				  return;
// 			  }
// 		  }
// 	  }
	  if(type==Chaondaoju){


	  }else if(type==Friendlist){


		  int idx=(int)(long long)cell->getUserData();

		  if(idx == dataUnion.size())
		  {
			  onInviteClick(NULL);
		  }
		  else
		  {
			  Music::sharedMusicPlayer()->buttonClicked();
			  result_idx=idx;
			  Myobject* object=new Myobject();
			  object->data=dataUnion.at(idx);
			  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FRIND_PLAY,(CCObject*)object);
		  }

	  }else if(type==Xiaogktable){

        
	  }else if(type==Xiaogkfriendlist){

		   Music::sharedMusicPlayer()->buttonClicked();
		   int idx=(int)(long long)cell->getUserData();
		   Myobject* object=new Myobject();
		   object->current_id=idx;
		   CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FRINDGKMSG_PLAY,(CCObject*)object);

		   if(this->parentlayerTag==ACountlayer_tag){

			   int idx=(int)(long long)cell->getUserData();
			   Myobject* object=new Myobject();
			   object->current_id=idx;
			   CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FRINDGKMSG_ACOUNTPLAY,(CCObject*)object);
		   }

	  }


 }

 CCSize Baselisttable::tableCellSizeForIndex(CCTableView *table, unsigned int idx){

	 
	 if(this->isVetical==false){
		 return CCSize(size.width+layerspace,size.height);
	 }else{
		 return CCSize(size.width,size.height+layerspace);
	 }
	 

	 return size;
 }

 CCTableViewCell* Baselisttable::tableCellAtIndex(CCTableView *table, unsigned int idx){

	  
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

	 if(type == Friendlist && idx == dataUnion.size())
	 {
		 cell->setUserData((void*)idx);
		 createInviteButton(cell);
	 }
	 else
	 {
		 addDataimg(cell,idx);
	 }
	 return cell;
 }

 CCMenu* Baselisttable::createInviteButton(CCNode* parent)
 {
	 string yao_str=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_yaoqing.png");
	 string st_str=LFStrings::getValue("invitemore");
	 
	 CCSprite* itemSpr = CCSprite::create(yao_str.c_str());

	 CCLabelTTF* label =CCLabelTTF::create(LFStrings::getValue("yaoqin").c_str(),fontStr_KlavikaBold, 30);
	 label->setPosition(CCPoint(itemSpr->getContentSize().width/2 + 50,  itemSpr->getContentSize().height/2));
	 itemSpr->addChild(label);

	 CCLabelTTF* label1 =CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,30);
	 label1->setPosition(CCPoint(itemSpr->getContentSize().width/2,  -itemSpr->getContentSize().height/2));
	 itemSpr->addChild(label1);

	 CCSprite* itemSpr2 = CCSprite::create(yao_str.c_str());
	 CCLabelTTF* label2 =CCLabelTTF::create(st_str.c_str(),fontStr_KlavikaBold,30);
	 label2->setPosition(CCPoint(itemSpr2->getContentSize().width/2, -itemSpr2->getContentSize().height/2));
	 itemSpr2->addChild(label2);
	 itemSpr2->setAnchorPoint(ccp(0.5f, 0.5f));
	 itemSpr2->setColor(ccc3(200, 200, 200));


	 CCMenuItemImage* item = CCMenuItemImage::create();
	 item->initWithNormalSprite(itemSpr, itemSpr2, NULL, this, menu_selector(Baselisttable::onInviteClick));
	 CCMenu* menu = CCMenu::create(item, NULL);

	 menu->setTouchPriority(kCCMenuHandlerPriority-1);
	 menu->setPosition(ccp(size.width/2,size.height/2));
	 parent->addChild(menu);

	 return menu;
 }

 void Baselisttable::onInviteClick(CCObject* obj)
 {
	 Music::sharedMusicPlayer()->buttonClicked();
	 if (!CmdHelper::getInstance()->isLogin())
	 {
		 CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Baselisttable::loginResult), NetMessage::MSG_LoginResult, NULL);
		 CmdHelper::getInstance()->login();
	 }
	 else
	 {
		 CmdHelper::getInstance()->inviteFriends();
	 }
 }

 void Baselisttable::loginResult(CCObject* pMsg)
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
	 //CCMessageBox("Login Failed", "Tip");
 }

 unsigned int Baselisttable::numberOfCellsInTableView(CCTableView *table){

	 int num=0;

	 if(type==Chaondaoju){
		 num=2;
	 }else if(type==Friendlist){
		 num=dataUnion.size() + 1; // +1 for invitation button

	 }else if(type==Xiaogktable){
		 if(season_num%8==0){
			 num=season_num/8;
		 }else{
			 num=season_num/8+1;
		 }
	 }else if(type==Xiaogkfriendlist){
		 num=users.size();
	 }else if(type==Yaoqingfriendlist){

		 num=allusers.size();
		 if(num%2==0){
			 num=allusers.size()/2;
		 }else{
			 num=allusers.size()/2+1;
		 }
	 }
	 return num;
 }



 void Baselisttable::setDataType(int type){

	  //数据初始化
	  this->type=type;
	  if(type==Chaondaoju){
		  
	  }else if(type==Friendlist){
		  

		  CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Baselisttable::click_zan), NetMessage::MSG_PraiseSolution, NULL);
		  mark_sns=false;
		  result_idx=0;
		  failresult_idx=0;
	  }else if(type==Xiaogktable){

		  season_num=GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(this->sectionID);
		  SeasonInfor* s_msg=GameInforEditer::getGameInfor()->getSeasonInfor(this->sectionID);
		  if(s_msg){
		      msg_setion=s_msg->getAllSectionInfors();
			  CCLOG("hehe--->%d",msg_setion.size());
		  }
	  }else if(type==Xiaogkfriendlist){




	  }else if(type==Yaoqingfriendlist){


	  }
	  
 }



 void Baselisttable::setMtabletype(int type,CCSize size,int itemnum,CCScrollViewDirection direction,CCTableViewVerticalFillOrder order,int space){

	 /*
	 typedef enum {
	 kCCScrollViewDirectionNone = -1,
	 kCCScrollViewDirectionHorizontal = 0,
	 kCCScrollViewDirectionVertical,
	 kCCScrollViewDirectionBoth
	 } CCScrollViewDirection;
	 */

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
	 mTableView->setTouchPriority(kCCMenuHandlerPriority-1);
	 mTableView->setDirection(direction);
	 mTableView->setPosition(CCPoint(0,0));
	 mTableView->setDelegate(this);
	 mTableView->setVerticalFillOrder(order);
	 this->addChild(mTableView,2);
	 //mTableView->setContentOffset(CCPoint(-layersize.width*2,0));
     

 }


 void Baselisttable::addDataimg(CCTableViewCell *cell, unsigned int idx){

      if(type==Chaondaoju){
		  drawChaondju(cell,idx);
	  }else if(type==Friendlist){
		  drawFriendlist(cell,idx);
	  }else if(type==Xiaogktable){
		  drawXiaogklist(cell,idx);
	  }else if(type==Xiaogkfriendlist){ //Xiaogkfriendlist
		  drawSnsfriendlist(cell,idx);
	  }else if(type==Yaoqingfriendlist){
		  drawYaoqingfriend(cell,idx);
	  }
 }


 //布局

 void Baselisttable::drawChaondju(CCTableViewCell *cell, unsigned int idx){



	  //530 80
	  string back_file=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_2.png"); 
	  CCSprite* back=CCSprite::create(back_file.c_str());
      back->setPosition(CCPoint(size.width/2, size.height/2));
	  cell->addChild(back);
	 
	  //图标 从配置里面读
	  string tb_file=getBytypeandfile(idx);

	  CCSprite* daoju_sp=CCSprite::create(tb_file.c_str());
	  daoju_sp->setPosition(CCPoint(daoju_sp->getContentSize().width/2,back->getContentSize().height/2));
	  back->addChild(daoju_sp);


	  int num=getBytypeandidnum(idx);
	  //数量
	  string num_str=CCString::createWithFormat("X%d",num)->getCString();

	  CCLabelTTF* num_label = CCLabelTTF::create(num_str.c_str(),fontStr_KlavikaBold,40);
	  num_label->setColor(ccc3(138,207,255));
	  num_label->setPosition(CCPoint(back->getContentSize().width*0.25,back->getContentSize().height/2));
	  back->addChild(num_label);
	  //购买按钮

	  string buy_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_goumai.png");
	  CCMenuItemImage* buy_ccbt=CCMenuItemImage::create(buy_bt.c_str(),buy_bt.c_str(),this,
		  menu_selector(Baselisttable::allCallback_buy));
	  buy_ccbt->setUserData((void*)idx);
	  buy_ccbt->setPosition(CCPoint(back->getContentSize().width-buy_ccbt->getContentSize().width,back->getContentSize().height*0.5));
      fillOuty(buy_ccbt,buy_bt);


	  CCMenu* pMenu = CCMenu::create(buy_ccbt,NULL);
	  pMenu->setTouchPriority(kCCMenuHandlerPriority-1);
	  pMenu->setPosition(CCPointZero);
	  back->addChild(pMenu, 1);
      
	  /*
	  string shuoming=LFStrings::getValue("buy");
	  CCLabelTTF* shuom = CCLabelTTF::create(shuoming.c_str(),fontStr_KozukaGothicProEL,50);
	  shuom->setColor(ccc3(0,0,0));//ccc3(138,207,255)
	  shuom->setPosition(CCPoint(buy_ccbt->getContentSize().width*0.5+20,buy_ccbt->getContentSize().height/2));
	  buy_ccbt->addChild(shuom);
	  */

	  string money_num=getBylauguageMoney(idx);
	  CCLabelTTF* my_txt = CCLabelTTF::create(money_num.c_str(),fontStr_KlavikaBold,30);
	  my_txt->setPosition(CCPoint(buy_ccbt->getPositionX()+buy_ccbt->getContentSize().width/2+my_txt->getContentSize().width/2+5,back->getContentSize().height/2));
	  back->addChild(my_txt);

 }
 /*
 void Baselisttable::setFriendmethod (Friendmethod* fp){

	  this->friendlist=fp;
 }
 */
 void Baselisttable::drawFriendlist(CCTableViewCell *cell, unsigned int idx){

	  
	 cell->setUserData((void*)idx);
	 //530 80
	 string back_file=ResManager::getManager()->getFilePath(g_stop+"ZT_tanchukuang_2.png"); 
	 CCSprite* back=CCSprite::create(back_file.c_str());
	 back->setPosition(CCPoint(size.width/2, size.height/2));
	 cell->addChild(back);
	 //头像 名字 播放次数 点赞次数
	 //图标
	 string tb_file=ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png");
	 urlSprite* toux_sp=urlSprite::create(tb_file.c_str(),dataUnion.at(idx).pictrue.c_str());
	 back->addChild(toux_sp);

	 toux_sp->setScaleX(back->getContentSize().height/toux_sp->getContentSize().width);
	 toux_sp->setScaleY(back->getContentSize().height/toux_sp->getContentSize().height);
	 toux_sp->setPosition(CCPoint(toux_sp->boundingBox().size.width/2,back->boundingBox().size.height/2));

	 string name=dataUnion.at(idx).name;
	 CCLabelTTF* name_label = CCLabelTTF::create(name.c_str(),fontStr_KlavikaBold,30);
	 //name_label->setColor(ccc3(138,207,255));
	 name_label->setPosition(CCPoint(toux_sp->getPositionX()+toux_sp->boundingBox().size.width/2+name_label->getContentSize().width/2+5,back->boundingBox().size.height/2));
	 back->addChild(name_label);

	 string z_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan.png");
	 string z_select=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_zan.png"); //ZT_anniu_zan_select.png");
	 CCMenuItemImage* z_ccbt=CCMenuItemImage::create(z_bt.c_str(),z_select.c_str(),this,
		 menu_selector(Baselisttable::clickZ_buy));
	 z_ccbt->setUserData((void*)idx);
	 z_ccbt->setPosition(CCPoint(back->getContentSize().width*0.65,back->getContentSize().height*0.25));
	 //fillOuty(z_ccbt,z_bt);

	 string z_num=CCString::createWithFormat("%d",dataUnion.at(idx).upTimes)->getCString();
	 CCLabelTTF* z_lb=CCLabelTTF::create(z_num.c_str(),fontStr_KlavikaBold, 30);
	 z_lb->setPosition(CCPoint(10+z_ccbt->getPositionX()+z_ccbt->getContentSize().width/2+z_lb->getContentSize().width/2,z_ccbt->getPositionY()));
	 back->addChild(z_lb);
	 clickzanmaps.insert(make_pair(idx,z_lb));

     
	 string b_bt=ResManager::getManager()->getFilePath(g_stop+"ZT_icon_xiazai.png");
	 CCMenuItemImage* b_ccbt=CCMenuItemImage::create(b_bt.c_str(),b_bt.c_str(),this,
		 menu_selector(Baselisttable::play_friend));
	 b_ccbt->setUserData((void*)idx);
	 b_ccbt->setPosition(CCPoint(back->getContentSize().width*0.65,back->getContentSize().height*0.8));
	 fillOuty(b_ccbt,b_bt);


	 string chankan=ResManager::getManager()->getFilePath(g_stop+"ZT_anniu_chakan.png");
	 CCMenuItemImage* ck_ccbt=CCMenuItemImage::create(chankan.c_str(),chankan.c_str(),this,
		 menu_selector(Baselisttable::play_chak));
	 ck_ccbt->setUserData((void*)idx);
	 ck_ccbt->setPosition(CCPoint(back->getContentSize().width - ck_ccbt->boundingBox().size.width/2 - 10, back->getContentSize().height*0.5));
	 fillOuty(ck_ccbt,chankan);

     
	 string b_num=CCString::createWithFormat("%d",dataUnion.at(idx).downloadTimes)->getCString();
	 CCLabelTTF* bst_lb=CCLabelTTF::create(b_num.c_str(),fontStr_KlavikaBold, 30);
	 bst_lb->setPosition(CCPoint(10+b_ccbt->getPositionX()+b_ccbt->getContentSize().width/2+bst_lb->getContentSize().width/2,b_ccbt->getPositionY()));
	 back->addChild(bst_lb);
	 playmaps.insert(make_pair(dataUnion.at(idx).solutionId,bst_lb));


	 CCMenu* pMenu = CCMenu::create(z_ccbt,b_ccbt,ck_ccbt,NULL);
	 pMenu->setPosition(CCPointZero);
	 back->addChild(pMenu,1);
 }


 void Baselisttable::setGksetionId(int id){this->sectionID=id;}

 //小关卡列表
 void Baselisttable::drawXiaogklist(CCTableViewCell *cell, unsigned int idx){

	  
	  string bg_file="GQ_jiesuo_1.png";
	  int gs_mark=1;
	  /*
	  if(){
     
	  }
	  */
	  bg_file=ResManager::getManager()->getFilePath(g_xiaogk+bg_file);
	 

	  for(int i=1;i<=8;i++){

		  CCSprite* bg=CCSprite::create(bg_file.c_str());
		  float p_x=0;
		  float p_y=0;
		  p_x=(size.width-(bg->getContentSize().width*4))/3;
		  if(i<=4){
			 p_y=size.height-bg->getContentSize().height/2-100;
		     bg->setPosition(CCPoint(bg->getContentSize().width/2+(p_x+bg->getContentSize().width)*(i-1),p_y));

		  }else{

			  p_y=bg->getContentSize().height/2;
			  bg->setPosition(CCPoint(bg->getContentSize().width/2+(p_x+bg->getContentSize().width)*((i-4)-1),p_y));

		  }

		   cell->addChild(bg);
	  }

	 



 }

 void Baselisttable::drawSnsfriendlist(CCTableViewCell *cell, unsigned int idx){

	 cell->setUserData((void*)idx);
	 int num=0;
	 float fontsize=30;
	  string data[4]={CCString::createWithFormat("%d",idx+1)->getCString(),ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png"),users.at(idx).name,CCString::createWithFormat("%d",solution_task.at(idx).score)->getCString()}; 
	 //string data[4]={CCString::createWithFormat("%d",idx+1)->getCString(),ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png"),"name","9000"}; 

	 CCLabelTTF* data0 = CCLabelTTF::create(data[num++].c_str(),fontStr_KlavikaBold,40);
	
	 
	 urlSprite* playerSp=urlSprite::create(data[num++].c_str(),users.at(idx).pictrue.c_str());
	 //urlSprite* playerSp=urlSprite::create(data[num++].c_str(),"");

	 float start_fontx=(size.width-(data0->getContentSize().width+playerSp->getContentSize().width+10))/2;
	 data0->setPosition(CCPoint(start_fontx+data0->getContentSize().width/2,size.height/2+playerSp->getContentSize().height/2+10));
	 cell->addChild(data0);


     CCLabelTTF* data1 = CCLabelTTF::create(data[num++].c_str(),fontStr_KlavikaBold,fontsize);

     CCLabelTTF* data2 = CCLabelTTF::create(data[num++].c_str(),fontStr_KlavikaBold,fontsize);


	 float num_hinght=(playerSp->getContentSize().height+data1->getContentSize().height+data2->getContentSize().height);
	 float keyong_hight=1;//(size.height*0.8f)/(num_hinght);
	 


	 playerSp->setPosition(CCPoint(data0->getPositionX()+data0->getContentSize().width/2+10+playerSp->getContentSize().width/2,size.height/2+playerSp->getContentSize().height/2+10));
	 cell->addChild(playerSp);

	 
	 
	 data1->setPosition(CCPoint(size.width/2,size.height*0.35f));
	 cell->addChild(data1);

	 
	 data2->setPosition(CCPoint(size.width/2,size.height*0.1f)); 
	 
	 cell->addChild(data2);
 
 }

 void Baselisttable::drawYaoqingfriend(CCTableViewCell *cell, unsigned int idx){


	 string str[4]={ResManager::getManager()->getFilePath(g_public+"SZ_yuyan_anniu_xuanze_select.png"),ResManager::getManager()->getFilePath(g_public                  +"SZ_yuyan_anniu_xuanze.png"),ResManager::getManager()->getFilePath(g_stop+"GQ_touxiang_moren.png"),allusers.at(idx).name.c_str()}; 

	 int num=0;
	 for(int i=0;i<2;i++){
		 if(i==0){
			  if((idx*2+i)<allusers.size()){
				  CCMenuItemImage* p=CCMenuItemImage::create(str[0].c_str(),str[1].c_str(),this,
					  menu_selector(Baselisttable::yaoqing_friend));
				  p->setPosition(CCPoint(size.width*0.2,size.height*0.5));
				  p->setUserData((void*)(idx*2+i));

				  CCMenu* me=CCMenu::create(p,NULL);
				  me->setPosition(CCPointZero);
				  cell->addChild(me);

				  urlSprite* sp=urlSprite::create(str[2].c_str(),allusers.at(idx*2+i).pictrue.c_str());
				  sp->setPosition(CCPoint(p->getPositionX()+p->getContentSize().width/2+sp->getContentSize().width/2+5,size.height/2));
				  cell->addChild(sp);

				  CCLabelTTF* n_la = CCLabelTTF::create(allusers.at(idx*2+i).name.c_str(),fontStr_KlavikaBold,30,CCSize(150,0),CCTextAlignment::kCCTextAlignmentCenter);
				  n_la->setPosition(CCPoint(sp->getPositionX()+sp->getContentSize().width/2+n_la->getContentSize().width/2,size.height/2));
				  cell->addChild(n_la);

			  }
		 }else if(i==1){

			 if((idx*2+i)<allusers.size()){

				 CCMenuItemImage* p=CCMenuItemImage::create(str[0].c_str(),str[1].c_str(),this,
					 menu_selector(Baselisttable::yaoqing_friend));
				 p->setPosition(CCPoint(size.width*0.6,size.height*0.5));
				 p->setUserData((void*)(idx*2+i));

				 CCMenu* me=CCMenu::create(p,NULL);
				 me->setPosition(CCPointZero);
				 cell->addChild(me);

				 urlSprite* sp=urlSprite::create(str[2].c_str(),allusers.at(idx*2+i).pictrue.c_str());
				 sp->setPosition(CCPoint(p->getPositionX()+p->getContentSize().width/2+sp->getContentSize().width/2+5,size.height/2));
				 cell->addChild(sp);

				 CCLabelTTF* n_la = CCLabelTTF::create(allusers.at(idx*2+i).name.c_str(),fontStr_KlavikaBold,30,CCSize(150,0),CCTextAlignment::kCCTextAlignmentCenter);
				 n_la->setPosition(CCPoint(sp->getPositionX()+sp->getContentSize().width/2+n_la->getContentSize().width/2,size.height/2));
				 cell->addChild(n_la);
			 }
		 }
	 }
 }

 void Baselisttable::yaoqing_friend(CCObject* object){

	 Music::sharedMusicPlayer()->buttonClicked();
	 string str[2]={ResManager::getManager()->getFilePath(g_public+"SZ_yuyan_anniu_xuanze_select.png"),ResManager::getManager()->getFilePath(g_public                  +"SZ_yuyan_anniu_xuanze.png")}; 
	 CCMenuItemImage* p=(CCMenuItemImage*)object;

	 int idx=(int)(long long)p->getUserData();
	 if(fxves.at(idx)==1){
          fxves.at(idx)=0;
		  p->setNormalImage(CCSprite::create(str[1].c_str()));
		  p->setSelectedImage(CCSprite::create(str[0].c_str()));
	 }else if(fxves.at(idx)==0){
		 fxves.at(idx)=1;
		 p->setNormalImage(CCSprite::create(str[0].c_str()));
		 p->setSelectedImage(CCSprite::create(str[1].c_str()));

	 }




 }

 void Baselisttable::dinweiMyself(size_t paim){
	 string platerm,player_name;
	 bool isgetname=History::getHistory()->getLastUserPlatformAndID(platerm,player_name);
	 if(isgetname==false){
		 return;
	 }
	 if(users.size()==0){
		 return;
	 }
	 for(size_t i=0;i<users.size();i++){

		 string platerm,player_name;
		 bool isgetname=History::getHistory()->getLastUserPlatformAndID(platerm,player_name);
		 if(users.at(i).name.compare(player_name)==0){
			  paim=i+1;
			  if(paim>users.size()){
				  return;
			  }
			  break;
		 }else{
			 return;
		 }
	 }
	 int h_data=paim;
	 if(h_data<=2){  
		 h_data=0;
	 }else{
		 h_data=h_data-2;
	 }
	 CCPoint offset=CCPoint(-(h_data)*size.width,0);
	 mTableView->setContentOffset(offset); 
 }


 //读取配置文件
 string Baselisttable::getBytypeandfile(int id){
     
	 string file=ResManager::getManager()->getFilePath(g_stop+"ZT_chaonengli.png");
	 if(type==Chaondaoju){
        
	 }
	 return file;
 }

 int Baselisttable::getBytypeandidnum(int id){

	 int num=0;
	 if(type==Chaondaoju){
		 if(id==0){
			 num=1;
		 }else{
			 num=10;
		 }
	 }
	 return num;
 }


 string Baselisttable::getBylauguageMoney(int id){


	 string my="$ 0.99";

	 if(id==0){
		 my="$ 0.99";
	 }else if(id==1){
		 my="$ 2.99";
	 }
	 return my;

 }

 //超能道具购买
 void Baselisttable::allCallback_buy(CCObject* object){

	 Music::sharedMusicPlayer()->buttonClicked();
	  CCMenuItemImage* p=(CCMenuItemImage*)object;
	  int id=(int)(long long)p->getUserData();
	  //
	  ProductID iProductID = ProductID_SuperTool_One;
	  if (id == 1)
	  {
		  iProductID = ProductID_SuperTool_Ten;
	  }
	  CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Baselisttable::payResult), NetMessage::MSG_PayResult, NULL);
	  CmdHelper::getInstance()->buyProduct(iProductID);
 }

 void Baselisttable::payResult(CCObject* pMsg)
 {
	 CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NetMessage::MSG_PayResult);
	 MsgData* pdata = (MsgData*)pMsg;
	 if (pdata && pdata->errCode == MsgData::eSuccess)
	 {
		 ProductID id = *(ProductID*)pdata->data;
		 if (id == ProductID_SuperTool_One)
		 {
			 MessageBar::show(LFStrings::getValue("Getonesuperpower").c_str());
			 //CCMessageBox("Buy successfully", "Tip"); 
			 //CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAONDJBUY,(CCObject*)"0");
		 }
		 else if (id == ProductID_SuperTool_Ten)
		 {
			 MessageBar::show(LFStrings::getValue("Gettensuperpower").c_str());
			 //CCMessageBox("Buy successfully", "Tip");
		 }
	 }
	 else
	 {
		 MessageBar::show(LFStrings::getValue("Purchasefailed").c_str());
		 //CCMessageBox("Purchase failed", "Tip");
	 }
     CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAONDJBUY,(CCObject*)"0");
	 CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAONDJBUYN,(CCObject*)"0");
 }
 
 void Baselisttable::click_zan(CCObject* object){

	  MsgData* data=(MsgData*)object;
	  if(data->errCode == MsgData::eSuccess)
	  {
		  const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
		  if(info){
			  friendSolutionlist(info,true);
              Myobject* myob=new Myobject();
			  bool ishave=false;
			  for(size_t i=0;i<dataUnion.size();i++){
				  if(dataUnion.at(i).solutionId.compare(solutionid_click)==0){
					  myob->data=dataUnion.at(i);
					  ishave=true;
					  break;
				  }
			  }
			  CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHPARENTCLICKZ,(CCObject*)myob);
		  }
	  }
 }


 void Baselisttable::snsData(CCObject* object){

	 const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	 if(info&&zanfail.size()>failresult_idx){
		 friendSolutionlist(info,false);
		 string z_num=CCString::createWithFormat("%d",dataUnion.at(zanfail.at(failresult_idx)).upTimes)->getCString();
		 clickzanmaps.at(zanfail.at(failresult_idx))->setString(z_num.c_str());
	 }
	 

 }

 //点赞
 void Baselisttable::clickZ_buy(CCObject* object){


     //NetMessage::MSG_PraiseSolution
	 //jiefa id
	 /*
	 CCMenuItemImage* p=(CCMenuItemImage*)object;
	 int idx=(int)(long long)p->getUserData();
	 result_idx=idx;
	 CmdHelper::getInstance()->praiseSolution(dataUnion.at(idx).solutionId,true);
	 */
 }

 void Baselisttable::clickZ_idx(string idx){
	 Music::sharedMusicPlayer()->buttonClicked();
	 solutionid_click=idx;
	 CmdHelper::getInstance()->praiseSolution(idx,true, true);
 }

 //播放
 void Baselisttable::play_friend(CCObject* object){

    

 }

 void Baselisttable::refresheplay_num(string usid){

	 const TaskInfor* info=SocialInfoManager::getInstance()->getSectionInfor(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	 if(info){
		 friendSolutionlist(info,false);
		 string z_num=CCString::createWithFormat("%d",dataUnion.at(result_idx).downloadTimes)->getCString();
		 playmaps.at(usid)->setString(z_num.c_str());
	 }
	  
 }

 void Baselisttable::play_chak(CCObject* object){

	 CCMenuItemImage* p=(CCMenuItemImage*)object;
	 int idx=(int)(long long)p->getUserData();
	 result_idx=idx;
	 Myobject* ob=new Myobject();
	 ob->data=dataUnion.at(idx);
	 CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FRIND_PLAY,(CCObject*)ob);
 }

 //


 void Baselisttable::fillOuty(CCMenuItemImage* p,string imgfile){


	 CCSprite* s=CCSprite::create(imgfile.c_str());
	 s->setScale(1.10f);
	 p->setSelectedImage(s);

	 CCSprite* n=CCSprite::create(imgfile.c_str());
	 CCLayer* layer=CCLayer::create();
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


 void  Baselisttable::setGkmsglayeruser(vector<TaskSolution>& us){

	   users.clear();
	   solution_task.clear();
	   for(size_t i=0;i<us.size();i++){
		   solution_task.push_back(us.at(i));
		   const TaskUser*  p_user=SocialInfoManager::getInstance()->getUserInfor(us.at(i).userId);
		   if(p_user){
		     TaskUser user;
			 user.name=p_user->name;
			 user.pictrue=p_user->pictrue;
			 user.userId=p_user->userId;
		     users.push_back(user);
		   }
      } 
	  mTableView->reloadData();
 }




 void  Baselisttable::yaoqingfriends(vector<TaskUser>& as){

	   allusers.clear();
	   for(size_t i=0;i<as.size();i++){
		   allusers.push_back(as.at(i));
		   fxves.push_back(1);
	   }
	   mTableView->reloadData();
 }

 void  Baselisttable::friendSolutionlist(const TaskInfor* info,bool isload){

	 
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

	 if(isload==true){
	    mTableView->reloadData();
	 }
 }


 void Baselisttable::setParentequaltasg(int tag){

	  this->parentlayerTag=tag;
	  
 }


 void Baselisttable::moveTableCellAt(CCTableViewCell* cell, int idx)
 {
	 if (!cell) return;
	 int cellNum = cell->getIdx() - idx;
	 float height = this->tableCellSizeForIndex(mTableView,0).height;

	 CCLOG("%s ---> begin move cell", __FUNCTION__);
	 float timeNum = 0.5f;
	 cell->runAction(CCSequence::create(
		 CCMoveBy::create(timeNum, ccp(0,cellNum*height)),
		 CCDelayTime::create(0.03f),
		 CCCallFunc::create(this, callfunc_selector(Baselisttable::tableCellMoveEnd)),
		 NULL));

	 for (int i=0,count=this->numberOfCellsInTableView(mTableView); i<count; ++i)
	 {
		 CCTableViewCell* pCell = mTableView->cellAtIndex(i);
		 if (!pCell || pCell == cell) continue;

		 pCell->runAction(CCMoveBy::create(timeNum, ccp(0,-height)));
	 }
 }

 //更新social
 void Baselisttable::tableCellMoveEnd()
 {
	 CCLOG("%s --->cell move end", __FUNCTION__);
	 //gengxin
	 int seasonId = LevelManager::getManager()->getLastSeasonId();
	 int sectionId = LevelManager::getManager()->getLastSectionId();
	 SectionInfor* pInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId, sectionId);
	 if (pInfor)
	 {
		 SocialInfoManager::getInstance()->setSelfSectionScore(seasonId, sectionId, pInfor->iRankingScore, pInfor->iStarNum);
	 }

// 	 vector<TaskSolution> vec = SocialInfoManager::getInstance()->getSectionInfor(seasonId, sectionId)->users;
// 	 this->setGkmsglayeruser(vec);
	 CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FRIND_BEYOND_ACOUNT_REFRESH, NULL);
 }