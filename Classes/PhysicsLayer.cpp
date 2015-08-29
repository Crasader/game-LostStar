#include "PhysicsLayer.h"
#include "CustomeSprite.h"
#include "LFBox2dHelper.h"
#include "LevelManager.h"
#include "MainFrameLayer.h"

//using namespace lf_helper;

//extern b2Body* whichBodyContainThePoint(b2Vec2 p,b2World* mWorld,bool extension = false);

#define Tag_Star_Item	11111
#define Tag_Left_Item	11112
#define Tag_Right_Item	11113

PhysicsLayer::PhysicsLayer(CCLayer* layer)
{
	mainFrameLayer = layer;
	mWorld = new b2World(b2Vec2(0,-20.0f));
	mWorld->SetAllowSleeping(true);    
	mWorld->SetContinuousPhysics(true);
	mWorld->SetContactListener(this);
	mWorld->SetContactFilter(this);

	winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* bg = CCSprite::create(ResManager::getManager()->getSharedFilePath("main_background.png").c_str());
	addChild(bg,-1);
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));

	//rope texture  rope.png
	mRopeSpriteBatchNode = CCSpriteBatchNode::create(ResManager::getManager()->getSharedFilePath("rope.png").c_str());
	addChild(mRopeSpriteBatchNode);

	// 创建元素
	{
		b2Body* start = createCenterBody(b2Vec2(winSize.width/2/PTM_RATIO,790.0f/PTM_RATIO),0);
		createLeftBody(b2Vec2( (400.0f - 250)/PTM_RATIO,700.0f/PTM_RATIO),-15);
		createRightBody(b2Vec2( (701.0f + 150)/PTM_RATIO,680.0f/PTM_RATIO),30);
		start->SetLinearVelocity(b2Vec2(0,-15));
	}

	CCMenuItemImage* restar_test = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath("restart.png").c_str(),
		ResManager::getManager()->getSharedFilePath("restart_select.png").c_str(),
		this,SEL_MenuHandler(&PhysicsLayer::restarClicked));
	//CCMenuItemImage* setting = CCMenuItemImage::create("restart.png","restart_select.png",this,SEL_MenuHandler(&PhysicsLayer::restarClicked));

	restar_test->setPosition(ccp(500,50));
	menu = CCMenu::create(restar_test,NULL);
	addChild(menu);
	menu->setPosition(CCPointZero);
	moveThing();
	schedule(schedule_selector(PhysicsLayer::tick));
}
PhysicsLayer::~PhysicsLayer()
{
	//rope
	for (size_t i=0; i<mRopes.size(); ++i) {
		delete mRopes[i];
	}
	mRopes.clear();

	delete mWorld;
}

b2Body* PhysicsLayer::createCenterBody(b2Vec2 p,float angle)
{
	CustomeSprite* sprite = new CustomeSprite;
	sprite->initWithFile(ResManager::getManager()->getSharedFilePath("Play_buttom.png").c_str());
	addChild(sprite,0,Tag_Star_Item);
	sprite->release();
	sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.userData = sprite;
	b2Body* bodyA = mWorld->CreateBody(&bodyDef);

	b2CircleShape circle;
	circle.m_radius = 100.0f/2.0f/PTM_RATIO;// = 1.0f;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = Fixture_Density;
	fixtureDef.friction = Fixture_Friction;
	fixtureDef.restitution = Fixture_Restitution;

	bodyA->CreateFixture(&fixtureDef);


	if (angle != 0.0f) {
		bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
	}

	createRope(b2Vec2(winSize.width/2/PTM_RATIO,800.0f/PTM_RATIO),430.0f/PTM_RATIO,bodyA,true);

	return bodyA;
}
b2Body* PhysicsLayer::createLeftBody(b2Vec2 p,float angle)
{
	CustomeSprite* sprite = new CustomeSprite;
	sprite->initWithFile(ResManager::getManager()->getSharedFilePath("board_typeface_star.png").c_str());
	addChild(sprite, 0,Tag_Left_Item);
	sprite->release();
	sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.userData = sprite;
	b2Body* bodyA = mWorld->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(sprite->getContentSize().width/2/PTM_RATIO,sprite->getContentSize().height/2/PTM_RATIO);


	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = Fixture_Density;
	fixtureDef.friction = Fixture_Friction;
	fixtureDef.restitution = Fixture_Restitution;
	bodyA->CreateFixture(&fixtureDef);

	if (angle != 0.0f) {
		bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
	}

	// 创建左边悬吊的绳子
	{
		b2Body* anchorBody;
		b2Vec2 ropeAnchorPos(266.0f / PTM_RATIO , (800.0f)  / PTM_RATIO );
		CCPoint relatvivePoint = ccp(-90.0f,0.0f);
		// 得到旋转过后的点
		relatvivePoint = ccpRotateByAngle(relatvivePoint,ccp(1,0),CC_DEGREES_TO_RADIANS(angle));
		b2Vec2 anchorPos(p.x + relatvivePoint.x/PTM_RATIO, p.y + relatvivePoint.y/PTM_RATIO);

		sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("rope_anchor.png").c_str());
		addChild(sprite);
		sprite->release();
		sprite->setPosition(CCPointMake(anchorPos.x*PTM_RATIO, anchorPos.y*PTM_RATIO));
		sprite->setVisible(false);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = anchorPos;
		bodyDef.userData = sprite;
		anchorBody = mWorld->CreateBody(&bodyDef);
		b2PolygonShape box;
		box.SetAsBox(0.1f, 0.1f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = Fixture_Density;
		fixtureDef.friction = Fixture_Friction;
		fixtureDef.restitution = Fixture_Restitution;
		anchorBody->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef;
		jointDef.Initialize(anchorBody, bodyA, anchorBody->GetWorldCenter());
		mWorld->CreateJoint(&jointDef);

		createRope(ropeAnchorPos,-1,anchorBody);


	}
	// 创建右边悬吊的锚点
	{
		b2Body* anchorBody;
		b2Vec2 ropeAnchorPos(466.0f / PTM_RATIO , (800.0f)  / PTM_RATIO );
		CCPoint relatvivePoint = ccp(140.0f,0.0f);
		// 得到旋转过后的点
		relatvivePoint = ccpRotateByAngle(relatvivePoint,ccp(1,0),CC_DEGREES_TO_RADIANS(angle));
		b2Vec2 anchorPos(p.x + relatvivePoint.x/PTM_RATIO, p.y + relatvivePoint.y/PTM_RATIO);

		sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("rope_anchor.png").c_str());
		addChild(sprite);
		sprite->release();
		sprite->setPosition(CCPointMake(anchorPos.x*PTM_RATIO, anchorPos.y*PTM_RATIO));
		sprite->setVisible(false);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = anchorPos;
		bodyDef.userData = sprite;
		anchorBody = mWorld->CreateBody(&bodyDef);
		b2PolygonShape box;
		box.SetAsBox(0.1f, 0.1f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = Fixture_Density;
		fixtureDef.friction = Fixture_Friction;
		fixtureDef.restitution = Fixture_Restitution;
		anchorBody->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef;
		jointDef.Initialize(anchorBody, bodyA, anchorBody->GetWorldCenter());
		mWorld->CreateJoint(&jointDef);

		createRope(ropeAnchorPos,-1,anchorBody,false);
	}
	return bodyA;
}
b2Body* PhysicsLayer::createRightBody(b2Vec2 p,float angle)
{	
	CustomeSprite* sprite = new CustomeSprite;
	sprite->initWithFile(ResManager::getManager()->getSharedFilePath("board_typeface_fall.png").c_str());
	addChild(sprite, 0,Tag_Right_Item);
	sprite->release();
	sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.userData = sprite;
	b2Body* bodyA = mWorld->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(sprite->getContentSize().width/2/PTM_RATIO,160.0f/2/PTM_RATIO,b2Vec2(0,-0.8f),0);


	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = Fixture_Density;
	fixtureDef.friction = Fixture_Friction;
	fixtureDef.restitution = Fixture_Restitution;

	bodyA->CreateFixture(&fixtureDef);


	if (angle != 0.0f) {
		bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
	}
	// 创建左边悬吊的绳子
	{
		b2Body* anchorBody;		// 绳子的顶端
		b2Vec2 ropeAnchorPos(610.0f / PTM_RATIO , (800.0f)  / PTM_RATIO );
		CCPoint relatvivePoint = ccp(-90.0f,0.0f);
		// 得到旋转过后的点
		relatvivePoint = ccpRotateByAngle(relatvivePoint,ccp(1,0),CC_DEGREES_TO_RADIANS(angle));

		b2Vec2 anchorPos(p.x + relatvivePoint.x/PTM_RATIO, p.y + relatvivePoint.y/PTM_RATIO);
		float length = b2Distance(ropeAnchorPos,anchorPos);

		sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("rope_anchor.png").c_str());
		addChild(sprite);
		sprite->release();
		sprite->setPosition(CCPointMake(anchorPos.x*PTM_RATIO, anchorPos.y*PTM_RATIO));
		sprite->setVisible(false);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = anchorPos;
		bodyDef.userData = sprite;
		anchorBody = mWorld->CreateBody(&bodyDef);
		b2PolygonShape box;
		box.SetAsBox(0.1f, 0.1f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = Fixture_Density;
		fixtureDef.friction = Fixture_Friction;
		fixtureDef.restitution = Fixture_Restitution;
		anchorBody->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef;
		jointDef.Initialize(anchorBody, bodyA, anchorBody->GetWorldCenter());
		mWorld->CreateJoint(&jointDef);

		createRope(ropeAnchorPos,length,anchorBody,false);


	}
	// 创建右边悬吊的锚点
	{
		b2Body* anchorBody;
		b2Vec2 ropeAnchorPos(710.0f / PTM_RATIO , (800.0f)  / PTM_RATIO );
		CCPoint relatvivePoint = ccp(111.0f,0.0f);
		// 得到旋转过后的点
		relatvivePoint = ccpRotateByAngle(relatvivePoint,ccp(1,0),CC_DEGREES_TO_RADIANS(angle));
		b2Vec2 anchorPos(p.x + relatvivePoint.x/PTM_RATIO, p.y + relatvivePoint.y/PTM_RATIO);
		float length = b2Distance(ropeAnchorPos,anchorPos);

		sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("rope_anchor.png").c_str());
		addChild(sprite);
		sprite->release();
		sprite->setPosition(CCPointMake(anchorPos.x*PTM_RATIO, anchorPos.y*PTM_RATIO));
		sprite->setVisible(false);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = anchorPos;
		bodyDef.userData = sprite;
		anchorBody = mWorld->CreateBody(&bodyDef);
		b2PolygonShape box;
		box.SetAsBox(0.1f, 0.1f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = Fixture_Density;
		fixtureDef.friction = Fixture_Friction;
		fixtureDef.restitution = Fixture_Restitution;
		anchorBody->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef;
		jointDef.Initialize(anchorBody, bodyA, anchorBody->GetWorldCenter());
		mWorld->CreateJoint(&jointDef);

		createRope(ropeAnchorPos,length,anchorBody);
	}
	return bodyA;
}

void PhysicsLayer::createRope(b2Vec2 anchorPos,float length,b2Body* hungBody,bool showRope /* = true*/)
{
	b2Body* anchorBody;
	CustomeSprite* sprite = NULL;
	{
		sprite = new CustomeSprite;
		sprite->initWithFile(ResManager::getManager()->getSharedFilePath("rope_anchor.png").c_str());
		addChild(sprite);
		sprite->release();
		sprite->setPosition(CCPointMake(anchorPos.x*PTM_RATIO, anchorPos.y*PTM_RATIO));

		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position = anchorPos;
		bodyDef.userData = sprite;
		anchorBody = mWorld->CreateBody(&bodyDef);

		b2PolygonShape box;
		box.SetAsBox(0.1f, 0.1f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = Fixture_Density;
		fixtureDef.friction = Fixture_Friction;
		fixtureDef.restitution = Fixture_Restitution;

		anchorBody->CreateFixture(&fixtureDef);
	}
	if (showRope)
	{//创建rope
		VRope* newRope = new VRope;
		newRope->initWithBodysAndRopeLenght(mWorld, anchorBody, hungBody, mRopeSpriteBatchNode, length);
		newRope->reset();
		mRopes.insert(mRopes.end(), newRope);
		newRope->setAnchorSprite(sprite);
	}else
	{
		//创建关节
		b2RopeJointDef jd;
		jd.bodyA=hungBody;
		jd.bodyB=anchorBody;
		jd.localAnchorA = b2Vec2(0,0);
		jd.localAnchorB = b2Vec2(0,0);
		jd.maxLength= length;
		mWorld->CreateJoint(&jd);
	}

}

void PhysicsLayer::restarClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	CCDirector::sharedDirector()->replaceScene(MainFrameLayer::createScene());
}
void PhysicsLayer::settingClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
}

void PhysicsLayer::tick(float time)
{
	int velocityIterations = 8;
	int positionIterations = 4;
	//mWorld->Step(1.0f/60.0f, velocityIterations, positionIterations);
	mWorld->Step(time, velocityIterations, positionIterations);

	for (b2Body* b = mWorld->GetBodyList(); b;/* b = b->GetNext()*/)
	{
		if (b->GetUserData() != NULL) 
		{
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			if (myActor->getTag() != Tag_Star_Item)
				myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
		b = b->GetNext();
	}

	// +++ Update rope physics
	for(size_t i=0;i<mRopes.size();i++) {
		mRopes[i]->update(time);
		mRopes[i]->updateSprites();
	}
}

void PhysicsLayer::lfClick(CCPoint glPoint)
{
	b2Body* pickedBody = NULL;
	//b2Body* pickedBody = whichBodyContainThePoint(b2Vec2(glPoint.x/PTM_RATIO,glPoint.y/PTM_RATIO),mWorld,true);
	if (pickedBody && pickedBody->GetUserData())
	{
		CCSprite* node = (CCSprite*)pickedBody->GetUserData();
		if (node->getTag() == Tag_Star_Item)
		{
			CCLog("Star item clicked .....");
			
			mainFrameLayer->setTouchEnabled(true);
			LevelManager::getManager()->goToSeasonUI();
		}
	}
	
}




//
//=======================碰撞回调=========================//
void PhysicsLayer::BeginContact(b2Contact *contact){
}

void PhysicsLayer::EndContact(b2Contact *contact){
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	CCSprite* spriteA = (CCSprite*)(bodyA->GetUserData());
	CCSprite* spriteB = (CCSprite*)bodyB->GetUserData();

	if (spriteA == NULL || spriteB ==NULL) {
		return;
	}

	if ( (spriteA->getTag() == Tag_Left_Item && spriteB->getTag() == Tag_Right_Item) ||
		 (spriteA->getTag() == Tag_Right_Item && spriteB->getTag() == Tag_Left_Item) )
	{
		float dampLast = contact->GetFixtureA()->GetBody()->GetLinearDamping();
		float dampLastAn = contact->GetFixtureA()->GetBody()->GetAngularDamping();
		if ( dampLast <= 0)
			dampLast = 0.5;
		if ( dampLastAn <= 0)
			dampLastAn = 0.5;

		float factor = 1.1f;
		float factorAn = 2.0f;
		contact->GetFixtureA()->GetBody()->SetLinearDamping(dampLast*factor);
		contact->GetFixtureA()->GetBody()->SetAngularDamping(dampLastAn*factorAn);
		contact->GetFixtureB()->GetBody()->SetLinearDamping(dampLast*factor);
		contact->GetFixtureB()->GetBody()->SetAngularDamping(dampLastAn*factorAn);
		
	}
}

void PhysicsLayer::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse){
}

bool PhysicsLayer::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	CCSprite* spriteA = (CCSprite*)( (fixtureA->GetBody())->GetUserData());
	CCSprite* spriteB = (CCSprite*)( (fixtureB->GetBody())->GetUserData());

	if (spriteA == NULL || spriteB ==NULL) {
		return true;
	}

	if (spriteB->getTag() == Tag_Star_Item)
	{
		return false;
	}
	if (spriteA->getTag() == Tag_Star_Item)
	{
		return false;
	}
	
	return true;
	
}