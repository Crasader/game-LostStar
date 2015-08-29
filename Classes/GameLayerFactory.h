#ifndef _GameLayerFactory_H_
#define _GameLayerFactory_H_ 

#include "CommonDefine.h"
#include "GameScenseDefine.h"
#include "StringIntMaper.h"
#include "GameInforEditer.h"
#include <list>
#include <vector>
#include "include_json.h"
 
/**
 *	注意seasonInfor文档里面，为了不引起未知的错误，应该保证，配置信息中，开始的全是有效关卡，并且从seasonId = 1开始，第一个就是seasonId = 1
 *	也就是不要对现在的结构改动。
 *	
 *	对于有效关卡之后面的，可以是任意的，都没有影响了。
 *	所以，如果要临时添加season在第一个有效season之前，那么就不能通过配置，而只能程序另外动态添加，才不会出现问题(比如:title，shadow，lock之类的)
 */
struct ConfigSectionInfo
{
	int iSectionId;
	int	iPefect_Gold;			//该关卡的完美通关金币数
	int iBest_Gold;				//该关卡优良通关金币数
	int iRole_in;				//该关卡角色默认出现的位置	

	ConfigSectionInfo()
	{
		iSectionId = 0;
		iPefect_Gold = 0;
		iBest_Gold = 0;
		iRole_in = 0;
	}
};

struct ConfigSeasonInfo
{
	int iSeasonId;
	int iSectionNum;
	int iDeblockNum;

	ConfigSeasonInfo()
	{
		iSeasonId = 1;
		iSectionNum = 0;
		iDeblockNum = 0;
	}

	std::vector<ConfigSectionInfo> sectionInfoVec;
};

class GameLayerFactory
{
	static GameLayerFactory* gameLayerFactory;

	std::vector< std::vector<GameLayerInfo> > gameLayers;//存放所有的关卡;
	StringIntMaper* maper;

	//add by jiangjie
	std::vector<GameLayerInfo>			 m_newitemLGameLayers;//存放所有新元素教学的关卡信息;
	std::vector<ConfigSeasonInfo>		 m_ConfigSeasonInfoVec;

public:

	void initAllNewitemsGameLayerInfo();

	GameLayerInfo* getNewItemGameLayerinfo(int newitemId);


	const ConfigSeasonInfo* getConfigSeasonInfo(int seasonId);
	const std::vector<ConfigSeasonInfo>* getConfigSeasonInfoVec();

	const std::vector<ConfigSectionInfo>* getConfigSectionInfoVec(int seasonId);
	const ConfigSectionInfo* getConfigSectionInfo(int seasonId, int sectionId);

	std::pair<int, int> getLevelFormat(int iIndex);
	//end
public:
	static GameLayerFactory* getInstance();
	GameLayerInfo* getGameLayer(int season, int section);//从内存中直接获取棋盘，前提是先调用了loadAllChessBoard。;
	GameLayerInfo* getGameLayer();
	std::vector<std::vector<GameLayerInfo> > getAllChessBoards();//得到所有的棋盘;
	bool loadAllGameLayerBySeasonInfo();//载入所有的board存放在内存中;
	bool initGameLayer(GameLayerInfo* gameLayer,int season, int section);//根据season和section来载入一个配置文件，并生成chessBoard;
	bool initGameLayerByFile(const char* fileName,GameLayerInfo* gameLayer); //根据指定的配置文件，载入内存，并生成chessBoard;
	int getConfigSeasonNum();
	int getValidSeasonNum();						// 获得游戏的SeasonNum，因为有些season没有带有关卡，那么就认为不是GameSeason
	// 这里会联查infor.txt 和 config.txt 来确定某个类型的season的个数
	int getConfigSeasonNum(ItemType seasonType);
	//获取每个season的section数（包括隐藏关卡）
	int getAllSectionNumBySeasonId(int seasonId);
	//获取每个season的section数（不包括隐藏关卡）
	int getNomalSectionNumBySeasonId(int seasonId);

	int getLastCreateSeasonId(){return lastSeasonId;}
	int getLastCreateSectionId(){return lastSectionId;}
	//获得所有的Section的数量（包括隐藏关卡）
	int getTotalSectionNum();

	//获得所有的Section的数量（不包括隐藏关卡）
	int getNormalTotalSectionNum();

	//根据section的Id和section的Id，得出该Section的位置，比如第一大关有3个section，那么第二关，sectionId=3时，
	//那么index就等于1*3+3 -1 （index从0开始）
	int getSectionIndex(int seasonId,int sectionId);
	//根据sectionIndex，求出该section位于的seasonId
	int getSeasonIdBySectionIndex(int sectionIndex);
	//根据sectionIndex，求出该section位于的sectionId
	int getSectionIdBySectionIndex(int sectionIndex);
	//根据当前的SectionId和SeasonId，获得下一关的SeasonId,如果没有了，那么还返回-1
	int getNextSeasonId(int seasonId,int sectionId, bool bSecretStage = false);
	//根据当前的SectionId和SeasonId，获得下一关的SectionId,如果没有了，那么还返回-1
	int getNextSectionId(int seasonId,int sectionId, bool bSecretStage = false);
	~GameLayerFactory();
private:

	GameLayerFactory();
	//在得到root[movement]之后调用
	void initMovement(Json::Value movementRoot,GameMovement* movement,float32 oX,float32 oY);
	//在得到root[shapeRotation]之后调用
	void initShapeRotation(Json::Value rotationRoot,GameShapeRotation* rotation,float32 oX,float32 oY);
    
    void initShapeSpringSlide(Json::Value springRoot,GameShapePrismaticSpring* spring,float32 oX,float32 oY);
    
    void initStrobe(Json::Value strobeRoot,GameToolsStrobe* strobe,float32 oX,float32 oY);
    void initEMCannon(Json::Value eMCannonRoot,EMCannon* em,float32 oX,float32 oY);
    
    //初始话GameShape
    void initGameShape(Json::Value shapeRoot,GameShape* gameShape,float32 oX,float32 oY);
    
    GameToolsRope initRope(Json::Value ropeRoot,float32 oX,float32 oY);
    
    //解析锅炉
    GameToolsBoiler initBoiler(Json::Value boilerRoot,float32 oX,float32 oY);
    
    //解析弹簧床
    GameToolsSpringBed initSpringBed(Json::Value bedRoot,float32 oX,float32 oY);
    
	int lastSeasonId,lastSectionId;
	bool initSeasonInfo();//根据配置文件seasonInfo.txt初始化内部的关卡信息，比如知道有几章，每章有几个小节;
};

#endif