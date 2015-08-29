
#include "SocialSeasonInfor.h"
#include "CmdHelper.h"
#include "History.h"
#include "Recorder.h"
#include "include_json.h"
#include "LFFileUtiles.h"
#include "GameLayerFactory.h"
#include <cassert>
#include <cstdlib>

#if _USE_VIRTURE_USER_INFOR_ == 1
const char* _VIRTUE_USER_ID_		= "1386146831694197";
const char* _VIRTUE_USER_NAME_		= "fuxiao";
const char* _VIRTUE_USER_PICTURE_	= "https://fbcdn-profile-a.akamaihd.net/hprofile-ak-xfa1/v/t1.0-1/p50x50/10551094_1386928481616032_6900265282582756767_n.jpg?oh=0348537dd972bc5681a9fe6713fa3494&oe=556B4862&__gda__=1431737572_816ed28b49a4e61c61d4e0e169cb9c8d";
bool _VIRTUE_IsLogin = false;
#endif

#define _LOCAL_SOCIAL_TEMPLATE_DATA_ 0


#define FILE_PATH "config/FriendsInfor.txt"
#define OFFICIAL_SOLUTION_ID_FORMAT "CamelliaSolution_%s_%d_%d"


SocialInfoManager* SocialInfoManager::_instanceSocialManager = nullptr;


SocialInfoManager::SocialInfoManager()
	:m_bHasInitFriendsAndMe(false)
{
}
	
SocialInfoManager* SocialInfoManager::getInstance()
{
	if (! _instanceSocialManager){
		_instanceSocialManager = new SocialInfoManager();
	}
	return _instanceSocialManager;
}

bool SocialInfoManager::initLocalData()
{
#if _LOCAL_SOCIAL_TEMPLATE_DATA_ == 1
	initLocalTemplateData();
	return;
#endif

	LFFileUtiles *fileUtiles = LFFileUtiles::getLFFileUtiles();
	//fileUtiles->copyFileFromResToDocIfNotExist(Temp_FILE_PATH, NULL, FILE_PATH);

	Json::Reader reader;
	Json::Value root;

	unsigned long size = 0;
	//const char* doc = fileUtiles->readDocFile(FILE_PATH,&size);
	const char* doc = fileUtiles->readResourceFile(FILE_PATH,&size);
	//文件读取失败就返回NULL;
	if (size<=0)
	{
		CCLog("%s \n=======readInforFile---->Load gameInfor-> %s  fail !", __FUNCTION__, FILE_PATH);
		return false;
	}
	string doc_str = string(doc, size);
	delete[] doc;

	//解析不成功就返回空;
	if (!reader.parse(doc_str, root))
	{
		CCLog("\n=======parseInforString---->Parse social failed !");
		return false;
	}


	Json::Value jsonFriendsList = root["friendList"];
	Json::Value jsonSectionList = root["sections"];
	//1. friends
	mVirtualUserList.clear();
	{
		vector<TaskUser> friendsVec(jsonFriendsList.size());
		for (int i=0,count=jsonFriendsList.size(); i<count; ++i)
		{
			friendsVec[i].userId = jsonFriendsList[i]["ID"].asString();
			friendsVec[i].name = jsonFriendsList[i]["name"].asString();
			friendsVec[i].pictrue = jsonFriendsList[i]["pic"].asString();
			mVirtualUserList.push_back(friendsVec[i].userId);
		}
		string platform,id,name,pic;
		if (History::getHistory()->getLastUserPlatformAndID(platform,id,&name,&pic)){
			TaskUser me;
			me.userId = id;
			me.name = name;
			me.pictrue = pic;
			friendsVec.push_back(me);
		}
		//设置数据
		this->setFriendsAndMeInfor(friendsVec);
	}

	//2. sections
	{
		int maxSeason = 1;
		vector<TaskInfor> allSections(jsonSectionList.size());
		for (int i=0,count=jsonSectionList.size(); i<count; ++i)
		{
			allSections[i].seasonId = jsonSectionList[i]["seasonId"].asInt();
			allSections[i].sectionId = jsonSectionList[i]["sectionId"].asInt();
			allSections[i].maxScore = 0;
			allSections[i].avgScore = 0;
			allSections[i].playerCount = 0;
			if (allSections[i].seasonId > maxSeason) maxSeason = allSections[i].seasonId;//找到最大的seasonId
			const Json::Value& friendsInfo = jsonSectionList[i]["friendsInfo"];
			//all friends
			for (size_t friendIdx=0; friendIdx<friendsInfo.size(); ++friendIdx)
			{
				TaskSolution temp;
				temp.seasonId = allSections[i].seasonId;
				temp.sectionId = allSections[i].sectionId;
				temp.userId = friendsInfo[friendIdx]["ID"].asString();
				temp.star = friendsInfo[friendIdx]["star"].asInt();
				temp.score = friendsInfo[friendIdx]["score"].asInt();
				temp.upTimes = friendsInfo[friendIdx]["praise"].asInt();
				temp.downloadTimes = friendsInfo[friendIdx]["download"].asInt();
				temp.solutionId = CCString::createWithFormat(OFFICIAL_SOLUTION_ID_FORMAT, temp.userId.c_str(), allSections[i].seasonId, allSections[i].sectionId)->getCString();
				temp.solutionData = CCFileUtils::sharedFileUtils()->fullPathForFilename(friendsInfo[friendIdx]["solution"].asString().c_str());
				allSections[i].users.push_back(temp);
			}
			//self ?
		}

		//设置数据
		vector<vector<TaskInfor> > allTask(maxSeason+1);
		for (int i=0,count=allSections.size(); i<count; ++i)
		{
			allTask[allSections[i].seasonId].push_back(allSections[i]);
		}
		for (int i=1; i<=maxSeason; ++i)
		{
			this->setSeasonInfor(i, allTask[i]);
		}
	}
	return true;
}
bool SocialInfoManager::isOfficialOrVirtualSolutionId(string solutionId)
{
	string temp = OFFICIAL_SOLUTION_ID_FORMAT;
	temp = temp.substr(0, temp.find_first_of("_"));
	return (solutionId.find(temp) == 0);
}

void SocialInfoManager::resetLocalData()
{
	m_bHasInitFriendsAndMe = false;
	mUserList.clear();
	mVirtualUserList.clear();
	mAllTask.clear();
	initLocalData();
}

	//获取一个关卡的完整信息
const TaskInfor* SocialInfoManager::getSectionInfor(int seasonId, int sectionId)
{
	for (size_t i=0; i<mAllTask.size(); ++i)
	{
		if (mAllTask[i].first == seasonId){
			for (size_t j=0; j<mAllTask[i].second.size(); ++j)
			{
				if (mAllTask[i].second[j].seasonId == seasonId && mAllTask[i].second[j].sectionId == sectionId)
					return &mAllTask[i].second[j];
			}
		}
	}
	return nullptr;
}

	//获取一个关卡的 全服最高分
int SocialInfoManager::getSectionMaxScore(int seasonId, int sectionId)
{
	const TaskInfor* pTask = this->getSectionInfor(seasonId, sectionId);
	if (pTask){
		return pTask->maxScore;
	}else {
		return 0;
	}
}
	//获取一个关卡的全服平局分
int SocialInfoManager::getSectionAvgScore(int seasonId, int sectionId)
{
	const TaskInfor* pTask = this->getSectionInfor(seasonId, sectionId);
	if (pTask){
		return pTask->avgScore;
	}else {
		return 0;
	}
}




	// 设置用户 和朋友的信息
void SocialInfoManager::setFriendsAndMeInfor(const vector<TaskUser>& vec)
{
	vector<TaskUser> temp;
	for (int i=0,count=mUserList.size(); i<count; ++i)
	{
		if (this->isOfficalUserID(mUserList[i].userId)){
			temp.push_back(mUserList[i]);
		}
	}
	mUserList = vec;
	mUserList.insert(mUserList.end(), temp.begin(), temp.end());
	m_bHasInitFriendsAndMe = true;
}
vector<TaskUser> SocialInfoManager::getFriendsAndMe()
{
	assert(m_bHasInitFriendsAndMe);
	return mUserList;
}
vector<TaskUser> SocialInfoManager::getFriendsAndMe_WithoutVirtualUser() //除去本地配置的官方好友，主要用于cmdhelper请求服务器数据时使用
{
	assert(m_bHasInitFriendsAndMe);
	vector<TaskUser> temp = mUserList;
	for (int i=0,count=mVirtualUserList.size(); i<count; ++i)
	{
		for (vector<TaskUser>::iterator it=temp.begin(); it!=temp.end(); ++it)
		{
			if (it->userId == mVirtualUserList[i]){
				temp.erase(it);
				break;
			}
		}
	}
	return temp;
}
vector<TaskUser> SocialInfoManager::getFriendsExceptMe()
{
	assert(m_bHasInitFriendsAndMe);
	string plat,id;
	History::getHistory()->getLastUserPlatformAndID(plat,id);
	vector<TaskUser> temp;
	for (int i=0,count=mUserList.size(); i<count; ++i)
	{
		if (mUserList[i].userId != id)
			temp.push_back(mUserList[i]);
	}
	return temp;
}
const TaskUser* SocialInfoManager::getUserInfor(string userId)
{
	assert(m_bHasInitFriendsAndMe);
	for (int i=0,count=mUserList.size(); i<count; ++i)
	{
		if (mUserList[i].userId == userId)
			return &mUserList[i];
	}
	return nullptr;
}
/*获取进展到当前season的friend
* @param  seasonId 需要查询的season
* return 进展到当前season的好友列表，  key:userID  value:sectionID
*/
map<string,unsigned int> SocialInfoManager::getFriendProgressBySeason(unsigned int seasonId)
{
	map<string,unsigned int> ret;

	int sectionNum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId);
	// 1. 把所有玩过当前season的好友都找到 
	for (int sectionId=1; sectionId<=sectionNum; ++sectionId)
	{
		const TaskInfor* pInfor = this->getSectionInfor(seasonId, sectionId);
		if (pInfor && pInfor->users.size() > 0)
		{
			for (int i=0,count=pInfor->users.size(); i<count; ++i)
			{
				map<string,unsigned int>::iterator it = ret.find(pInfor->users[i].userId);
				if (it == ret.end()){//section 1
					ret.insert(map<string,unsigned int>::value_type(pInfor->users[i].userId, sectionId));
				}else{
					it->second = sectionId;
				}
			}
		}
	}
	// 2. 除去打到下一个season的好友，那么剩下的好友就是进度在当前season
	if (GameLayerFactory::getInstance()->getConfigSeasonNum() <= seasonId){
		return ret; //已是最后一章
	}
	const TaskInfor* pNextSectionInfo = this->getSectionInfor(seasonId+1, 1);//下一章第一关的taskinfor
	if (pNextSectionInfo != nullptr){
		for (int i=0,count=pNextSectionInfo->users.size(); i<count; ++i)
		{
			map<string,unsigned int>::iterator it = ret.find(pNextSectionInfo->users[i].userId);
			if (it != ret.end()){
				ret.erase(it);
			}
		}
	}

	//sort
	for (map<string,unsigned int>::iterator it1=ret.begin(); it1!=ret.end(); ++it1)
	{
		for (map<string,unsigned int>::iterator it2=it1; it2!=ret.end(); ++it2)
		{
			if (it2->second > it1->second)
			{
				swap(it1, it2);
			}
		}
	}

	return ret;
}
vector<string> SocialInfoManager::getFriendProgressBySection(unsigned int seasonId, unsigned int sectionId) //获取进度在当前section的好友
{
	vector<string> ret;
	// 1. 把所有玩过当前section的好友都找到 
	const TaskInfor* pInfor = this->getSectionInfor(seasonId, sectionId);
	if (pInfor==nullptr || pInfor->users.empty()){
		return ret;
	}
	for (int i=0,count=pInfor->users.size(); i<count; ++i)
	{
		ret.push_back(pInfor->users[i].userId);
	}
	// 2. 除去打到下一个section的好友，那么剩下的好友就是进度在当前section (可能没打挑战关，直接进入下一个season 须第三步筛选)
	int maxSectionIdx = GameLayerFactory::getInstance()->getTotalSectionNum()-1;
	int nextSectionIdx = GameLayerFactory::getInstance()->getSectionIndex(seasonId, sectionId) + 1;
	if (nextSectionIdx > maxSectionIdx){ 
		return ret;//当前关卡已是最后一关，那么打过这关的进度都在这里
	}else {
		std::pair<int, int> nextSection = GameLayerFactory::getInstance()->getLevelFormat(nextSectionIdx);
		const TaskInfor* pNextSectionInfo = this->getSectionInfor(nextSection.first, nextSection.second);
		if (pNextSectionInfo!=nullptr){
			for (int i=0,count=pNextSectionInfo->users.size(); i<count; ++i)
			{
				for (vector<string>::iterator it=ret.begin(); it!=ret.end(); ++it)
				{
					if (*it == pNextSectionInfo->users[i].userId){
						ret.erase(it);
						break;
					}
				}
			}
		}
	}
	// 3. 除去跳过挑战关卡，直接打下一season的好友
	if (GameLayerFactory::getInstance()->getConfigSeasonNum() <= seasonId){
		return ret; //已是最后一章
	}
	const TaskInfor* pNextSeason1 = this->getSectionInfor(seasonId+1, 1);//下一章第一关的taskinfor
	if (pNextSeason1 != nullptr){
		for (int i=0,count=pNextSeason1->users.size(); i<count; ++i)
		{
			for (vector<string>::iterator it=ret.begin(); it!=ret.end(); ++it)
			{
				if (*it == pNextSeason1->users[i].userId){
					ret.erase(it);
					break;
				}
			}
		}
	}
	return ret;
}

//找到该关卡的信息
TaskSolution* SocialInfoManager::getSolutionInfor(string solutionId)
{
	if (solutionId.empty()) return nullptr;

	for (int i=0,iCount=mAllTask.size(); i<iCount; ++i)
	{
		for (int j=0,jCount=mAllTask[i].second.size(); j<jCount; ++j)
		{
			vector<TaskSolution> &vec = mAllTask[i].second.at(j).users;
			for (int sIdx=0,sCount=vec.size(); sIdx<sCount; ++sIdx)
			{
				if (vec[sIdx].solutionId == solutionId){
					return &vec[sIdx];
				}
			}
		}
	}
	CCLog("%s ---> not found %s", __FUNCTION__, solutionId.c_str());
	return nullptr;
}
const TaskSolution* SocialInfoManager::getSolutionInfor(int seasonId, int sectionId, string userId)
{
	for (int i=0,iCount=mAllTask.size(); i<iCount; ++i)
	{
		for (int j=0,jCount=mAllTask[i].second.size(); j<jCount; ++j)
		{
			vector<TaskSolution> &vec = mAllTask[i].second.at(j).users;
			for (int sIdx=0,sCount=vec.size(); sIdx<sCount; ++sIdx)
			{
				if (vec[sIdx].seasonId == seasonId && vec[sIdx].sectionId == sectionId && vec[sIdx].userId == userId){
					return &vec[sIdx];
				}
			}
		}
	}
	return nullptr;
}
	//设置关卡解法信息
void SocialInfoManager::setSolutionData(string solutionId, const string& data)
{
	TaskSolution* pSolut = getSolutionInfor(solutionId);
	if (pSolut) {
		pSolut->solutionData = data;
	}else {
		assert(false);
	}
}
string SocialInfoManager::getSolutionData(int seasonId, int sectionId, string userId)
{
	const TaskSolution* pSolut = this->getSolutionInfor(seasonId, sectionId, userId);
	if (pSolut && !pSolut->solutionData.empty())
	{
		return pSolut->solutionData;
	}else
	{
		//如果是空  看看是不是 self
		string plat, id;
		if (History::getHistory()->getLastUserPlatformAndID(plat,id) && !id.empty() && id == userId)
		{
			return Recorder::getInstance()->getSectionUserRecordAbsoluteFileName(seasonId, sectionId, plat, id);
		}
	}
	return "";
}
string SocialInfoManager::getSolutionData(string solutionId)//本地没有数据时 string为空
{
	if (solutionId.empty()) return "";

	const TaskSolution* pSolut = getSolutionInfor(solutionId);
	if (pSolut) {
		return pSolut->solutionData;
	}else {
		CCLog("%s ---> not found %s", __FUNCTION__, solutionId.c_str());
		return "";
	}
}

int sortFriendsByScore(const void* a, const void* b)
{
	//降序
	return ( ((TaskSolution*)b)->score ) - ((TaskSolution*)a)->score;
}
	//设置一个season的关卡信息（好友排行列表、全服平均分）
void SocialInfoManager::setSeasonInfor(int seasonId, const vector<TaskInfor>& list) //结构中存在指针，所以接口内部会拷贝一份后重新设置指针地址
{
	vector<TaskInfor> * pInfoList = nullptr;
	for (int i=0,count=mAllTask.size(); i<count; ++i)
	{
		if (mAllTask[i].first == seasonId){
			mAllTask[i].second = list;
			pInfoList = &mAllTask[i].second;
			break;
		}
	}
	if (! pInfoList){
		mAllTask.push_back(std::pair<int,vector<TaskInfor> >(seasonId,list));
		pInfoList = &mAllTask[mAllTask.size()-1].second;
	}
	//sort
	for (int i=0,count=pInfoList->size(); i<count; ++i)
	{
		qsort(&pInfoList->at(i).users[0], pInfoList->at(i).users.size(), sizeof(TaskSolution), &sortFriendsByScore);
	}
}

void SocialInfoManager::setSectionSocialInfor(const vector<TaskInfor>& list)
{
	for (int idxInfo=0,count=list.size(); idxInfo<count; ++idxInfo)
	{
		//找到对应的 seasonId
		bool bNotFoundSeason = true;
		for (int idxSeason=0; idxSeason<mAllTask.size(); ++idxSeason)
		{
			if (mAllTask[idxSeason].first == list[idxInfo].seasonId) 
			{
				//找到对应的sectionId
				bool bNotHasSection = true;
				for (int idxSection=0; idxSection<mAllTask[idxSeason].second.size(); ++idxSection)
				{
					if (mAllTask[idxSeason].second.at(idxSection).sectionId == list[idxInfo].sectionId)
					{
						//如果有这个section数据  那么可能存在配置的官方账号，所以不能覆盖
						//mAllTask[idxSeason].second.at(idxSection) = list[idxInfo];
						vector<TaskSolution> tempOldUers;
						for (vector<TaskSolution>::iterator it=mAllTask[idxSeason].second.at(idxSection).users.begin(); it!=mAllTask[idxSeason].second.at(idxSection).users.end(); ++it)
						{
							if (this->isOfficalUserID(it->userId)){
								tempOldUers.push_back(*it);
							}
						}
						mAllTask[idxSeason].second.at(idxSection) = list[idxInfo];
						mAllTask[idxSeason].second.at(idxSection).users.insert(mAllTask[idxSeason].second.at(idxSection).users.end(), tempOldUers.begin(), tempOldUers.end());
						bNotHasSection = false;
						break;
					}
				}
				if (bNotHasSection)
				{
					mAllTask[idxSeason].second.push_back(list[idxInfo]);
				}

				bNotFoundSeason = false;
				break;
			}
		}
		if (bNotFoundSeason) //没有这个season的数据
		{
			vector<TaskInfor> tempVec;
			tempVec.push_back(list[idxInfo]);
			mAllTask.push_back(std::make_pair(list[idxInfo].seasonId, tempVec));
		}
	}
	//sort
	for (int season=0; season<mAllTask.size(); ++season)
	{
		for (int section=0; section<mAllTask[season].second.size(); ++section)
		{
			qsort(&mAllTask[season].second.at(section).users[0], mAllTask[season].second.at(section).users.size(), sizeof(TaskSolution), &sortFriendsByScore);
		}
	}
}
void SocialInfoManager::setSelfSectionScore(unsigned int seasonId, unsigned int sectionId, int score, int star)
{
	string userId;
	if (! CmdHelper::getInstance()->getCurrentUserID(userId) || userId.empty()){
		return ;
	}
	//找到对应的 seasonId
	bool bNotFoundSeason = true;
	for (int idxSeason=0; idxSeason<mAllTask.size(); ++idxSeason)
	{
		if (mAllTask[idxSeason].first == seasonId) 
		{
			//找到对应的sectionId
			bool bNotHasSection = true;
			for (int idxSection=0; idxSection<mAllTask[idxSeason].second.size(); ++idxSection)
			{
				if (mAllTask[idxSeason].second.at(idxSection).sectionId == sectionId)
				{
					//找到userid
					bool bNotFindSelf = true;
					for (vector<TaskSolution>::iterator it=mAllTask[idxSeason].second.at(idxSection).users.begin(); it!=mAllTask[idxSeason].second.at(idxSection).users.end(); ++it)
					{
						if (it->userId == userId)
						{
							it->score = score;
							it->star = star;
							//it->solutionId = ;
							bNotFindSelf = false;
							qsort(&mAllTask[idxSeason].second.at(idxSection).users[0], mAllTask[idxSeason].second.at(idxSection).users.size(), sizeof(TaskSolution), &sortFriendsByScore);
							return;
						}
					}
					if (bNotFindSelf)
					{
						TaskSolution tempSelf;
						tempSelf.userId = userId;
						tempSelf.seasonId = seasonId;
						tempSelf.sectionId = sectionId;
						//tempSelf.solutionId = ;
						tempSelf.score = score;
						tempSelf.star = star;
						mAllTask[idxSeason].second.at(idxSection).users.push_back(tempSelf);
						qsort(&mAllTask[idxSeason].second.at(idxSection).users[0], mAllTask[idxSeason].second.at(idxSection).users.size(), sizeof(TaskSolution), &sortFriendsByScore);
						return;
					}
					bNotHasSection = false;
					break;
				}
			}
			if (bNotHasSection)
			{
				TaskInfor tempTask;
				tempTask.seasonId = seasonId;
				tempTask.sectionId = sectionId;
				TaskSolution tempSelf;
				tempSelf.userId = userId;
				tempSelf.seasonId = seasonId;
				tempSelf.sectionId = sectionId;
				//tempSelf.solutionId = ;
				tempSelf.score = score;
				tempSelf.star = star;
				tempTask.users.push_back(tempSelf);
				mAllTask[idxSeason].second.push_back(tempTask);
				return;
			}
			bNotFoundSeason = false;
			break;
		}
	}
	if (bNotFoundSeason) //没有这个season的数据
	{
		vector<TaskInfor> tempVec;
		TaskInfor tempTask;
		tempTask.seasonId = seasonId;
		tempTask.seasonId = sectionId;
		TaskSolution tempSelf;
		tempSelf.userId = userId;
		tempSelf.seasonId = seasonId;
		tempSelf.sectionId = sectionId;
		//tempSelf.solutionId = ;
		tempSelf.score = score;
		tempSelf.star = star;
		tempTask.users.push_back(tempSelf);
		tempVec.push_back(tempTask);
		mAllTask.push_back(std::make_pair(seasonId, tempVec));
	}
}
bool SocialInfoManager::isOfficalUserID(string userId) //判断是否官方账号的id
{
	for (int i=0,count=mVirtualUserList.size(); i<count; ++i)
	{
		if (mVirtualUserList[i] == userId) return true;
	}
	return false;
}



void SocialInfoManager::initLocalTemplateData()
{
#if _LOCAL_SOCIAL_TEMPLATE_DATA_ == 1
	const int _friends_num_ = 10;
	const int _one_season_task_num_ = 20;
	int _task_id_begin_ = 10000000;
	//1. user info
	vector<TaskUser> taskUsers(_friends_num_);
	for (int i=0; i<_friends_num_; ++i)
	{
		taskUsers[i].userId = CCString::createWithFormat("%d", atoi(_VIRTUE_USER_ID_) + i)->getCString();//i=0就是自己
		taskUsers[i].name   = taskUsers[i].userId;
		taskUsers[i].pictrue= _VIRTUE_USER_PICTURE_;
	}
	this->setFriendsAndMeInfor(taskUsers);

	//2. task info
//	unsigned long tempSize;
	for (int seasonId=1; seasonId<=5; ++seasonId)
	{
		vector<TaskInfor> oneSeasonTaskList(_one_season_task_num_);
		//1). taks info
		for (int iTask=0; iTask<_one_season_task_num_; ++iTask)
		{
			int sectionId = iTask + 1;
			oneSeasonTaskList[iTask].seasonId  = seasonId;
			oneSeasonTaskList[iTask].sectionId = sectionId;
			oneSeasonTaskList[iTask].maxScore = 1000-iTask*27;
			oneSeasonTaskList[iTask].avgScore = 800 - iTask*4;
			oneSeasonTaskList[iTask].playerCount = 100000000;
			//2). friends solution info
			for (int jFriends=0; jFriends<_friends_num_; ++jFriends)
			{
				TaskSolution temp;
				temp.userId = taskUsers[jFriends].userId;
				temp.seasonId  = seasonId;
				temp.sectionId = sectionId;
				temp.star = 3;
				temp.score = 1000-jFriends*27;
				temp.solutionId = CCString::createWithFormat("%d", ++_task_id_begin_)->getCString();
				temp.downloadTimes = rand() % 10000;
				temp.upTimes = rand() % 100000;
				//3). solution data
				temp.solutionData = Recorder::getInstance()->getSectionOfficialRecordAbsoluteFileName(seasonId,sectionId);
// 				char *pData = (char*)CCFileUtils::sharedFileUtils()->getFileData(Recorder::getInstance()->getSectionOfficialRecordAbsoluteFileName(seasonId,sectionId).c_str(), "rb", &tempSize);
// 				if (pData && tempSize > 0){
// 					temp.solutionData.append(pData, tempSize);
// 					CCAssert(temp.solutionData.length() == tempSize, "");
// 					delete[] pData;
// 				}
				oneSeasonTaskList[iTask].users.push_back(temp);
			}
		}
		this->setSeasonInfor(seasonId, oneSeasonTaskList);
	}

#endif
}