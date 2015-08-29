#ifndef __CircleSprite_H__
#define __CircleSprite_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseShakeLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
class PrimarySprite : public CCSprite
{
protected:
	ccColor4F color;
public:
	virtual void draw() = 0;
};

class CircleSprite : public PrimarySprite
{
private:
	CCPoint center; //圆形的中心点
	float radius;   //半径
	float width;    //线条宽度
	bool fill;      //是否填充，暂未实现
public:
	//创建函数
	static CircleSprite* create(CCPoint cen, int rad, float width, ccColor4F color){
		CircleSprite *pSprite = new CircleSprite();
		if (pSprite && pSprite->init())//pSprite->initWithTexture(NULL, CCRectMake(cen.x, cen.x, rad*2, rad*2)))
		{
			//
			pSprite->width = width;
			pSprite->center = cen; 
			pSprite->radius = rad; 
			pSprite->color = color;
			pSprite->setPosition(cen);  //注意这里需要设置精灵的位置
			//
			pSprite->autorelease(); 
			return pSprite;
		}
		CC_SAFE_DELETE(pSprite);
		return NULL;

	}
public:
	void draw(){ 
		glLineWidth(width);
		ccDrawColor4F(color.r, color.g, color.b, color.a);
		//根据圆的半径来计算分割份数，一般够用
		ccDrawCircle(CCPointZero, radius, 0,radius*2.0 , false);  //注意这里用坐标(0,0)！
	}
};
#endif