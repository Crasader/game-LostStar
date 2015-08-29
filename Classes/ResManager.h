#ifndef __ResManager_H__
#define __ResManager_H__

#include "cocos2d.h"
#include <string>

using namespace std;
using namespace cocos2d;

const std::string g_rankinglayerPath = "rankinglayer/";
const std::string g_collectlayerPath = "collectlayer/";
const std::string g_daojuPath = "daoju/";
const std::string g_seasonlayerPath = "seasonlayer/";
const std::string g_mainlayerPath = "mainlayer/";
const std::string g_buttonPath = "button/";
const std::string g_storelayerPath = "storelayer/";
const std::string g_gameinPath = "gamein/";
const std::string g_achievPath = "achievement/";
const std::string g_loading_logoPath = "loading_logo/";

const std::string g_xiaogk = "xiaogk/";
const std::string g_stop="stop/";
const std::string g_public="public/";
const std::string g_gkjies="gkandjiesuan/";


//人物动画相关
const std::string g_ThiefAnimation_daxiao = "daxiao";
const std::string g_ThiefAnimation_paoyou = "paoyou";
const std::string g_ThiefAnimation_paozuo = "paozuo";
const std::string g_ThiefAnimation_tiaokai = "tiaokai";
const std::string g_ThiefAnimation_yaotou = "yaotou";
const std::string g_ThiefAnimation_zhaoji = "zhaoji";
const std::string g_ThiefAnimation_zuo = "zuo";
const std::string g_ThiefAnimation_zhan = "zhan";
const std::string g_ThiefAnimation_zuoxia = "zuoxia";
const std::string g_ThiefAnimation_siwang = "siwang";
const std::string g_ThiefAnimation_jingxia = "jingxia";
const std::string g_ThiefAnimation_jianqi = "jianqi";
const std::string g_ThiefAnimation_shenlanyao = "shenlanyao";

const int g_iTaiJieHight = 76;

class ResManager
{
public:

	static ResManager* getManager();		//拿到全局的资源管理器

	//清空与语言相关的缓存
	//同时重新加载和语言相关的资源
	void reloadResource();

	//统一获取资源的接口
	//1. filename 子路径+资源名
	//2. 改资源是否是多国版本，  默认 不区分多国。
	string getFilePath(string filename, bool diffrentLanguage = false);   

	//获取阴影资源。
	string getShadowPath(string path);


	//获得当前设备(系统)的语言
	LanguageType getDiviceLanguage();		

	//获取当前系统的
	string getFilePathByLanguageType(string fileName);

	string getFilePathByLanguageType(string fileName, LanguageType eLanguageType);		//获取相对应的语言图片路径

	//设置图片的语言系，默认不设置就是当前设备的语言
	void setLanguageType(LanguageType eLanguageType);
	LanguageType getLanguageType();

	//移除所有的图片缓存(用于重新设置语言时清理原有图片缓存)
	void removeAllTextureCache();
	void removeAllSpriteFrameCache();
	void removeAllAnimationCache();

	void loadAllArmatureInfo();
	void removeAllArmatureInfo();

	//获取共享的文件路径
	string getSharedFilePath();
	string getSharedFilePath(string fileName);

	string getLanguagePathString(LanguageType eLanguageType);
	string getLanguagePathString();

private:
	ResManager();
	~ResManager();

	LanguageType m_eLanguageType;		// 位置，用于标记语言

	//每次字符串被替换后，再在替换过的字符串中进行替换，是第一种，否则是第二种。
	string& replace_all(string& str,const string& old_value,const string& new_value)   
	{   
		while(true)   {   
			string::size_type   pos(0);   
			if(   (pos=str.find(old_value))!=string::npos   )   
				str.replace(pos,old_value.length(),new_value);   
			else   break;   
		}   
		return   str;   
	}   
    string& replace_all_distinct(string&  str,const string& old_value,const   string&   new_value)   
    {   
        for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   {   
            if(   (pos=str.find(old_value,pos))!=string::npos   )   
                str.replace(pos,old_value.length(),new_value);   
            else   break;   
        }   
        return   str;   
    }   
};


#endif //__ResManager_H__