#include "LFStrings.h"
#include "ResManager.h"

//#define  FILE_STRING_ROOT	"config/"
#define  FILE_STRING		(std::string("config/strings"))

static string currentStringFile = FILE_STRING;

map<string,string> LFStrings::data;

bool LFStrings::init()
{
	data.clear();
	//ResManager::getManager()->setLanguageType(kLanguageEnglish);
	currentStringFile = FILE_STRING+"_"+ ResManager::getManager()->getLanguagePathString();
	currentStringFile = currentStringFile.substr(0, currentStringFile.find_last_of("/")) + ".txt";

	Json::Reader reader;
	Json::Value root;
	unsigned long size;
	const char* doc = LFFileUtiles::getLFFileUtiles()->readResourceFile(currentStringFile,&size);
	if (size == 0)
	{
		CCMessageBox("can not find strings.txt", "error");
		CCLog("LFStrings::init ----> file [%s] have no content.",currentStringFile.c_str());
		return true;
	}

	if (doc == NULL)
	{
		CCMessageBox("can not find strings.txt", "error");
		CCLog("LFStrings::init ----> doc is NULL , open Error!");
		return false;
	}

	if (!reader.parse(string(doc,size),root))
	{
		CCMessageBox("LFStrings::init,reader.parse failed! [%s]",currentStringFile.c_str());
		CCLog("LFStrings::init,reader.parse failed! [%s]",currentStringFile.c_str());
		CCLog("%s",doc);
		delete [] doc;
		CCAssert(false,"");
		return false;
	}
	delete [] doc;

	//CCLog(doc);
	Json::Value _strings= root["strings"];
	if (!_strings.isNull() && _strings.isArray())
	{
		int _stringNum = _strings.size();
		for (int i=0;i<_stringNum;i++)
		{
			Json::Value onePair = _strings[i];
			string _key = onePair["key"].asString();
			string _value = onePair["value"].asString();
			data.insert(pair<string, string>(_key,_value));
		}
	}
	CCLog("LFStrings::init success !");
	return true;
}

string LFStrings::getValue(string _key)
{
	map<string, string>::iterator iter;
	//如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器;
	iter = data.find(_key);
	if(iter != data.end())
	{
		return iter->second;
	}
	CCLog("Waring : LFStrings::getValue --> _key[%s] is not found!",_key.c_str());
	return "";
}