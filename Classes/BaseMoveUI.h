#ifndef __BaseMoveUI_n_H__
#define __BaseMoveUI_n_H__
#include "iostream"
class MainThingEffect;
class BaseMoveUI
{
public:
	BaseMoveUI()
	{

	}
	virtual ~BaseMoveUI()
	{

	}
	virtual void moveChangeEnd(MainThingEffect *node) = 0;
};
#endif