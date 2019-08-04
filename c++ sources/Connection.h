#ifndef CONNECTION_H
#define CONNECTION_H

#include "Map.h"

typedef struct Location {
	COORD coordonate;
	Map mapa;
}Location;

class Connection
{
	friend class Game;
	int valid;
	int two_way;			//==0 ->the connection is one-way(entry-exit),>0 ->the connection is two-way(entry-exit;exit-entry);
	Location _entry;
	Location _exit;
public:
	void Load(std::ifstream&,Map[]);
	inline int IsTwoWay() {
		return two_way;
	}
};

#endif