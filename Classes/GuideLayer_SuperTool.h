#ifndef __GuideLayer_SuperTool_H__
#define __GuideLayer_SuperTool_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_SuperTool : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);
	void clickEnd();
	void guide_end();

	void usedEnd();
public:
	GuideLayer_SuperTool(int seasonId,int sectionId);
private:
	CCSprite*		m_sGuide_arrowsSpr;
	CCLabelTTF		*m_tLabel;
	int				m_nClickCount;
	bool			m_bIsUseTeaching;
};

#endif