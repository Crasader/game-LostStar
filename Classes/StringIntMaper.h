#ifndef __StringIntMaper_H__
#define __StringIntMaper_H__

#include "GameScenseDefine.h"
#include "CrossPlatformSizeManager.h"
#include <map>
#include <string>

using namespace std;

#define NO_MATCH_DATA -100 //没有匹配的数据;

//
//一个把string代表的int转换成int;
//

class StringIntMaper
{
private:
	static StringIntMaper* maper;
	void initMaper();
	StringIntMaper();
	map<string,int> data;
public:
	~StringIntMaper();

	static StringIntMaper* getMaper();

	//查找key为str的值，没有找到就返回NO_MATCH_DATA;
	int getInt(string str);


	//void test();//测试用的;
};

#endif