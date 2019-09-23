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

#define FIST_DMG 50

#include "Item.h"
#include "DialogueState.h"
#include "Game.h"
#include "Combat.h"

class Map;

typedef struct Damage {
	int damage;
	std::string type;
} Damage;

class Character
{
	friend class Game;
protected:
	int map_change_attempt,inventory_size,strength,dexterity,constitution,armor,damage_bonus;	//for npcs, inventory_size is auxiliary in the loading process
	float hp;
	std::string name;
	COORD coordonate;
	Item* inventory[MAX_STORAGE];
	Item* equipped_items[NUM_SLOTS];
	Map* current_map;
public:
	std::string GetName() {
		return name;
	}
	int Dualwield() {
		if (equipped_items[LHAND] == nullptr) return 0;
		else if (equipped_items[LHAND]->IsWeapon() && (!equipped_items[LHAND]->Is2h())) return 1;
		else return 0;
	}
	inline int died() {
		if (hp <= 0) {
			hp = 0;
			return 1;
		}
		else return 0;
	}
	inline float GetHP() {
		return hp;
	}
	void GetDamaged(float,Character*,int,int);
	inline int STR() {
		return strength;
	}
	inline int DEX() {
		return dexterity;
	}
	inline int CON() {
		return constitution;
	}
	inline void SetDmgBonus() {
		damage_bonus = 1;
	}
	inline void ResetDmgBonus() {
		damage_bonus = 0;
	}
	inline int HasDmgBonus() {
		return damage_bonus;
	}
	Damage GetWeaponDmg(int);
	float GetEvasion();
	int Has1h();
	inline int IsPlayer() {
		return 1;
	}
	void ShowStats();
	void RefreshArmor();
	void RefreshHP();
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
	inline int IsPlayer() {
		return 0;
	}
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

