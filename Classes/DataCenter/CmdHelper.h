
/*
* UI层数据请求接口 仅用于（sdk、network）相关。
* 需要网络请求的数据将会以消息的方式返回。
*/

#pragma once

#include "cocos2d.h"
#include "FXPlugin/FXPluginManager.h"
#include "net/NetworkPackageDispatch.h"
#include "SocialSeasonInfor.h"
#include "FXPlugin/ProductInfoHelper.h"

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <queue>

using namespace cocos2d;
using std::string;
using std::map;
using std::vector;


// 1. 消息池 --- 返回消息（logic 通知 UI）
namespace NetMessage{
	static const char*  MSG_LoginResult = "NetMessage:MSG_LoginResult";
	static const char*  MSG_ShareResult = "NetMessage:MSG_ShareResult";
	static const char*  MSG_InviteFriendsResult   = "NetMessage:MSG_InviteFriendsResult";
	static const char*  MSG_PayResult   = "NetMessage:MSG_PayResult";

	static const char*  MSG_SyncUserDataEnd = "NetMessage:MSG_SyncUserDataEnd"; //同步用户数据到服务器结束
	static const char*  MSG_GotSeasonSocialInfo = "NetMessage:MSG_GotSeasonSocialInfo"; //获取一个season的社交信息（好友分数、排行、关卡全服平均分）
	static const char*  MSG_PraiseSolution = "NetMessage:MSG_PraiseSolution"; //点赞
	static const char*  MSG_DownloadSolution = "NetMessage:MSG_DownloadSolution"; //下载解法
}

// 2. 消息结构
class MsgData : public CCObject
{
public:
	enum ErrCode
	{
		eSuccess = 0,
		eError,
	};
	MsgData(){
		errCode = eError;
		data = nullptr;
	}

	int errCode;
	void *data;
};

// 3. sdk 回调事件   （Android jni回调时一般情况是不在2dx主线程中的，此时调用UI接口，容易黑屏，或者崩溃，全部使用消息池
enum class SDKEvent
{
	eNoneEvent = 0,
	eShareRusult = 1,
	eLoginResult = 2,
	ePayResult = 3,
	eInviteFriendsRusult,
};


// 4. 命令接口cmdhelper
class CmdHelper : public CCNode
	, public NetworkMessageListener
	, public FX::UserLoginListener, public FX::InviteFriendsListener
	, public FX::ShareResultListener
	, public FX::PayResultListener
{
private:
	CmdHelper();
	bool init();
	static CmdHelper* _instanceCmdHelper;
public:
	static CmdHelper* getInstance();

#pragma region c-s 
public:
	//如果Facebook缓存了登录信息，那么下次进入游戏会是登录状态，直接同步服务器数据
	void initSyncServerData();

	//上传数据 同步该账户数据
	void SyncCurUserData(bool showWaiting);

	//上传解法信息
	void uploadTaskSolution(vector<std::pair<int,int> > sectionList);

	//下载解法信息
	void downloadTaskSolution(string solutionId, bool showWaiting);
	void downloadTaskSolution(int seasonId, int sectionId, string userId, bool showWaiting);

	//点赞praise=true  踩praise=false
	void praiseSolution(string solutionId, bool praise, bool showWaiting);

	//关卡全球数据，好友排名
	//传入当前正在玩的sectionid ，预下载接下来几关的数据  current --- current + preloadNum   (preloadNum + 1)
	void preloadSectionSocialInfo(unsigned int seasonId, unsigned int currentPlaySectionId, unsigned int preloadNum=5);
	void getSeasonSocialInfo(unsigned int seasonId, bool showWaiting);
	void getSocialInfo(vector<std::pair<unsigned int,unsigned int> > sections, bool showWaiting); // seasonId - sectionId
	bool isAlreadyGetSocialInfor(){return m_bAlreadyGetFriendsInfor;}

protected:
	void updateSocialInfo(float dt);
	void refreshSocialInfor(float dt);

	bool hasAccount(); //绑定过社交账号才能与服务器同步数据
	void displayErrorMessageBox(NetErrorCode errCode);
	void setLastNetworkError(NetErrorCode errcode);
	NetErrorCode getLastNetworkError();
private:
	NetErrorCode m_lastNetworkError;
#pragma endregion



#pragma region sdk
public:
	/* 登录 sdk， 须联网
	 * 调用前先判断是否已登录，如果已登录调用了改接口直接返回登录成功的消息
	 * 回调监听MSG_LoginCallback消息
	 * 如果登录成功，那么history中last platform、id 也会随之保存，datacenter数据立刻刷新，并开始同步服务器数据
	*/
	void login();
	void logout();
	bool isLogin();
	bool getCurrentUserID(string& id); // return is have account
	//邀请好友( = nullptr 使用默认消息)
	void inviteFriends(const char* message = nullptr);

	//分享  ( null 使用默认分享信息)
	void share(map<string,string>* shareContent=nullptr);

	//数据统计
	FX::PluginAnalytics* getAnalyticsInstance();

	//支付
	void buyProduct(ProductID productId);

protected:
	std::string getUserID();
	FX::UserInfo getUserInfo();
	void resetUserData(FX::UserInfo& info);
	//登录回调时，获取一次就够了
	//拿到完过该游戏的朋友列表
	void getCurUserAppFriends(FX::pfuncGetFriends callback);
	//拿到社交平台的所有朋友
	void getCurUserAllFriends(FX::pfuncGetFriends callback);
#pragma endregion



	//回调
protected:
	virtual void onLoginResult(FX::LoginResultCode ret, const char* msg);
	virtual void onShareResult(FX::ShareResultCode ret, const char* msg);
	virtual void onInviteFriendsResult(FX::InviteFriendsResultCode ret, const char* msg);
	virtual void onPayResult(FX::PayResultCode ret, const char* msg, FX::ProductInfoMap productInfo);
	void onGetFriendsResult(bool bSuccess, std::vector<FX::UserInfo> vec);
	void updateSdkResultDispatch(float dt);
	void loginResultProcess(FX::LoginResultCode ret, const char* msg);
	void shareResultProcess(FX::ShareResultCode ret, const char* msg);
	void inviteFriendsResultProcess(FX::InviteFriendsResultCode ret, const char* msg);
	void payResultProcess(FX::PayResultCode ret, const char* msg, FX::ProductInfoMap* productInfo);


	virtual void onMessageProcess(NetErrorCode, MessageCmd, void*, unsigned int);
	void onSyncCurUserDataRsp(const void* data, unsigned int dataLength);
	void onUploadTaskSolutionRsp(const void* data, unsigned int dataLength);
	void onDownloadTaskSolutionRsp(const void* data, unsigned int dataLength);
	void onPraiseSolutionRsp(const void* data, unsigned int dataLength);
	void onGetSeasonSocialInfoRsp(const void* data, unsigned int dataLength);

protected:

	//缓存登录时，可能连不上Facebook只连上gameServer，此时拿不到好友，进度就只有自己和机器人
	//在这种情况 就每过几分钟就查询一次好久，直到拿到好友列表且从gameServer拿到进度 (之后每1小时刷新一次进度数据)
	bool m_bAlreadyGetFriendsInfor;

	//多个网络调用时是顺序执行的，在调用时由调用者确定是否需要显示转圈
	bool m_bShowWaiting;

	//sdk的返回消息队列，等待主线程处理
	std::queue<std::tuple<SDKEvent, int, string, void*>> m_sdkResultQueue; //eventId，errorCode，errorMsg，dataPtr(delete when use end)
	std::mutex m_mutexSdkResultQueue;
};