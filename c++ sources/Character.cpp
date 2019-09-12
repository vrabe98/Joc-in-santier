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

Damage Character::GetWeaponDmg() {
	Damage dmg;
	if (equipped_items[RHAND] == nullptr) {
		dmg.type = "pumni";
		dmg.damage = FIST_DMG;
	}
	else {
		dmg.type = "arma";
		dmg.damage = equipped_items[RHAND]->GetDamage();
	}
	return dmg;
}

void Character::RefreshArmor() {		//function recalculates the armor value
	armor = 0;
	for (int i = 0; i <= 4; i++) {
		if (equipped_items[i] != nullptr) {
			armor += equipped_items[i]->GetArmor();
		}
	}
}

void Character::RefreshHP() {			//function replenishes HP, by recalculating its value
	float base = 100.0;
	hp = base * (1 + 0.05 * (strength - 10.0)) + constitution * 10.0;
}

void Character::ShowStats() {
	double main_hand_dmg=0,offhand_dmg=0;
	if (equipped_items[RHAND] != nullptr) main_hand_dmg = equipped_items[RHAND]->GetDamage();
	if (equipped_items[LHAND] != nullptr) offhand_dmg = equipped_items[LHAND]->GetDamage();
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "-------------------------------------------\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	std::cout << name<<" - Stats:";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "\n-------------------------------------------\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "Strength:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << strength;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nDexterity:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << dexterity;	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nConstitution:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << constitution;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nHP:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << hp;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nMain hand weapon damage:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << main_hand_dmg ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nOffhand weapon damage:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << offhand_dmg;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nArmor:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << armor<< "\n\n";
	std::cout << "Press any key to continue...";
	std::cin.ignore();
	std::cin.get();
	system("cls");
}

void Character::Equip(int item_id) {
	if (item_id >= inventory_size) printf("The number entered is invalid!");
	else if (inventory[item_id]->IsGeneric()) printf("The item you want to equip can't be equipped!");
	else if (equipped_items[inventory[item_id]->GetSlot()] != nullptr) printf("The slot is already occupied!");
	else {
		equipped_items[inventory[item_id]->GetSlot()] = inventory[item_id];
		inventory_size--;
		for (int i = item_id; i < inventory_size; i++) {
			inventory[i] = inventory[i + 1];
		}
	}
	RefreshArmor();
}

int Character::HasEquippedItems() {
	for (int i = 0; i < NUM_SLOTS - 1; i++) {
		if (equipped_items[i] != nullptr) return 1;
	}
	return 0;
}

void Character::Unequip(int slot) {
	if (slot >= NUM_SLOTS-1) printf("The number you entered is invalid!");
	else if (equipped_items[slot] == nullptr) printf("There's no equipment in that slot!");
	else {
		inventory_size++;
		inventory[inventory_size - 1] = equipped_items[slot];
		equipped_items[slot] = nullptr;
	}
	RefreshArmor();
}

void Character::Show_inventory(int want_to_equip) {
	while (1) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << "-------------------------------------------\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout << name<<" - Inventory:";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << "\n-------------------------------------------\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for (int i = 0; i < inventory_size; i++) {
			std::cout << "[";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			std::cout << i;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			std::cout << "]. ";
			inventory[i]->Show_info();
			printf("\n");
		}
		printf("\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << "-------------------------------------------\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout << "Equipped items:";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << "\n-------------------------------------------\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for (int i = 0; i < NUM_SLOTS; i++) {
			if (equipped_items[i] != nullptr) {
				std::cout << "[";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				std::cout << i;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				std::cout << "]. ";
				equipped_items[i]->Show_info();
				printf("\n");
			}
		}
		printf("\n\n");
		if ((inventory_size != 0 || HasEquippedItems()) && want_to_equip) {
			char opt;
			printf("Do you want to equip/unequip an item or exit (e/u/x): ");
			std::cin >> opt;
			if (opt == 'e') {
				int id;
				printf("Which item would you like to equip? (enter the order number from the inventory tab): ");
				std::cin >> id;
				Equip(id);
			}
			else if (opt == 'u') {
				int slot;
				printf("Which item would you like to unequip? (enter the order number from the equipment tab): ");
				std::cin >> slot;
				Unequip(slot);
			}
			else if (opt == 'x') return;
		}
		else if (!want_to_equip) return;
		else {
			printf("Nothing to see here...\n");
			std::cin.get();
			return;
		}
	}
}

void Character::Query_inventory(Object* cont) {
	if (cont==nullptr) {
		Show_inventory(1);
		std::cin.get();
		system("cls");
	}
	else {
		int opt;
		char opt_c;
		while (1) {
			Show_inventory(0);
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
			printf("Done? (y/n): ");
			std::cin >> opt_c;
			if (opt_c == 'y') return;
		}
	}
}

void Character::Interact_container(COORD new_coord) {
	char opt;
	printf("Do you want to transfer items to or from the container or just exit? (t/f/x)\n The option 'from' lets you view the inventory of the container without transfering any items.\n Your option: ");
	std::cin >> opt;
	if (opt == 't') {
		if (inventory_size) {
			Object* cont = current_map->Get_obj(new_coord);
			Query_inventory(cont);
		}
		else {
			printf("You have no items!");
			std::cin.ignore();
			std::cin.get();
			system("cls");
		}
	}
	else if (opt == 'f') {
		while (1) {
			Item* item = (current_map->Get_obj(new_coord))->Interact();
			if (item != nullptr) {
				inventory[inventory_size] = item;
				inventory_size++;
			}
			else return;
		}
	}
	else if (opt == 'x') {
		system("cls");
		return;
	}
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
		char opt;
		system("cls");
		std::cout << "Do you want to talk to " << npc_inter->GetName() << " or see his stats, or even see the items he has on? Or maybe you want to fight?"<<"\n(t/s/i/f/x-exit)";
		std::cin >> opt;
		if (opt == 't')
			npc_inter->Dialogue();
		else if (opt == 's')
			npc_inter->ShowStats();
		else if (opt == 'i') {
			npc_inter->Show_inventory(0);
			std::cin.ignore();
			std::cin.get();
		}
		else if (opt == 'f') {
			int result=Combat(this, npc_inter);
			if (result == 0) DeathScreen(this, npc_inter);
			else if (result == 1) {
				std::cout << "VICTORIE!\n" << npc_inter->GetName() << " a fost invins!";
				std::cin.get();
				system("cls");
			}
		}
		else return;
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
	while (!GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)&&!GetAsyncKeyState(VK_RETURN)&&!GetAsyncKeyState(0x49) && !GetAsyncKeyState(0x4B)) {}
	if (GetAsyncKeyState(0x49)&(1<<16)) {					//if I is pressed
		Query_inventory(nullptr);
	}
	else if (GetAsyncKeyState(0x4B) & (1 << 16)) {			//if the K key is pressed
		ShowStats();
	}
	else if (GetAsyncKeyState(VK_UP) & (1 << 16)) {			//if the UP arrow key is pressed
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

Character::Character(int x, int y,Map* starting_map,int inv_size,int str,int dex,int con,std::string nume,Item** inventory_copy,int items) {
	coordonate.X = x;
	coordonate.Y = y;
	current_map = starting_map;
	inventory_size = inv_size;
	strength = str;
	dexterity = dex;
	constitution = con;
	map_change_attempt = 1;
	name = nume;
	for (int i = 0; i < NUM_SLOTS; i++) {
		equipped_items[i] = nullptr;
	}
	for (int i = 0; i < items; i++) {
		inventory[i] = inventory_copy[i];
	}
	inventory_size = items;
	RefreshArmor();
	RefreshHP();
}

void Character::GetDamaged(float enemy_damage,Character* enemy,int riposte,int critted) {
	int block_roll;
	float received_dmg;
	block_roll = Roll_d20();
	if (block_roll < 0) block_roll = 0;
	else if (block_roll > 20) block_roll = 20;
	if (!riposte) {
		if (block_roll < 10||critted) {
			std::cout << name << " couldn't block the strike from " << enemy->GetName() << "\n";
			received_dmg = enemy_damage;			//no damage blocked
		}
		else if (block_roll < 19||(!critted)) {
			double block_mult = 1;
			if (equipped_items[RHAND] == nullptr && equipped_items[LHAND] == nullptr) block_mult = 0.5;	//fist block
			else if (equipped_items[LHAND] != nullptr) block_mult = equipped_items[LHAND]->BlockMultiplier();	//offhand block multiplier has priority
			else if (equipped_items[RHAND] == nullptr) block_mult = equipped_items[RHAND]->BlockMultiplier();	//main hand block multiplier, if no item in offhand
			std::cout << name << " blocked " << enemy->GetName() << "'s strike with his ";
			if (block_mult == 0.5) std::cout << "fists.\n";
			else if (block_mult == 0.4) std::cout << "weapon.\n";
			else if (block_mult == 0.1) std::cout << "shield.\n";
			received_dmg = block_mult* enemy_damage;		//average value, shields not implemented yet
		}
		else if (((block_roll == 19) || (block_roll == 20))&&(!critted)) {
			double block_mult = 1;
			float riposte_dmg = equipped_items[RHAND]->GetDamage();
			if (equipped_items[RHAND] == nullptr && equipped_items[LHAND] == nullptr) block_mult = 0.5;	//fist block
			else if (equipped_items[LHAND] != nullptr) block_mult = equipped_items[LHAND]->BlockMultiplier();	//offhand block multiplier has priority
			else if (equipped_items[RHAND] == nullptr) block_mult = equipped_items[RHAND]->BlockMultiplier();	//main hand block multiplier, if no item in offhand
			std::cout << name << " blocks the attack from " << enemy->GetName() << "with his ";
			if (block_mult == 0.5) std::cout << "fists";
			else if (block_mult == 0.4) std::cout << "weapon";
			else if (block_mult == 0.1) std::cout << "shield";
			std::cout<<". Oh no! " << enemy->GetName() << " let his guard down for a second! " << name << " sees the opportunity and strikes him!" << "\n";
			received_dmg = 0;						//100% block
			enemy->GetDamaged(riposte_dmg, this, 1,0);
		}
	}
	else {
		std::cout << name<<" is taken by surprise!"<<enemy->GetName()<<"'s riposte goes through his poor defense like a knife through butter!\n";
		received_dmg = enemy_damage;
	}
	received_dmg = received_dmg-(armor/20.0);
	if (received_dmg < 0) received_dmg = 0;
	std::cout << this->GetName() << " a luat " << received_dmg << " daune.\n";
	hp = hp - received_dmg;
}

int Character::Has1h() {
	if (equipped_items[RHAND] == nullptr||(!equipped_items[RHAND]->Is2h())) return 1;
	if (equipped_items[RHAND]->Is2h()) return 0;
}

float Character::GetEvasion() {
	float noshield_bonus = 0;
	if (equipped_items[LHAND] == nullptr) noshield_bonus = 1;
	else if (equipped_items[LHAND]->BlockMultiplier() == 0.4) noshield_bonus = 1;
	return 0.5 * (dexterity-10.0)+noshield_bonus*0.5;
}

Character::Character(){}
NPC::NPC() {
	root = nullptr;
}

int NPC::CheckNPC(COORD coord, int map_id) {
	if (Compare_coord3(coord, coordonate) && (map_id == current_map->Get_ID())) return 1;
	else return 0;
}

void NPC::Load(std::ifstream& npc_str,Map maps[],Item** database) {
	std::string aux;
	int map_ID;
	npc_str >> map_ID;
	current_map = &maps[map_ID];
	npc_str >> coordonate.X >> coordonate.Y;
	npc_str >> aux;
	if (aux != "Stats:") {
		printf("NPC file corrupted!");
		exit(1);
	}
	else {
		int stat;
		npc_str >> aux;
		if (aux != "STR") {
			printf("NPC file corrupted! Stats order is wrong!");
			exit(1);
		}
		else {
			npc_str >> stat;
			if (stat <= 20 && stat >= 0) strength = stat;
			else {
				printf("Wrong stat value! Stats need to be in the 0-20 range!");
				exit(1);
			}
		}
		npc_str >> aux;
		if (aux != "DEX") {
			printf("NPC file corrupted! Stats order is wrong!");
			exit(1);
		}
		else {
			npc_str >> stat;
			if (stat <= 20 && stat >= 0) dexterity = stat;
			else {
				printf("Wrong stat value! Stats need to be in the 0-20 range!");
				exit(1);
			}
		}
		npc_str >> aux;
		if (aux != "CON") {
			printf("NPC file corrupted! Stats order is wrong!");
			exit(1);
		}
		else {
			npc_str >> stat;
			if (stat <= 20 && stat >= 0) constitution = stat;
			else {
				printf("Wrong stat value! Stats need to be in the 0-20 range!");
				exit(1);
			}
		}
	}
	npc_str >> aux;
	if (aux != "Items:") {
		printf("NPC file corrupted!");
		exit(1);
	}
	else {
		int id=1, num_equipped;
		npc_str >> num_equipped;
		if (num_equipped > NUM_SLOTS) {
			std::cout << "Too many items for the character: " << name;
			exit(1);
		}
		else {
			for (int i = 0; i < num_equipped; i++) {
				npc_str >> id;
				id = id;
				equipped_items[database[id]->GetSlot()] = database[id];
			}
		}
	}
	npc_str.ignore();
	getline(npc_str,name, '\n');
	npc_str >> aux;
	if (aux != ";;") {
		printf("NPC file corrupted!");
		exit(1);
	}
	RefreshArmor();
	RefreshHP();
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