#include "MyBlink.h"
MyBlink* MyBlink::create(float duration, size_t uBlinks)
{
	MyBlink *pBlink = new MyBlink();
	pBlink->initWithDuration(duration, uBlinks);
	pBlink->autorelease();

	return pBlink;
}

bool MyBlink::initWithDuration(float duration, size_t uBlinks)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_nTimes = uBlinks;
		return true;
	}

	return false;
}

void MyBlink::stop()
{
	m_pTarget->setVisible(m_bOriginalState);
	CCActionInterval::stop();
}

void MyBlink::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_bOriginalState = pTarget->isVisible();
}

CCObject* MyBlink::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	MyBlink* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (MyBlink*)(pZone->m_pCopyObject);

	}
	else
	{
		pCopy = new MyBlink();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, (size_t)m_nTimes);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void MyBlink::update(float time)
{
	if (m_pTarget && ! isDone())
	{
		float slice = 1.0f / m_nTimes;
		float m = fmodf(time, slice);
		m_pTarget->setVisible(m > slice / 2 ? true : false);
	}
}

CCActionInterval* MyBlink::reverse(void)
{
	// return 'self'
	return MyBlink::create(m_fDuration, m_nTimes);
}