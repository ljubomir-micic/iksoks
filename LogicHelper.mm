//
//  LogicHelper.m
//  iksoks
//
//  Created by Љубомир Мићић on 1. 5. 2025..
//

#import "LogicHelper.h"
#import "connection.h"
#import "tabla.h"

@implementation LogicHelper {
    connection* conn;
    tabla* gameBoard;
@public
    BOOL isMultiplayer;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        // HOTEL HOLIVUD KARMA U LOBIJU
        isMultiplayer = FALSE;
        [self startGame];
    }
    return self;
}

- (int)initMultiplayer:(BOOL) isServ
                      :(NSString *)ip {
    if (self) {
        char* str = strdup([ip UTF8String]);
        conn = new connection(isServ, str);
        isMultiplayer = conn != nullptr;
        free(str);
    }
    return isMultiplayer;
}

- (int)sendData: (NSString *)data {
    if (self && isMultiplayer) {
        char* str = strdup([data UTF8String]);
        conn->sendSignal(str);
        free(str);
    }
    return isMultiplayer;
}

- (NSString *)recvData {
    return nullptr;
}

- (int)deinitMultiplayer {
    if (conn != nullptr) {
        delete conn;
        conn = nullptr;
        isMultiplayer = conn == nullptr;
    }
    return !isMultiplayer;
}

- (int)startGame {
    try {
        if (gameBoard != nullptr) { delete gameBoard; }
        gameBoard = new tabla();
        return 0;
    } catch (...) {
        return 1;
    }
    
    return 1;
}

- (int)handleInput:(int)x
                  :(int)y {
    return gameBoard->handleInput(x, y);
}

- (int)getValue:(int)x
               :(int)y {
    return gameBoard->getValue(x, y);
}

- (int)nextToVanish {
    return gameBoard->nextToVanish();
}

- (int)turn_t {
    if (self) {
        return gameBoard->turn_t();
    }
    return 1;
}

@end
