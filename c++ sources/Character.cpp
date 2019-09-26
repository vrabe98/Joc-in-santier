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
	else if (GetConsoleChar(coordonata) == 'K'||GetConsoleChar(coordonata)=='B') return 3;
	else {													//if accessible, check boundaries;
		if ((coordonata.X > caract.current_map->Dimx() - 1) || (coordonata.Y > caract.current_map->Dimy() - 1) || (coordonata.X < 0) || (coordonata.Y < 0))
			return 0;
		else return 1;
	}
}

Damage Character::GetWeaponDmg(int hand) {
	Damage dmg;
	if (hand==RHAND&&equipped_items[RHAND] == nullptr) {
		dmg.type = "pumni";
		dmg.damage = FIST_DMG;
	}
	else {
		dmg.type = "arma";
		dmg.damage = equipped_items[hand]->GetDamage();
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
	if (equipped_items[LHAND] != nullptr&&(!equipped_items[LHAND]->Is2h())) offhand_dmg = equipped_items[LHAND]->GetDamage();
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
	std::cout << "\nCharisma:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << charisma;
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
	std::cout << armor;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nCurrent cash amount:\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << currency<<"\n\n";
	std::cout << "Press any key to continue...";
	std::cin.ignore();
	std::cin.get();
	system("cls");
}

void Character::Equip(int item_id) {
	if (item_id >= inventory_size) printf("The number entered is invalid!");
	else if (inventory[item_id]->IsGeneric()) printf("The item you want to equip can't be equipped!");
	else if ((equipped_items[inventory[item_id]->GetSlot()] != nullptr)&&(inventory[item_id]->GetSlot()!=RHAND)) printf("The slot is already occupied!");
	else {
		if(equipped_items[RHAND]==nullptr&&inventory[item_id]->GetSlot()==RHAND) equipped_items[RHAND] = inventory[item_id];
		else if (equipped_items[LHAND] == nullptr && inventory[item_id]->GetSlot() == RHAND) equipped_items[LHAND] = inventory[item_id];
		else equipped_items[inventory[item_id]->GetSlot()] = inventory[item_id];
		if (inventory[item_id]->IsWeapon() && inventory[item_id]->Is2h()) equipped_items[LHAND] = inventory[item_id];
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
		if (equipped_items[slot]->IsWeapon() && equipped_items[slot]->Is2h()) equipped_items[LHAND] = nullptr;
		equipped_items[slot] = nullptr;
	}
	RefreshArmor();
}

void Character::Show_inventory(int want_to_equip,int vendor_op) {
	while (1) {
		if(!vendor_op) system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << "-------------------------------------------\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout << name << " - Inventory:";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << "\n-------------------------------------------\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for (int i = 0; i < inventory_size; i++) {
			std::cout << "[";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			std::cout << i;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			std::cout << "]. ";
			inventory[i]->Show_info(vendor_op);
			printf("\n");
		}
		printf("\n\n");
		if (vendor_op) return;
		if (!vendor_op) {
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
					if (i == LHAND && equipped_items[i]->Is2h()) {
						printf("Slot occupied by two-handed weapon.\n");
					}
					else equipped_items[i]->Show_info(0);
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
}

void Character::AddToInventory(Item* transferred_item) {
	if (transferred_item != nullptr) {
		inventory[inventory_size] = transferred_item;
		inventory_size++;
	}
	else return;
}

Item* Character::RemoveFromInventory(int order_nr) {
	Item* item;
	item = inventory[order_nr];
	for (int i = order_nr; i < inventory_size - 1; i++) {
		inventory[i] = inventory[i + 1];
	}
	inventory_size--;
	return item;
}

void Character::Query_inventory(Object* cont) {
	if (cont == nullptr) {
		Show_inventory(1,0);
		std::cin.get();
		system("cls");
	}
	else {
		int opt;
		char opt_c;
		while (1) {
			Show_inventory(0,0);
			printf("Which item would you like to deposit in the container? (enter the order number): ");
			std::cin >> opt;
			if (opt >= inventory_size) Query_inventory(cont);
			else {
				Item* item=RemoveFromInventory(opt);
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
			AddToInventory(item);
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

void NPC::Interact(Character* mainchar) {
	char opt;
	system("cls");
	std::cout << "Do you want to talk to " << name << " or see his stats, or even see the items he has on? Or maybe you want to fight?" << "\n(t/s/i/f/x-exit)";
	std::cin >> opt;
	if (opt == 't')
		Dialogue();
	else if (opt == 's')
		ShowStats();
	else if (opt == 'i') {
		Show_inventory(0,0);
		std::cin.ignore();
		std::cin.get();
	}
	else if (opt == 'f') {
		int result = Combat(mainchar,this);
		if (result == 0) DeathScreen(mainchar,this);
		else if (result == 1) {
			std::cout << "VICTORIE!\n" << name << " a fost invins!";
			std::cin.get();
			system("cls");
		}
		else if (result == 2) {
			std::cout << "Ai fugit ca ultimul las de " << name << ". Csf, tot in viata esti, desi nu meriti.\n";
			std::cin.get();
			system("cls");
		}
	}
	else return;
}

void Vendor::Interact(Character* mainchar) {
	char opt;
	system("cls");
	std::cout << "Do you want to talk to " << name << " or buy/sell something?" << "\n(t/b/x-exit)";
	std::cin >> opt;
	if (opt == 't')
		Dialogue();
	else if (opt == 'b') {
		BuySell(mainchar);
	}
	else return;
}

void Character::Interact_NPC(COORD new_coord) {
	NPC* npc_inter = nullptr;
	npc_inter=game.GetNPC(new_coord, current_map->Get_ID());
	if (npc_inter) {
		npc_inter->Interact(this);
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

Character::Character(int x, int y,Map* starting_map,int inv_size,int str,int dex,int con,int cha,float currency,std::string nume,Item** inventory_copy,int items) {
	coordonate.X = x;
	coordonate.Y = y;
	current_map = starting_map;
	inventory_size = inv_size;
	strength = str;
	dexterity = dex;
	constitution = con;
	charisma = cha;
	this->currency = currency;
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
			else if (equipped_items[LHAND] == nullptr) block_mult = equipped_items[RHAND]->BlockMultiplier();	//main hand block multiplier, if no item in offhand
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
	received_dmg = received_dmg-(armor/20.0f);
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
	return 0.5f * (dexterity-10.0f)+noshield_bonus*0.5f;
}

Character::Character(){}
NPC::NPC() {
	currency = 0;
	root = nullptr;
}

int NPC::CheckNPC(COORD coord, int map_id) {
	if (Compare_coord3(coord, coordonate) && (map_id == current_map->Get_ID())) return 1;
	else return 0;
}

void Vendor::Load(std::ifstream& vendor_str, Map maps[], Item** database) {
	std::string aux;
	int map_ID,stat,invsize;
	vendor_str >> map_ID;
	current_map = &maps[map_ID];
	vendor_str >> coordonate.X >> coordonate.Y;
	vendor_str >> stat;
	if (stat <= 20 && stat >= 0) charisma = stat;
	else {
		printf("Wrong stat value! Stats need to be in the 0-20 range!");
		exit(1);
	}
	vendor_str >> invsize;
	vendor_str >> aux;
	aux = aux;
	if (aux != "Items:") {
		printf("Vendor file corrupted!");
		exit(1);
	}
	else {
		for (int i = 0; i <invsize; i++) {
			int item_id;
			Item* added;
			vendor_str >> item_id;
			added = database[item_id];
			AddToInventory(added);
		}
	}
	vendor_str.ignore();
	getline(vendor_str, name, '\n');
	vendor_str >> aux;
	if (aux != ";;") {
		printf("Vendor file corrupted!");
		exit(1);
	}
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
		npc_str >> aux;
		if (aux != "CHA") {
			printf("NPC file corrupted! Stats order is wrong!");
			exit(1);
		}
		else {
			npc_str >> stat;
			if (stat <= 20 && stat >= 0) charisma = stat;
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

void Vendor::Draw(Map* map, int style) {
	if (current_map == map) {
		CONSOLE_SCREEN_BUFFER_INFO info_consola;
		HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(consola, &info_consola);
		SetConsoleCursorPosition(consola, coordonate);
		SetConsoleTextAttribute(consola, style + 1);
		printf("B");
		SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
		printf("B");
		SetConsoleTextAttribute(consola, 7);
		std::cout << " - " + name << "\n";
	}
}

void Vendor::Display_inventories(Character* mainchar) {
	system("cls");
	Show_inventory(0,1);
	mainchar->Show_inventory(0,1);
}

void Vendor::BuySell(Character* mainchar) {
	int option;
	char opt;
	while (1){
		Display_inventories(mainchar);
		std::cout << "Buy or sell? (b/s/x-exit)" << "\n";
		std::cin >> opt;
		if (opt == 'b') {
			std::cout << "Enter the order number of the item you wish to buy: ";
			std::cin >> option;
			if (option >= inventory_size) {
				std::cout << "The item number you entered is larger than the inventory size of the vendor!";
				std::cin.get();
				return;
			}
			else {
				Item* transferred = RemoveFromInventory(option);
				float sum = transferred->GetPrice() * (1 + (this->CHA()-mainchar->CHA()) * 0.1);
				std::cout << "Sum to be paid: " << sum << "\n";
				if (sum > mainchar->GetCash()) {
					std::cout << "Too expensive!";
				}
				else {
					mainchar->AddToInventory(transferred);
					mainchar->Pay(sum);
				}
				std::cin.get();
				std::cin.get();
				system("cls");
			}
		}
		else if (opt == 's') {
			std::cout << "Enter the order number of the item you wish to sell: ";
			std::cin >> option;
			if (option >= mainchar->GetInvSize()) {
				std::cout << "The item number you entered is larger than your own inventory size!";
				std::cin.get();
				system("cls");
				return;
			}
			else {
				Item* transferred = mainchar->RemoveFromInventory(option);
				float sum = (-1) * transferred->GetPrice() * (1 + (mainchar->CHA() - this->CHA()) * 0.1);
				std::cout << "Sum to be received: " << (-1)*sum<<"\n";
				AddToInventory(transferred);
				mainchar->Pay(sum);
				std::cin.get();
				std::cin.get();
				system("cls");
			}
		}
		else if (opt == 'x') {
			system("cls");
			return;
		}
	}
}