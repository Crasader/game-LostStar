#ifndef __AddItemLayer_H__
#define __AddItemLayer_H__
#include "LFLayer.h"
#include <vector>
#include <string>
#include "ResManager.h"
#include "BaseContentLayer.h"

// test   开发者使用

using namespace std;

class AddItemLayer : public LFLayer
{
public:
	AddItemLayer(BaseContentLayer* layer);
	~AddItemLayer();

	CCNode* pickItem(CCPoint worldPoint);
protected:
	virtual void lfClick(CCPoint glPoint);
	virtual void lfDoubleClickDown(CCPoint glPoint);
	virtual void lfTouchesBegan(CCPoint glPoint);
	virtual void lfTouchesMoved(CCPoint glPoint);
	virtual void lfTouchesEnded(CCPoint glPoint);

private:
	vector<CCNode*> itemVec;
	ResManager* resMgr;
	BaseContentLayer* mLayer;
	CCNode* selectedItem;

	void initItems();
	void sortItems();
	void removeItems(CCNode* item);
	string getImageFileName(GameShapeType type);
};

#endif