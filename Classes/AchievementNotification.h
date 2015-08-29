#ifndef __AchievementNotification_H__
#define __AchievementNotification_H__

#include "LFLayer.h"
#include "AchievementManager.h"
#include <string>

#include "particle_nodes/CCParticleExamples.h"
#include "particle_nodes/CCParticleSystem.h"

using namespace std;

class AchievementNotification : public LFLayer
{
private:
	//CCSprite* boardSheet;
	string fileName;
	string parFileName;
	static AchievementNotification* instance;
	bool mShowing;
	bool forceTermination;	// 强制终止

	void setString(CCLabelTTF* lable ,CCSprite* base, string str);

	// 显示了之后，回去了，才调用，用于移除这个board和显示下一个board
	void boardShowEnd(CCNode* board);
	// 刚移下来，显示。就调用
	// 用于移除显示任务，代表他完全显示了一次
	void boardShowing(CCNode* board,void* achievement);

	CCParticleSystem* createPar(CCPoint pos,const char* file,float width,float height);
protected:
	AchievementNotification();

	// 开始监控
	void startMonitoring(float t);

	void tick(float t);

	// 激活没有完成的任务
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	// 停止正在完成的任务
	virtual void onExit();

	/**
	 *	每次放到场景里面run的时候，就会开始
	 */
	void showImmediately(Achievement a);
public:
	static AchievementNotification* create();

	static AchievementNotification* currentLayer();

	/**
	 *	每次都会新创建一个board所有可以不断的显示消息
	 *	
	 *	只是加入到显示队列
	 */
	void showAchievement(Achievement a);

	/**
	 *	关闭监控，就不会再从队列里面取数据去显示了。
	 *	主要用于跨关的时候，我们可以先关闭它，再加入。
	 */
	void stopMonitoring();
};


#endif