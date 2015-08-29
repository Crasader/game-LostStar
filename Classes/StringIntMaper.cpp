#include "StringIntMaper.h"

StringIntMaper* StringIntMaper::maper;

StringIntMaper::StringIntMaper()
{
	initMaper();
}

StringIntMaper::~StringIntMaper()
{
	data.clear();
}

void StringIntMaper::initMaper()
{
	//table的枚举;
	data.insert(pair<string, int>("GameTableType_Table144",GameTableType_Table144));
	data.insert(pair<string, int>("GameTableType_Table192",GameTableType_Table192));
	data.insert(pair<string, int>("GameTableType_Table288",GameTableType_Table288));
	data.insert(pair<string, int>("GameTableType_Table336",GameTableType_Table336));

	//shape的枚举;
	data.insert(pair<string, int>("GameShapeType_Box48",GameShapeType_Box48));
	data.insert(pair<string, int>("GameShapeType_Box24",GameShapeType_Box24));
	data.insert(pair<string, int>("GameShapeType_Box48_24",GameShapeType_Box48_24));
	data.insert(pair<string, int>("GameShapeType_Box72_24",GameShapeType_Box72_24));
	data.insert(pair<string, int>("GameShapeType_Box96_24",GameShapeType_Box96_24));
	data.insert(pair<string, int>("GameShapeType_Box120_24",GameShapeType_Box120_24));
	data.insert(pair<string, int>("GameShapeType_Box72_48",GameShapeType_Box72_48));
	data.insert(pair<string, int>("GameShapeType_Box96_48",GameShapeType_Box96_48));
	data.insert(pair<string, int>("GameShapeType_Box168_24",GameShapeType_Box168_24));
	data.insert(pair<string, int>("GameShapeType_Box192_24",GameShapeType_Box192_24));
	data.insert(pair<string, int>("GameShapeType_Box360_24",GameShapeType_Box360_24));
	data.insert(pair<string, int>("GameShapeType_Cross72",GameShapeType_Cross72));
	data.insert(pair<string, int>("GameShapeType_Cross48",GameShapeType_Cross48));
	data.insert(pair<string, int>("GameShapeType_RedStar60",GameShapeType_RedStar60));
	data.insert(pair<string, int>("GameShapeType_GrayStar60",GameShapeType_GrayStar60));
	data.insert(pair<string, int>("GameShapeType_Triangle56",GameShapeType_Triangle56));
	data.insert(pair<string, int>("GameShapeType_Fork24",GameShapeType_Fork24));
	data.insert(pair<string, int>("GameShapeType_Bomb48",GameShapeType_Bomb48));
	data.insert(pair<string, int>("GameShapeType_Bomb24",GameShapeType_Bomb24));
	data.insert(pair<string, int>("GameShapeType_Bomb48_24",GameShapeType_Bomb48_24));
	data.insert(pair<string, int>("GameShapeType_T72",GameShapeType_T72));
	data.insert(pair<string, int>("GameShapeType_T48",GameShapeType_T48));
	data.insert(pair<string, int>("GameShapeType_L48",GameShapeType_L48));
	data.insert(pair<string, int>("GameShapeType_L72_48",GameShapeType_L72_48));
	data.insert(pair<string, int>("GameShapeType_L72_72",GameShapeType_L72_72));
	data.insert(pair<string, int>("GameShapeType_Circle48",GameShapeType_Circle48));
	data.insert(pair<string, int>("GameShapeType_Circle24",GameShapeType_Circle24));
	data.insert(pair<string, int>("GameShapeType_HalfCircle96",GameShapeType_HalfCircle96));
	/*
		GameShapeType_Circle72,		//2012-8-1新加;
		GameShapeType_HalfCircle48,	//2012-8-1新加;
		GameShapeType_Box216_24,	//2012-8-1新加;//9*1
		GameShapeType_Box240_24,	//2012-8-1新加;//10*1
		GameShapeType_Box264_24,	//2012-8-1新加;//11*1
	*/
	data.insert(pair<string, int>("GameShapeType_Circle72",GameShapeType_Circle72));
	data.insert(pair<string, int>("GameShapeType_HalfCircle48",GameShapeType_HalfCircle48));
	data.insert(pair<string, int>("GameShapeType_Box216_24",GameShapeType_Box216_24));
	data.insert(pair<string, int>("GameShapeType_Box240_24",GameShapeType_Box240_24));
	data.insert(pair<string, int>("GameShapeType_Box264_24",GameShapeType_Box264_24));

    data.insert(pair<string, int>("GameShapeType_LocalTimeDecBomb",GameShapeType_LocalTimeDecBomb));
    data.insert(pair<string, int>("GameShapeType_LocalTimeDecBomb_24",GameShapeType_LocalTimeDecBomb_24));
    data.insert(pair<string, int>("GameShapeType_GlobalTimeAscBomb",GameShapeType_GlobalTimeAscBomb));
        
    //旋转的新属性
    data.insert(pair<string, int>("GAME_ROTATION_Type_Resilience",GAME_ROTATION_Type_Resilience));
    data.insert(pair<string, int>("GAME_ROTATION_Type_Free",GAME_ROTATION_Type_Free));
    
    //新炸弹
    data.insert(pair<string, int>("GameShapeType_Bomb24_Circle",GameShapeType_Bomb24_Circle));
    data.insert(pair<string, int>("GameShapeType_Bomb48_Circle",GameShapeType_Bomb48_Circle));
    data.insert(pair<string, int>("GameShapeType_Bomb24_Hard",GameShapeType_Bomb24_Hard));
    data.insert(pair<string, int>("GameShapeType_Bomb48_Hard",GameShapeType_Bomb48_Hard));
    data.insert(pair<string, int>("GameShapeType_Bomb24_DarkWhole",GameShapeType_Bomb24_DarkWhole));
    data.insert(pair<string, int>("GameShapeType_Bomb48_DarkWhole",GameShapeType_Bomb48_DarkWhole));
    
    

}

StringIntMaper* StringIntMaper::getMaper()
{
	if (maper == NULL)
	{
		maper = new StringIntMaper;
	}
	return maper;
}

int StringIntMaper::getInt(string str)
{
	map<string, int>::iterator iter;
	//如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器;
	iter = data.find(str);
	if(iter != data.end())
	{
		return iter->second;
	}
	return NO_MATCH_DATA;
}

//void StringIntMaper::test()
//{
//	int GameShapeType_Box48 = getInt("GameShapeType_Box48");//1
//	int GameShapeType_Bomb48_24 = getInt("GameShapeType_Bomb48_24");//20
//	int GameShapeType_T72____ONON = getInt("GameShapeType_T72____ONON");//-100
//	int GameShapeType_HalfCircle96 = getInt("GameShapeType_HalfCircle96");//28
//
//	int GameTableType_Table144 = getInt("GameTableType_Table144");//1
//	int GameTableType_Table336 = getInt("GameTableType_Table336");//4
//	int GameTableType_Table192 = getInt("GameTableType_Table192");//2
//	int GameTableType_Table144___NONONO = getInt("GameTableType_Table144___NONONO");//-100
//
//
//}
