#include "GameLayerFactory.h"
#include "CommonDefine.h"
#include "VersionControl.h"
using namespace std;

#include <string>

GameLayerFactory* GameLayerFactory::gameLayerFactory;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//配置文件的key;
    std::string seasonInfoTxt = "seasonInfo.txt";
	std::string config = "";
#else
	//配置文件的key;
	std::string seasonInfoTxt = "config/seasonInfo.txt";
	std::string config = "config/";
#endif

//#define DUFAULT_DENSITY -1000 //默认的密度,也就是说如果;

//数据常量的定义;
std::string info = "seasonInfo";
std::string sectionNum = "sectionNum";
std::string _seasonId = "seasonId";
std::string deblockNum = "deblockNum";

std::string sections = "sections";
std::string sectionID = "sectionID";
std::string prefect_Gold = "prefect_Gold";
std::string best_Gold = "best_Gold";
std::string Role_in = "Role_in";

std::string table_array = "table_array";
std::string shape_array = "shape_array";
std::string throwPoint_array = "throwPoint_array";
std::string rope_Array = "rope_Array";
std::string boiler_Array = "boiler_Array";
std::string springBed_Array = "springBed_Array";
std::string strobe_Array = "strobe_Array";
std::string emcannon_Array = "emcannon_Array";
std::string drawable_Array = "drawable_Array";
std::string failedWhenRedStarFall = "failedWhenRedStarFall";
std::string needRedStarBeginEffective = "needRedStarBeginEffective";

//评分元素
std::string is_key_item = "is_key_item";
std::string min_click_num = "min_click_num";
std::string min_time_cost = "min_time_cost";

// 初始缩放
std::string init_scale = "init_scale";
std::string init_scale_center_pos_x = "init_scale_center_pos_x";
std::string init_scale_center_pos_y = "init_scale_center_pos_y";


//桌子的数据;
std::string type = "type";
std::string point_x = "point_x";
std::string point_y = "point_y";
std::string angle = "angle";
std::string effective = "effective";//是否有效 1有效，0无效;
//移动属性
std::string movement = "GameMovement";
std::string enable = "enable";
std::string moveStartPoint_x = "moveStartPoint_x";
std::string moveStartPoint_y = "moveStartPoint_y";
std::string moveBackPoint_x = "moveBackPoint_x";
std::string moveBackPoint_y = "moveBackPoint_y";
std::string duration = "duration";

//移动滑轨
std::string elasticForce = "elasticForce";
std::string maxDistacne = "maxDistacne";

//drawableItem
std::string drawableType = "drawableType";
std::string drawableType_Em = "drawableType_Em";

//电磁炮
std::string em_type = "em_type";
std::string closeForBegin = "closeForBegin";//开始是否是关闭的
std::string width_PTM_RATIO_num = "width_PTM_RATIO_num";
std::string isLeft = "isLeft";

//旋转属性
//ShapeRotation
string handleAble = "handleAble";
string stopForBegin = "stopForBegin";//开始是否是停止的
string rotationType = "rotationType";
string shapeRotation = "GameShapeRotation";
string shapePrismaticSpring = "shapePrismaticSpring";
string anchorPoint_x = "anchorPoint_x";
string anchorPoint_y = "anchorPoint_y";
string motorSpeed = "motorSpeed";
string maxMotorTorque = "maxMotorTorque";

//其他形状的数据;
//std::string type = "type";
//std::string point_x = "point_x";
//std::string point_y = "point_y";
//std::string angle = "angle";
std::string canDestroy = "canDestroy";
std::string restitution = "restitution";
std::string density = "density";
std::string isBullet = "isBullet";


//抛掷的形状数据;
//std::string type = "type";
//std::string point_x = "point_x";//就没有用了;
//std::string point_y = "point_y";//就没有用了;
//std::string angle = "angle";
//std::string canDestroy = "canDestroy";
std::string initForce_x = "initForce_x";//被投掷物品的初始受力;b2Vec
std::string initForce_y = "initForce_y";//被投掷物品的初始受力;b2Vec
std::string initTorque = "initTorque";


//抛掷点的数据,就是形状的定义，多了2个参数initForce，initTorque,然后加上投掷的地方和时间;
std::string throwPoint_x = "throwPoint_x";//抛掷点;
std::string throwPoint_y = "throwPoint_y";//抛掷点;
std::string throwShape_array = "throwShape_array";
std::string timeGap = "timeGap";

//绳子的属性
//string anchorPoint_x = "anchorPoint_x";
//string anchorPoint_y = "anchorPoint_y";
string length = "length";
string joinedShape = "joinedShape";//连接的一个shape

//锅炉的属性
//std::string point_x = "point_x";
//std::string point_y = "point_y";


//弹簧床的属性
//std::string point_x = "point_x";
//std::string point_y = "point_y";
//std::string restitution = "restitution";



GameLayerFactory* GameLayerFactory::getInstance()
{
	if(gameLayerFactory == NULL)
	{
		gameLayerFactory = new GameLayerFactory();
	}
	return gameLayerFactory;
}

GameLayerFactory::GameLayerFactory()
{
	maper = StringIntMaper::getMaper();
}
GameLayerFactory::~GameLayerFactory()
{
	gameLayers.clear();
}

//
//载入配置文件，并生成ChessBoard存放在vector中，范围是[seasonStart,seasonEnd],[sectionStart,sectionEnd];
bool GameLayerFactory::loadAllGameLayerBySeasonInfo()
{
	bool ret = initSeasonInfo();
	if (!ret)
	{
		return ret;
	}
	//载入文件，从1_1.txt开始载入;
	for(size_t i=1;i<=m_ConfigSeasonInfoVec.size();i++)
	{
		std::vector<GameLayerInfo> sections;
		// 获得本Season的SectionNum
		int sectionNum = getAllSectionNumBySeasonId(i);
		for (int j=1;j<=sectionNum;j++)
		{
			GameLayerInfo gameLayer;
			ret = initGameLayer(&gameLayer,i,j);//初始化关卡;
			if (!ret)
			{
				CCLog("Error:init game layer seasonId = %d,sectionId = %d",i,j);
				return ret;
			}

			// 测试：初始化配置
			/*
			{
				if (i == 1)
				{
					switch (j)
					{
					case 1:
						gameLayer.initScale = 2.0f;
						gameLayer.initScaleCenterPos = ccp(505,282);
						break;
					case 2:
						gameLayer.initScale = 2.0f;
						gameLayer.initScaleCenterPos = ccp(507,282);
						break;
					case 3:
						gameLayer.initScale = 2.0f;
						gameLayer.initScaleCenterPos = ccp(511,236);
						break;
					case 4:
						gameLayer.initScale = 1.8f;
						gameLayer.initScaleCenterPos = ccp(583,262);
						break;
					case 5:
						gameLayer.initScale = 2.0f;
						gameLayer.initScaleCenterPos = ccp(533,264);
						break;
					case 6:
						gameLayer.initScale = 1.3f;
						gameLayer.initScaleCenterPos = ccp(531,437);
						break;
					case 7:
						gameLayer.initScale = 1.0f;
						gameLayer.initScaleCenterPos = ccp(1,1);
						break;
					case 8:
						gameLayer.initScale = 1.0f;
						gameLayer.initScaleCenterPos = ccp(1,1);
						break;
					case 9:
						gameLayer.initScale = 1.3f;
						gameLayer.initScaleCenterPos = ccp(539,248);
						break;
					case 10:
						gameLayer.initScale = 1.0f;
						gameLayer.initScaleCenterPos = ccp(1,1);
						break;
					case 11:
						gameLayer.initScale = 1.0f;
						gameLayer.initScaleCenterPos = ccp(1,1);
						break;
					case 12:
						gameLayer.initScale = 1.0f;
						gameLayer.initScaleCenterPos = ccp(1,1);
						break;
					case 13:
						gameLayer.initScale = 1.2f;
						gameLayer.initScaleCenterPos = ccp(607,401);
						break;
					case 14:
						gameLayer.initScale = 1.3f;
						gameLayer.initScaleCenterPos = ccp(551,433);
						break;
					case 15:
						gameLayer.initScale = 1.3f;
						gameLayer.initScaleCenterPos = ccp(550,296);
						break;
					case 16:
						gameLayer.initScale = 1.5f;
						gameLayer.initScaleCenterPos = ccp(509,415);
						break;
					case 17:
						gameLayer.initScale = 1.3f;
						gameLayer.initScaleCenterPos = ccp(531,296);
						break;
					case 18:
						gameLayer.initScale = 1.4f;
						gameLayer.initScaleCenterPos = ccp(451,463);
						break;
					case 19:
						gameLayer.initScale = 1.25f;
						gameLayer.initScaleCenterPos = ccp(521,411);
						break;
					case 20:
						gameLayer.initScale = 1.0f;
						gameLayer.initScaleCenterPos = ccp(470,407);
						break;
					default:
						break;
					}
				}
			}
			*/

			sections.insert(sections.end(),gameLayer);
		}
		this->gameLayers.insert(this->gameLayers.end(),sections);
	}

	return ret;
}

void GameLayerFactory::initAllNewitemsGameLayerInfo()
{
// 	int x = 0;
// 	while (true)
// 	{
// 		GameLayerInfo info;
// 		if (initGameLayerByFile(CCString::createWithFormat("%snewitem_%d.txt", config.c_str(), x+1)->getCString(), &info))
// 		{
// 			m_newitemLGameLayers.push_back(info);
// 		}
// 		else
// 		{
// 			return;
// 		}
// 		++x;
// 	}
}

GameLayerInfo* GameLayerFactory::getNewItemGameLayerinfo(int newitemId)
{
	if (newitemId > (int)m_newitemLGameLayers.size() || newitemId <= 0)
	{
		return NULL;
	}
	return &m_newitemLGameLayers.at(newitemId-1);
}

GameLayerInfo* GameLayerFactory::getGameLayer()
{
	return getGameLayer(lastSeasonId,lastSectionId);
}
//
//从缓存中取棋盘,都是从1开始;
GameLayerInfo* GameLayerFactory::getGameLayer(int season, int section)
{
	if(season>(int)m_ConfigSeasonInfoVec.size() || season<1)
	{
		CCLog("%s%d","========Error:season can not biger than total season number(or smaller than 1),current seasonNum = ",m_ConfigSeasonInfoVec.size());
		return NULL;
	}
	int sectionNum = getAllSectionNumBySeasonId(season);
	if (section > sectionNum || section<1)
	{
		CCLog("%s%d","========Error:section can not biger than current season's section number(or smaller than 1),current sectionNum = ",sectionNum);
		return NULL;
	}
	lastSeasonId = season;
	lastSectionId = section;
	return &gameLayers[season-1][section-1];
}

//
//根据配置文件seasonInfo。来初始化信息;
//seasonInfo中有多少个元素就有多少关，关号从1开始，如1,2,3;
//seasonInfo中的sectionNum代表该关有多少小节，也是从1编号;
//那么就就以确定一个关卡文件1_2.txt,1_3.txt等;
bool GameLayerFactory::initSeasonInfo()
{
	Json::Reader reader;
	Json::Value root;
	unsigned long size = 0;

	std::string fullFileName = g_BoundleResourcePath;
	if (fullFileName!="")
	{
		fullFileName += "/";
	}
	fullFileName += seasonInfoTxt;

	CCLog("%s%s","Load seasonInfo->",fullFileName.c_str());
	char* doc = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFileName.c_str(),"r",&size);
	//文件读取失败就返回NULL;
	if (size<=0)
	{
		CCLog("%s%s%s","Load seasonInfo->",fullFileName.c_str(),"fail");
        CCAssert(false, "载入seasonInfo.txt失败!");
		return false;
	}
	//CCLog(doc);
	//解析不成功就返回空;
	if (!reader.parse(doc, root))
	{
		CCLog("%s%s%s","Parse seasonInfo->",fullFileName.c_str(),"fail");
        CCAssert(false, "解析seasonInfo.txt失败!");
		return false;
	}
	Json::Value seasons = root[info];
	for (size_t i = 0; i < seasons.size(); i++)
	{
		ConfigSeasonInfo curSeasonInfo;
		Json::Value tempSeason = seasons[i];
		curSeasonInfo.iSeasonId = tempSeason[_seasonId].asInt();
		curSeasonInfo.iSectionNum = tempSeason[sectionNum].asInt();
		curSeasonInfo.iDeblockNum = tempSeason[deblockNum].asInt();
		
		Json::Value sectionInfos = tempSeason[sections];
		for (size_t j = 0; j < sectionInfos.size(); j++)
		{
			Json::Value tempSection = sectionInfos[j];
			
			ConfigSectionInfo curSectionInfo;
			curSectionInfo.iSectionId = tempSection[sectionID].asInt();
			curSectionInfo.iPefect_Gold = tempSection[prefect_Gold].asInt();
			curSectionInfo.iBest_Gold = tempSection[best_Gold].asInt();
			curSectionInfo.iRole_in = tempSection[Role_in].asInt();
			curSeasonInfo.sectionInfoVec.push_back(curSectionInfo);
		}

		if (curSeasonInfo.sectionInfoVec.size() != curSeasonInfo.iSectionNum)
		{
			CCAssert(false, "seasoninfo.sectionInfoVec.size() != seasoninfo.iSectionNum");
		}
		m_ConfigSeasonInfoVec.push_back(curSeasonInfo);
	}

	delete[] doc;
	return true;
}
//
//根据season和section找到对应的配置文件，然后载入进来初始化chessBoard;
bool GameLayerFactory::initGameLayer(GameLayerInfo* gameLayer,int season, int section)
{
	char fileName[128];
	sprintf(fileName,"%s%d%s%d%s",config.c_str(),season,"_",section,".txt");
	return initGameLayerByFile(fileName,gameLayer);
}
//
//根据文本文件，初始化chessBoard.返回true，则成功，fasle则失败;
bool GameLayerFactory::initGameLayerByFile(const char* fileName,GameLayerInfo* gameLayer)
{
	Json::Reader reader;
	Json::Value root;
	unsigned long size = 0;
	
	std::string fullFileName = g_BoundleResourcePath;
	if (fullFileName!="")
	{
		fullFileName += "/";
	}
	fullFileName += fileName;

	if (!CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(fullFileName.c_str()).c_str()))
	{
		CCLog("%s%s%s","Error:--->Load File->",fullFileName.c_str(),"fail!");
		return false;
	}

	char* doc = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFileName.c_str(),"r",&size);
	//文件读取失败就返回NULL;
	if (size<=0)
	{
		CCLog("%s%s%s","Error:--->Load File->",fullFileName.c_str(),"fail!");
        CCAssert(false, "读取关卡配置文件失败");
		return false;
	}
	//CCLog(doc);
	//解析不成功就返回空;
	if (!reader.parse(doc, root))
	{
		CCLog("%s%s%s","Error:--->Parse File->",fullFileName.c_str()," fail!");
        CCAssert(false, "解析关卡配置文件失败");
		return false;
	}

	Json::Value _tables = root[table_array];
	Json::Value _shapes = root[shape_array];
	Json::Value _throwPoints = root[throwPoint_array];
    Json::Value _ropes = root[rope_Array];
    Json::Value _boiler = root[boiler_Array];
    Json::Value _springBed = root[springBed_Array];
    Json::Value _strobes = root[strobe_Array];
    Json::Value _eMCannons = root[emcannon_Array];
    Json::Value _drawbleItems = root[drawable_Array];

	CCSize gWinSize = CCDirector::sharedDirector()->getWinSize();
    
	float32 oX = gWinSize.width/2/PTM_RATIO;// 就是当前的屏幕中心

	float32 oY = gWinSize.height/10/PTM_RATIO+5.5;// 就是10分之1的下面+5.5个单位  --> 76.8 + 5.5*24 = 208.8 --> 8.7的box2d位置

//注意：有些是默认参数，所以配置文件就可能没有配置，所以要判断是否不为空,目前的做法是要求每个参数都配上;
	//解析failedWhenRedStarFall;
	{
		gameLayer->failedWhenRedStarFall = root[failedWhenRedStarFall].asBool();
	}
    
    //解析failedWhenRedStarFall;
	{
        if (root[needRedStarBeginEffective].isNull()) {
            gameLayer->needRedStarBeginEffective = true;
        }else{
            gameLayer->needRedStarBeginEffective = root[needRedStarBeginEffective].asBool();
        }
	}

	// 解析初始缩放数据
	{
		if (!root[init_scale].isNull() && 
			!root[init_scale_center_pos_x].isNull() && 
			!root[init_scale_center_pos_y].isNull())
		{
			float posx = root[init_scale_center_pos_x].asDouble();
			float posy = root[init_scale_center_pos_y].asDouble();

			gameLayer->initScale = root[init_scale].asDouble();
			gameLayer->initScaleCenterPos = ccp(posx,posy);
		}
	}

	// 分数的部分
	{
		if (root[min_click_num].isNull()) {
			gameLayer->min_click_num = -1;
		}else{
			gameLayer->min_click_num = root[min_click_num].asInt();
		}

		if (root[min_time_cost].isNull()) {
			gameLayer->min_time_cost = -1;
		}else{
			gameLayer->min_time_cost = root[min_time_cost].asDouble();
		}
	}
    

	//解析table_array;
	{
		if (!_tables.isNull() && _tables.isArray())
		{
			int tableNum = _tables.size();
			GameTableType tableType = GameTableType_Table144;
			for (int i=0;i<tableNum;i++)
			{
				Json::Value tempTable = _tables[i];
				if (tempTable[type].isInt())//如果是int型，那么就是以前旧版本的，没有用的;
				{
					tableType = (GameTableType)tempTable[type].asInt();
				}else
				{
					tableType = (GameTableType)maper->getInt(tempTable[type].asString());
				}
                // ipad2 pos = [16,9.26667]
				GameTable table = GameTable(tableType, 
											b2Vec2(oX+tempTable[point_x].asDouble(), oY+tempTable[point_y].asDouble()), 
											tempTable[angle].asDouble(), 
											tempTable[effective].asBool());

				if (!tempTable[movement].isNull())//有移动配置;
				{
					GameMovement move;
					initMovement(tempTable[movement],&move,oX,oY);
					table.setMovement(move);
				}
				if (!tempTable[is_key_item].isNull())
				{
					table.is_key_item = tempTable[is_key_item].asBool();
				}
				gameLayer->gameTableVec.insert(gameLayer->gameTableVec.end(),table);
			}
		}
	}

	//解析shape_array;
	{
		if (!_shapes.isNull() && _shapes.isArray())
		{
			int shapeNum = _shapes.size();
			GameShapeType shapeType = GameShapeType_Bomb24;
			for (int i=0;i<shapeNum;i++)
			{
				Json::Value tempShape = _shapes[i];
                GameShape shape(shapeType,b2Vec2());
                initGameShape(tempShape, &shape, oX, oY);
				gameLayer->gameShapeVec.insert(gameLayer->gameShapeVec.end(),shape);
			}
		}
	}

	//解析throwPoint_array
	{
		if (!_throwPoints.isNull() && _throwPoints.isArray())
		{
			int throwPointNum = _throwPoints.size();
			//解析每一个抛掷点;
			for (int i=0;i<throwPointNum;i++)
			{
				Json::Value tempThrowPoint = _throwPoints[i];
				ThrowPoint throwPoint;
				//解析每一个抛掷点可以抛掷的形状ThrowShape;
				if (!tempThrowPoint[throwShape_array].isNull() && tempThrowPoint[throwShape_array].isArray())
				{
					Json::Value throwShapes = tempThrowPoint[throwShape_array];
					int throwShapeNum = throwShapes.size();
					GameShapeType shapeType = GameShapeType_Bomb24;
					for (int j=0;j<throwShapeNum;j++)
					{
						Json::Value tempThrowShape = throwShapes[j];
						if (tempThrowShape[type].isInt())//如果是int型，那么就是以前旧版本的，没有用的;
						{
							shapeType = (GameShapeType)tempThrowShape[type].asInt();
						}else
						{
							shapeType = (GameShapeType)maper->getInt(tempThrowShape[type].asString());
						}
						ThrowShape throwShape = ThrowShape( shapeType,
															b2Vec2(oX+tempThrowShape[point_x].asDouble(), oY+tempThrowShape[point_y].asDouble()), 
															tempThrowShape[angle].asDouble(), 
															tempThrowShape[canDestroy].asBool());

						throwShape.initForce = b2Vec2(tempThrowShape[initForce_x].asDouble(),tempThrowShape[initForce_y].asDouble());
						throwShape.initTorque = tempThrowShape[initTorque].asDouble();
						throwShape.shape.restitution = tempThrowShape[restitution].asDouble();
						throwShape.shape.isBullet = tempThrowShape[isBullet].asBool();
						if (!tempThrowShape[is_key_item].isNull())
						{
							throwShape.shape.is_key_item = tempThrowShape[is_key_item].asBool();
						}else
						{
							throwShape.shape.is_key_item = false;
						}

                        //配置旋转的属性
                        if (!tempThrowShape[shapeRotation].isNull())
                        {
                            GameShapeRotation rotation;
                            initShapeRotation(tempThrowShape[shapeRotation],&rotation,oX,oY);
                            throwShape.shape.rotation = rotation;
                        }
                        
                        
						//密度的配置;
						float _density = Fixture_Density;
						if (!tempThrowShape[density].isNull())
						{
							_density = tempThrowShape[density].asDouble();
						}
						throwShape.shape.density = _density;

						throwPoint.shapeVec.insert(throwPoint.shapeVec.end(),throwShape);
					}
				}

				throwPoint.point = b2Vec2(oX+tempThrowPoint[throwPoint_x].asDouble(), oY+tempThrowPoint[throwPoint_y].asDouble());
				throwPoint.timeGap = tempThrowPoint[timeGap].asInt();

				gameLayer->throwPointVec.insert(gameLayer->throwPointVec.end(),throwPoint);
			}
		}
	}
	
    //解析绳子
    {
        if (!_ropes.isNull() && _ropes.isArray())
		{
			int ropeNum = _ropes.size();
			
			for (int i=0;i<ropeNum;i++)
			{
				Json::Value tempRope = _ropes[i];
                GameToolsRope rope = initRope(tempRope, oX, oY);
                
                gameLayer->ropeVec.insert(gameLayer->ropeVec.end(), rope);
			}
		}
    }
    
    //解析锅炉
    {
        if (!_boiler.isNull() && _boiler.isArray())
		{
			int boilerNum = _boiler.size();
			
			for (int i=0;i<boilerNum;i++)
			{
				Json::Value tempBoiler = _boiler[i];
                GameToolsBoiler boiler = initBoiler(tempBoiler, oX, oY);
                gameLayer->boilerVec.insert(gameLayer->boilerVec.end(),boiler);
			}
		}
        
    }
    
    //解析弹簧床
    {
        if (!_springBed.isNull() && _springBed.isArray())
		{
			int bedNum = _springBed.size();
			
			for (int i=0;i<bedNum;i++)
			{
				Json::Value tempBed = _springBed[i];
                GameToolsSpringBed bed = initSpringBed(tempBed, oX, oY);
                gameLayer->springBedVec.insert(gameLayer->springBedVec.end(), bed);
                
                //测试代码，添加一个桌子
//                {
//                    GameTable table(GameTableType_Table144, b2Vec2(bed.point.x, bed.point.y-1), 0, false);
//                    gameLayer->gameTableVec.insert(gameLayer->gameTableVec.end(),table);
//                }
			}
		}
    }
    
    //解析闸门
    {
        if (!_strobes.isNull() && _strobes.isArray())
		{
			int strobeNum = _strobes.size();
			
			for (int i=0;i<strobeNum;i++)
			{
				Json::Value tempStrobe = _strobes[i];
                
                GameToolsStrobe strobe;
                initStrobe(tempStrobe, &strobe, oX, oY);
                gameLayer->strobeVec.insert(gameLayer->strobeVec.end(), strobe);
			}
		}
    }
    
    //解析Drawable元素
    {
        if (!_drawbleItems.isNull() && _drawbleItems.isArray())
		{
			int drawableNum = _drawbleItems.size();
			
			for (int i=0;i<drawableNum;i++)
			{
				Json::Value tempEm = _drawbleItems[i];
                std::string _drawableType = tempEm[drawableType].asString();
                //电磁炮
                if (_drawableType == drawableType_Em) {
                    EMCannon em;
                    initEMCannon(tempEm, &em, oX, oY);
                    gameLayer->emCannonVec.insert(gameLayer->emCannonVec.end(), em);
                }else{
                    CCLog("\n警告：------>当前drawable类型[%s]不支持解析!",_drawableType.c_str());
                }
               
			}
		}
        
    }
    
	//释放doc配置文档;
	delete [] doc;
	return true;
}

void GameLayerFactory::initGameShape(Json::Value shapeRoot, GameShape *gameShape, float32 oX, float32 oY)
{
    GameShapeType shapeType = GameShapeType_Bomb24;
    
    Json::Value tempShape = shapeRoot;
    if (tempShape[type].isInt())//如果是int型，那么就是以前旧版本的，没有用的;
    {
        shapeType = (GameShapeType)tempShape[type].asInt();
    }else
    {
        int iType = maper->getInt(tempShape[type].asString());
        if (NO_MATCH_DATA == iType) {
            CCAssert(false, "无法识别该Type类型，也许Map中没有这个字符串的对应的映射");
        }
        shapeType = (GameShapeType)iType;
    }
    
    //密度的配置;
    float _density = Fixture_Density;
    if (!tempShape[density].isNull())
    {
        _density = tempShape[density].asDouble();
    }
    GameShapeRotation rotation;
    if (!tempShape[shapeRotation].isNull())
    {
        initShapeRotation(tempShape[shapeRotation],&rotation,oX,oY);
    }
    
    GameShapePrismaticSpring spring;
    
    if (!tempShape[shapePrismaticSpring].isNull()) {
        initShapeSpringSlide(tempShape[shapePrismaticSpring], &spring, oX, oY);
    }
    
    if (spring.enable&&rotation.enable) {
     
        CCAssert(false, "当前配置文件错误！不应该同时开启旋转和弹性滑轨");
    }
    
	if (!tempShape[is_key_item].isNull())
	{
		gameShape->is_key_item = tempShape[is_key_item].asBool();
	}
    gameShape->type = shapeType;
    gameShape->angle = tempShape[angle].asDouble();
    gameShape->canDestroy = tempShape[canDestroy].asBool();
    gameShape->point = b2Vec2(oX+tempShape[point_x].asDouble(),oY+tempShape[point_y].asDouble());
    gameShape->restitution = tempShape[restitution].asDouble();
    gameShape->density = _density;
    gameShape->isBullet = tempShape[isBullet].asBool();
    gameShape->rotation = rotation;
    gameShape->spring = spring;
    
}

GameToolsRope GameLayerFactory::initRope(Json::Value ropeRoot, float32 oX, float32 oY)
{
    
    GameShape shape(GameShapeType_Bomb24,b2Vec2());
    GameToolsRope rope(b2Vec2(),0,shape);
    
    rope.ropeLength =  ropeRoot[length].asDouble();
    rope.anchorPoint = b2Vec2(oX+ropeRoot[anchorPoint_x].asDouble(),
                              oY+ropeRoot[anchorPoint_y].asDouble());
    
	if (!ropeRoot[is_key_item].isNull())
		rope.is_key_item = ropeRoot[is_key_item].asBool();

    
    initGameShape(ropeRoot[joinedShape], &shape, oX, oY);
    
    rope.shapeToHang = shape;
    
    return rope;
}

GameToolsBoiler GameLayerFactory::initBoiler(Json::Value boilerRoot,float32 oX,float32 oY)
{
    GameToolsBoiler boiler;
    boiler.point = b2Vec2(oX+boilerRoot[point_x].asDouble(),
                          oY+boilerRoot[point_y].asDouble());
    
	if (!boilerRoot[is_key_item].isNull())
		boiler.is_key_item = boilerRoot[is_key_item].asBool();

    return boiler;
}
GameToolsSpringBed GameLayerFactory::initSpringBed(Json::Value bedRoot, float32 oX, float32 oY)
{
    GameToolsSpringBed bed;
    bed.point = b2Vec2(oX+bedRoot[point_x].asDouble(),
                       oY+bedRoot[point_y].asDouble());
    bed.restitution = bedRoot[restitution].asDouble();
    
    return bed;
}


//movementRoot就是tempTable[movement]
void GameLayerFactory::initMovement(Json::Value movementRoot,GameMovement* movement,float32 oX,float32 oY)
{
	Json::Value _movement = movementRoot;
	movement->enable = _movement[enable].asBool();
	movement->moveStartPoint = b2Vec2(oX+_movement[moveStartPoint_x].asDouble(),
                                         oY+_movement[moveStartPoint_y].asDouble());
	movement->moveBackPoint = b2Vec2(oX+_movement[moveBackPoint_x].asDouble(),
                                        oY+_movement[moveBackPoint_y].asDouble());
	movement->duration = _movement[duration].asDouble();

	/**
	"GameMovement": {
	"enable":true,
	"moveStartPoint_x":8.0,
	"moveStartPoint_y":56.9
	"moveBackPoint_x":65.9
	"moveBackPoint_y":66.7
	"duration":7.7
	}
	*/
}

//在得到root[shapeRotation]之后调用
void GameLayerFactory::initShapeRotation(Json::Value rotationRoot,GameShapeRotation* rotation,
                                         float32 oX,float32 oY)
{
//	rotation->anchorPosition = b2Vec2(oX+rotationRoot[anchorPosition_x].asDouble(),
//                                      oY+rotationRoot[anchorPosition_y].asDouble());
    
    rotation->anchorPosition = b2Vec2(rotationRoot[anchorPoint_x].asDouble(),
                                      rotationRoot[anchorPoint_y].asDouble());
    
    if (!rotationRoot[rotationType].isNull()) {
        rotation->rotationType = maper->getInt(rotationRoot[rotationType].asString()) ;
    }
    
    if (!rotationRoot[stopForBegin].isNull()) {
        rotation->stopForBegin = rotationRoot[stopForBegin].asBool();
    }

	rotation->enable = rotationRoot[enable].asBool();
    //默认旋转不可控
    if (!rotationRoot[handleAble].isNull()) {
        rotation->handleAble = rotationRoot[handleAble].asBool();
    }
	rotation->motorSpeed = rotationRoot[motorSpeed].asDouble();
	rotation->maxMotorTorque = rotationRoot[maxMotorTorque].asDouble();
    
	GameMovement move;
	initMovement(rotationRoot[movement],&move,oX,oY);
	rotation->move = move;
}

void GameLayerFactory::initShapeSpringSlide(Json::Value springRoot, GameShapePrismaticSpring *spring, float32 oX, float32 oY)
{
    spring->anchorPosition = b2Vec2(springRoot[anchorPoint_x].asDouble(),
                                      springRoot[anchorPoint_y].asDouble());
    
    
	spring->enable = springRoot[enable].asBool();
    spring->angle = springRoot[angle].asDouble();
    spring->elasticForce = springRoot[elasticForce].asDouble();
//    spring->maxDistacne = springRoot[maxDistacne].asDouble();
}

//在得到root[strobe_Array][i]之后调用
void GameLayerFactory::initStrobe(Json::Value strobeRoot, GameToolsStrobe *strobe, float32 oX, float32 oY)
{
    strobe->angle = strobeRoot[angle].asDouble();
    strobe->point = b2Vec2(strobeRoot[point_x].asDouble()+oX,strobeRoot[point_y].asDouble()+oY);

	if (!strobeRoot[is_key_item].isNull())
		strobe->is_key_item = strobeRoot[is_key_item].asBool();
}
//emcannon_Array
void GameLayerFactory::initEMCannon(Json::Value eMCannonRoot, EMCannon *em, float32 oX, float32 oY)
{
    em->em_type = eMCannonRoot[em_type].asString();
    
    if (!eMCannonRoot[closeForBegin].isNull()) {
        em->closeForBegin = eMCannonRoot[closeForBegin].asBool();
    }
    
    em->width_PTM_RATIO_num = eMCannonRoot[width_PTM_RATIO_num].asInt();
    
    em->isLeft =eMCannonRoot[isLeft].asBool();
    
    em->angle = eMCannonRoot[angle].asDouble();
    
    em->pos = b2Vec2(eMCannonRoot[point_x].asDouble()+oX,eMCannonRoot[point_y].asDouble()+oY);
}


int GameLayerFactory::getConfigSeasonNum()
{
	return m_ConfigSeasonInfoVec.size();
}

int GameLayerFactory::getValidSeasonNum()
{
#if Show_End_BigGuanQia != 0
	return getConfigSeasonNum();
#endif
	return getConfigSeasonNum()-1;
}

int GameLayerFactory::getConfigSeasonNum(ItemType seasonType)
{
	int ret = getConfigSeasonNum();
	list<SeasonInfor*> tempList = GameInforEditer::getGameInfor()->getSessonInforList();
	list<SeasonInfor*>::iterator it = tempList.begin();
	for (;it!=tempList.end();it++)
	{
		if ( (*it)->seasonType != seasonType)
		{
			ret --;
		}
	}
	return ret;
}

int GameLayerFactory::getAllSectionNumBySeasonId(int seasonId)
{
	for (size_t i=0;i<m_ConfigSeasonInfoVec.size();i++)
	{
		if (m_ConfigSeasonInfoVec.at(i).iSeasonId == seasonId)
		{
			return m_ConfigSeasonInfoVec.at(i).iSectionNum;
		}
	}
	return 0;
}

int GameLayerFactory::getNomalSectionNumBySeasonId(int seasonId)
{
	for (size_t i=0;i<m_ConfigSeasonInfoVec.size();i++)
	{
		if (m_ConfigSeasonInfoVec.at(i).iSeasonId == seasonId)
		{
#if Version_Free != 0
			return  m_ConfigSeasonInfoVec.at(i).iSectionNum;
#endif
			return m_ConfigSeasonInfoVec.at(i).iSectionNum - 4;
		}
	}
	return 0;
}

int GameLayerFactory::getSectionIndex(int seasonId,int sectionId)
{
	int index = 0;
	//得出从seasonId = 1，到seasonId之前一共有多少个section
	for (int i=1;i<seasonId;i++)
	{
		index += getAllSectionNumBySeasonId(i);
	}
	index += sectionId;

	index--;


	return index;
}

int GameLayerFactory::getSeasonIdBySectionIndex(int sectionIndex)
{
	int seasonNum = getConfigSeasonNum();

	int sectionNum = sectionIndex+1;
	int id = 1;
	for (id = 1;id<=seasonNum;id++)
	{
		sectionNum -= getAllSectionNumBySeasonId(id);//index
		if (sectionNum <= 0)
		{
			return id;
		}
	}
	return id;
}

int GameLayerFactory::getSectionIdBySectionIndex(int sectionIndex)
{
	int seasonNum = getConfigSeasonNum();
	int sectionNum = (sectionIndex+1);
	for (int id=1;id<=seasonNum;id++)
	{
		sectionNum -= getAllSectionNumBySeasonId(id);//index
		if (sectionNum <= 0)
		{
			//返回Id
			return (sectionNum + getAllSectionNumBySeasonId(id));
		}
	}

	return sectionNum;
}

int GameLayerFactory::getTotalSectionNum()
{
	int num = 0;
	for (size_t seasonId = 1; seasonId <= m_ConfigSeasonInfoVec.size(); seasonId++)
	{
		num += m_ConfigSeasonInfoVec.at(seasonId-1).iSectionNum;
	}
	return num;
}

int GameLayerFactory::getNormalTotalSectionNum()
{
	int num = 0;
	for (size_t seasonId = 1; seasonId <= m_ConfigSeasonInfoVec.size(); seasonId++)
	{
		num += m_ConfigSeasonInfoVec.at(seasonId-1).iSectionNum - 4;
	}
	return num;
}

int GameLayerFactory::getNextSeasonId(int seasonId,int sectionId, bool bSecretStage/* = false*/)
{
	int nextSeasonId = seasonId;
	int sctioncount = getNomalSectionNumBySeasonId(seasonId);
	if (bSecretStage)
	{
		sctioncount = getAllSectionNumBySeasonId(seasonId);
	}
	if (sectionId >= sctioncount)
	{//如果这一节是本章的最后一节，那么就返回下一章
		nextSeasonId++;
	}

	if (nextSeasonId > getConfigSeasonNum())
	{//如果下一节的章超出了范围，那么就返回-1;
		nextSeasonId = -1;
	}

	return nextSeasonId;
}
int GameLayerFactory::getNextSectionId(int seasonId,int sectionId, bool bSecretStage/* = false*/)
{
	int nextSectionId = sectionId+1;
	if (getNextSeasonId(seasonId,sectionId, bSecretStage) != -1)
	{//如果下一章有效

		int sctioncount = getNomalSectionNumBySeasonId(seasonId);
		if (bSecretStage)
		{
			sctioncount = getAllSectionNumBySeasonId(seasonId);
		}
		if (nextSectionId > sctioncount)
		{
			nextSectionId = 1;
		}
	}else
	{//下一章无效，那么本章节就是最后一章的最后一关;
		nextSectionId = -1;
	}
	return nextSectionId;
}

const std::vector<ConfigSeasonInfo>* GameLayerFactory::getConfigSeasonInfoVec()
{
	return &m_ConfigSeasonInfoVec;
}

const ConfigSeasonInfo* GameLayerFactory::getConfigSeasonInfo(int seasonId)
{
	for (size_t i = 0; i < m_ConfigSeasonInfoVec.size(); i++)
	{
		if (seasonId == m_ConfigSeasonInfoVec.at(i).iSeasonId)
		{
			return &m_ConfigSeasonInfoVec.at(i);
		}
	}
	CCAssert(false, "not find ConfigSectionInfo");
	return NULL;
}

const std::vector<ConfigSectionInfo>* GameLayerFactory::getConfigSectionInfoVec(int seasonId)
{
	for (size_t i = 0; i < m_ConfigSeasonInfoVec.size(); i++)
	{
		if (seasonId == m_ConfigSeasonInfoVec.at(i).iSeasonId)
		{
			return &m_ConfigSeasonInfoVec.at(i).sectionInfoVec;
		}
	}
	CCAssert(false, "not find ConfigSectionInfoVec");
	return NULL;
}

const ConfigSectionInfo* GameLayerFactory::getConfigSectionInfo(int seasonId, int sectionId)
{
	for (size_t i = 0; i < m_ConfigSeasonInfoVec.size(); i++)
	{
		if (seasonId == m_ConfigSeasonInfoVec.at(i).iSeasonId)
		{
			for (size_t j = 0; j < m_ConfigSeasonInfoVec.at(i).sectionInfoVec.size(); j++)
			{
				if (m_ConfigSeasonInfoVec.at(i).sectionInfoVec.at(j).iSectionId == sectionId)
				{
					return &m_ConfigSeasonInfoVec.at(i).sectionInfoVec.at(j);
				}
			}
		}
	}
	CCAssert(false, "not find ConfigSectionInfo");
	return NULL;
}

std::pair<int, int> GameLayerFactory::getLevelFormat(int iIndex)
{
	int sectionnum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(1);
	++iIndex;
	int level = iIndex/sectionnum;
	int index = iIndex%sectionnum;
	if (level == 0 && index == 0)
	{
		level = level+1;
		index = sectionnum;
	}
	else if (index == 0)
	{
		level = level;
		index = sectionnum;
	}
	else
	{
		++level;
	}

	return std::pair<int, int>(level, index);
}