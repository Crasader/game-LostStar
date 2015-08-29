//
//  CustomeSprite.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-10-25.
//
//

#include "CustomeSprite.h"
#include "BaseContentLayer.h"
#include "BaseContentWrapLayer.h"
#include "Music.h"
#include <typeinfo>
#include "SpriteHelper.h"
#include "BackgroudLayerFactory.h"
#include "SpriteHelper.h"
#include "AchievementManager.h"
#include "LFBox2dHelper.h"
#include "BaseScense.h"
#include "Recorder.h"
#include <vector>
#include "AppMacros.h"
#include "LevelManager.h"

extern const float g_PlaySpeedScale;

#define USE_PARTICLE_BOILER 1

RecordSprite::RecordSprite()
{
	if (BaseContentLayer* player = BaseContentLayer::shareBaseContentLayer())
	{
		//（不管是否记录，都需要执行添加操作，因为记录和不记录使用的是一套）
		int icout = player->getCurRecordCount();
		player->setCurRecordCount(icout+1);
		m_SaveData.id = icout+1;

		player->addToRecordSpriteVec(this);
	}
}

void RecordSprite::onEnter()
{
	CCSprite::onEnter();
}

RecordSprite::~RecordSprite()
{
 	if (BaseContentLayer* player = BaseContentLayer::shareBaseContentLayer())
 	{
// 		//不是当前场景中的元素直接return,防止切换场景的时候崩溃
// 		bool bCurScene = false;
// 		for (int i = 0; i < player->getAllNeedRecordSpriteVec()->size(); i++)
// 		{
// 			if (this == player->getAllNeedRecordSpriteVec()->at(i))
// 			{
// 				bCurScene = true;
// 				break;
// 			}
// 		}
// 		if (!bCurScene)
// 		{
// 			return;
// 		}
		//添加该元素到将要移除的数组，用于下一帧从当前记录数组里面移除，保持同步
		//（不管是否记录，都需要执行移除操作，因为记录和不记录使用的是一套）
 		player->addToWillBeRemoveRecordSpriteVec(this);
 		if (player->getCurPlayMode() == playMode_Normal && player->getRecord())
 		{
			//当要记录时，注册销毁事件
			player->recordEventID(this, eventID_Remove);
 		}
 	}
}

CustomeSprite::CustomeSprite()
{
    resMgr = ResManager::getManager();
    userData = NULL;
    anchorBody = NULL;
    myShadow = NULL;
	mIsKeyItem = false;
	mIsUsedByUser = false;

    fragmentBase = NULL;
	fragmentVec.clear();
}

CustomeSprite* CustomeSprite::create(const char *pszFileName)
{
	CustomeSprite* s = new CustomeSprite();
	s->initWithFile(pszFileName);
	s->autorelease();
	return s;
}

CustomeSprite* CustomeSprite::createWithTexture(CCTexture2D* pTexture, const CCRect& rect)
{
	CustomeSprite* s = new CustomeSprite();
	s->initWithTexture(pTexture, rect);
	s->autorelease();
	return s;
}


CustomeSprite::~CustomeSprite()
{
	removeShadowIfPosible();
	CC_SAFE_RELEASE(fragmentBase);
}

bool CustomeSprite::initWithFile(const char *pszFilename){
    
    mFileName = pszFilename;

	// 先看是否可以用frame来初始化，用于那些打到纹理图册的
	{
		string fileName = mFileName.substr(mFileName.find_last_of("/")+1);
		if (CC_CONTENT_SCALE_FACTOR() == 2)	// 高清
		{
			fileName = SpriteHelper::replace_all_distinct(fileName,".png","-ipadhd.png");
		}
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName.c_str());
		
		if (frame)
			{
				mSpriteFrame = frame;
				return initWithSpriteFrame(frame);
			}
	}
    
    return CCSprite::initWithFile(pszFilename);
};
void CustomeSprite::addFragment_ResMgr(const char* fragmentFileName,CCPoint pos,float angle)
{
	//addFragment(ResManager::getManager()->getSharedFilePath(fragmentFileName).c_str(),pos,angle);
	string tempName = fragmentFileName;
	if (CC_CONTENT_SCALE_FACTOR() == 2)	// 高清
	{
		fragmentFileName = SpriteHelper::replace_all_distinct(tempName,".png","-ipadhd.png").c_str();
	}
	// 如果是用了TP纹理图册，就不在使用路径了
	addFragment(fragmentFileName,pos,angle);
}

void CustomeSprite::splitSpriteToFragment()
{
	CCRect frameRect = mSpriteFrame->getRect();
	CCPoint pixOff = mSpriteFrame->getOffsetInPixels();
	CCSize size = frameRect.size; //this->getContentSize();
	
	//随机分图，分成多个sprite
	int cw = (int)(3.0f + CCRANDOM_0_1() * (float)(size.width/48));
	int ch = (int)(3.0f + CCRANDOM_0_1() * (float)(size.height/48));
	int iw = size.width / cw;
	int ih = size.height / ch;

	
	int x = 0;
	int y = 0;
	int offX = frameRect.getMinX();
	int offY = frameRect.getMinY();
	const float OFF_CELL = 0.3f;
	for (int i = 0; i< ch; i++)
	{	
		int nextY = (int)((float)(i * ih) + (float)( (float)ih * OFF_CELL + (float)ih * (1.0f - OFF_CELL) * CCRANDOM_0_1()));
		if( i == ch - 1)
		{
			nextY = size.height - 1;
		}
		int nextX = 0;
		for (int j = 0; j < cw; j++)
		{
			x = nextX;
			nextX = (int)((float)(j * iw) + (float)( (float)iw * OFF_CELL + (float)iw * (1.0f - OFF_CELL) * CCRANDOM_0_1()));
			if(j == cw - 1)
			{
				nextX = size.width - 1;
			}
			CCSprite* frag = CCSprite::create();
			int w = nextX - x;
			int h = nextY - y;
			int tx = x;
			int ty = y;
			
			int oy = (y + h / 2);
			if(mSpriteFrame->isRotated())
			{
				int temp = tx;
				tx = ty;
				ty = temp;
			}
			else
			{
				oy = size.height - oy;
			}
			frag->initWithTexture(getTexture(), CCRectMake(offX + tx, offY + ty, w, h), mSpriteFrame->isRotated());
			addFragmentSprite(frag, ccp(x + w / 2 + pixOff.x, oy + pixOff.y), 0);
		}
		y = nextY;
	}
}
void CustomeSprite::addFragment(const char* fragmentFileName,CCPoint pos,float angle)
{
	//CCSprite* fragment = CCSprite::create(fragmentFileName);
	CCSprite* fragment = CCSprite::createWithSpriteFrameName(fragmentFileName);
	addFragmentSprite(fragment, pos, angle);
}

void CustomeSprite::addFragmentSprite(CCSprite* fragment,CCPoint pos,float angle)
{
	// 还没有基地，就创建一个
	if (fragmentBase == NULL)
	{
		// 创建碎片的基地,使用纹理图册的情况下getTexture 不对
		//fragmentBase = CCSprite::create(getTexture());
		fragmentBase = CCSprite::create();
		fragmentBase->setContentSize(CCSizeMake(getContentSize().width,getContentSize().height));

		fragmentBase->setOpacity(0);
		fragmentBase->retain();

		fragmentBase->setVisible(false);
		fragmentBase->setScale(0.95f);
	}

	fragmentBase->addChild(fragment);
	fragment->setPosition(pos);
	//fragment->setRotation(angle);

	fragmentVec.insert(fragmentVec.end(),fragment);
}
void CustomeSprite::fragmentBomb()
{
	fragmentBomb(ccp(getContentSize().width/2,getContentSize().height/2));
}
void CustomeSprite::pauseFragmentBomb()
{
	if (fragmentBase)
	{
		SpriteHelper::pauseNodeAndChildren(fragmentBase);
	}
	
}
void CustomeSprite::resumeFragmentBomb()
{
	if (fragmentBase)
	{
		SpriteHelper::resumeNodeAndChildren(fragmentBase);
	}
}

//木头破碎动画
void CustomeSprite::fragmentBomb(CCPoint relativeBombCenterPos)
{
	if (fragmentBase == NULL)
	{
		CCLog("------> Waring, sprite %s have no fragment to Bomb!",mFileName.c_str());
		return;
	}
	
	// 加到背景层
// 	//CCLayer* bg = BackgroudLayerFactory::getCurrentBgLayer();
// 	BaseScense* scene = BaseScense::getCurrentScene();
// 	CCLayer* bg = scene->getBackgroudCoverLayer();
// 	bg->addChild(fragmentBase,0,Tag_Fragment);
//	//getParent()->addChild(fragmentBase);
	this->getParent()->addChild(fragmentBase, -1,Tag_Fragment);
	fragmentBase->setRotation(getRotation());
	fragmentBase->setPosition(getPosition());
	fragmentBase->setVisible(true);

	// 计算炸开的效果因子，对于最大距离使用22的体验比较好，所以，都把他们缩到这个区间
	float scale = 1.0f;
	float maxDistance = 0.0f;
	float temp = maxDistance;
	for (size_t i=0;i<fragmentVec.size();i++)
	{
		temp = ccpDistanceSQ(relativeBombCenterPos,fragmentVec[i]->getPosition());
		if ( temp > maxDistance)
			maxDistance = temp;
	}
	maxDistance = powf(maxDistance,0.5f);
	scale = 22.0f/maxDistance;
	
	//CCLog("-------fragment bomb out power scale = %f , max distance = %f",scale,maxDistance);

	// 炸开
	for (size_t i=0;i<fragmentVec.size();i++)
	{
		CCPoint vectorCenterToMe = ccpMult(ccpSub(fragmentVec[i]->getPosition(),relativeBombCenterPos),scale);
		CCRotateBy* rotate= CCRotateBy::create(10.0f,360 * CCRANDOM_0_1());
		CCMoveTo* move= CCMoveTo::create(10.0f,ccpMult(vectorCenterToMe,(CCRANDOM_0_1()*20+1)));
		CCFadeTo* fade = CCFadeTo::create(1.2f,0);
		fragmentVec[i]->runAction(rotate);
		fragmentVec[i]->runAction(move);
		fragmentVec[i]->runAction(CCEaseIn::create(fade,2));

		//CCLog("vectorCenterToMe = [%f,%f]",vectorCenterToMe.x,vectorCenterToMe.y);
	}

	// 下坠
	float griverty = 100.0f;													// 重力
	float fallTime = powf(( 2 * fragmentBase->getPositionY() / griverty),0.5f);	// 时间比是平方根关系
	fragmentBase->runAction(CCSequence::create(
		CCDelayTime::create(0.1f),												// 延迟0.1s之后就下坠
		CCEaseIn::create(CCMoveTo::create(fallTime/2,ccp(fragmentBase->getPositionX(),-60)),3),
		//CCCallFuncN::create(this,SEL_CallFuncN(&BaseContentLayer::removeNode)),
		CCRemoveSelf::create(),
		NULL));

	//fragmentBase->runAction(CCSequence::create(
	//	CCDelayTime::create(8.0f),												
	//	CCCallFuncN::create(this,SEL_CallFuncN(&BaseContentLayer::removeNode))
	//	,NULL));

	// 粒子效果
	{
		//减少低端机的粒子效果
		int iCount = 100;
// 		if (getMachinePerformanceLevel() <= 0)
// 		{
// 			iCount = iCount/2;
// 		}
		
		ResManager* manager = ResManager::getManager();
		CCParticleSystem* m_emitter = new CCParticleFire();
		// 48*48 的面积 用100个
		float num = getContentSize().width*getContentSize().height / (48*48) * iCount;
		m_emitter->initWithTotalParticles(int(num));
		m_emitter->setBlendAdditive(false);
		m_emitter->setAutoRemoveOnFinish(true);
		m_emitter->setDuration(0.5f);
		getParent()->addChild(m_emitter, 10);
		m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(manager->getSharedFilePath(g_gameinPath+"fire.png").c_str()));
		m_emitter->setPosition(getPosition());
		m_emitter->setPosVar(ccp(getContentSize().width/2,getContentSize().height/2));
		m_emitter->setStartColor(ccc4f(0.8f,0.8f,0.8f,0.1f));
		m_emitter->setEndColor(ccc4f(0.65f,0.65f,0.65f,0.03f));
		m_emitter->setEndColorVar(ccc4f(0.0f,0.0f,0.0f,0.0f));
		m_emitter->setLife(0.8f);
		m_emitter->setLifeVar(0.3f);
		m_emitter->setSpeed(30.0f);
		m_emitter->setSpeedVar(20.0f);
		m_emitter->setScale(getScale());

		m_emitter->setRotation(getRotation());
	}


	// 声音
	{
		float ran = CCRANDOM_0_1();
		string file = "box_bomb_big1.wav";
		if (getContentSize().width > 48 && getContentSize().height > 48)
		{
			if (ran >= 0.66f)
			{
				file = "box_bomb_big1.wav";
			}
			else if (ran >= 0.33f)
			{
				file = "box_bomb_big2.wav";
			}
			else
			{
				file = "box_bomb_big3.wav";
			}
		}else
		{
			if (ran >= 0.66f)
			{
				file = "box_bomb_small1.wav";
			}
			else if (ran >= 0.33f)
			{
				file = "box_bomb_small2.wav";
			}
			else
			{
				file = "box_bomb_small3.wav";
			}
		}
		
		Music::sharedMusicPlayer()->playEffectSound(file);
	}
}
bool CustomeSprite::isKeyItem()
{
	return mIsKeyItem;
}
void CustomeSprite::setIsKeyItem(bool key)
{
	mIsKeyItem = key;

	if (mIsKeyItem)
	{
		//setColor(ccBLACK);
	}

}
void CustomeSprite::removeNode(CCNode* node)
{
	if (node)
		node->removeFromParentAndCleanup(true);
}

void CustomeSprite::setUseedByUser()
{
	mIsUsedByUser = true;
}
bool CustomeSprite::isUseedByUser()
{
	return mIsUsedByUser;
}
std::string CustomeSprite::getFileName(){
    return mFileName;
};

void CustomeSprite::setData(CustomeSprite *data)
{
    userData = data;
}
CustomeSprite* CustomeSprite::getData()
{
    return userData;
}
void CustomeSprite::setAnchorBody(b2Body *anchor)
{
    this->anchorBody = anchor;
}

b2Body* CustomeSprite::getAnchorBody()
{
    return anchorBody;
}

void CustomeSprite::setMyShadow(CustomeSprite *shadow)
{
    this->myShadow = NULL;//shadow;
}
CustomeSprite* CustomeSprite::getMyShadow()
{
    return NULL;//myShadow;
}

void CustomeSprite::removeShadowIfPosible()
{
    if (myShadow!=NULL) {
        myShadow->removeFromParentAndCleanup(true);
        myShadow = NULL;
    }
}

//在销毁他的相关的body时候会调用
void CustomeSprite::destoryBody()
{
    
}




//=======================================
// 马达
//=======================================
MotorSprite::MotorSprite(bool handleAble,int seasonId,bool moveAble)
{
    joint = NULL;
    anchorBody = NULL;
    innerSpeed = realySpeed = transSpeed = 0.0f;
	stopState = NULL;
	m_bStop = false;
	//if (handleAble) {
	//	initWithFile(ResManager::getManager()->getSharedFilePath("turn_anchor_on_behind.png").c_str());
	//	transMotor = new CustomeSprite();
	//	transMotor->initWithFile(ResManager::getManager()->getSharedFilePath("turn_anchor_on_button.png").c_str());

	//	stopState = new CustomeSprite();
	//	stopState->initWithFile(ResManager::getManager()->getSharedFilePath("turn_anchor_on_button_select.png").c_str());
	//	transMotor->addChild(stopState);
	//	stopState->setPosition(ccp(transMotor->getContentSize().width/2,transMotor->getContentSize().height/2));
	//	stopState->setVisible(false);
	//}else{

	//	initWithFile(ResManager::getManager()->getSharedFilePath("turn_anchor_no_behind.png").c_str());
	//	transMotor = new CustomeSprite();
	//	transMotor->initWithFile(ResManager::getManager()->getSharedFilePath("turn_anchor_no_button.png").c_str());
	//}
	//transMotor->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
	//addChild(transMotor);
	//transMotor->release();
	//CC_SAFE_RELEASE(stopState);
	//rulable = handleAble;
	//schedule(schedule_selector(MotorSprite::tick));

	if (!moveAble)
	{
		transMotor = new CustomeSprite();
		transMotor->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"turn_anchor_no_circle_behind.png").c_str());
	}else
	{
		transMotor = new CustomeSprite();
		transMotor->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"turn_anchor_no_track_behind.png").c_str());
	}

	if (handleAble) {
		initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"turn_anchor_on_button.png").c_str());
		stopState = new CustomeSprite();
		stopState->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"turn_anchor_on_button_select.png").c_str());
		addChild(stopState, 0, Tag_Mator_Turn_Anchor);
		stopState->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
		stopState->setVisible(false);
	}else{
		initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"turn_anchor_no_button.png").c_str());
	}

    
	// 加到背景层去
	//BackgroudLayerFactory::getCurrentBgLayer()->addChild(transMotor,1);
	BaseScense* scene = BaseScense::getCurrentScene();
	CCLayer* bgCover = scene->getBackgroudCoverLayer();
	bgCover->addChild(transMotor,1);
    transMotor->release();

	CC_SAFE_RELEASE(stopState);
    rulable = handleAble;
    
    schedule(schedule_selector(MotorSprite::tick));
}
void MotorSprite::setBgGearScale(float scale)
{
	if (transMotor)
		transMotor->setScale(scale);
}
void MotorSprite::tick(float time)
{
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() != playMode_Normal)
	{
		return;
	}
	//把背景层中的他设置位置
	if(transMotor)
		transMotor->setPosition(getPosition());

    //先把角度归一化
    {
        //if (innerMotor->getRotation() >= 360 || innerMotor->getRotation()<=-360) {
        //    innerMotor->setRotation(0);
        //}
        if (transMotor && (transMotor->getRotation() >= 360 || transMotor->getRotation()<=-360) ) {
            transMotor->setRotation(0);
        }
        if (this->getRotation() >= 360 || this->getRotation()<=-360) {
            this->setRotation(0);
        }
    }
    
    //开始旋转
    {
        //innerMotor->setRotation(innerMotor->getRotation() + innerSpeed);
		if(transMotor)
			transMotor->setRotation(transMotor->getRotation() + transSpeed);
        this->setRotation(this->getRotation() + realySpeed);
    }
    
	if (BaseContentLayer::shareBaseContentLayer()->getRecord())
	{
		//更新旋转的速度为马达的真正属性。
		{
			if (joint!=NULL) {
				setRealyMotorSpeed(joint->GetMotorSpeed());
			}
		}
	}
    
}

MotorSprite* MotorSprite::createMotor(bool handleAble,int seasonId,bool moveAble)
{
    MotorSprite* motor = new MotorSprite(handleAble,seasonId,moveAble);
    motor->autorelease();
    return motor;
}
void MotorSprite::onExit()
{
	CCSprite::onExit();

}
//切换状态
void MotorSprite::swith(bool mute/* = false*/)
{
    if ( abs(joint->GetMotorSpeed()) <= 0.0f) {
        remuseRotation();
    }else{
        stopRotation();
    }
	if (!mute)
	{
		Music::sharedMusicPlayer()->playEffectSound("motor_sound.wav");
	}
}

void MotorSprite::stopRotation()
{
	m_bStop = true;
	//setColor(ccGRAY);
	//transMotor->setColor(ccGRAY);
	this->setRealyMotorSpeed(0.0f);
	if (stopState)
	{
		//点击暂停按钮后，会设置透明度为0
		//需要恢复透明度为255
		stopState->setOpacity(255);
		stopState->setVisible(true);
	}
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
	{
		if (rulable)
		{
			if (joint!=NULL)
			{
				joint->SetMotorSpeed(0.0f);
			}
		}
		if (BaseContentLayer::shareBaseContentLayer()->getRecord())
		{
			BaseContentLayer::shareBaseContentLayer()->recordEventID(this, eventID_Motor_Off);
		}
	}
}
void MotorSprite::remuseRotation()
{
	m_bStop = false;
    //setColor(ccWHITE);
    //transMotor->setColor(ccWHITE);
	if (stopState)
	{
		//点击暂停按钮后，会设置透明度为0
		//需要恢复透明度为255
		stopState->setOpacity(255);
		stopState->setVisible(false);
	}
	this->setRealyMotorSpeed(jointBornSpeed);
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
	{
		if (rulable)
		{
			if (joint!=NULL) 
			{
				joint->SetMotorSpeed(jointBornSpeed);
			}
		}
		if (BaseContentLayer::shareBaseContentLayer()->getRecord())
		{
			BaseContentLayer::shareBaseContentLayer()->recordEventID(this, eventID_Motor_On);
		}
	}
}

void MotorSprite::setJoint(b2RevoluteJoint* j)
{
    this->joint = j;
    
    if (joint!=NULL) {
        jointBornSpeed = joint->GetMotorSpeed();
    }else
    {
        jointBornSpeed = 0;
		if (transMotor)
		{
			transMotor->removeFromParentAndCleanup(true);
		}
		transMotor = NULL;
    }
}
void MotorSprite::destoryBody()
{
	setJoint(NULL);
    joint = NULL;
    anchorBody = NULL;
}

//设置马达的转速，其实就是中心轴的转动速度，这个速度最快
void MotorSprite::setMotorSpeed(float speed)
{
    innerSpeed = -speed;
    
    transSpeed = (realySpeed+innerSpeed)/2;
}
//设置马达的真正的转速，也就是实际上带动的物体的转速，即马达实际的转动，就是带动物体的那一层，这个速度最慢
//测试发现，joint->GetMotorSpeed()就是当时设置的速度，也就是realySpeed也是innerSpeed这么多。
void MotorSprite::setRealyMotorSpeed(float speed)
{
    realySpeed = -speed;
    
    transSpeed = (realySpeed+innerSpeed)/2;
}

bool MotorSprite::isClickMe(CCPoint glPoint)
{
    return boundingBox().containsPoint(glPoint);
}

bool MotorSprite::handlable()
{
    return rulable;
}

CCRect MotorSprite::boundingBox()
{
    CCRect bound = CustomeSprite::boundingBox();
    bound.origin.x -= BasicLength/2;
    bound.origin.y -= BasicLength/2;
    bound.size.width += BasicLength;
    bound.size.height += BasicLength;

	//CCRect bound = CustomeSprite::boundingBox();
	//bound.origin.x = getPositionX() - BasicLength;
	//bound.origin.y = getPositionY() - BasicLength;
	//bound.size.width == BasicLength*2;
	//bound.size.height == BasicLength*2;
    return bound;
}

//=======================================
// 弹性齿轮
//=======================================

SpringGear::~SpringGear()
{
    unscheduleAllSelectors();
}

void SpringGear::tick(float time)
{
//    if (joint!=NULL) {
//        float angle = joint->GetReferenceAngle();
//        gear_5->setRotation(CC_RADIANS_TO_DEGREES(angle));
//        gear_4->setRotation(CC_RADIANS_TO_DEGREES(angle)*0.4);
//        gear_3->setRotation(CC_RADIANS_TO_DEGREES(angle)*0.2);
//        gear_2->setRotation(CC_RADIANS_TO_DEGREES(angle)*0.1);
//        this->setRotation(0.0f);
//    }
    
    if (rJoint!=NULL) {
        float angle = rJoint->GetJointAngle();
        float angle_dregree = -CC_RADIANS_TO_DEGREES(angle);
        gear_5->setRotation(angle_dregree);
        gear_4->setRotation(angle_dregree*0.4);
        gear_3->setRotation(angle_dregree*0.2);
        gear_2->setRotation(angle_dregree*0.1);
        this->setRotation(0.0f);
        
        if (bunableBody!=NULL) {

            bunableBody->SetAngularDamping(1.0f);
            bunableBody->ApplyAngularImpulse(angle_dregree*k);
        }
        
    }
}
SpringGear::SpringGear(float k)
{
    initWithFile(resMgr->getSharedFilePath(g_gameinPath+"elasticity_gear_1.png").c_str());
    
    this->k = k;
    
    rJoint = NULL;
    anchorBody = NULL;
    bundableShape = NULL;
    bunableBody = NULL;
    
    gear_5 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"elasticity_gear_5.png").c_str());//最里面
    gear_4 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"elasticity_gear_4.png").c_str());
    gear_3 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"elasticity_gear_3.png").c_str());
    gear_2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"elasticity_gear_2.png").c_str());
    
    CCPoint center = CCPointMake(getContentSize().width/2, getContentSize().height/2);
    
    addChild(gear_5,5);
    addChild(gear_4,4);
    addChild(gear_3,3);
    addChild(gear_2,2);
    
    gear_5->setPosition(center);
    gear_4->setPosition(center);
    gear_3->setPosition(center);
    gear_2->setPosition(center);
    
    schedule(schedule_selector(SpringGear::tick));
}
void SpringGear::destoryBody()
{
//    joint = NULL;
    rJoint = NULL;
}
void SpringGear::setRotationJoint(b2RevoluteJoint *joint)
{
    rJoint = joint;
}
//void SpringGear::setWeldJoint(b2WeldJoint* joint)
//{
//    this->joint = joint;
//}





//=======================================
// 弹性滑轨
//=======================================
void SpringSlide::tick(float time)
{
    if (joint!=NULL)
    {
        float distacne = joint->GetJointTranslation();
        sprite3->setRotation(distacne*1000);
        if (bundableBody!=NULL) {
            
            float angle = joint->GetReferenceAngle();
            
            float force = -distacne*k*10;
//            if ( abs(force) < 100 ) {
//                return;
//            }
            b2Vec2 i;
            i.Set( sin(CC_RADIANS_TO_DEGREES(angle))*force / 60, cos(CC_RADIANS_TO_DEGREES(angle))*force / 60 );
            
            bundableBody->SetLinearDamping(1.0f);
            
            bundableBody->ApplyLinearImpulse(i, bundableBody->GetWorldCenter());
        }
    }
}
SpringSlide::SpringSlide(float k)
{
    joint = NULL;
    bundableBody = NULL;
    
    this->k = 200;
    this->k = k;
    
    initWithFile(resMgr->getSharedFilePath(g_gameinPath+"pulley_1.png").c_str());
    sprite2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"pulley_2.png").c_str());
    sprite3 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"pulley_3.png").c_str());
    
    CCPoint center = CCPointMake(getContentSize().width/2, getContentSize().height/2);
    
    sprite2->setPosition(center);
    sprite3->setPosition(center);
    
    addChild(sprite2,2);
    addChild(sprite3,3);
    
    schedule(schedule_selector(SpringSlide::tick));
}

void SpringSlide::setPrismaticJoint(b2PrismaticJoint* j)
{
    joint = j;
}

void SpringSlide::destoryBody()
{
    joint = NULL;
}


//=======================================
// 黑洞
//=======================================

DarkHoleBombExplosionCache* cache = NULL;

DarkHoleBombExplosionCache::DarkHoleBombExplosionCache()
{
	ResManager* resMgr = ResManager::getManager();
	texture = CCTextureCache::sharedTextureCache()->addImage(resMgr->getSharedFilePath(g_gameinPath+"blackhole.png").c_str());
	texture->retain();

// 	// 保存3个
// 	for (int i=0;i<1;i++)
// 	{
// 		newOne();
// 	}
};

DarkHoleBombExplosionCache::~DarkHoleBombExplosionCache()
{
	texture->release();
}

void DarkHoleBombExplosionCache::newOne()
{
	ResManager* resMgr = ResManager::getManager();
	int x_grid = 40;
	int y_grid = 40;

	RecordSprite* sprite = new RecordSprite;
	sprite->autorelease();
	sprite->initWithFile( resMgr->getSharedFilePath(g_gameinPath+"blackhole.png").c_str() );
	CCGrid3DAction* twirl = LFTwirl::create(sprite->getContentSize().width/2,-1.0f,1.1f,CCSize(x_grid,y_grid), 1.5f);
	((LFTwirl*)twirl)->preCreatGrid(sprite);
	restoreOne(sprite,twirl);

	CCLog("DarkHoleBombExplosionCache  new one !");
}

DarkHoleBombExplosionCache* DarkHoleBombExplosionCache::getCache()
{
	if (!cache)
	{
		cache = new DarkHoleBombExplosionCache();
	}
	return cache;
}
// 会自动 retain的
void DarkHoleBombExplosionCache::restoreOne(RecordSprite* bombSprite,CCGrid3DAction* bombAction)
{
	for (size_t i=0;i<bombVec.size();i++)
	{
		if (bombVec[i].bombAction == bombAction || bombVec[i].bombSprite == bombSprite)
		{
			// 已经有了，不再保存进去
			return;
		}
	}

	bombAction->retain();
	bombSprite->retain();
	bombSprite->removeFromParentAndCleanup(false);

	int x_grid = 40;
	int y_grid = 40;
	// 重置网格
	{
 		CCGrid3D* grid = (CCGrid3D*)bombSprite->getGrid();
 		grid->calculateVertexPoints();
// 		int i, j;
// 		CCGrid3D* grid = (CCGrid3D*)bombSprite->getGrid();
// 		for (i = 0; i < x_grid + 1; ++i)
// 		{
// 			for (j = 0; j < y_grid + 1; ++j)
// 			{
// 				ccVertex3F v =grid->originalVertex(ccp(i, j));
// 				grid->setVertex(ccp(i,j),v);
// 			}
// 		}
	}
	((LFTwirl*)bombAction)->initWithPosition(bombSprite->getContentSize().width/2,-1.0f,1.1f,CCSize(x_grid,y_grid), 1.5f);


	DarkHoleBombExplosion temp;
	temp.bombAction = bombAction;
	temp.bombSprite = bombSprite;

	bombVec.insert(bombVec.end(),temp);

	CCLog("DarkHoleBombExplosionCache  restore one !");
}

//得到的是autorealease的
DarkHoleBombExplosion DarkHoleBombExplosionCache::getBombExplosion()
{
	if (bombVec.size() <= 0)
		newOne();

	DarkHoleBombExplosion one = *(bombVec.begin());
//	bombVec.erase(bombVec.begin());
	bombVec.clear();

	one.bombAction->autorelease();
	one.bombSprite->autorelease();
	one.bombSprite->removeAllChildrenWithCleanup(true);
	one.bombAction->setTarget(one.bombSprite);

	CCLog("DarkHoleBombExplosionCache  get one !");
	return one;
}



//创建一个黑洞炸弹，指定他的半径
DarkHoleBomb::DarkHoleBomb(float r,const char* filaName,const char* bombFileName)
{
    initWithFile(resMgr->getSharedFilePath(g_gameinPath+filaName).c_str());
    radius = r;
    r_power = radius*radius;
 	DarkHoleBombExplosion temp = DarkHoleBombExplosionCache::getCache()->getBombExplosion();
 	bombExplosion = temp.bombSprite;
	bombExplosion->retain();

	m_bBombed = false;
	bombOutExplosion = NULL;
	twirl = NULL;
	bombOutAction = NULL;
	par = NULL;

	twirl = temp.bombAction;
	twirl->retain();

    float halfWidth = bombExplosion->getContentSize().width/2;
    endScale = radius/halfWidth;
    bombExplosion->setScale(0.0f);

#if USE_BLACKHOLE_COVER
	bombCover = new RecordSprite;
	bombCover->initWithFile((resMgr->getSharedFilePath(g_gameinPath+"blackhole_cover.png").c_str() ));
	bombCover->autorelease();
//	bombCover = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"blackhole_cover.png").c_str());
	bombCover->setScale(0.0f);
	bombCover->retain();
#endif
	par = createFogParticle(this,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	par->setVisible(false);
	par->setTag(Tag_Box_DarkHoleBomb_Par);
}
bool DarkHoleBomb::isInMe(CCNode* item)
{
	CCPoint posItem = item->getPosition();
	CCPoint myPos = getPosition();
	float distacne_power = (myPos.x - posItem.x) * (myPos.x - posItem.x) + (myPos.y - posItem.y)*(myPos.y - posItem.y);
	if (distacne_power<=r_power) {
		return true;
	}
	return false;
}
//爆炸
void DarkHoleBomb::bomb(b2World* world)
{
	//取出那些会炸到的物体
	{
		immediatelyDestoryBody.clear();
		outRangeItemToDelete.clear();
		applayBombItems.clear();
		for (b2Body* b = world->GetBodyList(); b; /*b = b->GetNext()*/)
		{// 注意在这里面最好不要删除物体，因为很有可能，你删除的物体就是之前的body的Next
			if (b->GetUserData() != NULL)
			{
				CCNode* item = (CCNode*)b->GetUserData();

				//如果是使用了超能道具，则黑洞不能吸收红星
				if(item->getTag() == Tag_Box_RedStar)
				{
					BaseContentLayer* contentLayer = (BaseContentLayer*)item->getParent();
					if(contentLayer->isUsingSuperTool())
					{
						b = b->GetNext();
						continue;
					}
				}

				//指定的类型Tag_Box_Strobe Tag_Box_EMCannoon Tag_Box_Table Tag_Box_Table_Ineffective                
				//                if (item->getTag()!=Tag_Box_Table && item->getTag()!=Tag_Box_Table_Ineffective )
				if(b->GetType() == b2_dynamicBody)//只吸收动态物体
				{
					if (isInMe(item))
					{
						if (item!=this)
						{
							//待会删除
							setToBomb(item);
						}

						//删除他们的物理几何体，Sprite待会删除
						b2Body* tmp = b;
						b = b->GetNext();

						//是砸门的东西一律让闸门自己销毁
						if (item->getTag() == Tag_Box_Strobe_Head)
						{
							((Strobe*)((CustomeSprite*)item)->getData())->destoryHeadByDarkBomb(item, this);

						}else if (item->getTag() == Tag_Box_Strobe)
						{
							((Strobe*)item)->destoryBodyByDarkBomb(this);

						}else if(item->getTag() == Tag_Box_EMCannoon_Head)
						{
							((EMCannonSprite*)((CustomeSprite*)item)->getData())->destoryHeadByDarkBomb(item, this);

						}else if(item->getTag() == Tag_Box_EMCannoon)
						{
							((EMCannonSprite*)item)->destoryBodyByDarkBomb(this);
						}else if(item->getTag() == Tag_Box_EMCannoon)
						{
							((EMCannonSprite*)item)->destoryBodyByDarkBomb(this);
						}
						else//对于不需要销毁额外的body的物体就在下面处理
						{
							BaseContentLayer* layer = BaseContentLayer::shareBaseContentLayer();
							//如果吸入的是马达，那么把接受触摸的马达就去掉
							if(item->getTag() == Tag_Anchor_Mator)
							{
								BaseContentLayer::shareBaseContentLayer()->shapeMotorDestoried((MotorSprite*)item);
								layer->checkShapeWhenAnchorPointDestoryed((CustomeSprite*)item);
							}
							else if (item->getTag() == Tag_Anchor_SpringGear||
								item->getTag() == Tag_Anchor_SpringSlide)
							{
								layer->checkShapeWhenAnchorPointDestoryed((CustomeSprite*)item);
							}
							//else if (item->getTag() == Tag_Box_Boiler) {
							//	BaseContentLayer::shareBaseContentLayer()->removeBoiler((Boiler*)item);
							//}

							//对于是CustomeSprite的，都拿出他的Data，看他的Data是否是锚点，是的话把锚点也消除
							if ( typeid(*item) == typeid(CustomeSprite))
							{                                
								b2Body* anchor = BaseContentLayer::shareBaseContentLayer()->checkAnchorWhenShapeDestoryed((CustomeSprite*)item);
								if (anchor!=NULL) {
									if (anchor->GetUserData()!=NULL) {
										addOutRangeItem((CCNode*)anchor->GetUserData());
										anchor->SetUserData(NULL);
									}

									setToImmediatelyDelete(anchor);
								}
							}

							//                     //检查绳索
							//BaseContentLayer::shareBaseContentLayer()->checkRopesWhenBodyDestroied(tmp,true);
							b2Body* ropeAnchor = BaseContentLayer::shareBaseContentLayer()->checkRopesWhenBodyDestroied(tmp);
							if(ropeAnchor!=NULL && ropeAnchor->GetUserData()!=NULL)
							{//有绳子锚点，那么就先把物理的东西全部清楚
								//加入到待会消除的对列
								addOutRangeItem((CCNode*)ropeAnchor->GetUserData());
								ropeAnchor->SetUserData(NULL);
								setToImmediatelyDelete(ropeAnchor);
							}

							//                            //删除他们的物理几何体，Sprite待会删除
							tmp->SetUserData(NULL);
							world->DestroyBody(tmp);
						}


						item->stopAllActions();
						item->unscheduleAllSelectors();
						if (item->getTag() == Tag_Box_RedStar) {
							BaseContentLayer::shareBaseContentLayer()->redStarDestoried(item);
						}
						if (typeid(*item) == typeid(CustomeSprite)) {
							((CustomeSprite*)item)->destoryBody();
						}

						//                        //删除他们的物理几何体，Sprite待会删除
						//                        b2Body* tmp = b;
						//                        b = b->GetNext();
						//                        tmp->SetUserData(NULL);
						//                        world->DestroyBody(tmp);
						continue;
					}
				}
			}

			b = b->GetNext();
		}

		//删除立即删除的
		{
			for (size_t i=0; i<immediatelyDestoryBody.size(); i++)
			{
				//检查绳索
				BaseContentLayer::shareBaseContentLayer()->checkRopesWhenBodyDestroied(immediatelyDestoryBody[i],true);
				//BaseContentLayer::shareBaseContentLayer()->checkRopesWhenBodyDestroied(immediatelyDestoryBody[i]);
				immediatelyDestoryBody[i]->SetUserData(NULL);
				world->DestroyBody(immediatelyDestoryBody[i]);
			}
		}
		immediatelyDestoryBody.clear();
	}

	AchievementManager::getInstance()->blackHoleAbsorption(applayBombItems);

	playBombAnimation();
}

void DarkHoleBomb::playBombAnimation()
{
	Music::sharedMusicPlayer()->playEffectSound("black_hole_bomb.wav");

	m_bBombed = true;

	CCLayer* contentLayer = BaseScense::getCurrentScene()->getBaseContentWrapLayer()->getWrapper();
	float layerScl = contentLayer->getScale();
	CCPoint layerPos = contentLayer->getPosition();
	
	
	getParent()->addChild(bombExplosion,0);
	if (bombOutExplosion)
		getParent()->addChild(bombOutExplosion,0);

#if USE_BLACKHOLE_COVER
	getParent()->addChild(bombCover,0);
	bombCover->setPosition(getPosition());
	bombCover->setTag(Tag_DarkBomb_bombCover);
#endif

	CCPoint p = getPosition();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int dw = winSize.width / 2;
	int dh = winSize.height / 2;

	bombExplosion->setTag(Tag_DarkBomb_bombExplosion);
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_PlayGameRecord)
	{
		layerScl *= 0.68f;
//		bombExplosion->setScale(bombExplosion->getScale()*0.68f);
	}
		
	bombExplosion->setPosition( ccp( dw - ( dw - p.x ) * layerScl + layerPos.x, dh - ( dh - p.y ) * layerScl + layerPos.y ) );

	if (bombOutExplosion)
	{
		bombOutExplosion->setPosition(getPosition());
	}

	CCLOG("bombExplosion pos is {x = %f, y = %f}", bombExplosion->getPosition().x, bombExplosion->getPosition().y);

	// 确定了位置后计算
	((LFTwirl*)twirl)->preCalculateDistance();


	//黑洞先放大，然后停顿一下，然后就开始缩小了。
	CCScaleTo* scaleTo = CCScaleTo::create(0.15f, endScale);
	CCDelayTime* delay = CCDelayTime::create(0.8f);
	//CCCallFuncN* playBlackHoleAnim = CCCallFuncN::create(this, SEL_CallFuncN(&DarkHoleBomb::playBlackholeParticle));
	//CCRotateBy* rotate = CCRotateBy::create(0.8f*tt, 20.0f);
	CCCallFuncN* callBack = CCCallFuncN::create(this, SEL_CallFuncN(&DarkHoleBomb::bombAction));
	CCCallFuncN* callBack2 = CCCallFuncN::create(this, SEL_CallFuncN(&DarkHoleBomb::bombedOut));
	CCAction* action = CCSequence::create(scaleTo,callBack2, delay,callBack,NULL);

	//CCAction* action2 = CCSequence::create((CCFiniteTimeAction*)scaleTo->copy()->autorelease(),(CCFiniteTimeAction*)delay->copy()->autorelease(),NULL);
#if USE_BLACKHOLE_COVER
	CCAction* action2 = CCSequence::create(
		(CCFiniteTimeAction*)scaleTo->copy()->autorelease(),
		//CCFadeTo::create(1.3f,0),
		CCFadeTo::create(1.0f, 0),
		CCCallFuncN::create(this,SEL_CallFuncN(&DarkHoleBomb::removeNode)),
		NULL);

	bombCover->runAction(action2);
#endif
	//bombCover->runAction(CCRepeat::create(CCRotateBy::create(3.0f,360),-1));
	bombExplosion->runAction(action);

	bombExplosion->setOpacity(200);
	if (bombOutExplosion)
	{	
		bombOutExplosion->setOpacity(200);
		bombOutExplosion->setVisible(true);
		bombOutExplosion->runAction(bombOutAction);
		bombOutExplosion->setScale(endScale);
	}
	{
		CCParticleSystemQuad* m_emitter = CCParticleSystemQuad::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"BlackHoleParticle.plist").c_str());
		ccBlendFunc tBlendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
		m_emitter->setBlendFunc(tBlendFunc);
		m_emitter->setPositionType(kCCPositionTypeGrouped);
		bombExplosion->addChild(m_emitter,-1);
		m_emitter->setPosition(ccp(bombExplosion->getContentSize().width/2,bombExplosion->getContentSize().height/2));
		m_emitter->runAction(CCRotateBy::create(delay->getDuration()*2,720));
		m_emitter->setScale(0.3f);
		m_emitter->setBlendAdditive(true);

		// 防止崩溃
		if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
			m_emitter->retain();

		m_emitter->setVisible(false);
	}

	setOpacity(0);
	setVisible(false);
	if (getMyShadow())
	{
		getMyShadow()->setVisible(false);
	}

}

void DarkHoleBomb::playBlackholeParticle(CCNode* node)
{
	CCArmature* pArmature = CCArmature::create("xuanwo");
	pArmature->getAnimation()->play("xuanwo");
	node->addChild(pArmature);
	CCSize size = node->getContentSize();
	pArmature->setPosition(ccp(size.width/2, size.height/2 + 5));
	//ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
	//pArmature->setBlendFunc(blend);
}
void DarkHoleBomb::bombedOut(CCNode* node)
{
	CCLayer* contentLayer = BaseScense::getCurrentScene()->getBaseContentWrapLayer()->getWrapper();
	float layerScl = contentLayer->getScale();
	CCPoint layerPos = contentLayer->getPosition();
	float scale = 1.0f/bombExplosion->getScale();
	CCPoint pos = bombExplosion->getPosition();
	CCSize size =  bombExplosion->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int dw = winSize.width / 2;
	int dh = winSize.height / 2; 

	for (size_t i=0;i<applayBombItems.size();i++)
	{
		CCPoint wholdRelativedPoint = bombExplosion->convertToNodeSpace(applayBombItems[i]->getPosition());
		CCPoint posChild = applayBombItems[i]->getPosition();
		applayBombItems[i]->retain();
		applayBombItems[i]->removeFromParentAndCleanup(true);
		float itemScl = applayBombItems[i]->getScale() * scale * layerScl;
		applayBombItems[i]->setScale(itemScl);
		bombExplosion->addChild(applayBombItems[i], 0, Tag_In_DarkBomb);
		applayBombItems[i]->release();
				
		if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
		{
			applayBombItems[i]->setPosition(ccp((posChild.x - (dw - (dw - pos.x + layerPos.x)/layerScl)) * itemScl + size.width / 2, 
				(posChild.y - (dh - (dh - pos.y + layerPos.y)/layerScl)) * itemScl + size.height / 2) );
		}
		
		((CCSprite*)applayBombItems[i])->setColor(ccc3(65,65,65));
		//((CCSprite*)applayBombItems[i])->setOpacity( ((CCSprite*)applayBombItems[i])->getOpacity()*0.2f);
#if 0
		// 阴影
		CustomeSprite* shadow = ((CustomeSprite*)applayBombItems[i])->getMyShadow();
		if (!shadow)
		{
			continue;
		}
		shadow->retain();
		shadow->removeFromParentAndCleanup(true);
		shadow->setScale(shadow->getScale()*scale);
		bombExplosion->addChild(shadow);
		shadow->release();
		wholdRelativedPoint = bombExplosion->convertToNodeSpace(shadow->getPosition());
		shadow->setPosition(wholdRelativedPoint);
		//shadow->setOpacity(shadow->getOpacity()*0.4f);
#endif
	}
	
	// 让要接受爆炸的物体做动作
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
	{
		CCAction* action = CCMoveTo::create(8.0f,ccp(bombExplosion->getContentSize().width/2,bombExplosion->getContentSize().height/2));
		for (size_t i=0;i<applayBombItems.size();i++)
		{
			// 为了一体化的跟着黑洞做动画，我们就把他们变成黑洞的子物体
			// 于是下面的“阴影做动画”就不在用了
// 			//阴影做动画
// 			if (((CustomeSprite*)applayBombItems[i])->getMyShadow()) {
// 				((CustomeSprite*)applayBombItems[i])->getMyShadow()->runAction((CCFiniteTimeAction*)action->copy()->autorelease());
// 			}
			applayBombItems[i]->runAction((CCFiniteTimeAction*)action->copy()->autorelease());
		}
	}
}
void DarkHoleBomb::bombAction(CCNode* dark)
{
	bombExplosion->setVisible(true);

	CCCallFuncN* callBack = CCCallFuncN::create(this, SEL_CallFuncN(&DarkHoleBomb::actionEnd));

	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
	{
		for (size_t i=0; i<outRangeItemToDelete.size(); i++) {
			if (outRangeItemToDelete[i]!=NULL) {
				((CustomeSprite*)outRangeItemToDelete[i])->removeShadowIfPosible();
				outRangeItemToDelete[i]->removeFromParentAndCleanup(true);
			}
		}
	}
	else
	{
		outRangeItemToDelete.clear();
	}


	for (size_t i=0; i<applayBombItems.size(); i++) {

		//如果是电磁炮或者是闸门的头部,直接让他们自己销毁
		if (applayBombItems[i]->getTag() == Tag_Box_Strobe_Head)
		{
			((Strobe*)((CustomeSprite*)applayBombItems[i])->getData())->removeAllIfNeed();
		}
		else if (applayBombItems[i]->getTag() == Tag_Box_Strobe)
		{
			((Strobe*)applayBombItems[i])->removeAllIfNeed();

		}else if(applayBombItems[i]->getTag() == Tag_Box_EMCannoon_Head)
		{
			((EMCannonSprite*)((CustomeSprite*)applayBombItems[i])->getData())->removeAllIfNeed();
		}
		else if (applayBombItems[i]->getTag() == Tag_Box_EMCannoon)
		{
			((EMCannonSprite*)applayBombItems[i])->removeAllIfNeed();

		}
	}

	//bombCover->runAction(CCScaleTo::create(0.4f,0.0f));
	runAction(CCSequence::create(CCDelayTime::create(0.65f),CCCallFuncN::create(this,SEL_CallFuncN(&DarkHoleBomb::collapse)),NULL));
    
	bombExplosion->runAction(CCSequence::create(twirl,callBack,NULL));
	Music::sharedMusicPlayer()->playEffectSound("black_hole_sound.wav");
}
void DarkHoleBomb::actionEnd(CCNode* dark)
{
	bombExplosion->removeAllChildren();
// 	for (int i=0; i<applayBombItems.size(); i++) {
// //		((CustomeSprite*)applayBombItems[i])->removeShadowIfPosible();
// 		applayBombItems[i]->removeFromParentAndCleanup(true);
// 	}
	//bombCover->removeAllChildrenWithCleanup(true);
}
void DarkHoleBomb::collapse(CCNode* null)
{
	if (getMyShadow())
	{
		getMyShadow()->runAction(CCScaleTo::create(0.1f,0.0f));
		getMyShadow()->runAction(CCFadeTo::create(0.1f, 0));
	}
	runAction(CCFadeTo::create(0.1f,0));
	runAction(CCSequence::create(
		CCScaleTo::create(0.1f, 0.0f),
		CCCallFuncN::create(this,SEL_CallFuncN(&DarkHoleBomb::killSelf)),
		NULL));

}
void DarkHoleBomb::killSelf(CCNode* nullNul)
{
	removeFromParentAndCleanup(true);
}
void DarkHoleBomb::setToImmediatelyDelete(b2Body *toDelete)
{
    for (size_t i=0; i<immediatelyDestoryBody.size(); i++)
    {
        if (immediatelyDestoryBody[i] == toDelete)
        {
            return;
        }
    }
    immediatelyDestoryBody.insert(immediatelyDestoryBody.end(), toDelete);
}

void DarkHoleBomb::setToBomb(cocos2d::CCNode *item)
{
    for (size_t i=0; i<applayBombItems.size(); i++)
    {
        if (applayBombItems[i]==item)
        {
            return;
        }
    }
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
	{
		BaseContentLayer::shareBaseContentLayer()->recordEventID((RecordSprite*)item, eventID_setToBomb);
	}
    applayBombItems.insert(applayBombItems.end(), item);
}

void DarkHoleBomb::addOutRangeItem(cocos2d::CCNode *item)
{
    for (size_t i=0; i<outRangeItemToDelete.size(); i++)
    {
        if (item == NULL || outRangeItemToDelete[i]==item)
        {
            return;
        }
	}
	if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
	{
		BaseContentLayer::shareBaseContentLayer()->recordEventID((RecordSprite*)item, eventID_AddOutRangeItem);
	}
    outRangeItemToDelete.insert(outRangeItemToDelete.end(), item);
}


//创建一个粒子系统，并加入到item中，也返回这个粒子系统的指针
CCParticleSystem* DarkHoleBomb::createFogParticle(CCSprite* item,const char* fileName)
{
	CCParticleSmoke* par = new CCParticleSmoke();
	par->autorelease();
	int particalNum = 800;
	par->initWithTotalParticles(particalNum);
	par->setTexture( CCTextureCache::sharedTextureCache()->addImage(fileName));
	par->setAutoRemoveOnFinish(true);
	par->setEmitterMode(kCCParticleModeGravity);
	par->setPositionType(kCCPositionTypeGrouped);
	par->setGravity(CCPointZero);
	ccColor4F startColor;
	startColor.a = 0.005f;//0.005f;
	startColor.r = 211.0f/255.0f / 2.0f;//0.414
	startColor.g = 252.0f/255.0f / 2.0f;//0.494
	startColor.b = 253.0f/255.0f / 2.0f;//0.496

	ccColor4F endColor;
	endColor.a = 0.02f;//0.02f;
	endColor.r = 211.0f/255.0f / 2.0f;//0.414
	endColor.g = 252.0f/255.0f / 2.0f;//0.494
	endColor.b = 253.0f/255.0f / 2.0f;//0.496

	par->setStartColor(startColor);
	par->setEndColor(endColor);
	par->setStartSize(20.0f);
	par->setStartSizeVar(10.0f);
	par->setEndSize(80.0f);
	par->setEndSizeVar(20.0f);
	par->setTotalParticles(particalNum);

	par->setAngle(90.0f);
	par->setAngleVar(180.0f);

	par->setLife(2.3f);
	par->setLifeVar(0.5f);
	par->setSpeed(30.0f);
	par->setSpeedVar(5.0f);

	par->setTangentialAccel(0.0f);
	par->setTangentialAccelVar(0.0f);
	par->setRadialAccel(10.0f);
	par->setRadialAccelVar(0.0f);
	par->setDuration(-1);

	par->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
	par->setPosVar(CCPointZero);
	item->addChild(par);

	//防止崩溃
	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
		par->retain();

	return par;
}

DarkHoleBomb::~DarkHoleBomb()
{
	// 保存回去
//	DarkHoleBombExplosionCache::getCache()->restoreOne(bombExplosion,twirl);

    bombExplosion->release();
#if USE_BLACKHOLE_COVER
	bombCover->release();
#endif
	CC_SAFE_RELEASE(bombOutAction);
	CC_SAFE_RELEASE(bombOutExplosion);
	CC_SAFE_RELEASE(twirl);
    stopAllActions();
}



//=======================================
// 闸门：他的body的销毁请一定要让他自己来
//=======================================
Strobe::Strobe()
{
    world = NULL;
    myCenterBody = NULL;
    leftBody = NULL;
    rightBody = NULL;
    open = true;
    leftHead = rightHead = NULL;
    centerLength = 6;

	float time = 0.04f;
	{
		string files[] = {
			"strobe144_1.png",
			"strobe144_2.png",
			"strobe144_3.png",
			"strobe144_4.png"
		};
		openAction = (CCFiniteTimeAction*)SpriteHelper::createAction_pList(files,sizeof(files)/sizeof(string),time,false);
		openAction->retain();
	}

	{
		string left_files[] = {
			"strobehead_left_1.png",
			"strobehead_left_2.png",
			"strobehead_left_3.png",
			"strobehead_left_4.png"
		};
		leftHeadOpenAction = (CCFiniteTimeAction*)SpriteHelper::createAction_pList(left_files,sizeof(left_files)/sizeof(string),time,false);
		leftHeadOpenAction->retain();
	}

	{
		string right_files[] = {
			"strobehead_right_1.png",
			"strobehead_right_2.png",
			"strobehead_right_3.png",
			"strobehead_right_4.png"
		};
		rightHeadOpenAction = (CCFiniteTimeAction*)SpriteHelper::createAction_pList(right_files,sizeof(right_files)/sizeof(string),time,false);
		rightHeadOpenAction->retain();
	}
}
void Strobe::createHeadSprite(CCLayer* layer)
{
    
    leftHead = new CustomeSprite();
    leftHead->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"strobehead_left_1.png").c_str());
    rightHead = new CustomeSprite();
    rightHead->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"strobehead_right_1.png").c_str());
    
    layer->addChild(leftHead,0,Tag_Box_Strobe_Head);
    layer->addChild(rightHead,0,Tag_Box_Strobe_Head);
    
    //让两个头都带有自己的引用
    leftHead->setData(this);
    rightHead->setData(this);
    
    leftHead->release();
    rightHead->release();
}

b2Body* Strobe::createLeftHead()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    
    float posX = cos( CC_DEGREES_TO_RADIANS( 180+myDiscribt.angle) )*(centerLength/2+0.5);
    float posY = sin( CC_DEGREES_TO_RADIANS( 180+myDiscribt.angle) )*(centerLength/2+0.5);
    
//    bodyDef.position.Set(myDiscribt.point.x-3.5f, myDiscribt.point.y);
    bodyDef.position.Set(myDiscribt.point.x+posX, myDiscribt.point.y+posY);
    bodyDef.userData = leftHead;
    b2Body* bodyA = world->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        box.SetAsBox(0.5f, 1.0f, b2Vec2(0.0f, 0.0f), 0);//左边头
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = Fixture_Density;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        bodyA->CreateFixture(&fixtureDef);
        
        if (myDiscribt.angle != 0.0f) {
            bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(myDiscribt.angle));
        }
        
    }
    
    return bodyA;
}
b2Body* Strobe::createRightHead()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    
    float posX = cos( CC_DEGREES_TO_RADIANS( myDiscribt.angle) )*(centerLength/2+0.5);
    float posY = sin( CC_DEGREES_TO_RADIANS( myDiscribt.angle) )*(centerLength/2+0.5);
    
//    bodyDef.position.Set(myDiscribt.point.x+3.5f, myDiscribt.point.y);
    bodyDef.position.Set(myDiscribt.point.x+posX, myDiscribt.point.y+posY);
    bodyDef.userData = rightHead;
    b2Body* bodyA = world->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;

        box.SetAsBox(0.5f, 1.0f, b2Vec2(0.0f, 0.0f), 0);//左边头
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = Fixture_Density;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        bodyA->CreateFixture(&fixtureDef);
        
        if (myDiscribt.angle != 0.0f) {
            bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(myDiscribt.angle));
        }
        
    }
    
    return bodyA;
}
b2Body* Strobe::createCenterBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(myDiscribt.point.x, myDiscribt.point.y);
    bodyDef.userData = this;
    b2Body* bodyA = world->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
		//box.SetAsBox(centerLength/2, 0.5f, b2Vec2(0.0f, 0.0f), 0);//中间
		box.SetAsBox(centerLength/2, 0.25f, b2Vec2(0.0f, 0.25f), 0);//中间
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = Fixture_Density;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        bodyA->CreateFixture(&fixtureDef);
        
        if (myDiscribt.angle != 0.0f) {
            bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(myDiscribt.angle));
        }
        
    }
    
    return bodyA;
}
void Strobe::createBody(){
    
    leftBody = createLeftHead();
    rightBody = createRightHead();
    myCenterBody = createCenterBody();
}

void Strobe::timeToCloseDoor(float time)
{
    unschedule(schedule_selector(Strobe::timeToCloseDoor));
    closeTheDoor();
}
Strobe* Strobe::createStrobe(CCLayer* layerToHoldHead,b2World* w,GameToolsStrobe strobe)
{
    Strobe* s = new Strobe();
	s->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"strobe144_1.png").c_str());
    s->world = w;
    s->myDiscribt = strobe;
    s->createHeadSprite(layerToHoldHead);
    s->createBody();//就属于关闭
    s->open = false;
    s->autorelease();
    return s;
}
// open，没有中间的板子，就是false
// close，有中间的板子，就是true
bool Strobe::isOpened()
{
	return open;
}

bool ignoreMeForStrobe(b2Body* body)
{
	if (body && body->GetUserData())
	{
		CCNode* node = (CCNode*)body->GetUserData();
		if (node->getTag() == Tag_Box_Strobe ||
			node->getTag() == Tag_Box_Strobe_Head)
		{
			return true;
		}
	}
	return false;
}

bool Strobe::anyOtherBodyInLine()
{
	return LF::isAnyFixtureInLine(world,leftBody->GetWorldCenter(), rightBody->GetWorldCenter(),ignoreMeForStrobe,0.5f);
}
void Strobe::swicth()
{
    if (leftHead == NULL || rightHead==NULL) {
        return;
    }

    if (open) {
        
		if (anyOtherBodyInLine())
		{
			return;
		}        
        closeTheDoor();
    }else{
        openTheDoor();
    }
}

void Strobe::openTheDoor()
{
    if (open) {
        return;
    }
   
	myCenterBody->SetUserData(NULL);
    world->DestroyBody(myCenterBody);
	myCenterBody = NULL;

	//重新创建Body,防止box2d没有响应
	myCenterBody = createCenterBody();

	playStoreAnimation(true);

	BaseContentLayer::shareBaseContentLayer()->recordEventID(this, eventID_Strobe_On);
}

void Strobe::closeTheDoor()
{
    if (!open) {
        return;
    }
    
	myCenterBody->SetUserData(NULL);
	world->DestroyBody(myCenterBody);
	myCenterBody = NULL;

	//重新创建Body,防止box2d没有响应
    myCenterBody = createCenterBody();

	playStoreAnimation(false);

	BaseContentLayer::shareBaseContentLayer()->recordEventID(this, eventID_Strobe_Off);
}

void Strobe::playStoreAnimation(bool bOpen)
{
	open = bOpen;
	if (myShadow!=NULL) {
		myShadow->setVisible(!bOpen);
	}

	stopAllActions();

	getParent()->reorderChild(this,-1);
		
	int iFade = 255.0f;
	if (bOpen)
	{
		iFade = 150.0f;
		leftHead->runAction((CCFiniteTimeAction*)(leftHeadOpenAction->copy()->autorelease()));
		rightHead->runAction((CCFiniteTimeAction*)(rightHeadOpenAction->copy()->autorelease()));
	}
	else
	{
		leftHead->runAction(((CCFiniteTimeAction*)(leftHeadOpenAction->copy()->autorelease()))->reverse());
		rightHead->runAction(((CCFiniteTimeAction*)(rightHeadOpenAction->copy()->autorelease()))->reverse());
	}
	CCFadeTo* fade = CCFadeTo::create(0.2f, iFade);
	runAction(fade);

	playDoorAnimation(bOpen);
}

void Strobe::playDoorAnimation(bool bOpen)
{
	if (bOpen)
	{
		Music::sharedMusicPlayer()->playEffectSound("strobe_open.wav");
		runAction( ((CCFiniteTimeAction*)(openAction->copy()->autorelease())) );
	}
	else
	{
		Music::sharedMusicPlayer()->playEffectSound("strobe_close.wav");
		runAction( ((CCFiniteTimeAction*)(openAction->copy()->autorelease()))->reverse() );

		Music::sharedMusicPlayer()->playEffectSound("gate_sound.wav");
	}
}

void Strobe::onExit()
{
    // 必须要完成父类的，不然就是bug
    CustomeSprite::onExit();
    
	stopAllActions();
	CC_SAFE_RELEASE(openAction);
	CC_SAFE_RELEASE(leftHeadOpenAction);
	CC_SAFE_RELEASE(rightHeadOpenAction);
}
CCRect Strobe::boundingBox()
{
    CCRect ret = CustomeSprite::boundingBox();
	CCLog("Strobe boundingBox origin = [%f,%f] ,size = [%f,%f]",ret.origin.x,ret.origin.y,ret.size.width,ret.size.height);
    ret.origin.x += 60.0f;
	//ret.origin.x += BasicLength/2;
    //ret.origin.y -= BasicLength/2;
    ret.size.width -= 60.0f*2;
    ret.size.height -= BasicLength/2;

	CCLog("Strobe used boundingBox origin = [%f,%f] ,size = [%f,%f]",ret.origin.x,ret.origin.y,ret.size.width,ret.size.height);
    return ret;
}

bool Strobe::pointInMe(CCPoint glPoint)
{
    if (leftHead->boundingBox().containsPoint(glPoint) ||
       rightHead->boundingBox().containsPoint(glPoint) ||
       this->boundingBox().containsPoint(glPoint)) {
        
        return true;
    }
    
    return false;
}

//销毁闸门的所有body：左，中，右
void Strobe::destoryBodyByDarkBomb(DarkHoleBomb *bomb)
{
    if (leftBody) {
        
        bomb->setToImmediatelyDelete(leftBody);

        leftBody = NULL;
    }
    if (rightBody) {
        bomb->setToImmediatelyDelete(rightBody);

        rightBody = NULL;
    }
    if (myCenterBody!=NULL) {
        bomb->setToImmediatelyDelete(myCenterBody);
        myCenterBody = NULL;
    }
    //BaseContentLayer::shareBaseContentLayer()->removeStrobe(this);

}

void Strobe::destoryHeadByDarkBomb(cocos2d::CCNode *head,DarkHoleBomb *bomb)
{
    if (head == leftHead) {
        if (leftBody) {
            bomb->setToImmediatelyDelete(leftBody);
            leftBody = NULL;
        }
        leftHead = NULL;//此时head都已经交给外部处理了
    }
    if (head == rightHead) {
        if (rightBody) {
            bomb->setToImmediatelyDelete(rightBody);
            rightBody = NULL;
        }
        rightHead = NULL;
    }
    
    //BaseContentLayer::shareBaseContentLayer()->removeStrobe(this);
}

//销毁闸门的所有body：左，中，右
void Strobe::destoryBody()
{
//    if (leftBody) {
//        leftBody->SetUserData(NULL);
//        world->DestroyBody(leftBody);
//        leftBody = NULL;
//    }
//    if (rightBody) {
//        rightBody->SetUserData(NULL);
//        world->DestroyBody(rightBody);
//        rightBody = NULL;
//    }
//    if (myCenterBody!=NULL) {
//        myCenterBody->SetUserData(NULL);
//        world->DestroyBody(myCenterBody);
//        myCenterBody = NULL;
//    }
//    BaseContentLayer::shareBaseContentLayer()->removeStrobe(this);
}
//如果是闸门的头部要销毁，一定用这个方法销毁
void Strobe::destoryHead(cocos2d::CCNode *head)
{
//    if (head == leftHead) {
//        if (leftBody) {
//            leftBody->SetUserData(NULL);
//            world->DestroyBody(leftBody);
//            leftBody = NULL;
//        }
//        leftHead = NULL;//此时head都已经交给外部处理了
//    }
//    if (head == rightHead) {
//        if (rightBody) {
//            rightBody->SetUserData(NULL);
//            world->DestroyBody(rightBody);
//            rightBody = NULL;
//        }
//        rightHead = NULL;
//    }
//    
//    BaseContentLayer::shareBaseContentLayer()->removeStrobe(this);
}
//这里是真正开始移除SPrite时调用的，这里就完成把没有移除的精灵移除掉
void Strobe::removeAllIfNeed()
{
    if (leftHead!=NULL) {
        if (leftBody) {
            leftBody->SetUserData(NULL);
            world->DestroyBody(leftBody);
            leftBody = NULL;
        }
        leftHead->removeFromParentAndCleanup(true);
        leftHead = NULL;
    }
    if (rightHead!=NULL) {
        if (rightBody) {
            rightBody->SetUserData(NULL);
            world->DestroyBody(rightBody);
            rightBody = NULL;
        }
        rightHead->removeFromParentAndCleanup(true);
        rightHead = NULL;
    }
    if (myCenterBody!=NULL) {
        myCenterBody->SetUserData(NULL);
        world->DestroyBody(myCenterBody);
        myCenterBody = NULL;
    }
    setVisible(false);
    //BaseContentLayer::shareBaseContentLayer()->removeStrobe(this);
}



//===========================================
// 电磁炮
//===========================================

//构造一个电磁炮
EMCannonSprite::EMCannonSprite(EMCannon e,b2World* w,CCLayer* layerToAddHead)
{
    world = w;
    leftHead = NULL;
    rightHead = NULL;
    myBody = NULL;
    leftBody = rightBody = NULL;
    isOpen = false;//默认是开启电磁炮的
    
    if (e.width_PTM_RATIO_num <= 4) {
        e.width_PTM_RATIO_num = 6;
    }
    emcannon = e;
        
    angle = e.angle;

    
    initWithFile(resMgr->getSharedFilePath("EMCannon.png").c_str());
    if (e.em_type == EMCannon_type_Single)
    {
        isSingle = true;
        float width = 43*24;
        if (e.isLeft) {
//            float width = 43*24;
//            setTextureRect(CCRectMake(0, 0, width, 24));
            setPosition(ccp ( (e.pos.x+1) * PTM_RATIO + width/2,e.pos.y * PTM_RATIO) );
        }else
        {
//            float width = 43*24;
//            setTextureRect(CCRectMake(0, 0, width, 24));
            setPosition(ccp ( (e.pos.x-1) * PTM_RATIO - width/2,e.pos.y * PTM_RATIO) );
        }
        emcannon.width_PTM_RATIO_num = width/PTM_RATIO;
//        emcannon.width_PTM_RATIO_num = getContentSize().width/PTM_RATIO;
    }
    else
    {
        isSingle = false;
        setTextureRect(CCRectMake(0, 0, (e.width_PTM_RATIO_num-4)*PTM_RATIO, 24));
        setPosition(ccp (e.pos.x * PTM_RATIO,e.pos.y * PTM_RATIO) );
    }
    
    bodyPos.Set(getPositionX()/PTM_RATIO, getPositionY()/PTM_RATIO);
    
    if (e.em_type == EMCannon_type_Single) {
        
        if (e.isLeft) {
            
            leftHead = new CustomeSprite();
            leftHead->initWithFile("EMCannon_left.png");
        }else{
            rightHead = new CustomeSprite();
            rightHead->initWithFile("EMCannon_right.png");
        }
        
    }else if(e.em_type == EMCannon_type_Double){
        leftHead = new CustomeSprite();
        leftHead->initWithFile("EMCannon_left.png");
        rightHead = new CustomeSprite();
        rightHead->initWithFile("EMCannon_right.png");
    }
    
    createBody();

    schedule(schedule_selector(EMCannonSprite::tick));
    
    leftBody = createLeftHead(layerToAddHead);
    rightBody = createRightHead(layerToAddHead);
    
    widthBack = emcannon.width_PTM_RATIO_num;
    isIntercepting = false;
    //如果开始是关闭的，那么就关闭。默认是开启的。
    if (emcannon.closeForBegin) {
        swicth();
    }
}

b2Body* EMCannonSprite::createHead(b2World* world,b2Vec2 pos,CCSprite* userData)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.userData = userData;
    b2Body* bodyA = world->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        box.SetAsBox(1.0f, 1.0f, b2Vec2(0.0f, 0.0f), 0);//左边头
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = Fixture_Density;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        bodyA->CreateFixture(&fixtureDef);
        
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}
void EMCannonSprite::createBody()
{//width_PTM_RATIO_num在单炮的情况下，其实是没有用的，但是还是给他赋了值，用于描诉光柱的长度
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    
    float posX = emcannon.pos.x;
    float posY = emcannon.pos.y;
    if (emcannon.em_type == EMCannon_type_Single && emcannon.isLeft )
    {//左单炮
        posX = posX + ((emcannon.width_PTM_RATIO_num)/2+1) * cos( CC_DEGREES_TO_RADIANS(angle) );
        posY = posY + ((emcannon.width_PTM_RATIO_num)/2+1) * sin( CC_DEGREES_TO_RADIANS(angle) );
    }
    else if (emcannon.em_type == EMCannon_type_Single && !emcannon.isLeft)
    {//右单炮
        
        posX = posX + ((emcannon.width_PTM_RATIO_num)/2+1) * cos( CC_DEGREES_TO_RADIANS(180+angle) );
        posY = posY + ((emcannon.width_PTM_RATIO_num)/2+1) * sin( CC_DEGREES_TO_RADIANS(180+angle) );
        
    }else
    {//双炮
        posX = bodyPos.x;
        posY = bodyPos.y;
    }
    setPosition(ccp(posX*PTM_RATIO,posY*PTM_RATIO));
    if (isSingle) {
        setTextureRect(CCRectMake(0, 0, emcannon.width_PTM_RATIO_num*PTM_RATIO, 24));
    }else{
        setTextureRect(CCRectMake(0, 0, (emcannon.width_PTM_RATIO_num-4)*PTM_RATIO, 24));
    }
    
    
    bodyDef.position.Set(posX, posY);
    bodyDef.userData = this;
    b2Body* bodyA = world->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        box.SetAsBox(((float)emcannon.width_PTM_RATIO_num-2-2)/2.0f, 0.25f,b2Vec2(0.0f, 0.0f), 0);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = Fixture_Density;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        bodyA->CreateFixture(&fixtureDef);
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    myBody = bodyA;
}
void EMCannonSprite::tick(float time)
{
    if (isOpen && isSingle) {
        return;
    }
    vector<b2Fixture*> f;
    f.clear();
    //把table找出来
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData()!=NULL) {
            
            CCNode* node =(CCNode*)b->GetUserData();
            
            if (node->getTag() == Tag_Box_Table||
                node->getTag() == Tag_Box_Table_Ineffective)
            {
                                b2Fixture* fixture = b->GetFixtureList();
                for (; fixture!=NULL; fixture = fixture->GetNext())
                {
                    f.insert(f.end(), fixture);
                }                  
            }
            
        }
    }
    
    BaseContentLayer* layer = BaseContentLayer::shareBaseContentLayer();
    if (isSingle && emcannon.isLeft)
    {//左单炮
        
        b2Vec2 p;
        bool intercept = layer->isAnyFixtureInLine2(emcannon.pos, emcannon.angle, &p, f);
        
        if (intercept)
        {//本来是好的，但是被打断之后就重建
            
            float distance = getDistacne(emcannon.pos, p)-1;
//            if (emcannon.width_PTM_RATIO_num <= distance)
//            {
//                return;
//            }
            isIntercepting = true;
            powerOff(true);
            emcannon.width_PTM_RATIO_num = distance;
            powerOn(true);
        }
        else if (!intercept && isIntercepting)
        {//本来是打断了重建的，但是离开了，那么就恢复
            isIntercepting = false;
            powerOff(true);
            emcannon.width_PTM_RATIO_num = widthBack;
            powerOn(true);
        }
        
    }
    else if(isSingle && !emcannon.isLeft)
    {//右单炮
        
        b2Vec2 p;
        bool intercept = layer->isAnyFixtureInLine2(emcannon.pos, emcannon.angle+180, &p, f);
        if (intercept)
        {//本来是好的，但是被打断之后就重建
            float distance = getDistacne(emcannon.pos, p)-1;
//            if (emcannon.width_PTM_RATIO_num <= distance)
//            {
//                return;
//            }
            isIntercepting = true;
            powerOff();
            emcannon.width_PTM_RATIO_num = distance;
            powerOn();
        }
        else if (!intercept && isIntercepting)
        {//本来是打断了重建的，但是离开了，那么就恢复
            isIntercepting = false;
            powerOff();
            emcannon.width_PTM_RATIO_num = widthBack;
            powerOn();
        }
    }
    else
    {//双炮
        b2Vec2 p;
        bool intercept = layer->isAnyFixtureInLine2(leftBody->GetWorldCenter(),
                                                    rightBody->GetWorldCenter(), &p, f);
        
        if (intercept && !isIntercepting)
        {//本来是好的，但是被打断之后就重建
            isIntercepting = true;
            powerOff();
            schedule(schedule_selector(EMCannonSprite::tick));
        }
        else if (!intercept && isIntercepting)
        {//本来是打断了重建的，但是离开了，那么就恢复
            isIntercepting = false;
            powerOn();
        }
    }
    
}

float EMCannonSprite::getDistacne(b2Vec2 p1, b2Vec2 p2)
{
    return sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );
}

bool EMCannonSprite::isBodyShelterMe(b2Body *body)
{
    if (body!=NULL && body->GetUserData()!=NULL) {
        
        
    }
    return false;
}

void EMCannonSprite::powerOn(bool animation /*= false*/)
{
    if (isOpen == false || (leftHead == NULL && rightHead ==NULL) ) {
        return;//已经是powerOn了
    }
    createBody();
    if (animation) {
        CCFadeTo* fade = CCFadeTo::create(0.2f, 255);
        runAction(fade);
    }else{
        setVisible(true);
    }
    isOpen = false;
    schedule(schedule_selector(EMCannonSprite::tick));
}
void EMCannonSprite::powerOff(bool animation /*= false*/)
{
    if (isOpen || (leftHead == NULL && rightHead ==NULL) ) {
        return;//已经是powerOff了
    }
    if (animation) {
        CCFadeTo* fade = CCFadeTo::create(0.2f, 0);
        runAction(fade);
    }else{
        setVisible(false);
    }
    
    unschedule(schedule_selector(EMCannonSprite::tick));
    myBody->SetUserData(NULL);
    world->DestroyBody(myBody);
    myBody = NULL;
    
    isOpen = true;
}

void EMCannonSprite::swicth()
{
    if (leftHead == NULL && rightHead ==NULL) {
        return;
    }
    if (isOpen) {//打开电磁炮
        powerOn();
    }else{//关闭电磁炮
        powerOff();
    }
}

//否可以攻击到这个区域。可以就返回true
bool EMCannonSprite::canAttact(CCRect rect)
{
    if (isOpen) {
        return false;
    }
    return boundingBox().intersectsRect( rect);
}

bool EMCannonSprite::pointInMe(CCPoint glPoint)
{
    if (
        (leftHead!=NULL && leftHead->boundingBox().containsPoint( glPoint)) ||
        (rightHead!=NULL && rightHead->boundingBox().containsPoint( glPoint)) )
    {
        return true;
    }
    return false;
}

CCRect EMCannonSprite::boundingBox()
{
    CCRect r = CCSprite::boundingBox();
    
    r.size.height = 24;
    
    return r;
}

b2Body* EMCannonSprite::createLeftHead(CCLayer* headToAdd)
{
    if (emcannon.em_type == EMCannon_type_Single && !emcannon.isLeft) {
        return NULL;
    }
    headToAdd->addChild(leftHead,0,Tag_Box_EMCannoon_Head);
    leftHead->setData(this);
    b2Vec2 pos;
    if (emcannon.em_type == EMCannon_type_Single)
    {
        pos.Set(emcannon.pos.x,emcannon.pos.y);
    }else
    {
        float posX = ( (emcannon.width_PTM_RATIO_num-2-2)/2.0f + 1.0f )*cos(CC_DEGREES_TO_RADIANS(180+angle));
        float posY = ( (emcannon.width_PTM_RATIO_num-2-2)/2.0f + 1.0f )*sin(CC_DEGREES_TO_RADIANS(180+angle));
        
        pos.Set(emcannon.pos.x+posX,emcannon.pos.y+posY);
//        pos.Set(emcannon.pos.x - emcannon.width_PTM_RATIO_num/2.0f + 1.0f,emcannon.pos.y);
    }
    
    return createHead(world, pos, leftHead);
}
b2Body* EMCannonSprite::createRightHead(CCLayer* headToAdd)
{
    if (emcannon.em_type == EMCannon_type_Single && emcannon.isLeft) {
        
        return NULL;
    }
    headToAdd->addChild(rightHead,0,Tag_Box_EMCannoon_Head);
    rightHead->setData(this);
    b2Vec2 pos;
    if (emcannon.em_type == EMCannon_type_Single)
    {
        pos.Set(emcannon.pos.x,emcannon.pos.y);
    }else
    {
        float posX = ( (emcannon.width_PTM_RATIO_num-2-2)/2.0f + 1.0f )*cos(CC_DEGREES_TO_RADIANS(angle));
        float posY = ( (emcannon.width_PTM_RATIO_num-2-2)/2.0f + 1.0f )*sin(CC_DEGREES_TO_RADIANS(angle));
        pos.Set(emcannon.pos.x + posX,emcannon.pos.y + posY);
    }
    
    return createHead(world, pos, rightHead);
}

void EMCannonSprite::destoryBodyByDarkBomb(DarkHoleBomb *bomb)
{
    isOpen = true;
    unschedule(schedule_selector(EMCannonSprite::tick));
    if (leftBody) {
        bomb->setToImmediatelyDelete(leftBody);
        leftBody = NULL;
    }
    if (rightBody) {
        bomb->setToImmediatelyDelete(rightBody);
        rightBody = NULL;
    }
    if (myBody!=NULL) {
        bomb->setToImmediatelyDelete(myBody);
        myBody = NULL;
    }
    BaseContentLayer::shareBaseContentLayer()->removeEmCannon(this);
}

void EMCannonSprite::destoryHeadByDarkBomb(cocos2d::CCNode *head, DarkHoleBomb *bomb)
{
    isOpen = true;
    unschedule(schedule_selector(EMCannonSprite::tick));
    if (head == leftHead) {
        if (leftBody) {
            bomb->setToImmediatelyDelete(leftBody);
            leftBody = NULL;
        }
        leftHead = NULL;//此时head都已经交给外部处理了
    }
    if (head == rightHead) {
        if (rightBody) {
            bomb->setToImmediatelyDelete(rightBody);
            rightBody = NULL;
        }
        rightHead = NULL;
    }
    
    BaseContentLayer::shareBaseContentLayer()->removeEmCannon(this);
}

//销毁闸门的所有body：左，中，右
void EMCannonSprite::destoryBody()
{
//    if (leftBody) {
//        leftBody->SetUserData(NULL);
//        world->DestroyBody(leftBody);
//        leftBody = NULL;
//    }
//    if (rightBody) {
//        rightBody->SetUserData(NULL);
//        world->DestroyBody(rightBody);
//        rightBody = NULL;
//    }
//    if (myBody!=NULL) {
//        myBody->SetUserData(NULL);
//        world->DestroyBody(myBody);
//        myBody = NULL;
//    }
//    BaseContentLayer::shareBaseContentLayer()->removeEmCannon(this);
}
//如果是闸门的头部要销毁，一定用这个方法销毁
void EMCannonSprite::destoryHead(cocos2d::CCNode *head)
{
//    if (head == leftHead) {
//        if (leftBody) {
//            leftBody->SetUserData(NULL);
//            world->DestroyBody(leftBody);
//            leftBody = NULL;
//        }
//        leftHead = NULL;//此时head都已经交给外部处理了
//    }
//    if (head == rightHead) {
//        if (rightBody) {
//            rightBody->SetUserData(NULL);
//            world->DestroyBody(rightBody);
//            rightBody = NULL;
//        }
//        rightHead = NULL;
//    }
//    
//    BaseContentLayer::shareBaseContentLayer()->removeEmCannon(this);
}
//这里是真正开始移除SPrite时调用的，这里就完成把没有移除的精灵移除掉
void EMCannonSprite::removeAllIfNeed()
{
    if (leftHead!=NULL) {
        if (leftBody) {
            leftBody->SetUserData(NULL);
            world->DestroyBody(leftBody);
            leftBody = NULL;
        }
        leftHead->removeFromParentAndCleanup(true);
        leftHead = NULL;
    }
    if (rightHead!=NULL) {
        if (rightBody) {
            rightBody->SetUserData(NULL);
            world->DestroyBody(rightBody);
            rightBody = NULL;
        }
        rightHead->removeFromParentAndCleanup(true);
        rightHead = NULL;
    }
    if (myBody!=NULL) {
        myBody->SetUserData(NULL);
        world->DestroyBody(myBody);
        myBody = NULL;
    }
    setVisible(false);
    BaseContentLayer::shareBaseContentLayer()->removeEmCannon(this);
}


EMCannonSprite::~EMCannonSprite()
{
    CC_SAFE_RELEASE(leftHead);
    CC_SAFE_RELEASE(rightHead);
    
}




//=======================================
// 锅炉
//=======================================

int Boiler::lite_level = Boiler::Lite_no;

Boiler::Boiler()
{
    resMgr = ResManager::getManager();
    m_emitter1 = NULL;
    m_emitter2 = NULL;
    m_emitter3 = NULL;
    m_emitter4 = NULL;
    m_emitter5 = NULL;
    //m_emitter6 = NULL;
	doorPar = NULL;
	autoFirePar[0] = autoFirePar[1] = autoFirePar[2]  = autoFirePar[3] = autoFirePar[4] = NULL;
    body = NULL;
	mBoilerFan = NULL;
    boiler_angle = 0.0f;
	firing = false;
	mB_openDoor = false;
	lastUsedMe = 0;
	emitTime = 0;
	mAutoFire = false;
	mUpDoor = NULL;
	mDownDoor = NULL;
	mCover = NULL;
	mCurSmokeIdToUse = 0;
	mCurSmokeMouseID = 0;
	initWithState(firing);
}

void Boiler::start()
{
	unschedule(schedule_selector(Boiler::tickForWaiting));
	schedule(schedule_selector(Boiler::tickForWaiting));
}

Boiler::~Boiler()
{
	mSmokeBoies.clear();
}

void Boiler::emitSingleBody()
{
	b2World* world = this->body->GetWorld();

	


	CCSize boilerSize = this->getContentSize();
	int size = mEmitterPos.size();
	mCurSmokeMouseID++;
	
	int mouseId = mCurSmokeMouseID % size;
	CCPoint pos = mEmitterPos.at(mouseId);
	float angle = mEmitterAngle.at(mouseId);

	
	float destAngle = angle + CCRANDOM_MINUS1_1() * 11.0f;

	b2Body* body = createSmokeBody(world, destAngle);

#if 0
	int addIdx = (mCurSmokeMouseID / size) % 5;
	float addLen = 3.0f * (float)addIdx;
	pos = pos + ccp(addLen * cos(destAngle), addLen * sin(destAngle));
#endif
	//CCLog("%d, %d, %f\n", mCurSmokeMouseID, mouseId, addLen);

	//CCLOG("angle: %f\n", destAngle);
	float addAngle = this->boiler_angle;
	if(addAngle == 90)
	{
		addAngle -= 0.45f;
	}
	else if(addAngle == 180)
	{
		//TODO
	}
	else if(addAngle == 270 || addAngle == -90)
	{
		addAngle += 0.45f;
	}
	float rad = CC_DEGREES_TO_RADIANS(destAngle + addAngle);
	pos = pos.rotateByAngle(ccp(boilerSize.width/2, boilerSize.height/2), addAngle);
	CCPoint destPos = getPosition() + pos;
	body->SetTransform(b2Vec2((destPos.x - boilerSize.width/2) / PTM_RATIO, (destPos.y - boilerSize.height/2) / PTM_RATIO), rad);
	//body->SetLinearDamping(0.0f);
	float impulse = 0.00001 + CCRANDOM_0_1() * 0.000002;
	body->ApplyLinearImpulse(b2Vec2(impulse * cos(rad), impulse * sin(rad)), b2Vec2(0,0));

#if 0
	b2Body* body = mSmokeBoies.at(mCurSmokeIdToUse);
	b2Vec2 pos(0,0);
	body->SetTransform(pos, 0);

	b2Vec2 force(20, 20);
	b2Vec2 point(10, 10);
	body->ApplyLinearImpulse(force, body->GetWorldCenter());

	mCurSmokeIdToUse = mCurSmokeIdToUse + 1;
	if(mCurSmokeIdToUse >= mSmokeBoies.size())
	{
		mCurSmokeIdToUse = 0;
	}
#endif
}
b2Body* Boiler::createSmokeBody(b2World* world, float angle)
{
	BoilerBubble* sprite = new BoilerBubble;

	sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"bubble.png").c_str());
	sprite->setScale(0);
	sprite->mAngle = angle;
	getParent()->addChild(sprite);
	sprite->release();
	sprite->setTag(Tag_Boiler_Bubble);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0, 0);
	bodyDef.userData = sprite;
	b2Body* bodyA = world->CreateBody(&bodyDef);
	bodyA->SetBullet(true);
	bodyA->SetGravityScale(-1.0f);
	sprite->body = bodyA;
	//b2MassData mass;
	//mass.mass = -1.0f;
	//bodyA->SetMassData(&mass);
	b2CircleShape circle;
	circle.m_radius = 0.005f;

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;	
	fixtureDef.density = 0.01f;
	fixtureDef.friction = 0.0f; //Fixture_Friction;
	fixtureDef.restitution = 0.2f;
	bodyA->CreateFixture(&fixtureDef);

	return bodyA;
	
}
void Boiler::createSmokeBodies(b2World* world)
{
	for(int i = 0; i < 20; i++)
	{
		//b2Body* body = createSmokeBody(world);
		//mSmokeBoies.push_back(body);
	}
}

void Boiler::initWithState(bool fireing)
{
	init();
//	removeAllChildrenWithCleanup(true);

	if (fireing)
	{
		if (mUpDoor)
		{
			mUpDoor->setVisible(false);
		}

		if (mDownDoor)
		{
			mDownDoor->setVisible(false);
		}

		if (mCover)
		{
			mCover->setVisible(false);
		}
		initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"boiler_2.png").c_str());
	}else
	{
		initWithFile(resMgr->getSharedFilePath(g_gameinPath+"boiler_1_bg.png").c_str());
		if (!mUpDoor)
		{
//			mUpDoor = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath + "boiler_1_door_up.png").c_str());
			mUpDoor = CCSprite::createWithSpriteFrameName("boiler_1_door_up.png");
			addChild(mUpDoor);
		}
		mUpDoor->setVisible(true);
		mUpDoor->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));

		if (!mDownDoor)
		{
//			mDownDoor = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath + "boiler_1_door_down.png").c_str());
			mDownDoor = CCSprite::createWithSpriteFrameName("boiler_1_door_down.png");
			addChild(mDownDoor);
		}
		mDownDoor->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
		mDownDoor->setVisible(true);
		
		if (!mCover)
		{
//			mCover = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath + "boiler_1_cover.png").c_str());
			mCover = CCSprite::createWithSpriteFrameName("boiler_1_cover.png");
			addChild(mCover);
		}
		mCover->setVisible(true);
		mCover->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
	}
	
}
void Boiler::setFan(CCSprite* fan)
{
	mBoilerFan = fan;
	addChild(mBoilerFan);
	mBoilerFan->setPosition(ccp(24,18));
	mBoilerFan->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f,360.0f)));

}
void Boiler::setAngle(float angle)
{
    this->boiler_angle = angle;
}
void Boiler::setLite(int lite_level)
{
	Boiler::lite_level = lite_level;
}
//停止当前的粒子系统发射
void Boiler::stop()
{
	Music::sharedMusicPlayer()->stopEffectSound(mSoundId);
	if (!firing)
		return;

	firing = false;
	emitTime = 0;
	/*initWithState(firing);*/
#if USE_PARTICLE_BOILER
    removeParticle(m_emitter1);
    removeParticle(m_emitter2);
    removeParticle(m_emitter3);
    removeParticle(m_emitter4);
    removeParticle(m_emitter5);
    //removeParticle(m_emitter6);
#endif
	initWithState(firing);

	if(mBoilerFan)
	{
		mBoilerFan->stopAllActions();
		mBoilerFan->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f,360.0f)));
	}


	lastUsedMe = 0;
}
//按下的时候调用，代表发射了。
void Boiler::fire()
{
	if (firing)
		return;
	stopAutoFire();
	closeDoorToStopFire();
	firing = true;
	emitTime = 0;
	initWithState(firing);
#if USE_PARTICLE_BOILER
    m_emitter1 = createParticle(ccp(42,20),0.0f,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
    m_emitter2 = createParticle(ccp(36,36),38.0f,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
    m_emitter3 = createParticle(ccp(23,40),90.0f,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
    m_emitter4 = createParticle(ccp(7,37),127.0f,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
    m_emitter5 = createParticle(ccp(3,21),180.0f,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	//m_emitter6 = createParticle(ccp(3,21),180.0f,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	addChild(m_emitter1);
	addChild(m_emitter2);
	addChild(m_emitter3);
	addChild(m_emitter4);
	addChild(m_emitter5);
	//addChild(m_emitter6);
#else
	mEmitterPos.clear();
	mEmitterAngle.clear();
	addEmitterPos(ccp(46,20),0.0f);
	addEmitterPos(ccp(41,40),38.0f);
	addEmitterPos(ccp(23,44),90.0f);
	addEmitterPos(ccp(4,40),127.0f);
	addEmitterPos(ccp(0,21),180.0f);
#endif
    
    
    mSoundId = Music::sharedMusicPlayer()->playEffectSound("boiler_sound.wav", true);

	if (mBoilerFan)
	{
		mBoilerFan->stopAllActions();
		mBoilerFan->runAction(CCRepeatForever::create(CCRotateBy::create(0.2f,360.0f)));
	}

	lastUsedMe = 0;
}
void Boiler::openDoorToFire()
{
	if (mB_openDoor)
		return;
	mB_openDoor = true;
	mUpDoor->stopAllActions();
	mDownDoor->stopAllActions();

	mUpDoor->setPositionY(getContentSize().height/2 + 10);
	mDownDoor->setPositionY(getContentSize().height/2 - 10);
//	if (getMachinePerformanceLevel() > 0)
	{
		doorPar = createDoorFireParticle(ccp(24,22),resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
		addChild(doorPar);

		schedule(schedule_selector(Boiler::stopOpenFire),0.5f );
		if (BaseContentLayer::shareBaseContentLayer()->getCurPlayMode() == playMode_Normal)
		{
			Music::sharedMusicPlayer()->playEffectSound("boiler_openDoor.wav");
		}
	}
	
	

}
void Boiler::stopOpenFire(float t)
{
	unschedule(schedule_selector(Boiler::stopOpenFire));
	closeDoorToStopFire();
}
void Boiler::closeDoorToStopFire()
{
	if (!mB_openDoor)
		return;
	mB_openDoor = false;

	lastUsedMe = 0;

	mUpDoor->runAction(CCMoveBy::create(0.4f,ccp(0,-10)));
	mDownDoor->runAction(CCMoveBy::create(0.4f,ccp(0,10)));
	if (doorPar)
		doorPar->setDuration(0.02f);

}
void Boiler::autoFire()
{
	if (mAutoFire)
		return;

	for (int i=0;i<5;i++)
	{
		if (autoFirePar[i])
		{
			if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
			{
				autoFirePar[i]->removeAllChildrenWithCleanup(true);
				CC_SAFE_RELEASE(autoFirePar[i]);
			}
		}
	}

//	if (getMachinePerformanceLevel() > 0)
	{

		autoFirePar[0] = createAutoFireParticle(ccp(45,21),0.0f,0.5f,40,4,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str(),lite_level);
		autoFirePar[1] = createAutoFireParticle(ccp(39,39),38.0f,1.0f,60,4,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str(),lite_level);
		autoFirePar[2] = createAutoFireParticle(ccp(23,44),90.0f,1.2f,80,6,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str(),lite_level);
		autoFirePar[3] = createAutoFireParticle(ccp(5,39),127.0f,1.2f,80,6,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str(),lite_level);
		autoFirePar[4] = createAutoFireParticle(ccp(0,21),180.0f,1.0f,60,4,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str(),lite_level);
		//autoFirePar[5] = createAutoFireParticle(ccp(4,17),180.0f,0.5f,40,4,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str(),lite_level);
		for (int i=0;i<5;i++)
		{
			if (autoFirePar[i])
			{
				addChild(autoFirePar[i]);
			}
		}

		mAutoFire = true;
	}
	
	

}
void Boiler::stopAutoFire()
{
	if (!mAutoFire)
		return;

	for (int i=0;i<5;i++)
	{
		if (autoFirePar[i])
			autoFirePar[i]->setDuration(0.01f);
	}
	mAutoFire= false;
}

void Boiler::tickForWaiting(float t)
{
	if (BaseContentLayer::shareBaseContentLayer()->getSuccessful())
	{
		unschedule(schedule_selector(Boiler::tickForWaiting));
		return;
	}
	lastUsedMe += t;
	
	// 闲置了1秒，才自动fire
	if (!mAutoFire && !firing && lastUsedMe >= 0.4f)
	{
//		if (getMachinePerformanceLevel() > 0)
		{
			autoFire();
		}
	}

	if (!firing && !mB_openDoor && lastUsedMe >= 5.0f)
	{
//		if (getMachinePerformanceLevel() > 0)
		{
			openDoorToFire();
		}
		lastUsedMe = 0;
	}

#if !USE_PARTICLE_BOILER
	if(firing)
	{
		emitTime += t;
		//if(emitTime > 1.0f / 20.0f)
		{
			for(int i = 0; i < 5; i++)
			{
				emitSingleBody();
			}
			emitTime = 0.0f;
		}
	}
	else
	{
		emitTime = 0.0f;
	}
#endif

}
CCParticleSystem* Boiler::createAutoFireParticle(CCPoint relativePos,float angle,float life,int num,float speed,const char* image,int lite_level)
{
	ccColor4F endColor;
	endColor.a = 0;
	endColor.b = 0.1f;
	endColor.r = 0.1f;
	endColor.g = 0.1f;
	ccColor4F startColor;
	startColor.a = lite_level!=Lite_no ? 0.5f : 0.3f;
	startColor.b = 0.3f;
	startColor.r = 0.3f;
	startColor.g = 0.3f;


	int particleNum = num;
	float startSize = 5.5f;
	float startSizeVar =  0.5f;
	float endSize = 10.0f;
	float endSizeVar = 1.2f;

	if (lite_level == Lite_1)
	{
		particleNum = num/5;
		startSize = 11.0f;
		startSizeVar = 1.0f;
		endSize =  20;
		endSizeVar = 2.4f;
		life *= 1.2f;
	}
	else if (lite_level == Lite_2)
	{
		particleNum = num/10;

		startSize = 18.0f;
		startSizeVar = 2;
		endSize =  20;
		endSizeVar = 5.0;
		life = 2.0f;
	}

	CCParticleSystem* particle = CCParticleSun::create();

	particle->setAutoRemoveOnFinish(true);
	particle->setEmitterMode(kCCParticleModeGravity);
	particle->setPositionType(kCCPositionTypeFree);
	particle->setTotalParticles(particleNum);
	particle->setStartSize(startSize);
	particle->setStartSizeVar(startSizeVar);
	particle->setEndSize(endSize);
	particle->setEndSizeVar(endSizeVar);
	particle->setDuration(-1);//无限的时间长度
	particle->setLife(life);
	particle->setLifeVar(life/3.0f);
	particle->setAngle(angle);
	particle->setAngleVar(0.5f);
	particle->setSpeed(speed);
	particle->setSpeedVar(speed/2.5f);
	particle->setRadialAccel(2);
	particle->setEndColor(endColor);
	float GRAVITY = 10.0f;
	switch((int)boiler_angle)
	{
	case 0:
		particle->setGravity(ccp(0, GRAVITY));
		break;
	case 180:
		particle->setGravity(ccp(0, -GRAVITY));
		break;
	case 90:
		particle->setGravity(ccp(GRAVITY, 0));
		break;
	case 270:
	case -90:
		particle->setGravity(ccp(-GRAVITY, 0));
		break;
	}
	particle->setPosVar(ccp(2,2));
	particle->setStartColor(startColor);
	particle->setTexture( CCTextureCache::sharedTextureCache()->addImage(image) );
	particle->setPosition(relativePos);

	//防止崩溃
	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
		particle->retain();

	return particle;
}
CCParticleSystem* Boiler::createParticle(CCPoint relativePos,float angle,const char* image)
{
	ccColor4F endColor;
	endColor.a = 0;
	endColor.b = 1.0f;
	endColor.r = 1.0f;
	endColor.g = 1.0f;
	ccColor4F startColor;
	startColor.a = 0.6f;
	startColor.b = 0.6f;
	startColor.r = 0.6f;
	startColor.g = 0.6f;

	CCParticleSystem* particle = CCParticleGalaxy::create();

	particle->setAutoRemoveOnFinish(true);
	particle->setEmitterMode(kCCParticleModeGravity);
	particle->setPositionType(kCCPositionTypeFree);


	int particleNum = 800;
	float startSize = 10.0f;
	float startSizeVar = 0.2f;
	float endSize =  50.0f;
	float endSizeVar =  2.2f;
	float life = 0.1f;
	float angleVar = 5.0f;

	if (lite_level == Lite_1)
	{
		particleNum = 40;
		startSize *= 1.2f;
		startSizeVar *= 2;
		endSize  *=  1.2f;
		endSizeVar *= 1.0f;
		life *= 1.2f;
		startColor.a = 0.4f;
		angleVar = 2.0f;
	}
	else if (lite_level == Lite_2)
	{
		particleNum = 10;
		startSize *= 1.2f;
		startSizeVar *= 2;
		endSize  *=  1.2f;
		endSizeVar *= 1.0f;
		life *= 1.2f;
		startColor.a = 0.4f;
		angleVar = 2.0f;
	}
// 	if (getMachinePerformanceLevel() <= 0)
// 	{
// 		particleNum /= 2;
// 	}
	

	particle->setTotalParticles(particleNum);
	particle->setStartSize(startSize);
	particle->setStartSizeVar(startSizeVar);
	particle->setEndSize(endSize);
	particle->setEndSizeVar(endSizeVar);
	particle->setDuration(-1);//无限的时间长度
	particle->setLife(0.1f);
	particle->setLifeVar(0.02f);
	particle->setAngle(angle);
	particle->setAngleVar(angleVar);
	particle->setSpeed(500.0f);
	particle->setSpeedVar(20.0f);
	particle->setEndColor(endColor);

	float GRAVITY = 2500.0f;
	switch((int)boiler_angle)
	{
	case 0:
		particle->setGravity(ccp(0, GRAVITY));
		break;
	case 180:
		particle->setGravity(ccp(0, -GRAVITY));
		break;
	case 90:
		particle->setGravity(ccp(GRAVITY, 0));
		break;
	case 270:
	case -90:
		particle->setGravity(ccp(-GRAVITY, 0));
		break;
	}
	
	particle->setPosVar(CCPointZero);
	particle->setStartColor(startColor);
	particle->setTexture( CCTextureCache::sharedTextureCache()->addImage(image) );
	particle->setPosition(relativePos);

	//防止崩溃
	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
		particle->retain();

	return particle;
}

void Boiler::addEmitterPos(CCPoint relativePos, float angle)
{
	mEmitterPos.push_back(relativePos);
	mEmitterAngle.push_back(angle);
}

CCParticleSystem* Boiler::createDoorFireParticle(CCPoint relativePos,const char* image)
{
	ccColor4F endColor;
	endColor.a = 0;
	endColor.b = 0.2f;
	endColor.r = 0.2f;
	endColor.g = 0.2f;
	ccColor4F startColor;
	startColor.a = lite_level!=Lite_no ? 0.5f : 0.3f;
	startColor.b = 0.3f;
	startColor.r = 0.3f;
	startColor.g = 0.3f;

	CCParticleSystem* particle = CCParticleSun::create();
	
	int particleNum = 300;
	float startSize = 10.5f;
	float startSizeVar = 5.5f;
	float endSize =  10.0f;
	float endSizeVar =  1.2f;
	float life = 1.5f;

	if (lite_level == Lite_1)
	{
		particleNum = 30;
		startSize = 21.0f;
		startSizeVar = 11;
		endSize =  20;
		endSizeVar = 2.4f;
		life = 3.0f;
	}
	else if (lite_level == Lite_2)
	{
		particleNum = 15;

		startSize = 20.0f;
		startSizeVar = 15;
		endSize =  20;
		endSizeVar = 4.4f;
		life = 4.0f;
	}
	particle->setAutoRemoveOnFinish(true);
	particle->setEmitterMode(kCCParticleModeGravity);
	particle->setPositionType(kCCPositionTypeFree);
	particle->setTotalParticles(particleNum);
	particle->setStartSize(startSize);
	particle->setStartSizeVar(startSizeVar);
	particle->setEndSize(endSize);
	particle->setEndSizeVar(endSizeVar);
	particle->setDuration(-1);//无限的时间长度
	particle->setLife(life);
	particle->setLifeVar(life/1.5f);
	particle->setAngle(90);
	particle->setAngleVar(15.0f);
	particle->setSpeed(20);
	particle->setSpeedVar(10);
	particle->setRadialAccel(-15);
	particle->setEndColor(endColor);
	float GRAVITY = 10.0f;
	switch((int)boiler_angle)
	{
	case 0:
		particle->setGravity(ccp(0, GRAVITY));
		break;
	case 180:
		particle->setGravity(ccp(0, -GRAVITY));
		break;
	case 90:
		particle->setGravity(ccp(GRAVITY, 0));
		break;
	case 270:
	case -90:
		particle->setGravity(ccp(-GRAVITY, 0));
		break;
	}
	particle->setPosVar(ccp(9,9));
	particle->setStartColor(startColor);
	particle->setTexture( CCTextureCache::sharedTextureCache()->addImage(image) );
	particle->setPosition(relativePos);

	particle->retain();
	particle->autorelease();

	////防止崩溃
	//if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	//	particle->retain();

	return particle;
}
//抬起之后调用，代表停止
void Boiler::removeParticle(CCParticleSystem* par)
{
	if (par !=NULL)
	{
		par->setDuration(0.1f);
		par = NULL;
	}

}
void Boiler::onExit()
{
	CustomeSprite::onExit();
	stop();
}
bool Boiler::piontInMe(cocos2d::CCPoint glPoint)
{
    CCRect bound = boundingBox();
    if (boiler_angle == 0) {
        bound.size.height += 20;
        bound.origin.y -= 20;
    }else if(boiler_angle == 90)
    {
        bound.origin.x += 20;
        bound.size.width += 20;
        
    }else if(boiler_angle == 180)
    {
        bound.origin.y += 20;
        bound.size.height += 20;
        
    }else if(boiler_angle == 270 || boiler_angle == -90)
    {
        bound.origin.x -= 20;
        bound.size.width += 20;
    }
    
    if (bound.containsPoint( glPoint))
    {
        return true;
    }
    return false;
}





//=======================================
// 时间减速效果
//=======================================
TimerDec* TimerDec::createTimerDec(const char* fileName /*= NULL*/)
{
    TimerDec* t = new TimerDec(fileName);
    
    t->autorelease();
    
    return t;
}

//测试obj是否在我的减速范围之类
bool TimerDec::isInTimeDecArea(CCNode* obj)
{
    //没有减速，就直接返回false
    if (!isDecing) {
        return false;
    }
    
    if (getDistanceFromMe(obj) <= radius) {
        
        return true;
    }
    
    return false;
}

//开始减速
void TimerDec::action()
{
	soudId = Music::sharedMusicPlayer()->playEffectSound("timer_bomb_dur.wav");
	setScale(0.5);
    isDecing = true;

	if (m_bSmall)
	{
		par = createFogParticle(this,300,2.4f,16,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	}
	else
	{
		par = createFogParticle(this,800,4.3f,30,resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	}
	par->setTag(Tag_Box_Frost_Par);
}
// void TimerDec::onExit()
// {
//     // 必须要移除，不然有bug
//     CustomeSprite::onExit();
//     
// 	Music::sharedMusicPlayer()->stopEffectSound(soudId);
// }
TimerDec::TimerDec(const char* fileName/* = NULL*/)
{
	ResManager* resMgr = ResManager::getManager();
	par = NULL;
	m_bSmall = false;
    if (fileName == NULL) {
        initWithFile(resMgr->getSharedFilePath(g_gameinPath+"Frost_ward.png").c_str());
    }else{
        initWithFile(fileName);
		m_bSmall = true;
    }
    isDecing = false;
    radius = getContentSize().width/2;
	this->setTag(Tag_Box_Frost);
}
TimerDec::~TimerDec()
{
	Music::sharedMusicPlayer()->stopEffectSound(soudId);
}

//创建一个粒子系统，并加入到item中，也返回这个粒子系统的指针
CCParticleSystem* TimerDec::createFogParticle(CCSprite* item,int partNum,float life,float speed,const char* fileName)
{
	CCParticleGalaxy* par = new CCParticleGalaxy();
    par->autorelease();

	//减少低端机的粒子效果
	int particalNum = partNum;
// 	if (getMachinePerformanceLevel() <= 0)
// 	{
// 		particalNum = particalNum/2;
// 	}

    par->initWithTotalParticles(particalNum);
    par->setTexture(CCTextureCache::sharedTextureCache()->addImage(fileName));
	par->setAutoRemoveOnFinish(true);
    par->setEmitterMode(kCCParticleModeGravity);
    par->setPositionType(kCCPositionTypeFree);
    par->setGravity(CCPointZero);
    ccColor4F startColor;
    startColor.a = 0.005f;
    startColor.r = 211.0f/255.0f / 2.0f;//0.414
    startColor.g = 252.0f/255.0f / 2.0f;//0.494
    startColor.b = 253.0f/255.0f / 2.0f;//0.496
    
    ccColor4F endColor;
    endColor.a = 0.02f;
    endColor.r = 211.0f/255.0f / 2.0f;//0.414
    endColor.g = 252.0f/255.0f / 2.0f;//0.494
    endColor.b = 253.0f/255.0f / 2.0f;//0.496
    
    par->setStartColor(startColor);
    par->setEndColor(endColor);
    par->setStartSize(20.0f);
    par->setStartSizeVar(10.0f);
    par->setEndSize(80.0f);
    par->setEndSizeVar(20.0f);
    par->setTotalParticles(particalNum);
    
    par->setAngle(90.0f);
    par->setAngleVar(180.0f);
    
    par->setLife(life);
    par->setLifeVar(0.5f);
    par->setSpeed(speed);
    par->setSpeedVar(5.0f);
    par->setTangentialAccel(0.0f);
    par->setTangentialAccelVar(0.0f);
    par->setRadialAccel(10.0f);
    par->setRadialAccelVar(0.0f);
    par->setDuration(-1);
    
    par->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
    par->setPosVar(CCPointZero);
    item->addChild(par);
	//防止崩溃
	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
		par->retain();
    
    return par;
}

//needSqrt 如果为flase，那么就返回平方值，而不是根号之后的值
float TimerDec::getDistanceFromMe(CCNode* node,bool needSqrt/* = true */)
{
    if (getParent() != node->getParent()) {
        CCAssert(false, "当前只能处理处于一个坐标系下面的计算");
        return 9999999.9f;
    }
    
    CCPoint nodePos = node->getPosition();
    CCPoint myPos = getPosition();
    
    float dis = pow((nodePos.x - myPos.x), 2) +  pow((nodePos.y - myPos.y), 2);
    
    if (needSqrt) {
        dis = sqrt(dis);
    }
    
    return dis;
    
}



TableBack::TableBack(CustomeSprite* table,bool needFoot/* = true*/)
{
	mTable = table;
	CCAssert(table->getFileName()!="","创建桌面的脚前，需要把桌面都创建好");
	string tableFootFile = table->getFileName();

	if (tableFootFile.find("_no",0) != string::npos)
	{// 无效台面

		if (needFoot)
		{
			SpriteHelper::replace_all_distinct(tableFootFile,"no.png","no_corner.png");
			initWithFile(tableFootFile.c_str());
		}else
		{
			init();
		}

		string tableBackFile = table->getFileName();
		SpriteHelper::replace_all_distinct(tableBackFile,".png","_behind.png");
		CustomeSprite* foot = CustomeSprite::create(tableBackFile.c_str());
		addChild(foot);
		foot->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
	}else
	{
		SpriteHelper::replace_all_distinct(tableFootFile,".png","_behind.png");
		initWithFile(tableFootFile.c_str());
	}
	schedule(schedule_selector(TableBack::follow));
}
void TableBack::follow(float time)
{
	setPosition(mTable->getPosition());
	setRotation(mTable->getRotation());
}

void TableBack::createTableBg(CustomeSprite* table,bool needFoot/* = true*/)
{
	TableBack* bg = new TableBack(table,needFoot);
	bg->setPosition(ccp(table->getPositionX(),table->getPositionY()));

	table->getParent()->addChild(bg);

	//BackgroudLayerFactory::getCurrentBgLayer()->addChild(bg,2);
	bg->release();
}


//=========================卡子==========================

TableBlock::TableBlock(CustomeSprite* table,CCPoint pos1,CCPoint pos2, CCPoint starPos,CCPoint end)
{
	init();
	mTable = table;
	CCPoint offset = ccp(0,3);
	relativePos1 = ccpAdd(pos1,offset);
	relativePos2 = ccpAdd(pos2,offset);

	leftBlock = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_part_board.png").c_str());
	rightBlock = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_part_board.png").c_str());

	leftLight1 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_part_light1.png").c_str());
	leftLight2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_part_light2.png").c_str());

	rightLight1 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_part_light1.png").c_str());
	rightLight2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_part_light2.png").c_str());

	leftBlock->addChild(leftLight1);leftLight1->setPosition(ccp(leftBlock->getContentSize().width/2,leftBlock->getContentSize().height/2));
	leftBlock->addChild(leftLight2);leftLight2->setPosition(ccp(leftBlock->getContentSize().width/2,leftBlock->getContentSize().height/2));

	rightBlock->addChild(rightLight1);rightLight1->setPosition(ccp(rightBlock->getContentSize().width/2,rightBlock->getContentSize().height/2));
	rightBlock->addChild(rightLight2);rightLight2->setPosition(ccp(rightBlock->getContentSize().width/2,rightBlock->getContentSize().height/2));

	//BackgroudLayerFactory::getCurrentBgLayer()->addChild(leftBlock,0);
	//BackgroudLayerFactory::getCurrentBgLayer()->addChild(rightBlock,0);

	BaseScense* scene = BaseScense::getCurrentScene();
	CCLayer* bgCover = scene->getBackgroudCoverLayer();
	bgCover->addChild(leftBlock,0);
	bgCover->addChild(rightBlock,0);

	setStartEndPos(starPos,end);
	CCPoint vectorP1ToP2 = ccpSub(end,starPos);
	trackAngle = CC_RADIANS_TO_DEGREES(ccpAngle(vectorP1ToP2,ccp(1,0)));
	if (vectorP1ToP2.y < 0) // 修正
		trackAngle = -trackAngle;	

	updatePos();
	schedule(schedule_selector(TableBlock::follow));
}

void TableBlock::createTableBlock(CustomeSprite* table,CCPoint pos1,CCPoint pos2, CCPoint starPos,CCPoint end)
{

	//现在台面不需要卡子了
	return;
/*
	TableBlock* block = new TableBlock(table,pos1,pos2,starPos,end);
	block->autorelease();

	//BackgroudLayerFactory::getCurrentBgLayer()->addChild(block,1);

	BaseScense* scene = BaseScense::getCurrentScene();
	CCLayer* bgCover = scene->getBackgroudCoverLayer();
	bgCover->addChild(block,1);
*/
}
void TableBlock::setStartEndPos(CCPoint start,CCPoint end)
{
	mStartPos = start;
	mEndPos = end;

	tempDistanceSQ = ccpDistanceSQ(start,end);
}
void TableBlock::updatePos()
{
	float angle = -mTable->getRotation();
	
	CCPoint pos1 = ccpRotateByAngle(relativePos1,ccp(0,0),angle); // 绕旋转中心转
	CCPoint pos2 = ccpRotateByAngle(relativePos2,ccp(0,0),angle); // 绕旋转中心转

	leftBlock->setPosition(ccpAdd(pos1 , mTable->getPosition()));
	leftBlock->setRotation(trackAngle + 90);
	rightBlock->setPosition(ccpAdd(pos2 , mTable->getPosition()));
	rightBlock->setRotation(trackAngle + 90);

}
void TableBlock::follow(float time)
{
	updatePos();
	float dis = ccpDistanceSQ(mStartPos,mTable->getPosition());
	float percent = dis / tempDistanceSQ;

	if (percent < 0)
	{
		percent = 0;
	}
	if (percent > 1)
	{
		percent = 1;
	}
	
	{
		leftLight1->setOpacity(255 * percent);
		rightLight1->setOpacity(255 * percent);
	}

	{
		rightLight2->setOpacity(255 * (1-percent));
		leftLight2->setOpacity(255 * (1-percent));
	}
}


BoilerBubble::BoilerBubble()
	:mLife(0.0f),
	mAngle(0.0f),
	body(NULL)
{
	ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
	this->setBlendFunc(blend);
	mBaseAlpha = 0.1f + CCRANDOM_0_1() * 0.9;
	mBaseScale = 0.05f + CCRANDOM_0_1() * 0.4;
	schedule(schedule_selector(BoilerBubble::step));
}
void BoilerBubble::step(float dt)
{
	const float MAX_LIFE = 0.25f;
	float interp = mLife / MAX_LIFE;
	float alpha = 255.0f;
	float fullalphaTime = 0.6f;
	if(interp > fullalphaTime)
	{
		 alpha = 255.0f * (1.0f - (interp - fullalphaTime) / (1.0f - fullalphaTime));
	}
	this->setOpacity(mBaseAlpha * alpha);
	float scale = mBaseScale + 0.1f * interp;
	//this->setScale(scale);
	this->setScaleX(mBaseScale * (1.2f - 0.2f * interp));
	this->setScaleY(mBaseScale);
	this->setRotation(360.0f - mAngle);
	mLife += dt;
	b2Body* body = this->body;
	if(mLife > MAX_LIFE && body)
	{
		
		b2World* world = body->GetWorld();
		world->DestroyBody(body);
		body = NULL;
		getParent()->removeChild(this);
	}
}
