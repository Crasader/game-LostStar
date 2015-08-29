#include "Logo.h"
#include "ResManager.h"
#include "SpriteHelper.h"
#include "Music.h"
#include "LevelManager.h"
#include "TexturePreLoad.h"
#include "Starseneui.h"

std::string keyboardClick = "keyboard.wav";

Logo::Logo()
{
	init();
	action = NULL;
    
    Music::sharedMusicPlayer()->loadEffectSound("keyboard.wav");
}
Logo::~Logo()
{
	CC_SAFE_RELEASE(action);
    Music::sharedMusicPlayer()->unLoadEffectSound("keyboard.wav");
    
}
void Logo::showChar(cocos2d::CCObject *charactor)
{
    CCNode* ch = (CCNode*)charactor;
    Music::sharedMusicPlayer()->playEffectSound("keyboard.wav");
    ch->setVisible(true);
}
CCScene* Logo::createLogoScene()
{
	//CCScene* scene = CCScene::create();

	//Logo* logo = createLayer();

	//scene->addChild(logo);

	//不用logo
	CCScene* scene = Starseneui::scene();

	return scene;
}
Logo* Logo::createLayer()
{
	Logo* logo = new Logo();
	logo->autorelease();
	logo->initLogoLayer();
	return logo;
}

void Logo::initLogoLayer()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	logoSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath("logo.png").c_str());
	
	addChild(logoSpr);
	logoSpr->setPosition(ccp(winSize.width/2,winSize.height/2));
    
    ResManager* manager = ResManager::getManager();
    float delayTime = 0.6f;
    //字符的打出效果
    {
        
        float posY = 403;
        float posX = 345;
        float distance = 0;
        float delayDis = 0.2f;
        
        int charNum = 8;
        std::string logoCharsPic[] = {"C.png","A.png","M.png","E.png","L.png","L.png","I.png","A.png"};//CAMELLIA
        for (int i=0; i<charNum; i++)
        {
            CCSprite* charactor = CCSprite::create(manager->getSharedFilePath(logoCharsPic[i]).c_str());
            charactor->setPosition(ccp(posX,posY));
            charactor->setVisible(false);
            addChild(charactor,25);
            
            posX += charactor->getContentSize().width;
            posX += distance;
            
            
            CCDelayTime* delay = CCDelayTime::create(delayTime);
            CCAction* action = SpriteHelper::createAction(delay, SEL_CallFuncO(&Logo::showChar), charactor);
            delayTime += delayDis;
            
            charactor->runAction(action);
        }
        
        {//镭射光线效果，writeLine.png pinkLine.png
            
            
            
            CCProgressTimer* writeP = createProgressTimer(manager->getSharedFilePath("writeline.png").c_str());
            CCProgressTimer* pinkP = createProgressTimer(manager->getSharedFilePath("pinkline.png").c_str());
            writeP->setPercentage(0.0f);
            pinkP->setPercentage(0.0f);
            pinkP->setPosition(ccp(542,443));
            writeP->setPosition(ccp(540,346));
            
            
            CCDelayTime* delay = CCDelayTime::create(delayTime);
            CCProgressTo* progressTo = CCProgressTo::create(1.0f,100.0f);
            CCAction* actionW= CCSequence::create(delay,progressTo,NULL);
            writeP->runAction(actionW);
            
            CCDelayTime* delay2 = CCDelayTime::create( (delayTime+=0.6f) );
            CCProgressTo* progressTo2 = CCProgressTo::create(1.3f,100.0f);delayTime+=1.3f;
            CCAction* action2= CCSequence::create(delay2,progressTo2,NULL);
            pinkP->runAction(action2);
            
            addChild(writeP);
            addChild(pinkP);
            
        }
        
        
        schedule(schedule_selector(Logo::logoEnd), delayTime+0.7f);
    }
    

}

CCProgressTimer* Logo::createProgressTimer(const char *fileName)
{
    CCSprite* sprite = CCSprite::create(fileName);
//
//    CCProgressTimer *progress = CCProgressTimer::create(sprite);
//    progress->setType(type);
    
    CCProgressTimer *progress = CCProgressTimer::create(sprite);
	progress->setType( kCCProgressTimerTypeBar );
//	progress->setVisible(false);
	progress->setMidpoint(ccp(0,0));
	progress->setBarChangeRate(ccp(1, 0));
    
    return progress;
}

void Logo::logoEnd(float time)
{
	unschedule(schedule_selector(Logo::logoEnd));

	//if (GameInforEditer::getGameInfor()->isFirstRunMe())
	//{//第一次启动，则进入初始化的引导画面
	//	CCScene* scene = BaseGuide::createGuideScene("guide",2);
	//	CCDirector::sharedDirector()->replaceScene(scene);
	//}else
	//{//如果不是第一次启动，那么就直接进入主界面
	//	CCScene* scene = MainFrameLayer::createScene();
	//	CCTransitionFade* tran = CCTransitionFade::create(0.5,scene);
	//	CCDirector::sharedDirector()->replaceScene(tran);
	//}

	{//如果不是第一次启动，那么就直接进入主界面
		CCScene* scene = MainFrameLayer::createScene();
		CCTransitionFade* tran = CCTransitionFade::create(0.5,scene);
		CCDirector::sharedDirector()->replaceScene(tran);
	}
}

void Logo::playAction(float time)
{
	unschedule(schedule_selector(Logo::playAction));
	logoSpr->runAction(action);
}