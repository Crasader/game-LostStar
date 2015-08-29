//
//  BaseBackgroundLayer.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#ifndef __CurtainLayerClosedFrame__
#define __CurtainLayerClosedFrame__

#include "cocos2d.h"
#include <string>
#include <list>
#include "ResManager.h"
#include "SpriteHelper.h"

using namespace cocos2d;
using namespace std;

/**
 *	窗帘的一帧，只用于ipad高清情况，用于减少内存的。
 *	这样对于CurtainLayer，就可以全部都使用低清的。
 *	然后关闭的状态就最后用清晰的图片代替就是了
 */
class CurtainLayerClosedFrame : public CCLayer
{
private:
	CurtainLayerClosedFrame();
public:
	static CurtainLayerClosedFrame* create();
};
#endif
