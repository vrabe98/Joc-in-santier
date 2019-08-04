#ifndef OBJECT_H
#define OBJECT_H

#define MAX_STORAGE 100

#include "Item.h"

class Object
{
protected:
	int map_ID, obj_ID;
	std::string name;
	COORD coordonate;
public:
	virtual void Load(std::ifstream&,Item**)=0;
	virtual void Interact() = 0;
	virtual int IsContainer() = 0;
	virtual int IsGeneric() = 0;
	virtual void Draw()=0;
	inline COORD GetCoord() {
		return coordonate;
	}
	inline void Set_ID(int id) {
		obj_ID = id;
	}
	inline int Get_map() {
		return map_ID;
	}
};

class Generic_object :public Object {
public:
	void Draw();
	void Load(std::ifstream&,Item**);
	inline void Interact() {
		printf("Interaction not possible");
	}
	inline int IsContainer() {
		return 0;
	}
	inline int IsGeneric() {
		return 1;
	}
};

class Container:public Object {
	int inventory_size;
	Item* inventory[MAX_STORAGE];
public:
	void Load(std::ifstream&,Item**);
	void Interact();
	void Draw();
	inline int Get_inventory_size() {
		return inventory_size;
	}
	inline int IsContainer() {
		return 1;
	}
	inline int IsGeneric() {
		return 0;
	}
	void Load_inventory(std::ifstream&,Item**);
};
#endif