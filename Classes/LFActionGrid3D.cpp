#include "LFActionGrid3D.h"

LFTwirl::LFTwirl()
{
	gridDistanceArray = NULL;
}
LFTwirl::~LFTwirl()
{
	if (gridDistanceArray)
	{
		delete [] gridDistanceArray;
	}
	
}
LFTwirl* LFTwirl::create(float radius,float mAngleFactor,float mScaleFactor,const CCSize& gridSize, float duration)
{
	LFTwirl *pAction = new LFTwirl();

	if (pAction)
	{
		if (pAction->initWithPosition(radius,mAngleFactor,mScaleFactor,gridSize, duration))
		{
			pAction->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(pAction);
		}
	}

	return pAction;
}
//CCGridBase* LFTwirl::getGrid(void)
//{
//	////原来是这样的，要改成我们自己的
//	//return CCGrid3D::gridWithSize(m_sGridSize);
//
//	//return LFGrid3D::gridWithSize(m_sGridSize);
//}

bool LFTwirl::initWithPosition(float radius,float angleFactor,float scaleFactor,const CCSize& gridSize, float duration)
{
//	if (CCGrid3DAction::initWithSize(gridSize, duration))
//Richard
	if(CCGrid3DAction::initWithDuration(duration, gridSize))
	{
		//setPosition(worldCenterPos);
		mAngle = 0.0f;
		mScale = 1.0f;
		mRadius = radius;

        flag = 0;
		mScaleFactor = 1.0f/scaleFactor;
		mAngleFactor = angleFactor;

		//CCDirector::sharedDirector()->setDe
		return true;
	}

	return false;
}

void LFTwirl::startWithTarget(cocos2d::CCNode *pTarget)
{
    if (m_pTarget) {
        return;
    }else{
        CCGrid3DAction::startWithTarget(pTarget);
    }
}

void LFTwirl::preCreatGrid(CCNode* actionTarget)
{
	CCAssert(actionTarget!=NULL,"");
	startWithTarget(actionTarget);
}
void LFTwirl::preCalculateDistance()
{
	setPosition(m_pTarget->getPosition());

//	gridDistanceArray = new float[ (m_sGridSize.x + 1) * (m_sGridSize.y + 1) ];
//Richard
// 原先类型为int   现在CCSize为float 墙砖为int
	gridDistanceArray = new float[ ((int)m_sGridSize.width + 1) * ((int)m_sGridSize.height + 1) ];
	int i, j;
	int index = 0;
	for (i = 0; i < m_sGridSize.width + 1; ++i)
	{
		for (j = 0; j < m_sGridSize.height + 1; ++j)
		{
			ccVertex3F v = originalVertex(ccp(i, j));
			CCPoint relativedPos = ccp(v.x - m_position.x,v.y - m_position.y);
			float dis = ccpLength(relativedPos) + 1.0f;
			gridDistanceArray[index] = dis;
			index ++;
		}
	}

//	setPosition(m_pTarget->getPosition());

}
CCObject* LFTwirl::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	LFTwirl* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (LFTwirl*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new LFTwirl();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCGrid3DAction::copyWithZone(pZone);

	//worldCenterPos, radius,endAngle,endScale,gridSize, duration
	pCopy->initWithPosition(mRadius,mAngleFactor,mScaleFactor,m_sGridSize, m_fDuration);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

float LFTwirl::similarDistance(cocos2d::CCPoint p1, cocos2d::CCPoint p2)
{
    float dis_x = abs(p1.x - p2.x);
    float dis_y = abs(p1.y - p2.y);
    
    return dis_x + dis_y;
}

void LFTwirl::update(float time)
{
	//// 优化部分
	//{
	//	flag ++;
	//	if (flag % 2 != 0)
	//		return;
	//	flag = 0;
	//}

	int i, j;
	int index = 0;
	for (i = 0; i < m_sGridSize.width + 1; ++i)
	{
		for (j = 0; j < m_sGridSize.height + 1; ++j)
		{
			ccVertex3F v = originalVertex(ccp(i, j));
			CCPoint relativedPos = ccp(v.x - m_position.x,v.y - m_position.y);
			CCPoint newPos;
            float dis = 0.0f;
            if (gridDistanceArray)
            {
                dis = gridDistanceArray[index];index++;
            }else
            {
                dis = ccpLength(relativedPos) + 1.0f;
            }

			// 基于相对中心坐标计算
			{
				float tempAngle = mAngle;
				tempAngle = tempAngle / dis * (mRadius*10);// 跟半径成反比，半径越大，转动角度越小
                newPos = ccpRotateByAngle(relativedPos,ccp(1,0),CC_DEGREES_TO_RADIANS(tempAngle));
				newPos.x *= mScale;
				newPos.y *= mScale;
			}
			
			v.x = newPos.x + m_position.x ;
			v.y = newPos.y + m_position.y;

			setVertex(ccp(i, j), v);
		}
	}

	mScale *= mScaleFactor;
	mAngle += mAngleFactor;
}