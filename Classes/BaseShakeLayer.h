#ifndef __BaseShakeLayer_n_H__
#define __BaseShakeLayer_n_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vector"
#include "iostream"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

class BaseShakeLayer : public CCLayer
{
public:
	BaseShakeLayer();
	virtual ~BaseShakeLayer();

	static CCScene* scene();
	virtual bool init();
	CREATE_FUNC(BaseShakeLayer);
	virtual void onEnter();
	virtual void onExit();
	void replaceLayer(CCNode *outNode,CCNode *inNode,CCObject *obj = NULL,SEL_CallFuncN handler = NULL);
	void closeMyself();
private:
	void showShake();
	void removeLayerColor(CCNode *node);

	void nodeAndAllChildRunAction(CCNode* node, CCActionInterval* action,CCObject *obj = NULL,SEL_CallFuncN handler = NULL);
private:
	CCLayerColor				*m_sColorLayer;
	size_t				m_nNodeCount;
	vector<int>					m_nNodeCountList;
};

#endif // __BaseShakeLayer_n_H__