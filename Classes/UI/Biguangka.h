#pragma once

#include <map>
#include <vector>
#include "cocos-ext.h" 
#include "cocos2d.h"
#include "Baselisttable.h"
#include "urlSprite.h"
#include "AchievementManager.h"

USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace std;

class Biglisttable;

class Biguangka :public CCLayer 
{
public:
	Biguangka(void);
	~Biguangka(void);
	Biguangka(int season);

	CREATE_FUNC(Biguangka);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	static Biguangka*  creteByseason(int season);

public:
	void datainint();
	void drawPublicback();

	void drawsnsPicture();
	void netDatainint(int curren_season);
	int  getByseasoncheck(int season);
	void drawLayer(int id);
	void drawUrlpiture(int i,CCLayer* layer);

public:

	void allCallback(CCObject* pSender);
    void fillOuty(CCMenuItemImage* p,string imgfile);
	void BigjuliOut(float changeOut,int current,int xlpage);

	
	void Fadeoutbig(int curent);
	
    map<int,CCLayer*> layer_map;
	
	int getCurrentseason();
	void setCurrentseason(int tag);
	void AllOut(int ccurrent);
	void Allin(int ccurrent);
	void update(float delta);

	Biglisttable* getBiglisttable(){return biglist;};
	void getImagecolor4Bpos(CCImage *p,CCPoint point,int idx);

	void updateMsglayer(CCObject* object);
	void refreshlaguage(CCObject* object);
	void refreshLockButton(CCObject* object);
	void refreshSnsProgress();
	void moveOver();
private:

	CCMenuItemImage* itemleft;
	CCMenuItemImage* itemright;

	float dk_width;
	CCSize size;
	Biglisttable* biglist;
	int curren_season;//当前章节数
	int maxseson;//总章节数
	map<int,CCScale9Sprite*> scla9_map;
	map<int,vector<CCSprite*> > sprite_map;
	map<int,vector<CCMenuItemSprite*> > menusprite_map;
	map<int,ccColor3B> colors; 
	std::vector<CCMenuItemImage*> _startButtons;
	std::vector<CCMenuItemImage*> _lockButtons;
	std::vector<CCLabelTTF*>	  _levelNums;
	std::vector<CCSprite*>		  _levelLocks;
	CCSprite* heseSp;
	vector<CCSprite*> backs_v;
	//map<int,CCScale9Sprite*> scla9_kuang_map;
	float sps_juli;

	vector<CCScale9Sprite*> mBGs;
	vector<CCRenderTexture*> mRenderStarts;
	vector<CCLabelTTF*> labeles;

	int		m_iLastChangBgPage;

	map<int,TaskUser> gk_txvs;

	//
	CCSprite* back_ph;
	//CCLabelTTF* bT;
	map<int,vector<urlSprite*> >  frinedprogress_maps;
	map<int,CCLayer*> layers_map;
	//bool		_hasSnsLayerCreated;

};


class Biglisttable :public CCLayer ,public CCScrollViewDelegate  
{
public:
	Biglisttable(void);
	~Biglisttable(void);
	CREATE_FUNC(Biglisttable);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	void ontouchCilick(CCTouch *pTouch, CCEvent *pEvent);
	bool SpisContaintPoint(CCSprite* ps,CCPoint p);

public:
	//bool  two_check;
	void inintData();

	void drawBiglistscroview();
	void drowList(CCPoint p,int page);

	void diguisetOut(CCNode* node);
	void diguibyjuliOut(CCNode* node,float changeOut);
	void diguibyjuliIn(CCNode* node,float changeOut);
	void setBiguangka(Biguangka* kuan);

	void setBigcurent(float time);
	void createGradientClipShader(float leftX, float rightX);
	int getCurntId();

	void refreshLanguge();

	void scrollToSeason(int seasonId, bool bAnimation = false);
	void setStartTouchPonit(CCPoint point){start_ponit = point;};
	void setEndTouchPonit(CCPoint point){end_point = point;};
	void setScroviewOffsetPonit(CCPoint point){scroview_point = point;};

	CCScrollView* getScrollView(){return scrollView;};
	void getFullVersion();
private:
	int type;
	int layerspace;
	bool isVetical;
	CCSize size;
	int itemnum;
	int maxseson;
	CCGLProgram* mGradientClipShader;
private:

	map<int,vector<CCLabelTTF*>> lable_maps;
 
	map<int,CCLayer*> layer_map;
	CCLayer* scroll_layer;
	CCScrollView *scrollView;
	CCSize scrow_size;
	int current_page;
	int old_page;
	CCPoint start_ponit;
	CCPoint end_point;
	int mark_firstmove;
	CCPoint moveold;
	CCPoint movecurent;


	CCPoint moveold_fade;
	CCPoint movecurent_fade;

	Biguangka* kuanlist;
	CCPoint scroview_point;
    int direction;

	bool changePage;
	
	float ch_check;
	bool isTouch; 
	float ch;
	
};
