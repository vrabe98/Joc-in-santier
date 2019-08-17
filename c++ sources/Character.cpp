#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "Character.h"


extern Game game;

int Compare_coord3(COORD coord1, COORD coord2) {
	if ((coord1.X == coord2.X) && (coord1.Y == coord2.Y)) return 1;
	else return 0;
}

char GetConsoleChar(COORD coord) {
	SMALL_RECT rect;
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	PCHAR_INFO buffer=(PCHAR_INFO)malloc(1*sizeof(CHAR_INFO));
	COORD buffer_size,position;
	if (!buffer) {
		printf("Dynamic allocation error!");
		exit(1);
	}
	buffer_size.X = buffer_size.Y = 1;
	position.X = position.Y = 0;
	rect.Top = coord.Y;
	rect.Left = coord.X;
	rect.Bottom = rect.Top;
	rect.Right = rect.Left;
	ReadConsoleOutput(consola, buffer, buffer_size, position, &rect);
	return buffer->Char.AsciiChar;
}

void Character::Draw() {								//function draws both the character and the current map
	CONSOLE_SCREEN_BUFFER_INFO info_consola;
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	if (map_change_attempt) system("cls");
	else for (int i = 0; i <= exp2(20); i++) {}
	current_map->Draw();									//drawing the current map;
	GetConsoleScreenBufferInfo(consola, &info_consola);	//this sequence draws the character with another color than the map
	SetConsoleCursorPosition(consola, coordonate);
	SetConsoleTextAttribute(consola, 2);
	printf("X");
	SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
	SetConsoleTextAttribute(consola, 7);
	if (map_change_attempt == 1) map_change_attempt = 0;
}

int Check_terrain(Character caract,COORD coordonata) {		//function checks if the terrain is accessible,if not returns 0;
	if (GetConsoleChar(coordonata) == '|' || GetConsoleChar(coordonata) == '+' || GetConsoleChar(coordonata) == '-' || GetConsoleChar(coordonata) == 'O')
		return 0;
	else if (GetConsoleChar(coordonata) == '0') return 2;
	else if (GetConsoleChar(coordonata) == 'K') return 3;
	else {													//if accessible, check boundaries;
		if ((coordonata.X > caract.current_map->Dimx() - 1) || (coordonata.Y > caract.current_map->Dimy() - 1) || (coordonata.X < 0) || (coordonata.Y < 0))
			return 0;
		else return 1;
	}
}

void Character::Show_inventory() {
	system("cls");
	printf("Inventory:\n");
	for (int i = 0; i < inventory_size; i++) {
		printf("%d. ", i);
		inventory[i]->Show_info();
		printf("\n");
	}
	printf("\n\n");
}

void Character::Query_inventory(Object* cont) {
	Show_inventory();
	if (cont==nullptr) {
		system("pause");
		system("cls");
	}
	else {
		int opt;
		printf("Which item would you like to deposit in the container? (enter the order number): ");
		std::cin >> opt;
		if (opt >= inventory_size) Query_inventory(cont);
		else {
			Item* item;
			item = inventory[opt];
			for (int i = opt; i < inventory_size - 1; i++) {
				inventory[i] = inventory[i + 1];
			}
			inventory_size--;
			cont->Transfer_to(item);
			system("cls");
		}
	}
}

void Character::Interact_container(COORD new_coord) {
	std::string opt;
	printf("Do you want to transfer items to or from the container? (to/from/anything else)\n The option 'from' lets you view the inventory of the container without transfering any items.\n Your option: ");
	std::cin >> opt;
	if (opt == "to") {
		Object* cont = current_map->Get_obj(new_coord);
		Query_inventory(cont);
	}
	else if (opt == "from") {
		Item* item = (current_map->Get_obj(new_coord))->Interact();
		if (item != nullptr) {
			inventory[inventory_size] = item;
			inventory_size++;
		}
	}
	else system("cls");
}

std::string NPC::GetName() {
	return name;
}

void NPC::Dialogue() {
	root->Enter_dialogue(name);
}

void Character::Interact_NPC(COORD new_coord) {
	NPC* npc_inter = nullptr;
	npc_inter=game.GetNPC(new_coord, current_map->Get_ID());
	if (npc_inter) {
		npc_inter->Dialogue();
	}
	else {
		printf("NPC not found! Error!");
		exit(1);
	}
}

void Character::Move() {
	COORD new_coord = coordonate;
	int entered_connection = 0;
	int check_ter;
	while (!GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)&&!GetAsyncKeyState(VK_RETURN)&&!GetAsyncKeyState(0x49)) {}
	if (GetAsyncKeyState(0x49)&(1<<16)) {					//if I is pressed
		Query_inventory(nullptr);
	}
	else if (GetAsyncKeyState(VK_UP) & (1 << 16)) {				//if the UP arrow key is pressed
		new_coord.Y-=1;
	}
	else if (GetAsyncKeyState(VK_DOWN) & (1 << 16)) {		//DOWN arrow key press
		new_coord.Y+=1;
	}
	else if (GetAsyncKeyState(VK_LEFT) & (1 << 16)) {		//LEFT arrow key press
		new_coord.X-=1;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & (1 << 16)) {		//RIGHT arrow key press
		new_coord.X+= 1;
	}
	else if (GetAsyncKeyState(VK_RETURN) & (1 << 16)) {
		game.Enter_connection();
		entered_connection = 1;
	}
	check_ter = Check_terrain(*this, new_coord);
	if (check_ter == 2) {			//container interaction
		Interact_container(new_coord);
	}
	else if (check_ter == 3) {		//NPC dialogue interaction
		Interact_NPC(new_coord);
	}
	else if ((check_ter == 1) && !entered_connection) coordonate = new_coord;
}

void Character::Change_map(Map* map,COORD coord) {
	current_map = map;
	coordonate = coord;
}

Character::Character(int x, int y,Map* starting_map,int inv_size) {
	coordonate.X = x;
	coordonate.Y = y;
	current_map = starting_map;
	inventory_size = inv_size;
	map_change_attempt = 1;
}

Character::Character(){}
NPC::NPC(){}

int NPC::CheckNPC(COORD coord, int map_id) {
	if (Compare_coord3(coord, coordonate) && (map_id == current_map->Get_ID())) return 1;
	else return 0;
}

void NPC::Load(std::ifstream& npc_str,Map maps[]) {
	std::string aux;
	int map_ID;
	npc_str >> map_ID;
	current_map = &maps[map_ID];
	npc_str >> coordonate.X >> coordonate.Y;
	npc_str.ignore();
	getline(npc_str,name, '\n');
	npc_str >> aux;
	if (aux != ";;") {
		printf("NPC file corrupted!");
		exit(1);
	}
}

void NPC::Draw(Map* map,int style) {
	if (current_map == map) {
		CONSOLE_SCREEN_BUFFER_INFO info_consola;
		HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(consola, &info_consola);
		SetConsoleCursorPosition(consola, coordonate);
		SetConsoleTextAttribute(consola, style + 1);
		printf("K");
		SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
		printf("K");
		SetConsoleTextAttribute(consola, 7);
		std::cout << " - " + name << "\n";
	}
}