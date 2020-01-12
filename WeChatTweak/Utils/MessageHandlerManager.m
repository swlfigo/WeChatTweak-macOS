//
//  MessageHandlerManager.m
//  QQRedPackHelper
//
//  Created by Sylar on 2020/1/1.
//  Copyright © 2020 tangxianhai. All rights reserved.
//

#import "MessageHandlerManager.h"
#import <AFNetworking/AFNetworking.h>
#import <GCDWebServer.h>
#import <GCDWebServerDataResponse.h>
#import <GCDWebServerURLEncodedFormRequest.h>
#import "WeChatTweakHeaders.h"
@interface MessageHandlerManager()
@property(nonatomic,strong)AFHTTPSessionManager *manager;
@property (nonatomic, strong, nullable) GCDWebServer *server;
@end

static int port = 4444;

@implementation MessageHandlerManager
+ (instancetype)sharedInstance{
    static MessageHandlerManager *config = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        config = [[MessageHandlerManager alloc] init];
    });
    return config;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _manager = [AFHTTPSessionManager manager];
        _manager.responseSerializer = [AFJSONResponseSerializer serializer];//返回格式 JSON
        _manager.responseSerializer.acceptableContentTypes=[[NSSet alloc] initWithObjects:@"application/xml", @"text/xml",@"text/html", @"application/json",@"text/plain",nil];
        _manager.requestSerializer = [AFJSONRequestSerializer serializer];
    }
    return self;
}
- (void)postMessageToServer:(NSDictionary *)infoDic{
    if (!infoDic) {
        return;
    }
    //本地5400端口
    [_manager POST:@"http://127.0.0.1:5400/wechatmessage" parameters:infoDic progress:nil success:nil failure:nil];
}

- (void)startListener {
    if (self.server != nil) {
        return;
    }
    self.server = [[GCDWebServer alloc] init];
    [self.server addHandlerForMethod:@"POST" path:@"/wechat/send-message" requestClass:[GCDWebServerDataRequest class] processBlock:^GCDWebServerResponse * _Nullable(__kindof GCDWebServerRequest * _Nonnull request) {
        NSDictionary *requestBody = [request jsonObject];
        if (requestBody && requestBody[@"toUserID"]) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [MessageHandlerManager  sendMessageWithInfo:requestBody];
            });
            return [GCDWebServerResponse responseWithStatusCode:200];
        }
        return [GCDWebServerResponse responseWithStatusCode:404];
    }];
    
    [self.server startWithOptions:@{GCDWebServerOption_Port: [NSNumber numberWithInt:port],
                                    GCDWebServerOption_BindToLocalhost: @(YES)} error:nil];
}



+ (void)sendMessageWithInfo:(NSDictionary *)messageInfo{
    if (!messageInfo[@"msgType"]) return;
    NSString *keyword = messageInfo[@"toUserID"];
    if (!keyword) return;
    NSArray<WCContactData *> *contacts = ({
        MMServiceCenter *serviceCenter = [objc_getClass("MMServiceCenter") defaultCenter];
        ContactStorage *contactStorage = [serviceCenter getService:objc_getClass("ContactStorage")];
        GroupStorage *groupStorage = [serviceCenter getService:objc_getClass("GroupStorage")];
        NSMutableArray<WCContactData *> *array = [NSMutableArray array];
        [array addObjectsFromArray:[contactStorage GetAllFriendContacts]];
        [array addObjectsFromArray:[groupStorage GetGroupContactList:2 ContactType:0]];
        array;
    });
    NSArray<WCContactData *> *results = ({
        NSMutableArray<WCContactData *> *results = [NSMutableArray array];
        for (WCContactData *contact in contacts) {
            BOOL isOfficialAccount = (contact.m_uiCertificationFlag >> 0x3 & 0x1) == 1;
            BOOL containsNickName = [contact.m_nsNickName.lowercaseString containsString:keyword];
            BOOL containsUsername = [contact.m_nsUsrName.lowercaseString containsString:keyword];
            BOOL containsAliasName = [contact.m_nsAliasName.lowercaseString containsString:keyword];
            BOOL containsRemark = [contact.m_nsRemark.lowercaseString containsString:keyword];
            BOOL containsNickNamePinyin = [contact.m_nsFullPY.lowercaseString containsString:keyword];
            BOOL containsRemarkPinyin = [contact.m_nsRemarkPYFull.lowercaseString containsString:keyword];
            BOOL matchRemarkShortPinyin = [contact.m_nsRemarkPYShort.lowercaseString isEqualToString:keyword];
            if (!isOfficialAccount && (containsNickName || containsUsername || containsAliasName || containsRemark || containsNickNamePinyin || containsRemarkPinyin || matchRemarkShortPinyin)) {
                [results addObject:contact];
            }
        }
        results;
    });
    if (results.count == 0) {
        return;
    }
    NSInteger msgType = [messageInfo[@"msgType"] integerValue];
    if (msgType == 1) {
        //文字消息
        NSString *msg = messageInfo[@"msgContent"];
        if (!msg || msg.length == 0) return;
        NSString *currentUserName = [objc_getClass("CUtility") GetCurrentUserName];
        MessageService *service = [[objc_getClass("MMServiceCenter") defaultCenter] getService:objc_getClass("MessageService")];
        [service SendTextMessage:currentUserName toUsrName:keyword msgText:msg atUserList:nil];
    }else if (msgType == 3){
//        图片信息
        NSString *filePath = messageInfo[@"file-Path"];
        if (!filePath) return;
        if ([[NSFileManager defaultManager]fileExistsAtPath:filePath] == NO) return;
        NSURL *fileURLPath = [NSURL fileURLWithPath:filePath];
        NSData *imageData = [NSData dataWithContentsOfURL:fileURLPath];
        if (!imageData) return;
//        NSString *currentUserName = [objc_getClass("CUtility") GetCurrentUserName];
//        [[objc_getClass("MMMessageSendLogic") alloc]sendImageMessageWithImageData:imageData imageInfo:nil];
    }
}



- (void)stopListener {
    if (self.server == nil) {
        return;
    }
    [self.server stop];
    [self.server removeAllHandlers];
    self.server = nil;
}
@end
