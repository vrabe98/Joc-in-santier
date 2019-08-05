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
	virtual Item* Interact() = 0;
	virtual void Transfer_to(Item*)=0;
	virtual int IsContainer() = 0;
	virtual int IsGeneric() = 0;
	virtual void Show_inventory() = 0;
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
	inline void Transfer_to(Item* item) {
		printf("The item is not a container!");
	}
	inline void Show_inventory() {
		printf("The object is not a container!");
	}
	inline Item* Interact() {
		printf("Interaction not possible");
		return nullptr;
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
	Item* Interact();
	void Draw();
	void Transfer_to(Item*);
	void Show_inventory();
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