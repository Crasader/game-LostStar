#ifndef __GuideLayer_H__
#define __GuideLayer_H__
#include "BaseGuideLayer.h"

/**
 *	可用于只需要引导一个点击的，教学地方。
 */
class GuideLayer : public BaseGuideLayer
{
private:
	int needRepeatNum;
	CCPoint pos;

	CCSprite* guide_arrows_0;
	CCLabelTTF* guide_txt_0;

	CCSprite* guide_arrows_1;
	CCLabelTTF* guide_txt_1;

	CCSprite* spriteMask;

	int state;

	// -1 则没有
	int getGuideIndex();

	// 手指到指定地点了
	void fingerPosReady(CCNode* finger);

	void guideTimmer();

	void guideLastOne();

	void oneGuide(float delay,CCPoint endPos);

	void oneGuideEnd(CCNode* fingerLight);

	virtual void lfClick(CCPoint glPoint);
	virtual void lfTouchesBegan(CCPoint glPoint);
	virtual void lfTouchesEnded(CCPoint glPoint);

	void startGuide();

	void pauseGame(float t);

public:
	GuideLayer(int seasonId,int sectionId);

	~GuideLayer();

};

#endif