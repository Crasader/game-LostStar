//
//  GameScenseDefine.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.;
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.;
//

#ifndef demo_box2d_GameScenseDefine_h
#define demo_box2d_GameScenseDefine_h
#include "cocos2d.h"
#include "CommonDefine.h"
#include <vector>
#include "Box2D/Box2D.h"
#include "cocos-ext.h"
#include "Recorder.h"

using namespace cocos2d;

#define CurainLayerTag 1122334455

enum GameTableType {
	GameTableType_Table144 = 1,	//6*1
	GameTableType_Table192,		//8*1
	GameTableType_Table288,		//12*1
	GameTableType_Table336,		//14*1
};

enum GameShapeType {
    GameShapeType_Box48 = 1,//2*2
    GameShapeType_Box24,	//1*1
    GameShapeType_Box48_24,	//2*1
    GameShapeType_Box72_24,	//3*1
    GameShapeType_Box96_24,	//4*1
    GameShapeType_Box120_24,//5*1
    GameShapeType_Box72_48,	//3*2
    GameShapeType_Box96_48,	//4*2
    GameShapeType_Box168_24,//7*1
    GameShapeType_Box192_24,//8*1
    GameShapeType_Box360_24,//15*1
    GameShapeType_Cross72,
    GameShapeType_Cross48,
    GameShapeType_RedStar60,
    GameShapeType_GrayStar60,
    GameShapeType_Triangle56,
    GameShapeType_Fork24,
    GameShapeType_Bomb48,
    GameShapeType_Bomb24,
    GameShapeType_Bomb48_24,
    GameShapeType_T72,
    GameShapeType_T48,
    GameShapeType_L48,
    GameShapeType_L72_48,
    GameShapeType_L72_72,
    GameShapeType_Circle48,
    GameShapeType_Circle24,
    GameShapeType_HalfCircle96,
	GameShapeType_Circle72,		//2012-8-1新加;
	GameShapeType_HalfCircle48,	//2012-8-1新加;
	GameShapeType_Box216_24,	//2012-8-1新加;//9*1
	GameShapeType_Box240_24,	//2012-8-1新加;//10*1
	GameShapeType_Box264_24,	//2012-8-1新加;//11*1
//    GameShapeType_SpringBall24, //弹性球
    
    GameShapeType_LocalTimeDecBomb,//局部的时间减速炸弹
    GameShapeType_LocalTimeDecBomb_24,//局部的时间减速炸弹
    GameShapeType_GlobalTimeAscBomb,//全局的时间加速炸弹

    GameShapeType_Bomb24_Circle,//圆形炸弹
    GameShapeType_Bomb48_Circle,
    GameShapeType_Bomb24_Hard,//不可摔碎
    GameShapeType_Bomb48_Hard,
    GameShapeType_Bomb24_DarkWhole,//黑洞
    GameShapeType_Bomb48_DarkWhole,
    
    };
struct GameMovement {
    bool enable; //true表示move有效;
    b2Vec2 moveStartPoint;
    b2Vec2 moveBackPoint;
    float duration; //startPoint与backPoint之间作一次往返移动的时间;
    
    GameMovement(){
        enable = false;
    }
};

struct GameShapeRotation {
    bool enable;
    bool stopForBegin;//开始是停止的
    bool handleAble;//是否可以人工控制；
    
    int rotationType;//旋转方式：弹性旋转和自动旋转，默认是自动旋转，就是以前的旋转方式
    
    b2Vec2 anchorPosition; //旋转中心在shape中心的相对位置
    float motorSpeed; //每秒旋转角度，可为负
    float maxMotorTorque; //马达的最大扭矩
    
    GameMovement move; //anchor的movement
    
    GameShapeRotation(){
        enable = false;
        stopForBegin = false;
        handleAble = false;
        rotationType = GAME_ROTATION_Type_Free;
    }
};

//弹性滑轨
struct GameShapePrismaticSpring
{
    bool enable;
    float angle;//滑动方向，默认就以几何体本身的角度,相对于几何体的角度
    float elasticForce;//弹力
    float maxDistacne;//弹性的最大限度,一般不需要配置
    b2Vec2 anchorPosition; //控制中心在shape中心的相对位置
    
    GameShapePrismaticSpring()
    {
        enable = false;
        angle = 90.0f;//默认90度，就是竖直的
        elasticForce = 0.0f;
        maxDistacne = 3.0f;
        anchorPosition.Set(0, 0);
    }
    
    GameShapePrismaticSpring(float angle,float elasticForce)
    {
        enable = false;
        this->angle = angle;
        this->elasticForce = elasticForce;
        this->maxDistacne = 3.0f;
        this->anchorPosition.Set(0, 0);
    }
    
    GameShapePrismaticSpring& operator = (const GameShapePrismaticSpring& obj){
        enable = obj.enable;
        angle = obj.angle;
        elasticForce = obj.elasticForce;
        maxDistacne = obj.maxDistacne;
        anchorPosition = obj.anchorPosition;
        return *this;
    };
    
};


struct  GameTable {
    GameTableType type;
    b2Vec2 point;
    float32 angle;
    bool effective; //红星掉落在上面是否有效;
	bool is_key_item;
    GameMovement move;
    
    GameTable(GameTableType t, b2Vec2 p, float32 a=0, bool e=true){
        type = t;
        point = p;
        angle = a;
        effective = e;
		is_key_item = false;
        
    };
    
    GameTable& operator = (const GameTable& obj){
        type = obj.type;
        point = obj.point;
        angle = obj.angle;
        effective = obj.effective;
		is_key_item = obj.is_key_item;

        move = obj.move;
        return *this;
    };
    
    void setMovement(GameMovement m){
        move = m;
    }
};


struct GameShape {
    GameShapeType type;
    float32 angle;
    bool canDestroy;
    b2Vec2 point;
    float32 restitution;
	float32 density;
	bool isBullet;
	bool is_key_item;
    
    GameShapeRotation rotation;//旋转，不能和弹性一同开启
    GameShapePrismaticSpring spring;//弹性，不能和旋转一同开启
    
	GameShape()
	{
		point = b2Vec2(0,0);
		angle = 0;
		canDestroy = true;
		restitution = 0.0f;
		density = Fixture_Density;
		isBullet = false;
		is_key_item = false;
	}
    GameShape(GameShapeType t, b2Vec2 p, float32 a=0, bool c=true, float32 r=0.0,float32 d = Fixture_Density,bool b = false){
        type = t;
        point = p;
        angle = a;
        canDestroy = c;
        restitution = r;
		density = d;
		isBullet = b;

		is_key_item = false;
    };
    
    GameShape& operator = (const GameShape& obj){
        type = obj.type;
        angle = obj.angle;
        point = obj.point;
        canDestroy = obj.canDestroy;
        restitution = obj.restitution;
        density = obj.density;
		isBullet = obj.isBullet;
		is_key_item = obj.is_key_item;
        
        return *this;
    };
};

struct ThrowShape {
    GameShape shape;
    
    b2Vec2 initForce; //被投掷物品的初始受力;
    float initTorque; //被投掷物品的初始扭矩;
    
    ThrowShape(GameShapeType t, b2Vec2 p, float32 a=0, bool c=true)
    :shape(t, p, a, c)
    {
    };
};
struct ThrowPoint {
    b2Vec2 point; //投掷点;
    std::vector<ThrowShape> shapeVec; //投掷的物品，循环投掷;
    unsigned int timeGap; //投掷时间间隔单位:ThrowPoint_MinTimeGap;
};

//弹簧床
struct GameToolsSpringBed;
//锅炉
struct GameToolsBoiler;
//绳子
struct GameToolsRope;
//闸门
struct GameToolsStrobe;
//电磁炮
struct EMCannon;


//风的属性
struct WindProperty
{
    bool enable;
    
    b2Vec2 windPos;//风力点
    
    float angle;//风向
    
    float force;//风力
    
    WindProperty()
    {
        enable = false;
        
        //默认风力点在屏幕的正左边
        windPos.Set(-2,CCDirector::sharedDirector()->getWinSize().height/2/PTM_RATIO);
        
        angle = 0.0f;//默认向又右吹，角度为0嘛。逆时针方向计算角度
        
        force = 0.0f;
    }
};


//弹簧床
struct GameToolsSpringBed {
    float restitution;
    b2Vec2 point;
};

//锅炉
struct GameToolsBoiler {
    b2Vec2 point;
	bool is_key_item;
	GameToolsBoiler()
	{
		is_key_item = false;
	}
};

//绳子
struct GameToolsRope {
    b2Vec2 anchorPoint;
    GameShape shapeToHang;
    float ropeLength;
	bool is_key_item;
    
    GameToolsRope(b2Vec2 p, float len, GameShape shape)
    :shapeToHang(shape){
        anchorPoint = p;
        shapeToHang = shape;
        ropeLength = len;
		is_key_item = false;
    }
};


//电磁炮
struct EMCannon
{
    string em_type;
    //电磁炮的宽度,对于单炮，这个属性没用,是物理单位世界的，1则是24
    float width_PTM_RATIO_num;
    
    bool closeForBegin;//开始是关闭的
    
    bool isLeft;//是否是左边的炮，即从左向右发射。只针对单炮
    
    //电磁炮的位置，对于“对炮”,那么这个Pos就是两个炮的中点
    //对于单炮的话，这个点就是这个炮的位置。
    b2Vec2 pos;
    
    float angle;
    
    EMCannon()
    {
        em_type = EMCannon_type_Single;
        isLeft = true;
        angle = 0.0f;
        closeForBegin = false;
    }
    EMCannon(string typeStr,int w,b2Vec2 p)
    {
        if (typeStr != EMCannon_type_Double && typeStr != EMCannon_type_Single) {
            CCAssert(false, "电磁炮的类型只能是EMCannon_Type_Single或者EMCannon_Type_Double");
        }
        em_type = typeStr;
        width_PTM_RATIO_num = w;
        pos = p;
        angle = 0.0f;
        closeForBegin = false;
    }
    
    EMCannon& operator = (const EMCannon& obj){
        em_type = obj.em_type;
        pos = obj.pos;
        isLeft = obj.isLeft;
        angle = obj.angle;
        closeForBegin = obj.closeForBegin;
        width_PTM_RATIO_num = obj.width_PTM_RATIO_num;
        return *this;
    };
};


struct GameToolsTransportGate {
    b2Vec2 pointA;
    b2Vec2 pointB;
};

//闸门，一个悬空的装置
struct GameToolsStrobe
{
    b2Vec2 point;
    float32 angle;
	bool is_key_item;

    GameToolsStrobe()
    {
        //GameToolsStrobe(b2Vec2(0,0));//这样调用没有用
		angle = 0;
		is_key_item = false;
    };
    
    GameToolsStrobe(b2Vec2 p, float32 a=0)
    {
        point = p;
        angle = a;
		is_key_item = false;
    };
    
    
    GameToolsStrobe& operator = (const GameToolsStrobe& obj){
        point = obj.point;
        angle = obj.angle;
		is_key_item = obj.is_key_item;
        return *this;
    };
};

//一个layer就是一关;
struct GameLayerInfo {
	std::vector<GameTable> gameTableVec;
	std::vector<GameShape> gameShapeVec;
	std::vector<ThrowPoint> throwPointVec;
	std::vector<GameToolsSpringBed> springBedVec;
	std::vector<GameToolsBoiler> boilerVec;
	std::vector<GameToolsRope> ropeVec;
	std::vector<EMCannon> emCannonVec;//电磁炮
	std::vector<GameToolsStrobe> strobeVec;
	bool failedWhenRedStarFall;
	bool needRedStarBeginEffective;		//是否需要开场动画
	int min_click_num;					// 本关最小可以在点击了多少下之内过
	float min_time_cost;				// 本关最小可以在多少秒之内过
	float initScale;
	CCPoint initScaleCenterPos;
	WindProperty wind;

	GameLayerInfo(){
		failedWhenRedStarFall = true;
		needRedStarBeginEffective = true;
		min_click_num = -1;
		min_time_cost = -1;

		// 初始化的缩放
		initScale = 1.0f;
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		initScaleCenterPos = ccp(winSize.width/2,winSize.height/2);
	}

	int getKeyItemNum()			// 获得关卡的关键元素个数
	{
		int num = 0;
		for (size_t i=0;i<gameTableVec.size();i++)
		{
			if (gameTableVec[i].is_key_item)
				num ++;
		}

		for (size_t i=0;i<gameShapeVec.size();i++)
		{
			if (gameShapeVec[i].is_key_item)
				num ++;
		}

		for (size_t i=0;i<throwPointVec.size();i++)
		{
			for (size_t j=0;j<throwPointVec[i].shapeVec.size();j++)
			{
				if (throwPointVec[i].shapeVec[j].shape.is_key_item)
				{
					num ++;
				}
			}
		}

		for (size_t i=0;i<boilerVec.size();i++)
		{
			if (boilerVec[i].is_key_item)
				num ++;
		}

		for (size_t i=0;i<ropeVec.size();i++)
		{
			if (ropeVec[i].is_key_item)
				num ++;
		}

		for (size_t i=0;i<strobeVec.size();i++)
		{
			if (strobeVec[i].is_key_item)
				num ++;
		}
		return num;
	}
};

class BaseContentLayer;

class GameScenseDefine {
private:
    GameScenseDefine();
    ~GameScenseDefine();
    
    static GameScenseDefine* _GameScenseDefine;
    
    std::vector<GameLayerInfo> mGameLayerVec;

	std::vector<BaseContentLayer*>		m_AllBaseContentLayerVec;
public:
    static GameScenseDefine* SharedGameScenseDefine();
    
    int getSceneTotalCount();
    
    //autorelease
    CCScene* createScenseByIndex(int index,bool needCurtain = true);

	std::vector<BaseContentLayer*>* getAllBaseContentLayerVec(){return &m_AllBaseContentLayerVec;};
	void removeBaseContentLayer(BaseContentLayer* pLayer)
	{
		for (size_t i = 0; i < m_AllBaseContentLayerVec.size(); i++)
		{
			if (m_AllBaseContentLayerVec.at(i) == pLayer)
			{
				m_AllBaseContentLayerVec.erase(m_AllBaseContentLayerVec.begin()+i);
				break;
			}
		}
	}
	cocos2d::extension::CCScrollView* playRecord(int seasonID, int sectionID, std::vector< std::vector<record> >* allRecordData, bool bAutoPlay /*= false*/);

	void createBaseContentLayer(int seasonid, int sectionid);
};

#endif





