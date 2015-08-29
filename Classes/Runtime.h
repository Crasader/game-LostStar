
#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include "cocos2d.h"
#include "Debug.h"
using namespace cocos2d;

//
//一个用于存储运行时的数据的类;
//
struct GameScore
{
	// 数据信息
	int timeStar;					// 时间相关的获得的星星数量
	int clickStar;					// 根据点击相关的星星获得数量

	float timeCost;					// 游戏耗时
	int clickedNum;					// 点击的次数，任何的一次点击都是
	int effectiveClickedNum;		// 有效点击的次数
	int usedItemNum;				// 使用过的元素个数
	int usedKeyItemNum;				// 使用过的关键元素个数
	bool isSuccessedOnKeyTable;		// 成功时，红星是否停在关键台面上
	int keyItemMedalNum;			// 关键元素的勋章个数
	int keyTableMedalNum;			// 关键台面的勋章个数
	bool isABetterScore;			// 是否是一个更好的成绩
	bool isUsedSuperTool;			// 是否使用了超能道具

	GameScore()
	{
		timeStar = 0;
		clickStar = 0;					// 根据点击相关的星星获得数量
		timeCost = 0;					// 游戏耗时
		clickedNum = 0;					// 点击的次数，任何的一次点击都是
		effectiveClickedNum = 0;		// 有效点击的次数
		usedItemNum = 0;				// 使用过的元素个数
		usedKeyItemNum = 0;				// 使用过的关键元素个数
		isSuccessedOnKeyTable = false;	// 成功时，红星是否停在关键台面上
		keyItemMedalNum = 0;			// 关键元素的勋章个数
		keyTableMedalNum = 0;			// 关键台面的勋章个数
		isABetterScore = false;			// 是否是一个更好的成绩
		isUsedSuperTool = false;		// 是否使用了超能道具
	}

	GameScore& operator = (const GameScore& obj){
		timeStar = obj.timeStar;
		clickStar = obj.clickStar;
		timeCost = obj.timeCost;
		clickedNum = obj.clickedNum;
		effectiveClickedNum = obj.effectiveClickedNum;
		usedItemNum = obj.usedItemNum;
		usedKeyItemNum = obj.usedKeyItemNum;
		isSuccessedOnKeyTable = obj.isSuccessedOnKeyTable;
		keyItemMedalNum = obj.keyItemMedalNum;
		keyTableMedalNum = obj.keyTableMedalNum;
		isABetterScore = obj.isABetterScore;
		isUsedSuperTool = obj.isUsedSuperTool;
		return *this;
	};

	int getStarNum()
	{
		return clickStar + timeStar;
	}

	void log()
	{
#ifdef DEBUG_ENABLE_SUCESS_LOG
		CCLog("");
		CCLog("====================== GameScore ===========================");
		CCLog("timeStar = %d",timeStar);
		CCLog("clickStar = %d",clickStar);
		CCLog("timeCost = %f",timeCost);
		CCLog("clickedNum = %d",clickedNum);
		CCLog("effectiveClickedNum = %d",effectiveClickedNum);
		CCLog("usedItemNum = %d",usedItemNum);
		CCLog("usedKeyItemNum = %d",usedKeyItemNum);
		CCLog("isSuccessedOnKeyTable = %s",isSuccessedOnKeyTable?"True":"False");
		CCLog("keyItemMedalNum = %d",keyItemMedalNum);
		CCLog("keyTableMedalNum = %d",keyTableMedalNum);
		CCLog("isABetterScore = %s",isABetterScore?"True":"False");
		CCLog("====================== GameScore ===========================");
		CCLog("");
#endif
	}
};

class Runtime
{
private:
	int effectiveClickNum;				// 有效点击的点击次数
	int clickNum;						// 所有的点击都会记录
	bool canTiming;						// 是否可以计时

	int usedItemNum;					// 所使用的元素的个数
	int usedKeyItemNum;					// 使用了的关键元素个数
	float timeCost;						// 记录游戏用时
	bool isSuccessedOnKeyTable;			// 在关键桌面上成功的
	bool isUsedSuperTool;

	static Runtime* instance;

	GameScore mGameScoreBuffer;			// 缓存，用于保存 genGameScore 之后的结果，可以通过getGameScore获得
	bool effectiveGameScore;			// 标记mGameScoreBuffer是否是有效的，gen之后就有效，clear之后就无效
	Runtime();

public:
	static Runtime* getRuntime();
	// 为了能够成功后，在任何时候都能获得相同的数据，
	// 所有应该在一成功之后就调用genGameScore，保存游戏的数据
	// 之后的所有获取都应该从getGameScore获取，这样既高效，
	// 还能保证数据的一致性
	GameScore genGameScore();			// 生成游戏成绩的数据，在成功之后调用.是基于当前游戏数据
	GameScore getGameScore();			// 返回上次genGameScore的结果

	void	gameLoop(float time);		// 游戏的tick里面回调，用于计时
	void	setSuccessdOnKeyTable();	// 在关键台面上成功了
	void	setUsedSuperTool(bool used);
	void	oneMoreItemUsed();			// 又一个Item被使用了调用
	void	oneMoreKeyItemUsed();		// 又一个关键的Item被使用了
	void	clearData();				//清除记录的数据，只在BaseContentLayer构造时清空
	void	click();					//发生一次点击，用于记录该次点击
	void	effectiveClick();			//一次有效点击（只是点击可消除的才记录）
};


#endif