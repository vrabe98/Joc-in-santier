#ifndef GAME_H
#define GAME_H

#define MAX_MAP 100
#define MAX_CONNECTIONS 200
#define MAX_CHARACTERS 100
#define MAX_ITEMS 100

#include "Map.h"
#include "Connection.h"
#include "Character.h"
#include "Item.h"

class Character;
class NPC;
class Item;
class Armor;
class Weapon;
class Generic_item;

class Game
{
	friend class Character;
	int num_maps, num_conn,num_chars,num_items;
	Map maps[MAX_MAP];
	Connection con[MAX_CONNECTIONS];
	NPC *npcs[MAX_CHARACTERS];
	Character* main_character;
	Item* item_db[MAX_STORAGE];
public:
	void Enter_connection();
	Connection Search(COORD ,int);
	int Check_NPC(int, std::string);
	NPC* GetNPC(COORD, int);
	void Load_maps(std::ifstream&);
	void Load_MainCharacter(std::ifstream&);
	void Load_connections(std::ifstream&);
	void Load_objects(std::ifstream&);
	void Load_npcs(std::ifstream&);
	void Load_item_db(std::ifstream&);
	void Load_dialogues(std::ifstream&);
	void Load(std::string,std::string,std::string,std::string,std::string,std::string,std::string);
	void Play();
};

#endif;