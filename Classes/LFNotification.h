//#pragma once
//
//#include "cocos2d.h"
//#include <list>
//
//using namespace std;
//using namespace cocos2d;
//
//
//// 可以被通知的元素，空类
//class LFNotfication_Notify{};
//typedef void (LFNotfication_Notify::*SEL_NotifyVoid)(void*);
//
//
//
//// 监听的元素
//class NotifyObserverItem
//{
//private:
//
//public:
//	NotifyObserverItem(LFNotfication_Notify *target, 
//		SEL_NotifyVoid selector,
//		const char *name,
//		void *defaultData);
//
//	// 传递数据
//	void performSelector(void *obj);
//};
////
//class LFNotification
//{
//private:
//	list<NotifyObserverItem*> itemVec;
//	static LFNotification* notify;
//	LFNotification(void);
//public:
//	static LFNotification* getNotify();
//
//	void addObserver();
//	void removeObserver();
//
//	~LFNotification(void);
//};
//
