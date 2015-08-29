#include "SafeBoarder.h"
#include "CustomeSprite.h"
#include "ResManager.h"
#include "BaseContentLayer.h"
#include "SpriteHelper.h"
#include "AppMacros.h"

#define TOOL_POWER				1500.0f
#define TOOL_MAX_EFFETIVE_DIS	15

SafeBoarder::SafeBoarder(b2World* mWorld,CCLayer* layer)
{
	this->mWorld = mWorld;
	this->mLayer = layer;

	mCoverLayer = NULL;

	float width = 1024.0f;
	float height = 768.0f;
	float x_exturn = 14;


	// 新的boarder的效果
//	{
		mCoverLayer = CCLayer::create();
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		mLayer->getParent()->getParent()->addChild(mCoverLayer, 1, Tag_SafeBoard_CoverLayer);
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		mCoverLayer->setPosition(ccp( (visibleSize.width-designResolutionSize.width)/2.0f,0 + g_iTaiJieHight/2 ));


		CCNode* nodeTrigle = CCNode::create();
		nodeTrigle->runAction(CCRepeat::create(CCSequence::create(
			CCDelayTime::create(0.17f),
			CCCallFunc::create(this,SEL_CallFunc(&SafeBoarder::createNew)),
			NULL
			),1));
		mCoverLayer->addChild(nodeTrigle);

// 		CCSprite* one = CCSprite::create(ResManager::getManager()->getSharedFilePath("daoju_xiaoguo_chaonengli_2.png").c_str());
// 		mCoverLayer->addChild(one);
// 		CCPoint glPoint = ccp(designResolutionSize.width/2.0f, designResolutionSize.height/2.0f);
// 		one->setPosition(glPoint);
// 		one->setOpacity(0);
// 		one->setScaleX(CCDirector::sharedDirector()->getWinSize().width/designResolutionSize.width);
// 		one->setScaleY(CCDirector::sharedDirector()->getWinSize().height/designResolutionSize.height);
// 
// 		one->runAction(CCFadeIn::create(0.16f));

		/*CCLayer* popLayer = CCLayer::create();
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		scene->addChild(popLayer);
		for (int i=0;i<3;i++)
		{
			CCSprite* one = CCSprite::create("daoju_xiaoguo_chaonengli.png");
			popLayer->addChild(one);
			CCPoint glPoint = ccp(width/2.0f,height/2.0f);
			one->setPosition(glPoint);

			one->runAction(CCRepeat::create(CCSequence::create(
				CCDelayTime::create(0.1f+i*0.2f),
				CCSpawn::create(
				CCScaleTo::create(0.2f,0.9f),
				CCFadeTo::create(0.2f,20),
				NULL),
				CCHide::create(),
				CCSpawn::create(
				CCScaleTo::create(0.0f,1.0),
				CCFadeTo::create(0.0f,255),
				NULL),
				CCShow::create(),
				NULL
				),-1));
		}*/

//	}



	CCAction* action = CCSequence::create(
		CCFadeTo::create(0.2f,255),
		CCDelayTime::create(3.0f),
		CCRepeat::create(CCSequence::create(
			CCFadeTo::create(0.7f,100),
			CCFadeTo::create(0.7f,255),
			NULL
		),-1),NULL);
	int init_opatcity = 0;

	float xoffset = (CCDirector::sharedDirector()->getWinSize().width/2 - designResolutionSize.width/2);

	//下面
	{
		CustomeSprite* sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("superpower_biankuang.png").c_str());
		sprite->setOpacity(init_opatcity);
		sprite->runAction((CCAction*)action->copy()->autorelease());

		CCPoint glPoint = ccp(layer->getContentSize().width/2,sprite->getContentSize().height/2);
		CCPoint naltivePos = layer->convertToNodeSpace(glPoint);
		naltivePos.y += g_iTaiJieHight/2;
		b2Body* side = createSide(naltivePos,width,0,sprite,Tag_SafeBoard_D);
		boaders.push_back(side);
	}
	//上面
	{
		CustomeSprite* sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("superpower_biankuang.png").c_str());
		sprite->setOpacity(init_opatcity);
		sprite->runAction((CCAction*)action->copy()->autorelease());

		CCPoint glPoint = ccp(layer->getContentSize().width/2,layer->getContentSize().height - sprite->getContentSize().height/2);
		CCPoint naltivePos = layer->convertToNodeSpace(glPoint);

		b2Body* side = createSide(naltivePos,width,180,sprite,Tag_SafeBoard_T);
		boaders.push_back(side);
	}

	const float shift_x = (CCDirector::sharedDirector()->getWinSize().width - 1024)/2.0f;
	//左面
	{
		CustomeSprite* sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("superpower_biankuang.png").c_str());
		sprite->setOpacity(init_opatcity);
		sprite->runAction((CCAction*)action->copy()->autorelease());

		CCPoint glPoint = ccp(sprite->getContentSize().height + shift_x - x_exturn,layer->getContentSize().height/2);
		CCPoint naltivePos = layer->convertToNodeSpace(glPoint);
		naltivePos.x -= xoffset;
		b2Body* side = createSide(naltivePos,width,-90,sprite,Tag_SafeBoard_L);
		boaders.push_back(side);
	}
	//右面
	{
		CustomeSprite* sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("superpower_biankuang.png").c_str());
		sprite->setOpacity(init_opatcity);
		sprite->runAction((CCAction*)action->copy()->autorelease());

		CCPoint glPoint = ccp(layer->getContentSize().width - sprite->getContentSize().height - shift_x + x_exturn,layer->getContentSize().height/2);
		CCPoint naltivePos = layer->convertToNodeSpace(glPoint);
		naltivePos.x += xoffset;
		b2Body* side = createSide(naltivePos,width,90,sprite,Tag_SafeBoard_R);
		boaders.push_back(side);
	}

}


void SafeBoarder::createNew()
{
	float delay = 0.016f;
	CCSprite* one = CCSprite::create(ResManager::getManager()->getSharedFilePath("daoju_xiaoguo_chaonengli.png").c_str());
	mCoverLayer->addChild(one);
	CCPoint glPoint = ccp(designResolutionSize.width/2.0f, designResolutionSize.height/2.0f);
	one->setPosition(glPoint);

	one->setOpacity(0);

	float scalex = CCDirector::sharedDirector()->getWinSize().width/designResolutionSize.width;
	one->setScaleX(scalex);

	float scaley = CCDirector::sharedDirector()->getWinSize().height/designResolutionSize.height;
	one->setScaleY(scaley);

	one->runAction(CCSequence::create(
		CCSequence::create(
		CCSpawn::create(
		CCFadeTo::create(10*delay, 255), NULL),
		CCCallFunc::create(this,SEL_CallFunc(&SafeBoarder::createNew)), NULL),
		CCSpawn::create(
		CCScaleTo::create(30*delay, scalex*0.9f, scaley*0.85f), CCFadeTo::create(30*delay, 0), NULL),
		CCRemoveSelf::create(),
		NULL
		));
}


void SafeBoarder::timeToCreateNodeBoarder(CCNode* not_use)
{
	CCSprite* one = CCSprite::create(ResManager::getManager()->getSharedFilePath("daoju_xiaoguo_chaonengli.png").c_str());
	mCoverLayer->addChild(one);
	CCPoint glPoint = ccp(designResolutionSize.width/2.0f, designResolutionSize.height/2.0f);
	one->setPosition(glPoint);

	float scale = CCDirector::sharedDirector()->getWinSize().width/designResolutionSize.width;
	one->setScaleX(scale);

	one->runAction(CCSequence::create(
		CCSpawn::create(
			CCScaleTo::create(0.5f, scale - 0.1f, 0.9f),
			CCFadeTo::create(0.5f,20),
		NULL),
		CCHide::create(),
		CCRemoveSelf::create(),
		NULL
		));
}

b2Body* SafeBoarder::createSide(CCPoint pos,float width,float angle,CCSprite* sprite,int TAG)
{
	mLayer->addChild(sprite, 0, TAG);
	sprite->setPosition(pos);
	sprite->setRotation(angle);
	sprite->setVisible(false);

	// 不要这个效果了，换效果了
	sprite->setVisible(false);

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(pos.x/PTM_RATIO, pos.y/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body* bodyA = mWorld->CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(width/PTM_RATIO, 0.5f);


	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;	
	fixtureDef.density = Fixture_Density;
	fixtureDef.friction = Fixture_Friction;
	fixtureDef.restitution = 1.0f;

	bodyA->CreateFixture(&fixtureDef);

	if (angle != 0.0f) {
		bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
	}

	return bodyA;
}

vector<b2Body*> SafeBoarder::getBoarders()
{
	return boaders;
}

SafeBoarder::~SafeBoarder(void)
{
}


class RemoveAbleSprite : public CCSprite 
{
private:
	RemoveAbleSprite()
	{
	}
public:
	static RemoveAbleSprite* create(string fileName)
	{
		RemoveAbleSprite* ret = new RemoveAbleSprite();
		ret->initWithFile(fileName.c_str());
		ret->autorelease();
		return ret;
	}
	static RemoveAbleSprite* create(CCTexture2D* texture)
	{
		RemoveAbleSprite* ret = new RemoveAbleSprite();
		ret->initWithTexture(texture);
		ret->autorelease();
		return ret;
	}

	void removeNode(CCNode* node)
	{
		if (node)
		{
			node->removeFromParent();
		}
	}
};


// 需要传入要产生作用的目标
SuperTool::SuperTool(b2World* mWorld,BaseContentLayer* baseLayer)
{
	this->mWorld = mWorld;
	this->mBaseLayer = baseLayer; // 要用它的工具函数
	this->mCurrentBlowPos = b2Vec2(-100,-100);
	this->mBlowing = false;
	this->mSafeBoards.clear();
	this->effectTemplate = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"waibu.png").c_str());
	this->effectTemplate->setOpacity(0);
	this->mBaseLayer->addChild(effectTemplate,20);

	createAndRunWaves(); // 一直播放
	effectTemplate->setVisible(mBlowing);
	
}
SuperTool::~SuperTool()
{
}

void SuperTool::setBlowPos(b2Vec2 posInWorld)
{
	mCurrentBlowPos = posInWorld;
	effectTemplate->setPosition(ccp(mCurrentBlowPos.x * PTM_RATIO,mCurrentBlowPos.y * PTM_RATIO));
}
void SuperTool::setBorders(vector<b2Body*> safeBoards)
{
	mSafeBoards = safeBoards;
}

// 吹
void SuperTool::blow_tick(float applayTime)
{
	if (!mBlowing)
		return;

	b2Vec2 forceCenter = mCurrentBlowPos;

	for (b2Body* b = mWorld->GetBodyList(); b != NULL; b = b->GetNext())
	{
		b2Fixture* fixture = b->GetFixtureList();//一个fixture带一个shape，多个fixture夹出一个物体

		for (; fixture!=NULL; fixture=fixture->GetNext())
		{
			b2Shape* shape = (b2Shape*)fixture->GetShape();

			b2Transform transform;//存储一个物体的位置和旋转角度的结构体
			transform.Set(b->GetPosition(), b->GetAngle());

			b2AABB aabb;//一个描述外围边框的结构体 bounding box

			//根据transform，计算出某个孩子的形状的。关联的边框坐标，存放在aabb中
			shape->ComputeAABB(&aabb, transform, 0);

			b2Vec2 shapeCenterP = aabb.GetCenter();//即将受力的形状的位置;

			float32 xDelta = shapeCenterP.x - forceCenter.x;
			float32 yDelta = shapeCenterP.y - forceCenter.y;
			float32 distance = sqrt(pow((xDelta),2)+pow((yDelta), 2));



			if (distance > TOOL_MAX_EFFETIVE_DIS) {
				continue;
			}

			b2Vec2 force = b2Vec2(0,0);
			//计算与爆炸点之间是否有遮挡
			bool shielded = false;
			{
				float32 plus = (float32)BasicLength/(float32)PTM_RATIO/2;
				int testCount = distance/plus;
				float32 xPlus = plus * xDelta/distance;
				float32 yPlus = plus * yDelta/distance;
				b2Vec2 testP = shapeCenterP;
				for (int i=0; i<testCount; ++i) {
					testP.x -= xPlus;
					testP.y -= yPlus;
					b2Fixture* fixtureTmp = mBaseLayer->whichFixtureContainThePoint(testP);
					if (fixtureTmp != NULL) {

						shielded = true;

						for (int boarderIndex = 0,boardCount=mSafeBoards.size(); boarderIndex<boardCount; boarderIndex++)
						{
							b2Fixture* boilerFixture = mSafeBoards[boarderIndex]->GetFixtureList();
							for (; boilerFixture!=NULL; boilerFixture=boilerFixture->GetNext()){
								if (fixtureTmp == boilerFixture) {
									//fixture是锅炉,不算挡住
									shielded = false;
									break;
								}
							}

							if (!shielded)
								break;
						}
						

						if (fixtureTmp == fixture) {
							//fixture是自己，不算挡住
							shielded = false;
						}
						if (shielded) {
							break;
						}

					}
				}
			}

			if (!shielded) {
				//防止锅炉距离很近的时候力量太大
				static const float distanceFactorForBoiler = 3.0f;
				force = b2Vec2(
					TOOL_POWER*(xDelta/distance)/pow(distance+distanceFactorForBoiler,2.0f), 
					TOOL_POWER*(yDelta/distance)/pow(distance+distanceFactorForBoiler,2.0f));
			}

			if (pow(force.x, 2)+pow(force.y, 2) >= 1000)
			{
				b2Vec2 imp;
				imp.x = force.x * applayTime;
				imp.y = force.y * applayTime;
				b->ApplyLinearImpulse(imp, shapeCenterP);
			}
		}
	}







}

void SuperTool::createAndRunWaves()
{
	effectTemplate->stopAllActions();
	effectTemplate->removeAllChildren();

	float delay = 0.016f;

	CCSprite* pneibu = CCSprite::create(ResManager::getManager()->getFilePath(g_gameinPath+"neibu.png").c_str());
	effectTemplate->addChild(pneibu, -1, Tag_SafeBoard_Neibu);
	pneibu->setPosition(ccp(effectTemplate->getContentSize().width/2,effectTemplate->getContentSize().height/2));
	pneibu->setScale(0.0f);
	pneibu->setOpacity(0);

	pneibu->runAction(CCSequence::create(
		CCSpawn::create(
			CCFadeTo::create(delay*5, 255), 
			CCScaleTo::create(delay*5, 1.2f),
			NULL),
			CCSequence::create(
		CCRepeat::create(
			CCSequence::create(CCRotateBy::create(delay*5, -3), CCRotateBy::create(delay*5, 3), NULL), 5),
		NULL), NULL));

	createNew();
// 	int i = 0;
// //	for (int i = 0; i < 5; i++)
// 	{
// 		CCSprite* wave = CCSprite::createWithTexture(effectTemplate->getTexture(), effectTemplate->getTextureRect());
// 		effectTemplate->addChild(wave,0);
// 		wave->setPosition(ccp(effectTemplate->getContentSize().width/2,effectTemplate->getContentSize().height/2));
// 		wave->setScale(0.5f);
// 
// //		wave->setOpacity(0);
// 
// 		// 旋转
// 		//wave->runAction(CCSequence::create(CCDelayTime::create(delay*i),CCRepeat::create(CCRotateBy::create(3.0f,360),-1),NULL));
// 		wave->runAction(CCSequence::create(
// 			CCDelayTime::create(delay*i*10),
// 			CCRepeat::create(
// 			CCSequence::create(
// 			CCFadeTo::create(0.0f, 255),
// 			CCSpawn::create(
// 			CCFadeTo::create(35*delay, 0),
// 			CCScaleTo::create(35*delay, 1.5f), NULL),
// 			CCCallFuncN::create(this,SEL_CallFuncN(&SuperTool::resetFireItem)), NULL),
// 			-1),
// 			NULL
// 			));
// 	}
}

void SuperTool::createNew()
{
	if (!mBlowing)
	{
		return;
	}

	float delay = 0.016f;
	CCSprite* wave = CCSprite::createWithTexture(effectTemplate->getTexture(), effectTemplate->getTextureRect());
	effectTemplate->addChild(wave,0);
	wave->setPosition(ccp(effectTemplate->getContentSize().width/2,effectTemplate->getContentSize().height/2));
	
	wave->setScale(0.5f);
	wave->setOpacity(0);

	wave->runAction(CCSequence::create(
		//CCRepeat::create(
		CCSequence::create(
		CCSpawn::create(
		CCFadeTo::create(10*delay, 255),
		CCScaleTo::create(10*delay, 0.8f), NULL),
		CCCallFunc::create(this,SEL_CallFunc(&SuperTool::createNew)), NULL),
		CCSpawn::create(
		CCFadeTo::create(25*delay, 0),
		CCScaleTo::create(25*delay, 1.5f), NULL),
		CCRemoveSelf::create(),
		//-1),
		NULL
		));
}

// 开始
void SuperTool::start()
{
	mBlowing = true;
	effectTemplate->setVisible(mBlowing);
	effectTemplate->stopAllActions();
	createAndRunWaves();
}

void SuperTool::resetFireItem(CCNode* sprite)
{
	sprite->setScale(0.5f);
	((CCSprite*)sprite)->setOpacity(0);
}

// 停
void SuperTool::stop()
{
	mBlowing = false;

	float delay = 0.016f;
	if (CCSprite* pneibu = (CCSprite*)effectTemplate->getChildByTag(Tag_SafeBoard_Neibu))
	{
		pneibu->runAction(CCSequence::create(
			CCSpawn::create(
			CCFadeTo::create(delay*10, 0), 
			CCScaleTo::create(delay*10, 0.0f),
			NULL),
			CCRemoveSelf::create(),
		 NULL));
	}

	effectTemplate->runAction(CCSequence::create(
		CCDelayTime::create(delay*12.0f),
		CCCallFunc::create(this,SEL_CallFunc(&SuperTool::clear)),
		NULL));
}

void SuperTool::clear()
{
	effectTemplate->stopAllActions();
	mBlowing = false;
	effectTemplate->setOpacity(0);
	effectTemplate->removeAllChildren();
}

void SuperTool::remove()
{
	effectTemplate->removeFromParent();
}