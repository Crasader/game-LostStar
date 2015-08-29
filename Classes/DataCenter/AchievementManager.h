#ifndef __AchievementManager_H__
#define __AchievementManager_H__

#include "cocos2d.h"
#include <string>
#include "LFFileUtiles.h"
#include <vector>
#include "include_json.h"
#include "Box2d/Box2D.h"
#include "Debug.h"

using namespace std;
using namespace cocos2d;

/**
 *	一个成就数据
 */
struct Achievement
{
	string name;
	string filename;
	string describe;
	int id;
    int iseasonid;
    int isectionid;
	Achievement()
	{
		name = "";
		filename = "";
		describe = "";
		id = 0;
        iseasonid = 0;
        isectionid = 0;
 	}
	Achievement(string name);


	bool operator == (const Achievement& obj){

		return id == obj.id;
	};

	Achievement& operator = (const Achievement& obj){
		this->name = obj.name;
		this->filename = obj.filename;
		this->id = obj.id;
		this->iseasonid = obj.iseasonid;
		this->isectionid = obj.isectionid;
		this->describe = obj.describe;
		return *this;
	};
};

/**
 *	成就的管理器
 *	需要外界把一些所需的情况传入进来，传的越多，成就信息就越丰富
 */
class AchievementManager/*: public CCObject*/
{
protected:
	AchievementManager();
private:
	static AchievementManager* instance;
	LFFileUtiles* m_pFileUtiles;
	vector<Achievement> ownAchievementsVec;	//
    
    vector<Achievement> m_allDefalutAchievementVec;	//

	int mSeasonId;
	int mSectionId;
	CCNode* mRedStar;
	b2Body* mRedStarBody;

	b2World* mWorld;

	bool mIsRedStarCollected;
	bool mIsRedStarCreatedInScreen;	// 是否红星在屏幕里面创建
	bool mIsRedStarFlyOutScreen;	// 是否红星飞出了屏幕之外，也就是高度超过了Screen

	bool mIsRedStarOnceInScreen;	// 红星是否在屏幕里面出现过

	float mRedStarStableTimeAfterFlyOutScreen;

	int mLastClickedTag;
	float mTimeForLastClick;	// 现在和上次有效点击之间的时间差
	float mTimeForRedStarWithoutCollection;
	int mRestartClickedNum;
	int mAlarmClockBiNum;		// 闹钟bi响的声音
	int mBoilerUsedNum;
	int mRestitutionBall24Num;
	int mUsedRestitutionBall24Num;
	int mTimerDesBombNum;
	int mMatorUsedTime;
	int mUsedTimerDesBombNum;
	int mHandlableMotorNum;
	int mTotalMotorNum;
	int mUsedMotorNum;

	int mTotalNomalShapeNum;		// 一般的几何体，可以消除的，没有马达的几何体
	int mClickedNormalShape;

	float mRedStarHorizontalFlyTime;	// 红心水平飞行的时间
	float m_CurrentMaxRedStarHorizontalFlyTime;	// 当前的关卡，红星水平飞行的最长时间
	bool mIsRedStarStableAfterHorizontalFly;
	float mRedStarStableTimeAfterHorizontalFly;	// 红星稳定的时间

	float mLastRedStarAngle;
	float mRedStarHighSpeedRotatedAngle;	// 红星

	bool mIsRedStarInRope;		// 红星在绳子上

	float mRedstarContactTableTime;
	bool mIsFirstContactTableStatic;
	CCNode* mContactTable;


	bool m_bHaveNew;

	bool parseAchievementFile();
    
    void initAllDefaultAchievements();


	/**
	 *	对于台面的判断会有问题，因为台面只有一个fixture
	 */
	bool isAnySpriteInArea(CCNode* exceptedNode,CCPoint pos,float r); 

	/**
	 *	添加一个成就数据到内存，成功就返回true
	 */
	bool addAchievement(Achievement a);	

	bool isRedStarStabled();

	/**
	 *	是否已经有了某个成就
	 */
	bool alreadyHaveAchievement(Achievement a);

	bool isInScreen(CCNode* node);

	// 在要出现Dangerous的地方，看一下这个Dangerous是不是有效
	// 也就是看有没有挡住星星的无效台面，不可点击马达，锅炉
	bool isDangerousEffetive(CCNode* redStar,CCNode* table);

	// 红星在空中时,就可以开始触发检测空中滞留时间
	void setRedStarInAir();
	void setRedStarCollectioned();
public:
	static AchievementManager* getInstance();
	static void destroy();
	void initData();

	void test();

	void tick(float t);
    
    //获取服务器需要的数据，格式为  “100010010011100”，其中1表示获得，0表示未获得
    size_t getSaveToServerData();
    void saveServerDataToFile(size_t value);
    
    //返回配置的成就列表
	vector<Achievement> getDefaultAchievementList() {return m_allDefalutAchievementVec;}	//

	//返回成就列表
	vector<Achievement> getAchievementList() {return ownAchievementsVec;}	// 

	//获取是否有新成就
	void setHaveNew(bool e){m_bHaveNew = e;};
	bool getHaveNew(){return m_bHaveNew;};

	/**
	 *	保存成就信息到磁盘
	 */
	void save();

	/**
	 *	生成 成就的String形式的数据，这里返回json格式的数据
	 */
	string genAchievementString();

	/**
	 *	清除保存的信息，每一个section游戏都应该清理一下。
	 *	这里需要currentSeationId和currentSectionId是因为，有些数据对于本关来说是不需要清理的
	 *	比如：restart次数，如果currentSeationId，currentSectionId和上次的一样，那么就不能被清理
	 */
	void clearData(int currentSeationId,int currentSectionId);
	
	//======================下面是设置成就数据===============

	/**
	 *	外界点击了重新开始时需要调用。
	 */
	void restartItemClicked();

	// 闹钟bi响了
	void alarmClockBi();

	// 红星从屏幕上面飞出去，并失败了，回调
	void redStarFlyOutOffScreenAndFailed();
	/**
	 *	成功 时需要回调
	 */
	void sucessed(CCSprite* redStar,CCSprite* table,b2Body* tableBody);

	// 每次的有效点击，就回调，node为NULL 就是绳子
	void effctiveClick(CCNode* node,b2Body* body);

	//void redStarOnTableStatic(float dt);
	//void redStarOnTableMove();

	void redStarContactTable(CCNode*table);
	// 在台面上，就会回调，并传入滑移
	void redStarOnTable(b2Vec2 shift,float dt);

	// 世界中，发生碰撞之后，需要调用
	void beginContact(CCNode* node1,CCNode* node2);
	void endContact(CCNode* node1,CCNode* node2);
	void PostSolve(CCNode* node1,CCNode* node2);
	// 注入红星，用于记录红星的数据，在createRedStar时就设置
	void setRedStar(CCNode* redStar,b2Body* body,bool isThrow);

	void setWorld(b2World* world);

	// 创建一个弹性小球时 需要调用
	void createdOneRestitutionBall24();

	// 创建了一个时间减速炸弹时调用
	void createdATimeDecBomb();

	// createShape 创建一般的几何体时回调
	void createdOneNormalShape();

	// 创建了一个马达
	void createdOneMortoShape(bool handable);

	// 黑洞吸收时回调,传入会吸收进去的物体
	void blackHoleAbsorption(const vector<CCNode*> absorptions);

	// 开始成功界面时
	void congratulationed();
	
	// 某一张完毕之后
	void seasonFinished(int seasonId);

	void createdARope(CCNode* hungedShape);
	// 绳子砍断之后调用，传回被悬吊的物体
	void ropeCuted(CCNode* hungedShape);

	Achievement getAchievementDefaultInfo(int iAchievementid);
	int getAchievementIdByName(string name);

	//重新加载配置的成就信息
	void reloadAchievementDefaultInfo();
};

#endif