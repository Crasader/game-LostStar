#ifndef __GuideLayer_CanotUseMotor_H__
#define __GuideLayer_CanotUseMotor_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_CanotUseMotor : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);
	CCSprite* m_pGuide_arrows;
	CCLabelTTF* m_pGuide_txt;
public:
	GuideLayer_CanotUseMotor(int seasonId,int sectionId);
	void startGuide();
};

#endif