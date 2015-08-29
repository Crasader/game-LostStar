//
//  VRope.m
//
//  Created by patrick on 16/10/2010.
//

#include "VRope.h"
#include "LineIntersect.h"
#include "CommonDefine.h"
#include "AchievementManager.h"
#include "BaseContentLayer.h"

//increase value to have less segments per rope, decrease to have more segments
#define RopeSegmentFactor 12

//被切断的绳子的下落速度
#define CuttedRopeFallSpeed 60.0f
//被切断的绳子的消失速度
#define CuttedRopeDisappearSpeed 80.0f

void VRope::initWithBodys(b2World* world, b2Body *body1, b2Body *body2, CCSpriteBatchNode *spriteSheetArg) {
    initWithBodysAndRopeLenght(world, body1, body2, spriteSheetArg, b2Distance(body1->GetPosition(), body2->GetPosition()));
}

void VRope::initWithBodysAndRopeLenght(b2World* world, b2Body *body1, b2Body *body2, cocos2d::CCSpriteBatchNode *spriteSheetArg, float ropeLength){
    bodyA = body1;
    bodyB = body2;
    mWorld = world;
    
	float length =  ropeLength; //
	if (length < 0)
		length = b2Distance(body1->GetPosition(), body2->GetPosition());
    //创建关节
    b2RopeJointDef jd;
    jd.bodyA=bodyA;
    jd.bodyB=bodyB;
    jd.localAnchorA = b2Vec2(0,0);
    jd.localAnchorB = b2Vec2(0,0);
    jd.maxLength= length;
    mJoint = (b2RopeJoint*)mWorld->CreateJoint(&jd);
    
    
    CCPoint pointA = ccp(bodyA->GetPosition().x*PTM_RATIO,bodyA->GetPosition().y*PTM_RATIO);
    CCPoint pointB = ccp(bodyB->GetPosition().x*PTM_RATIO,bodyB->GetPosition().y*PTM_RATIO);
    spriteSheet = spriteSheetArg;
    createRope(pointA, pointB, (length+0.1)*PTM_RATIO);
}

b2Body* VRope::getBodyA(){
    return bodyA;
}
void VRope::setBodyA(b2Body* bodyA)
{
	this->bodyA = bodyA;
}
void VRope::setBodyB(b2Body* bodyB)
{
	this->bodyB = bodyB;
}

b2Body* VRope::getBodyB(){
    return bodyB;
}

bool VRope::checkRopeWhenBodyDestroied(b2Body *body){
    if (bodyA == body) {
        //已经销毁掉的body需要清空
        bodyA = NULL;
        
        //body销毁了，则其上的关节也已经被自动销毁了。需要清空
        mJoint = NULL;
        
        return true;
    }
    if (bodyB == body) {
        //已经销毁掉的body需要清空
        bodyB = NULL;
        
        //body销毁了，则其上的关节也已经被自动销毁了。需要清空
        mJoint = NULL;
        
        return true;
    }
    
    return false;
}


void VRope::reset() {
	CCPoint pointA;
	CCPoint pointB;
	if (bodyA == NULL) {
        pointA = ccp(vPoints[0]->x, vPoints[0]->y);
    }else{
        pointA = ccp(bodyA->GetPosition().x*PTM_RATIO,bodyA->GetPosition().y*PTM_RATIO);
    }
    if (bodyB == NULL) {
        pointB = ccp(vPoints[numPoints-1]->x, vPoints[numPoints-1]->y);
    }else{
        pointB = ccp(bodyB->GetPosition().x*PTM_RATIO,bodyB->GetPosition().y*PTM_RATIO);
    }
	resetWithPoints(pointA, pointB);
}

void VRope::update(float dt) {

	if (numPoints <= 0)
	{
		return;
	}
	
    CCPoint pointA;
	CCPoint pointB;
	if (bodyA == NULL) {
		//pointA = ccp(vPoints[0]->x, vPoints[0]->y - CuttedRopeFallSpeed*dt);
		pointA = ccp(vPoints[0]->x, vPoints[0]->y); // LiFeng 修改
    }else{
        pointA = ccp(bodyA->GetPosition().x*PTM_RATIO,bodyA->GetPosition().y*PTM_RATIO);
    }
    if (bodyB == NULL) {
		//pointB = ccp(vPoints[numPoints-1]->x, vPoints[numPoints-1]->y - CuttedRopeFallSpeed*dt);
		pointB = ccp(vPoints[numPoints-1]->x, vPoints[numPoints-1]->y);// LiFeng 修改
    }else{
        pointB = ccp(bodyB->GetPosition().x*PTM_RATIO,bodyB->GetPosition().y*PTM_RATIO);
    }
    
    //
    if (isRopeCutted() && mSpritesOpacity != 0) {
        if (mSpritesOpacity < CuttedRopeDisappearSpeed*dt) {
            mSpritesOpacity = 0;

			vector<VRope*>* ropes = BaseContentLayer::shareBaseContentLayer()->getRopes();
			for (size_t i = 0; i < ropes->size(); i++)
			{
				if (this == ropes->at(i))
				{
					ropes->erase(ropes->begin()+i);
					break;
				}
			}
			delete this;
			return;
        }else{
            mSpritesOpacity -= CuttedRopeDisappearSpeed*dt;
        }

		for (size_t i = 0; i < ropeSprites.size(); i++)
		{
			ropeSprites[i]->setOpacity(mSpritesOpacity);
		}
    }
    
	updateWithPoints(pointA, pointB, dt);
}
/*
void VRope::shortenRope(float deltaLen){
    mRopeLength -= deltaLen;
    
    int segmentFactor = RopeSegmentFactor; 
	int num = mRopeLength / segmentFactor;
    if (num >= numPoints) {
        return;
    }
    
    int numSegToCut = numPoints - num;
    numPoints = num;
    
    for (int i=0; i<numSegToCut; ++i) {
        delete *vPoints.begin();
        vPoints.erase(vPoints.begin());
        
        vSticks.erase(vSticks.begin());
        
        spriteSheet->removeChild(*ropeSprites.begin(), true);
        ropeSprites.erase(ropeSprites.begin());
    }
}

void VRope::lengthenRope(float deltaLen){
    
}
*/
void VRope::initWithPoints(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB, CCSpriteBatchNode *spriteSheetArg,float length/* = -1*/) {
    spriteSheet = spriteSheetArg;
	bodyA = NULL;
	bodyB = NULL;
    createRope(pointA, pointB,length);
}

void VRope::createRope(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB, float ropeLenght) {
    
    float distance;
    if (ropeLenght < 0) {
        distance = ccpDistance(pointA,pointB);
    }else{
        distance = ropeLenght;
    }
    
	int segmentFactor = RopeSegmentFactor; //increase value to have less segments per rope, decrease to have more segments
	numPoints = distance/segmentFactor;
    
    mRopeLength = segmentFactor * numPoints;
    
	CCPoint diffVector = ccpSub(pointB,pointA);
	float multiplier = distance / (numPoints-1);
	antiSagHack = 0.1f; //HACK: scale down rope points to cheat sag. set to 0 to disable, max suggested value 0.1
    
	for(int i=0;i<numPoints;i++) {
		CCPoint tmpVector = ccpAdd(pointA, ccpMult(ccpNormalize(diffVector),multiplier*i*(1-antiSagHack)));
		VPoint* tmpPoint = new VPoint;
        tmpPoint->setPos(tmpVector.x, tmpVector.y);
        vPoints.insert(vPoints.end(), tmpPoint);
	}
    
	for(int i=0;i<numPoints-1;i++) {
        VStick tmpStick;
        tmpStick.initWith(vPoints[i], vPoints[i+1]);
		vSticks.insert(vSticks.end(), tmpStick);
	}
    
	if(spriteSheet!=NULL) {
		for(int i=0;i<numPoints-1;i++) {
            VPoint* point1 = vSticks[i].getPointA();
            VPoint* point2 = vSticks[i].getPointB();
			CCPoint stickVector = ccpSub(ccp(point1->x,point1->y),ccp(point2->x,point2->y));
			float stickAngle = ccpToAngle(stickVector);
            
            CCTexture2D* texture = spriteSheet->getTexture();
            RecordSprite* tmpSprite = new RecordSprite;
			tmpSprite->setTag(Tag_Box_RopeBatchSprite);
			tmpSprite->autorelease();
			tmpSprite->initWithTexture(texture, CCRectMake(0,0,multiplier,texture->getContentSize().height));
			ccTexParams params = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
            tmpSprite->getTexture()->setTexParameters(&params);
            tmpSprite->setPosition(ccpMidpoint(ccp(point1->x,point1->y),ccp(point2->x,point2->y)));
            tmpSprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(stickAngle));
            spriteSheet->addChild(tmpSprite);
            ropeSprites.insert(ropeSprites.end(), tmpSprite);
		}

		//// LiFeng 添加
		//ropeSprites[ropeSprites.size()-1]->setOpacity(100);
	}else{
        CCAssert(false, "not init");
    }
    
}
int VRope::getRopeBlockNum()
{
	return ropeSprites.size();
}
void VRope::setRopeBlockOpacity(size_t index,GLubyte o)
{
	if (index < 0 || index > (ropeSprites.size()-1) )
		return;
	ropeSprites[index]->setOpacity(o);
}
void VRope::resetWithPoints(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB) {
	float distance = ccpDistance(pointA,pointB);
	CCPoint diffVector = ccpSub(pointB,pointA);
	float multiplier = distance / (numPoints - 1);
	for(int i=0;i<numPoints;i++) {
		CCPoint tmpVector = ccpAdd(pointA, ccpMult(ccpNormalize(diffVector),multiplier*i*(1-antiSagHack)));
        VPoint* tmpPoint = vPoints[i];
        tmpPoint->setPos(tmpVector.x, tmpVector.y);
		
	}
}

void VRope::removeSprites() {
	for (size_t i = 0; i<ropeSprites.size(); i++) {
        RecordSprite* tmpSprite = ropeSprites[i];
		tmpSprite->removeFromParent();
	}
    ropeSprites.clear();
	numPoints = 0;
}

bool VRope::isRopeCutted(){
    if (bodyA == NULL || bodyB == NULL) {
        return true;
    }else{
        return false;
    }
}

void VRope::destroiyRope(){
    if (mJoint != NULL && bodyA != NULL && bodyB != NULL) {
        mWorld->DestroyJoint(mJoint);
        mJoint = NULL;
    }
    
    for(size_t i=0;i<vPoints.size();i++) {
        delete vPoints[i];
	}
    
    vPoints.clear();
    vSticks.clear();
    
    removeSprites();

    numPoints = 0;
    mRopeLength = 0;
}



bool VRope::cutRopeWithTowPoints(cocos2d::CCPoint pA, cocos2d::CCPoint pB, VRope** newRopeOut){
    //已经砍断的就不要再砍啦
    if (isRopeCutted()) {
        return false;
    }
    
    bool cuted = false;
    
    size_t pointsCounter = 1;
    for (pointsCounter=1; pointsCounter<vPoints.size(); ++pointsCounter) {
        CCPoint ropePointA(vPoints[pointsCounter-1]->x, vPoints[pointsCounter-1]->y);
        CCPoint ropePointB(vPoints[pointsCounter]->x, vPoints[pointsCounter]->y);
        if (intersect_in2(pA, pB, ropePointA, ropePointB)) {
            //线段相交，砍断
            cuted = true;
            break;
        }
    }
    
    if (!cuted) {
        return false;
    }
	AchievementManager::getInstance()->ropeCuted((CCNode*)bodyB->GetUserData());

	//提前10帧左右播放切割绳子动画
	int offset = 10;
	int iCurRecordFrame = BaseContentLayer::shareBaseContentLayer()->getCurRecordFrameIndex();

	int index = iCurRecordFrame-offset;
	if (index > 0)
	{
		if (pointsCounter >= ropeSprites.size())
		{
			--pointsCounter;
		}
		
		bool bFind = false;
		std::vector< std::vector<record> >* allFaremData = BaseContentLayer::shareBaseContentLayer()->getAllFramesData();
		for (size_t i = 0; i < allFaremData->at(index).size(); i++)
		{
			if (allFaremData->at(index).at(i).id == ropeSprites[pointsCounter]->m_SaveData.id)
			{
				bFind = true;
				allFaremData->at(index).at(i).eventId = eventID_CutRopeAction;
				break;
			}
		}

		if (!bFind)
		{
			ropeSprites[pointsCounter]->m_SaveData.eventId = eventID_CutRopeAction;
			allFaremData->at(index).push_back(ropeSprites[pointsCounter]->m_SaveData);
			ropeSprites[pointsCounter]->m_SaveData.eventId = eventID_None;
		}
	}
	createNewRope(newRopeOut, pointsCounter);

	BaseContentLayer::shareBaseContentLayer()->recordEventID(this, eventID_CutRopeStart+pointsCounter);
	
    return true;
}

void VRope::createNewRope(VRope** newRopeOut, int pointsCounter)
{
	//创建新的一节绳子
	{
		*newRopeOut = new VRope;

		(*newRopeOut)->numPoints = pointsCounter+1;
		for (int i=0; i<(*newRopeOut)->numPoints-1; ++i) {
			(*newRopeOut)->vPoints.insert((*newRopeOut)->vPoints.end(), vPoints[i]);
			(*newRopeOut)->vSticks.insert((*newRopeOut)->vSticks.end(), vSticks[i]);
			(*newRopeOut)->ropeSprites.insert((*newRopeOut)->ropeSprites.end(), ropeSprites[i]);
		}

		//切断的那一个point分两份
		VPoint* p = new VPoint;
		*p = *vPoints[(*newRopeOut)->numPoints-1];
		(*newRopeOut)->vPoints.insert((*newRopeOut)->vPoints.end(), p);
		(*newRopeOut)->vSticks[(*newRopeOut)->vSticks.size()-1].initWith((*newRopeOut)->vSticks[(*newRopeOut)->vSticks.size()-1].getPointA(), p);

		(*newRopeOut)->spriteSheet = spriteSheet;
		(*newRopeOut)->antiSagHack = antiSagHack;
		(*newRopeOut)->mWorld = mWorld;
		(*newRopeOut)->bodyA = NULL;//LiFeng 修改
		//(*newRopeOut)->bodyA = bodyA;
		(*newRopeOut)->bodyB = NULL;
		(*newRopeOut)->mJoint = NULL;
		(*newRopeOut)->mRopeLength = RopeSegmentFactor * ((*newRopeOut)->numPoints-1);
	}

	//修改当前绳子
	{
		std::vector<VPoint*> tmpPoints;
		std::vector<VStick> tmpSticks;
		std::vector<RecordSprite*> tmpSprites;
		for (int i=pointsCounter; i<numPoints-1; ++i) {
			tmpPoints.insert(tmpPoints.end(), vPoints[i]);
			tmpSticks.insert(tmpSticks.end(), vSticks[i]);
			tmpSprites.insert(tmpSprites.end(), ropeSprites[i]);
		}


		ropeSprites.clear();
		tmpPoints.insert(tmpPoints.end(), vPoints[numPoints-1]);
		vPoints = tmpPoints;
		vSticks = tmpSticks;
		ropeSprites = tmpSprites;

		numPoints -= pointsCounter;

		if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
		{
			if (mJoint != NULL) {
				mWorld->DestroyJoint(mJoint);
				mJoint = NULL;
				//隐藏绳子的锚点
				if ( bodyA!=NULL) {
					if (bodyA->GetUserData()!=NULL) {
						//单独的动画统一先删除原有的，然后新添加sprite来执行动画
						RecordSprite* pTmp = ((RecordSprite*)bodyA->GetUserData());
						CCSprite* pNode = CCSprite::createWithTexture(pTmp->getTexture(), pTmp->getTextureRect());
						pNode->setPosition(pTmp->getPosition());
						pTmp->getParent()->addChild(pNode, pTmp->getZOrder());
						pNode->runAction(CCSequence::create(CCFadeTo::create(2.0f, 0), CCRemoveSelf::create(), NULL));

						((CCNode*)bodyA->GetUserData())->removeFromParent();
					}
				}
			}
		}

		// LIFeng 加入
		if (bodyA)
		{
			mWorld->DestroyBody(bodyA);
		}

		bodyA = NULL;

		mRopeLength = RopeSegmentFactor * (numPoints-1);

	}
}

void VRope::changeNum(CCNode* pNode)
{
	for (size_t i = 0; i < ropeSprites.size(); i++)
	{
		if (pNode == ropeSprites.at(i))
		{
			ropeSprites.erase(ropeSprites.begin()+i);
			break;
		}
	}
	--numPoints;
}

void VRope::updateWithPoints(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB, float dt) {
    
	if (numPoints <= 0)
	{
		return;
	}
	
    //manually set position for first and last point of rope
    vPoints[0]->setPos(pointA.x, pointA.y);
	//[[vPoints objectAtIndex:0] setPos:pointA.x y:pointA.y];
    vPoints[numPoints-1]->setPos(pointB.x, pointB.y);
	//[[vPoints objectAtIndex:numPoints-1] setPos:pointB.x y:pointB.y];
    
    //CCLog("pointA [%f,%f], pointB [%f,%f]", pointA.x, pointA.y, pointB.x, pointB.y);
	
	//update points, apply gravity
	for(int i=1;i<numPoints-1;i++) {
        //CCLog("1111111111points[%d] = [%f, %f]", i, vPoints[i].x, vPoints[i].y);
        vPoints[i]->applyGravity(dt);
		//[[vPoints objectAtIndex:i] applyGravity:dt];
        vPoints[i]->update();
		//[[vPoints objectAtIndex:i] update];
        //CCLog("1111111111points[%d] = [%f, %f]", i, vPoints[i].x, vPoints[i].y);
	}
	
	//contract sticks
	int iterations = 4;
	for(int j=0;j<iterations;j++) {
		for(int i=0;i<numPoints-1;i++) {
            //CCLog("222222222222points[%d] = [%f, %f]", i, vPoints[i].x, vPoints[i].y);
            vSticks[i].contract();
			//[[vSticks objectAtIndex:i] contract];
            //CCLog("222222222222points[%d] = [%f, %f]", i, vPoints[i].x, vPoints[i].y);
		}
	}
}

void VRope::updateSprites() {
	if(spriteSheet!=NULL) {
		for(int i=0;i<numPoints-1;i++) {
            VPoint* point1 = vSticks[i].getPointA();
			//VPoint *point1 = [[vSticks objectAtIndex:i] getPointA];
            VPoint* point2 = vSticks[i].getPointB();
			//VPoint *point2 = [[vSticks objectAtIndex:i] getPointB];
			CCPoint point1_ = ccp(point1->x,point1->y);
			CCPoint point2_ = ccp(point2->x,point2->y);
			float stickAngle = ccpToAngle(ccpSub(point1_,point2_));
            RecordSprite* tmpSprite = ropeSprites[i];
			//CCSprite *tmpSprite = [ropeSprites objectAtIndex:i];

            tmpSprite->setPosition(ccpMidpoint(point1_,point2_));
			//[tmpSprite setPosition:ccpMidpoint(point1_,point2_)];
            tmpSprite->setRotation(-CC_RADIANS_TO_DEGREES(stickAngle));
			//[tmpSprite setRotation: -CC_RADIANS_TO_DEGREES(stickAngle)];
		}
	}	
}

GLubyte VRope::getOpacity(){
    return mSpritesOpacity;
}

void VRope::debugDraw() {
    /*
	//Depending on scenario, you might need to have different Disable/Enable of Client States
	//glDisableClientState(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	//set color and line width for ccDrawLine
	glColor4f(0.0f,0.0f,1.0f,1.0f);
	glLineWidth(5.0f);
	for(int i=0;i<numPoints-1;i++) {
		//"debug" draw
		VPoint *pointA = [[vSticks objectAtIndex:i] getPointA];
		VPoint *pointB = [[vSticks objectAtIndex:i] getPointB];
		ccDrawPoint(ccp(pointA.x,pointA.y));
		ccDrawPoint(ccp(pointB.x,pointB.y));
		//ccDrawLine(ccp(pointA.x,pointA.y),ccp(pointB.x,pointB.y));
	}
	//restore to white and default thickness
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glLineWidth(1);
	//glEnableClientState(GL_TEXTURE_2D);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
     */
}

VRope::VRope(){
	mSpritesOpacity = 255;
	ropeAchorSprite = NULL;
	this->setTag(Tag_Box_Rope);
}

VRope::~VRope(){
    destroiyRope();
}
/*
void VRope::dealloc() {
	for(int i=0;i<numPoints;i++) {
		[[vPoints objectAtIndex:i] release];
		if(i!=numPoints-1)
			[[vSticks objectAtIndex:i] release];
	}
	[vPoints removeAllObjects];
	[vSticks removeAllObjects];
	[vPoints release];
	[vSticks release];
	[super dealloc];
}
*/








