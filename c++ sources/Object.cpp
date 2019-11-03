#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "Object.h"

void WriteOneCharConsole(COORD coordonate, char caracter, unsigned short attribute);

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

void Container::Load_inventory(std::ifstream& obj_stream, Item** db) {
	int item_index;
	obj_stream >> inventory_size;
	inventory_size = inventory_size;
	for (int i = 0; i < inventory_size; i++) {
		obj_stream >> item_index;
		inventory[i] = db[item_index];
	}
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
	WriteOneCharConsole(coordonate, '0', obj_ID + 1);
	SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
	WriteOneCharConsole(info_consola.dwCursorPosition, '0', obj_ID + 1);
	info_consola.dwCursorPosition.X++;
	SetConsoleCursorPosition(consola, info_consola.dwCursorPosition);
	std::cout << " - " + name << "\n";
}

void Container::Transfer_to(Item* item) {
	inventory[inventory_size] = item;
	inventory_size++;
}

void Container::Show_inventory() {
	printf("Inventory:\n");
	for (int i = 0; i < inventory_size; i++) {
		std::cout << "[";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		std::cout << i;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "]. ";
		inventory[i]->Show_info(0);
		printf("\n");
	}
	printf("\n\n");
}

Item* Container::Interact() {
	char c;
	system("cls");
	Show_inventory();
	printf("Do you want to transfer any item to your inventory? (y/n): ");
	std::cin >> c;
	if (c == 'n') {
		system("cls");
		return nullptr;
	}
	else if (c == 'y') {
		int opt;
		Item* item;
		printf("Which item do you want to get? (enter the order number, the one before the item name): ");
		std::cin >> opt;
		if (opt >= inventory_size) return nullptr;
		else {
			item = inventory[opt];
			for (int i = opt; i < inventory_size-1; i++) {
				inventory[i] = inventory[i + 1];
			}
			inventory_size--;
			system("cls");
			return item;
		}
	}
	else {
		system("cls");
		Interact();
	}
}