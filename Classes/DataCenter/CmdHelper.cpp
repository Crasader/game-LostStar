
#include "CmdHelper.h"
#include "FXPlugin/FXPluginManager.h"
#include "History.h"
#include "AchievementManager.h"
#include "GameInforEditer.h"
#include "UserDataManager.h"
#include "net/NetworkThreadProc.h"
#include "net/NetworkWaitingLayer.h"
#include "protomsg.pb.h"
#include "protocol.h"
#include "Recorder.h"
#include "GameLayerFactory.h"
#include "LevelManager.h"
#include "support/zip_support/ZipUtils.h"
#include "LFStrings.h"
#include "MessageBar.h"
#include "VersionControl.h"


#define CHECK_PARSE_RSP_DATA(b) if(!(b)){\
	CCAssert(false,"parse rsp data error");\
	return;\
}

//与服务器通信时 解法数据 压缩与否开关
#define COMPRESS_SOLUTIONG_ENABLE 1
//从服务器更新本地社交好友进度 间隔时间 s  (如果是 0 那么就使用preload)
#define REFRESH_SOCIAL_INFOR_TIME 1800
#define GET_SOCIAL_INFOR_TIME 120

#define DEUBG_DISPLAY_INFORMATION 0
#define DEBUG_ALWAYS_BUY_SUCCESS 0


CmdHelper* CmdHelper::_instanceCmdHelper = nullptr;
static int s_syncSocialDataStep = 0; // 登录社交平台都 要经过2个步骤同步数据后，玩家才能操作

CmdHelper::CmdHelper()
	: m_lastNetworkError(NetSuccess)
	, m_bAlreadyGetFriendsInfor(false)
	, m_bShowWaiting(true)
{
}

CmdHelper* CmdHelper::getInstance()
{
	if (!_instanceCmdHelper) {
		_instanceCmdHelper = new CmdHelper();
		_instanceCmdHelper->init();
	}
	return _instanceCmdHelper;
}
bool CmdHelper::init()
{
	NetworkPackageDispatch::getInstance()->setMsgProc(this);

	CCNode::onEnter();
	CCNode::onEnterTransitionDidFinish();
#if (REFRESH_SOCIAL_INFOR_TIME > 0)
	this->schedule(schedule_selector(CmdHelper::updateSocialInfo), /*REFRESH_SOCIAL_INFOR_TIME*/ GET_SOCIAL_INFOR_TIME);
#endif
	this->schedule(schedule_selector(CmdHelper::updateSdkResultDispatch), 0.5f);
	return true;
}

void CmdHelper::initSyncServerData()
{
	static bool s_bHasInit = false;
	if (s_bHasInit) return;
	s_bHasInit = true;

	if (this->isLogin())
	{
		//缓存登录 不用玩家等待、在后台执行
		//NetworkWaitingLayer::showWaiting();
		m_bShowWaiting = false;
		this->onLoginResult(FX::LoginResultCode::eSuccess, "");
		// 在onLoginResult:eSuccess 会处理登录成功的账号设置，所以这里不需要了。
	}
	else
	{
		this->getAnalyticsInstance()->setAccount(this->getAnalyticsInstance()->getDeviceId());
		this->getAnalyticsInstance()->setAccountType(FX::PluginAnalytics::eAccountAnonymous);
	}
}

//上传数据 同步该账户数据
void CmdHelper::SyncCurUserData(bool showWaiting)
{
	if (! hasAccount()) return;

	m_bShowWaiting = showWaiting;
	string platform,userId;
	bool bHas = History::getHistory()->getLastUserPlatformAndID(platform, userId);

	CCLog("info: %s ---> ", __FUNCTION__);
	::Star::SyncReq req;
	// 1. required AccountInfo acc        = 1;    //账户
	::Star::AccountInfo* pAcc = req.mutable_acc();
	pAcc->set_platform(::Star::PlatformType_Facebook);
	pAcc->set_account(userId);
	// 2. repeated TaskInfo task          = 2;    //关卡信息
	for (int i=1; ; ++i)
	{
		SeasonInfor * pSeason = GameInforEditer::getGameInfor()->getSeasonInfor(i);
		if (! pSeason) break;
		list<SectionInfor*> sections = pSeason->getAllSectionInfors();
		for (list<SectionInfor*>::const_iterator it=sections.begin(); it != sections.end(); ++it)
		{
			if ((*it)->isSuccessed)
			{
				::Star::TaskInfo* sectionInfo = req.add_task();
				long long task_id = (*it)->seasonId;
				task_id <<= 16;
				task_id |= (*it)->sectionId;
				sectionInfo->set_task_id(task_id);
				sectionInfo->set_star((*it)->iStarNum);
				sectionInfo->set_score((*it)->iRankingScore);
			}
		}
		CCLog("  --- season[%d]", i);
	}

	string data = req.SerializeAsString();
	NetworkThreadProc::getInstance()->sendData(Req_Sync, data.c_str(), data.length());
	if (m_bShowWaiting) NetworkWaitingLayer::showWaiting();

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("sync", "info");
#endif
}
void CmdHelper::onSyncCurUserDataRsp(const void* data, unsigned int dataLength)
{
	if (--s_syncSocialDataStep <= 0) 
		NetworkWaitingLayer::hideWaiting();
	::Star::SyncRsp rsp;
	int rsp_code = MessageErrorCode::Error;

	if (data){
		CHECK_PARSE_RSP_DATA ( rsp.ParseFromArray(data, dataLength) );
		rsp_code = rsp.rsp_code();
	}
	else {
		rsp_code = MessageErrorCode::Error;
	}

	CCLog("%s ---> Rsp_Sync rsp_code = %d", __FUNCTION__, rsp_code);
	MsgData msg_data;
	if (rsp_code != MessageErrorCode::Success){
		displayErrorMessageBox(this->getLastNetworkError());
		msg_data.errCode = MsgData::eError;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_SyncUserDataEnd, &msg_data);
		return;
	}
	string platform,userId;
	CCAssert(History::getHistory()->getLastUserPlatformAndID(platform, userId), "玩家登录过才能和游戏服务器同步数据");
	//获取数据
	CCLOG("rsp need_up_task:");
	// 1. repeated TaskInfo need_up_task  = 2;    //需要客户端上传解法的关卡信息
	vector<std::pair<int,int> > need_up_solution;
	int upNum = rsp.need_up_task_size();
	for (int i=0; i<upNum; ++i)
	{
		long long task_id = rsp.need_up_task(i).task_id();
		int seasonId  = (0x00000000ffff0000LL & task_id) >> 16;
		int sectionId = (0x000000000000ffffLL & task_id);
		need_up_solution.push_back(std::make_pair(seasonId, sectionId));
		CCLOG("  ---%d: %d-%d", 1, seasonId, sectionId);
	}
	// 2. repeated SolutionInfo down_task = 3;    //需要客户端更新的关卡
	CCLOG("rsp down SolutionInfo:");
	int downNum = rsp.down_task_size();
	for (int i=0; i<downNum; ++i)
	{
		// 需要用的数据有 1)分值 2)解法内容(解法内容去掉)
		const ::Star::SolutionInfo& solutInfo = rsp.down_task(i);
		long long task_id = solutInfo.task_id();
		int seasonId  = (0x00000000ffff0000LL & task_id) >> 16;
		int sectionId = (0x000000000000ffffLL & task_id);
		int star = solutInfo.star();
		int score = solutInfo.score();
/*		const string& solutData = solutInfo.data();
		//解压
		char* data = const_cast<char*>(solutData.c_str());
		unsigned int dataLength = solutData.length();
#if COMPRESS_SOLUTIONG_ENABLE == 1
		unsigned char* decompressData = NULL;
		int decompressDataLength = ZipUtils::ccInflateMemory((unsigned char*)solutData.c_str(), solutData.length(), &decompressData);
		if (decompressDataLength <= 0 || decompressData == NULL){
			CC_SAFE_DELETE_ARRAY(decompressData);
			CCLog("Error: %s ---> decompress solution data failed, rsp size = %d", __FUNCTION__, solutData.length());
			CCMessageBox("uncompress solution data error!", "error");
			continue;
		}
		data = (char*)decompressData;
		dataLength = decompressDataLength;
#endif
		//save
		// 2) solution
		string filePath = Recorder::getInstance()->getSectionUserRecordAbsoluteFileName(seasonId, sectionId, platform, userId);
		FILE *pf = fopen(filePath.c_str(), "wb");
		if (pf == NULL) {
			CCLog("Error: %s ---> open file error : %s", __FUNCTION__, filePath.c_str());
			continue;
		}
		int count = fwrite(data, sizeof(char), dataLength, pf);
		if (count != dataLength){
			CCLog("Error: %s ---> fwirte solution to file error, data length[%d], had write count[%d]", __FUNCTION__, dataLength, count);
			fclose(pf);
			remove(filePath.c_str()); //删除文件
			continue;
		}else {
			fclose(pf);
		}
#if COMPRESS_SOLUTIONG_ENABLE == 1
		delete[] data;
#endif
		*/
		// 1) score  (解法保存成功之后才能保存分数) --- 只是更新了内存数据，还未保存在本地infor文件中,全部更新完了一次保存
		SectionInfor* pSection = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
		//如果本地没有数据  就创建
		if (!pSection){
			SeasonInfor* pSeason = GameInforEditer::getGameInfor()->getSeasonInfor(seasonId);
			if (!pSeason){
				pSeason = new SeasonInfor();
				pSeason->seasonId = seasonId;
				pSeason->score = 0;
				pSeason->avgScore = 0;
				pSeason->minScore = 0;
				pSeason->seasonType = type_Normal;
				GameInforEditer::getGameInfor()->addSeasonInfor(pSeason);
			}
			pSeason->isPlayed = true;
			pSeason->locked = false;
			pSection = new SectionInfor();
			pSection->seasonId = seasonId;
			pSection->sectionId = sectionId;
			pSection->isPlayed = true;
			pSection->locked = false;
			pSection->iStarNum = star;
			pSection->iRankingScore = score;
			pSection->avgScore = 0;
			pSection->minScore = 0;
			pSection->isSuccessed = true;
			pSection->sectionType = type_Normal;
			//add
			pSeason->addSectionInfor(pSection);
			pSeason->score += pSection->iStarNum;
		} //可能在玩的过程中同步成功了，这时完成了一关分数比服务器的好，那么就不能用服务器的覆盖
		else if (pSection->isBetterThanMe(pSection->iRankingScore)){
			pSection->iStarNum = star;
			pSection->iRankingScore = score;
			pSection->isSuccessed = true;
			pSection->locked = false;
			pSection->isPlayed = true;
		}

		if (pSection->iStarNum > score){
			CCLog("Error: %s ---> server sync down data less than client", __FUNCTION__);
		}
		CCLOG("  ---%d-%d: star[%d], score[%d]", seasonId, sectionId, star, score);
	}
	GameInforEditer::getGameInfor()->correctData(); //同步后 可能出现已解锁的关卡丢失
	GameInforEditer::getGameInfor()->saveSessonInforList();//保存score
	GameInforEditer::getGameInfor()->initLastClickVec();//重新初始化最后点击的关卡
	LevelManager::getManager()->reset();
	LevelManager::getManager()->setLastSeasonId(GameInforEditer::getGameInfor()->getLastPlayedSeasonId());
	LevelManager::getManager()->setLastSectionId(GameInforEditer::getGameInfor()->getLastPlayedSectionId());
	LevelManager::getManager()->setNeedUnLockSeason(GameInforEditer::getGameInfor()->getLastUnlockedSeasonId());

	//3. 保存完数据后，上传server需要的解法信息
	this->uploadTaskSolution(need_up_solution);

	//让客户端知道
	msg_data.errCode = MsgData::eSuccess;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_SyncUserDataEnd, &msg_data);

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox(CCString::createWithFormat("rsp sync: rsp down from server, downSize[%d], will upSize[%d]", downNum, upNum)->getCString(), "info");
#endif
}

//上传解法信息  (不用再该函数中涉及UI操作，有子线程会异步调用该接口)
void CmdHelper::uploadTaskSolution(vector<std::pair<int,int> > sectionList)
{
	if (! hasAccount()) return;

	if (sectionList.empty()) {
		CCLog("%s ---> section list is empty", __FUNCTION__);
		return;
	}

	string platform,userId;
	bool bHas = History::getHistory()->getLastUserPlatformAndID(platform, userId);

	CCLog("info: %s ---> ", __FUNCTION__);
	::Star::UploadSolutionReq req;
	// 1. required AccountInfo acc        = 1;    //账户
	::Star::AccountInfo* pAcc = req.mutable_acc();
	pAcc->set_platform(::Star::PlatformType_Facebook);
	pAcc->set_account(userId);
	// 2. repeated SolutionInfo solution  = 2;    //解法信息
	int up_num = 0;
	unsigned long dataLength;
	for (int i=0,count=sectionList.size(); i<count; ++i)
	{
		int seasonId = sectionList[i].first;
		int sectionId = sectionList[i].second;

		::Star::SolutionInfo* pSolution = req.add_solution();
		++up_num;
		//1) task_id
		long long task_id = seasonId;
		task_id <<= 16;
		task_id |= sectionId;
		pSolution->set_task_id(task_id);
		//2) score
		SectionInfor* pSecInfo = GameInforEditer::getGameInfor()->getSectionInfor(seasonId, sectionId);
		if (pSecInfo) {
			pSolution->set_star(pSecInfo->iStarNum);
			pSolution->set_score(pSecInfo->iRankingScore);
		}else {
			pSolution->set_star(0);
			pSolution->set_score(0);
			CCAssert(false, "不应该没有！");
		}
		//3) solution file data
		string filePath = Recorder::getInstance()->getSectionUserRecordAbsoluteFileName(seasonId,sectionId,platform,userId);
		char* pRecordData = (char*)CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), "rb", &dataLength);
		if (pRecordData && dataLength > 0)
		{
			//压缩
#if COMPRESS_SOLUTIONG_ENABLE == 1
			unsigned char* compressData = NULL;
			unsigned int compressDataLength = ZipUtils::ccDeflatedMemory((unsigned char*)pRecordData, dataLength, &compressData);
			if (compressDataLength == 0 || compressData == NULL){
				CC_SAFE_DELETE_ARRAY(compressData);
				delete[] pRecordData;
				CCMessageBox("compress solution data error!", "error");
				continue; //压缩失败  不上传了。
			}
			CCLog("info: %s ---> %d-%d solution compress data size = %u", __FUNCTION__, seasonId, sectionId, compressDataLength);
			delete[] pRecordData;
			pRecordData = (char*)compressData;
			dataLength = compressDataLength;
#endif
			pSolution->set_data(pRecordData, dataLength);
			delete[] pRecordData;
		}
		CCLog("  --- %d-%d", seasonId, sectionId);
	}

	if (up_num == 0) {
		return;
	}
	string data = req.SerializeAsString();
	NetworkThreadProc::getInstance()->sendData(Req_UploadSolution, data.c_str(), data.length());

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("uploadTaskSolution", "info");
#endif
}
void CmdHelper::onUploadTaskSolutionRsp(const void* data, unsigned int dataLength)
{
	::Star::UploadSolutionRsp rsp;
	int rsp_code = MessageErrorCode::Error;

	if (data){
		CHECK_PARSE_RSP_DATA ( rsp.ParseFromArray(data, dataLength) );
		rsp_code = rsp.rsp_code();
	}
	else {
		CCLog("%s ---> time out!", __FUNCTION__);
		rsp_code = MessageErrorCode::Error;
	}

	CCLog("%s ---> Rsp_UploadSolution rsp_code = %d", __FUNCTION__, rsp_code);

	MsgData msg_data;
	if (rsp_code != MessageErrorCode::Success){
		msg_data.errCode = MsgData::eError;
	}else {
		msg_data.errCode = MsgData::eSuccess;
	}
	//注意：上传关卡信息 属于同步玩家数据的第二阶段，所以回应消息为MSG_SyncUserDataEnd
	// 第一阶段 也会向ui层post消息了
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_SyncUserDataEnd, &msg_data);

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox(CCString::createWithFormat("rsp upload solution %s", rsp_code==MessageErrorCode::Success?"success":"error")->getCString(), "info");
#endif
}

//下载解法信息
void CmdHelper::downloadTaskSolution(string solutionId, bool showWaiting)
{
	if (solutionId.empty()) {
		MsgData msg_data;
		msg_data.errCode = MsgData::eError;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_DownloadSolution, &msg_data);
		return;
	}

	TaskSolution *pInfor = SocialInfoManager::getInstance()->getSolutionInfor(solutionId);
	if (! pInfor) return;

	this->downloadTaskSolution(pInfor->seasonId, pInfor->sectionId, pInfor->userId, showWaiting);
}
void CmdHelper::downloadTaskSolution(int seasonId, int sectionId, string userId, bool showWaiting)
{
	if (! hasAccount()) return;

	m_bShowWaiting = showWaiting;
	CCLog("info: %s ---> seasion[%d], section[%d], userId[%s]", __FUNCTION__, seasonId, sectionId, userId.c_str());
	::Star::DownloadSolutionReq req;
	// 1. required AccountInfo acc        = 1;    //下载账户
	::Star::AccountInfo* pAcc = req.mutable_acc();
	pAcc->set_platform(::Star::PlatformType_Facebook);
	pAcc->set_account(userId);
	// 2. optional string solution_id     = 2;    //解法id
	string solutionId = SocialInfoManager::getInstance()->getSolutionInfor(seasonId, sectionId, userId)->solutionId;
//	s_download_solution_id.push(solutionId);
	req.set_solution_id(solutionId.c_str());

	string data = req.SerializeAsString();
	NetworkThreadProc::getInstance()->sendData(Req_DownloadSolution, data.c_str(), data.length());
	if (m_bShowWaiting )NetworkWaitingLayer::showWaiting();

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("downloadSolution", "info");
#endif
}
void CmdHelper::onDownloadTaskSolutionRsp(const void* data, unsigned int dataLength)		
{
	NetworkWaitingLayer::hideWaiting();

//	const string down_solution_id = s_download_solution_id.front();
//	s_download_solution_id.pop();

	::Star::DownloadSolutionRsp rsp;
	int rsp_code = MessageErrorCode::Error;
	if (data){
		CHECK_PARSE_RSP_DATA ( rsp.ParseFromArray(data, dataLength) );
		rsp_code = rsp.rsp_code();
	}
	else {
		CCLog("%s ---> time out!", __FUNCTION__);
		rsp_code = MessageErrorCode::Error;
	}
	CCLog("%s ---> Rsp_DownloadSolution rsp_code = %d", __FUNCTION__, rsp_code);

	MsgData msg_data;
	if (rsp_code != MessageErrorCode::Success){
		displayErrorMessageBox(this->getLastNetworkError());
		msg_data.errCode = MsgData::eError;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_DownloadSolution, &msg_data);
		return;
	}
	//获取数据
	CCLOG("rsp SolutionInfo:");
	string platform,userId;
	CCAssert(History::getHistory()->getLastUserPlatformAndID(platform, userId), "玩家登录过才能和游戏服务器同步数据");
	//1. optional SolutionInfo soulution = 2;    //解法信息
    const string down_solution_id = rsp.soulution().solution_id();
	CCLOG("  ---solutionId[%s]: data length[%d]", down_solution_id.c_str(), rsp.soulution().data().length());
	if (rsp.soulution().data().size() > 0){
	//解压
#if COMPRESS_SOLUTIONG_ENABLE == 1
		unsigned char* decompressData = NULL;
		int decompressDataLength = ZipUtils::ccInflateMemory((unsigned char*)rsp.soulution().data().c_str(), rsp.soulution().data().length(), &decompressData);
		if (decompressDataLength > 0 && decompressData != NULL){
			string tempData;
			tempData.append((char*)decompressData, decompressDataLength);
			TaskSolution* pSolutionInfo = SocialInfoManager::getInstance()->getSolutionInfor(down_solution_id);
			pSolutionInfo->solutionData = tempData;
			pSolutionInfo->downloadTimes += 1;//rsp.soulution().download_times();
			CCLOG(" ---decompress data length[%d], downloadTimes[%d]", decompressDataLength, rsp.soulution().download_times());
		}else{
			CCLOG("Error: %s ---> decompress solution data error! decompressDataLength[%d]", __FUNCTION__, decompressDataLength);
			CCMessageBox("uncompress solution data error!", "error");
		}
		CC_SAFE_DELETE_ARRAY(decompressData);
#else
		SocialInfoManager::getInstance()->setSolutionData(down_solution_id, rsp.soulution().data());
#endif
	}

	msg_data.errCode = MsgData::eSuccess;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_DownloadSolution, &msg_data);

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox(CCString::createWithFormat("rsp down solution %s", rsp_code==MessageErrorCode::Success?"success":"error")->getCString(), "info");
#endif
}

//点赞
void CmdHelper::praiseSolution(string solutionId, bool praise, bool showWaiting)
{
	//是否是官方关卡
	if (SocialInfoManager::getInstance()->isOfficialOrVirtualSolutionId(solutionId)){
		TaskSolution* pSolutInfo = SocialInfoManager::getInstance()->getSolutionInfor(solutionId);
		CCAssert(pSolutInfo != nullptr, "");
		pSolutInfo->upTimes += 1;
		MsgData msg_data;
		msg_data.errCode = MsgData::eSuccess;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_PraiseSolution, &msg_data);
		return;
	}

	if (! hasAccount()) return;

	m_bShowWaiting = showWaiting;
	string platform,userId;
	bool bHas = History::getHistory()->getLastUserPlatformAndID(platform, userId);

	CCLog("info: %s ---> solutionId[%s]", __FUNCTION__, solutionId.c_str());


	::Star::UpSolutionReq req;
	// 1. required AccountInfo acc        = 1;    //点赞账户
	::Star::AccountInfo* pAcc = req.mutable_acc();
	pAcc->set_platform(::Star::PlatformType_Facebook);
	pAcc->set_account(userId);
	// 2. optional string solution_id     = 2;    //解法id
//	s_praise_solution_id.push(solutionId);
	req.set_solution_id(solutionId.c_str());
	// 3. optional bool is_up             = 3;    //true:赞
	req.set_is_up(praise);

	string data = req.SerializeAsString();
	NetworkThreadProc::getInstance()->sendData(Req_UpSolution, data.c_str(), data.length());
	NetworkWaitingLayer::showWaiting();

	//analytics
	//对好友解法点赞 --- begin
	CmdHelper::getInstance()->getAnalyticsInstance()->onBegin("PraiseSolution");

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("praiseSolution", "info");
#endif
}
void CmdHelper::onPraiseSolutionRsp(const void* data, unsigned int dataLength)		
{
	NetworkWaitingLayer::hideWaiting();
//	const string praise_solution_id = s_praise_solution_id.front();
//	s_praise_solution_id.pop();

	::Star::UpSolutionRsp rsp;
	int rsp_code = MessageErrorCode::Error;
	if (data){
		CHECK_PARSE_RSP_DATA ( rsp.ParseFromArray(data, dataLength) );
		rsp_code = rsp.rsp_code();
	}
	else {
		CCLog("%s ---> time out!", __FUNCTION__);
		rsp_code = MessageErrorCode::Error;
	}
	CCLog("%s ---> Rsp_UpSolution rsp_code = %d", __FUNCTION__, rsp_code);

	MsgData msg_data;
	if (rsp_code != MessageErrorCode::Success){
		displayErrorMessageBox(this->getLastNetworkError());
		msg_data.errCode = MsgData::eError;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_PraiseSolution, &msg_data);
		//analytics
		//对好友解法点赞 --- end (error)
		CmdHelper::getInstance()->getAnalyticsInstance()->onFailed("PraiseSolution", "network error");
		return;
	}
	//获取数据
	CCLOG("rsp SolutionInfo:");
	//1. optional SolutionInfo soulution = 2;    //解法信息
	// not need, 客户端自动模拟点赞次数+1, (如果立刻刷新服务器的数据，此时别人点赞了，玩家会看到自己点赞刷新后 赞的次数增加了不只一次)
    const string praise_solution_id = rsp.soulution().solution_id();
    TaskSolution* pSolutInfo = SocialInfoManager::getInstance()->getSolutionInfor(praise_solution_id);
	CCAssert(pSolutInfo != nullptr, "");
	pSolutInfo->upTimes += 1;
//	CCLOG("  ---solutionId[%s], client praiseTimes[%d], server praiseTimes[%d]", praise_solution_id.c_str(), pSolutInfo->upTimes, rsp.soulution().up_times());

	msg_data.errCode = MsgData::eSuccess;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_PraiseSolution, &msg_data);
	//analytics
	//对好友解法点赞 --- end (success)
	CmdHelper::getInstance()->getAnalyticsInstance()->onCompleted("PraiseSolution");

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("rsp praise solution", "info");
#endif
}

//关卡全球数据，好友排名
void CmdHelper::preloadSectionSocialInfo(unsigned int seasonId, unsigned int currentPlaySectionId, unsigned int preloadNum/* = 5*/)
{
#if REFRESH_SOCIAL_INFOR_TIME == 0

	//当前关卡没有数据，或者下一关没有数据 就要开始预下载
	bool preload = false;
	vector<std::pair<unsigned int, unsigned int> > preloadVec;


	if (SocialInfoManager::getInstance()->getSectionInfor(seasonId, currentPlaySectionId) == nullptr){
		preload = true;
		preloadVec.push_back(std::make_pair(seasonId, currentPlaySectionId));
	}
	else {
		int nextSectionIdx = GameLayerFactory::getInstance()->getSectionIndex(seasonId, currentPlaySectionId) + 1;
		if (nextSectionIdx < GameLayerFactory::getInstance()->getTotalSectionNum())
		{
			std::pair<unsigned int, unsigned int> nextSection = GameLayerFactory::getInstance()->getLevelFormat(nextSectionIdx);
			if (SocialInfoManager::getInstance()->getSectionInfor(nextSection.first, nextSection.second) == nullptr){
				preload = true;
			}
		}
	}

	if (preload)
	{
		int totalIdx = GameLayerFactory::getInstance()->getTotalSectionNum() - 1;
		int nextIdx = GameLayerFactory::getInstance()->getSectionIndex(seasonId, currentPlaySectionId) + 1;  // 0-99 + 1
		for (int i=1; i<=preloadNum && nextIdx<=totalIdx; ++nextIdx,++i)
		{
			preloadVec.push_back(GameLayerFactory::getInstance()->getLevelFormat(nextIdx));// idx: 0-99
		}

		if (preloadVec.size() > 0)
		{
			this->getSocialInfo(preloadVec);
			NetworkWaitingLayer::hideWaiting();
		}
	}
#endif
}

void CmdHelper::updateSocialInfo(float dt)
{
	//正处于登录状态才去取 社交数据
	//logout之后 社交好友的数据会被清除，只保留本地配置的数据
	if (isLogin())
	{
		if (m_bAlreadyGetFriendsInfor)
		{
			this->unschedule(schedule_selector(CmdHelper::updateSocialInfo));
			this->unschedule(schedule_selector(CmdHelper::refreshSocialInfor));
			this->schedule(schedule_selector(CmdHelper::refreshSocialInfor), REFRESH_SOCIAL_INFOR_TIME);
		}
		else 
		{
			m_bShowWaiting = false;
			this->getCurUserAppFriends(std::bind(&CmdHelper::onGetFriendsResult, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
}
void CmdHelper::refreshSocialInfor(float dt)
{
	m_bShowWaiting = false;
	this->getCurUserAppFriends(std::bind(&CmdHelper::onGetFriendsResult, this, std::placeholders::_1, std::placeholders::_2));
}

void CmdHelper::getSeasonSocialInfo(unsigned int seasonId, bool showWaiting)
{
	if (! hasAccount()) return;

	m_bShowWaiting = showWaiting;
	string platform,userId;
	bool bHas = History::getHistory()->getLastUserPlatformAndID(platform, userId);

	CCLog("info: %s ---> seasonId[%d]", __FUNCTION__, seasonId);
	::Star::QueryFriendsInfoReq req;
	// 1. required PlatformType platform  = 1;    //关联平台类型
	req.set_platform(::Star::PlatformType_Facebook);
	// 2. repeated string account_list    = 2;    //账号列表
	vector<TaskUser> users = SocialInfoManager::getInstance()->getFriendsAndMe_WithoutVirtualUser();
	CCAssert(users.size()>0, "");
	for (int i=0,count=users.size(); i<count; ++i)
	{
		req.add_account_list(users[i].userId);
	}
	// 3. repeated int64 task_list        = 3;    //关卡列表
	const std::vector<ConfigSectionInfo>* sectionVec = GameLayerFactory::getInstance()->getConfigSectionInfoVec(seasonId);
	CCAssert(sectionVec->size()>0, "");
	for (int i=0,count=sectionVec->size(); i<count; ++i)
	{
		long long task_id = seasonId;
		task_id <<= 16;
		task_id |= sectionVec->at(i).iSectionId;
		req.add_task_list(task_id);
	}
	if (req.task_list().size() <= 0) return;

	string data = req.SerializeAsString();
	NetworkThreadProc::getInstance()->sendData(Req_QueryFriendsInfo, data.c_str(), data.length());

	if (m_bShowWaiting) NetworkWaitingLayer::showWaiting();

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("getSeasonSocialInfo", "info");
#endif
}
void CmdHelper::getSocialInfo(vector<std::pair<unsigned int,unsigned int> > sections, bool showWaiting)
{
	if (! hasAccount()) return;
	if (sections.empty()) return;
	vector<TaskUser> users = SocialInfoManager::getInstance()->getFriendsAndMe_WithoutVirtualUser();
	if (users.size() == 0)
	{
		CCLOG("%s ---> getFriendsAndMe_WithoutVirtualUser size = 0", __FUNCTION__);
		return;
	}

	m_bShowWaiting = showWaiting;
	string platform,userId;
	bool bHas = History::getHistory()->getLastUserPlatformAndID(platform, userId);

	CCLog("info: %s ", __FUNCTION__);
	::Star::QueryFriendsInfoReq req;
	// 1. required PlatformType platform  = 1;    //关联平台类型
	req.set_platform(::Star::PlatformType_Facebook);
	// 2. repeated string account_list    = 2;    //账号列表
	for (int i=0,count=users.size(); i<count; ++i)
	{
		req.add_account_list(users[i].userId);
	}
	// 3. repeated int64 task_list        = 3;    //关卡列表
	CCAssert(sections.size()>0, "");
	for (int i=0,count=sections.size(); i<count; ++i)
	{
		long long task_id = sections[i].first;
		task_id <<= 16;
		task_id |= sections[i].second;
		req.add_task_list(task_id);
	}
	string data = req.SerializeAsString();
	NetworkThreadProc::getInstance()->sendData(Req_QueryFriendsInfo, data.c_str(), data.length());

	if (m_bShowWaiting) NetworkWaitingLayer::showWaiting();

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox("getSocialInfo", "info");
#endif
}
void CmdHelper::onGetSeasonSocialInfoRsp(const void* data, unsigned int dataLength)		
{
	NetworkWaitingLayer::hideWaiting();

	::Star::QueryFriendsInfoRsp rsp;
	int rsp_code = MessageErrorCode::Error;
	if (data){
		CHECK_PARSE_RSP_DATA ( rsp.ParseFromArray(data, dataLength) );
		rsp_code = rsp.rsp_code();
	}
	else {
		CCLog("%s ---> time out!", __FUNCTION__);
		rsp_code = MessageErrorCode::Error;
	}
	CCLog("%s ---> Rsp_QueryFriendsInfo rsp_code = %d", __FUNCTION__, rsp_code);

	MsgData msg_data;
	if (rsp_code != MessageErrorCode::Success){
		if (m_bAlreadyGetFriendsInfor) m_bAlreadyGetFriendsInfor = false;
		//同步数据的时候 会多个一起请求，如果失败一个，网络就会断开，后面都就不会处理了。
		msg_data.errCode = MsgData::eError;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_GotSeasonSocialInfo, &msg_data);
		return;
	}
	//获取数据
	/*
	 * taskInfor以移除
	*/

	string platform,userId;
	CCAssert(History::getHistory()->getLastUserPlatformAndID(platform, userId), "玩家登录过才能和游戏服务器同步数据");
	int friendNum = rsp.friend__size();
/*	int taskNum = rsp.task_size();
	vector<TaskInfor> taskList(taskNum);
	//1. repeated TaskInfo task          = 2;    //关卡信息
	CCLOG("rsp TaskInfo:");
	for (int i=0; i<taskNum; ++i)
	{
		const ::Star::TaskInfo& taskInf = rsp.task(i);
		long long task_id = taskInf.task_id();
		taskList[i].seasonId  = (0x00000000ffff0000LL & task_id) >> 16;
		taskList[i].sectionId = (0x000000000000ffffLL & task_id);
		taskList[i].maxScore = taskInf.highest_score();
		taskList[i].avgScore = taskInf.average_score();
		taskList[i].playerCount = taskInf.player_count();
		CCLOG("  ---%d-%d: maxScore[%d], avgScore[%d], playerCount[%d]", 
			taskList[i].seasonId, taskList[i].sectionId, taskList[i].maxScore, taskList[i].avgScore, taskList[i].playerCount);
	}*/
	//2. repeated FriendsInfo friend     = 3;    //好友信息
	CCLOG("rsp FriendsInfo: friends num = %d", friendNum);
	vector<TaskInfor> taskList;
	for (int i=0; i<friendNum; ++i)
	{
		const ::Star::FriendsInfo& friendInfo = rsp.friend_(i);
		int solu_size = friendInfo.solution_size();
		CCLOG(" [%d] friend solution: %s", i, friendInfo.account().c_str());
		for (int solutIdx=0; solutIdx<solu_size; ++solutIdx)
		{
			//拿到次玩家的每一个solution , 找到它是属于哪一个关卡的解法， 再放到该关卡信息中
			TaskSolution temp;
			const ::Star::SolutionInfo& solutInf = friendInfo.solution(solutIdx);
			temp.userId = friendInfo.account();
			long long task_id = solutInf.task_id();
			temp.seasonId  = (0x00000000ffff0000LL & task_id) >> 16;
			temp.sectionId = (0x000000000000ffffLL & task_id);
			temp.star = solutInf.star();
			temp.score = solutInf.score();
			temp.solutionId = solutInf.has_solution_id() ? solutInf.solution_id() : "";
			temp.downloadTimes = solutInf.has_download_times() ? solutInf.download_times() : 0;
			temp.upTimes = solutInf.has_up_times() ? solutInf.up_times() : 0;
			//temp.data  ---> 另外的命令请求后才能拿到data
			CCLOG("  ---%d-%d: star[%d], score[%d], solutionId[%s], downloadTimes[%d], upTimes[%d]", 
				temp.seasonId, temp.sectionId, temp.star, temp.score, temp.solutionId.c_str(), temp.downloadTimes, temp.upTimes);
			//放到对应的taskinfor中
			bool bNotFound = true;
			for (size_t taskIdx=0; taskIdx<taskList.size(); ++taskIdx)
			{
				if (taskList[taskIdx].seasonId == temp.seasonId && taskList[taskIdx].sectionId == temp.sectionId){
					//去重
					bool bfind = false;
					for (vector<TaskSolution>::iterator it=taskList[taskIdx].users.begin(); it != taskList[taskIdx].users.end(); ++it)
					{
						if (it->userId == temp.userId){
							bfind = true;
							break;
						}
					}
					if (!bfind){
						taskList[taskIdx].users.push_back(temp);
					}
					bNotFound = false;
					break;
				}
			}
			if (bNotFound){
				TaskInfor tempTask;
				tempTask.seasonId = temp.seasonId;
				tempTask.sectionId = temp.sectionId;
				tempTask.avgScore = 0;
				tempTask.maxScore = 0;
				tempTask.playerCount = 0;
				tempTask.users.push_back(temp);
				taskList.push_back(tempTask);
			}
		}
	}
	//设置数据
	SocialInfoManager::getInstance()->setSectionSocialInfor(taskList);

	msg_data.errCode = MsgData::eSuccess;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_GotSeasonSocialInfo, &msg_data);


#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox(CCString::createWithFormat("rsp social info, taskNum[%d], firendsAndMeNum[%d]", taskList.size(), friendNum)->getCString(), "info");
#endif
}

bool CmdHelper::hasAccount()
{
	string platform,userId;
	return History::getHistory()->getLastUserPlatformAndID(platform, userId);
}

void CmdHelper::displayErrorMessageBox(NetErrorCode errCode)
{
	switch (errCode)
	{
	case NetSuccess:
		break;
	case NetError:
		MessageBar::show(LFStrings::getValue("Networkerror").c_str());
		//CCMessageBox("Network error", "Error");
		break;
	case NetTimeOut:
		MessageBar::show(LFStrings::getValue("Connectiontimeout").c_str());
		//CCMessageBox("Connection timeout", "Error");
		break;
	case NetDataError:
		MessageBar::show(LFStrings::getValue("DataError").c_str());
		//CCMessageBox("Data Error", "Error");
		break;
	default:
		break;
	}
}
void CmdHelper::setLastNetworkError(NetErrorCode errcode)
{
	m_lastNetworkError = errcode;
}
NetErrorCode CmdHelper::getLastNetworkError()
{
	return m_lastNetworkError;
}





void CmdHelper::login()
{
#if _USE_VIRTURE_USER_INFOR_ == 1
	_VIRTUE_IsLogin = true;
	this->onLoginResult(FX::LoginResultCode::eSuccess, NULL);
	return;
#endif

	m_bShowWaiting = true;

	//统计登录Facebook事件
	this->getAnalyticsInstance()->onBegin("login");

	//已登录直接返回登录成功消息
	if (this->isLogin())
	{
		//统计登录Facebook事件
		this->getAnalyticsInstance()->onCompleted("login");
		MsgData rsp;
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_LoginResult, &rsp);
		return;
	}
	//未登录
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	pUser->setLoginListener(this);
	pUser->login();
	NetworkWaitingLayer::showWaiting();
}

void CmdHelper::onLoginResult(FX::LoginResultCode ret, const char* msg)
{
	std::unique_lock<std::mutex> lock(m_mutexSdkResultQueue);
	m_sdkResultQueue.push(std::make_tuple(SDKEvent::eLoginResult, (int)ret, string(msg==nullptr?"":msg), nullptr));
}
void CmdHelper::loginResultProcess(FX::LoginResultCode ret, const char* msg)
{
	NetworkWaitingLayer::hideWaiting();
	//登录成功，首先刷新数据
	MsgData rsp;
	if (ret == FX::LoginResultCode::eSuccess)
	{
		FX::UserInfo userInfor = this->getUserInfo();
		if (userInfor.id.empty())
		{
			rsp.errCode = MsgData::eError;
			this->logout();
			//统计登录Facebook事件
			this->getAnalyticsInstance()->onFailed("login", "not get user infor");
		}
		else //登录成功且 拿到userinfor
		{
			rsp.errCode = MsgData::eSuccess;
			string platform,id;
			bool bhas = History::getHistory()->getLastUserPlatformAndID(platform,id);
			//登录成功后 立刻切换账号数据
			if (!bhas || platform != "Facebook" || id != userInfor.id){
				this->resetUserData(userInfor);
			}
			CCLog("login success, user id = %s", userInfor.id.c_str());
			s_syncSocialDataStep = 2;
			//1. 登录成功刷新数据后，立刻与服务器同步数据
			this->SyncCurUserData(m_bShowWaiting);
			//2. 登录成功刷新数据后，立刻获取好友列表 //. 获取好友进度
			this->getCurUserAppFriends(std::bind(&CmdHelper::onGetFriendsResult, this, std::placeholders::_1, std::placeholders::_2));

			//统计登录Facebook事件(必须在 下面更新账号之前)
			this->getAnalyticsInstance()->onCompleted("login");
			//统计
			this->getAnalyticsInstance()->setAccount(userInfor.id.c_str());
			this->getAnalyticsInstance()->setAccountType(FX::PluginAnalytics::eAccountFacebook);
			this->getAnalyticsInstance()->setAccountName(userInfor.name.c_str());
		}
	}
	else 
	{
		rsp.errCode = MsgData::eError;
		CCLog("login error, msg = %s", msg);

		//统计账号
		this->getAnalyticsInstance()->setAccount(this->getAnalyticsInstance()->getDeviceId());
		this->getAnalyticsInstance()->setAccountType(FX::PluginAnalytics::eAccountAnonymous);
		//统计登录Facebook事件
		this->getAnalyticsInstance()->onFailed("login", msg);
	}
	// notification
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_LoginResult, &rsp);
}

void CmdHelper::logout()
{
#if _USE_VIRTURE_USER_INFOR_ == 1
	_VIRTUE_IsLogin = false;
#else
	//退出的统计 放在退出按钮处。 因为在登录缓存状态 且无账户信息，会调用logout
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	pUser->logout();
#endif
	m_bAlreadyGetFriendsInfor = false;
	//退出后  删除社交好友进度，发送刷新ui的消息
	SocialInfoManager::getInstance()->resetLocalData();
	MsgData msg_data;
	msg_data.errCode = MsgData::eSuccess;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_GotSeasonSocialInfo, &msg_data);
}

bool CmdHelper::getCurrentUserID(string& id)
{
	string plat;
	return History::getHistory()->getLastUserPlatformAndID(plat,id);
}

void CmdHelper::resetUserData(FX::UserInfo& info)
{
	//. reset
	History::getHistory()->setLastUserPlatformAndID("Facebook", info.id, info.name, info.picture);
	GameInforEditer::getGameInfor()->destroy();
	GameInforEditer::getGameInfor()->initData();
	SocialInfoManager::getInstance()->resetLocalData();//删除本地模拟数据
	AchievementManager::getInstance()->destroy();
	AchievementManager::getInstance()->initData();
	UserDataManager::getInstance()->destroy();
	UserDataManager::getInstance()->initData();
	Recorder::getInstance()->resetUserData();
}
bool CmdHelper::isLogin()
{
#if _USE_VIRTURE_USER_INFOR_ == 1
	return _VIRTUE_IsLogin;
#endif
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	bool ret = pUser->isLogined();
	if (ret){
		if (this->getUserID().empty()){
			this->logout();
			return false;
		}
		return true;
	}
	return false;
}
std::string CmdHelper::getUserID()
{
#if _USE_VIRTURE_USER_INFOR_ == 1
	if (_VIRTUE_IsLogin) return _VIRTUE_USER_ID_;
	else return "";
#endif
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	return pUser->getUserID();
}
FX::UserInfo CmdHelper::getUserInfo()
{
#if _USE_VIRTURE_USER_INFOR_ == 1
	FX::UserInfo ret;
	ret.id = _VIRTUE_USER_ID_;
	ret.name = _VIRTUE_USER_NAME_;
	ret.picture = _VIRTUE_USER_PICTURE_;
	return ret;
#endif
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	return pUser->getUserInfo();
}

void CmdHelper::inviteFriends(const char* message)
{
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();

	if (message == nullptr)
	{
		int idx = rand() % 3 + 1;
		pUser->inviteFriends(this, LFStrings::getValue(CCString::createWithFormat("shareDescribe_%02d", idx)->getCString()).c_str());
	}else
	{
		pUser->inviteFriends(this, message);
	}
}
void CmdHelper::onInviteFriendsResult(FX::InviteFriendsResultCode ret, const char* msg)
{
	std::unique_lock<std::mutex> lock(m_mutexSdkResultQueue);
	m_sdkResultQueue.push(std::make_tuple(SDKEvent::eInviteFriendsRusult, (int)ret, string(msg==nullptr?"":msg), nullptr));
}
void CmdHelper::inviteFriendsResultProcess(FX::InviteFriendsResultCode ret, const char* msg)
{
	MsgData rsp;
	if (ret == FX::InviteFriendsResultCode::eSuccess) rsp.errCode = MsgData::eSuccess;
	else rsp.errCode = MsgData::eError;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_InviteFriendsResult, &rsp);

	// 
	if (ret == FX::InviteFriendsResultCode::eSuccess)
		MessageBar::show(LFStrings::getValue("InviteSuccess").c_str());
	else 
		MessageBar::show(LFStrings::getValue("InviteFailed").c_str());
}

//分享
void CmdHelper::share(map<string,string>* shareContent)
{
	if (!isLogin()){
		//MessageBar::show(LFStrings::getValue("Pleaseloginatfirst").c_str());
		//CCMessageBox("Please login at first!", "Tip");
		login();
		return;
	}

	FX::PluginShare *pShare = FX::PluginManager::getInstance()->getPluginShare();
	pShare->setShareListener(this);
	if (!shareContent || shareContent->empty())
	{
		map<string,string> content;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		content["link"] = "https://itunes.apple.com/us/app/lost-star-landing/id980188049?l=zh&ls=1&mt=8";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		content["link"] = "http://www.camellia-network.com";
#else
		content["link"] = "http://www.camellia-network.com";
#endif

#if Version_Free != 0
		content["name"] = LFStrings::getValue("shareName") + " Free";
#else
		content["name"] = LFStrings::getValue("shareName");
#endif
		//content["caption"] = "";
		int idx = rand() % 3 + 1;
		content["description"] = LFStrings::getValue(CCString::createWithFormat("shareDescribe_%02d", idx)->getCString());
		content["pictureURL"] = "http://www.2ndfantasy.com/for_fb/lost_star_fb.jpg";
		pShare->share(&content);
	}
	else
	{
		pShare->share(shareContent);
	}
}

void CmdHelper::onShareResult(FX::ShareResultCode ret, const char* msg)
{
	std::unique_lock<std::mutex> lock(m_mutexSdkResultQueue);
	m_sdkResultQueue.push(std::make_tuple(SDKEvent::eShareRusult, (int)ret, string(msg==nullptr?"":msg), nullptr));
}
void CmdHelper::shareResultProcess(FX::ShareResultCode ret, const char* msg)
{
	MsgData rsp;
	if (ret == FX::ShareResultCode::eSuccess) rsp.errCode = MsgData::eSuccess;
	else rsp.errCode = MsgData::eError;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_ShareResult, &rsp);

	// 
	if (ret == FX::ShareResultCode::eSuccess)
		MessageBar::show(LFStrings::getValue("sharedSuccess").c_str());
	else 
		MessageBar::show(LFStrings::getValue("sharedFailed").c_str());
}

//拿到完过该游戏的朋友列表
void CmdHelper::getCurUserAppFriends(FX::pfuncGetFriends callback)
{
	// 使用模拟账号时，是使用本地配置文本里面的好友
#if _USE_VIRTURE_USER_INFOR_ == 1
	vector<FX::UserInfo> vec;
	FX::UserInfo temp;
	//
	temp.id = "1391720927806122";
	temp.name = "Mr.boshi";
	temp.picture = "ipad/nativeFacebookPicture_boshi.png";
	vec.push_back(temp);
// 	temp.id = "1391720927806121";
// 	temp.name = "camellia";
// 	temp.picture = _VIRTUE_USER_PICTURE_;
// 	vec.push_back(temp);
	//
	this->onGetFriendsResult(true, vec);
	return;
#endif

	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	pUser->getAppFriendList(callback);
}
//拿到社交平台的所有朋友
void CmdHelper::getCurUserAllFriends(FX::pfuncGetFriends callback)
{
	FX::PluginUser *pUser = FX::PluginManager::getInstance()->getPluginUser();
	pUser->getAllFriendList(callback);
}
void CmdHelper::onGetFriendsResult(bool bSuccess, std::vector<FX::UserInfo> vec)
{
	if (--s_syncSocialDataStep)
		NetworkWaitingLayer::hideWaiting();

	//如果查询好友成功，那么之后每一小时才更新一次数据，否则每两分钟查询一次，直至查询成功(第一步，第二步在查询好友进度完成后还要赋值)
	m_bAlreadyGetFriendsInfor = bSuccess;
	if (! bSuccess){
		this->unschedule(schedule_selector(CmdHelper::updateSocialInfo));
		this->schedule(schedule_selector(CmdHelper::updateSocialInfo), GET_SOCIAL_INFOR_TIME);
	}

	//就算bSuccess为false   也要查询玩家自己的
	vec.push_back(this->getUserInfo());
	int count = vec.size();
	vector<TaskUser> taskUsers(count);
	CCLOG("%s ---> get friends rsp: size = %d", __FUNCTION__, count);
	for (int i=0; i<count; ++i)
	{
		taskUsers[i].userId = vec[i].id;
		taskUsers[i].name = vec[i].name;
		taskUsers[i].pictrue = vec[i].picture;
		CCLOG("  %d--- userId[%s], name[%s]", i, vec[i].id.c_str(), vec[i].name.c_str());
	}
	SocialInfoManager::getInstance()->setFriendsAndMeInfor(taskUsers);

#if DEUBG_DISPLAY_INFORMATION == 1
	CCMessageBox(CCString::createWithFormat("friends count = %d", vec.size()-1)->getCString(), "info");
#endif

	// query all
	vector<std::pair<unsigned int, unsigned int> > queryVec;
// 	int lastSeaonid = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
// 	int lastSectionid = GameInforEditer::getGameInfor()->getLastPlayedSectionId();
// 	int lastidx = GameLayerFactory::getInstance()->getSectionIndex(lastSeaonid, lastSectionid);
	int totalNum = GameLayerFactory::getInstance()->getTotalSectionNum();
	for (int i=0; i<totalNum; ++i)
	{
		queryVec.push_back(GameLayerFactory::getInstance()->getLevelFormat(i));
	}
	this->getSocialInfo(queryVec, m_bShowWaiting);
}



void CmdHelper::buyProduct(ProductID productId)
{
	NetworkWaitingLayer::showWaiting();

	//购买的物品数据
	FX::ProductInfoMap infomap;
	infomap["id"] = CCString::createWithFormat("%d",productId)->getCString();

	//获取配置的物品信息
	const ProductInfo* info = ProductInfoHelper::getHelper()->getOneProductInfo(productId);
	string channel = ProductInfoHelper::getHelper()->getChannel();
	if (!info || channel.empty())
	{
		CCAssert(false, "not found the productid!!!");
		onPayResult(FX::PayResultCode::ePayFail, "not found the productid!!!", infomap);
		return;
	}

	//生成orderID
	////////////////////////////////////////////////////////////////////
	string orderId = this->getAnalyticsInstance()->getDeviceId();
	char buf[48];
	time_t tt = time(NULL);//这句返回的只是一个时间戳
	tm* t= localtime(&tt);
	sprintf(buf, "%d-%02d-%02d_%02d:%02d:%02d", 
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	orderId += buf;
	//////////////////////////////////////////////////////////////////////
	for (map<string,string>::const_iterator it=info->channelProduct.begin(); it!=info->channelProduct.end(); ++it)
	{
		if (it->first == channel){
			infomap["Product_Id"] = it->second; // productId
			break;
		}
	}
	infomap["Product_OrderID"] = orderId;
	infomap["Product_Price"] = CCString::createWithFormat("%d", info->price/100)->getCString();
	infomap["Product_Name"] = info->name.c_str();
	infomap["Product_Count"] = "1";//商品份数(除非游戏需要支持一次购买多份商品，否则传1即可)
	//////////////////////////////////////////////////////////////////////


	//analytics
	this->getAnalyticsInstance()->onChargeRequest(orderId.c_str(), info->name.c_str(), info->price/100.0f, "CNY", info->price/100.0, "AppStore");
	FX::PluginManager::getInstance()->getPluginAnalytics(PLUGIN_ANALYTICS_DataEye)->onChargeRequest(
		orderId.c_str(), info->name.c_str(), info->price/100.0f, "CNY", info->price/100.0, "AppStore");


#if DEBUG_ALWAYS_BUY_SUCCESS == 1 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	onPayResult(FX::PayResultCode::ePaySuccess, "", infomap);
	return;
#endif

	FX::PluginIAP* pIAP = FX::PluginManager::getInstance()->getPluginIAP();
	if (!pIAP)
	{
		onPayResult(FX::PayResultCode::ePayFail, "has not inplement iap", infomap);
		return;
	}
	//设置监听
	pIAP->setPayListener(this);
	pIAP->payForProduct(infomap);
}

void CmdHelper::onPayResult(FX::PayResultCode ret, const char* msg, FX::ProductInfoMap productInfo)
{
	std::unique_lock<std::mutex> lock(m_mutexSdkResultQueue);
	
	FX::ProductInfoMap* pData = new FX::ProductInfoMap();
	*pData = productInfo;
	m_sdkResultQueue.push(std::make_tuple(SDKEvent::ePayResult, (int)ret, string(msg==nullptr?"":msg), (void*)pData));
	// delete by dispatch method
}
void CmdHelper::payResultProcess(FX::PayResultCode ret, const char* msg, FX::ProductInfoMap* productInfo)
{
	NetworkWaitingLayer::hideWaiting();

	if (productInfo == nullptr){
		CCLOG("ERROR: %s ---> product info is null", __FUNCTION__);
		return;
	}

	ProductID prodId = (ProductID)atoi(productInfo->at("id").c_str());
	MsgData rspData;
	rspData.data = &prodId;

	switch (ret)
	{
	case FX::PayResultCode::ePaySuccess:
		rspData.errCode = MsgData::eSuccess;
		break;
	case FX::PayResultCode::ePayCancle:
		rspData.errCode = MsgData::eError;
		break;
	case FX::PayResultCode::ePayFail:
		rspData.errCode = MsgData::eError;
		break;
	case FX::PayResultCode::ePayNetworkError:
		rspData.errCode = MsgData::eError;
		break;
	default:
		rspData.errCode = MsgData::eError;
		break;
	}

	//1. if success, save data to native.
	if (rspData.errCode == MsgData::eSuccess)
	{
		//analytics
		this->getAnalyticsInstance()->onChargeSuccess(productInfo->at("Product_OrderID").c_str());
		FX::PluginManager::getInstance()->getPluginAnalytics(PLUGIN_ANALYTICS_DataEye)->onChargeSuccess(productInfo->at("Product_OrderID").c_str());

		if (prodId == ProductID_OfficialSolution)
		{
			GameInforEditer::getGameInfor()->setHasPayOfficialSolution(true);
			//analytics
			//统计超能道具的使用情况
			CmdHelper::getInstance()->getAnalyticsInstance()->onPurchase("官方解法", 1, atoi(productInfo->at("Product_Price").c_str()));
			FX::PluginManager::getInstance()->getPluginAnalytics(PLUGIN_ANALYTICS_DataEye)->onPurchase(
				"官方解法", 1, atoi(productInfo->at("Product_Price").c_str()));
		}
		else if (prodId == ProductID_SuperTool_One)
		{
			GameInforEditer::getGameInfor()->addSuperTool(1);
			//analytics
			//统计超能道具的使用情况
			CmdHelper::getInstance()->getAnalyticsInstance()->onPurchase("超能道具", 1, atoi(productInfo->at("Product_Price").c_str()));
			FX::PluginManager::getInstance()->getPluginAnalytics(PLUGIN_ANALYTICS_DataEye)->onPurchase(
				"超能道具", 1, atoi(productInfo->at("Product_Price").c_str()));
		}
		else if (prodId == ProductID_SuperTool_Ten)
		{
			GameInforEditer::getGameInfor()->addSuperTool(10);
			//analytics
			//统计超能道具的使用情况
			CmdHelper::getInstance()->getAnalyticsInstance()->onPurchase("超能道具", 10, atoi(productInfo->at("Product_Price").c_str()));
			FX::PluginManager::getInstance()->getPluginAnalytics(PLUGIN_ANALYTICS_DataEye)->onPurchase(
				"超能道具", 10, atoi(productInfo->at("Product_Price").c_str()));
		}
	}

	//2. post msg to ui
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NetMessage::MSG_PayResult, &rspData);
	//ProductID id = *(ProductID*)rspData.data;
}

FX::PluginAnalytics* CmdHelper::getAnalyticsInstance()
{
	return FX::PluginManager::getInstance()->getPluginAnalytics(PLUGIN_ANALYTICS_TalkingData);
}





void CmdHelper::onMessageProcess(NetErrorCode code, MessageCmd cmd, void* data, unsigned int dataLength)
{

#if DEUBG_DISPLAY_INFORMATION == 1
	if (code == NetTimeOut)
	{
		CCMessageBox("Time out, Please check your network status", "Error");
	}
	else if (code == NetError)
	{
		CCMessageBox("Network Error", "Error");
	}
#endif

	if (code != NetSuccess) {
		this->setLastNetworkError(code);
	}

	switch (cmd)
	{
	case Rsp_Sync:
		onSyncCurUserDataRsp(data, dataLength);
		break;
	case Rsp_UploadTaskInfo:
		{
			CCLog("%s ---> cmd[Rsp_UploadTaskInfo] not use", __FUNCTION__);
			CCAssert(false,"");
		}
		break;
	case Rsp_UploadSolution:
		onUploadTaskSolutionRsp(data,dataLength);
		break;
	case Rsp_DownloadSolution:
		onDownloadTaskSolutionRsp(data,dataLength);
		break;
	case Rsp_UpSolution:
		onPraiseSolutionRsp(data,dataLength);
		break;
	case Rsp_QueryFriendsInfo:
		onGetSeasonSocialInfoRsp(data,dataLength);
		break;
	case Rsp_RequestError:
	default:
		CCLog("%s ---> error cmd[%d]", __FUNCTION__, cmd);
		break;
	}
}

void CmdHelper::updateSdkResultDispatch(float dt)
{
	std::unique_lock<std::mutex> lock(m_mutexSdkResultQueue, std::try_to_lock);
	if (lock.owns_lock() && m_sdkResultQueue.size() > 0)
	{
		SDKEvent eventId = SDKEvent::eNoneEvent;
		int errCode = -1; // 0 -> success
		string msg;
		void * pData; //the data will delete
		std::tie(eventId, errCode, msg, pData) = m_sdkResultQueue.front();
		m_sdkResultQueue.pop();

		switch (eventId)
		{
		case SDKEvent::eShareRusult:
			{
				shareResultProcess((FX::ShareResultCode)errCode, msg.c_str());
			}
			break;
		case SDKEvent::eLoginResult:
			{
				loginResultProcess((FX::LoginResultCode)errCode, msg.c_str());
			}
			break;
		case SDKEvent::ePayResult:
			{
				FX::ProductInfoMap* pProdInfor = (FX::ProductInfoMap*)pData;
				payResultProcess((FX::PayResultCode)errCode, msg.c_str(), pProdInfor);
				if (pProdInfor){
					pProdInfor->clear();
					delete pProdInfor;
				}
			}
			break;
		case SDKEvent::eInviteFriendsRusult:
			{
				inviteFriendsResultProcess((FX::InviteFriendsResultCode)errCode, msg.c_str());
			}
			break;
		case SDKEvent::eNoneEvent:
		default:
			CCLOG("ERROR: %s ---> error event id[%d]", __FUNCTION__, eventId);
			break;
		}
	}
}

//stringCompress()