#ifndef __LFFileUtiles_H__
#define __LFFileUtiles_H__

#include "cocos2d.h"
#include <string>

using namespace cocos2d;
using namespace std;
//一个自己封装的文件类.
//目前文件分为两种，一个是只读，一个是读写的文件。



class LFFileUtiles
{
private:
	static LFFileUtiles* fileUtiles;
	string resRoot;

	LFFileUtiles();
public:

	static LFFileUtiles* getLFFileUtiles();
	void test();   

	// 得到Resource根目录路径
	const char* getResourceRootPath();

	/**
	 *	判断Resource下面的某个文件是否存在，存在就返回true
	 *	不存在就返回false
	 *	调用方法isResourceFileExist("infor/infor.txt");
	 *	对于iOS，我们会把前缀路径去掉，等同于调用isResourceFileExist("infor.txt");
	 */
	bool isResourceFileExist(string file);

	//读取Resources下面的一个文件：需要自己 delete[] 
	//调用方法：
	//	getResourceFile("kkkk/kkkk/file.text",&s);
	const char* readResourceFile(string file,unsigned long *size);
	string getResourceFile(string fileName,bool& success);
	/*
	读取doc下面的文件。 需要自己 delete[] 
	安卓就是/data/data/[package]/file
	iOS就是/var/mobile/Applications/[应用程序号]/Documents//file

	调用方法：
	getDocFile("kkkk/kkkk/file.text",&s);
	iOS下面会自动子保留file.text
	*/
	const char* readDocFile(string file,unsigned long *size);


	//把文件拷贝到doc下面，win32是不需要用的。用了也没效果
	//如果key 为非空，那么就用key加密
	bool copyFileFromResToDocIfNotExist(string fromFileName,const char* key = NULL,string toFileName="", bool bCheckExist = true);

	//保存到Doc文件：调用方法writeToDocFile("KK/KKK/test.t","axjkankjn");
	bool writeToDocFile(string fileName,string data,const char* model = "w");

	// 得到写doc的全路径
	string getDocFilePath(string relativePath);

	bool writeDataToFile(string data,string fullFileFath,const char* model = "w");
	unsigned char* readFile(const char* fileFullPath,unsigned long * pSize);

	bool isFileExist(string fileFullPath);
	//完整路径的文件拷贝
	bool fileCopy(const char* from,const char* to);

	//指定doc 子目录下 创建一个文件， 如果子目录不存在则创建子目录
	bool createDocFileInNewFolder(string folderName, string fileName);
};



#endif