#include "BaseGuide.h"
#include "ResManager.h"

BaseGuide::BaseGuide(std::string guidePicBaseName,int picNum)
{
	init();
	currentGuideIndex = 0;
	goToNestAction = NULL;
	switching = false;
	switchTime = 0.5f;
	this->guidePicBaseName = guidePicBaseName;
	this->picNum = picNum;
	winSize = CCDirector::sharedDirector()->getWinSize();
	//初始化
	initGuideLayer(guidePicBaseName,picNum);
}
BaseGuide::~BaseGuide()
{
	CC_SAFE_RELEASE(guideSpriteArray);
	CC_SAFE_RELEASE(goToNestAction);
}
CCScene* BaseGuide::createGuideScene(std::string baseFileName,int guidePicNum)
{
	CCScene* scene = CCScene::create();

	BaseGuide* guide = new BaseGuide(baseFileName,guidePicNum);
	guide->autorelease();

	scene->addChild(guide);
	return scene;
}
void BaseGuide::initGuideLayer(std::string baseFileName,int guidePicNum)
{
	CCFiniteTimeAction* switchAct = createSwitchAction();
	CCCallFunc* callBack = CCCallFunc::create(this,SEL_CallFunc(&BaseGuide::switchGuideFinished));
	goToNestAction = CCSequence::create(switchAct,callBack,NULL);
	goToNestAction->retain();
	
	guideSpriteArray = CCArray::createWithCapacity(guidePicNum);
	guideSpriteArray->retain();
	char guideFileName[56];
	for (int i=1;i<=picNum;i++)
	{
		sprintf(guideFileName,"%s%s%d%s",baseFileName.c_str(),"_",i,".png");//获得诸如guide1.png的图片
		//CCSprite* guideImage = CCSprite::create(ResManager::getManager()->getSharedFilePath(baseFileName).c_str());
		CCSprite* guideImage = CCSprite::create(baseFileName.c_str());
		guideSpriteArray->addObject(guideImage);

		addChild(guideImage,picNum-i);//第一个放在最上面，也许一会要改成移动切换，当前是透明转换
		guideImage->setPosition(ccp(winSize.width/2,winSize.height/2));
	}

	setTouchEnabled(true);
}

CCFiniteTimeAction* BaseGuide::createSwitchAction()
{
	CCFadeTo* fadeTo = CCFadeTo::create(switchTime,0);
	return fadeTo;
}

void BaseGuide::guideEnd()
{
	CCScene* scene = MainFrameLayer::createScene();
	CCTransitionSlideInR* tran = CCTransitionSlideInR::create(0.5, scene);
	CCDirector::sharedDirector()->replaceScene(tran);
	//保存已经启动过程序的标志
	GameInforEditer::getGameInfor()->firstRunOverApp();
}

void BaseGuide::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (switching)
		return;
	switching = true;

	if ((unsigned)currentGuideIndex < guideSpriteArray->count()-1)
	{//还有引导图片

		((CCSprite*)guideSpriteArray->objectAtIndex(currentGuideIndex))->runAction(goToNestAction);

	}else
	{//进入主界面
		guideEnd();
	}

}
void BaseGuide::switchGuideFinished()
{
	switching = false;

	((CCSprite*)guideSpriteArray->objectAtIndex(currentGuideIndex))->setVisible(false);

	currentGuideIndex++;
}