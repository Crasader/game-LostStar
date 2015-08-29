#ifndef __GuideLayer_Scale_h__
#define __GuideLayer_Scale_h__

#include "cocos2d.h"

USING_NS_CC;

class GuideLayer_Scale : public cocos2d::CCLayerColor
{
public:
	GuideLayer_Scale(void);
	~GuideLayer_Scale(void);

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

    void scaleEnd(CCObject* pObj);
    
	CREATE_FUNC(GuideLayer_Scale);

protected:
    int                 m_iCallbackTimes;

	CCSprite*			m_pScaleBig;
    CCSprite*			m_pScaleSmall;
	CCLabelTTF			*m_tTipStr;
};

#endif //__scaleTeachLayer_h__