#include "GuideLayerFactory.h"
#include "GuideLayer.h"
#include "GuideLayer_SuperTool.h"
#include "GuideLayer_ClickFinger.h"
#include "GuideLayer_CannotDestory.h"
#include "GuideLayer_DoubleTable.h"
#include "GuideLayer_Bomb.h"
#include "GuideLayer_SpringBall.h"
#include "GuideLayer_SpringBed.h"
#include "GuideLayer_Strobe.h"
#include "GuideLayer_CutRope.h"
#include "GuideLayer_ClickMotor.h"
#include "GuideLayer_CanotUseMotor.h"
#include "GuideLayer_UseBolier.h"
#include "GuideLayer_FreazyBomb.h"
#include "GuideLayer_ClickBlackHole.h"
#include "GuideLayer_DoubleClick.h"
#include "GuideLayer_JuqingDaoju.h"
#include "VersionControl.h"
#include "GameLayerFactory.h"
#include "UserDataManager.h"
#include "GuideLayer_Score.h"

#define OPEN_GUIDE	1

GuideLayerFactory* GuideLayerFactory::mFactory;

GuideLayerFactory* GuideLayerFactory::getFactory()
{
	if (!mFactory)
	{
		mFactory = new GuideLayerFactory();
	}
	return mFactory;
}
GuideLayerFactory::GuideLayerFactory()
{
	currentGuideLayer = NULL;
}

BaseGuideLayer* GuideLayerFactory::getGuideLayerNormal(int seasonId,int sectionId)
{
	BaseGuideLayer* layer = NULL;
	// 初始化layers，如果当前的seasonId等于SEASONID，sectionId等于SECTIONID，那么就会new一个layer
	// 并且是autorelease的
	std::map<int, BaseGuideLayer*> GuideLayersMap;

#define INIT_LAYERS(GUIDELAYSMAP,SEASONID,SECTIONID,GUIDE_LAYER_NAME) \
	layer = new GUIDE_LAYER_NAME(SEASONID,SECTIONID); \
	GUIDELAYSMAP.insert(std::pair<int, BaseGuideLayer*>((SEASONID-1)*GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId)+SECTIONID, layer));\
	layer->autorelease();

#if OPEN_GUIDE != 0
#if Version_Free != 0
	//教学相关
	INIT_LAYERS(GuideLayersMap,1,1,GuideLayer);
	INIT_LAYERS(GuideLayersMap,1,2,GuideLayer_ClickFinger);
	INIT_LAYERS(GuideLayersMap,1,3,GuideLayer_CannotDestory);
	INIT_LAYERS(GuideLayersMap,1,4,GuideLayer_Bomb);
	INIT_LAYERS(GuideLayersMap,1,5,GuideLayer_CutRope);
	INIT_LAYERS(GuideLayersMap,1,6,GuideLayer_ClickMotor);
	INIT_LAYERS(GuideLayersMap,1,7,GuideLayer_CanotUseMotor);
//	INIT_LAYERS(GuideLayersMap,1,8,GuideLayer_Score);		//不要这个评分的教学了
	INIT_LAYERS(GuideLayersMap,1,9,GuideLayer_Strobe);
	INIT_LAYERS(GuideLayersMap,1,10,GuideLayer_SpringBed);
#else
	//教学相关
	INIT_LAYERS(GuideLayersMap,1,1,GuideLayer);
	INIT_LAYERS(GuideLayersMap,1,2,GuideLayer_ClickFinger);
//	INIT_LAYERS(GuideLayersMap,1,3,GuideLayer_Score);		//不要这个评分的教学了
	INIT_LAYERS(GuideLayersMap,1,4,GuideLayer_CannotDestory);
//	INIT_LAYERS(GuideLayersMap,1,5,GuideLayer_JuqingDaoju);
	INIT_LAYERS(GuideLayersMap,1,6,GuideLayer_DoubleTable);
	INIT_LAYERS(GuideLayersMap,1,7,GuideLayer_CutRope);
	INIT_LAYERS(GuideLayersMap,1,/*9*/8,GuideLayer_Bomb);
	INIT_LAYERS(GuideLayersMap,1,/*12*/11,GuideLayer_SpringBall);
	INIT_LAYERS(GuideLayersMap,1,/*13*/12,GuideLayer_SpringBed);
	INIT_LAYERS(GuideLayersMap,1,/*16*/14,GuideLayer_Strobe);
//	INIT_LAYERS(GuideLayersMap,1,/*19*/16,GuideLayer_SuperTool);
//	INIT_LAYERS(GuideLayersMap,1,20,GuideLayer_DoubleClick);  //缩放不要了
	INIT_LAYERS(GuideLayersMap,2,1,GuideLayer_ClickMotor);
	INIT_LAYERS(GuideLayersMap,2,6,GuideLayer_CanotUseMotor);
	INIT_LAYERS(GuideLayersMap,3,1,GuideLayer_UseBolier);
	INIT_LAYERS(GuideLayersMap,4,1,GuideLayer_FreazyBomb);
	INIT_LAYERS(GuideLayersMap,5,1,GuideLayer_ClickBlackHole);
#endif
#endif

	std::map<int, BaseGuideLayer*>::iterator l_it = GuideLayersMap.find((seasonId-1)*GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId)+sectionId);
	if (l_it != GuideLayersMap.end())
	{
		layer = l_it->second;
	}
	else
	{
		layer = NULL;
	}
	if (layer == NULL)
	{
		// 默认情况
		layer = new BaseGuideLayer(seasonId,sectionId);
		layer->autorelease();
	}
	currentGuideLayer = layer;
	return layer;
}

BaseGuideLayer* GuideLayerFactory::getGuideLayerLite(int seasonId,int sectionId)
{
	BaseGuideLayer* layer = NULL;

	do 
	{
		// 指定的情况
		if (seasonId == 1 && sectionId == 1)
		{
			layer = new GuideLayer(seasonId,sectionId);
			layer->autorelease();
			break;
		}

		// 默认情况
		layer = new BaseGuideLayer(seasonId,sectionId);
		layer->autorelease();
	} while (false);


	currentGuideLayer = layer;
	return layer;
}

BaseGuideLayer* GuideLayerFactory::getGuideLayer(int seasonId,int sectionId)
{
	BaseGuideLayer* layer = NULL;
	// lite版本
#if Version_Lite == 1
	layer = getGuideLayerLite(seasonId,sectionId);
#else
	layer = getGuideLayerNormal(seasonId,sectionId);
#endif

	float x_offset = (CCDirector::sharedDirector()->getWinSize().width - 1024)/2.0f;
	layer->setPositionX(x_offset);

	return layer;
}