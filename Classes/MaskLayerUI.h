#ifndef __MaskLayerUI_H__
#define __MaskLayerUI_H__

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
USING_NS_CC;
class MaskLayerUI : public CCLayer
{
public:
	MaskLayerUI();
	virtual ~MaskLayerUI();

	
	virtual bool init();
	CREATE_FUNC(MaskLayerUI);
	virtual void onEnter();
	virtual void onExit();

	void addCircle(CCPoint centerPos);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void inline setTouchPriority(int touchPriority)
	{
		m_nTouchPriority = touchPriority;
	}
private:
	
private:
	CCPoint				m_pCircleCenterPos;
	vector<CCLayerColor*>m_lNodeList;
	int					m_nRadius;
	int					m_nTouchPriority;
};

#endif // __MaskLayerUI_H__