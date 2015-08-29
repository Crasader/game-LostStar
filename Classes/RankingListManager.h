#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

struct RankList_PerfectPassOrEvaluate
{
	unsigned int order; 			//排名
	unsigned int val;	//3星通关数
	string name;		//用户昵称
	unsigned int time;		//提交时间
	unsigned int id;		//用户id

	RankList_PerfectPassOrEvaluate()
	{
		order = 0;
		val = 0;
		name = "";
		time = 0;
		id = 0;
	}
};


class RankingListManager
{
private:
	RankingListManager();
public:
	static RankingListManager* getInstance();

	//3星通关
	vector<RankList_PerfectPassOrEvaluate> getOrderPerfectPass(); 
	//成就数量
	vector<RankList_PerfectPassOrEvaluate> getOrderEvaluate(); 
	//3星通关
	void setOrderPerfectPass(vector<RankList_PerfectPassOrEvaluate> vec); 
	//成就
	void setOrderEvaluate(vector<RankList_PerfectPassOrEvaluate> vec);  




	RankList_PerfectPassOrEvaluate getMyPerfectPass();
	RankList_PerfectPassOrEvaluate getMyEvaluate();
	void setMyEvaluateOrder(unsigned int val);
	void setMyPerfectPassOrder(unsigned int val);

	void reorderList(vector<RankList_PerfectPassOrEvaluate>& vec);
	void save();
private:
	unsigned int _myPerfectPassOrder;
	vector<RankList_PerfectPassOrEvaluate> _perfectPassOrderList;

	unsigned int _myEvaluateOrder;
	vector<RankList_PerfectPassOrEvaluate> _evaluateOrderList;
};