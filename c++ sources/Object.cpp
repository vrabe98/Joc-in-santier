#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "Object.h"

void Generic_object::Load(std::ifstream& obj_stream,Item** db) {
	std::string aux;
	obj_stream >> coordonate.Y >> coordonate.X;
	obj_stream >> map_ID;
	obj_stream.ignore();
	getline(obj_stream, name,'\n');
	obj_stream >> aux;
	aux = aux;
	if (aux != ";;") {
		printf("Objects file corrupted!");
		exit(1);
	}
}
void Generic_object::Draw() {
	CONSOLE_SCREEN_BUFFER_INFO info_consola;
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(consola, &info_consola);
	SetConsoleCursorPosition(consola, coordonate);
	SetConsoleTextAttribute(consola, obj_ID + 1);
	printf("O");
	SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
	printf("O");
	SetConsoleTextAttribute(consola, 7);
	std::cout << " - " + name << "\n";
}

void Container::Load(std::ifstream& obj_stream,Item** db) {
	std::string aux;
	Load_inventory(obj_stream,db);
	obj_stream >> coordonate.Y >> coordonate.X;
	obj_stream >> map_ID;
	obj_stream.ignore();
	getline(obj_stream, name, '\n');
	obj_stream >> aux;
	aux = aux;
	if (aux != ";;") {
		printf("Objects file corrupted!");
		exit(1);
	}
}

void Container::Draw() {
	CONSOLE_SCREEN_BUFFER_INFO info_consola;
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(consola, &info_consola);
	SetConsoleCursorPosition(consola, coordonate);
	SetConsoleTextAttribute(consola, obj_ID + 1);
	printf("0");
	SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
	printf("0");
	SetConsoleTextAttribute(consola, 7);
	std::cout << " - " + name << "\n";
}

void Container::Load_inventory(std::ifstream& obj_stream,Item** db) {
	int item_index;
	obj_stream >> inventory_size;
	inventory_size = inventory_size;
	for (int i = 0; i < inventory_size; i++) {
		obj_stream >> item_index;
		inventory[i] = db[item_index];
	}
}

Item* Container::Interact() {
	char c;
	system("cls");
	printf("Inventory:\n");
	for (int i = 0; i < inventory_size; i++) {
		printf("%d. ", i);
		inventory[i]->Show_info();
		printf("\n");
	}
	printf("\n\n");
	printf("Do you want to transfer any item to your inventory? (y/n) ");
	std::cin >> c;
	if (c == 'n') {
		system("cls");
		return nullptr;
	}
	else if (c == 'y') {
		int opt;
		Item* item;
		printf("Which item do you want to get? (enter the order number, the one before the item name) ");
		std::cin >> opt;
		if (opt >= inventory_size) return nullptr;
		else {
			item = inventory[opt];
			inventory[opt] = nullptr;
			system("cls");
			return item;
		}
	}
	else Interact();
}