#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
#include "GameInforEditer.h"
#include "Uimsg.h"
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;




struct Backspsizemsg
{

	 CCSize sprite_size;
	 float distance_one;

};

class Xiaogktablelist;
class Xiaocheckpoint :public CCLayer
{
public:
	Xiaocheckpoint(void);
	~Xiaocheckpoint(void);
	CREATE_FUNC(Xiaocheckpoint);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
public:
	void drawLayer();
	
public:
	void faihuiDa(CCObject* object);
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void allCallback(CCObject* ob);
public:
    //获取每个光卡的金子数
	void setcurrentId(int season);
	int  getcurrentId();
	int getNumscore();
 	void setIsyingchang();
	bool isetLeftbutton();
	bool isrightbutton();
	//进入游戏
	void BigjuliOut(float changeOut,int current,int xlpage);

	void labelRefresh();
public:
	
	void changeBGColor(CCObject* pObj);

	CCSprite* getBacksprite();
	int getYeshu();
	//根据章节 获取小关卡数
	int getByseasoncheck(int season);
	void setHesepoint(int page);
	void jianxiao(int page);
	void updateMsglayer(CCObject* object);

	void refreshLanguge(CCObject* object);
	void refreshUI(int seasonid);
private:
	 
	 int current_season;
	 
	 CCSize onekp;
	 CCSize size;
	 int current_page;
	 
	 
	 CCPoint moveold;
	 CCPoint movecurent;
	 int mark_firstmove;
	 //记录一个sparite的大小
	 Backspsizemsg sp_quyu; 
	 map<int,vector<CCSprite*> > msg_mapsps;
	 //关卡信息
	 vector<SectionInfor*> msg_setion;
	 CCMenuItemImage* itemleft;
	 CCMenuItemImage* itemright;
	 //点击事件所需
	 vector<CCSprite*> sps;
	 CCSprite* actionsp;
	 //动画所需
	 vector<CCSprite*> spsaction;
	 int mark_action;
	 CCSprite* mBG;
	 Xiaogktablelist* list;

	 CCSprite* heseSp;
	 vector<CCSprite*> backs_v;
	 float sps_juli;
	 
	CCLabelTTF* star_l;


};


class Xiaogktablelist :public CCLayer,public CCScrollViewDelegate {

public:
	Xiaogktablelist(void);
	~Xiaogktablelist(void);
	CREATE_FUNC(Xiaogktablelist);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	void createGradientClipShader(float leftX, float rightX);
	void setSpriteClip(CCSprite* sprite);


	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	bool ontouchCilick(CCTouch *pTouch, CCEvent *pEvent);

	void dingwei();
public:
	//获取每个光卡的金子数
	void setcurrentId(int season);
	//初始化
	void drawLayer();
	void getMarkbackstr(string &back,int &mark,int page,int i);
	void drowList(CCPoint p,int page);
	void refreshPagelable();
	void refreshLayer(int page);
	void refreshlable(int page);
	void drowunlockstate(CCLayer* layer);
	void drowlockstate(CCLayer* layer,int page,int i);

	int getYeshu();
	//根据章节 获取小关卡数
	int getByseasoncheck(int season);
	bool isJiesuo(int key,bool mark);
	int getNumscore();
	//联网功能 获取好友头像 读取配置文件想 头像 CCSprite* sps;
	void netDatainint();
	void  snsData(CCObject* obj);
	void addNodelabel();
	CCSprite* getByyshuandid(int key);
	bool SpisContaintPoint(CCSprite* ps,CCPoint p);
	//进入游戏
	void startGame();
	void diguibyjuliOut(CCNode* node,float changeOut);
	void setXiaogk(Xiaocheckpoint* p);

	double mLastTouchTime;
	void shuangxin(int curentseason);
	void getImagecolor4Bpos(CCImage *p,CCPoint point,int idx);
	void updateCs(float dt);
	
	void slideToPage3(float dt);
	void slideToPage3Over();
	void playLockInfoAnimation();
	void refresh3rdLayer();
private:

	CCPoint end_point;
	CCPoint moveold;
	CCPoint movecurent;
	int current_season;
	int current_page;
	int old_page;
	CCSize size;

	//关卡信息
	vector<SectionInfor*> msg_setion;
	map<int,vector<CCSprite*> > msg_mapsps;
	map<int,CCSpriteBatchNode*> batchnodemaps;
    map<int,TaskUser> gk_txvs;
    map<int,CCLayer*> layer_map;
	map<int,ccColor3B> colors; 
	map<int,CCLayer*> lockstate_layers;


	CCSprite* mBG;
	int select_gkid;
	CCLayer* scroll_layer;
	CCScrollView *scrollView;
	CCSize scrow_size;
	static int layernum;
	Backspsizemsg sp_quyu; 
	//好友列表信
	CCPoint start_ponit;
	int mark_firstmove;
	int dianjinum;
	Xiaocheckpoint* prent;
	CCPoint scroview_point;
	CCPoint movecurent_fade;
	CCPoint moveold_fade;
	int direction;
	float ju_li;
	CCGLProgram* mGradientClipShader;
	bool button;
	bool isTouch;
	int refresh_page;
	int num;
};