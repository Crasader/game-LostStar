//
//  CommonDefine.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_CommonDefine_h
#define demo_box2d_CommonDefine_h

typedef float ccTime;

extern char g_BoundleResourcePath[256];
extern bool g_IsRetina;



#define EMCannon_type_Single "EMCannon_Type_Single"
#define EMCannon_type_Double "EMCannon_Type_Double"

#define GAME_ROTATION_Type_Free 0//自由旋转
#define GAME_ROTATION_Type_Resilience 1//弹性旋转

#define LOGO_TIME 2.0f

//#define PTM_RATIO 24.0f
extern float PTM_RATIO;
#define PTM_RATIO_INIT	24.0f

//炸弹最小的有效范围
#define BOMB_MIN_EFFETIVE_DIS 6

//图形的基准单位
#define BasicLength 24.0f

//每一小节的最高得分是多少，也就是满分是多少
#define SECTION_FULL_SCORE 3

//投掷点投掷最小时间间隔，秒
#define ThrowPoint_MinTimeGap 1.0f

#define Time_Dec_Gap 10.0f

#define Time_Asc_Gap 10.0f

#define Time_DoorOpen 5.0f//门开起后持续时间

//
#define Tag_DarkBomb_bombCover 23453
#define Tag_DarkBomb_bombExplosion 23454
#define Tag_In_DarkBomb 23455
#define Tag_Box_can_Destroied 23456
#define Tag_Box_cannot_Destroied 23457
#define Tag_Box_Bomb 23458
#define Tag_Timer_Dec_Bomb 23459
#define Tag_Timer_Asc_Bomb 23460
#define Tag_Box_Bomb_Cirecle 23461
#define Tag_Box_Bomb_Hard 23462
#define Tag_Box_Bomb_DarkWhole 23463
#define Tag_Box_Frost		   23464
#define Tag_Box_Frost_Par	23465
#define Tag_Boiler_Bubble      23500

#define Tag_StopGame3Layer	96675

#define Tag_Box_DarkHoleBomb_Par	33459
#define Tag_Box_RedStar_Mask 33458
#define Tag_Box_GoldenStar 33457
#define Tag_Box_RedStar 33456
#define Tag_Box_Table 43456
#define Tag_Box_Table_Ineffective 43457
#define Tag_Box_Boiler 53458
#define Tag_Box_Rope_Anchor 8888888
#define Tag_Box_Rope 8888889
#define Tag_Box_RopeBatchSprite 8888890

#define Tag_Box_SpringBed 53457
#define Tag_Timer 789898
#define Tag_Timer_Boarder 799999
#define Tag_SpringSlide_Cricle 789999//弹性滑轨上的，安装在Shape上面的圆圈
#define Tag_SafeBoard_L 999000	//安全边框,左
#define Tag_SafeBoard_R 999001	//安全边框,右
#define Tag_SafeBoard_T 999002	//安全边框,上
#define Tag_SafeBoard_D 999003	//安全边框,下

#define Tag_Mator_Turn_Anchor 1000000

#define Tag_Anchor_Mator 1000001 //马达
#define Tag_Anchor_SpringGear 1000002 //弹性此轮
#define Tag_Anchor_SpringSlide 1000003 //弹性滑轨

#define Tag_Box_Strobe_Head 63454//闸门的头
#define Tag_Box_EMCannoon_Head 63455//电磁炮的头
#define Tag_Box_Strobe 63456//闸门
#define Tag_Box_EMCannoon 63457//电磁炮

#define Tag_SpecialItem 30000 //关卡特殊道具
#define Tag_SpecialItem_Copy 30001 //关卡特殊道具副本


#define Fixture_Density 1.0f
#define Fixture_Friction 0.18f
#define Fixture_Restitution 0.0f

//红星在桌面上停留多久算成功
#define RedstarStayOnTable_TimeToSuc 1.0f

//多久时间可以点击第二次
#define TouchDestroyTimeGap 1.5

//炸弹威力系数
#define BombExplosionPower 2000.0f
//炸弹撞击敏感度系数
#define BombImpulseSensitivity 18

#define SpringBedSensity 5

//下面定义教学配置的信息
#define ALL -1	//代表全部

//对于红星，有效桌面等都是在第一章第一节提示，这里就不配置了

#define RED_STAR_PROMPT_TIME 0.5f	//红星的指示时间


#define BOMB_PROMPT_SEASON_ID	1 //炸弹提示的SeasonId
#define BOMB_PROMPT_SECTION_ID	14 //炸弹提示的SectionId,如果为ALL，则全部提示

#define CAN_NOT_DESTORY_PROMPT_SEASON_ID 1	//不可以消除的几何体，在点击时需要提示，那么需要提示的场景Id是这么多
#define CAN_NOT_DESTORY_PROMPT_SECTION_ID	ALL		//代表第一章的每一个Section，对于不可点击的n都提示

#define WAITING_PROMPT_SEASON_ID 1	//等待显示的图标的提示章节
#define WAITING_PROMPT_SECTION_ID ALL //第一章都全部提示

//锅炉最大有效范围
#define Boiler_MAX_EFFETIVE_DIS 6
//锅炉力量系数
#define BoilerForcePower 2000.0f


#endif
