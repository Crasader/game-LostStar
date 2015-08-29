//
//  Bomb.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-13.
//
//

#ifndef RedStarFall_Bomb_h
#define RedStarFall_Bomb_h

#include "cocos2d.h"
#include <vector>
#include "CustomeSprite.h"

using namespace std;
using namespace cocos2d;

class Bomb : public CCSprite 
{
public:
    //创建一个炸弹，full为true那么就是创建一个360度的炸弹。
    //不然就是180度的。
    static Bomb* createBomb(bool full);
    //播放爆炸动画
    void bomb();
    ~Bomb();
	Bomb(bool full);
private:
    void bombEnd(CCNode* bomb);
	virtual void onExit();
    CCAction* bombAction;
	CCParticleSystem* par;

	CCParticleSystem* createParticle();
};

class BombCache
{
private:
	static BombCache* cache;
	vector<Bomb*> bufferVec;

	BombCache();
public:
	static BombCache* getCache();
	void clear();
	void newOneInBuffer();
	void addOneInBuffer(Bomb* bomb);
	Bomb* getBomb();
};


#endif
