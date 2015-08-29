//
//  BaseContentLayer.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_BaseContentLayer_h
#define demo_box2d_BaseContentLayer_h
#include "CommonDefine.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "ContactListener.h"
#include "GameScenseDefine.h"
//#include "SectionGuide.h"
#include <list>
#include "CCTexture2DMutable.h"
#include "VRope.h"
#include "CustomeSprite.h"
#include "LFLayer.h"
#include "CocoStudio/Armature/CCArmature.h"
#include "GameLayerFactory.h"
#include "cocos-ext.h"
using namespace cocos2d;
using std::vector;

//多久没有动屏幕时，产生流光效果。
#define TIME_FLOWLIGHT_ACTION 4
#define LONG_TIME_ACTION_TAG	231980
class BaseScense;
class SafeBoarder;
class SuperTool;
class Guankamsg;

enum playMode
{
	playMode_Normal = 1,
	playMode_PlayGameRecord,
};

enum Tag
{
	Tag_GuankaLabel = 1,
	Tag_StarStateBar,
	Tag_GoldStart = 545,

	Tag_TaiJieStart = 646,
};

enum Role_Pos
{
	Role_Pos_Left = 0,
	Role_Pos_Right,
	Role_Pos_Middle,
};

class BaseContentLayer: public LFLayer {
	friend class SuperTool;
public:
	BaseContentLayer(BaseScense* scense, playMode curPlayMode = playMode_Normal, std::vector< std::vector<record> >* allRecordData = NULL);
	~BaseContentLayer();
    
    b2Body* createTable(GameTable table);
    void createThrowPoint(ThrowPoint throwPoint);
    b2Body* createShape(GameShape shape,bool isThrowShape = false);
    b2Body* createGameToolsRope(GameToolsRope toolsRope);
    b2Body* createGameToolsBoiler(GameToolsBoiler toolsBoiler);
    b2Body* createGameToolsSpringBed(GameToolsSpringBed toolsSpringBed);
    void createGameToolsStrobe(GameToolsStrobe strobe);
	void createSpecialItem(string fileStr, b2Vec2 pos); // 关卡特殊道具
    
    b2Body* createEMCannon(EMCannon emCannon);
    
    //void removeStrobe(Strobe* s);

    void removeEmCannon(EMCannonSprite* em);
    //void removeBoiler(Boiler* b);

	void setCCBladeVisible(bool visibility);		// 设置光刀是否可见

    static BaseContentLayer* shareBaseContentLayer();
	static void setCurBaseContentLayer(BaseContentLayer* pBaseContentLayer);
	static BaseContentLayer* getPreBaseContentLayer();
	static void setPreBaseContentLayer(BaseContentLayer* pBaseContentLayer);

	void setPTM_RATIO(float ratio);
    
    void shapeMotorDestoried(MotorSprite* motor);
    
    b2Body* checkAnchorWhenShapeDestoryed(CustomeSprite* shape);
    
    void checkShapeWhenAnchorPointDestoryed(CustomeSprite* anchor);
    
	// 开启超能道具
	void openSuperTool();
	// 是否正在用超能道具
	bool isUsingSuperTool();
    //红心被摧毁了...
    void redStarDestoried(CCNode* redStar);
    
    void setFailedWhenRedStarFall(bool flag);
    void setRedStarBeginEffevtive(bool flag);
	void setMinTimeCost(float time){this->min_time_cost = time;};
	void setMinClickNum(int num){this->min_click_num = num;};
    
    void redstarContactTable(b2Vec2 restarPoint,b2Body* contactedTable);
    void redstarEndContactTable();
    
    void springBedContactedBegin(b2Body* springBed,b2Body* contactObj);
    void springBedContactEnd(b2Body* springBed,b2Body* contactObj);
    
    void springBedContact(b2Body* springBed,b2Body* contactObj,float power);
    
    void springBedActionEnd(CCObject* bed);
    
    
	void setWaitingPause(bool pause);

	void removeNode(CCNode* toRemove);
    
    void setBombToExplosion(b2Body* bomb);
    
    void setWind(WindProperty w);
    
    void setToDestory(b2Body* toDestory);
    
    void freezeLayer();
	////设置SectionGuide,以供回调；
 //   void setSectionGuide(SectionGuide* guide);

	//获得需要提示的RedStart位置
	CCPoint getRedStartPos(){return initRedStartPos;}
	//获得需要提示的Table位置
	CCPoint getTablePos(){return lastCreateTablePos;}
	//获得需要提示的Bomb位置
	CCPoint getBombPos(){return lastCreateBombPos;}
	//获得需要提示等待的位置
	CCPoint getWatingtPos(){return timerPos;}
	CCNode* getTimer(){ return getChildByTag(Tag_Timer);};
	//获得当前是否是处于等待状态
	bool isWatting();

	bool getLayerFreezy(){return mLayerFreezed;};
	void setLayerFreezy(bool e){mLayerFreezed = e;};

    b2World* getWorld(){return mWorld;}
    
    CCNode* getCurrentRedStar(){return getChildByTag(Tag_Box_RedStar);};
	b2Body* getCurrentRedStarFromWorld();
    
    bool isAnyFixtureInLine(b2Vec2 startPoint,b2Vec2 endPoint,std::vector<b2Fixture*> except, float testStep = 0.5);//0.5格
    
    //测试test中是否有fixture在线上
    bool isAnyFixtureInLine2(b2Vec2 startPoint,b2Vec2 endPoint,b2Vec2* point,std::vector<b2Fixture*> test, float testStep = 0.5);
    
    //射线：angel=0，则是从左直接到右边,返回相交点
    bool isAnyFixtureInLine2(b2Vec2 startPoint,float angle,b2Vec2* point,std::vector<b2Fixture*> test, float testStep = 0.5);
   
	//float32 getExtensionValue(b2Body* currentBody);
	//bool ignoreMe(b2Body* currentBody);

	//extension为true，那么就是扩展点击范围
    b2Body* whichBodyContainThePoint(b2Vec2 p,bool extension = false);
    b2Fixture* whichFixtureContainThePoint(b2Vec2 p,bool extension = false);
    //获得point点到shape距离。
    float minDistanceFromShape(b2Shape* shape,b2Vec2 point,b2Body* shapeBody);
    //获得point点到vertex[]顶点所构成的多边形的距离。
    float minDistanceFromPolygon(b2Vec2 vertex[] ,int vertexCount,b2Vec2 point);
    
    float distance(b2Vec2 p1,b2Vec2 p2);
    //点到线段的距离
    float minDistanceFromLineSegment(b2Vec2 a,b2Vec2 b,b2Vec2 p);
    //检查所有绳索，在物体销毁时是否需要销毁.destoryAnchorNow那么就会立即销毁锚点
    b2Body* checkRopesWhenBodyDestroied(b2Body* body,bool destoryAnchorNow = false);

	static void addBluredSpriteToMap(std::string fileName);

	void postLfDoubleClickDown(CCPoint glPoint);


	enum _iTag
	{
		diamond_tag = 1223,
	};

private:
    virtual void draw();

	void useSuperToolClicked(CCObject* alertNode);

	void fixScaleBackEnd(CCObject* wraplayer);

	void particleCallBack(CCNode* par);

	// 长时间没操作的回调
	void longTimeNoOpCallback(float t);
public:
	// 执行长时间没操作的动画
	void runNoOpAction();
private:
	void getJuqingDaoJu(CCNode *node);
	// 停止长时间没操作的动画
	void stopNoOpAction();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	virtual void lfDoubleClickDown(CCPoint glPoint);

	void scaleBack();

    CCParticleSystem* createParticle(CCPoint relativePos,float angle,const char* image);

	CCParticleSystem* createRedStarFirePar(CCPoint posVar);
	CCParticleSystem* createRedStarStarPar(CCPoint posVar);
    
    //检查两个点之间的连线是否穿过绳子，如果是，则切断绳子.切断就会返回true
	//isKeyItem 用于查看是否是关键元素
    bool cutRopesWhenPointsThrough(CCPoint pA, CCPoint pB,bool & isKeyItem);

	//得到body的每一个Fixture的中心
	std::list<b2Vec2> getBodyFixtureCeneters(b2Body* body);

	MotorSprite* getClickedMotor(CCPoint glPoint);

    void tick(ccTime dt);
	void specialToolCollide();
    
//    void startRedStarEffective();
//    void startTableEffective();
    void redStartEffectEnd(CCObject* effective);
    
    //初始化效果完毕
    void tableEffectiveEnd(CCObject* effectiveObj);

	// 红星进入了洞里面，这里修改红星的位置和颜色等，把他放在出口
	void redStarGetInTheHole(CCNode* redStar);
    //结束动画完毕之后就会掉这个函数，这里用作显示成功界面
    void endEffectiveEnd(CCNode* redStar);
    
    //t添加桌子刻痕
    void addTableNick(CCNode* table);
    //田间桌子的高光图片,TABLE_HL_TAG
    void addTableHL(const char* fileName,CCNode* table);
    //添加桌子的流光效果,TABLE_FL_TAG
    void addTableFlowLight(CCNode* table);
    
    void flowLightDoAction();

    void runTableflowLightAction(CCSprite* table,CCSprite* flowLight);
    
    void flowLightEnd(CCObject* light);
    
    void addRedStarFlowLight(CCNode* redStar);
    void runRedStarFlowLightAction(CCNode* redStar);
    
    //得到红心左肩的适当的旋转角度，用于跑流光
    float getRedStarFlowLightRotateAngel(CCNode* currentRedStar);
    
    void timeToFlowlightAction(float time);
    
    void bombExplosion(b2Body* bombBody);
    
    void windBlowing();//刮风
    
    void ThrowPointSchedule(ccTime dt);
    void boilerApplyForce(b2Body* boiler, float applyTime);
    
    void timerDecActionEnd(CCObject* dec);
    void timerAscActionEnd(CCObject* dec);
    
    void failed(float time);
    
    b2Body* createTable144(b2Vec2 p, GameMovement move, float32 angle=0, bool effective=true, bool isKeyItem=false);
    b2Body* createTable192(b2Vec2 p, GameMovement move, float32 angle=0, bool effective=true, bool isKeyItem=false);
    b2Body* createTable288(b2Vec2 p, GameMovement move, float32 angle=0, bool effective=true, bool isKeyItem=false);
    b2Body* createTable336(b2Vec2 p, GameMovement move, float32 angle=0, bool effective=true, bool isKeyItem=false);

	void tableBlockFollowCallBack(CCNode* block,void* data);
	void createTableTrack(CCSpriteBatchNode* middleSheet,CCPoint point1,CCPoint point2,CCPoint offset); // 创建台面轨道

	void createMotorTrack(CCSpriteBatchNode* middleSheet,CCPoint point1,CCPoint point2,CCPoint offset); // 创建马达轨道

	// 创建一个轨道
	void createTrack(CCSpriteBatchNode* middleSheet,CCPoint point1,CCPoint point2,CCSprite* leftHead,CCSprite* rightHead,CCPoint offset);
	void createTrack(CCNode* pParent, CCPoint point, float width, float angle);
	void createTableExit();	// 创建出口
    
    b2Body* createTimeDesBomb(b2Vec2 p, float32 angle, float32 desity);
    b2Body* createTimeDesBomb24(b2Vec2 p, float32 angle, float32 desity);
    b2Body* createTimeAscBomb(b2Vec2 p, float32 angle, float32 desity);
    
    b2Body* createBomb48(b2Vec2 p, float32 angle=0, float32 desity=Fixture_Density);
    b2Body* createBomb24(b2Vec2 p, float32 angle=0, float32 desity=Fixture_Density);
    b2Body* createBomb24_Circle(b2Vec2 p, float32 angle, float32 desity);
    b2Body* createBomb48_Circle(b2Vec2 p, float32 angle, float32 desity);
//    //创建一个方块24等。图片是fileName,
    b2Body* createSquare24(b2Vec2 p, float32 angle, float32 desity,const char* fileName,int tag);
    b2Body* createSquare48(b2Vec2 p, float32 angle, float32 desity, const char *fileName, int tag);
    
    b2Body* createDarkHole24(b2Vec2 p, float32 angle, float32 desity);
    b2Body* createDarkHole48(b2Vec2 p, float32 angle, float32 desity);

    b2Body* createBomb48_24(b2Vec2 p, float32 angle=0, float32 desity=Fixture_Density);
    b2Body* createRedStar60(b2Vec2 p, float32 angle=0, float32 desity=Fixture_Density,bool isThrowShape = false);
    
    b2Body* createCross72(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createCross48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density, bool isBullet=false);
    b2Body* createBox48_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox72_48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox96_48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox168_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox192_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox216_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox240_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox264_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox360_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createGrayStar60(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createTriangle56(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createFork24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createT72(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createT48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox96_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox72_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createBox120_24(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createL48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createL72_72(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createL72_48(b2Vec2 p, float32 angle=0, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createCircle48(b2Vec2 p, bool canDestroy=true, float32 restitution=0.0, float32 desity=Fixture_Density);
    b2Body* createCircle24(b2Vec2 p, bool canDestroy=true, float32 restitution=0.0, float32 desity=Fixture_Density);
    b2Body* createCircle72(b2Vec2 p, bool canDestroy=true, float32 restitution=0.0, float32 desity=Fixture_Density);
    b2Body* createHalfCircle96(b2Vec2 p, bool canDestroy=true, float32 desity=Fixture_Density);
    b2Body* createHalfCircle48(b2Vec2 p, bool canDestroy=true, float32 desity=Fixture_Density);

	void preBomb(float time);		// 用于爆炸一次，提前载入炸弹

	//add by jiangjie
	void showRedStarSuccess(CCNode* pRedStar, CCNode* table);
	void showDiamond(cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType movementType, const char *movementID);
	void diamondMoveDone(CCNode* pDiamond);
	void showTableFinishAnim(CCNode* pRedStar);
	void runToDiamond(CCNode* pNode);
	void thiefMoveDone(CCNode* pThief);
	void layerDown();
	void layerUpOver();
	void layerUp();
	void thiefLeaveOut(CCNode* pThief);
	
	const ConfigSectionInfo* m_curLevelinfo;
	
public:
	void onEnter();

	void reallyStart(CCNode* pNode);

	void showCongratulationLayer();
	
	std::vector<RecordSprite*>* getAllNeedRecordSpriteVec(){return &m_AllNeedRecordSpriteVec;};

	void addToRecordSpriteVec(RecordSprite* pNode)
	{
		m_AllNeedRecordSpriteVec.push_back(pNode);
	};
	void addToWillBeRemoveRecordSpriteVec(RecordSprite* pNode)
	{
		if (!pNode)
		{
			return;
		}

// 		if (m_iCurPlayMode != playMode_Normal)
// 		{
// 			CCLOG("--------------------------------------> m_iPlayFrame is %d, id is %d", m_iPlayFrame, pNode->m_SaveData.id);
// 		}
// 		else if (m_bRecord)
// 		{
// 			CCLOG("--------------------------------------> m_iCurRecordFrame is %d, id is %d", m_iCurRecordFrame, pNode->m_SaveData.id);
// 		}
		m_WillBeRemoveRecordSpriteVec.push_back(pNode);
	};

	void SaveData();

	void starPlay();
	void playRecord(float dt);
	void recordEventID(RecordSprite* pNode, int iEventID);

	int getCurRecordFrameIndex(){return m_iCurRecordFrame;};

	int getCurRecordCount(){return m_iRecordTotalCount;};
	void setCurRecordCount(int icount){m_iRecordTotalCount = icount;};

	std::vector< std::vector<record> >*  getAllFramesData(){return &m_AllFramesData;};

	bool getRecord(){return m_bRecord;};
	void setRecord(bool e){m_bRecord = e;};

	playMode getCurPlayMode(){return m_iCurPlayMode;};

	vector<VRope*>* getRopes(){return &mRopes;};

	std::vector<record>*  getCurFrameData(){return &m_CurFrameData;};
	void removeFromRecordSpriteVec(vector<RecordSprite*>& willRemoveFromVec);
	void updateRecordInfo();
	void addToRopes(std::vector<VRope*> &vopes);
	void removeRopeWhenPlayRecord(int frameIndex);
	void addPlayRecordFinger(CCNode* pNode);

	void begineThrowPointSchedule(float time);
	void endThrowPointSchedule();

	//动画相关
	void MovementEventCallFunc(cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType movementType, const char *movementID);
	
	void initRandomArmatureVec();
	bool getAnimationDone(){return m_bAnimationOver;};
	void showGameImmediately();
	void playPaoArmature();
	void stopArmature();
	cocos2d::extension::CCArmature* getArmature(){return m_pArmature;};
	void playZuo(CCNode* pNode);
	void playOrPause(CCObject* pObj);
	void showPlayOrPauseBtn(cocos2d::extension::CCScrollView* pParent, bool bPlay);
	void setRoleVisible(bool e);

	void setPlayRecord(bool e){m_bPlayRecord = e;};
	bool getPlayRecord(){return m_bPlayRecord;};

	void allItemFadeTo(CCNode* pNode, float time, int fade);
	void playZuoXia(CCNode* pNode);

	void addTaiJie(CCPoint point = CCPointZero);
	void addGuanKaName(CCNode* pParent);
	void setInfoVisible(bool e);

    void setLayerFreezyTrue();
	Guankamsg* showGuanKaMsgLayer();

	void showBaseContentLayerInAnimation();
	void BaseContentLayerInAnimationOver();
    
    void startAnimation(float dt);
	void removeSuperTool();

	void playBombEffectBySize(int bombSize);
	void showSuperToolGuide();

	void checkDiamondPos(float dt);
	int getEffectLevel(float dis);

	bool getSuccessful(){return m_bSuccessful;};
	void setScuessful(bool e){m_bSuccessful = e;};
private:
	float			m_fTickTotalTime;

	int				m_curSeasonID;
	int				m_curSectionID;

	int				m_iEffectLevel;
	int				m_iPlayEffectTimes;
	bool			m_bPlayEffect;
	CCPoint			m_fDiamondPreviousPosition;

	int				m_iCurRecordFrame;
	int				m_iRecordTotalCount;
	size_t	m_iPlayFrame;
	
	bool			m_bSuccessful;
	bool			m_bRecord;
	playMode		m_iCurPlayMode;
	bool			m_bAnimationOver;
	long long		m_llLastTouchBeginTime;
	bool			m_bPlayRecord;
	cocos2d::extension::CCArmature*		m_pArmature;

	std::vector<record>					m_CurFrameData;
	std::vector< std::vector<record> >  m_AllFramesData;
	std::vector<RecordSprite*>			m_CurNeedRecordSpriteVec;
	std::vector<RecordSprite*>			m_AllNeedRecordSpriteVec;
	std::vector<RecordSprite*>			m_WillBeRemoveRecordSpriteVec;

	std::vector<CCNode*>				m_CollisionWithRoleNode;

	std::map<float, CCPoint>			m_TouchMovedPointsMap;

	std::vector<string>					m_randomArmatureVec;
	std::vector<string>					m_tempRandomArmatureVec;

	bool			m_bShowRanodomArmature;	

	//end
    
    //冻结layer，不再做任何处理。整个layer休眠
    bool mLayerFreezed;
    
    bool timeAscing;//时间加速？？？
    
//    bool isFlowLightAction;
    
    float redStarFadeTime;
    float redStatDelayTime;
    
    b2World* mWorld;
    

	//SectionGuide* guide;

	CCParticleSystem* redStar_FirePar;
	CCParticleSystem* redStar_StarPar;
    

    WindProperty wind;

	CCPoint mTouchStarPoint;
	bool isMoving;
    BaseScense* mScense;
	ContactListener* mContactListener;
	b2ContactFilter* mContactFilter;
    CCLayer* touchTrailLayer;
    //红星是否接触到桌面
    bool mRedstarContactTable;
	//红星接触到的桌面
	b2Body* contactedTable;
    //红星与桌面中心点的相对位置
    b2Vec2 mPositionRedstarFromTable;
    //红星接触桌面并停止移动后所经历的时间
    ccTime mRedstarContactTableTime;

	CCNode* m_pSuccessRedSar;
	CCNode* m_pSuccessPlatform;
    
    time_t mLastBoxDestroyTime;
    
	CCSprite* mSpriteToDisappear;

    //几何体消失坠落的时长
    float mSpriteToDisappearFallTime;
    //几何体坠落过程中旋转速度
    float mSpriteToDisaappearFallRotation;
    //几何体X方向初始速度
    float mSpriteToDisappearSpeedX;

	float min_time_cost;
	float min_click_num;

    //
    std::vector<b2Body*> mBombToExpolsionVec;
    
    std::list<TimerDec*> mTimerDecList;
    
    std::vector<MotorSprite*> motorVec;
    
    //std::vector<Strobe*> strobeVec;
    
    std::vector<EMCannonSprite*> emVec;
    
    std::vector<b2Body*> toDestoryVec;
    
    //std::vector<Boiler*> boilerVec;

	int boilerNum;	// 记录锅炉的个数，用于当锅炉太多的时候，就使用lite版
    
    

	CCPoint initRedStartPos;
	CCPoint lastCreateTablePos;
	CCPoint lastCreateBombPos;
	CCPoint timerPos;
 
    //投掷点
    struct ThrowPointData{
        ThrowPoint throwPoint;
        unsigned int lastThrowTime; //单位 ThrowPoint_MinTimeGap
    };
    std::vector<ThrowPointData> mThrowPointDataVec;
    
	//struct GameTable

    //table移动
    struct GameTableMovementData{
        GameMovement movement;
        b2Body* body;
        bool currentGoAhead; //当前是前进还是后退的标志
        
        CCSprite* ghost;
        CCPoint ghostLastTickPosition;
        
        GameTableMovementData(){
            currentGoAhead = true;
        }
    };
    std::vector<GameTableMovementData> mGameTableMovementDataVec;
    
    bool mFailedWhenRedStarFall;
    
    bool needRedStarBeginEff;
    
    //表示当前是否已经存在红星。如果存在则不能再抛掷红星了
    bool mRedstarExist;
    
    //rope
    CCSpriteBatchNode* mRopeSpriteBatchNode;
	// 台面的轨道
	CCSpriteBatchNode* track;

	CCSpriteBatchNode* motorTrackBatchNode;

    std::vector<VRope*> mRopes;
    CCPoint mToucheMovePointPrev; //记录上一个touchmove的点, x,y < 0 表示无效
    std::vector<std::vector<CCSprite*> > mRopesCuted; //已经被砍断的绳子，需要做相应消失效果处理
    
    //boiler
    b2Body* mActiveBoiler;
    
    CCFiniteTimeAction* springBedAction;
    
    std::vector<CCNode*> effectveTableVec;
    std::vector<CCNode*> effectveRedStarVec;
    
	unsigned int mDidaSoundId;
	bool mIsLastClickedItem;

	SafeBoarder* mSafeBoarder;
	SuperTool* mSuperTool;
	bool mUseSuperTool;
	bool mCreateSuperToolWhenScaleBack;
    
    //2 points
    CCPoint _pointA;
    CCPoint _pointB;

protected:
    CCSize mScreenSize;
};

#endif
