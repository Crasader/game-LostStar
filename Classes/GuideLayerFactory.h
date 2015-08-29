//
//  BackgroudLayerFactory.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#ifndef __RedStarFall__GuideLayerFactory__
#define __RedStarFall__GuideLayerFactory__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include "BaseGuideLayer.h"

using namespace cocos2d;

class GuideLayerFactory {
private:
	static GuideLayerFactory* mFactory;

	BaseGuideLayer* currentGuideLayer;

	GuideLayerFactory();

	// 正常版本的
	BaseGuideLayer* getGuideLayerNormal(int seasonId,int sectionId);
	// lite版本的(Free)
	BaseGuideLayer* getGuideLayerLite(int seasonId,int sectionId);
public:
	static GuideLayerFactory* getFactory();

	BaseGuideLayer* getGuideLayer(int seasonId,int sectionId);
	//CC

};

#endif 
