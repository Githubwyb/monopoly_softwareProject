#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"game.h"
#include "events.h"

extern int state;

int roll(void) //产生一个1-6的随机数
{
    return ((rand() % 6) + 1); //返回一个1-6的随机数
};

int step(int roll_num, Player *player, Map map[70]) //输入行走步数 //////////
{
    int steps, i;
    for (steps = 1; steps <= roll_num; steps++) {
        i = (player->position + steps < 69) ? steps : steps - 70;
        if (map[player->position + i].gift != '#') //不经炸弹与路障时
            continue;
        else {
            if (map[player->position + i].gift == '#')
                map[player->position + i].gift = '0';
            break;
        }
    }

    if (i > roll_num) {
        i = roll_num;
    }
    player->position += i;
    state = 1;
    return 0;
}

int hospital_judge(Player *player, Map map[70]) //判断是否需要进医院 //////////
{
    if (map[player->position].gift == '@') {
        map[player->position].gift = '0';
        player->position = 14;//玩家传送到医院
        player->skip = 3;
        return 1;
    } else
        return 0;
}

void land_judge(Player *player, Map map[70]) {
    if ((player->position != 0) && (player->position != 14) && (player->position != 28) && (player->position != 35) &&
        (player->position != 49) && (player->position != 63) && (player->position >= 0) &&
        (player->position <= 63))//Íæ¼Ò´¦ÔÚÍÁµØ
    {
        if (map[player->position].owner == '0') //此地无主
        {
            land_buy(player, map);
        } else if (map[player->position].owner == player->label) {
            land_upgrade(player, map);
        } else if (map[player->position].owner != player->label) {
            land_paid(player, map);
        }
    } else if (player->position == 28) {
        printf("Welcome to 道具屋\n");//
    } else if (player->position == 35) {
        printf("Welcome to 礼品屋\n");//
    } else if (player->position == 49) {
        printf("Welcome to 监狱\n");//
        player->skip = 2;
    } else if (player->position == 63) {
        printf("Welcome to 霍格沃茨！/破釜酒吧\n");//
    } else if ((player->position > 63) && (player->position <= 69)) {
        points_get(player, map);
    }
    state = 2;
}

void land_buy(Player *player, Map map[70]) {
    printf("Do you wanna buy this land?Please input Y or N.\n");
    while (1) {
        gets(cmd_string);

        if (cmd_string[0] == 'Y' || cmd_string[0] == 'y') {
            if ((player->position > 0) && (player->position < 28) && (player->position != 14))
                player->money -= 200;
            if ((player->position > 28) && (player->position < 35))
                player->money -= 500;
            if ((player->position > 35) && (player->position < 63) && (player->position != 49))
                player->money -= 300;
            map[player->position].owner = player->label;
            printf("You have bought it!\n");
            break;
        } else if (cmd_string[0] == 'N' || cmd_string[0] == 'n') {
            printf("You didn't buy it!\n");
            break;
        } else {
            printf("Please input Y or N again!\n");
        }
    }
}

int land_upgrade(Player *player, Map map[70]) {
    if (map[player->position].level == MAP_LEVEL_3) {
        return CMD_SUCCESS;
    }
    printf("Do you wanna upgrade your land?Please input Y or N.\n");
    while (1) {
        gets(cmd_string);

        if (cmd_string[0] == 'Y' || cmd_string[0] == 'y') {
            map[player->position].level++;
            if ((player->position > 0) && (player->position < 28) && (player->position != 14))
                player->money -= 200;
            if ((player->position > 28) && (player->position < 35))
                player->money -= 500;
            if ((player->position > 35) && (player->position < 63) && (player->position != 49))
                player->money -= 300;
            printf("You have upgrade it!\n");
            break;
        } else if (cmd_string[0] == 'N' || cmd_string[0] == 'n') {
            printf("You didn't upgrate it!\n");
            break;
        } else {
            printf("Please input Y or N again!\n");
        }
    }
    return CMD_SUCCESS;
}

void land_paid(Player *player, Map map[70]) {
    int tax, i;
    if ((player->position > 0) && (player->position < 28) && (player->position != 14))
        tax = (map[player->position].level - '0' + 1) * 200 / 2;
    if ((player->position > 28) && (player->position < 35))
        tax = (map[player->position].level - '0' + 1) * 500 / 2;
    if ((player->position > 35) && (player->position < 63) && (player->position != 49))
        tax = (map[player->position].level - '0' + 1) * 300 / 2;
    player->money -= tax;
    for (i = 0; i < player_num; i++) //加钱//
    {
        if (map[player->position].owner == player[i - current_idx].label) {
            player[i - current_idx].money += tax;
            break;
        }
    }
    printf("There is %c's land, you paid for $%d.", player->label, tax);
}

void points_get(Player *player, Map map[70]) {
    int points;
    switch (player->position) {
        case 64: {
            points = 20;
            break;
        }
        case 65: {
            points = 80;
            break;
        }
        case 66: {
            points = 100;
            break;
        }
        case 67: {
            points = 40;
            break;
        }
        case 68: {
            points = 80;
            break;
        }
        case 69: {
            points = 60;
            break;
        }
    }
    player->points += points;
}

void gift_shop() {
    int ch;
    Player *player = &players[current_idx];

    if (player->gifts.barrier + player->gifts.bomb
        + players->gifts.robot + players->gifts.god >= 10) {
        printf("你已拥有10个道具，不能再买了哦\n");
    } else {
        do {
            printf("有3种道具可以选择，输入标号购买，按F离开\n");
            printf("(1)路障 50点 (2)炸弹 50点 (3)机器娃娃 30点\n");
            ch = getchar();
            fflush(stdin);
            switch (ch) {
                case '1': // 购买路障
                    if (player->points < 50) {
                        printf("点数不足\n");
                    } else {
                        player->points -= 50;
                        ++player->gifts.barrier;
                        printf("购买成功\n");
                    }
                    break;
                case '2':
                    if (player->points < 50) {
                        printf("点数不足\n");
                    } else {
                        player->points -= 50;
                        ++player->gifts.bomb;
                        printf("购买成功\n");
                    }
                    break;
                case '3':
                    if (player->points < 30) {
                        printf("点数不足\n");
                    } else {
                        player->points -= 30;
                        ++player->gifts.robot;
                        printf("购买成功\n");
                    }
                    break;
                case 'F':
                case 'f':
                    return;
            }

        } while (1);
    }
}

void present_shop() {
    int ch;
    Player *player = &players[current_idx];

    while (1) {
        printf("有3种礼品可以选择，输入标号获取，按F离开\n");
        printf("(1)奖金 2000元 (2)点数卡 200点 (3)财神 5轮免租\n");
        ch = getchar();
        fflush(stdin);
        switch (ch) {
            case '1':
                player->money += 2000;
                return;
            case '2':
                player->points += 200;
                return;
            case '3':
                player->gifts.god += 5;
                return;
            case 'F':
            case 'f':
                return;

        }
    }
}

void player_bankrupt() {
    Player *player = &players[current_idx];

    player->bankrupt = 1;
    char label = player->label;
    for (int j = 0; j < 70; ++j) {
        if (map[j].owner == label) {
            memset(&map[j], 0, sizeof(Map));
        }
    }
}