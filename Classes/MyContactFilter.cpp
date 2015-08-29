
#include "MyContactFilter.h"
#include "cocos2d.h"
#include "CommonDefine.h"
#include "CustomeSprite.h"

using namespace cocos2d;

MyContactFilter::MyContactFilter(void)
{
}

MyContactFilter::~MyContactFilter(void)
{
}

bool MyContactFilter::check_safeboarder(b2Fixture* fixtureA, b2Fixture* fixtureB,bool& collide)
{
	CCNode* nodeA = (CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* nodeB = (CCNode*)fixtureB->GetBody()->GetUserData();

	bool isAboutSafeBoard = false;
	collide = false;

	if (nodeA->getTag() == Tag_SafeBoard_D ||
		nodeA->getTag() == Tag_SafeBoard_T ||
		nodeA->getTag() == Tag_SafeBoard_L ||
		nodeA->getTag() == Tag_SafeBoard_R)
	{
		isAboutSafeBoard = true;
	}else
	{
		return isAboutSafeBoard; // 跟边框没关，直接返回
	}



	if (nodeA->getTag() == Tag_SafeBoard_D && 
		nodeB->getTag() == Tag_Box_RedStar && 
		fixtureB->GetBody()->GetLinearVelocity().y <= 0)
	{
		collide = true;
	}
	else if (
		nodeA->getTag() == Tag_SafeBoard_T && 
		nodeB->getTag() == Tag_Box_RedStar && 
		fixtureB->GetBody()->GetLinearVelocity().y >= 0)
	{
		collide = true;
	}
	else if (
		nodeA->getTag() == Tag_SafeBoard_L && 
		nodeB->getTag() == Tag_Box_RedStar && 
		fixtureB->GetBody()->GetLinearVelocity().x <= 0)
	{
		collide = true;
	}
	else if (
		nodeA->getTag() == Tag_SafeBoard_R && 
		nodeB->getTag() == Tag_Box_RedStar && 
		fixtureB->GetBody()->GetLinearVelocity().x >= 0)
	{
		collide = true;
	}


	return isAboutSafeBoard;
}
bool MyContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{ 
	CCNode* nodeA = (CCNode*)fixtureA->GetBody()->GetUserData();
	CCNode* nodeB = (CCNode*)fixtureB->GetBody()->GetUserData();
	if (nodeA == NULL || nodeB == NULL)
		return true;

	// 安全边框的限制
	{
		bool colli;
		if (check_safeboarder(fixtureA,fixtureB,colli))
		{
			return colli;
		}
		if (check_safeboarder(fixtureB,fixtureA,colli))
		{
			return colli;
		}
	}


	if (nodeA->getTag() == Tag_Box_Rope_Anchor || nodeB->getTag() == Tag_Box_Rope_Anchor)
	{
		return false;
	}

	if (nodeA->getTag() == Tag_Box_Strobe && ((Strobe*)nodeA)->isOpened())
	{
		return false;
	}
	if (nodeB->getTag() == Tag_Box_Strobe && ((Strobe*)nodeB)->isOpened())
	{
		return false;
	}

	return true;
} 
