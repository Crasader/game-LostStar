#pragma once

#include <map>
#include <vector>
#include "cocos-ext.h" 
#include "cocos2d.h"
#include "Baselisttable.h"
#include "CmdHelper.h"
#include "ShieldTouchlayer.h"
#include "Recorder.h"
#include "urlSprite.h"

USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace std;

class Acount:public CCLayer
{
public:
	Acount(void);
	~Acount(void);
	Acount(int current_score,int curent_star, int history_score,int history_star);
public:
	CREATE_FUNC(Acount);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	//结算总分数 当前获得金子数 历史金子总数
	static Acount* createByid(int current_score,int curent_star, int history_score,int history_star);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);


	void playBeyondFriendAnimate();
	void beyondFriendAnimateEnd_refreshData(CCObject* obj);
	bool m_playBeyondAnimate;

public:
 	void drawAcountlayer();
	void drawfriendlist();
	void setTruelayer();

	void actionFp();
	void savedata();
public:
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void allCallback(CCObject* ob);
	void allCallback2(CCObject* ob);

public:


	void action2_1();
	void action2_1hd();
	void action1_2();
	void action1_2hd();

	void hd1(CCNode* p);
	void hd2(CCNode* p);
	void firstAction();
	void actionode(CCNode* node);
	void starEnd();
	void starNoend();
	void nodesOut(CCNode* node);
	void nodesIn(CCNode* node);


public:
	void snsgetData();
	void snsData(CCObject* obj);
	void shuaixinData();
	//
	void layerinint();
	void actionStart();
	void btandanStar();
	void btandanStar2(CCNode* node);
	void starSaction();
	void endAction();

	void acount_chnageYaoqing();
	void yaoqing_changeacount();

	//donghua
	void Fadeinnodes(CCNode* node,int tag);
	void Fadeinnodes(CCNode* node,float time);
	void Outall(CCNode* node,int tag);
	void Outall(CCNode* node,float time);
	

	void actionupdate(float time);
	void starSactionMan();
	void starSactionMan(CCNode* pSender, void* data);


	void sectionSwitchaction();
	void sectionActionend();


	//初始化好友解法视频layer

	void Videofriendlayerinit();
	void friendSolutionlist();
	void freindvideoPlay(CCObject* object);
	void drawfriendVideo(msgSnsdata data);
	void acouTofriendvideo_actiondone(CCNode* node);
	void zan_button(CCObject* object);
	
	void actionFaihuimain(CCNode* node);
	void refreshSolution(CCObject* object);
	void click_zan(CCObject* object);
	void clickisz(CCObject* obj);

	void loginResult(CCObject* pMsg);
	void showTime(CCNode* pNode);
private:

	CCSize size;
	int acount_score;
	int curent_starnum;
	int history_score;
	int history_numstar;

	Baselisttable* list;
	Baselisttable* list2;
	CCLayerColor *color;
	//星星界面
	CCLayer* layer1;
	vector<CCNode*> nodes1;
    //邀请好友界面
	CCLayer* layer2;
	vector<CCNode*> nodes2;
	int mark;
	int num;
	CCSize touchsize;
	CCSprite* tg_sp;
	vector<CCNode*> vsps;
	vector<CCNode*> vspsstar;
	int num_star;
	int index;
	const TaskInfor* info_layer;
	vector<TaskUser> allusers;



	//动作时间管理
	float center_time;
	float left_time;
	float rght_time;
	float btandst_time;
	float starS_time;
	float starS_timeMan;
	float end_time;
	int jijiaqi;

	CCLayer* acount_layer;
	//左面layer 以及button
	CCLayer* left_layer;
	CCPoint leftstart_point;
	CCPoint leftend_point;
	//左面button
	CCLayer* leftlayer_button;
	CCPoint leftstart_pointb;
	CCPoint leftend_pointb;
    
	//zhongjianlayer
	CCLayer* centerlayer;
	CCPoint  center_point;
	CCPoint  centerend_point;
	//

	//biaotilayer
	CCLayer* layer_bT;
	CCPoint  bt_startP;
	CCPoint  bt_endP;

	//starlayer
	CCLayer* layer_anStar;
	vector<CCPoint> an_P;

	//sharelayer
	CCLayer* layer_share;
	CCLayer* layer_shareB;


	//
	CCLayer* layer_gameB;
	//成绩提高
	CCLayer* cjlayer;
	CCPoint cjlayer_startpoint;
	CCPoint cjlayer_endpoint;

	//多少分
    CCLabelTTF* score_l;
    
	CCLabelTTF* inN;
	CCLabelTTF* inS;

	CCLabelTTF* shN;
	CCLabelTTF* shS;

	CCLabelTTF* inN2;
	CCLabelTTF* inS2;

	int num_time;
	int all_time;

	int back_time;
	int first_actiontime;
	int bt_time;
	int xinxin_time;
	int text_time;
	int butoon_time;
	int dom_buttontime;
	int newcorlds_time;
	bool button_mark;


	ShieldTouchlayer* friendvideoplay_layer;
	bool isfriendVideolayerinit; //默认为false
	vector<msgSnsdata> dataUnion;
	vector<vector<record> > m_allRecordData;
	CCObject* freind_solution;
	int freind_idx;
	msgSnsdata data_play;
	urlSprite* toux_sp;
	CCLabelTTF* name_label;
	CCMenuItemImage* z_ccbt;
	CCLabelTTF* z_label;
	bool m_bIsPlayRecord;
	bool isclickZ;
};


