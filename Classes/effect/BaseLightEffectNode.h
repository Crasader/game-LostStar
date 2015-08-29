#ifndef __BaseLightEffectNode_n_H__
#define __BaseLightEffectNode_n_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

const float KLightTime = 0.01f;//闪光所需要的时间
const float KLightSideTime = 1.0f;//两次闪光所间隔的时间
class BaseLightEffectNode : public CCSprite
{
public:
	BaseLightEffectNode();
	virtual ~BaseLightEffectNode();

	virtual bool init();
	CREATE_FUNC(BaseLightEffectNode);
	virtual void onEnter();

	inline void setLightScale(float scale)
	{
		m_fLightScale = scale;
	}
protected:

private:
	void showLightEffect(float dt);
	void showLightEnd(CCNode *node);
private:
	int								m_nLightCount;
	map<int,CCSprite *>		m_sLightSprList;
	float							m_fLightScale;
};

#endif // __BaseLightEffectNode_n_H__