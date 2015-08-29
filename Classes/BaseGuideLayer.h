#ifndef __BaseGuideLayer_H__
#define __BaseGuideLayer_H__
#include "LFLayer.h"
#include <string>
#include "ResManager.h"
#include "BaseContentLayer.h"
#include "SpriteHelper.h"
#include "font.h"
#include "LFStrings.h"

using namespace std;
class BaseScense;

#define Finger_Pos_Offset_X 48.5f
#define Finger_Pos_Offset_Y (-49.0f - 12.0f) 

#define Tag_Shadow	1211

#define Finger_Armature_DianJi	"dianji"
#define Finger_Armature_ChuiQi	"chuiqi"
#define Finger_Armature_Finger	"finger"


class BaseGuideLayer : public LFLayer
{
protected:
	int mSeasonId;
	int mSectionId;
	cocos2d::extension::CCArmature*		mFinger;
	ResManager* resMgr;
	BaseScense* mBaseScene;
	CCSize mWinSize;
	BaseContentLayer* mBaseContentLayer;
	bool mBaseContentLayerTouchEff;
		
	void setBaseContentLayerCurrentTouchEffectve(bool eff){mBaseContentLayerTouchEff = eff;};

	void pauseBaseContentLayer();
	void resumeBaseContentLayer();

	string getTxtImageFile(std::string file);
	string getImageFile(std::string file);

public:
	BaseGuideLayer(int seasonId,int sectionId);

	void setBaseScene(BaseScense* scene);

	void setBaseContentLayer(BaseContentLayer* layer);

	// baseContentLayer中的事件的回调，如果不需要他baseContentLayer处理，那么就返回false
	// 就可以屏蔽了 begin的操作
	virtual bool baseContentLayerTouchsBegin(CCSet *pTouches, CCEvent *pEvent){return mBaseContentLayerTouchEff;};
	virtual bool baseContentLayerTouchsMoved(CCSet *pTouches, CCEvent *pEvent){return mBaseContentLayerTouchEff;};
	virtual bool baseContentLayerTouchsEnd(CCSet *pTouches, CCEvent *pEvent){return mBaseContentLayerTouchEff;};
	
	// 有效点击之后 会回调,NULL 代表是绳子
	virtual void baseContentLayerEffctiveClicked(CCNode* node){};

	virtual void startGuide(){};

	virtual void stopGuide();
	void hideAll();
	virtual void pause();
	virtual void resume();

	virtual void labelRunAction(CCLabelTTF* pTraget, CCAction* pAction);
	
	static CCLabelTTF* createShadowLabel(const char *string, const char *fontName = fontStr_KlavikaBold, float fontSize = 26);
	static CCSprite* createShadowArrow(const char *string);
};

#endif