//
//  MessageHandlerManager.h
//  QQRedPackHelper
//
//  Created by Sylar on 2020/1/1.
//  Copyright © 2020 tangxianhai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GCDWebServer/GCDWebServer.h>
#import <GCDWebServer/GCDWebServerDataResponse.h>

NS_ASSUME_NONNULL_BEGIN

@interface MessageHandlerManager : NSObject

+ (instancetype)sharedInstance;
+ (void)sendMessageWithInfo:(NSDictionary*)messageInfo;
-(void)postMessageToServer:(NSDictionary*)infoDic;
- (void)startListener;
- (void)stopListener;
@end

NS_ASSUME_NONNULL_END
