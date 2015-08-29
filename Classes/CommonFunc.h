#pragma once

#include "cocos2d.h"
using namespace cocos2d;


namespace FX {

//该node执行action，所有的child也执行action
static void nodeAndAllChildRunAction(CCNode* node, CCAction* action)
{
	if(node == NULL || action == NULL)
		return;

	//自己
	node->runAction(action);
	//childe
	CCArray* childs = node->getChildren();
	if (childs != NULL)
	{
		for (int i=0; i!=childs->count(); ++i)
		{
			nodeAndAllChildRunAction((CCNode*)childs->objectAtIndex(i), (CCAction*)action->copy()->autorelease());
		}
	}
}


// colorOrOpacity : 1/color   2/opacity   3/both
static void setAllChildrenColorOrOpacity(CCNode* node, ccColor3B color, GLubyte opacity, int colorOrOpacity)
{
	if (node == NULL) return;

	CCArray *children = node->getChildren();
	if( children != NULL )
		for (int i=0; i!=children->count(); ++i)
		{
			CCNode *node = dynamic_cast<CCNode*>(children->objectAtIndex(i));
			setAllChildrenColorOrOpacity(node, color, opacity, colorOrOpacity);
		}

		CCSprite *child = dynamic_cast<CCSprite*>(node);
		if (child)
		{
			if(colorOrOpacity == 1)
				child->setColor(color);
			else if(colorOrOpacity == 2)
				child->setOpacity(opacity);
			else if(colorOrOpacity == 3)
			{
				child->setColor(color);
				child->setOpacity(opacity);
			}

		}

}


}