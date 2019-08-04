
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Connection.h"

void Connection::Load(std::ifstream& f,Map map_arr[]){
	std::string terminator;
	int map;
	f.ignore();
	f >> two_way;
	f >> _entry.coordonate.Y >> _entry.coordonate.X >> map;
	_entry.mapa = map_arr[map];
	f >> _exit.coordonate.Y >> _exit.coordonate.X;
	f >> map;
	_exit.mapa = map_arr[map];
	f >> terminator;											//check if terminator string is correct
	if (terminator != ";;") {
		printf("Connection file corrupted!");
		std::cin.get();
		exit(1);
	}
	valid = 1;
}