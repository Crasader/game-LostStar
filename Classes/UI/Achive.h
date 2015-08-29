#pragma once

#include <map>
#include <vector>
#include "cocos-ext.h" 
#include "cocos2d.h"
#include "Baselisttable.h"
#include "AchievementManager.h"

USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace std;

class Achivetablelist;

struct  Changelist
{
	bool mark;// true是一级菜单 false二级菜单
	bool iszk;// true 是打开   false 是关闭

	string name;
	string describe;
	string filename;
};

class Achive: public CCLayer 
{
public:
	Achive(void);
	~Achive(void);

	CREATE_FUNC(Achive);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();


	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher(void);


public:
	void setAchivetablelist(Achivetablelist* p);
	void drawAchive();
	void valueChanged(CCObject *sender, CCControlEvent controlEvent);
	bool iscontainPoint(CCPoint po);
    
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void setSpritepostion(float num,float dex,bool mark);
public:
	CCSprite* sp_kongzhi;
	CCSprite* gd_spsrite;
	float shangxiang_y;
	float xianxian_y;
	float zongjuli;
	float scaleBeishu;
	void faihuiCallback(CCObject* ob);

	void change_sx(float oldhight,float currenthight);
	void changeBGColor(CCObject* pObj);
	void refreshlanguge(CCObject* object);
private:

	CCSize size;
	map<int,vector<ccColor4B> > color_maps;
	//返回配置的成就列表
	vector<Achievement> allahives;
	//返回成就列表
	vector<Achievement> wc_achives;
	Achivetablelist* list;
	
	CCSize spsize;
	CCPoint moveold;
	CCPoint movecurent;
	bool iscontaint;
	CCScale9Sprite*			m_pBg;
	CCLabelTTF* data1;

};





class Achivetablelist :public CCLayer , public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	Achivetablelist(void);
	~Achivetablelist(void);

	CREATE_FUNC(Achivetablelist);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	virtual void scrollViewDidScroll(CCScrollView* view);//CCTableViewDelegate
	virtual void scrollViewDidZoom(CCScrollView* view);  //CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell); //CCTableViewDelegate
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);//CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);//CCTableViewDataSource
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);//CCTableViewDataSourcec CCTableViewDelegate

public:
	void inintData();
	void setAchivelayer(Achive* p);
	//布局
	void setDataType(int type);
	void setMtabletype(int type,CCSize size,int itemnum,CCScrollViewDirection direction,CCTableViewVerticalFillOrder order,int space);// space 是二项之间的间隔
	//
	void addDataimg(CCTableViewCell *cell, unsigned int idx);
	//超能道具
	void drawAchivelist(CCTableViewCell *cell, unsigned int idx);
	bool isAchivewc(string name);
public:
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void allCallback(CCObject* ob);
	void setPrentlist(float num,float dex,bool mark);
	void changeIdxbutton(int idx);
	//_
	string moquduoyustring(string moqu,string src);
	CCPoint getTableconsetoff();
	float   gettableHight();
	float   gettablestartHight();
	void refreshlanguge();
public:
    Achive* main_a;
private:
	int type;
	int layerspace;
	bool isVetical;
	CCTableView *mTableView;
	CCSize size;
	int itemnum;
	
	//返回配置的成就列表
	vector<Achievement> allahives;
	//返回成就列表
	vector<Achievement> wc_achives;
	vector<Changelist> tablelist;//所有数据初始化

	vector<Changelist> changelist;


	string bg_strs[4];
	float hight_m;
	float hight_sm;
	
	CCPoint moveold;
	CCPoint movecurent;
};