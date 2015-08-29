#pragma once
#include <vector>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

/********************************************************************
 *	一个用于滑动处理的一个CPP集合，包括滑动初始速度的计算器，以及控制滑动的滑动器
 *******************************************************************/


// 计算瞬时速度的类
class VelocityCalculate
{
protected:
	vector<CCPoint> movePointRecord;
public:
	// 添加一个滑动时的一个瞬时点
	void addTouchMoveRecord(CCPoint glPoint);
	// 计算当前的瞬时速度
	CCPoint getCurrentInstantaneousSpeed();

	void clearRecord(){movePointRecord.clear();};
};


struct ScrollConfig
{
	CCNode* toScroll;		// 需要滑动的node
	CCPoint scrollSpeed;	// 初始的滑动速度
	float decelerate;		// 减速因子,必须是非负数， > 1.0f 就是加速了

	// 滑动的边框的设置，不可能使Node的postion滑出这个边框
	float minPosX;
	float minPosY;
	float maxPosX;
	float maxPosY;

	ScrollConfig()
	{
		toScroll = NULL;
	}
};

// 模拟滑动的类,需要添加到一个node上面，才可以执行tick
class ScrollController : public CCNode
{
protected:
	ScrollConfig mConfig;

	void moveing(float t);

	// 减速，返回值代表是否已经没有速度了，反会true就是没有速度了
	bool decelerate();

public:
	ScrollController(void);

	// 滑动，不会有回弹的，也就是滑到X地方，那么就不会滑到X+a的地方，然后回滚到X
	void scrollWithNoRebound(ScrollConfig config);

	void stopScrolling();

	~ScrollController(void);
};

