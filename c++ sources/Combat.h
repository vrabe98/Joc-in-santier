#ifndef COMBAT_H
#define COMBAT_H

class Character;
class NPC;

#define ROLL_MIN 0
#define ROLL_MAX 20

int Roll_d20();
void Round(Character*, Character*,int);
int Combat(Character*,NPC*);
void DeathScreen(Character*,Character*);

#endif