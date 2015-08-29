#ifndef __GuideLayer_ClickFinger_H__
#define __GuideLayer_ClickFinger_H__
#include "BaseGuideLayer.h"

#include "cocos-ext.h"

/**
 *	这用于
 */
class GuideLayer_ClickFinger : public BaseGuideLayer
{
public:
	GuideLayer_ClickFinger(int seasonId,int sectionId);
	void startGuide();

	void techUpdata(float dt);
	void openTouched(CCNode* pNode);
	void lfClick(CCPoint glPoint);
private:
	CCSprite*		m_sGuide_arrowsSpr;
	CCLabelTTF		*m_tLabel;

	bool            m_bIsTechEnd;

	cocos2d::extension::CCScale9Sprite*       m_pBg;
};

#endif