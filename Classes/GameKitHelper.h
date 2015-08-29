//
//  GameKitHelper.h
//  Cocos2dx_GameCenter
//
//  Created by Ryan.wu on 12-10-12.
//
//

//
//  GameKitHelper.h
//  toDefendTheEarth
//
//  Created by jingjing on 12-6-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
#import <sqlite3.h>

@interface GameKitHelper :  NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKMatchmakerViewControllerDelegate, GKMatchDelegate>{
    BOOL gameCenterAvailable;
    BOOL userAuthenticated;
    GKLocalPlayer* mLocalPlayer;
    sqlite3 *db;  
    
    
//    NSError* lastError;
//    NSMutableDictionary* achievements;
}

@property (assign, readonly) BOOL gameCenterAvailable;

+ (GameKitHelper *)sharedGameKitHelper;
- (void) authenticateLocalUser;

- (void) showLeaderboard;
- (void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;

/**
 *  显示成就界面
 */
- (void) showAchievements;
- (void) achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;
//获取成就描述和图片
- (NSArray*)retrieveAchievmentMetadata;
- (void) loadAchievements;

- (GKAchievement*) getAchievementByID:(NSString*) identifier;
- (void) reportAchievementWithID:(NSString*) identifier percentComplete:(float) percent;

- (void) reportScore: (int64_t) score forCategory: (NSString*) category;

- (void) reSendFailedAchieveData;

// 比赛
- (void) match:(GKMatch *)match didReceiveData:(NSData *)data fromPlayer:(NSString *)playerID;
//- (void) match:(GKMatch *)match didFailWithError:(NSError *)error;

- (void) matchmakerViewController:(GKMatchmakerViewController *)viewController didFailWithError:(NSError *)error;
- (void) matchmakerViewControllerWasCancelled:(GKMatchmakerViewController *)viewController;
//- (void) mat



// 数据库操作

// 打开数据库，没有就创建，失败就返回false
- (BOOL) createDataBaseByName:(NSString*) name;

- (void) closeDB;

- (void) logDB;

// 创建表，如果没有创建的话
- (BOOL) InitDBIfNotInitialized;

// 添加一个成就
- (void) addOneAchievementToDB:(NSString*) playerId Achieve:(NSString*) achieveId;
// 添加一个成绩
- (void) addOneScoreToDB:(NSString*) playerId Score:(int) score;

// 删除一个成就
- (void) deleteOneAchievementByPlayerId:(NSString*) playerId Achieve:(NSString*) achieveId;

- (void) deleteAllAchievementByPlayerId:(NSString*) playerId;

- (void) clearTable;

-(BOOL) execSql:(NSString *)sql;

@end

#endif