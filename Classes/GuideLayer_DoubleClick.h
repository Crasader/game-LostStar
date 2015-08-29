#ifndef __GuideLayer_DoubleClick_H__
#define __GuideLayer_DoubleClick_H__
#include "BaseGuideLayer.h"
#include <cmath>
#include <sys/timeb.h>
#include <time.h> 

/**
 *	这用于
 */
class GuideLayer_DoubleClick : public BaseGuideLayer
{
private:
	bool doubleClickGuided;
	CCLabelTTF* lable;
	CCSprite* mask;
	CCPoint lastPos;
	timeb lastClickDownTime;

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

	timeb currentTime()
	{
		struct timeb tp;
		ftime(&tp);
		return tp;
	}
public:
	GuideLayer_DoubleClick(int seasonId,int sectionId);
};

#endif