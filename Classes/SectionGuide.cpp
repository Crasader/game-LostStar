#include "SectionGuide.h"
#include "ResManager.h"
#include "SpriteHelper.h"

SectionGuide::SectionGuide(BaseContentLayer* baseContentLayer,int seasonId,int sectionId)
{
	mBaseContentLayer = baseContentLayer;
	redStartPrompt = NULL;
	redStartToTableArrows = NULL;
	boxClickPrompt = NULL;
	bombClickPrompt = NULL;
	watingPrompt = NULL;
	forbidenPrompt = NULL;
    shine = NULL;
    clickAction = NULL;
	int z = 5;
	isGuide = false;
	isUserDoEffectiveClickAffterGuide = false;

    winSize = CCDirector::sharedDirector()->getWinSize();
    center = CCPointMake(winSize.width/2, winSize.height/2);
    
    
    
	{//就针对第一章，第一关的教学配置
		if (seasonId == 1 && sectionId == 1)
		{//第一章，第一关的指导：目标的教学
			//添加红星的提示
			baseContentLayer->addChild(redStartPrompt = createRedStartPrompt(),z);
			//添加箭头指示
			baseContentLayer->addChild(redStartToTableArrows = createRedStartToTableArrows(),z);
			//添加点击方块的提示，就是手指
			baseContentLayer->addChild(boxClickPrompt = createBoxClickPrompt(),z);
            
            clickAction = createClickAct_retain();
		}
	}

	{//针对某一关需要加等待提示的地方
		if (seasonId == WAITING_PROMPT_SEASON_ID || WAITING_PROMPT_SEASON_ID == ALL)
		{
			if (sectionId == WAITING_PROMPT_SECTION_ID || WAITING_PROMPT_SECTION_ID == ALL)
			{
				baseContentLayer->addChild(watingPrompt = createWatingPrompt(),z);
                baseContentLayer->addChild(shine = createShine(),z);
			}
		}
	}
	
	{//添加不可消除的提示
		if (seasonId == CAN_NOT_DESTORY_PROMPT_SEASON_ID || CAN_NOT_DESTORY_PROMPT_SEASON_ID == ALL)
		{
			if (sectionId == CAN_NOT_DESTORY_PROMPT_SECTION_ID || CAN_NOT_DESTORY_PROMPT_SECTION_ID == ALL)
			{
				baseContentLayer->addChild(forbidenPrompt = createCanNotDestoryPrompt(),z);
			}
		}
	}

	{//添加炸弹提示
		if (seasonId == BOMB_PROMPT_SEASON_ID || BOMB_PROMPT_SEASON_ID == ALL)
		{
			if (sectionId == BOMB_PROMPT_SECTION_ID || BOMB_PROMPT_SECTION_ID == ALL)
			{
				baseContentLayer->addChild(bombClickPrompt = createBombClickPrompt(),z);
                clickAction = createClickAct_retain();
			}
		}
	}
	
}
SectionGuide::~SectionGuide()
{
    CC_SAFE_RELEASE(clickAction);
}

void SectionGuide::baseContenLayerClikced(CCPoint glClickPoint)
{
}
//只要是点击到了Shape都要回调
void SectionGuide::baseContenLayerEffectiveClicked(CCPoint glClickPoint,CCNode* clikedItem)
{
	hideInitPrompt();
	isUserDoEffectiveClickAffterGuide = true;
	if (clikedItem->getTag() == Tag_Box_can_Destroied)
	{//是可以点击的项目

		if (mBaseContentLayer->isWatting())
		{//如果处于等待状态，那么显示等待提示
			showWatingPrompt(mBaseContentLayer->getWatingtPos());
		}

	}else if (clikedItem->getTag() == Tag_Box_cannot_Destroied)
	{//不可点击的项目
		showForbidenPrompt(clikedItem->getPosition());
	}
}

void SectionGuide::showInitGuide()
{
	isGuide = true;
	isUserDoEffectiveClickAffterGuide = false;
	CCPoint bombClickPos = CCPointMake(mBaseContentLayer->getBombPos().x+30,mBaseContentLayer->getBombPos().y-33);
	CCPoint arrowsPos = CCPointMake(mBaseContentLayer->getRedStartPos().x-60,mBaseContentLayer->getRedStartPos().y-60);
	CCPoint boxPos = CCPointMake(542,245);
	//第一关就显示第一关的所有指示
	//获得需要提示Box的位置
	bool show1 = showFirstSectionGuide(mBaseContentLayer->getRedStartPos(),boxPos,arrowsPos);
	//有炸弹就提示炸弹
	bool show2 = showBombClickPrompt(bombClickPos);

	if ( (show1 || show2) == false)
	{//如果都没有显示，那么直接没有Guide
		isGuide = false;
	}
}

CCProgressTimer* SectionGuide::createRedStartPrompt()
{
	CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("circle.png").c_str());
	CCProgressTimer *progress = CCProgressTimer::create(sprite);
	progress->setType( kCCProgressTimerTypeRadial );
	progress->setVisible(false);
	return progress;
}

CCProgressTimer* SectionGuide::createRedStartToTableArrows()
{
	CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("arrows.png").c_str());
	CCProgressTimer *progress = CCProgressTimer::create(sprite);
	progress->setType( kCCProgressTimerTypeBar );
	progress->setVisible(false);
	progress->setMidpoint(ccp(0,1));
	progress->setBarChangeRate(ccp(0, 1));
	return progress;
}

CCSprite* SectionGuide::createBombClickPrompt()
{
//	CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("finger-Press.png").c_str());
    CCSprite* sprite = new CCSprite();
    sprite->init();
	sprite->setVisible(false);
	return sprite;
}

CCSprite* SectionGuide::createBoxClickPrompt()
{
//	CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("finger-Press.png").c_str());
	CCSprite* sprite = new CCSprite();
    sprite->init();
    sprite->setVisible(false);
	return sprite;
}

CCSprite* SectionGuide::createWatingPrompt()
{
	CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("waitingPrompt.png").c_str());
	sprite->setVisible(false);
	return sprite;
}

CCSprite* SectionGuide::createCanNotDestoryPrompt()
{
	CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("forbiden.png").c_str());
	sprite->setVisible(false);
	return sprite;
}

CCSprite* SectionGuide::createShine()
{
    CCSprite* sprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("shine.png").c_str());
	sprite->setVisible(false);
	return sprite;
}

CCAction* SectionGuide::createClickAct_retain()
{
    string frames[] = {
        ResManager::getManager()->getSharedFilePath("finger.png"),
        ResManager::getManager()->getSharedFilePath("finger-Press.png"),
        ResManager::getManager()->getSharedFilePath("finger.png"),
        ResManager::getManager()->getSharedFilePath("finger-Press.png"),
        ResManager::getManager()->getSharedFilePath("finger.png")};
    CCAction* act = (CCFiniteTimeAction*)SpriteHelper::createAction(frames, 5, 0.5f,false);
    act->retain();
    return act;
}


bool SectionGuide::showFirstSectionGuide(CCPoint redStarPos,CCPoint clickBoxPos,CCPoint arrowsPos)
{
	if (redStartPrompt!=NULL && boxClickPrompt!=NULL && redStartToTableArrows!=NULL && clickAction!= NULL)
	{
		redStartPrompt->setPosition(redStarPos);
		redStartToTableArrows->setPosition(arrowsPos);
		boxClickPrompt->setPosition(clickBoxPos);

		redStartPrompt->setPercentage(0.0f);
		redStartPrompt->setVisible(true);

        float delay = 0.8f;
        float clickTimeLast = 0.5f*5;
        
        CCAction* actionTouch = createAction(CCDelayTime::create(delay+clickTimeLast),
                                        SEL_CallFuncO(&SectionGuide::boxClickPromptFinishedShow),
                                        this);

        
        boxClickPrompt->stopAllActions();
        boxClickPrompt->init();
        boxClickPrompt->runAction(actionTouch);
        
        schedule(schedule_selector(SectionGuide::timeToShowFinger),delay);
		return true;
	}
	return false;
}
//手指的标示显示完毕，那么显示隐藏掉，可以开始点击操作了
void SectionGuide::boxClickPromptFinishedShow(CCObject* guide)
{
	SectionGuide* _guide = (SectionGuide*)guide;
//	_guide->boxClickPrompt->stopAllActions();
    
    CCAction* action = createAction(CCProgressTo::create(0.5f,100.0f),SEL_CallFuncO(&SectionGuide::redStartPromptFinishedShow),_guide);
    _guide->boxClickPrompt->setVisible(true);
    
//    _guide->boxClickPrompt->initWithFile(ResManager::getManager()->getSharedFilePath("finger-Press.png").c_str());
    
    _guide->redStartPrompt->runAction(action);
}
//红星的标示显示完毕，那么显示箭头的
void SectionGuide::redStartPromptFinishedShow(CCObject* guide)
{
	SectionGuide* _guide = (SectionGuide*)guide;
	_guide->redStartPrompt->stopAllActions();


	CCAction* action = createAction(CCProgressTo::create(0.5f,100.0f),SEL_CallFuncO(&SectionGuide::arrowsPromptFinishedShow),guide);
	_guide->redStartToTableArrows->setVisible(true);
	_guide->redStartToTableArrows->setPercentage(0.0f);
	_guide->redStartToTableArrows->runAction(action);
}
//箭头的标示显示完毕，那么显示手指点击
void SectionGuide::arrowsPromptFinishedShow(CCObject* guide)
{
	SectionGuide* _guide = (SectionGuide*)guide;
	_guide->redStartToTableArrows->stopAllActions();


    _guide->schedule(schedule_selector(SectionGuide::hideGuide),0.5f);
	
}

void SectionGuide::hideGuide(float time)
{
    unschedule(schedule_selector(SectionGuide::hideGuide));
    
    redStartToTableArrows->setVisible(false);
	redStartPrompt->setVisible(false);
	boxClickPrompt->setVisible(false);
    
    
    isGuide = false;
    
	//开始下次的显示
	schedule(schedule_selector(SectionGuide::showInitGuideAgain),GUIDE_GAP);
}

void SectionGuide::showInitGuideAgain(float time)
{
	unschedule(schedule_selector(SectionGuide::showInitGuideAgain));
	if (!isUserDoEffectiveClickAffterGuide)
	{
		showInitGuide();
	}
}


//闪烁的显示不可点击
void SectionGuide::showForbidenPrompt(CCPoint pos)
{
	if (forbidenPrompt != NULL)
	{

		CCAction* action = createAction(CCBlink::create(0.5,3),
										SEL_CallFuncO(&SectionGuide::hide),
										forbidenPrompt);

		forbidenPrompt->setVisible(true);
		forbidenPrompt->setPosition(pos);
		forbidenPrompt->runAction(action);
	}
}
//闪烁的显示等待
void SectionGuide::showWatingPrompt(CCPoint timmerPos)
{
	if (watingPrompt != NULL)
	{
		CCAction* action = createAction(CCBlink::create(0.5,6),
			SEL_CallFuncO(&SectionGuide::hide),
			watingPrompt);
		watingPrompt->setVisible(true);
		watingPrompt->setPosition(timmerPos);
		watingPrompt->runAction(action);
        shine->setVisible(true);
        CCAction* shineAct = createAction(CCDelayTime::create(0.5f),
                                          SEL_CallFuncO(&SectionGuide::hide), shine);
        shine->setPosition(center);
        shine->runAction(shineAct);

		//CCNode* timer = mBaseContentLayer->getTimer();
		//CCPoint timerPosCopy = timer->getPosition();
		//CCPoint startPosH = CCPointMake(timerPosCopy.x-10,timerPosCopy.y);
		//CCPoint endPosH = CCPointMake(timerPosCopy.x+10,timerPosCopy.y);
		//CCPoint startPosV = CCPointMake(timerPosCopy.x,timerPosCopy.y-10);
		//CCPoint endPosV = CCPointMake(timerPosCopy.x,timerPosCopy.y+10);
		////CCScaleTo* scaleTo = CCScaleTo::create(0.1f,1.5f);
		//CCMoveTo* moveTo1 = CCMoveTo::create(0.06f,startPosH);
		//CCMoveTo* moveTo2 = CCMoveTo::create(0.06f,endPosH);
		////CCMoveTo* moveTo3 = CCMoveTo::create(0.06f,startPosV);
		////CCMoveTo* moveTo4 = CCMoveTo::create(0.06f,endPosV);
		////CCRepeat* move = CCRepeat::create(CCSequence::create(moveTo1,moveTo3,moveTo4,moveTo2,NULL),3);
		//CCRepeat* move = CCRepeat::create(CCSequence::create(moveTo1,moveTo2,NULL),3);

		//
		////CCAction* timmerShake = CCSequence::create(scaleTo,move,NULL);

		//timer->runAction(move);

	}
}
//炸弹点击的，手指的标示
bool SectionGuide::showBombClickPrompt(CCPoint pos)
{
	if (bombClickPrompt != NULL && clickAction!=NULL)
	{
		//bombClickPrompt->setVisible(true);
		bombClickPrompt->setPosition(pos);

		CCAction* action = createAction(CCDelayTime::create(1.8f),
										SEL_CallFuncO(&SectionGuide::bombClickPromptFinishedShow),
										this);
		
        schedule(schedule_selector(SectionGuide::timeToShowFinger),0.8f);
		bombClickPrompt->runAction(action);
//        bombClickPrompt->runAction(clickAction);
		return true;
	}
	return false;
}
//炸弹点击的，手指的标示显示完毕，那么可以点击了
void SectionGuide::bombClickPromptFinishedShow(CCObject* guide)
{
	SectionGuide* _guide = (SectionGuide*)guide;
	_guide->bombClickPrompt->stopAllActions();
	_guide->bombClickPrompt->setVisible(false);
    _guide->isGuide = false;
	//开始下次的显示
	_guide->schedule(schedule_selector(SectionGuide::showInitGuideAgain),GUIDE_GAP);
}

void SectionGuide::hideWatingPrompt()
{
	if (watingPrompt != NULL)
	{
		watingPrompt->stopAllActions();
		watingPrompt->setVisible(false);
	}
}

void SectionGuide::hideInitPrompt()
{
	if (redStartPrompt!=NULL)
	{
		redStartPrompt->setVisible(false);
	}

	if (bombClickPrompt!=NULL)
	{
		bombClickPrompt->setVisible(false);
	}

	if (boxClickPrompt !=NULL)
	{
		boxClickPrompt->setVisible(false);
	}
	if (redStartToTableArrows !=NULL)
	{
		redStartToTableArrows->setVisible(false);
	}
}

void SectionGuide::hide(CCObject* obj)
{
	CCNode* item = (CCNode*)obj;
	if (item!=NULL)
	{
		item->setVisible(false);
	}
}

CCAction* SectionGuide::createAction(CCFiniteTimeAction* action,SEL_CallFuncO finishCallBack, CCObject* param)
{
	CCCallFuncO* fun = CCCallFuncO::create(NULL,finishCallBack,param);
	CCAction* seq = CCSequence::create(action,fun,NULL);

	return seq;
}

void SectionGuide::timeToShowFinger(float time)
{
    unschedule(schedule_selector(SectionGuide::timeToShowFinger));
    if (boxClickPrompt!=NULL) {
        boxClickPrompt->setVisible(true);
        
        boxClickPrompt->runAction(clickAction);
    }
    if (bombClickPrompt!=NULL) {
        bombClickPrompt->setVisible(true);
        bombClickPrompt->runAction(clickAction);
    }
}