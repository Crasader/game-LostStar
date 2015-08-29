#pragma once

#include <stdlib.h>

class CurtainLayerCallback 
{
public:
	virtual void curtainActionEnd() = 0;
};


class ICurtain
{
public:
	virtual void setAutoRemve(bool remove) = 0;
	virtual void action(CurtainLayerCallback* callback = NULL) = 0;
};

