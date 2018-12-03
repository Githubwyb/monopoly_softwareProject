#ifndef _EVENT_H_
#define _EVENT_H_

#include "game.h"

int roll();

int step(int roll_num, Player *player, Map map[70]);

int hospital_judge(Player *player, Map map[70]);

void land_judge(Player *player, Map map[70]);

void land_buy(Player *player, Map map[70]);

int land_upgrade(Player *player, Map map[70]);

void land_paid(Player *player, Map map[70]);

void points_get(Player *player, Map map[70]);

void gift_shop();

void present_shop();

void magic_shop();

void player_bankrupt();

#endif
