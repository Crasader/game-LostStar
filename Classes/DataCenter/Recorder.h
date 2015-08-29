//动态记录仪


#pragma once
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "cocos2d.h"
using namespace cocos2d;

#define RECORD_ASYNC 1


using std::vector;
using std::string;

enum eventID
{
	eventID_None = 0,
	eventID_Touch,
	eventID_TouchBomb_DarkWhole,
	eventID_Remove,
	eventID_Motor_On,
	eventID_Motor_Off,
	eventID_Strobe_On,
	eventID_Strobe_Off,
	eventID_BoilerStart,
	eventID_BoilerEnd,
	eventID_AddOutRangeItem,
	eventID_setToBomb,

	eventID_SpringBedAction,
	eventID_CutRopeAction,

	eventID_CutRopeStart = 1000,
};

struct record
{
	int id;
	float posX;
	float posY;
	float rotate;
	int eventId;
	record()
	{
		id = -1; posX = 0; posY = 0; rotate = 0; eventId = eventID_None;
	}
};

class Recorder : public CCNode
{
protected:
	Recorder(){}
	bool initData();
	static Recorder* _recorder;
public:
	static Recorder* getInstance();
	void resetUserData(); //用户登录，重置数据（用户第一次登录才有用）

	//获取用户自己的解法
	bool setOneSectionRecord(int sectionIdx, const vector<vector<record> >* list);
	bool getOneSectionRecord(int sectionIdx, vector<vector<record> >* _out);

	//获取官方解法
	bool getOfficialRecord(int sectionIdx, vector<vector<record> >* _out);


	//通过内存数据（string）解析出一个记录
	bool getRecordFromString(const string& _in_data, vector<vector<record> >* _out);
	//file 必须为doc文件 （sqllite_open需要可写权限才能打开成功）
	bool getRecordFromFile(const char* absoluteFileName, vector<vector<record> >* _out);
	//通过压缩数据获取解法
	bool getRecordFromCompressString(const char* _in_data, unsigned int size, vector<vector<record> > *_out);

	//获取 指定账号 指定关卡 的解法文件（绝对路径）
	// 不传 platform 和 id 获取的是未绑定社交账号的目录
	string getSectionUserRecordAbsoluteFileName(int seasonId, int sectionId, string platform="", string userId="");
	//获取某关卡 官方解法 文件绝对路径
	string getSectionOfficialRecordAbsoluteFileName(int seasonId, int sectionId);

protected:
	//用于多线程保存自己数据时使用 (makeSaveFile只能在2dx线程中调用)
	string makeSaveFile(int sectionIdx); //创建保存solution的文件，返回文件全路径
	//saveRecordToFile 不能使用2dx提供的接口
	static bool saveRecordToFile(string fileName, const vector<vector<record> >* list);



#if RECORD_ASYNC == 1
public:
	//Warning: 该回调接口是由recoder线程直接调用，所以请不要进行UI相关的操作
	typedef std::function<void (bool, int, string)> pfuncRecordCallback;  // bool: 保存成功否   int: sectionIdx   string: error message
	void setOneSectionRecordAsync(int seactionIdx, vector<vector<record> >* list, pfuncRecordCallback saveEndCallback);
protected:
	void threadRecord();//不能使用2dx提供的接口
	void asyncSaveEndDispacher(float dt);
private:
	//
	std::thread m_threadRecord;
	std::mutex m_mutexRecordDataQueue;
	std::condition_variable m_recordQueueCondVar;
	std::queue<std::tuple<int, string, vector<vector<record>>*, pfuncRecordCallback> > m_recordQueue;
	//
	std::mutex m_mutexCallbackQueue;
	std::queue<std::tuple<bool, int, pfuncRecordCallback, string>> m_recordCallbackQueue;
#endif

};