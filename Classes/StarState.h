#ifndef __StarState_H__
#define __StarState_H__
#include "cocos2d.h"

USING_NS_CC;
//游戏中三星状态显示类
class StarState : public CCNode{
private:
	CCNode* _dynamicLayer;//动态处理显示层
	unsigned int _clickCount;//当前操作步骤数量
	unsigned int _erxingCount;		//本关卡中允许二星的最大操作次数	
	unsigned int _sanxingCount;			//本关卡中允许三星最大操作次数
	unsigned int _lastStars;//最终获得的星星
	void addChildren();//初始化显示对象


	CCPoint			m_pointOne;
	CCPoint			m_pointTwo;
	CCPoint			m_pointThree;

	bool			m_bIsThreeStarRemove;
	bool			m_bIsSecondStarRemove;
	CCNode			*m_nShakeNode;
public:
	StarState();
	virtual ~StarState();
	virtual bool init();
	unsigned int getLastStars();//获取最后获得的星星数量
	void onClick(int effectiveClickNum);	//传入有效点击数
	CREATE_FUNC(StarState);
	static StarState* create(unsigned int sanxingCount, unsigned int erxingCount);
	void checkClick();
	void removeSecondxing(CCNode* pNode);
	void removeSanxing(CCNode* pNode);
	void useSuperTool(CCObject* pObj);

	void teach_scaleChange(int type);

private:
	void showStarShake(CCNode *node);
	void starShake(float dt);
	void continShake();
	void showStarShakeEnd(CCNode *node);

	void removeStar(CCNode *node);
	void removeThisStar(CCNode *node);
	void removeStarEnd(CCNode *node);
};

#endif