#ifndef __Debug__
#define __Debug__

#include "cocos2d.h"
using namespace cocos2d;

/**
 *	一个开启和关闭调试功能的头文件，发布的时候，全部禁用Debug即可
 *	规定一下，这里面定义的都是DEBUG_ENABLE_****这种，即都是是否启用某东西
 *	这样编程，就不会有疑问了
 */


//#define DEBUG_ENABLE_PACKAGE					// 启用背包按钮
//#define DEBUG_ENABLE_TOUCHMOVE_JUMP_LEVEL		// 启用滑动跳关
//#define DEBUG_ENABLE_TEXTURE_LOAD_LOG			// 启用纹理载入的输出日志
//#define DEBUG_ENABLE_MYSCORLLVIEW_LOG			// 启用MyScrollView日志
//#define DEBUG_ENABLE_SUCESS_LOG				// 启用成功时输出日志
//#define DEBUG_ENABLE_ACHIEVE_LOG				// 启用成就输出日志
//#define DEBUG_ENABLE_ALLWAYS_SHOW_HELP		// 一直都有Help按钮的显示
//#define DEBUG_ENABLE_ALLWAYS_NO_HELP			// 一直都没有Help按钮的显示

//#define DEBUG_ENABLE_TEXTURE_ASSERT		// 是否启用 Texture的assert，这再调试的时候打开，发布的时候请关闭。

#define DEBUG_BuyNotNeedSDK 1
#define DEBUG_IOSMarket 0

#endif