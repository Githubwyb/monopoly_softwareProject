#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include "drawMap.h"
#include "cmd.h"
#include "events.h"

u8 state = 0;//玩家的状态
int player_num;
int current_idx; // index for current user
Player players[4] = {{'A', 1000, 50, 2,  0, 0, {'0', '0', '0', '0'}},
                     {'S', 1000, 50, 8,  0, 0, {'0', '0', '0', '0'}},
                     {'J', 1000, 50, 50, 0, 0, {'0', '0', '0', '0'}},
                     {'Q', 1000, 50, 60, 0, 0, {'0', '0', '0', '0'}}};
char DEFAULT_MAP[] =
        {'S', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
         'H', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
         'T', '0', '0', '0', '0', '0', '0',
         'G', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
         'P', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
         'M', '$', '$', '$', '$', '$', '$'};
Map map[70];
u8 cmd_string[50];
int returnCode = CMD_NULL;

int main() {
    //初始化游戏数据
    srand(time(NULL));
    printf("\t\t\t********welcome to the Monopoly ********\n\n\n");
    game_start();
    system("color 0f");

    //游戏主循环
    while (1) {
        system("CLS");

        //绘制地图
        drawMap();

        //show error code
        set_pos(23, 16);
        show_result(returnCode);
        returnCode = CMD_NULL;

        set_pos(23, 18);
        switch (state) {
            case 0:
                if (players[current_idx].skip > 0) //当轮空数不为0
                {
                    players[current_idx].skip--;
                    state = 2;
                    break;
                }
                gets(cmd_string);
                returnCode = cmd_inputHandle(cmd_string);
                break;
            case 1:
                hospital_judge(players + current_idx, map);
                land_judge(players + current_idx, map);
                break;
            case 2:
                current_idx++;
                if (current_idx == player_num)
                    current_idx = 0;
                state = 0;
                break;
        }
    }
    return 0;
}

void set_pos(int x, int y) //设置光标位置
{
    COORD pos;
    HANDLE hOutput;
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);
}

int check_player(u8 *player_num) {
    u8 i = 0;
    u8 j = 0;
    for (i = 0; i < 4 && player_num[i]; i++) {
        for (j = 0; j < i; j++) {
            if (player_num[j] == player_num[i]) {
                return -1;
            }
        }
    }
    return 0;
}

int game_start(void) {
    u8 state = 0;
    u8 player_num[5];
    u8 rc = 0;
    s32 money = 0;
    user_init("QASJ", 10000);
    while (1) {
        switch (state) {
            case 0:
                printf("please input the player, sum: 2 - 4, (Q:1  A:2  S:3  J:4):\n");
                gets(cmd_string);
                returnCode = cmd_inputHandle(cmd_string);
                if (returnCode == CMD_NOT_FOUND) {
                    returnCode = CMD_NULL;
                    state = 1;
                } else {
                    return returnCode;
                }
                break;
            case 1:
                rc = sscanf(cmd_string, "%1[1-4]%1[1-4]%1[1-4]%1[1-4]", player_num, player_num + 1, player_num + 2,
                            player_num + 3);
                if (rc < 2 || rc > 4 || check_player(player_num) == -1) {
                    state = 2;
                } else {
                    name_init(player_num);
                    state = 3;
                }
                break;
            case 2:
                printf("player input erro, please input(sum: 2 - 4, Q:1  A:2  S:3  J:4):\n");
                gets(cmd_string);
                returnCode = cmd_inputHandle(cmd_string);
                if (returnCode == CMD_NOT_FOUND) {
                    returnCode = CMD_NULL;
                    state = 1;
                } else {
                    return returnCode;
                }
                break;
            case 3:
                printf("please input the money -- a num (1000 - 50000, default:10000):\n$");
                gets(cmd_string);
                returnCode = cmd_inputHandle(cmd_string);
                if (returnCode == CMD_NOT_FOUND) {
                    returnCode = CMD_NULL;
                    state = 4;
                } else {
                    return returnCode;
                }
                break;
            case 4:
                rc = sscanf(cmd_string, "%d", &money);
                if (rc == 1) {
                    if (money > 50000 || money < 1000) {
                        state = 5;
                    } else {
                        money_init(money);
                        return CMD_SUCCESS;
                    }
                } else {
                    return CMD_SUCCESS;
                }

                break;
            case 5:
                printf("money input error, please input a num (1000 - 50000, default:10000):\n$");
                gets(cmd_string);
                returnCode = cmd_inputHandle(cmd_string);
                if (returnCode == CMD_NOT_FOUND) {
                    returnCode = CMD_NULL;
                    state = 4;
                } else {
                    return returnCode;
                }
                break;
            case 6:
                break;
            default:
                break;
        }
    }
    return CMD_SUCCESS;
}

void init_map() {
    for (int i = 0; i < 70; i++) {
        map[i].level = DEFAULT_MAP[i];
        map[i].gift = MAP_GIFT_NONE;
        map[i].owner = MAP_OWNER_NONE;
    }
}

static RETURNMAP returnMap[] =
        {
                {CMD_NOT_FOUND,          "command not found!"},
                {CMD_SUCCESS,            "command success!"},
                {CMD_NULL,               "\0"},
                {CMD_PRAM_ERROR,         "param error!"},
                {CMD_GIFT_NOT_ENOUGH,    "gift not enough!"},
                {CMD_INCORRENT_POSITION, "position incorrect!"},
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
                //{ CMD_PRAM_ERROR ,          "command param error!" },
        };

int show_result(int returnCode) {
    u8 i = 0;
    for (i = 0; i < sizeof(returnMap) / sizeof(returnMap[0]); i++) {
        if (returnCode == returnMap[i].returnCode) {
            printf("%s", returnMap[i].string);
            return 0;
        }
    }

    printf("returnCode:%d not found!", returnCode);
    return -1;
}

int name_init(u8 *username) {
    u8 i = 0;
    for (i = 0; i < 4 && username[i]; i++) {
        switch (username[i]) {
            case '1':
            case 'Q':
            case 'q':
                players[i].label = 'Q';
                break;
            case '2':
            case 'A':
            case 'a':
                players[i].label = 'A';
                break;
            case '3':
            case 'S':
            case 's':
                players[i].label = 'S';
                break;
            case '4':
            case 'J':
            case 'j':
                players[i].label = 'J';
                break;
            default:

                return CMD_NOT_FOUND;
                break;
        }
        players[i].skip = 0;
        players[i].points = 0;
        players[i].position = 0;
        players[i].gifts.barrier = 0;
        players[i].gifts.robot = 0;
        players[i].gifts.bomb = 0;
        players[i].gifts.god = 0;
    }

    player_num = i;
    current_idx = 0;
    return 0;
}

int money_init(s32 money) {
    u8 i = 0;
    for (i = 0; i < player_num; i++) {
        players[i].money = money;
    }

    return CMD_SUCCESS;
}

int user_init(u8 *username, s32 money) {
    name_init(username);
    money_init(money);
    init_map();

    return CMD_SUCCESS;
}
