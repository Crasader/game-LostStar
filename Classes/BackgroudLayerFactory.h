//
//  BackgroudLayerFactory.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#ifndef __RedStarFall__BackgroudLayerFactory__
#define __RedStarFall__BackgroudLayerFactory__

#include <iostream>
#include "cocos2d.h"
#include "BaseBackgroundLayer.h"
#include <string>

using namespace cocos2d;

class BackgroudLayerFactory {
private:
	static BaseBackgroundLayer* mLayer;
public:
	static BaseBackgroundLayer* getCurrentBgLayer();
    //根据指定的章id创建对应的背景图层
    static BaseBackgroundLayer* createBgLayer(int seasonId,int seactionId);
};

#endif /* defined(__RedStarFall__BackgroudLayerFactory__) */
