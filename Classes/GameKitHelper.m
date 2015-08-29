
//
//  GameKitHelper.m
//  Cocos2dx_GameCenter
//
//  Created by Ryan.wu on 12-10-12.
//
//

//
//  GameKitHelper.m
//  toDefendTheEarth
//
//  Created by jingjing on 12-6-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//


#define DBNAME              @"gamecenter_failed.sqlite"
#define Table_Name          @"GamecenterFailed"
#define Player_Id           @"playerId"
#define Fail_type           @"fail_type"
#define AchievementId       @"AchievementId"
#define PlayerScore         @"score"

//int fail_type_ReportAchievement = 0;
//int fail_type_ReportScore       = 1;

#define fail_type_ReportAchievement  0
#define fail_type_ReportScore        1


#import "GameKitHelper.h"

@implementation GameKitHelper
@synthesize gameCenterAvailable;

//静态初始化 对外接口
static GameKitHelper *sharedHelper = nil;
static UIViewController* currentModalViewController = nil;
+ (GameKitHelper *) sharedGameKitHelper {
    if (!sharedHelper) {
        sharedHelper = [[GameKitHelper alloc] init];
    }
    return sharedHelper;
}

//用于验证
- (BOOL)isGameCenterAvailable {
    // check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    
    // check if the device is running iOS 4.1 or later
    NSString *reqSysVer =@"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer
                                           options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

- (id)init {
    if ((self = [super init])) {
        gameCenterAvailable = [self isGameCenterAvailable];
        mLocalPlayer = nil;
        
        // 数据库初始化
        [self createDataBaseByName:DBNAME];
        [self InitDBIfNotInitialized];
        
        [self logDB];
        
//        // 测试
//        {
//            [self addOneAchievementToDB:@"playerId001" Achieve:@"Achieve0001"];
//            [self addOneAchievementToDB:@"playerId002" Achieve:@"Achieve0002"];
//            [self addOneAchievementToDB:@"playerId003" Achieve:@"Achieve0003"];
//            [self addOneScoreToDB:@"playerId001" Score:1];
//            [self addOneScoreToDB:@"playerId002" Score:2];
//            [self addOneScoreToDB:@"playerId003" Score:3];
//            
//            [self logDB];
//            
//            [self clearTable];
//            
//            [self logDB];
//        }
        
        
        if (gameCenterAvailable) {
            NSNotificationCenter *nc =
            [NSNotificationCenter defaultCenter];
            [nc addObserver:self
                   selector:@selector(authenticationChanged)
                       name:GKPlayerAuthenticationDidChangeNotificationName
                     object:nil];
        }
    }
    return self;
}

// 后台回调登陆验证
// 验证通过了之后，才会调用 下面的authenticateWithCompletionHandler
// 切换之后，都会调用，所以这里设置变量。
- (void)authenticationChanged {
    
    if ([GKLocalPlayer localPlayer].isAuthenticated &&!userAuthenticated) {
        NSLog(@"Authentication changed: player authenticated.");
        userAuthenticated = TRUE;
    } else if (![GKLocalPlayer localPlayer].isAuthenticated && userAuthenticated) {
        NSLog(@"Authentication changed: player not authenticated");
        userAuthenticated = FALSE;
        
        mLocalPlayer = nil;
    }
    
}

- (void)authenticateLocalUser {
    
    if (!gameCenterAvailable) return;
    
    NSLog(@"Authenticating local user...");
    if ([GKLocalPlayer localPlayer].authenticated == NO)
    {
//        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:nil];
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
            if (error == nil) {
                userAuthenticated = YES;
                
                mLocalPlayer = [GKLocalPlayer localPlayer];
                
                [self reSendFailedAchieveData];
                
                //成功处理
                NSLog(@"成功");
                NSLog(@"1--alias--.%@",[GKLocalPlayer localPlayer].alias);
                NSLog(@"2--authenticated--.%d",[GKLocalPlayer localPlayer].authenticated);
                NSLog(@"3--isFriend--.%d",[GKLocalPlayer localPlayer].isFriend);
                NSLog(@"4--playerID--.%@",[GKLocalPlayer localPlayer].playerID);
                NSLog(@"5--underage--.%d",[GKLocalPlayer localPlayer].underage);
            }else {
                
                userAuthenticated = NO;
                
                // 登陆不成功，或者用户不登陆，就失败。
                mLocalPlayer = nil;
                //错误处理
                NSLog(@"失败  %@",error);
            }
        }];
        
        
    } else {
        NSLog(@"Already authenticated!");
    }
}

//显示排行榜
- (void) showLeaderboard
{
    if (!gameCenterAvailable) return;
    
    GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardController != nil) {
        leaderboardController.leaderboardDelegate = self;
        
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        currentModalViewController = [[UIViewController alloc] init];
        [window addSubview:currentModalViewController.view];
        [currentModalViewController presentModalViewController:leaderboardController animated:YES];
    }
}

- (void) showAchievements
{
    if (!gameCenterAvailable) return;
    
    GKAchievementViewController* achivViewContr = [[[GKAchievementViewController alloc] init] autorelease];
    if (achivViewContr != nil) {
        achivViewContr.achievementDelegate = self;
                
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        currentModalViewController = [[UIViewController alloc] init];
        [window addSubview:currentModalViewController.view];
        [currentModalViewController presentModalViewController:achivViewContr animated:YES];
        
        
    }
}
- (void) loadAchievements
{
    if (!gameCenterAvailable) return;
    
    NSMutableDictionary *achievementDictionary = [[NSMutableDictionary alloc] init];
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements,NSError *error)
     {
         if (error == nil) {
             NSArray *tempArray = [NSArray arrayWithArray:achievements];
             for (GKAchievement *tempAchievement in tempArray) {
                 [achievementDictionary setObject:tempAchievement forKey:tempAchievement.identifier];
                 NSLog(@"    completed:%d",tempAchievement.completed);
                 NSLog(@"    hidden:%d",tempAchievement.hidden);
                 NSLog(@"    lastReportedDate:%@",tempAchievement.lastReportedDate);
                 NSLog(@"    percentComplete:%f",tempAchievement.percentComplete);
                 NSLog(@"    identifier:%@",tempAchievement.identifier);
             }
         }
     }];
}

- (GKAchievement*) getAchievementByID: (NSString*) identifier
{
    if (!gameCenterAvailable) return nil;
    
    NSMutableDictionary *achievementDictionary = [[NSMutableDictionary alloc] init];
    GKAchievement *achievement = [achievementDictionary objectForKey:identifier];
    if (achievement == nil)
    {
        achievement = [[[GKAchievement alloc] initWithIdentifier:identifier] autorelease];
        [achievementDictionary setObject:achievement forKey:achievement.identifier];
    }
    return [[achievement retain] autorelease];
}

// 再没有登陆的情况下，我们就不管了。因为也不知到这个成就到底该属于谁
- (void) reportAchievementWithID: (NSString*) identifier percentComplete: (float) percent
{
    if (!gameCenterAvailable) return;
    
    GKAchievement *achievement = [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
    if (achievement)
    {
        achievement.percentComplete = percent;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             if (error != nil)
             {
                 //The proper way for your application to handle network errors is retain
                 //the achievement object (possibly adding it to an array). Then, periodically
                 //attempt to report the progress until it is successfully reported.
                 //The GKAchievement class supports the NSCoding protocol to allow your
                 //application to archive an achie
                 NSLog(@"报告成就进度失败 ,错误信息为: \n %@",error);
                 
                 
                 // 失败就存入数据库
                 if (mLocalPlayer != nil) {
                     [self addOneAchievementToDB:mLocalPlayer.playerID Achieve:identifier];
                 }
                 
             }else {
                 //对用户提示,已经完成XX%进度
                 NSLog(@"报告成就进度---->成功!");
                 NSLog(@"    completed:%d",achievement.completed);
                 NSLog(@"    hidden:%d",achievement.hidden);
                 NSLog(@"    lastReportedDate:%@",achievement.lastReportedDate);
                 NSLog(@"    percentComplete:%f",achievement.percentComplete);
                 NSLog(@"    identifier:%@",achievement.identifier);
             }
         }];
    }
}


- (NSArray*)retrieveAchievmentMetadata
{
    if (!gameCenterAvailable) return nil;
    
    //读取成就的描述
    [GKAchievementDescription loadAchievementDescriptionsWithCompletionHandler:
     ^(NSArray *descriptions, NSError *error) {
         if (error != nil)
         {
             // process the errors
             NSLog(@"读取成就说明出错");
         }
         if (descriptions != nil)
         {
             // use the achievement descriptions.
             for (GKAchievementDescription *achDescription in descriptions) {
                 NSLog(@"1..identifier..%@",achDescription.identifier);
                 NSLog(@"2..achievedDescription..%@",achDescription.achievedDescription);
                 NSLog(@"3..title..%@",achDescription.title);
                 NSLog(@"4..unachievedDescription..%@",achDescription.unachievedDescription);
                 NSLog(@"5............%@",achDescription.image);
                 
                 //获取成就图片,如果成就未解锁,返回一个大文号
                 /*
                  [achDescription loadImageWithCompletionHandler:^(UIImage *image, NSError *error) {
                  if (error == nil)
                  {
                  // use the loaded image. The image property is also populated with the same image.
                  NSLog(@"成功取得成就的图片");
                  UIImage *aImage = image;
                  UIImageView *aView = [[UIImageView alloc] initWithImage:aImage];
                  aView.frame = CGRectMake(50, 50, 200, 200);
                  aView.backgroundColor = [UIColor clearColor];
                  [[[CCDirector sharedDirector] openGLView] addSubview:aView];
                  }else {
                  NSLog(@"获得成就图片失败");
                  }
                  }];
                  */
             }
         }
     }];
    return nil;
}


- (void) reportScore: (int64_t) score forCategory: (NSString*) category
{
    if (!gameCenterAvailable) return;
    
    GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
    scoreReporter.value = score;
    
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if (error != nil)
        {
            // handle the reporting error
            NSLog(@"上传分数出错.");
            //If your application receives a network error, you should not discard the score.
            //Instead, store the score object and attempt to report the player’s process at
            //a later time.
            
            
            // 失败就存入数据库
            if (mLocalPlayer != nil) {
                [self addOneScoreToDB:mLocalPlayer.playerID Score:score];
            }
            
        }else {
            NSLog(@"上传分数成功");
        }
    }];
}

//关闭排行榜回调
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController{
    if(currentModalViewController !=nil){
        [currentModalViewController dismissModalViewControllerAnimated:NO];
        [currentModalViewController release];
        [currentModalViewController.view removeFromSuperview];
        currentModalViewController = nil;
    }
}

- (void) achievementViewControllerDidFinish:(GKAchievementViewController *)viewController
{
    if(currentModalViewController !=nil){
        [currentModalViewController dismissModalViewControllerAnimated:NO];
        [currentModalViewController release];
        [currentModalViewController.view removeFromSuperview];
        currentModalViewController = nil;
    }
}

- (void) match:(GKMatch *)match didReceiveData:(NSData *)data fromPlayer:(NSString *)playerID
{
    
}

- (void) matchmakerViewController:(GKMatchmakerViewController *)viewController didFailWithError:(NSError *)error
{
    
}

- (void) matchmakerViewControllerWasCancelled:(GKMatchmakerViewController *)viewController
{
    
}


// select * from test where playerId = '1111' and failType = 0;
// 验证通过之后调用了
- (void) reSendFailedAchieveData
{
    NSString* sqlQuery = [NSString stringWithFormat:@"select * from %@ where %@ = '%@' and %@ = %d;",
                          Table_Name,Player_Id,mLocalPlayer.playerID,Fail_type,fail_type_ReportAchievement];
    
    sqlite3_stmt * statement;
    
    // 取出 当前用户 还没有成功的
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:20];
    
    if (sqlite3_prepare_v2(db, [sqlQuery UTF8String], -1, &statement, nil) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            
            int keyId = sqlite3_column_int(statement, 0);
            char* playerId = (char*)sqlite3_column_text(statement, 1);
            int failType = sqlite3_column_int(statement, 2);
            char* achieveId = (char*)sqlite3_column_text(statement, 3);
            int score = sqlite3_column_int(statement, 4);
            
            // 暂时保存
            [array addObject:[NSString stringWithUTF8String:achieveId]];
            
            
            NSLog(@"key = [%d]  playerId = [%@]  failType = [%d] achievement = [%@]  score = [%d]",
                  keyId,
                  [NSString stringWithUTF8String:playerId],
                  failType,
                  [NSString stringWithUTF8String:achieveId],
                  score);
        }
    }
    sqlite3_finalize(statement);
    
    // 删除当前用户的未成功数据
    [self deleteAllAchievementByPlayerId:mLocalPlayer.playerID];
    
    NSEnumerator *enumerator;
    enumerator = [array objectEnumerator];
    
    // 依次重新发一次
    NSString* thingie;
    while (thingie = [enumerator nextObject]) {
        [self reportAchievementWithID:thingie percentComplete:100];
    }

}

// 数据库操作

- (BOOL) createDataBaseByName:(NSString *)name
{
    //连接数据库
    NSString* DBFilePath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    DBFilePath = [DBFilePath stringByAppendingPathComponent:DBNAME];

    if (sqlite3_open(DBFilePath.UTF8String, &db) != SQLITE_OK ) {
        NSLog(@"数据库 [%@] 打开(创建)失败",DBNAME);
        NSLog(@"%s,%d %s", __FILE__, __LINE__, sqlite3_errmsg(db));
        
        db = Nil;
        
        return NO;
    }
    return YES;
}

- (void) closeDB
{
    if (db != nil) {
        
        sqlite3_close(db);
        db = nil;
    }
    
}

- (void) logDB
{
    if (db == nil) {
        NSLog(@"logDB: data base is nil return.");
        return;
    }
    
    NSLog(@"\n\n================================DataBase====================================");
    
    NSString* sqlQuery = [NSString stringWithFormat:@"SELECT * FROM %@",Table_Name];
    
    sqlite3_stmt * statement;
    
    if (sqlite3_prepare_v2(db, [sqlQuery UTF8String], -1, &statement, nil) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            
            int keyId = sqlite3_column_int(statement, 0);
            char* playerId = (char*)sqlite3_column_text(statement, 1);
            int failType = sqlite3_column_int(statement, 2);
            char* achieveId = (char*)sqlite3_column_text(statement, 3);
            int score = sqlite3_column_int(statement, 4);
            
            NSLog(@"key = [%d]  playerId = [%@]  failType = [%d] achievement = [%@]  score = [%d]",
                  keyId,
                  [NSString stringWithUTF8String:playerId],
                  failType,
                  [NSString stringWithUTF8String:achieveId],
                  score);
        }
    }

    NSLog(@"\n================================DataBase====================================\n\n");
    
    sqlite3_finalize(statement);
}

- (BOOL) InitDBIfNotInitialized
{
    NSString* createTable = [NSString stringWithFormat:@"CREATE TABLE IF NOT EXISTS %@ (ID INTEGER PRIMARY KEY AUTOINCREMENT, %@ TEXT, %@ INTEGER, %@ TEXT, %@ INTEGER)",Table_Name,Player_Id,Fail_type,AchievementId,PlayerScore];
    
    BOOL ret = [self execSql:createTable];
    
    return ret;
}

-(void) addOneAchievementToDB:(NSString *)playerId Achieve:(NSString *)achieveId
{
    NSString *sql1 = [NSString stringWithFormat:
                      @"INSERT INTO '%@' ('%@', '%@', '%@' , '%@') VALUES ('%@', '%d', '%@','%d')",
                      Table_Name, Player_Id, Fail_type, AchievementId,PlayerScore,
                      playerId, fail_type_ReportAchievement,achieveId,-1];
    
    [self execSql:sql1];
}

// delete from test where playerid = '11111' and failType = 1 and Achievement = '0';
- (void) deleteOneAchievementByPlayerId:(NSString *)playerId Achieve:(NSString *)achieveId
{
    NSString *sql1 = [NSString stringWithFormat:
                      @"delete from %@ where %@ = '%@' and %@ = %d and %@ = '%@';",
                      Table_Name, Player_Id, playerId,Fail_type,fail_type_ReportAchievement, AchievementId,achieveId];
    
    [self execSql:sql1];
}

// delete from test where playerid = '2' and failType = 0;
- (void) deleteAllAchievementByPlayerId:(NSString *)playerId
{
    NSString *sql1 = [NSString stringWithFormat:
                      @"delete from %@ where %@ = '%@' and %@ = %d;",
                      Table_Name, Player_Id, playerId,Fail_type,fail_type_ReportAchievement];
    
    [self execSql:sql1];
}

// delete from test;
- (void) clearTable
{
    NSString *sql1 = [NSString stringWithFormat:@"delete from %@;",Table_Name];
    [self execSql:sql1];
}
-(void) addOneScoreToDB:(NSString *)playerId Score:(int)score
{
    NSString *sql1 = [NSString stringWithFormat:
                      @"INSERT INTO '%@' ('%@', '%@', '%@' , '%@') VALUES ('%@', '%d', '%@','%d')",
                      Table_Name, Player_Id, Fail_type,            AchievementId, PlayerScore,
                                  playerId, fail_type_ReportScore, @"none",       score];
    
    [self execSql:sql1];
}




-(BOOL)execSql:(NSString *)sql
{
    if (db == nil) {
        NSLog(@"execSql: data base is nil return. sql = [%@]",sql);
        return NO;
    }
    
    char *err;
    if (sqlite3_exec(db, [sql UTF8String], NULL, NULL, &err) != SQLITE_OK) {
        sqlite3_close(db);
        NSLog(@"数据库操作数据失败!");
        
        NSLog(@"%s,%d %s", __FILE__, __LINE__, sqlite3_errmsg(db));
        
        return NO;
    }
    return YES;
}

@end