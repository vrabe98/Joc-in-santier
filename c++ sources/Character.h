#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_STORAGE 100
#define NUM_SLOTS 2
#define BODY 0
#define HAND 1
#define NONEQUIPABLE -1

#include "Item.h"
#include "Game.h"

class Map;

class Character
{
	friend class Game;
protected:
	int map_change_attempt,inventory_size;
	COORD coordonate;
	Item* inventory[MAX_STORAGE];
	Item* equipped_items[NUM_SLOTS-1];
	Map* current_map;
public:
	void Change_map(Map*,COORD);
	void Draw();
	void Move();
	void Show_inventory();
	void Query_inventory(Object*);
	friend int Check_terrain(Character, COORD);
	Character();
	Character(int, int,Map*,int);
};


class NPC :protected Character 
{
	friend class Game;
	std::string name;
public:
	NPC();
	void Draw(Map*,int);
	void Load(std::ifstream&,Map[]);
};
#endif

