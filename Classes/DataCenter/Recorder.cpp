
#include "Recorder.h"
#include "sqlite/sqlite3.h"
#include "GameLayerFactory.h"
#include "History.h"
#include "support/zip_support/ZipUtils.h"

#include <string>
using std::string;

// user folter
#define DEFAULT_USER_FOLTER "record_none_user/"
#define SUB_FOLTER_FORMAT "record_%s_%s/"
// official folter
#define OFFICIAL_FILTER "record_official/"

// user file name
#define USER_FILENAME_FORMAT "u_%d_%d.db"
//#define USER_CUSTOM_FILENAME_FORMAT "c_%d_%d.db"
// official file name
#define OFFICIAL_FILENAME_FORMAT "o_%d_%d.db"


#define SECTION_RECORD_TABLE_FORMAT "section"

Recorder* Recorder::_recorder = NULL;

Recorder* Recorder::getInstance()
{
	if (!_recorder){
		_recorder = new Recorder();
		_recorder->initData();
	}
	return _recorder;
}

bool Recorder::initData()
{
#if RECORD_ASYNC == 1
	CCNode::init();
	this->onEnter();
	this->onEnterTransitionDidFinish();
	this->schedule(schedule_selector(Recorder::asyncSaveEndDispacher), 0.5f);
	m_threadRecord = std::thread(&Recorder::threadRecord, this);
#endif

	return true;
}
void Recorder::resetUserData() //用户登录，重置数据（用户第一次登录才有用）
{
// 	//test
// 	if (! LFFileUtiles::getLFFileUtiles()->isFileExist(CCFileUtils::sharedFileUtils()->getWritablePath() + DEFAULT_USER_FOLTER + "u_1_1.db"))
// 	{
// 		LFFileUtiles::getLFFileUtiles()->createDocFileInNewFolder(DEFAULT_USER_FOLTER, "u_1_1.db");
// 	}
	//第一次登录，那么绑定到之前的数据 (修改sub_folter文件夹文字)
	string platform, id;
	if (History::getHistory()->getLastUserPlatformAndID(platform,id))
	{
		string noneUserFolter = CCFileUtils::sharedFileUtils()->getWritablePath() + DEFAULT_USER_FOLTER;
		noneUserFolter = noneUserFolter.substr(0,noneUserFolter.length()-1);// 去掉最后的 '/'
		string logedUserFolter = CCFileUtils::sharedFileUtils()->getWritablePath() + CCString::createWithFormat(SUB_FOLTER_FORMAT, platform.c_str(), id.c_str())->getCString();
		logedUserFolter = logedUserFolter.substr(0,logedUserFolter.length()-1);// 去掉最后的 '/'
		//直接改名字（有noneuserfolter才会成功）
		if (rename(noneUserFolter.c_str(), logedUserFolter.c_str()) == 0) {
			CCLog("%s ---> rename success [%s] to [%s]", __FUNCTION__, noneUserFolter.c_str(), logedUserFolter.c_str());
		} else {
			//没有就创建文件夹
			bool ret = LFFileUtiles::getLFFileUtiles()->createDocFileInNewFolder(
				CCString::createWithFormat(SUB_FOLTER_FORMAT, platform.c_str(), id.c_str())->getCString(), 
				CCString::createWithFormat(USER_FILENAME_FORMAT, 1, 1)->getCString());
			CCLog("%s ---> create new user folter %s", __FUNCTION__, ret?"true":"error");
		}
	}
}

string Recorder::makeSaveFile(int sectionIdx)//创建保存solution的文件，返回文件全路径
{
	std::pair<int, int> level = GameLayerFactory::getInstance()->getLevelFormat(sectionIdx);

	string rootPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	string fileName = CCString::createWithFormat(USER_FILENAME_FORMAT, level.first, level.second)->getCString();
	string folder;
	{
		string platform, id;
		if (History::getHistory()->getLastUserPlatformAndID(platform,id)){
			folder = CCString::createWithFormat(SUB_FOLTER_FORMAT, platform.c_str(), id.c_str())->getCString();
		}
		else {
			folder = DEFAULT_USER_FOLTER;
		}
	}
	string absPath = rootPath + folder + fileName;
	//
	//if (! LFFileUtiles::getLFFileUtiles()->isFileExist(absPath))
	//直接删除了再创建，为了防止其它地方对该文件读写错误后，sqlite操作将失败
	remove(absPath.c_str());
	{
		LFFileUtiles::getLFFileUtiles()->createDocFileInNewFolder(folder, fileName);
	}
	CCLOG("db path is %s", absPath.c_str());

	return absPath;
}
bool Recorder::saveRecordToFile(string absPath, const vector<vector<record> >* list)
{
	sqlite3 *pDb = NULL;
	int result;  
	result = sqlite3_open(absPath.c_str(), &pDb);  
	if(result != SQLITE_OK)  
	{
		printf("open %s database failed,  number %d",absPath.c_str(),result);
		return false;
	}

	char sql[258];
	//删表
	sprintf(sql, "DROP TABLE %s", SECTION_RECORD_TABLE_FORMAT);
	result = sqlite3_exec(pDb, sql, NULL,NULL,NULL); 
	if(result != SQLITE_OK){
		printf("drop table %s failed", SECTION_RECORD_TABLE_FORMAT);
		//		return false;
	}
	//建表  (帧id，物件id，posx，posy，rotate，eventid)
	sprintf(sql, "CREATE TABLE %s(frameIdx INTEGER,id INTEGER DEFAULT -1,posX REAL,posY REAL,rotate REAL,eventid INTEGER)", SECTION_RECORD_TABLE_FORMAT);
	result = sqlite3_exec(pDb, sql, NULL,NULL,NULL); 
	if(result != SQLITE_OK){
		printf("create table %s failed", SECTION_RECORD_TABLE_FORMAT);
		sqlite3_close(pDb);
		return false;
	}
	//插入数据
	result = sqlite3_exec ( pDb , "begin transaction" , 0 , 0 , 0 ); // 开始一个事务 
	for (int iFrame=0,countFrame=list->size(); iFrame<countFrame; ++iFrame)
	{
		const vector<record>& oneFrame = list->at(iFrame);
		for (int iId=0,countId=oneFrame.size(); iId<countId; ++iId)
		{
			sprintf(sql,"insert into %s values(%d, %d, %.1f, %.1f, %.1f, %d)", SECTION_RECORD_TABLE_FORMAT, 
				iFrame + 1, oneFrame.at(iId).id, oneFrame.at(iId).posX, oneFrame.at(iId).posY, oneFrame.at(iId).rotate,oneFrame.at(iId).eventId);
			result = sqlite3_exec(pDb,sql,NULL,NULL,NULL);  
			if(result != SQLITE_OK){
				printf("%s insert data failed!", SECTION_RECORD_TABLE_FORMAT);
				sqlite3_close(pDb);
				return false;
			}
		}
	}
	result = sqlite3_exec ( pDb , "commit transaction" , 0 , 0 , 0 ); // 提交事务 
	sqlite3_close(pDb);
	return true;
}

bool Recorder::setOneSectionRecord(int sectionIdx, const vector<vector<record> >* list)
{
	if (!list || list->empty()) return false;
	
	string absPath = makeSaveFile(sectionIdx);
	return saveRecordToFile(absPath, list);
}

bool Recorder::getOneSectionRecord(int sectionIdx, vector<vector<record> >* _out)
{
	if (!_out) return false;
	_out->clear();

	std::pair<int, int> level = GameLayerFactory::getInstance()->getLevelFormat(sectionIdx);

	string rootPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	string fileName = CCString::createWithFormat(USER_FILENAME_FORMAT, level.first, level.second)->getCString();
	string folder;
	{
		string platform, id;
		if (History::getHistory()->getLastUserPlatformAndID(platform,id)){
			folder = CCString::createWithFormat(SUB_FOLTER_FORMAT, platform.c_str(), id.c_str())->getCString();
		}
		else {
			folder = DEFAULT_USER_FOLTER;
		}
	}
	string absPath = rootPath + folder + fileName;

	return getRecordFromFile(absPath.c_str(), _out);
}

bool Recorder::getOfficialRecord(int sectionIdx, vector<vector<record> >* _out)
{
	if (!_out) return false;
	_out->clear();

	std::pair<int, int> level = GameLayerFactory::getInstance()->getLevelFormat(sectionIdx);
	string absPath = getSectionOfficialRecordAbsoluteFileName(level.first, level.second);

	//官方数据在包里，包里的文件是只读权限，sqlite_open不能打开只读权限的文件，所以此处不能使用getRecordFromFile
	unsigned long size;
	unsigned char* pData = CCFileUtils::sharedFileUtils()->getFileData(absPath.c_str(), "rb", &size);
	if (pData && size > 0){
		string temp((char*)pData, size);
		delete[] pData;
		return getRecordFromString(temp, _out);
	}else {
		CC_SAFE_DELETE_ARRAY(pData);
		return false;
	}
}

//通过内存数据（string）解析出一个记录
bool Recorder::getRecordFromString(const string& _in_data, vector<vector<record> >* _out)
{
	//检测是否是本地文件 （如果是本地文件 就拷贝到doc进行解析）
	{
		if (_in_data.substr(_in_data.length()-3) == ".db")
		{
			//return getRecordFromFile(_in_data.c_str(), _out); //官方配置的好友解法在包里 没有可写权限，sqlite_open会打开失败
			unsigned long size;
			unsigned char* pData = CCFileUtils::sharedFileUtils()->getFileData(_in_data.c_str(), "rb", &size);
			if (pData && size > 0){
				string temp((char*)pData, size);
				delete[] pData;
				return getRecordFromString(temp, _out);
			}else {
				CC_SAFE_DELETE_ARRAY(pData);
				return false;
			}
		}
	}



	const string TempRecordFile = CCFileUtils::sharedFileUtils()->getWritablePath() + "TempRecordFile.db";

	FILE *pf = fopen(TempRecordFile.c_str(), "wb");
	if (pf == NULL) {
		CCLog("%s ---> open file error : %s", __FUNCTION__, TempRecordFile.c_str());
		return false;
	}
	// 1. write to file
	int count = fwrite(_in_data.c_str(), sizeof(char), _in_data.length(), pf);
	fclose(pf);
	if (count != _in_data.length()){
		CCLog("%s ---> fwirte solution to file error, data length[%d], had write count[%d]", __FUNCTION__, _in_data.length(), count);
		return false;
	}
	// 2. parse
	return getRecordFromFile(TempRecordFile.c_str(), _out);
}

bool Recorder::getRecordFromCompressString(const char* _in_data, unsigned int size, vector<vector<record> > *_out)
{
	_out->clear();
	if (_in_data == nullptr || size == 0) return false;

	unsigned char* decompressData = NULL;
	int decompressDataLength = ZipUtils::ccInflateMemory((unsigned char*)_in_data, size, &decompressData);
	if (decompressDataLength > 0 && decompressData != NULL){
		return this->getRecordFromString(string((char*)decompressData, decompressDataLength), _out);
	}else {
		return false;
	}
}

bool Recorder::getRecordFromFile(const char* absoluteFileName, vector<vector<record> >* _out)
{
	if (! _out) return false;
	_out->clear();

	string absPath = absoluteFileName;

	sqlite3 *pDb = NULL;
	int result;  
	result = sqlite3_open(absPath.c_str(), &pDb);  
	if(result != SQLITE_OK)  
	{
		CCLOG("open %s database failed,  number%d",absPath.c_str(),result);
		return false;
	}

	//select
	CCString sql;  
	CCString temp;
	char **re;  
	int r,c;  
	{
		//第3个参数是查询结果，它依然一维数组（不要以为是二维数组，更不要以为是三维数组）。
		//它内存布局是：第一行是字段名称，后面是紧接着是每个字段的值
		// col个数据之后才是表数据，之前的是列名字
		temp.initWithFormat(SECTION_RECORD_TABLE_FORMAT);
		sql.initWithFormat("select * from %s", temp.getCString());
		result = sqlite3_get_table(pDb,sql.getCString(),&re,&r,&c,NULL);
		if(result != SQLITE_OK)  
		{
			CCLOG("select table %s failed, number%d",temp.getCString(),result);
			sqlite3_close(pDb);
			return false;
		}

		CCLOG("%s: row is %d,column is %d",temp.getCString(),r,c);
		if (r == 0)
		{
			sqlite3_close(pDb);
			return false;
		}
		int f = 0;
		int frameId = CCString(re[c]).intValue(); //start frame id 
		record oneInfo;
		vector<record> oneFrame;
		CCLOG("total row is %d ", r);
		for (int i=1; i <= r; ++i)
		{
			//CCLOG("row %d ---> id = %s, count = %s", i, re[i*c], re[i*c +1]);
			f = CCString(re[i*c]).intValue();
			oneInfo.id = CCString(re[i*c+1]).intValue();
			oneInfo.posX = CCString(re[i*c+2]).floatValue();
			oneInfo.posY = CCString(re[i*c+3]).floatValue();
			oneInfo.rotate = CCString(re[i*c+4]).floatValue();
			oneInfo.eventId = CCString(re[i*c+5]).intValue();
			if (f == frameId){
				oneFrame.push_back(oneInfo);
			}
			else if (f > frameId){
				_out->push_back(oneFrame);
				oneFrame.clear();
				oneFrame.push_back(oneInfo);
				++frameId;
			}
			else {
				CCLOG("%s : frameid error", temp.getCString());
			}
		}
		// 最后一帧数据
		_out->push_back(oneFrame);

		sqlite3_free_table(re); 
	}
	sqlite3_close(pDb);

	return true;
}





string Recorder::getSectionUserRecordAbsoluteFileName(int seasonId, int sectionId, string platform, string userId)
{
	string rootPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	string fileName = CCString::createWithFormat(USER_FILENAME_FORMAT, seasonId, sectionId)->getCString();
	string folder;
	if (userId.empty()){
		folder = DEFAULT_USER_FOLTER;
	}
	else {
		folder = CCString::createWithFormat(SUB_FOLTER_FORMAT, platform.c_str(), userId.c_str())->getCString();
	}
	return rootPath + folder + fileName;
}
string Recorder::getSectionOfficialRecordAbsoluteFileName(int seasonId, int sectionId)
{
	string filePath = OFFICIAL_FILTER;
	filePath += CCString::createWithFormat(OFFICIAL_FILENAME_FORMAT, seasonId, sectionId)->getCString();
	return CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str());
}





#if RECORD_ASYNC == 1
void Recorder::setOneSectionRecordAsync(int seactionIdx, vector<vector<record> >* list, pfuncRecordCallback saveEndCallback)
{
	CCLOG("%s", __FUNCTION__);
	if (list == nullptr) return;
	std::unique_lock<std::mutex> lock(m_mutexRecordDataQueue);

	vector<vector<record> >* pData = new vector<vector<record> >();
	*pData = *list;
	m_recordQueue.push(std::make_tuple(seactionIdx, makeSaveFile(seactionIdx), pData, saveEndCallback));
	m_recordQueueCondVar.notify_all();
}

void Recorder::threadRecord()
{
	while (true)
	{
		int sectionIdx = -1;
		string absPath;
		vector<vector<record> >* pData = nullptr;
		pfuncRecordCallback pFunc = nullptr;
		{
			std::unique_lock<std::mutex> lock(m_mutexRecordDataQueue);
			if (m_recordQueue.empty())
			{
				printf("%s ---> waiting record buffer\n", __FUNCTION__);
				//1.先释放lock  2.然后等待被唤醒  3.被唤醒后等待获取lock
				m_recordQueueCondVar.wait(lock);
			}
			std::tie(sectionIdx, absPath, pData, pFunc) = m_recordQueue.front();
			m_recordQueue.pop();
		}
		printf("%s ---> sectionIdx = %d, path = %s\n", __FUNCTION__, sectionIdx, absPath.c_str());

		if (pData && sectionIdx >= 0)
		{
			bool ret = Recorder::saveRecordToFile(absPath, pData);
			delete pData;

			std::unique_lock<std::mutex> lock(m_mutexCallbackQueue);
			m_recordCallbackQueue.push(std::make_tuple(ret, sectionIdx, pFunc, ""));

			//子线程中不能直接调用UI线程接口
// 			if (pFunc != nullptr){
// 				pFunc(ret, sectionIdx, "");
// 			}
			printf("====================================================\n");
			printf("%s ---> async save record %s\n", __FUNCTION__, ret?"success":"failed");
			printf("====================================================\n");
		}
		fflush(stdout);
	}
}

void Recorder::asyncSaveEndDispacher(float dt)
{
	std::unique_lock<std::mutex> lock(m_mutexCallbackQueue, std::try_to_lock);
	if (lock.owns_lock() && m_recordCallbackQueue.size() > 0)
	{
		bool ret = false;
		int sectionIdx = -1;
		pfuncRecordCallback pfunc = nullptr;
		string msg;
		std::tie(ret, sectionIdx, pfunc, msg) = m_recordCallbackQueue.front();
		m_recordCallbackQueue.pop();
		if (pfunc != nullptr)
		{
			CCLOG("%s ---> dispach record result", __FUNCTION__);
			pfunc(ret, sectionIdx, msg);
		}
	}
}

#endif