#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<string.h>

void draw(int i);

char *userLabelToName(char label);

void drawMsg(char name, int money, int points, int position);

void drawMap() {
    char *name;
    int i;
    int empty;
    int play;
    HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hHandle, 15);
    printf("\n                        大富翁游戏\n\n");

    for (play = 0; play < 4; play++) {
        if (play >= player_num) {
            putchar('\n');
        } else {
            drawMsg(players[play].label, players[play].money, players[play].points, players[play].position);
        }
    }
    putchar('\n');

    for (i = 0; i < 29; i++) {
        if (i == 0) {
            printf("               ");
        }
        draw(i);
        if (i == 28) {
            putchar('\n');
        }
    }
    printf("               ");
    draw(69);

    for (empty = 0; empty < 27; empty++) {
        putchar(' ');
    }
    draw(29);
    putchar('\n');
    printf("               ");
    draw(68);
    for (empty = 0; empty < 27; empty++) {
        putchar(' ');
    }
    draw(30);
    putchar('\n');
    printf("               ");
    draw(67);
    for (empty = 0; empty < 27; empty++) {
        putchar(' ');
    }
    draw(31);
    putchar('\n');
    printf("               ");
    draw(66);
    for (empty = 0; empty < 27; empty++) {
        putchar(' ');
    }
    draw(32);
    putchar('\n');
    printf("               ");
    draw(65);
    for (empty = 0; empty < 27; empty++) {
        putchar(' ');
    }
    draw(33);
    putchar('\n');
    printf("               ");
    draw(64);
    for (empty = 0; empty < 27; empty++) {
        putchar(' ');
    }
    draw(34);
    putchar('\n');

    for (i = 63; i > 34; i--) {
        if (i == 63) {
            printf("               ");
        }
        draw(i);
        if (i == 35) {
            putchar('\n');
        }
    }

    putchar('\n');
    putchar('\n');
    name = userLabelToName(players[current_idx].label);
    printf("               %s>", name);
}

char *userLabelToName(char label) {
    switch (label) {
        case 'A':
            return "阿土伯";
        case 'S':
            return "孙小美";
        case 'Q':
            return "钱夫人";
        case 'J':
            return "金贝贝";
    }
}

void drawMsg(char name, int money, int points, int position) {
    printf("               %s---资产:%-8d\t点数:%-4d 位置:%d\n", userLabelToName(name), money, points, position);
}

void draw(int i) {
    int play;
    HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hHandle, 15);
    for (play = 0; play < player_num; play++) {
        if (players[play].position != i) {
            continue;
        }
        switch (players[play].label) {
            case MAP_OWNER_Q:
                //红
                SetConsoleTextAttribute(hHandle, 4);
                break;
            case MAP_OWNER_A:
                //绿
                SetConsoleTextAttribute(hHandle, 2);
                break;
            case MAP_OWNER_S:
                //蓝
                SetConsoleTextAttribute(hHandle, 9);
                break;
            case MAP_OWNER_J:
                //黄
                SetConsoleTextAttribute(hHandle, 6);
                break;
        }
        putchar(players[play].label);
        return;
    }
    if (map[i].gift != MAP_GIFT_NONE) {
        putchar(map[i].gift);
        return;
    }
    switch (map[i].owner) {
        case MAP_OWNER_Q:
            //红
            SetConsoleTextAttribute(hHandle, 4);
            break;
        case MAP_OWNER_A:
            //绿
            SetConsoleTextAttribute(hHandle, 2);
            break;
        case MAP_OWNER_S:
            //蓝
            SetConsoleTextAttribute(hHandle, 9);
            break;
        case MAP_OWNER_J:
            //黄
            SetConsoleTextAttribute(hHandle, 6);
            break;
    }
    putchar(map[i].level);
}
