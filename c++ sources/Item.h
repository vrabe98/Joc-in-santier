#ifndef ITEM_H
#define ITEM_H

#define CHEST 0				//Armor slots
#define HEAD 1
#define LEGS 2
#define ARMS 3
#define FEET 4
#define LHAND 6				//Weapon/shield slots
#define RHAND 5
#define NONEQUIPABLE -1		//Items that can't be equipped

#include "Quest.h"

class Container;
class Character;

class Item
{
protected:
	Quest_flag collection_flag;		//flag is set when moving the item to the inventory, unset when moving it out
	std::string name;
	int equip_slot,weight,id;
	float price;
public:
	virtual double BlockMultiplier()=0;		//also acts as shield type identifier
	virtual int Is2h()=0;
	virtual void Load(std::ifstream&)=0;
	virtual void Show_info(int) = 0;
	virtual int IsArmor()=0;
	virtual int IsWeapon()=0;
	virtual int IsGeneric() = 0;
	Quest_flag Get_collection_flag() {
		return collection_flag;
	}
	inline int GetSlot() {
		return equip_slot;
	}
	inline float GetPrice() {
		return price;
	}
	virtual int GetArmor()=0;
	virtual float GetDamage() = 0;
};

class Weapon:public Item
{
	float damage;
	int twohanded;
public:
	inline double BlockMultiplier() {
		return 0.4;
	}
	void Load(std::ifstream&);
	void Show_info(int);
	inline int Is2h() {
		return twohanded;
	}
	inline int IsWeapon() {
		return 1;
	}
	inline int IsArmor() {
		return 0;
	}
	inline int IsGeneric() {
		return 0;
	}
	inline int GetArmor() {
		return 0;
	}
	inline float GetDamage() {
		return damage;
	}
};

class Armor :public Item {
	int armor;
public:
	inline double BlockMultiplier() {
		return 1;
	}
	void Load(std::ifstream&);
	void Show_info(int);
	inline int Is2h() {
		return NULL;
	}
	inline int IsWeapon() {
		return 0;
	}
	inline int IsArmor() {
		return 1;
	}
	inline int IsGeneric() {
		return 0;
	}
	inline int GetArmor() {
		return armor;
	}
	inline float GetDamage() {
		return 0;
	}
};

class Generic_item :public Item {
public:
	inline double BlockMultiplier() {
		return 1;
	}
	void Load(std::ifstream&);
	void Show_info(int);
	inline int Is2h() {
		return NULL;
	}
	inline int IsWeapon() {
		return 0;
	}
	inline int IsArmor() {
		return 0;
	}
	inline int IsGeneric() {
		return 1;
	}
	inline int GetArmor() {
		return 0;
	}
	inline float GetDamage() {
		return 0;
	}
};

class Shield :public Item {
public:
	inline double BlockMultiplier() {
		return 0.1;
	}
	void Load(std::ifstream&);
	void Show_info(int);
	inline int Is2h() {
		return NULL;
	}
	inline int IsWeapon() {
		return 0;
	}
	inline int IsArmor() {
		return 1;
	}
	inline int IsGeneric() {
		return 0;
	}
	inline int GetArmor() {
		return 0;
	}
	inline float GetDamage() {
		return 0;
	}
};
#endif