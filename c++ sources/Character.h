#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_STORAGE 100
#define MAX_QUESTS 100

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
#include "Quest.h"
#include "Music.h"

class Map;

typedef struct Damage {
	float damage;
	std::string type;
} Damage;

class Character
{
	friend class Game;
protected:
	int inventory_size, strength, dexterity, constitution, charisma, armor, damage_bonus,level;	//for npcs, inventory_size is auxiliary in the loading process
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
	int Dualwield();
	int died();
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
	inline int CHA() {
		return charisma;
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
	inline int GetInvSize() {
		return inventory_size;
	}
	Damage GetWeaponDmg(int);
	float GetEvasion();
	int Has1h();
	void AddToInventory(Item*);
	Item* RemoveFromInventory(int);
	void ShowStats();
	void RefreshArmor();
	void RefreshHP();
	int HasEquippedItems();
	void Equip(int);
	void Unequip(int);
	void Interact_container(COORD);
	virtual void Draw(Map*,int)=0;
	void Show_inventory(int,int);
	void Query_inventory(Object*);
	friend int Check_terrain(Character*, COORD);
	Character();
};

class Main_character :public Character {
	int map_change_attempt, num_quests;
	float currency;
	Quest* qlist[MAX_QUESTS];		//quest list;
public:
	void Character_creation();
	void Set_map_change_attempt() {
		map_change_attempt = 1;
	}
	void Load_startquest(std::ifstream&);
	void RefreshQuests();
	void Quest_screen();
	void Move();
	void Interact_NPC(COORD);
	void Change_map(Map*, COORD);
	void Draw(Map* mapa=nullptr, int style=NULL);
	inline void Pay(float price) {
		currency -= price;
	}
	inline float GetCash() {
		return currency;
	}
	void Add_quest(Quest* q) {
		qlist[num_quests] = q;
		num_quests++;
	}
	Main_character(int, int, Map*, int, float,Item**, int);
};

class NPC :public Character
{
	friend class Game;
	DialogueState* root;
	Quest_flag death_flag;	//flag is set by killing the NPC
public:
	NPC();
	virtual void Interact(Main_character*);
	virtual void Load(std::ifstream&,Map[],Item**);
	void Draw(Map*, int);
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

class Vendor : public NPC
{
	friend class Game;
public:
	void Load(std::ifstream&, Map[], Item**);
	void BuySell(Main_character*);
	void Display_inventories(Character*);
	void Interact(Main_character*);
	void Draw(Map*, int);
};
#endif

