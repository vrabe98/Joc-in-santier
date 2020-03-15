#ifndef GAME_H
#define GAME_H

#define MAX_MAP 100
#define MAX_CONNECTIONS 200
#define MAX_CHARACTERS 100		//vendors are counted towards character limit
#define MAX_ITEMS 100
#define MAX_QUESTS 100			//maximum number of quests in the entire game

#include <map>
#include "Map.h"
#include "Connection.h"
#include "Character.h"
#include "Item.h"
#include "Quest.h"
#include "Music.h"

class Character;
class Main_character;
class NPC;
class Item;
class Armor;
class Weapon;
class Generic_item;

class Game
{
	friend class Character;
	friend class Main_character;
	int num_maps, num_conn, num_chars, num_items, num_vendors, num_quests;
	Map maps[MAX_MAP];
	Connection con[MAX_CONNECTIONS];
	NPC *npcs[MAX_CHARACTERS];
	Main_character* main_character;
	Item* item_db[MAX_STORAGE];
	Quest* quest_db[MAX_QUESTS];
	std::map<std::string,Quest_flag> unclaimed_flags;
public:
	inline std::map<std::string,Quest_flag>* Get_unclaimed_flags_map(){
		return &unclaimed_flags;
	}
	inline Quest* Get_quest_by_ID(int id) {
		return quest_db[id];
	}
	Main_character* Get_main_char(){
		return main_character;
	}
	void Set_flag(Quest_flag);
	void Splash(std::string);
	void Menu(std::string, std::string, std::string, std::string, std::string, std::string, std::string,std::string,std::string);
	void Enter_connection();
	Connection Search(COORD ,int);
	int Check_NPC(int, std::string);
	NPC* GetNPC(COORD, int);
	void Load_quests(std::ifstream&);
	void Load_maps(std::ifstream&);
	void Load_MainCharacter(std::ifstream&);
	void Load_connections(std::ifstream&);
	void Load_objects(std::ifstream&);
	void Load_npcs(std::ifstream&);
	void Load_item_db(std::ifstream&);
	void Load_dialogues(std::ifstream&);
	void Load_vendors(std::ifstream&);
	void Load(std::string,std::string,std::string,std::string,std::string,std::string,std::string,std::string,std::string);
	void Play();
};

#endif;