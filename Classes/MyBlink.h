#ifndef __MyBlink__
#define __MyBlink__

#include "cocos2d.h"
USING_NS_CC;
class MyBlink : public CCActionInterval
{
public:
	/** initializes the action */
	bool initWithDuration(float duration, size_t uBlinks);

	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void update(float time);
	virtual CCActionInterval* reverse(void);

public:

	/** creates the action */
	static MyBlink* create(float duration, size_t uBlinks);

	virtual void startWithTarget(CCNode *pTarget);
	virtual void stop();

protected:
	size_t m_nTimes;
	bool m_bOriginalState;
};
#endif