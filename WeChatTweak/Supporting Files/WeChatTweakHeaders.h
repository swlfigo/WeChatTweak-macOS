//
//  WeChatTweakHeaders.h
//  WeChatTweak
//
//  Created by Sunnyyoung on 2017/8/11.
//  Copyright © 2017年 Sunnyyoung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>
#import <objc/message.h>

typedef NS_ENUM(unsigned int, MessageDataType) {
    MessageDataTypeText     = 1,
    MessageDataTypeImage    = 3,
    MessageDataTypeVoice    = 34,
    MessageDataTypeVideo    = 43,
    MessageDataTypeSticker  = 47,
    MessageDataTypeAppUrl   = 49,
    MessageDataTypePrompt   = 10000
};

@interface NSString (MD5)

- (NSString *)md5String;

@end

@interface WeChat : NSObject

@property(nonatomic) BOOL isAppTerminating;

+ (instancetype)sharedInstance;
- (void)lock:(id)block;
- (void)showMainWindow;
- (void)startANewChatWithContact:(id)contact;

@end

@interface PathUtility : NSObject

+ (NSString *)GetCurUserDocumentPath;

@end

@interface MMSearchResultItem : NSObject

@property(nonatomic) unsigned long long type; // 0 is single chat, 1 is group chat
@property(readonly, nonatomic) NSString *identifier;

@end

@interface MessageData: NSObject

@property(nonatomic) MessageDataType messageType;
@property(nonatomic) unsigned int msgStatus;
@property(nonatomic) long long mesSvrID;
@property(retain, nonatomic) NSString *toUsrName;
@property(retain, nonatomic) NSString *fromUsrName;
@property(retain, nonatomic) NSString *msgContent;
@property(nonatomic) unsigned int msgCreateTime;
@property(nonatomic) unsigned int mesLocalID;

- (instancetype)initWithMsgType:(long long)arg1;
- (BOOL)isSendFromSelf;
- (id)getChatNameForCurMsg;

@end

@interface WCContactData : NSObject

@property(nonatomic) unsigned int m_uiCertificationFlag;
@property(retain, nonatomic) NSString *m_nsHeadImgUrl;
@property(retain, nonatomic) NSString *m_nsNickName;
@property(retain, nonatomic) NSString *m_nsUsrName;
@property(retain, nonatomic) NSString *m_nsAliasName;
@property(retain, nonatomic) NSString *m_nsRemark;
@property(retain, nonatomic) NSString *m_nsFullPY;
@property(retain, nonatomic) NSString *m_nsRemarkPYShort;
@property(retain, nonatomic) NSString *m_nsRemarkPYFull;
@property(retain, nonatomic) NSString *wt_avatarPath;

- (BOOL)isChatStatusNotifyOpen;

@end

@interface ContactStorage : NSObject

- (WCContactData *)GetContact:(NSString *)session;
- (NSArray<WCContactData *> *)GetAllFriendContacts;

@end

@interface GroupStorage: NSObject

- (WCContactData *)GetGroupContact:(NSString *)session;
- (NSArray<WCContactData *> *)GetAllGroups;
- (NSArray<WCContactData *> *)GetGroupContactList:(NSInteger)arg1 ContactType:(NSInteger)arg2;

@end

@interface MMServiceCenter : NSObject

+ (id)defaultCenter;
- (id)getService:(Class)name;

@end

@interface MessageService: NSObject

- (id)GetMsgData:(id)arg1 svrId:(unsigned long long)arg2;
- (void)DelMsg:(id)arg1 msgList:(id)arg2 isDelAll:(BOOL)arg3 isManual:(BOOL)arg4;
- (void)AddLocalMsg:(id)arg1 msgData:(id)arg2;
- (void)notifyDelMsgOnMainThread:(id)arg1 msgData:(id)arg2;
- (void)notifyAddRevokePromptMsgOnMainThread:(id)arg1 msgData:(id)arg2;

//消息
- (void)OnSyncModMsgStatus:(id)arg1;
- (void)OnSyncBatchAddFunctionMsgs:(id)arg1 isFirstSync:(BOOL)arg2;
- (void)FFImgToOnFavInfoInfoVCZZ:(NSArray*)arg1 isFirstSync:(BOOL)arg2;
- (void)notifyUIAndSessionOnMainThread:(id)arg1 withMsg:(id)arg2;

//Get
- (id)GetMsgData:(id)arg1 localId:(unsigned int)arg2;
@end

@interface AccountService: NSObject

- (BOOL)canAutoAuth;
- (void)AutoAuth;
- (void)onAuthOKOfUser:(id)arg1 withSessionKey:(id)arg2 withServerId:(id)arg3 autoAuthKey:(id)arg4 isAutoAuth:(BOOL)arg5;

@end

@interface LogoutCGI: NSObject

- (void)sendLogoutCGIWithCompletion:(id)arg1;

@end

@interface MMAvatarService: NSObject

- (NSString *)avatarCachePath;

@end

@protocol MASPreferencesViewController <NSObject>

@property(readonly, nonatomic) NSString *toolbarItemLabel;
@property(readonly, nonatomic) NSImage *toolbarItemImage;
@property(readonly, nonatomic) NSString *identifier;

@optional
@property(readonly, nonatomic) BOOL hasResizableHeight;
@property(readonly, nonatomic) BOOL hasResizableWidth;

@end

@interface MASPreferencesWindowController: NSWindowController

- (id)initWithViewControllers:(NSArray *)arg1;

@end

@interface MMMessageTableItem : NSObject

@property(retain, nonatomic) MessageData *message;

@end

@interface MMMessageCellView : NSTableCellView

@property(retain, nonatomic) NSView *avatarImgView;
@property(retain, nonatomic) MMMessageTableItem *messageTableItem;

@end

@interface MMImageMessageCellView : MMMessageCellView

@property(retain, nonatomic) NSImage *displayedImage;

@end


@interface MMCDNDownloadMgr : NSObject

- (BOOL)downloadCDNFileWithMessage:(id)arg1 type:(int)arg2 destinationPath:(id)arg3 signature:(id)arg4 fakeAeskey:(id)arg5 fakeSignature:(id)arg6;
- (BOOL)downloadCDNFileWithMessage:(id)arg1 type:(int)arg2 signature:(id)arg3 fakeAeskey:(id)arg4 fakeSignature:(id)arg5;
- (id)imageTmpPathWithMessage:(id)arg1;
- (id)imagePathWithMessage:(id)arg1;
- (BOOL)downloadImageWithMessage:(id)arg1 disableHevc:(BOOL)arg2;
- (BOOL)downloadImageWithMessage:(id)arg1;

@end

@interface SKBuiltinString_t : NSObject
@property(retain, nonatomic, setter=SetString:) NSString *string; // @synthesize string;
@end

@interface AddMsg : NSObject
@property(retain, nonatomic, setter=SetContent:) SKBuiltinString_t *content; // @synthesize content;
@property(retain, nonatomic, setter=SetFromUserName:) SKBuiltinString_t *fromUserName; // @synthesize fromUserName;
@property(nonatomic, setter=SetMsgType:) int msgType; // @synthesize msgType;
@property(retain, nonatomic, setter=SetToUserName:) SKBuiltinString_t *toUserName; // @synthesize toUserName;
@property (nonatomic, assign) unsigned int createTime;
@property(nonatomic, setter=SetNewMsgId:) long long newMsgId;
@end

@interface MMMessageCacheMgr : NSObject
- (void)downloadImageWithURLString:(id)arg1 thumbPath:(id)arg2 message:(id)arg3 completion:(id)arg4;
@end
