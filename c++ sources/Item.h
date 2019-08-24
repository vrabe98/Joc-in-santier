#ifndef ITEM_H
#define ITEM_H

#define CHEST 0				//Armor slots
#define HEAD 1
#define LEGS 2
#define ARMS 3
#define FEET 4
#define LHAND 5				//Weapon/shield slots
#define RHAND 6
#define NONEQUIPABLE -1		//Items that can't be equipped

class Container;
class Character;

class Item
{
protected:
	std::string name;
	int equip_slot,weight,id;
public:
	virtual void Load(std::ifstream&)=0;
	virtual void Show_info() = 0;
	friend void Equip(Character*);
	virtual int IsArmor()=0;
	virtual int IsWeapon()=0;
	virtual int IsGeneric() = 0;
	inline int GetSlot() {
		return equip_slot;
	}
};

class Weapon:public Item
{
	int damage,twohanded;
public:
	void Load(std::ifstream&);
	void Show_info();
	inline int IsWeapon() {
		return 1;
	}
	inline int IsArmor() {
		return 0;
	}
	inline int IsGeneric() {
		return 0;
	}

};

class Armor :public Item {
	int armor;
public:
	void Load(std::ifstream&);
	void Show_info();
	inline int IsWeapon() {
		return 0;
	}
	inline int IsArmor() {
		return 1;
	}
	inline int IsGeneric() {
		return 0;
	}
};

class Generic_item :public Item {
public:
	void Load(std::ifstream&);
	void Show_info();
	inline int IsWeapon() {
		return 0;
	}
	inline int IsArmor() {
		return 0;
	}
	inline int IsGeneric() {
		return 1;
	}
};
#endif