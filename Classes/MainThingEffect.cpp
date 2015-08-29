#include "MainThingEffect.h"
MainThingEffect::MainThingEffect()
	:CCSprite(),
	m_nRandomTime(0),
	m_nSecCount(0)
{

}
MainThingEffect::~MainThingEffect()
{

}
void MainThingEffect::moveEffect()
{
	m_nRandomTime = ((int)(CCRANDOM_0_1() * 10000) % 60) + 120;

	m_nSecCount = 0;

	this->schedule(schedule_selector(MainThingEffect::moveMyself),0);

	float scaleTo = 1.05f;
	if (this->getTag() == 5)
	{
		scaleTo = 1.0f;
	}
	else if (this->getTag() == 6)
	{
		scaleTo = 1.0f;
	}
	CCScaleTo* scale = CCScaleTo::create(m_nRandomTime * 1.0f / 60.0f,scaleTo);
	//this->stopAllActions();
	this->runAction(scale);
}
void MainThingEffect::moveMyself(float dt)
{
	const int sec = m_nRandomTime ;
	float x = 40.0f /(float)sec;
	float y = 40.0f /(float)sec;

	if (this->getTag() == 6)
	{
		x = 20.0f /(float)sec;
		y = 20.0f /(float)sec;
	}
	
	m_nSecCount++;
	if(m_nSecCount >= sec)
	{
		unschedule(schedule_selector(MainThingEffect::moveMyself));

		m_nSecCount = 0;

		this->schedule(schedule_selector(MainThingEffect::moveBack),0);
		CCScaleTo* scale = CCScaleTo::create(m_nRandomTime * 1.0f / 60.0f,1.0f);
		//this->stopAllActions();
		this->runAction(scale);

	}
	else
	{
		CCPoint point = ccpMult(ccp(x,y), 1 - powf(1.0f / sec * m_nSecCount, 1.0f / 2));
		CCPoint pos = this->getPosition();
		//pos.x += point.x;
		pos.y += point.y;
		this->setPosition(pos);
	}
}
void MainThingEffect::moveBack(float dt)
{
	const int sec = m_nRandomTime ;
	float x = 40.0f /(float)sec;
	float y = 40.0f /(float)sec;
	if (this->getTag() == 6)
	{
		x = 20.0f /(float)sec;
		y = 20.0f /(float)sec;
	}
	m_nSecCount++;
	if(m_nSecCount >= sec)
	{
		unschedule(schedule_selector(MainThingEffect::moveBack));
		m_uBaseUI->moveChangeEnd(this);
	}
	else
	{
		CCPoint point = ccpMult(ccp(x,y), 1 - powf(1.0f / sec * m_nSecCount, 1.0f / 2));
		CCPoint pos = this->getPosition();
		//pos.x -= point.x;
		pos.y -= point.y;
		this->setPosition(pos);
	}
}