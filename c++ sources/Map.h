#ifndef MAP_H
#define MAP_H

#define MAX_OBJ 100

#include "Object.h"

class Map
{
	friend class Game;
	char* map_arr;
	int map_id, dim_x, dim_y, num_obj;
	std::string name;
	Object *objects[MAX_OBJ];
public:
	void Draw();
	void Load(std::ifstream&);
	void Bind_object(Object*);
	inline int Get_ID() {
		return map_id;
	}
	Object* Get_obj(COORD);
	char GetLoc(COORD);
	inline int Dimx() {
		return dim_x;
	}
	inline int Dimy() {
		return dim_y;
	}
};

#endif

