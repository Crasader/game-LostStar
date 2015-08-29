#ifndef __MainThingEffect_n_H__
#define __MainThingEffect_n_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseMoveUI.h"
using namespace cocos2d;
using namespace cocos2d::extension;
USING_NS_CC;
class MainThingEffect : public CCSprite
{
public:
	MainThingEffect();
	virtual ~MainThingEffect();
	CREATE_FUNC(MainThingEffect);
	void moveEffect();
	inline void setParent(BaseMoveUI *base)
	{
		m_uBaseUI = base;
	}
private:
	void moveMyself(float dt);
	void moveBack(float dt);
private:
	int					m_nRandomTime;//要移动多长时间
	int					m_nSecCount;
	BaseMoveUI			*m_uBaseUI;
};

#endif // __MainThingEffect_n_H__