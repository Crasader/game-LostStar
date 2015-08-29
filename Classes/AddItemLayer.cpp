#include "AddItemLayer.h"
#include "CustomeSprite.h"

AddItemLayer::AddItemLayer(BaseContentLayer* layer)
{
	mLayer = layer;
	selectedItem = NULL;
	itemVec.clear();
	resMgr = ResManager::getManager();
	initItems();
	sortItems();
}
void AddItemLayer::initItems()
{
	int types[] = {
		//GameShapeType_RedStar60,
		GameShapeType_Bomb24,
		GameShapeType_Bomb48_24,
		GameShapeType_Bomb48,
		GameShapeType_Box48,
		GameShapeType_T48,
		GameShapeType_LocalTimeDecBomb,
		GameShapeType_Bomb48_DarkWhole,
		GameShapeType_Bomb24_DarkWhole,
		GameShapeType_Bomb48_Hard,
	};
	for(int i=0;i<sizeof(types)/sizeof(int);i++)
	{
		CustomeSprite* item = CustomeSprite::create(resMgr->getSharedFilePath(getImageFileName((GameShapeType)types[i])).c_str());
		itemVec.insert(itemVec.end(),item);
		item->setTag(types[i]);
		item->retain();
		addChild(item);
	}
}
void AddItemLayer::sortItems()
{
	float start_pos_x = 60;
	float start_pos_y = 700;
	float delta_y = 20;
	for (size_t i=0;i<itemVec.size();i++)
	{
		//itemVec[i]->setPosition(ccp(start_pos_x,start_pos_y));
		itemVec[i]->stopAllActions();
		itemVec[i]->runAction(CCMoveTo::create(0.2f,ccp(start_pos_x,start_pos_y)));

		start_pos_y -= delta_y;
		start_pos_y -= itemVec[i]->getContentSize().height;
	}
}
CCNode* AddItemLayer::pickItem(CCPoint worldPoint)
{
	for (size_t i=0;i<itemVec.size();i++)
	{
		if (itemVec[i]->boundingBox().containsPoint(convertToNodeSpace(worldPoint)) )
		{
			CCNode* node = itemVec[i];
			return node;
		}
	}
	return NULL;
}
void AddItemLayer::removeItems(CCNode* item)
{
	vector<CCNode*>::iterator it = itemVec.begin();
	for (;it!=itemVec.end();it++)
	{
		if ((*it) == item)
		{
			itemVec.erase(it);
			break;
		}
	}
	sortItems();
}
AddItemLayer::~AddItemLayer()
{
	for (size_t i=0;i<itemVec.size();i++)
		CC_SAFE_RELEASE_NULL(itemVec[i]);
	itemVec.clear();
}
void AddItemLayer::lfClick(CCPoint glPoint)
{
}
void AddItemLayer::lfDoubleClickDown(CCPoint glPoint)
{
}
void AddItemLayer::lfTouchesBegan(CCPoint glPoint)
{
	selectedItem = pickItem(glPoint);
	if (selectedItem)
	{
		selectedItem->setScale(1.4f);
	}
}
void AddItemLayer::lfTouchesMoved(CCPoint glPoint)
{
	if (selectedItem)
	{
		selectedItem->setPosition(glPoint);
	}
}
void AddItemLayer::lfTouchesEnded(CCPoint glPoint)
{
	if (selectedItem)
	{
		CCPoint layerPoint = mLayer->convertToNodeSpace(glPoint);
		selectedItem->setScale(1.0f);
		selectedItem->setPosition(layerPoint);
		GameShape shape;
		shape.type = (GameShapeType)selectedItem->getTag();
		shape.angle = 0;
		shape.restitution = Fixture_Restitution;
		shape.point = b2Vec2(layerPoint.x / PTM_RATIO , layerPoint.y / PTM_RATIO);
		mLayer->createShape(shape);

		removeItems(selectedItem);
		selectedItem->removeFromParentAndCleanup(true);
	}

	selectedItem = NULL;
}

string AddItemLayer::getImageFileName(GameShapeType type)
{
	string name = "";
	switch (type) {
		case GameShapeType_Box48:
			name = "box48.png";
			break;
		case GameShapeType_Box24:
			break;
		case GameShapeType_Box48_24:
			break;
		case GameShapeType_Box72_48:
			break;
		case GameShapeType_Box96_48:
			break;
		case GameShapeType_Box168_24:
			break;
		case GameShapeType_Box192_24:
			break;
		case GameShapeType_Box216_24:
			break;
		case GameShapeType_Box240_24:
			break;
		case GameShapeType_Box264_24:
			break;
		case GameShapeType_Box360_24:
			break;
		case GameShapeType_Cross72:
			break;
		case GameShapeType_Cross48:
			break;
		case GameShapeType_Triangle56:
			break;
		case GameShapeType_RedStar60:
			name = "star60.png";
			break;
		case GameShapeType_GrayStar60:
			break;
		case GameShapeType_Fork24:
			break;
		case GameShapeType_Bomb48:
			name = "bomb48.png";
			break;
		case GameShapeType_Bomb24:
			name = "bomb24.png";
			break;
		case GameShapeType_Bomb48_24:
			name = "bomb48_24.png";
			break;
		case GameShapeType_T72:
			break;
		case GameShapeType_T48:
			name = "T48.png";
			break;
		case GameShapeType_Box72_24:
			break;
		case GameShapeType_Box96_24:
			break;
		case GameShapeType_Box120_24:
			break;
		case GameShapeType_L48:
			break;
		case GameShapeType_L72_48:
			break;
		case GameShapeType_L72_72:
			break;
		case GameShapeType_Circle48:
			break;
		case GameShapeType_Circle24:
			break;
		case GameShapeType_Circle72:
			break;
		case GameShapeType_HalfCircle96:
			break;
		case GameShapeType_HalfCircle48:
			break;
		case GameShapeType_LocalTimeDecBomb:
			name = "timerdec.png";
			break;
		case GameShapeType_LocalTimeDecBomb_24:
			name = "timerdec.png";
			break;
		case GameShapeType_GlobalTimeAscBomb:
			break;
		case GameShapeType_Bomb24_Circle:
			break;
		case GameShapeType_Bomb48_Circle:
			break;
		case GameShapeType_Bomb24_Hard:
			break;
		case GameShapeType_Bomb48_Hard:
			name = "bomb48_no.png";
			break;
		case GameShapeType_Bomb24_DarkWhole://wormhole_bomb_24.png
			name = "wormhole_bomb_24.png";
			break;
		case GameShapeType_Bomb48_DarkWhole:
			name = "wormhole_bomb_48.png";
			break;
		default:
			CCLog("\n警告：未知的Shape类型！");
			break;
	}

	return name;
}