//
//  ContactListener.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "cocos2d.h"
#include "ContactListener.h"
#include "CommonDefine.h"
#include "BaseScense.h"
#include "BaseContentLayer.h"
#include "AchievementManager.h"


using namespace cocos2d;

ContactListener::ContactListener(BaseContentLayer* layer){
    mLayer = layer;
}

void ContactListener::BeginContact(b2Contact *contact){
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    
    
    
    CCSprite* spriteA = (CCSprite*)(bodyA->GetUserData());
    CCSprite* spriteB = (CCSprite*)bodyB->GetUserData();
    
    if (spriteA == NULL || spriteB ==NULL) {
        return;
    }
    
	AchievementManager::getInstance()->beginContact(spriteA,spriteB);

    if (spriteA!=NULL && spriteB!=NULL) {
        int tagA = spriteA->getTag();
        int tagB = spriteB->getTag();
        
        //桌面和红星相撞
        {
            if (tagA == Tag_Box_Table) {
                if (tagB == Tag_Box_RedStar) {
                    mLayer->redstarContactTable(bodyB->GetPosition(),bodyA);
                }
            }
            if (tagA == Tag_Box_RedStar) {
                if (tagB == Tag_Box_Table) {
                    mLayer->redstarContactTable(bodyA->GetPosition(),bodyB);
                }
            }
        }
        
        //弹簧床的碰撞 springBedContactedBegin
        {
            if (tagA == Tag_Box_SpringBed) 
            {
                mLayer->springBedContactedBegin(bodyA, bodyB);
            }
            if (tagB == Tag_Box_SpringBed)
            {
                mLayer->springBedContactedBegin(bodyB, bodyA);
            }
            
        }
        
        //和电磁炮相撞
        {
            if (tagA == Tag_Box_EMCannoon)
            {
                mLayer->setToDestory(bodyB);
            }
            if (tagB == Tag_Box_EMCannoon)
            {
                mLayer->setToDestory(bodyA);
            }
        }
        
        //红星和金星相撞
        {
            if (tagA == Tag_Box_GoldenStar) {
                if (tagB == Tag_Box_RedStar) {
                    //mLayer->redstarContactGoldenStar(bodyB->GetPosition(),bodyA);
                    mLayer->setToDestory(bodyA);
                }
            }
            if (tagA == Tag_Box_RedStar) {
                if (tagB == Tag_Box_GoldenStar) {
                    //mLayer->redstarContactGoldenStar(bodyA->GetPosition(),bodyB);
                    mLayer->setToDestory(bodyB);
                }
            }
        }
    }
}

void ContactListener::EndContact(b2Contact *contact){
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    
    CCSprite* spriteA = (CCSprite*)(bodyA->GetUserData());
    CCSprite* spriteB = (CCSprite*)bodyB->GetUserData();
    
    if (spriteA == NULL || spriteB ==NULL) {
        return;
    }
    
	AchievementManager::getInstance()->endContact(spriteA,spriteB);

    int tagA = spriteA->getTag();
    int tagB = spriteB->getTag();
    
    if (spriteA!=NULL && spriteB!=NULL) {
    
        if (tagA == Tag_Box_Table) {
            if (tagB == Tag_Box_RedStar) {
                mLayer->redstarEndContactTable();
            }
        }
        if (tagA == Tag_Box_RedStar) {
            if (tagB == Tag_Box_Table) {
                mLayer->redstarEndContactTable();
            }
        }
        
    }
    
    //弹簧床
    {
        //弹簧床的碰撞 springBedContactedBegin
        {
            if (tagA == Tag_Box_SpringBed)
            {
                mLayer->springBedContactEnd(bodyA, bodyB);
            }
            if (tagB == Tag_Box_SpringBed)
            {
                mLayer->springBedContactEnd(bodyB, bodyA);
            }
            
        }
    }
}

void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse){
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    
	CCNode* nodeA = (CCNode*)bodyA->GetUserData();
	CCNode* nodeB = (CCNode*)bodyB->GetUserData();
	if (nodeA && nodeB)
	{
		AchievementManager::getInstance()->PostSolve(nodeA,nodeB);
	}
	

    //for collision effect sound
    {
        b2Body* body = bodyA;
        b2Fixture* f = contact->GetFixtureA();
        
        
        CustomeSprite* sprite = (CustomeSprite*)body->GetUserData();
        
		// 弹簧床不播放声音
		if (nodeA->getTag() != Tag_Box_SpringBed || nodeB->getTag() != Tag_Box_SpringBed)
		{
			for (size_t i=0; i<sprite->mFixtureVelocityInfoVec.size(); ++i) {
				if (f == sprite->mFixtureVelocityInfoVec[i].fixture) {
					if (impulse->normalImpulses[0] > sprite->mFixtureVelocityInfoVec[i].collisionImpulse) {
						sprite->mFixtureVelocityInfoVec[i].collisionImpulse = impulse->normalImpulses[0];
					}
				}
			}
		}
    }
    {
        b2Body* body = bodyB;
        b2Fixture* f = contact->GetFixtureB();
        CustomeSprite* sprite = (CustomeSprite*)body->GetUserData();
        
		// 弹簧床不播放声音
		if (nodeA->getTag() != Tag_Box_SpringBed || nodeB->getTag() != Tag_Box_SpringBed)
		{
			for (size_t i=0; i<sprite->mFixtureVelocityInfoVec.size(); ++i) {
				if (f == sprite->mFixtureVelocityInfoVec[i].fixture) {
					if (impulse->normalImpulses[0] > sprite->mFixtureVelocityInfoVec[i].collisionImpulse) {
						sprite->mFixtureVelocityInfoVec[i].collisionImpulse = impulse->normalImpulses[0];
					}
				}
			}
		}
    }
    
    CCSprite* spriteA = (CCSprite*)(bodyA->GetUserData());
    CCSprite* spriteB = (CCSprite*)bodyB->GetUserData();
    
    if (spriteA!=NULL && spriteB!=NULL) {
        int tagA = spriteA->getTag();
        int tagB = spriteB->getTag();
        
        if (tagA == Tag_Box_Bomb) {
            for (int i=0; i<impulse->count; ++i) {
                if (abs(impulse->normalImpulses[i]) > BombImpulseSensitivity || abs(impulse->tangentImpulses[i]) > BombImpulseSensitivity) {
                    mLayer->setBombToExplosion(bodyA);
                }
            }
        }else if (tagB == Tag_Box_Bomb){
            for (int i=0; i<impulse->count; ++i) {
                if (abs(impulse->normalImpulses[i]) > BombImpulseSensitivity || abs(impulse->tangentImpulses[i]) > BombImpulseSensitivity) {
                    mLayer->setBombToExplosion(bodyB);
                }
            }
        }
        
        if (tagA == Tag_Box_SpringBed)
        {
            for (int i=0; i<impulse->count; ++i) {
                if (abs(impulse->normalImpulses[i]) > SpringBedSensity || abs(impulse->tangentImpulses[i]) > SpringBedSensity) {
                    
                    mLayer->springBedContact(bodyA, bodyB,
                                             impulse->normalImpulses[i]>impulse->tangentImpulses[i]?
                                             impulse->normalImpulses[i]:impulse->tangentImpulses[i]);
                    
                    break;
                }
            }
            
        }
        if (tagB == Tag_Box_SpringBed)
        {
            for (int i=0; i<impulse->count; ++i) {
                if (abs(impulse->normalImpulses[i]) > SpringBedSensity || abs(impulse->tangentImpulses[i]) > SpringBedSensity) {
                    
                    mLayer->springBedContact(bodyB, bodyA,
                                             impulse->normalImpulses[i]>impulse->tangentImpulses[i]?
                                             impulse->normalImpulses[i]:impulse->tangentImpulses[i]);
                    
                    break;
                }

            }
        }



		// 红星和安全边框的接触
		{
#define MAX_DISTANCE	10.0f
#define MIN_DISTANCE	5.0f
#define COLLI_POWER		1000.0f
			if (tagA == Tag_Box_RedStar && 
				(tagB == Tag_SafeBoard_L || tagB == Tag_SafeBoard_R || tagB == Tag_SafeBoard_T || tagB == Tag_SafeBoard_D))
			{
				b2WorldManifold manifold;
				contact->GetWorldManifold(&manifold);
				// 取碰撞点为能量点
				b2Vec2 forceCenter = manifold.points[0];

				{
					b2Body* b = bodyA;
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



						if (distance > MAX_DISTANCE) {
							continue;
						}

						b2Vec2 force = b2Vec2(0,0);
						//防止锅炉距离很近的时候力量太大
						force = b2Vec2(
							COLLI_POWER*(xDelta/distance)/pow(distance+MIN_DISTANCE,2.0f), 
							COLLI_POWER*(yDelta/distance)/pow(distance+MIN_DISTANCE,2.0f));

						if (pow(force.x, 2)+pow(force.y, 2) >= COLLI_POWER)
						{
							b2Vec2 imp;
							imp.x = force.x * 1.0f/60.0f;
							imp.y = force.y * 1.0f/60.0f;
							b->ApplyLinearImpulse(imp, shapeCenterP);
						}
					}
				}
			}


			if (tagB == Tag_Box_RedStar && 
				(tagA == Tag_SafeBoard_L || tagA == Tag_SafeBoard_R || tagA == Tag_SafeBoard_T || tagA == Tag_SafeBoard_D))
			{
				b2WorldManifold manifold;
				contact->GetWorldManifold(&manifold);
				// 取碰撞点为能量点
				b2Vec2 forceCenter = manifold.points[0];

				{
					b2Body* b = bodyB;
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



						if (distance > MAX_DISTANCE) {
							continue;
						}

						b2Vec2 force = b2Vec2(0,0);
						//防止锅炉距离很近的时候力量太大
						force = b2Vec2(
							MAX_DISTANCE*(xDelta/distance)/pow(distance+MIN_DISTANCE,2.0f), 
							MAX_DISTANCE*(yDelta/distance)/pow(distance+MIN_DISTANCE,2.0f));

						if (pow(force.x, 2)+pow(force.y, 2) >= COLLI_POWER)
						{
							b2Vec2 imp;
							imp.x = force.x * 1.0f/60.0f;
							imp.y = force.y * 1.0f/60.0f;
							b->ApplyLinearImpulse(imp, shapeCenterP);
						}
					}
				}
			}


		}

        
    }
}







