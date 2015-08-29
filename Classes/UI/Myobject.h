#pragma once
#include "cocos2d.h"
#include "Uimsg.h"
USING_NS_CC;
class Myobject:public CCObject
{
public:
	Myobject(void);
	~Myobject(void);
public:
	bool mark;
	int parent_id;
	int current_id;

	//小光卡id
	int season;
	int gk_id;

	int mark_oprate;
	int friend_id;

	CCNode*	pCurNode;

	float time;

	msgSnsdata data;

	Myobject& operator= (const Myobject& other)
	{
		this->mark = other.mark;
		this->parent_id = other.parent_id;
		this->current_id = other.current_id;
		this->season = other.season;
		this->gk_id = other.gk_id;
		this->mark_oprate = other.mark_oprate;
		this->friend_id = other.friend_id;
		this->pCurNode = other.pCurNode;
		this->time = other.time;
		return *this;
	}
};

