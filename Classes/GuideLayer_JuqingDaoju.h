#ifndef __GuideLayer_JuqingDaoju_H__
#define __GuideLayer_JuqingDaoju_H__
#include "BaseGuideLayer.h"
#include <cmath>

/**
 *	这用于
 */
class GuideLayer_JuqingDaoju : public BaseGuideLayer
{
private:
	bool doubleClickGuided;
	CCSprite* m_pTipSpr;

	void fingerPosReady(CCNode* finger);
	void baseContentLayerEffctiveClicked(CCNode* node);
	void lfClick(CCPoint glPoint);
	void doubleClickDown(CCPoint glPoint);
	void scaleBigRunning();
	void showCanDrag(float time);

	virtual void onEnter();
	virtual void onExit();

	void pause();
	void resume();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
public:
	GuideLayer_JuqingDaoju(int seasonId,int sectionId);

	CCSprite* getTipSpr(){return m_pTipSpr;};
};

#endif  //_GuideLayer_JuqingDaoju_H_