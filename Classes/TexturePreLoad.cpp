#include "TexturePreLoad.h"
#include "GameInforEditer.h"
#include "SpriteHelper.h"
#include "GameLayerFactory.h"
#include "Debug.h"
#include "VersionControl.h"

#define Default_TimeOut  10.0f	//10秒钟之后还没有找到图片就回调超时接口
#define IgnoreImageNum  5		//5张图片之后就可以忽略了，但是会延迟一段时间再忽略
#define IgnorePerImageDelay 2.0f	//每张延迟2秒钟

//#define T_LOG(format, ...)     cocos2d:://CCLog(format, ##__VA_ARGS__)
#define T_LOG(format, ...)     

TexturePreLoad* TexturePreLoad::instance;

void TextAssert(bool condition,const char* msg)
{
	//CCLog("TextAssert(%s,%s)",condition?"TRUE":"FALSE",msg);
#ifdef DEBUG_ENABLE_TEXTURE_ASSERT
	CCAssert(condition,msg);
#endif 
}

TexturePreLoad::TexturePreLoad()
{
	cache = CCTextureCache::sharedTextureCache();
	resMgr = ResManager::getManager();
	mNeedLoadTaskNum = 0;
	mLoadTextureCalledNum = 0;
	callback = NULL;
	tasks.clear();
	loadedText.clear();
	mBusy = false;
	mAutoClear = false;
	mTimeout = Default_TimeOut;
}

TexturePreLoad* TexturePreLoad::getInstance()
{
	if (!instance)
	{
		instance = new TexturePreLoad();
	}
	return instance;
}
bool TexturePreLoad::isBusy()
{
	return mBusy;
}
void TexturePreLoad::startSync()
{
	if (mBusy)
	{
		TextAssert(false,"任务还没有完成，不能再次开始");
		return;
	}
	mBusy = true;
	for (size_t i=0;i<tasks.size();i++)
	{
		//add by jj
//		loadTexture(tasks[i],false);
	}
	tasks.clear();
	mNeedLoadTaskNum = 0;
	mBusy = false;
}
void TexturePreLoad::startAsync(TexturePreLoadCallBack* callback)
{
	if (mBusy)
	{
		TextAssert(false,"任务还没有完成，不能再次开始");
		return;
	}
	mBusy = true;
	// 不在一起载入了，而是一张一张的载入
	this->callback = callback;
	//add by jj
	// 从第一张开始载入
// 	if ( tasks.size() > 0)
// 	{
// 		// 一次全部载入
// 		for (int i=0;i < tasks.size();i++)
// 		{
// 			loadTexture(tasks[i],true);
// 		}
// 	}else
	{
		loadOver();
	}
}
void TexturePreLoad::cancelCallback()
{
	this->callback = NULL;
}
void TexturePreLoad::setAutoClearMemery()
{
	mAutoClear = true;
}

void TexturePreLoad::clearCurrentLoadedTexture()
{
	for (size_t i=0;i<loadedText.size();i++)
	{
		cache->removeTextureForKey(loadedText[i].c_str());
	}
	loadedText.clear();
	// 正是IOS内存吃紧的解救办法
	CCDirector::sharedDirector()->purgeCachedData();
}	

//
int TexturePreLoad::preLoadOuterTexture()
{
	//add by jj
	long numStart = mLoadTextureCalledNum;
// 	preLoadMainPageTexture();
// 	if (GameInforEditer::getGameInfor()->isFirstRunMe())
// 	{
// 		preLoadInitGuideTexture();
// 	}
// 	preLoadSctionSelectorTexture();
// 	preLoadSeasonSelectorTexture();
	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadInnerTexture(int seasonId)
{
	long numStart = mLoadTextureCalledNum;
	preInGameTexture(seasonId);
	return mLoadTextureCalledNum - numStart;
}

int TexturePreLoad::preLoadSeasonSelectorTexture()
{
	long numStart = mLoadTextureCalledNum;
	// SeasonSelectorLayerBackgroud
	{
		addTask(resMgr->getSharedFilePath("main_background.png"));
		addTask(resMgr->getSharedFilePath("season_item_selected.png"));
		addTask(resMgr->getSharedFilePath("season_item_disselected.png"));
	}
	

	//SeasonSelectorLayerMenu
	{
		addTask(resMgr->getSharedFilePath("seasonItem_buttom_back.png"));
		addTask(resMgr->getSharedFilePath("seasonItem_buttom_right.png"));
 		addTask(resMgr->getSharedFilePath("seasonItem_buttom_right_select.png"));
 		addTask(resMgr->getSharedFilePath("seasonItem_buttom_left.png"));
 		addTask(resMgr->getSharedFilePath("seasonItem_buttom_left_select.png"));
	}

	//SeasonSelectorLayerTitle
	{
		int seasonNum = GameLayerFactory::getInstance()->getConfigSeasonNum();
		for (int i=0;i<seasonNum;i++)
		{
			addTask(resMgr->getFilePathByLanguageType(SpriteHelper::splicingNum_String("season_title_%d.png",i+1)).c_str());
		}
	}

	//SeasonSelectorLayer
	{
	}
	//SeasonItem
	{
		int seasonNum = GameLayerFactory::getInstance()->getConfigSeasonNum(type_Normal);
		for (int i=0;i<seasonNum;i++)
		{
//			addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("season_item_curtain_shadow_s%d.png",i+1)).c_str());
//			addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("season_item_curtain_girl_s%d.png",i+1)).c_str());
		}
	}
	
	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadMainPageTexture()
{

	long numStart = mLoadTextureCalledNum;
	//// PhysicsSimulationLayer
	//{
	//	addTask(resMgr->getSharedFilePath("main_background.png"));
	//	addTask(resMgr->getSharedFilePath("board_typeface_star.png"));
	//	addTask(resMgr->getSharedFilePath("board_typeface_fall.png"));
	//}

	// MainFrameLayer
	{
		addTask(resMgr->getSharedFilePath("main_background.png"));
		addTask(resMgr->getSharedFilePath("main_title.png"));
		addTask(resMgr->getSharedFilePath("Play_buttom.png"));
		addTask(resMgr->getSharedFilePath("Play_buttom_select.png"));
	}

	//// girl
	//{
	//	addTask(resMgr->getSharedFilePath("girl.png"));	// 纹理图册
	//}

	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadInitGuideTexture()
{
	long numStart = mLoadTextureCalledNum;

	//addTask(resMgr->getSharedFilePath("guide_bg_1.png"));
	//addTask(resMgr->getSharedFilePath("guide_bg_2.png"));
	//addTask(resMgr->getSharedFilePath("guide_curtain.png"));
	//addTask(resMgr->getSharedFilePath("guide_bg_cover.png"));
	//addTask(resMgr->getSharedFilePath("guide_girl.png"));


// 	addTask(resMgr->getSharedFilePath("main_background.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_1.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_2.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_2_cover.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_3.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_4.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_5.png"));
// 	addTask(resMgr->getSharedFilePath("guide_pic_2_cover.png"));

// 	for (int i=0;i<5;i++)
// 	{
// 		addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("guide_pic_5_%d.png",i+1)));
// 	}

	addTask(resMgr->getSharedFilePath("main_background.png"));
	addTask(resMgr->getSharedFilePath("kaichang_juqing_1.png"));
	addTask(resMgr->getSharedFilePath("kaichang_juqing_2.png"));
	addTask(resMgr->getSharedFilePath("kaichang_juqing_2_cover.png"));
	addTask(resMgr->getSharedFilePath("kaichang_juqing_3.png"));

	return numStart - mLoadTextureCalledNum;
}
int TexturePreLoad::preInGameTexture(int seasonId)
{
	long numStart = mLoadTextureCalledNum;
	// 窗帘
	{
		preLoadCurtainTexture(1,true);	// 只用s1的
		preLoadHDCurtainFrameLayerTexture(1);
	}
// 	// 碎片
// 	{
// 		addTask(resMgr->getSharedFilePath("fragements.png").c_str());
// 	}

	// 桌面
	{
//		addTask(resMgr->getSharedFilePath("table_shadow.png").c_str());
		addTask(resMgr->getSharedFilePath("table.png").c_str());
	}
	// 几何体
	{
		addTask(resMgr->getSharedFilePath("physicalItem.png").c_str());
//		addTask(resMgr->getSharedFilePath("physicalItem_shadow.png").c_str());
	}
	
	addTask("starfall0.png");
	addTask("boom0.png");

	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadGirlPickStarTexture()
{
	long numStart = mLoadTextureCalledNum;

	addTask(resMgr->getSharedFilePath("con_girl_part_scale_04.png").c_str());

	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadHDCurtainFrameLayerTexture(int seasonId)
{
	long numStart = mLoadTextureCalledNum;

#if USE_DOOR_REPLACE_CURTAIN != 1
	if(CC_CONTENT_SCALE_FACTOR() == 2)
	{
		// 只给高清的提供这2张就够了
		addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtainHD_s%d_left-ipadhd.png",seasonId)));
		addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtainHD_s%d_right-ipadhd.png",seasonId)));
	}else
	{
		// 这些图，不提供高清的
		addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",seasonId)));
		addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",seasonId)));
	}
#endif

	
	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadCurtainTexture(int seasonId,bool needLoadAnimation/* = false*/)
{
	long numStart = mLoadTextureCalledNum;

#if USE_DOOR_REPLACE_CURTAIN != 1
	addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",seasonId)));
	addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",seasonId)));


	if (!needLoadAnimation)
		return mLoadTextureCalledNum - numStart;

	// 关动画
	{
		string fileNames[] = {
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain5_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain6_s%d_left.png",seasonId)),

			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain5_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain6_s%d_right.png",seasonId))
		};
		for (int i=0;i<sizeof(fileNames)/sizeof(string);i++)
		{
			addTask(fileNames[i]);
		}
	}

	// 开动画
	{
		string fileNames[] = {
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_open_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_open_s%d_left.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_open_s%d_left.png",seasonId)),

			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_open_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_open_s%d_right.png",seasonId)),
			resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_open_s%d_right.png",seasonId))
		};
		for (int i=0;i<sizeof(fileNames)/sizeof(string);i++)
		{
			addTask(fileNames[i]);
		}
	}
#endif
	return mLoadTextureCalledNum - numStart;
}
int TexturePreLoad::preLoadSctionSelectorTexture()
{
	long numStart = mLoadTextureCalledNum;
	// CurtainLayer
	{
		preLoadCurtainTexture(1,false);		// 只用s1的
		preLoadHDCurtainFrameLayerTexture(1);
	}

	// SectionItem
	{
		addTask(resMgr->getSharedFilePath("sectionItem_lock_bg.png"));
		// 总共是3分，0,1,2,3
		for (int i=0;i<4;i++)
		{
			addTask(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("sectionItem_%d_bg.png",i)));
		}
		
	}

	// bug 修复 2013-4-10
	return mLoadTextureCalledNum - numStart;
	//return mLoadTextureCalledNum + numStart;
}

void TexturePreLoad::addTask(string imageFile)
{
	if (mBusy)
	{
		TextAssert(false,"任务还没有完成，不能再次加入");
		return;
	}
	mLoadTextureCalledNum ++ ;
	mNeedLoadTaskNum ++;

	loadedText.insert(loadedText.end(),imageFile);
	tasks.insert(tasks.end(),imageFile);
}

void TexturePreLoad::loadTexture(string file,bool async)
{
#ifdef DEBUG_ENABLE_TEXTURE_LOAD_LOG
	T_LOG("TexturePreLoad::loadTexture ----> to load texture : [%s]",file.c_str());
#endif

	if (async)
	{
		cache->addImageAsync(file.c_str(), this, callfuncO_selector(TexturePreLoad::textureLoadedCallback));
	}else
	{
		cache->addImage(file.c_str());
	}
}
void TexturePreLoad::triggerFirstTextureLoad(CCObject* tex)
{
	T_LOG("TexturePreLoad -----> triggerFirstTextureLoad load Sucess!");
}
void TexturePreLoad::textureLoadedCallback(CCObject* tex)
{
	mNeedLoadTaskNum --;
	if (mNeedLoadTaskNum < 0)
	{
		mNeedLoadTaskNum = 0;
		// bug 修复 2013-4-17
		// BUG : 修复了，纹理在home之后，又回来时，再次回调的bug。
		// 应该是当时失败的纹理，在下次
		// 这个完全是可以忽略的
		TextAssert(false,"运行时出现问题了，多半是超时引起的");
	}
	int index = tasks.size() - mNeedLoadTaskNum -1;

	T_LOG("texture loaded success --> [total = %d,left = %d]",tasks.size(),mNeedLoadTaskNum);
	
	if (callback)
	{
		callback->textureLoaded(mNeedLoadTaskNum,"NULL-Str");
	}

	if (mNeedLoadTaskNum <= 0)
	{
		loadOver();
	}
	// 看是否可以忽略剩下的图片
	else if (mNeedLoadTaskNum <= IgnoreImageNum)
	{
		float delay = IgnorePerImageDelay*mNeedLoadTaskNum;

		T_LOG("TexturePreLoad will ignore the rest %d image(s) after %f seconds",mNeedLoadTaskNum,delay);
		// 取消回调
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
			schedule_selector(TexturePreLoad::timeToIgnoreTheRestImage),this);
		// 回调
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
			schedule_selector(TexturePreLoad::timeToIgnoreTheRestImage),this,delay,false);
	}
}
void TexturePreLoad::timeToIgnoreTheRestImage(float t)
{
	// 取消回调
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(TexturePreLoad::timeToIgnoreTheRestImage),this);

	T_LOG("TexturePreLoad timeToIgnoreTheRestImage called ,the rest need load image num is %d",mNeedLoadTaskNum);

	loadOver();	
}
void TexturePreLoad::loadOver()
{
	// 如果不是忙碌过程，那么代表之前已经loadOver 过了，不再重复loadOver
	// 主要是预防：timeToIgnoreTheRestImage回调之后，真正的LoadImage之后又完成了的bug
	if (!mBusy)
		return;

	// 取消回调
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(TexturePreLoad::timeToIgnoreTheRestImage),this);

	if (callback)
		callback->loadOver();
	tasks.clear();
	mNeedLoadTaskNum = 0;
	mLoadTextureCalledNum = 0;

	T_LOG("TexturePreLoad loadOver !");

#ifdef DEBUG_ENABLE_TEXTURE_LOAD_LOG
	T_LOG("TexturePreLoad texture load over !");
#endif
	
	if (mAutoClear)
	{
		// 取消回调
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
			schedule_selector(TexturePreLoad::toClearSomeBuffer),this);
		// 回调
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
			schedule_selector(TexturePreLoad::toClearSomeBuffer),this,0.5f,false);

		mAutoClear = false;
	}
	mBusy = false;
}
void TexturePreLoad::toClearSomeBuffer(float t)
{
	// 取消回调
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(TexturePreLoad::toClearSomeBuffer),this);
	
	T_LOG("TexturePreLoad release some not use data ......");
	// 正是IOS内存吃紧的解救办法
	CCDirector::sharedDirector()->purgeCachedData();
}
void TexturePreLoad::timeoutCallback(float t)
{
	// 不处理，不检测超时
	return;

	if (mNeedLoadTaskNum <= 0)
	{
		TextAssert(false,"运行错误");
		return;
	}
	int index = tasks.size() - mNeedLoadTaskNum;
	T_LOG("TexturePreLoad time out when load texture '%s'",tasks[index].c_str());

	if (callback)
	{
		callback->textureTimeout(index,tasks[index]);
	}

	// 直接放弃，进入下一个纹理
	mNeedLoadTaskNum -- ;
	if (mNeedLoadTaskNum > 0)
	{
		loadTexture(tasks[index+1].c_str(),true);
	}
	else
	{
		loadOver();
	}
}