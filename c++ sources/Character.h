#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_STORAGE 100
#define NUM_SLOTS 7
#define CHEST 0				//Armor slots
#define HEAD 1
#define LEGS 2
#define ARMS 3
#define FEET 4
#define LHAND 6				//Weapon/shield slots
#define RHAND 5
#define NONEQUIPABLE -1		//Items that can't be equipped

#include "Item.h"
#include "DialogueState.h"
#include "Game.h"

class Map;

class Character
{
	friend class Game;
protected:
	int map_change_attempt,inventory_size,strength,dexterity,constitution;	//for npcs, inventory_size is auxiliary in the loading process
	std::string name;
	COORD coordonate;
	Item* inventory[MAX_STORAGE];
	Item* equipped_items[NUM_SLOTS];
	Map* current_map;
public:
	inline int STR() {
		return strength;
	}
	inline int DEX() {
		return dexterity;
	}
	inline int CON() {
		return constitution;
	}
	void ShowStats();
	int HasEquippedItems();
	void Equip(int);
	void Unequip(int);
	void Change_map(Map*,COORD);
	void Interact_container(COORD);
	void Interact_NPC(COORD);
	void Draw();
	void Move();
	void Show_inventory(int);
	void Query_inventory(Object*);
	friend int Check_terrain(Character, COORD);
	Character();
	Character(int, int,Map*,int,int,int,int,std::string,Item**,int);
};


class NPC :public Character 
{
	friend class Game;
	DialogueState* root;
public:
	NPC();
	void Draw(Map*,int);
	void Load(std::ifstream&,Map[],Item**);
	void Dialogue();
	std::string GetName();
	int CheckNPC(COORD, int);
	int CheckName(std::string aux) {
		aux = aux;
		return (name == aux);
	}
	inline void Bind_dialogue_root(DialogueState* root_state) {
		root = root_state;
	}
};
#endif

