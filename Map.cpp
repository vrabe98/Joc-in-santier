#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Map.h"

int Compare_coord2(COORD coord1, COORD coord2) {
	if ((coord1.X == coord2.X) && (coord1.Y == coord2.Y)) return 1;
	else return 0;
}

void Map::Draw() {												//draws a 2D map in the console
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	PSMALL_RECT rectangle = new SMALL_RECT;
	CHAR_INFO* buffer = new CHAR_INFO[dim_y*dim_x];
	COORD buffer_size, origin;
	buffer_size.Y = dim_y;
	buffer_size.X = dim_x;
	origin.X = origin.Y = 0;
	rectangle->Top = rectangle->Left = 0;
	rectangle->Bottom = dim_y - 1;
	rectangle->Right = dim_x - 1;
	for (int y = 0; y < dim_y; y++) {
		for (int x = 0; x < dim_x; x++) {
			buffer[y * dim_x + x].Char.AsciiChar = *(map_arr + dim_x * y+x);
			buffer[y * dim_x + x].Attributes = 7;
		}
	}
	WriteConsoleOutput(consola, buffer, buffer_size, origin, rectangle);
	buffer_size.X = 0;
	SetConsoleCursorPosition(consola, buffer_size);
	std::cout << name << "\n";
	for (int i = 0; i < num_obj; i++) {
		objects[i]->Draw();
	}
}

void Map::Load(std::ifstream& f) {								//Load a map from a file
	std::string terminator;
	f >> map_id;
	f.ignore();
	getline(f, name, '\n');
	f>> dim_y >> dim_x;
	num_obj = 0;
	map_arr = (char*)malloc(sizeof(char) * dim_x * dim_y);		//dynamic map allocation
	f.ignore();
	for (int y = 0; y < dim_y; y++) {							//reads the contents of the map, checking for spaces or newlines
		f.getline(map_arr + y * dim_x, dim_x+1);
	}
	f >> terminator;											//check if terminator string is correct
	if (terminator != ";;") {
		printf("Map file corrupted!");
		f.get();
		exit(1);
	}
}

Object* Map::Get_obj(COORD coordonata) {
	for (int i = 0; i < num_obj; i++) {
		if (Compare_coord2(coordonata, objects[i]->GetCoord())) return objects[i];
	}
}

char Map::GetLoc(COORD coord) {
	return *(map_arr + dim_x * coord.Y + coord.X);
}

void Map::Bind_object(Object* obj) {
	obj->Set_ID(num_obj);
	objects[num_obj] = obj;
	num_obj++;
}