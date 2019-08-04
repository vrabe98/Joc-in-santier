#ifndef ITEM_H
#define ITEM_H

#define BODY 0
#define HAND 1
#define NONEQUIPABLE -1

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

};

class Weapon:public Item
{
	int damage;
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