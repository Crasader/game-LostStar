//
//  VRope.h - 0.3
//
//  Updated by patrick on 28/10/2010.
//

/*
Verlet Rope for cocos2d
 
Visual representation of a rope with Verlet integration.
The rope can't (quite obviously) collide with objects or itself.
This was created to use in conjuction with Box2d's new b2RopeJoint joint, although it's not strictly necessary.
Use a b2RopeJoint to physically constrain two bodies in a box2d world and use VRope to visually draw the rope in cocos2d. (or just draw the rope between two moving or static points)

*** IMPORTANT: VRope does not create the b2RopeJoint. You need to handle that yourself, VRope is only responsible for rendering the rope
*** By default, the rope is fixed at both ends. If you want a free hanging rope, modify VRope.h and VRope.mm to only take one body/point and change the update loops to include the last point. 
 
HOW TO USE:
Import VRope.h into your class
 
CREATE:
To create a verlet rope, you need to pass two b2Body pointers (start and end bodies of rope)
and a CCSpriteBatchNode that contains a single sprite for the rope's segment. 
The sprite should be small and tileable horizontally, as it gets repeated with GL_REPEAT for the necessary length of the rope segment.

ex:
CCSpriteBatchNode *ropeSegmentSprite = [CCSpriteBatchNode batchNodeWithFile:@"ropesegment.png" ]; //create a spritesheet 
[self addChild:ropeSegmentSprite]; //add batchnode to cocos2d layer, vrope will be responsible for creating and managing children of the batchnode, you "should" only have one batchnode instance
VRope *verletRope = [[VRope alloc] init:bodyA pointB:bodyB spriteSheet:ropeSegmentSprite];

 
UPDATING:
To update the verlet rope you need to pass the time step
ex:
[verletRope updateRope:dt];

 
DRAWING:
From your layer's draw loop, call the updateSprites method
ex:
[verletRope updateSprites];

Or you can use the debugDraw method, which uses cocos2d's ccDrawLine method
ex:
[verletRope debugDraw];
 
REMOVING:
To remove a rope you need to call the removeSprites method and then release:
[verletRope removeSprites]; //remove the sprites of this rope from the spritebatchnode
[verletRope release];
 
There are also a few helper methods to use the rope without box2d bodies but with CGPoints only.
Simply remove the Box2D.h import and use the "WithPoints" methods.
 

For help you can find me on the cocos2d forums, username: patrickC
Good luck :) 

*/

#ifndef VROPE_H_
#define VROPE_H_

#include "VPoint.h"
#include "VStick.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <vector>
#include "CustomeSprite.h"

using namespace cocos2d;

class VRope:public RecordSprite{
	int numPoints;
    std::vector<VPoint*> vPoints;
	std::vector<VStick> vSticks;
	std::vector<RecordSprite*> ropeSprites;
	CCSpriteBatchNode* spriteSheet;
	float antiSagHack;

    b2World* mWorld;
	b2Body *bodyA;
	b2Body *bodyB;
    b2RopeJoint* mJoint;
    
    float mRopeLength;
    
    //绳子被切断后逐渐消失，mSpritesOpacity变为0则需要销毁了
    GLubyte mSpritesOpacity;
    
	CustomeSprite* ropeAchorSprite;	//记录绳子的锚点，用于标记绳子是否是关键元素

public:
    void initWithBodys(b2World* world, b2Body* body1, b2Body* body2, CCSpriteBatchNode* spriteSheetArg);
    void initWithBodysAndRopeLenght(b2World* world, b2Body* body1, b2Body* body2, CCSpriteBatchNode* spriteSheetArg, float ropeLength);
    void update(float dt);
    void reset();
    //void shortenRope(float deltaLen);
    //void lengthenRope(float deltaLen);
    b2Body* getBodyA();
    b2Body* getBodyB();

	void setBodyA(b2Body* bodyA);
	//仅用于播放录制的脚本时，判断绳子是否被切断
	void setBodyB(b2Body* bodyB);
    
    bool checkRopeWhenBodyDestroied(b2Body* body);
    
    void initWithPoints(CCPoint pointA, CCPoint pointB, CCSpriteBatchNode* spriteSheetArg,float length = -1);
    void createRope(CCPoint pointA, CCPoint pointB, float ropeLenght = -1);
    void resetWithPoints(CCPoint pointA, CCPoint pointB);
    void updateWithPoints(CCPoint pointA, CCPoint pointB, float dt);
    void debugDraw();
    void updateSprites();
    void removeSprites();
    
    bool isRopeCutted();
    
    void destroiyRope();

	void setRopeBlockOpacity(size_t index,GLubyte o);	// 设置绳子某个小块的透明度
	int getRopeBlockNum();					// 得到绳子的小块数量，也就是里面SPrite的个数

    //两个点的连线如果穿过绳子则砍断，返回true。并且一根绳子剪为两节，另外一节由newRopeOut返回。否则返回false
    bool cutRopeWithTowPoints(CCPoint pA, CCPoint pB, VRope** newRopeOut);

	void createNewRope(VRope** newRopeOut, int pointsCounter);
	GLubyte getOpacity();
	void setOpacity(GLubyte Opacity){mSpritesOpacity = Opacity;};
    void setAnchorSprite(CustomeSprite* anchor)
	{
		ropeAchorSprite = anchor;
	}
	CustomeSprite* getAnchor()
	{
		return ropeAchorSprite;
	}
	VRope();
    ~VRope();
	void changeNum(CCNode* pNode);
};

#endif















