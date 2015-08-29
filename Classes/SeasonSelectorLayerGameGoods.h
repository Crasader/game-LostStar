#ifndef __SeasonSelectorLayerGameGoods__
#define __SeasonSelectorLayerGameGoods__

#include "cocos2d.h"
#include "iostream"
using namespace std;
#include "vector"
USING_NS_CC;

class SeasonSelectorLayerMenu;

//游戏剧情道具类
class SeasonSelectorLayerGameGoods : public CCLayer{
private:
	//关卡编号seasonId
	unsigned int _seasonId;
	//控制
	bool _canNotTouche;
	//标题类 
	CCNode* _title;
	//已经获得的道具显示对象数组
	CCArray _goodsArray;
	//分享按钮
	CCMenu* _shareMenu;
	//旋转层
	CCNode* _xuanzhuanLayer;
	//菜单层
	SeasonSelectorLayerMenu* _menusLayer;

	CCLayerColor					*m_sLayerColor;
	CCNode							*m_sPhotoSpr;
	CCNode							*m_sOldPicSpr;
	bool							m_bIsTeaching;
	CCPoint							m_pCirclePos;
	unsigned int					m_nNodeCount;
	CCSprite						*m_sBianKuang;
	vector<CCSprite*>				m_sStarList;
public:
	void showOpenUnlock();
	//初始化头部信息
	void updateChildren();
	inline void setCircleTeachPos(CCPoint point)
	{
		m_pCirclePos = convertToWorldSpace(point);
	}
private:
	void nodeAndAllChildRunAction(CCNode* node, CCActionInterval* action,CCObject *obj = NULL,SEL_CallFuncN handler = NULL);
	void shakeEnd(CCNode *node);
	void beginTeach();//开始教学
	//点击没有获得的道具
	void onClickGameGoods(CCObject* object);
	//开始分享
	void onShared(CCObject* objeect);
	//动作完成
	void onRunActionComplete(CCNode* object);
	//收藏动作完成
	void onChoucangActionComplete(CCNode* object);
	//商店购买回调
	void onPayResult(CCObject* object);
	void Blink(CCNode *node,float dur);
	inline void BlinkEnd()
	{
		m_sBianKuang->stopAllActions();
		m_sBianKuang->removeFromParent();
		for (vector<CCSprite *>::iterator iter = m_sStarList.begin();iter != m_sStarList.end();iter ++)
		{
			(*iter)->stopAllActions();
			(*iter)->removeFromParent();
		}
	}
	void showFlashLight(CCNode *photo);//闪光灯的特效
	void showPhoto();
	void showFlashLightEnd(CCNode *photo);//闪光灯的特效结束
	void showWhiteLight();
	void showLightEffects(float dt);
	void showShake(CCNode *node);
	inline float randScale()
	{
		float fRet = 0.65f;
		float maxScale = 0.8f;
		float minScale = 0.5f;
		fRet = CCRANDOM_0_1()*(maxScale-minScale) + minScale;

		return fRet;
	}

	inline float randRotation()
	{
		float fRet = 0.0f;
		float maxScale = 360.0f;
		float minScale = 0.0f;
		fRet = CCRANDOM_0_1()*(maxScale-minScale) + minScale;

		return fRet;
	}
	inline void BlinkEnd(CCNode *node)
	{
		Blink(node,0.8f);
		//node->runAction(CCRepeatForever::create(CCBlink::create(5,6)));
	}

	inline void nodeAndAllChildRunAction(CCNode* node, CCAction* action)
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
public:
	SeasonSelectorLayerGameGoods();
	virtual ~SeasonSelectorLayerGameGoods();
	virtual void setSeasonId(unsigned int seasonId);
	//开始解锁剧情道具
	virtual void openUnlock();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onExit();
	virtual void onEnter();
	CREATE_FUNC(SeasonSelectorLayerGameGoods);
	static SeasonSelectorLayerGameGoods* create(CCNode* title,SeasonSelectorLayerMenu* menu);
};

#endif // !__SeasonSelectorLayerGameGoods__