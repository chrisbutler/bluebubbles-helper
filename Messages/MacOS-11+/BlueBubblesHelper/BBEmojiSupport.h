#import <Foundation/Foundation.h>

static inline BOOL BBSupportsEmojiTapbacks(void) {
    Class emoji = NSClassFromString(@"IMEmojiTapback");
    Class chat = NSClassFromString(@"IMChat");
    return [[NSProcessInfo processInfo] operatingSystemVersion].majorVersion >= 26 &&
        emoji != Nil && chat != Nil &&
        [emoji instancesRespondToSelector:NSSelectorFromString(@"initWithEmoji:isRemoved:")] &&
        [chat instancesRespondToSelector:NSSelectorFromString(@"sendTapback:forChatItem:")];
}
