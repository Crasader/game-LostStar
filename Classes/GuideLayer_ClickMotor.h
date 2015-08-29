#ifndef __GuideLayer_ClickMotor_H__
#define __GuideLayer_ClickMotor_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_ClickMotor : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);
	CCSprite* m_pGuide_arrows;
	CCLabelTTF* m_pGuide_txt;
public:
	GuideLayer_ClickMotor(int seasonId,int sectionId);

	void startGuide();
};

#endif