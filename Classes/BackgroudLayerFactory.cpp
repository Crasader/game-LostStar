//
//  BackgroudLayerFactory.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#include "BackgroudLayerFactory.h"
#include "BackgroundLayer_City.h"
#include "BackgroundLayer_Desert.h"
#include "BackgroundLayer_Ocean.h"
#include "BackgroundLayer_Forest.h"
#include "BackgroundLayer_OuterSpace.h"
#include "BackgroundLayer_Castle.h"

BaseBackgroundLayer* BackgroudLayerFactory::mLayer;
BaseBackgroundLayer* BackgroudLayerFactory::getCurrentBgLayer()
{
	return mLayer;
}

BaseBackgroundLayer* BackgroudLayerFactory::createBgLayer(int seasonId, int seactionId)
{
    BaseBackgroundLayer* layer = NULL;
//    switch (seasonId) {
//        case 1:
//            layer = new BackgroundLayer_City(seasonId,seactionId);
//            break;
//        case 2:
//            layer = new BackgroundLayer_Desert(seasonId,seactionId);
//            break;
//        case 3:
//            layer = new BackgroundLayer_Ocean(seasonId,seactionId);
//            break;
//        case 4:
//            layer = new BackgroundLayer_Forest(seasonId,seactionId);
//            break;
//        case 5:
//            layer = new BackgroundLayer_OuterSpace(seasonId,seactionId);
//            break;
//        case 6:
//            layer = new BackgroundLayer_Castle(seasonId,seactionId);
//            break;
//        default:
            layer = new BaseBackgroundLayer(seasonId,seactionId);
//            break;
//    }
    
    layer->autorelease();
	mLayer = layer;
    return layer;
}