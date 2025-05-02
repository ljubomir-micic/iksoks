//
//  LogicHelper.h
//  iksoks
//
//  Created by Љубомир Мићић on 1. 5. 2025..
//

#ifndef LogicHelper_h
#define LogicHelper_h

#import <Foundation/Foundation.h>

@interface LogicHelper : NSObject
- (instancetype) init;
- (int) initMultiplayer:(BOOL) isServ
                       :(NSString *)ip;
- (int) sendData: (NSString*) data;
- (NSString*) recvData;
- (int) deinitMultiplayer;
- (int) startGame;
- (int) handleInput:(int) x
                   :(int) y;
- (int)getValue:(int)x
               :(int)y;
- (int) nextToVanish; // return 4bit number
- (BOOL) isMultiplayer;
- (int) turn_t;
@end

#endif /* LogicHelper_h */
