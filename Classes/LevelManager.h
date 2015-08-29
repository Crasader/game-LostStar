#ifndef _LevelManager_H_
#define _LevelManager_H_

#include "LevelSelectListener.h"
#include "cocos2d.h"
#include "LoadingLayer.h"
using namespace cocos2d;

class LevelManager : public LevelSelectListener
{
private:
	static LevelManager* manager;
	int m_iLastSeasonId;
	int m_iLastSectionId;

	int needUnlockSeasonId;
	int needScrollToSeasonId;

	LevelManager();
	//season被选中之后调用
	void seasonSelected(int seasonId,int lastSeasonId,int lastSectionId,CCPoint seasonPos);
	//section被选中之后
	void sectionSelected(int sectionId,CCPoint sectionPos);
	//section页面放弃选择，也就是返回了
	void sectionBackToSeason(int fromSeasonId);

	bool m_bStartApp;		//是否是程序启动

	bool m_bShowLoadingAnimation;		//是否显示小人动画
	bool m_bNext;			//是否是点击了下一关
	bool m_bSlideToPage3;	//是否滑动到隐藏关卡页,仅用于16关完成后跳出到小关卡选择界面的判断	
	bool m_bShowUnlockAni;
public:
	static LevelManager* getManager();				// 得到LevelManager
	CCLayer* createSectionLayer(int seasonId);		// 得到第seasonId章的关卡Layer
	void goToSeasonUI(bool fromSection = false);		// 到season选择的界面,
	void goToInitGuideUI();		// 到引导界面,
	void goToSectionUI(int seasonId,bool needTrans = true);				// 到section选择界面
	void goToSectionUI(bool needTrans = true);							// 到上次section选择界面
	void goTo(int seasonId,int sectionId);			// 去某关
	void nextSection();								// 根据LevelManager的记录，进入下一关
	void preSection();								// 根据LevelManager的记录，进入上一关
	void setNeedUnLockSeason(int seasonId);			// 设置要做解锁动画的Season，设置了之后，那么当SeasonUI出现时就会播放动画
	void setNeedScrollToSeason(int seasonId);		// 设置要自动滚动过去的动画。用于一个关卡玩完毕之后自动滚走
	int getLastSeasonId(){return m_iLastSeasonId;};	// 返回最近一次的seasonId
	int getLastSectionId(){return m_iLastSectionId;};	// 返回最近一次的sectionId
	void retry(bool needCurtain);

	//add by jiangjie
	void GameInToMainLayer();
	void SectionToGameIn();
	void GameInToSeasonLayer();

	void setLastSeasonId(int iLastSeasonId, bool bSetClickSeasonId = true);
	void setLastSectionId(int ilastSectionId){m_iLastSectionId = ilastSectionId;};	// 设置最近一次的sectionId
	void setNext(bool e){m_bNext = e;};
	bool getNext(){return m_bNext;};
	void setShowLoadingAnimation(bool e){m_bShowLoadingAnimation = e;};
	bool getShowLoadingAnimation(){return m_bShowLoadingAnimation;};
    void reset();
	void GameInToSectionLayer();

	void setStartApp(bool e){m_bStartApp = e;};
	bool getStartApp(){return m_bStartApp;};

	void setSlideToPage3(bool e){m_bSlideToPage3 = e;};
	bool getSlideToPage3(){return m_bSlideToPage3;};

	void setShowUnlockAni(bool e){m_bShowUnlockAni = e;};
	bool getShowUnlockAni(){return m_bShowUnlockAni;};

	enum _iTag
	{
		gamegoods_tag = 10,
	};
	//end
};

#endif


