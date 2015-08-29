#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameInforEditer.h"
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
#include "Uimsg.h"


using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;


namespace Listbase{
class Baselisttable;
}


class Baselisttable  :public CCLayer , public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	Baselisttable(void);
	~Baselisttable(void);

	CREATE_FUNC(Baselisttable);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	 

	
	virtual void scrollViewDidScroll(CCScrollView* view);//CCTableViewDelegate
	virtual void scrollViewDidZoom(CCScrollView* view);  //CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell); //CCTableViewDelegate
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);//CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);//CCTableViewDataSource
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);//CCTableViewDataSource
    
public:
	//布局
	void setDataType(int type);
	void setMtabletype(int type,CCSize size,int itemnum,CCScrollViewDirection direction,CCTableViewVerticalFillOrder order,int space);// space 是二项之间的间隔
	//
	void addDataimg(CCTableViewCell *cell, unsigned int idx);

	CCMenu* createInviteButton(CCNode* parent);

	void onInviteClick(CCObject* obj);

	void loginResult(CCObject* pMsg);

	//超能道具
	void drawChaondju(CCTableViewCell *cell, unsigned int idx);

	//好友列表
	void drawFriendlist(CCTableViewCell *cell, unsigned int idx);
	//点赞回调
	void click_zan(CCObject* object);
	void snsData(CCObject* obj);
	//小关卡列表
	void drawXiaogklist(CCTableViewCell *cell, unsigned int idx);
	void setGksetionId(int id);
	//sns 服务器排名列表


	void drawSnsfriendlist(CCTableViewCell *cell, unsigned int idx);
	
	void drawYaoqingfriend(CCTableViewCell *cell, unsigned int idx);
	void yaoqing_friend(CCObject* object);

	void dinweiMyself(size_t paim);


	//吧一个cell移动到指定的idx， 其余位置的相应变化
	void moveTableCellAt(CCTableViewCell* cell, int idx);
	void tableCellMoveEnd();

public:
	vector<int> fxves;
	string getBytypeandfile(int id);
	int getBytypeandidnum(int id);
	string getBylauguageMoney(int id);


public:
	//购买超能道具
	void allCallback_buy(CCObject* object);
	//点赞
	void clickZ_buy(CCObject* object);
	void clickZ_idx(string idx);

	//播放
	void play_friend(CCObject* object);
	void play_chak(CCObject* object);
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void refresheplay_num(string usid);
	//大关卡显示使用
	void  setGkmsglayeruser(vector<TaskSolution>& users);


	//邀请用
	void  yaoqingfriends(vector<TaskUser>& allusers);

	//好友解法列表
	void  friendSolutionlist(const TaskInfor* info,bool isload);

	void payResult(CCObject* pMsg);

	void setParentequaltasg(int tag);

	//void setFriendmethod (Friendmethod* fp);

	CCTableView* getTaleView(){return mTableView;};

private:


	//data 
    
	int type;
	int layerspace;
	bool isVetical;
	CCTableView *mTableView;
	CCSize size;
	int itemnum;

	
	//关卡信息id
	int sectionID;
	list<SectionInfor*> msg_setion;
	int season_num;
	
	//定位
	map<int,CCTableViewCell*> mapcells;
	//dianzan
	map<int,CCLabelTTF*> clickzanmaps;
	map<string,CCLabelTTF*> playmaps;

	int zan_idx;
	bool mark_sns;
	vector<int> zanStack;
    vector<int> zanfail;
 
	int result_idx;
	size_t failresult_idx;
 	//
	vector<TaskUser> users;
	vector<TaskSolution> solution_task;

    
	//分享用
	vector<TaskUser> allusers;
	//
	vector<msgSnsdata> dataUnion;
	//Friendmethod* friendlist;
	string solutionid_click;

	int parentlayerTag; //不同根节点相同消息

};

/*
stack 的基本操作有：
入栈，如例：s.push(x);
出栈，如例：s.pop();注意，出栈操作只是删除栈顶元素，并不返回该元素。
访问栈顶，如例：s.top()
判断栈空，如例：s.empty()，当栈空时，返回true。
访问栈中的元素个数，如例：s.size()。
*/

/*

q.empty()               如果队列为空返回true，否则返回false  
q.size()                返回队列中元素的个数  
q.pop()                 删除队列首元素但不返回其值  
q.front()               返回队首元素的值，但不删除该元素  
q.push()                在队尾压入新元素  
q.back()                返回队列尾元素的值，但不删除该元素 

*/