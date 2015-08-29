#ifndef __CrossPlatformSizeManager_H__
#define __CrossPlatformSizeManager_H__
#include "cocos2d.h"

using namespace cocos2d;

//
//	这是一个关于不同分辨率下的一个缩放管理类,注意这是在没有开启view->setDesignResolutionSize(480, 320);
//	的情况下处理的，因为按理说开启了刚才的函数应该是可以平滑的帮我们的480,320的所有东西过渡到任何屏幕下面，但是，在ScrollVIew上行不通;
//	于是便有了这个管理的封装类，此类就用于将480,320的游戏大小数据，转化到当前屏幕的一个过渡的类。任何涉及到写像素，大小，长宽等过程时;
//	用户可以,直接按照480,320的数据写，但是最终给CCNode的时候，需要用这个类转一下。;
//	
//	注意一切的大小获取都用boundingBox，不要用contentSize，因为游戏中会有缩放，等。所以不要使用contentSize;
//
class CrossPlatformSizeManager
{
private:
	static CrossPlatformSizeManager* manager;
	CCSize CrossPlatform_defaultWinSize;
	CCSize CrossPlatform_currentWinSize;
	float factor;//当前的缩放因子;
	float getScaleFactor();//取得当前设备的显示分辨率和默认480*320分辨率的缩放因子;如果当前设备更大，那么就返回大于1的浮点数
	CrossPlatformSizeManager();

public:
	static CrossPlatformSizeManager* getManager();
	//实际真正的游戏屏幕的大小;
	CCSize getCurrentWinSize();
	//完成将在默认情况下的size，转化成当前设备的size;
	//默认情况下的size是480,320，也就是如果参数是480,320，而实际当前的大小是960,640，那么返回的值就是960,640;
	//如果不是长宽都一个比例，那么就使用能放下的模式;
	CCSize makeSize(CCSize size);
	CCPoint makePoint(CCPoint point);
	CCSprite* makeSprite(CCSprite* sprite);
	CCNode* makeNode(CCNode* node);

	//取得当前设备的显示分辨率和默认480*320分辨率的缩放因子;如果当前设备更大，那么就返回大于1的浮点数;
	float getFactor();

};
#endif