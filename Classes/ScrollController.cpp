#include "ScrollController.h"
#include <cmath>

#define	MAX_TOUCH_MOVE_RECORD	5
#define SCROLLING_SENSITIVITY	3.0f	// 滑动的速度超过了这个伐值，才移动
#define SCROLLING_MIN_V			0.5f	// 滑动的最小速度，速度降低到这个以下就会停止


// 添加一个滑动时的一个瞬时点
void VelocityCalculate::addTouchMoveRecord(CCPoint glPoint)
{
	movePointRecord.push_back(glPoint);

	if (movePointRecord.size() > MAX_TOUCH_MOVE_RECORD)
	{
		movePointRecord.erase(movePointRecord.begin());
	}
}
// 计算当前的瞬时速度
CCPoint VelocityCalculate::getCurrentInstantaneousSpeed()
{
	//// 数据不够2个就返回
	//if (movePointRecord.size() < 2)
	//	return CCPointZero;
	// 取后面的2个对于手指触摸，误差较大
	//return ccpSub(movePointRecord[movePointRecord.size()-1],movePointRecord[movePointRecord.size()-2]);


	CCAssert(MAX_TOUCH_MOVE_RECORD >= 4,"");

	// 我们取倒数第3第4两个做计算
	if (movePointRecord.size() < MAX_TOUCH_MOVE_RECORD)
		return CCPointZero;

	// 我们去倒数1-2，3-4中最小的一个值
	CCPoint move4_1 = ccpSub(movePointRecord[movePointRecord.size()-1],movePointRecord[movePointRecord.size()-4]);
	move4_1 = ccpMult(move4_1,1.0f/4.0f);
	CCPoint move2_1 = ccpSub(movePointRecord[movePointRecord.size()-1],movePointRecord[movePointRecord.size()-2]);

	return ccpDistanceSQ(CCPointZero,move4_1)>ccpDistanceSQ(CCPointZero,move2_1)?move2_1:move4_1;
}




ScrollController::ScrollController(void)
{
}

// 滑动，不会有回弹的，也就是滑到X地方，那么就不会滑到X+a的地方，然后回滚到X
void ScrollController::scrollWithNoRebound(ScrollConfig config)
{
	CCAssert(config.toScroll != NULL,"");
	mConfig = config;

	float dis_sq = ccpDistanceSQ(CCPointZero,config.scrollSpeed);

	if ( dis_sq < SCROLLING_SENSITIVITY*SCROLLING_SENSITIVITY)
	{
		CCLog("%s: speed[speed^2 = %f] is too small",__FUNCTION__,dis_sq);
		return;
	}

	stopScrolling();
	schedule(schedule_selector(ScrollController::moveing));
	CCLog("%s: scrolling [speed^2 = %f]",__FUNCTION__,dis_sq);
}

void ScrollController::moveing(float t)
{
	if (mConfig.toScroll == NULL)
	{
		return;
	}

	// 移动
	mConfig.toScroll->setPositionX(mConfig.toScroll->getPositionX() + mConfig.scrollSpeed.x);
	mConfig.toScroll->setPositionY(mConfig.toScroll->getPositionY() + mConfig.scrollSpeed.y);

	bool x_move_end = false;
	bool y_move_end = false;

	// 匡正范围
	if (mConfig.toScroll->getPositionX() < mConfig.minPosX)
	{
		mConfig.toScroll->setPositionX(mConfig.minPosX);
		x_move_end = true;
	}
	if (mConfig.toScroll->getPositionX() > mConfig.maxPosX)
	{
		mConfig.toScroll->setPositionX(mConfig.maxPosX);
		x_move_end = true;
	}

	if (mConfig.toScroll->getPositionY() < mConfig.minPosY)
	{
		mConfig.toScroll->setPositionY(mConfig.minPosY);
		y_move_end = true;
	}
	if (mConfig.toScroll->getPositionY() > mConfig.maxPosY)
	{
		mConfig.toScroll->setPositionY(mConfig.maxPosY);
		y_move_end = true;
	}

	if (decelerate())
	{
		CCLog("%s: speed is too small stop",__FUNCTION__);
		stopScrolling();
	}

	if (y_move_end && x_move_end)
	{
		CCLog("%s: y_move_end and x_move_end --> stop",__FUNCTION__);
		stopScrolling();
	}
}

// 减速
bool ScrollController::decelerate()
{
	mConfig.scrollSpeed.x *= abs(mConfig.decelerate);
	mConfig.scrollSpeed.y *= abs(mConfig.decelerate);

	if (ccpDistanceSQ(CCPointZero,mConfig.scrollSpeed) < SCROLLING_MIN_V*SCROLLING_MIN_V)
	{
		mConfig.scrollSpeed = CCPointZero;
		return true;
	}
	return false;

	//// X 方向
	//if (mConfig.scrollSpeed.x > 0)
	//{
	//	mConfig.scrollSpeed.x -= mConfig.decelerate;

	//	if (mConfig.scrollSpeed.x < 0)
	//	{
	//		mConfig.scrollSpeed.x = 0.0f;
	//	}

	//}
	//else if (mConfig.scrollSpeed.x < 0)
	//{
	//	mConfig.scrollSpeed.x += mConfig.decelerate;

	//	if (mConfig.scrollSpeed.x > 0)
	//	{
	//		mConfig.scrollSpeed.x = 0.0f;
	//	}
	//}

	//// Y 方向
	//if (mConfig.scrollSpeed.y > 0)
	//{
	//	mConfig.scrollSpeed.y -= mConfig.decelerate;

	//	if (mConfig.scrollSpeed.y < 0)
	//	{
	//		mConfig.scrollSpeed.y = 0.0f;
	//	}

	//}
	//else if (mConfig.scrollSpeed.y < 0)
	//{
	//	mConfig.scrollSpeed.y += mConfig.decelerate;

	//	if (mConfig.scrollSpeed.y > 0)
	//	{
	//		mConfig.scrollSpeed.y = 0.0f;
	//	}
	//}
	
}

void ScrollController::stopScrolling()
{
	unschedule(schedule_selector(ScrollController::moveing));
	CCLog("%s",__FUNCTION__);
}

ScrollController::~ScrollController(void)
{
}
