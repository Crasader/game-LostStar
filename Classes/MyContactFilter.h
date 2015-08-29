#ifndef __MyContactFilter_H__
#define __MyContactFilter_H__

#include "Box2d/Box2D.h"

class MyContactFilter : public b2ContactFilter
{ 
public: 
	MyContactFilter(void);
	~MyContactFilter(void);
protected:
	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);

	// 返回是否是跟边框有关，有关的话collide才有效
	// 只检测了fixtureB是红星的情况
	// 并没有反过来检测fixtureA是红星的情况
	bool check_safeboarder(b2Fixture* fixtureA, b2Fixture* fixtureB,bool& collide);
}; 

#endif
