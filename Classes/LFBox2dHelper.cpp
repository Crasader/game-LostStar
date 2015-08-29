#include "LFBox2dHelper.h"

LF_SPACE_BEGIN

//
b2Body* whichBodyContainThePoint(b2Vec2 p,b2World* mWorld,float32 (*getExtensionValue)(b2Body* currentBody),bool (* ignoreMe)(b2Body* currentBody)){

	b2Fixture* fixture = whichFixtureContainThePoint(p,mWorld,getExtensionValue,ignoreMe);

	if (fixture == NULL) {
		return NULL;
	}

	return fixture->GetBody();
}


//
b2Fixture* whichFixtureContainThePoint(b2Vec2 p,b2World* mWorld, float32 (*getExtensionValue)(b2Body* currentBody),bool (* ignoreMe)(b2Body* currentBody))
{
	//
	for (b2Body* b = mWorld->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {

			if (ignoreMe != NULL && (*ignoreMe)(b))
				continue;

			b2Fixture* fixture = b->GetFixtureList();

			for (; fixture!=NULL; fixture=fixture->GetNext())
			{
				b2Shape* shape = (b2Shape*)fixture->GetShape();

				b2Transform transform;
				transform.Set(b->GetPosition(), b->GetAngle());

				bool hit =shape->TestPoint(transform, p);
				if (hit)
				{
					return fixture;
				}
			}
		}
	}

	// 不需要扩展
	if (getExtensionValue == NULL)
	{
		return NULL;
	}

	// 扩展获取
	b2Fixture* fixtureRet = NULL;
	float currentMinDistacne = 999999.9f;
	for (b2Body* b = mWorld->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {

			if (ignoreMe != NULL && (*ignoreMe)(b))
				continue;

			float ext = (*getExtensionValue)(b);

			b2Fixture* fixture = b->GetFixtureList();
			for (; fixture!=NULL; fixture=fixture->GetNext()) {
				b2Shape* shape = (b2Shape*)fixture->GetShape();
				float distance = minDistanceFromShape(shape, p, b);
				if (distance < ext && distance < currentMinDistacne)
				{//距离更近
					currentMinDistacne = distance;
					fixtureRet = fixture;
				}
			}
		}
	}

	return fixtureRet;
}

bool isPointInTheBody(b2Vec2 p,b2Body* body, float32 (*getExtensionValue)(b2Body* currentBody))
{
	if (body->GetUserData() != NULL) {

		b2Fixture* fixture = body->GetFixtureList();

		for (; fixture!=NULL; fixture=fixture->GetNext())
		{
			b2Shape* shape = (b2Shape*)fixture->GetShape();

			b2Transform transform;
			transform.Set(body->GetPosition(), body->GetAngle());

			bool hit =shape->TestPoint(transform, p);
			if (hit)
			{
				return true;
			}
		}
	}

	// 不需要扩展
	if (getExtensionValue == NULL)
	{
		return false;
	}

	if (body->GetUserData() != NULL) {

		float ext = (*getExtensionValue)(body);

		b2Fixture* fixture = body->GetFixtureList();
		for (; fixture!=NULL; fixture=fixture->GetNext()) {
			b2Shape* shape = (b2Shape*)fixture->GetShape();
			float distance = minDistanceFromShape(shape, p, body);
			if (distance < ext)
			{//距离之内
				return true;
			}
		}
	}
	return false;
}

//
std::list<b2Vec2> getBodyFixtureCeneters(b2Body* body)
{
	b2Fixture* fixtureList = body->GetFixtureList();
	std::list<b2Vec2> centerPoints;
	for (; fixtureList!=NULL; fixtureList=fixtureList->GetNext()) 
	{
		b2Shape* shape = (b2Shape*)fixtureList->GetShape();
		b2Transform transform;//存储一个物体的位置和旋转角度的结构体
		transform.Set(body->GetPosition(), body->GetAngle());
		b2AABB aabb;//一个描述外围边框的结构体 bounding box

		//根据transform，计算出某个孩子的形状的。关联的边框坐标，存放在aabb中
		shape->ComputeAABB(&aabb, transform, 0);

		b2Vec2 shapeCenterP = aabb.GetCenter();//即将受力的形状的位置;

		centerPoints.insert(centerPoints.end(),shapeCenterP);
	}

	return centerPoints;
}

//
bool isAnyFixtureInLine(b2World* world,b2Vec2 startPoint, b2Vec2 endPoint,bool (* ignoreMe)(b2Body* currentBody),float testStep /* = 0.5 */)
{
	float32 xDelta = startPoint.x - endPoint.x;
	float32 yDelta = startPoint.y - endPoint.y;
	float32 distance = sqrt(pow((xDelta),2)+pow((yDelta), 2));

	{
		float32 plus = testStep;
		int testCount = distance/plus;
		float32 xPlus = plus * xDelta/distance;
		float32 yPlus = plus * yDelta/distance;
		b2Vec2 testP = startPoint;
		for (int i=0; i<testCount; ++i) {
			testP.x -= xPlus;
			testP.y -= yPlus;
			b2Fixture* fixtureTmp = whichFixtureContainThePoint(testP,world,NULL,ignoreMe);
			if (fixtureTmp != NULL) {
				return true;
			}
		}
	}
	return false;
}

//
bool isAnyFixtureInLine_NearstPos(b2Vec2 startPoint, float angle,float32 length,b2Vec2* point, std::vector<b2Fixture *> test,float testStep /* = 0.5 */)
{
	bool hit = false;

	vector<b2Vec2> interceptPoints;

	for (size_t i=0; i<test.size(); i++)
	{
		b2RayCastInput input;

		input.p1.Set(startPoint.x, startPoint.y);

		input.p2.Set(startPoint.x+cos(CC_DEGREES_TO_RADIANS(angle))*length,
			startPoint.y+sin(CC_DEGREES_TO_RADIANS(angle))*length);

		input.maxFraction = 1.0f;

		b2RayCastOutput output;

		bool h = test[i]->RayCast(&output, input, 0);

		if (h)
		{
			hit = true;
			b2Vec2 hitPoint = input.p1 +output.fraction * (input.p2 - input.p1);
			interceptPoints.insert(interceptPoints.end(), hitPoint);
		}
	}

	//找到最近点，返回
	if (hit) {
		float minDistance = (interceptPoints[0].x-startPoint.x)*(interceptPoints[0].x-startPoint.x)+
			(interceptPoints[0].y-startPoint.y)*(interceptPoints[0].y-startPoint.y);
		int minIndex = 0;
		for (size_t i=1; i<interceptPoints.size(); i++) {

			float temDistance = (interceptPoints[i].x-startPoint.x)*(interceptPoints[i].x-startPoint.x)+
				(interceptPoints[i].y-startPoint.y)*(interceptPoints[i].y-startPoint.y);

			if (temDistance<minDistance) {

				minDistance = temDistance;

				minIndex = i;
			}
		}

		point->Set(interceptPoints[minIndex].x, interceptPoints[minIndex].y);
		return true;
	}

	return false;
}
//
bool isAnyFixtureInLine2(b2Vec2 startPoint, b2Vec2 endPoint,b2Vec2* point, std::vector<b2Fixture *> test,float testStep /* = 0.5 */)
{
	for (size_t i=0; i<test.size(); i++)
	{

		b2RayCastInput input;

		input.p1.Set(startPoint.x, startPoint.y);

		input.p2.Set(endPoint.x,endPoint.y);

		input.maxFraction = 1.0f;

		b2RayCastOutput output;

		bool hit = test[i]->RayCast(&output, input, 0);

		if (hit)
		{
			b2Vec2 hitPoint = input.p1 +output.fraction * (input.p2 - input.p1);
			point->Set(hitPoint.x, hitPoint.y);
			return true;
		}
	}

	return false;
}
//
LF_SPACE_END