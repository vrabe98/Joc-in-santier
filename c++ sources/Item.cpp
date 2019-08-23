#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "Item.h"

void Weapon::Load(std::ifstream& item_db_str) {
	std::string aux;
	item_db_str >> id;
	item_db_str.ignore();
	getline(item_db_str, name, '\n');
	item_db_str >> equip_slot;
	item_db_str >> weight;
	weight = weight;
	item_db_str >> damage;
	item_db_str >> aux;
	if (aux != ";;") {
		printf("Item database corrupted!");
		exit(1);
	}
}

void Armor::Load(std::ifstream& item_db_str) {
	std::string aux;
	item_db_str >> id;
	item_db_str.ignore();
	getline(item_db_str, name, '\n');
	item_db_str >> equip_slot;
	item_db_str >> weight;
	weight = weight;
	item_db_str >> armor;
	item_db_str >> aux;
	if (aux != ";;") {
		printf("Item database corrupted!");
		exit(1);
	}
}

void Generic_item::Load(std::ifstream& item_db_str) {
	std::string aux;
	int garbage;
	item_db_str >> id;
	item_db_str.ignore();
	getline(item_db_str, name, '\n');
	item_db_str >> equip_slot;
	item_db_str >> weight;
	item_db_str >> garbage;
	item_db_str >> aux;
	if (aux != ";;") {
		printf("Item database corrupted!");
		exit(1);
	}
}

void Generic_item::Show_info() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("Type: generic (non-equipable)\n");
	printf("Weight: %d\n", weight);
}

void Armor::Show_info() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("Type: armor (equip slot 1)\n");
	printf("Weight: %d\n", weight);
	printf("Armor value: %d\n", armor);
}

void Weapon::Show_info() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name<<"\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("Type: weapon (equip slot 0)\n");
	printf("Weight: %d\n", weight);
	printf("Damage: %d\n", damage);
}