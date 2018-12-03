#ifndef RICH_GAME_H
#define RICH_GAME_H

#include "cmd.h"

typedef unsigned char u8;

typedef struct Player {
    u8 label;
    int money;
    int points;
    int position;
    int skip; // 轮空次数
    int bankrupt;

    struct Gift {
        u8 barrier;
        u8 bomb;
        u8 robot;
        u8 god;
    } gifts;
} Player;

typedef struct Players {
    int player_num;
    int current_idx;
    Player players[4];
} Players;

typedef struct Map {
    u8 level;
    u8 owner;
    u8 gift;
} Map;

typedef struct GlobalData {
    int *player_num;
    int *current_idx;
    Player players[4];
    Map *map;
} GlobalData;

typedef struct {
    int returnCode;
    u8 string[50];
} RETURNMAP;

extern int player_num;
extern int current_idx; // index for current user
extern Player players[4];
extern Map map[70];
extern u8 cmd_string[50];
extern int returnCode;

//Map中level的值
#define    MAP_LEVEL_0                        '0'
#define    MAP_LEVEL_1                        '1'
#define    MAP_LEVEL_2                        '2'
#define    MAP_LEVEL_3                        '3'
#define    MAP_LEVEL_TOOL                    'T'
#define    MAP_LEVEL_START                    'S'
#define    MAP_LEVEL_GIFT                    'G'
#define    MAP_LEVEL_MAGIC                    'M'
#define    MAP_LEVEL_HOSPITAL                'H'
#define    MAP_LEVEL_PRISON                'P'
#define    MAP_LEVEL_MINE                    '$'

//Map中owner的值
#define    MAP_OWNER_NONE                    '0'
#define    MAP_OWNER_Q                        'Q'
#define    MAP_OWNER_A                        'A'
#define    MAP_OWNER_S                        'S'
#define    MAP_OWNER_J                        'J'

//Map中gift的值
#define    MAP_GIFT_NONE                    '0'
#define    MAP_GIFT_BARRIER                '#'
#define    MAP_GIFT_BOMB                    '@'

enum {
    CMD_NOT_FOUND = -1,             //命令没找到
    CMD_SUCCESS = 0,                //正常
    CMD_NULL = 1,                   //正常状态
    CMD_PRAM_ERROR = 2,
    CMD_GIFT_NOT_ENOUGH = 3,
    CMD_INCORRENT_POSITION = 4,
};

#define false                            0
#define true                            1

void init_map();

void set_pos(int x, int y);

void init_map();

int show_result(int errorCode);

int name_init(u8 *);

int money_init(s32 money);

int user_init(u8 *username, s32 money);

int game_start();

#endif