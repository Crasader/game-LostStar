#ifndef __LFShareKit_H__
#define __LFShareKit_H__
#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

#define facebookBaseShareUrlPre  "http://www.facebook.com/sharer/sharer.php?s=100&p[title]=Pick Up The Star&p[summary]="
#define facebookBaseShareUrlPreEnd "&p[url]=http://www.camellia-network.com/"

//http://twitter.com/share?text=An%20Awesome%20Link&url=http://www.google.com">
//Share This on Twitter
#define twitterBaseShareUrl  "http://twitter.com/intent/tweet?text="
#define sinaBaseShareUrl  "http://v.t.sina.com.cn/share/share.php?title="
#define renrenBaseShareUrl  "http://widget.renren.com/dialog/share?title="
#define tencentBaseShareUrl  "http://share.v.t.qq.com/index.php?c=share&a=index&url="
#define lfShareContent  "%E5%A4%A7%E5%AE%B6%E5%A5%BD%EF%BC%8C%E7%BB%99%E5%A4%A7%E5%AE%B6%E6%8E%A8%E8%8D%90%E4%B8%80%E6%AC%BE%E6%B8%B8%E6%88%8F,%E5%90%8D%E5%AD%97%E6%98%AF%E3%80%8ARed%20Star%20Fall%E3%80%8B%EF%BC%8C%E5%BE%88%E7%9B%8A%E6%99%BA%EF%BC%8C%E5%BE%88%E5%A5%BD%E7%8E%A9%E7%9A%84%EF%BC%81%EF%BC%81"
#define lfShareContent_coded  "%E5%A4%A7%E5%AE%B6%E5%A5%BD%EF%BC%8C%E7%BB%99%E5%A4%A7%E5%AE%B6%E6%8E%A8%E8%8D%90%E4%B8%80%E6%AC%BE%E6%B8%B8%E6%88%8F,%E5%90%8D%E5%AD%97%E6%98%AF%E3%80%8ARed%20Star%20Fall%E3%80%8B%EF%BC%8C%E5%BE%88%E7%9B%8A%E6%99%BA%EF%BC%8C%E5%BE%88%E5%A5%BD%E7%8E%A9%E7%9A%84%EF%BC%81%EF%BC%81"

enum ShareTo{
	kShareToFacebook = 0,
	kShareToTwitter,
	kShareToSinaWeibo,
	kShareToRenren,
	kShareToTencent,
	kShareToAll,
};

class LFShareKit
{
private:
	static LFShareKit* kit;
	bool enableGameCenter;
    /**
     *  构造的时候会去校验GameCenter
     */
	LFShareKit(void);
	/**
	 *	验证GameCenter，必须最先调用
	 */
	void gameCenterAuthenticateLocalUser();
public:
	~LFShareKit(void);
    /**
     *  得到LFShareKit实例，第一次调用时就会验证GameCenter
     */
	static LFShareKit* getKit();
	//分享网站
	void share(ShareTo shareto,string content);
	//分享到网站，使用默认的分享字符串
	void share(ShareTo shareto);
	void openURl(string url);

	bool isGameCenterEnabled();

	/**
	 *	显示GameCenter
	 */
	void showGameCenter();
    
    void showGameCenterAchievement();
    
    void reportAchievementToGameCenter(string identifier,float percent);
	/**
	 *	向GameCenter上报成绩
	 *	score	: 上报的分数
	 *	category: 分数上报到GameCenter创建的列表的哪一个字段
	 */
	void reportScoreToGameCenter(float score,string category);
};

#endif