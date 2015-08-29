/*
 * CCSprite的封装，用于创建网络图片的sprite
 * 主要用于获取社交平台朋友头像后，是一个url链接，改类实现使用图片链接创建一个sprite
 * 警告：创建时，会立刻从网络加载图片，为了创建sprite后立刻有视觉效果，
 * --需要调用者再提供一张本地图片作为默认显示。当网络图片加载到本地后，将会更新显示。
*/



#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
USING_NS_CC;
USING_NS_CC_EXT;
using std::string;

#define NATIVE_PICTURE_URL "nativeFacebookPicture_"

// 可以显示在线图片的CCSprite
class urlSprite : public CCSprite
{

public:
	/*
		创建一个显示在线图片的Sprite
		* defaultImagePath		默认图片路径（当图片加载中时显示）
		* url								图片url 
		* @  如果url使用本地文件，那么文件名须以 "nativeFacebookPicture_"为前缀，如：  nativeURL_MyPhoto.png
	*/
	static urlSprite* create(const char* defaultImagePath, const char* url);

	bool init(const char* defaultImagePath, const char* url);

	void onDownloadCompleted(CCHttpClient *sender, CCHttpResponse *response);
protected:
	// 下载图片
	void downloadImage(const char* url);
	// 获取图片路径
	std::string getImagePath(const char* url);
	// 图片是否存在
	bool isExist(const char* url);
	// 保存图片
	std::string saveImage(const std::string& data);
private:
	string m_url;
};
