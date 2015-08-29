
#ifndef __MAIN_FRAME_LAYER_H__
#define __MAIN_FRAME_LAYER_H__

#include "cocos2d.h"
#include "CommonDefine.h"
#include "BaseGuide.h"
#include "GameInforEditer.h"
#include "Music.h"
#include <string>
#include "BaseMainOptionLayer.h"
#include "PhysicsLayer.h"
#include <vector>
#include "LFAlert.h"
/**牛秋晨修改与5月7日**/
#include "BaseMoveUI.h"
#include "MainThingEffect.h"
#include <map>
/*****/

#include "cocos-ext.h"


using namespace cocos2d::extension;

using namespace cocos2d;
using namespace std;
const float KDMJEyeTime = 0.1f;//都敏俊眨眼所需要的时间
const float KDMJEyeSideTime = 2.0f;//都敏俊两次眨眼所间隔的时间

const float KQsyEyeTime = 0.07f;//千颂伊眨眼所需要的时间
const float KQsyEyeSideTime = 2.0f;//千颂伊两次眨眼所间隔的时间

const float KDMJPushUpTime = 0.1f;//都敏俊抬手所需要的时间
const float KDMJPushUpSideTime = 3.0f;//都敏俊两次抬手所间隔的时间
const float KDMJPushDownTime = 2.0f;//都敏俊抬手到放手所间隔的时间
class MainFrameLayer : public BaseMainOptionLayer,public BaseMoveUI
{
public:
	//创建一个Logo的场景
	static CCScene* createScene();
	static MainFrameLayer* createLayer();
	~MainFrameLayer();

	
public:
	virtual void moveChangeEnd(MainThingEffect *node);
private:
	void moveThing();
	void dmjEye(float dt);
	void qsyEye(float dt);

	void dmjPushUpArm(float dt);
	void dmjPushDownArm(float dt);
	void dmjStar(float dt);

	MainFrameLayer();
	virtual void keyBackClicked();//Android 返回键
	virtual void keyMenuClicked();//Android 菜单键

	void startItemClicked(CCObject* obj);
	virtual void settingClicked(CCObject* obj);
	virtual void shareClicked(CCObject* obj);
	void storeClicked(CCObject *obj);
	void rankingClicked(CCObject *obj);
	void collectClicked(CCObject *obj);
	void restarClicked(CCObject* obj);
	void achieveClicked(CCObject* obj);
	void languageItemClicked(CCObject*obj);
	void exitItemClicked(CCObject* obj);

	void helpItemClicked(CCObject* obj);
	void moreGameItemClicked(CCObject* obj);

	void setMenuItemVisible(CCMenuItemImage* menuItem,CCMenuItemImage* menuItemRoot, bool up=true);
	

	void moveOut(vector<CCMenuItemImage*> group,CCNode* root, bool up=true);
	void moveBack(vector<CCMenuItemImage*> group,CCNode* root, bool down=true);

	void toShowUpdateAlertIfNeed(float t);
	void toShowUFO(float dt);
	void toShowStarEffect(float dt);
	void toShowStartMenuEffect(float dt);
	void commentOkClicked(CCObject* obj);
	void commentNeverClicked(CCObject* obj);
	void commentLatterClicked(CCObject* obj);

	//void toShowInputPhoneNumerIfNeed(float t);
	void phoneNumberInputed(CCObject* null_obj);
	virtual void onExit();
	virtual void onEnter();
	
	

	void tick(float t);

	void rotateBack(CCNode *node);


	//动画
	
	void dataLoaded(float percent);
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void playerAnimation(CCNode* node,string animationame,CCPoint point,bool isloop);
	void animationEvent(CCArmature *armature, MovementEventType movementType, const char *movementID);
	void xinUi();
	void yuntuoXian();
	void Buttonfadein();

	void facebookClicked(CCObject* obj);
	void setClicked(CCObject* obj);
	void achiveClicked(CCObject* obj);
	void gamecenterClicked(CCObject* obj);

	void onFrameEvent(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);
	
private:
	vector<MainThingEffect*>		m_sMainThingList;
	map<int,CCSprite *>		m_sDmjEyeList;
	int								m_nDmjEyeCount;
	map<int,CCSprite *>		m_sQsyEyeList;
	int								m_nQsyEyeCount;

	map<int,CCSprite *>		m_sDmjArmList;
	int								m_nDmjArmCount;

	map<int,CCSprite *>		m_sDmjStarList;
	int								m_nDmjStarCount;

	int								m_nStarRotate;

	CCSprite* mBG;

	vector<CCMenuItemImage*> settingGroup;
	vector<CCMenuItemImage*> shareGroup;
	CCMenuItemImage* shareItem;
	CCMenuItemImage* languageItem;

	CCMenuItemImage* storeItem;
	CCMenuItemImage* rankingItem;
	CCMenuItemImage* mCollectItem;


	CCSprite* setPic;
	CCSprite* sharePic;
	LFAlert* lfAlert;

	MainThingEffect *mTilte;
	MainThingEffect *mStar;
	CCMenuItemImage *mStartItem;

	float width;
	float hight;
	CCArmature* armature;
	int mark;
};



#endif