#include "Myobject.h"


Myobject::Myobject(void)
{
	mark = false;
	parent_id = 0;
	current_id = 0;

	season = 0;
	gk_id = 0;

	mark_oprate = 0;
	friend_id = 0;

	time = 0.5f;
	pCurNode = NULL;
}


Myobject::~Myobject(void)
{


}
