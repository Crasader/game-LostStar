//
//  CustomeSprite.h
//  RedStarFall
//
//  Created by Ryan Wu on 12-9-11.
//
//

#ifndef RedStarFall_CustomeSprite_h
#define RedStarFall_CustomeSprite_h
#include "cocos2d.h"
#include <string>
#include "LFActionGrid3D.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "particle_nodes/CCParticleExamples.h"
#include "particle_nodes/CCParticleSystem.h"
#else
#include "particle_nodes/CCParticleSystem.h"
#include "particle_nodes/CCParticleExamples.h"
#endif

#include "Box2D/Box2D.h"
#include "GameScenseDefine.h"
#include <cmath>
#include <list>
#include <vector>
#include "ResManager.h"
#include "Recorder.h"

using namespace std;

using namespace cocos2d;

#define Tag_Fragment 11111111

#define USE_BLACKHOLE_COVER 1


struct FixtureVelocityInfo {
    b2Fixture* fixture;
    float collisionImpulse; //set by ContactListener, clear by tick
    b2Vec2 linerVelocityBeforeContact;
    b2Vec2 fixtureCenterBeforeContack;
};


//所有需要记录的元素精灵只需要继承RecordSprite类即可
//当创建元素时，自动添加到记录数组里面，当元素移除时，自动移除当前数组，并注册移除事件

class RecordSprite : public CCSprite
{
public:
	RecordSprite();
	~RecordSprite();
	void onEnter();

public:
	record				m_SaveData;
};

class CustomeSprite : public RecordSprite {
public:
    
    CustomeSprite();
	~CustomeSprite();
    
	static CustomeSprite* create(const char *pszFileName);
	static CustomeSprite* createWithTexture(CCTexture2D* pTexture, const CCRect& rect);

    bool initWithFile(const char *pszFilename);
    std::string getFileName();
    //在销毁他的相关的body时候会调用
    virtual void destoryBody();
    void setData(CustomeSprite* data);
    CustomeSprite* getData();
    b2Body* getAnchorBody();
    void setAnchorBody(b2Body* anchor);
    void setMyShadow(CustomeSprite* shadow);
    CustomeSprite* getMyShadow();
    void removeShadowIfPosible();
	//关键元素
	void setIsKeyItem(bool key);
	bool isKeyItem();
	bool isUseedByUser();
	void setUseedByUser();

	/**
	 *	创建一个片段，加在base里面，pos就是相对base的位置
	 *	并设置它的旋转角度
	 */
	void addFragment(const char* fragmentFileName,CCPoint pos,float angle);

	void addFragmentSprite(CCSprite* fragment,CCPoint pos,float angle);

	//从本身拆分碎片
	void splitSpriteToFragment();
	
	/**
	 *	创建一个片段，加在base里面，pos就是相对base的位置
	 *	并设置它的旋转角度
	 */
	void addFragment_ResMgr(const char* fragmentFileName,CCPoint pos,float angle);

	// 碎片爆炸
	// 就是把身上自带的那些碎片拿出来放到图层里面去播放
	// 这之后，外界就可以删除自己这个SPrite了，碎片的那些会自动处理
	void fragmentBomb(CCPoint relativeBombCenterPos);

	void fragmentBomb();

	void pauseFragmentBomb();
	void resumeFragmentBomb();
    
    std::vector<FixtureVelocityInfo> mFixtureVelocityInfoVec;
protected:
    std::string mFileName;
	CCSpriteFrame* mSpriteFrame;
    CustomeSprite* userData;
    ResManager* resMgr;
    CustomeSprite* myShadow;
	bool mIsKeyItem;	// 是否是关键元素
	bool mIsUsedByUser;	// 是否被使用过了
    b2Body* anchorBody;

	// 碎片
	vector<CCSprite*> fragmentVec;
	CCSprite* fragmentBase;			// 碎片的承载体

	void removeNode(CCNode* node);
    
};

//马达,自己是最外面的那一层，就是带动几何体的那一层
class MotorSprite : public CustomeSprite
{
private:
    float innerSpeed;//就是配置文件的值
    float realySpeed;//就是马达真正游戏中旋转的值，发现其实就是innerSpeed
    float transSpeed;

	bool  m_bStop;
    
    float jointBornSpeed;//joint原本的值
    ////最里面的转轴，即第一层
    //CustomeSprite* innerMotor;
    //中间传动层的轴，也就是第2层
    CustomeSprite* transMotor;

	CustomeSprite* stopState;
    
    b2RevoluteJoint* joint;
        
    bool rulable;//这个马达是否可以人工操作。
    
    MotorSprite(bool handleAble,int seasonId,bool moveAble);
    
    void tick(float time);
	virtual void onExit();
public:
    static MotorSprite* createMotor(bool handleAble,int seasonId,bool moveAble);
    
    void stopRotation();
	// 设置背景的大此轮的scale
	void setBgGearScale(float scale);
    
    //切换状态
    void swith(bool mute = false);
    
    void remuseRotation();
    
    void setJoint(b2RevoluteJoint* j);
    
    virtual void destoryBody();
    
    //设置马达的转速，其实就是中心轴的转动速度，这个速度最快
    void setMotorSpeed(float speed);
    //设置马达的真正的转速，也就是实际上带动的物体的转速，即马达实际的转动，就是带动物体的那一层，这个速度最慢
    //测试发现，joint->GetMotorSpeed()就是当时设置的速度，也就是realySpeed也是innerSpeed这么多。
    void setRealyMotorSpeed(float speed);
    
    bool isClickMe(CCPoint glPoint);
    
    bool handlable();
    
	bool getIsStop(){return m_bStop;};

    virtual CCRect boundingBox();
    
    CustomeSprite* bundableShape;//绑定的元素
};

//弹性齿轮，自己是最外面的那一个
class SpringGear : public CustomeSprite
{
private:
    CCSprite* gear_2;
    CCSprite* gear_3;
    CCSprite* gear_4;
    CCSprite* gear_5;//最里面的
    
    float k;//弹性系数，角度越大，扭矩也大
    
//    b2WeldJoint* joint;
    b2RevoluteJoint* rJoint;
    ~SpringGear();
    
    void tick(float time);
public:
    SpringGear(float k);
    virtual void destoryBody();
//    void setWeldJoint(b2WeldJoint* joint);
    void setRotationJoint(b2RevoluteJoint* joint);
    CustomeSprite* bundableShape;//绑定的元素
    
    b2Body* bunableBody;

};

//弹性滑轨
class SpringSlideAnchor : public CustomeSprite
{
private:
    void tickApplayForce(float time)
    {
//        if (j!=NULL) {
//            
//        }
    }
public:
    
//    b2PrismaticJoint* j;
//    b2Body* bundableBody;
    CustomeSprite* bundableShape;//绑定的元素
    
//    SpringSlideAnchor()
//    {
//        j = NULL;
//        bundableBody = NULL;
//        bundableShape = NULL;
//        
//        schedule(schedule_selector(SpringSlideAnchor::tickApplayForce));
//    }
};

//弹性滑轨
class SpringSlide : public CustomeSprite
{
private:
    CCSprite* sprite2;//
    CCSprite* sprite3;//最上层的
        
    b2PrismaticJoint* joint;
    
    float k;
    
    void tick(float time);
public:
    SpringSlide(float k);
    
    void setPrismaticJoint(b2PrismaticJoint* j);
    
    virtual void destoryBody();
    
//    CustomeSprite* bundableShape;//绑定的元素
    
    b2Body* bundableBody;
};

struct DarkHoleBombExplosion
{
	RecordSprite* bombSprite;
	CCGrid3DAction* bombAction;
};

/**
 *	黑洞炸弹的 吸收cache，因为创建的时候容易卡顿
 *	里面存储了爆炸炸开的图片，和他对应的收缩效果动画
 */

class DarkHoleBombExplosionCache
{
public:
	static DarkHoleBombExplosionCache* getCache();

	DarkHoleBombExplosion getBombExplosion();

	void restoreOne(RecordSprite* bombSprite,CCGrid3DAction* bombAction);
	~DarkHoleBombExplosionCache();
private:
//	static DarkHoleBombExplosionCache* cache;
	CCTexture2D* texture;
	vector<DarkHoleBombExplosion> bombVec;


	void newOne();
	DarkHoleBombExplosionCache();
};

//黑洞炸弹
class DarkHoleBomb : public CustomeSprite
{
private:
    float radius;
    float r_power;
    float endScale;

	bool m_bBombed;

    RecordSprite* bombExplosion;
#if USE_BLACKHOLE_COVER
	RecordSprite* bombCover;
#endif
	//CCSprite* bombLighting;

	RecordSprite* bombOutExplosion;
	CCFiniteTimeAction* bombOutAction;

    b2World* world;
	CCParticleSystem* par;
    
    CCGrid3DAction* twirl ;
	vector<CCNode*> outRangeItemToDelete;//炸弹外面的，但是跟炸弹里面东西相关的物体需要删除
    vector<CCNode*> applayBombItems;
    vector<b2Body*> immediatelyDestoryBody;
    

    void bombAction(CCNode* dark);
    void actionEnd(CCNode* dark);

	void bombedOut(CCNode* node);		// 炸弹炸开了之后
	void playBlackholeParticle(CCNode* node);

	void collapse(CCNode* null);//坍缩
	void killSelf(CCNode* nullNul);

public:
    void addOutRangeItem(CCNode* item);

	CCParticleSystem* createFogParticle(CCSprite* item,const char* fileName);

    //创建一个黑洞炸弹，指定他的半径
    DarkHoleBomb(float r,const char* filaName,const char* bombFileName);
    

	bool isInMe(CCNode* item);
	void setToBomb(CCNode* item);
    //爆炸
    void bomb(b2World* world);
    
    void setToImmediatelyDelete(b2Body* toDelete);
    
    ~DarkHoleBomb();
	void playBombAnimation();
	bool getIsBombed(){return m_bBombed;};
};

// 一个闸门
class Strobe : public CustomeSprite
{
private:
    CustomeSprite* leftHead;
    CustomeSprite* rightHead;
    b2World* world;
    b2Body* myCenterBody;
    b2Body* leftBody;
    b2Body* rightBody;
	CCFiniteTimeAction* openAction;
	CCFiniteTimeAction* leftHeadOpenAction;
	CCFiniteTimeAction* rightHeadOpenAction;

    
    int centerLength;//中间的长度，是box2d中的单位
    
    GameToolsStrobe myDiscribt;
    bool open;
    
    Strobe();
    void createHeadSprite(CCLayer* layer);
    
    b2Body* createLeftHead();
    b2Body* createRightHead();
    b2Body* createCenterBody();

    void createBody();
    
    void timeToCloseDoor(float time);
    
    void toRemoveFromParent(CCNode* node);

	virtual void onExit();

	bool anyOtherBodyInLine();
public:
    static Strobe* createStrobe(CCLayer* layerToHoldHead,b2World* w,GameToolsStrobe strobe);
    
    void swicth();

	bool isOpened();
    
    void openTheDoor();
    
    void closeTheDoor();
    
    virtual CCRect boundingBox();
    
    bool pointInMe(CCPoint glPoint);
    
    void destoryBodyByDarkBomb(DarkHoleBomb* bomb);
    
    virtual void destoryBody();
    
    void removeAllIfNeed();
    
    void destoryHead(CCNode* head);
    
    void destoryHeadByDarkBomb(cocos2d::CCNode *head,DarkHoleBomb* bomb);
	void playStoreAnimation(bool bOpen);
	void playDoorAnimation(bool bOpen);
};

//电磁炮
class EMCannonSprite : public CustomeSprite
{
private:
    CustomeSprite* leftHead;
    CustomeSprite* rightHead;
    EMCannon emcannon;
    
    bool isIntercepting;//是否被打断了
    
    int widthBack;//电磁炮的宽度备份
    
    bool closeForBegin;//开始是关闭的
    bool isSingle;
    bool isOpen;//是否是开门状态，也就是没有炮
    b2World* world;
    b2Body* myBody;
    b2Body* leftBody;
    b2Body* rightBody;
    float angle;
    b2Vec2 bodyPos;//闪电，激光的位置
    
    void tick(float time);
    
    b2Body* createHead(b2World* world,b2Vec2 pos,CCSprite* userData);
    void createBody();
    
    void powerOn(bool animation = false);
    void powerOff(bool animation = false);
    float getDistacne(b2Vec2 p1,b2Vec2 p2);
    bool isBodyShelterMe(b2Body* body);
public:
    //构造一个电磁炮
    EMCannonSprite(EMCannon e,b2World* w,CCLayer* layerToAddHead);
    
    void swicth();
    
    //否可以攻击到这个区域。可以就返回true
    bool canAttact(CCRect rect);
    
    bool pointInMe(CCPoint glPoint);
    
    CCRect boundingBox();
    
    b2Body* createLeftHead(CCLayer* headToAdd);
    b2Body* createRightHead(CCLayer* headToAdd);
    
    virtual void destoryBody();
    void destoryBodyByDarkBomb(DarkHoleBomb* bomb);
    
    void removeAllIfNeed();
    
    void destoryHead(CCNode* head);
    void destoryHeadByDarkBomb(cocos2d::CCNode *head,DarkHoleBomb* bomb);   
    
    
    ~EMCannonSprite();
};


//锅炉
class Boiler : public CustomeSprite
{
public:
	static const int Lite_no = 0;
	static const int Lite_1 = 1;
	static const int Lite_2 = 2;	// 比Lite_1更轻量级
private:
	//static bool lite;	//轻量级的
	static int lite_level;
    //反时针
	CCParticleSystem*  m_emitter1;
	CCParticleSystem*  m_emitter2;
	CCParticleSystem*  m_emitter3;
	CCParticleSystem*  m_emitter4;
	CCParticleSystem*  m_emitter5;
	//CCParticleSystem*  m_emitter6;
    ResManager* resMgr;
	CCParticleSystem* autoFirePar[5];
	CCParticleSystem* doorPar;

	float lastUsedMe;	// 上次用我，到现在的间隔
	float emitTime;
	bool firing;		// 是否在fire
	bool mAutoFire;		// 是否正在自动fire
	bool mB_openDoor;	// 是否开门fire了？
    float boiler_angle;
    CCSprite* mBoilerFan;
	CCSprite* mUpDoor;
	CCSprite* mDownDoor;
	CCSprite* mCover;

	vector<b2Body*> mSmokeBoies;
	vector<CCPoint> mEmitterPos;
	vector<float> mEmitterAngle;
	int mCurSmokeIdToUse;

	b2Body* createSmokeBody(b2World* world, float angle);
	
	void initWithState(bool fireing);
    CCParticleSystem* createParticle(CCPoint relativePos,float angle,const char* image);
	void addEmitterPos(CCPoint relativePos, float angle);
	void removeParticle(CCParticleSystem* par);	//抬起之后调用，代表停止
	CCParticleSystem* createAutoFireParticle(CCPoint relativePos,float angle,float life,int num,float speed,const char* image,int lite_level);
	CCParticleSystem* createDoorFireParticle(CCPoint relativePos,const char* image);
	void autoFire();
	void stopAutoFire();

	void openDoorToFire();			// 开门fire
	void closeDoorToStopFire();		// 关门停止fire

	virtual void onExit();
	void stopOpenFire(float t);		// 停止open的fire
	void tickForWaiting(float t);	// 等待的tick，闲时动画
	void emitSingleBody();
public:
    
    b2Body* body;
    
    Boiler();
	~Boiler();

	static void setLite(int lite_level);
    
    void setAngle(float angle);

	void setFan(CCSprite* fan);
    
    //停止当前的粒子系统发射
    void stop();
    
    //按下的时候调用，代表发射了。
    void fire();
    
    bool piontInMe(CCPoint glPoint);
    
	void createSmokeBodies(b2World* world);
	void start();
	//
    size_t mSoundId;
	int mCurSmokeMouseID;
};

//时间减速炸弹
class TimerDec : public CustomeSprite
{
public:
    static TimerDec* createTimerDec(const char* fileName = NULL);
    
    //测试obj是否在我的减速范围之类
    bool isInTimeDecArea(CCNode* obj);
    bool getIsDecing(){return isDecing;};
    //开始减速
    void action();
private:
	size_t soudId;

    TimerDec(const char* fileName = NULL);
	~TimerDec();
    //创建一个粒子系统，并加入到item中，也返回这个粒子系统的指针
    CCParticleSystem* createFogParticle(CCSprite* item,int partNum,float life,float speed,const char* fileName);
    
protected:
    bool isDecing;//是否处于减速期
    
    float radius;//减速半径
    
    CCParticleSystem* par;
	bool m_bSmall;
    
//	void onExit();

    //needSqrt 如果为flase，那么就返回平方值，而不是根号之后的值
    float getDistanceFromMe(CCNode* node,bool needSqrt = true);
};

class TableBack : public CustomeSprite
{
private:
	CustomeSprite* mTable;

	void follow(float time);
	TableBack(CustomeSprite* table,bool needFoot = true);
public:
	static void createTableBg(CustomeSprite* table,bool needFoot = true);
};

class TableBlock : public CustomeSprite
{
private:
	CustomeSprite* mTable;
	CCPoint relativePos1;
	CCPoint relativePos2;
	CCSprite* leftBlock;
	CCSprite* rightBlock;

	CCSprite* leftLight1;
	CCSprite* leftLight2;
	CCSprite* rightLight1;
	CCSprite* rightLight2;

	float tempDistanceSQ;

	float trackAngle;

	CCPoint mStartPos;
	CCPoint mEndPos;

	void follow(float time);

	void updatePos();

	TableBlock(CustomeSprite* table,CCPoint pos1,CCPoint pos2, CCPoint starPos,CCPoint end);
public:
	// 创建桌面卡子，只针对 移动方向跟 桌面不平行的情况
	// 会创建2个卡子，pos1就是 卡子1相对桌面中心的位置，pos2就是卡子2的位置
	static void createTableBlock(CustomeSprite* table, CCPoint pos1 , CCPoint pos2 , CCPoint starPos,CCPoint end);

	void setStartEndPos(CCPoint start,CCPoint end);
};

class BoilerBubble : public CustomeSprite
{
private:
	float mLife;
	float mBaseAlpha;
	float mBaseScale;
public:
	BoilerBubble();
	void step(float dt);
	b2Body* body;
	float mAngle;
};

#endif
