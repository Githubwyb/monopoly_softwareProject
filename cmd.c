#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "game.h"
#include "events.h"

static u8 dev = 'a' - 'A';

static int str_lower(u8 *string) {
    while (*string) {
        if (*string >= 'A' && *string <= 'Z') {
            *string += dev;
        }
        string++;
    }
    return 0;
}

static int cmd_preset_user(const u8 *cmdString) {
    u8 user_name[5];
    u8 user_num = 0;
    user_num = sscanf(cmdString, "preset user %1[aqsj]%1[aqsj]%1[aqsj]%1[aqsj]", user_name, user_name + 1,
                      user_name + 2, user_name + 3);
    if (user_num >= 1 && user_num <= 4) {
        return user_init(user_name, 10000);
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_map(const u8 *cmdString) {
    int n = 0;
    u8 level[2];
    u8 user_name[2];
    u8 rc = 0;
    rc = sscanf(cmdString, "preset map %d %1[aqsj] %1[0-3]", &n, user_name, level);
    if (rc == 3) {
        user_name[0] -= dev;
        map[n].level = level[0];
        map[n].owner = user_name[0];
        return 0;
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_fund(const u8 *cmdString) {
    u8 user_name[2];
    s32 money = 0;
    u8 rc = 0;
    u8 i = 0;
    rc = sscanf(cmdString, "preset fund %1[aqsj] %d", user_name, &money);
    if (rc == 2) {
        user_name[0] -= dev;
        for (i = 0; i < 4; i++) {
            if (user_name[0] == players[i].label) {
                players[i].money = money;
                return 0;
            }
        }
        return CMD_NOT_FOUND;
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_credit(const u8 *cmdString) {
    u8 user_name[2];
    u32 credit = 0;
    u8 rc = 0;
    u8 i = 0;
    rc = sscanf(cmdString, "preset credit %1[aqsj] %d", user_name, &credit);
    if (rc == 2) {
        user_name[0] -= dev;
        for (i = 0; i < 4; i++) {
            if (user_name[0] == players[i].label) {
                players[i].points = credit;
                return 0;
            }
        }
        return CMD_NOT_FOUND;
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_gift(const u8 *cmdString) {
    u8 gift[8];
    u8 user_name[2];
    int n = 0;
    u8 rc = 0;
    u8 i = 0;
    rc = sscanf(cmdString, "preset gift %1[aqsj] %[a-z] %d", user_name, gift, &n);
    if (rc == 3) {
        user_name[0] -= dev;
        if (strncmp((const char *) gift, "bomb", strlen("bomb")) == 0) {
            for (i = 0; i < 4; i++) {
                if (user_name[0] == players[i].label) {
                    players[i].gifts.bomb = n;
                    return 0;
                }
            }
            return CMD_NOT_FOUND;
        }
        if (strncmp((const char *) gift, "barrier", strlen("barrier")) == 0) {
            for (i = 0; i < 4; i++) {
                if (user_name[0] == players[i].label) {
                    players[i].gifts.barrier = n;
                    return 0;
                }
            }
            return CMD_NOT_FOUND;
        }
        if (strncmp((const char *) gift, "robot", strlen("robot")) == 0) {
            for (i = 0; i < 4; i++) {
                if (user_name[0] == players[i].label) {
                    players[i].gifts.robot = n;
                    return 0;
                }
            }
            return CMD_NOT_FOUND;
        }
        if (strncmp((const char *) gift, "god", strlen("god")) == 0) {
            for (i = 0; i < 4; i++) {
                if (user_name[0] == players[i].label) {
                    players[i].gifts.god = n;
                    return 0;
                }
            }
            return CMD_NOT_FOUND;
        }
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_bomb(const u8 *cmdString) {
    int n = 0;
    u8 rc = 0;
    rc = sscanf(cmdString, "preset bomb %d", &n);
    if (rc == 1) {
        map[n].gift = MAP_GIFT_BOMB;
        return 0;
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_barrier(const u8 *cmdString) {
    int n = 0;
    u8 rc = 0;
    rc = sscanf(cmdString, "preset barrier %d", &n);
    if (rc == 1) {
        map[n].gift = MAP_GIFT_BARRIER;
        return 0;
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_userloc(const u8 *cmdString) {
    u8 user_name[2];
    int n = 0;
    int m = 0;
    u8 rc = 0;
    u8 i = 0;
    rc = sscanf(cmdString, "preset userloc %1[aqsj] %d %d", user_name, &n, &m);
    if (rc == 2 || rc == 3) {
        user_name[0] -= dev;
        for (i = 0; i < 4; i++) {
            if (user_name[0] == players[i].label) {
                players[i].position = n;
                if (m) {
                    players[i].skip = m;
                }
                return CMD_SUCCESS;
            }
        }
        return CMD_NOT_FOUND;
    }

    return CMD_NOT_FOUND;
}

static int cmd_preset_nextuser(const u8 *cmdString) {
    u8 user_name[2];
    u8 rc = 0;
    u8 i = 0;
    u8 j = 5;
    rc = sscanf(cmdString, "preset nextuser %1[aqsj]", user_name);
    if (rc == 1) {
        user_name[0] -= dev;
        for (i = 0; i < player_num; i++) {
            if (user_name[0] == players[i].label) {
                j = i;
                break;
            }
        }

        if (j >= 4) {
            return -1;
        }

        i = current_idx;
        while (1) {
            i++;

            if (i == player_num) {
                i = 0;
            }

            if (i == j) {
                return 0;
            }

            players[i].skip++;
        }

        return -1;
    }

    return -1;
}

static CMD_MAP preset_map[] =
        {
                {"user",     cmd_preset_user},
                {"map",      cmd_preset_map},
                {"fund",     cmd_preset_fund},
                {"credit",   cmd_preset_credit},
                {"gift",     cmd_preset_gift},
                {"bomb",     cmd_preset_bomb},
                {"barrier",  cmd_preset_barrier},
                {"userloc",  cmd_preset_userloc},
                {"nextuser", cmd_preset_nextuser},
        };

static int cmd_preset(const u8 *cmdString) {
    u8 cmd_preset[50];
    u8 rc = 0;
    u8 i = 0;
    rc = sscanf(cmdString, "preset %[a-z]", &cmd_preset);
    if (rc == 1) {
        for (i = 0; preset_map[i].action; i++) {
            if (strncmp((const char *) cmd_preset, (const char *) preset_map[i].cmd,
                        strlen((const char *) cmd_preset)) == 0) {
                return preset_map[i].action(cmdString);
            }
        }
    }

    return -1;
}

static int cmd_roll(const u8 *cmdString) {
    step(roll(), players + current_idx, map);
    return CMD_SUCCESS;
}

static int cmd_sell(const u8 *cmdString) {
    int n = 0;
    u8 rc = 0;
    rc = sscanf(cmdString, "sell %d", &n);
    if (rc == 1) {
        printf("%s:%d\n", __FUNCTION__, n);
        return CMD_SUCCESS;
    }

    return -1;
}

static int cmd_block(const u8 *cmdString) {
    int n = 0;
    u8 rc = 0;
    rc = sscanf(cmdString, "block %d", &n);
    if (rc == 1) {
        if (n > 10 || n < -10) {
            return CMD_PRAM_ERROR;
        }
        int block_position = (players[current_idx].position + n + 70) % 70;
        if (players[current_idx].gifts.barrier == 0) {
            return CMD_GIFT_NOT_ENOUGH;
        }
        if (map[block_position].gift != MAP_GIFT_NONE || map[block_position].level == MAP_LEVEL_HOSPITAL ||
            map[block_position].level == MAP_LEVEL_PRISON || block_position == 0) {
            return CMD_INCORRENT_POSITION;
        }
        int play;
        for (play = 0; play < player_num; play++) {
            if (block_position == players[play].position) {
                return CMD_INCORRENT_POSITION;
            }
        }
        map[block_position].gift = MAP_GIFT_BARRIER;
        players[current_idx].gifts.barrier--;
        return CMD_SUCCESS;
    }

    return CMD_NOT_FOUND;
}

static int cmd_bomb(const u8 *cmdString) {
    int n = 0;
    u8 rc = 0;
    int play;
    rc = sscanf(cmdString, "bomb %d", &n);
    // printf("%s:%d\n", __FUNCTION__, n);
    if (rc == 1) {

        if (n > 10 || n < -10) {
            return CMD_PRAM_ERROR;
        } else {
            int block_position = (players[current_idx].position + n + 70) % 70;//判断炸弹的位置
            if (players[current_idx].gifts.bomb == 0) {
                return CMD_GIFT_NOT_ENOUGH;//判断炸弹的数量是否足够
            }
            if (map[block_position].gift != MAP_GIFT_NONE || map[block_position].level == MAP_LEVEL_HOSPITAL
                || map[block_position].level == MAP_LEVEL_PRISON || block_position == 0) {
                return CMD_INCORRENT_POSITION;

            }


            for (play = 0; play < player_num; play++) {
                if (block_position == players[play].position) {
                    return CMD_INCORRENT_POSITION;
                }
            }
            map[block_position].gift = MAP_GIFT_BOMB;
            players[current_idx].gifts.bomb--;
            return CMD_SUCCESS;
        }
        return CMD_SUCCESS;
    }

    return CMD_NOT_FOUND;
}

static int cmd_robot(const u8 *cmdString) {
    //  printf("%s\n", __FUNCTION__);
    int temp_position;
    int i;
    for (i = 1; i <= 10; i++) {
        temp_position = (players[current_idx].position + i + 70) % 70;
        if (map[temp_position].gift == MAP_GIFT_BARRIER || map[temp_position].gift == MAP_GIFT_BOMB)
            map[temp_position].gift = MAP_GIFT_NONE;
    }
    return CMD_SUCCESS;
}


static int cmd_help(const u8 *cmdString) {
    printf("%s\n", __FUNCTION__);
    return CMD_SUCCESS;
}

static int cmd_quit(const u8 *cmdString) {
    exit(0);
    return CMD_SUCCESS;
}

static int cmd_step(const u8 *cmdString) {
    int n = 0;
    u8 rc = 0;
    rc = sscanf(cmdString, "step %d", &n);
    if (rc == 1) {
        step(n, players + current_idx, map);
        return CMD_SUCCESS;
    }

    return CMD_NOT_FOUND;
}

static int cmd_dump(const u8 *cmdString) {
    FILE *pFile = NULL;
    int i, j, k;
    char *giftname[4] = {"barrier", "bomb", "robot", "god"};
    pFile = fopen("dump.txt", "w");
    if (pFile) {
        fprintf(pFile, "user ");
        for (i = 0; i < player_num; i++) {
            if (players[i].money >= 0) {
                fprintf(pFile, "%c", players[i].label);
            }
        }
        fprintf(pFile, "\n");
        for (i = 0; i < 70; i++) {
            if (map[i].owner != MAP_OWNER_NONE) {
                fprintf(pFile, "map[%d] %c %c\n", i, map[i].owner, map[i].level);
            }
        }

        for (i = 0; i < player_num; i++) {
            if (players[i].money >= 0) {
                fprintf(pFile, "fund %c %d\n", players[i].label, players[i].money);
            }
        }

        for (i = 0; i < player_num; i++) {
            if (players[i].money >= 0) {
                if (players[i].points > 0)
                    fprintf(pFile, "credit %c %d\n", players[i].label, players[i].points);
            }
        }

        for (i = 0; i < player_num; i++) {
            char *arr = (char *)&(players[i].gifts);
            for (j = 0; j < 4; j++) {
                if (players[i].money >= 0) {
                    if (arr[j] > 0) {
                        fprintf(pFile, "gift %c %s %d\n", players[i].label, giftname[j], arr[j]);
                    }
                }
            }
        }

        for (i = 0; i < 70; i++) {
            if (map[i].gift != MAP_GIFT_NONE) {
                if (map[i].gift == '#') {
                    fprintf(pFile, "barrier %d\n", i);
                } else {
                    fprintf(pFile, "bomb %d\n", i);
                }
            }
        }

        for (i = 0; i < player_num; i++) {
            if (players[i].money >= 0) {
                fprintf(pFile, "userloc %c %d\n", players[i].label, players[i].position);
            }
        }
        k = 1;
        while (players[(current_idx + k) % player_num].money < 0) {
            if (k == player_num)
                break;
            k++;
        }
        fprintf(pFile, "nextuser %c\n", players[(current_idx + k) % player_num].label);

    } else {
        printf("dump.txt create failed!\n");
        system("pause");
    }
    fclose(pFile);
    return 0;
}

static CMD_MAP cmd_map[] = {
        {"preset", cmd_preset},
        {"roll",   cmd_roll},
        {"sell",   cmd_sell},
        {"block",  cmd_block},
        {"bomb",   cmd_bomb},
        {"robot",  cmd_robot},
        {"help",   cmd_help},
        {"quit",   cmd_quit},
        {"step",   cmd_step},
        {"dump",   cmd_dump},
};

int cmd_inputHandle(u8 *cmd) {
    u8 i = 0;
    for (i = 0; i < 50; i++) {
        if (!cmd[i]) {
            cmd[i] = '\0';
            if (i < 4) {
                return CMD_NOT_FOUND;
            }
            break;
        }
    }

    str_lower(cmd);
    for (i = 0; cmd_map[i].action; i++) {
        if (strncmp((const char *) cmd, (const char *) cmd_map[i].cmd, strlen((const char *) cmd_map[i].cmd)) == 0) {
            return cmd_map[i].action(cmd);
        }
    }

    return CMD_NOT_FOUND;
}
