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
	item_db_str >> price;
	item_db_str >> equip_slot;
	item_db_str >> twohanded;
	item_db_str >> weight;
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
	item_db_str >> price;
	item_db_str >> equip_slot;
	item_db_str >> weight;
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
	item_db_str >> price;
	item_db_str >> equip_slot;
	item_db_str >> weight;
	item_db_str >> garbage;
	item_db_str >> aux;
	if (aux != ";;") {
		printf("Item database corrupted!");
		exit(1);
	}
}

void Shield::Load(std::ifstream& item_db_str) {
	std::string aux;
	int garbage;
	item_db_str >> id;
	item_db_str.ignore();
	getline(item_db_str, name, '\n');
	item_db_str >> price;
	item_db_str >> equip_slot;
	item_db_str >> weight;
	item_db_str >> garbage;
	item_db_str >> aux;
	if (aux != ";;") {
		printf("Item database corrupted!");
		exit(1);
	}
}

void Generic_item::Show_info(int vendor_op) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (vendor_op) std::cout << "	-	Price: " << price;
	std::cout << "\n";
	printf("Item type: generic (non-equipable)\n");
	printf("Weight: %d\n", weight);
}

void Shield::Show_info(int vendor_op) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (vendor_op) std::cout << "	-	Price: " << price;
	std::cout << "\n";
	printf("Item type: shield\n");
	printf("Slot: Left hand\n");
	printf("Weight: %d\n", weight);
}

void Armor::Show_info(int vendor_op) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (vendor_op) std::cout << "	-	Price: " << price;
	std::cout << "\n";
	printf("Item type: armor\n");
	switch (equip_slot) {
	case CHEST:	printf("Slot: Chest\n");
		break;
	case HEAD:	printf("Slot: Head\n");
		break;
	case ARMS:	printf("Slot: Arms\n");
		break;
	case LEGS:	printf("Slot: Legs\n");
		break;
	case FEET:	printf("Slot: Feet\n");
		break;
	}
	printf("Weight: %d\n", weight);
	printf("Armor value: %d\n", armor);
}

void Weapon::Show_info(int vendor_op) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << name;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (vendor_op) std::cout << "	-	Price: " << price;
	std::cout << "\n";
	printf("Item type: weapon\n");
	if (twohanded) {
		printf("Two-handed\n");
		printf("Slots: both hands\n");
	}
	else {
		printf("One-handed\n");
		printf("Slot: Right Hand\n");
	}
	printf("Weight: %d\n", weight);
	printf("Damage: %f\n", damage);
}