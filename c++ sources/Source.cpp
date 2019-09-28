#include <iostream>
#include <Windows.h>
#include "Game.h"

Game game;

void WriteOneCharConsole(COORD coordonate, char caracter, unsigned short attribute) {
	LPDWORD written = new DWORD;
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleOutputAttribute(consola, &attribute, 1, coordonate, written);
	WriteConsoleOutputCharacter(consola, &caracter, 1, coordonate, written);
}

int Compare_coord(COORD coord1, COORD coord2) {
	if ((coord1.X == coord2.X) && (coord1.Y == coord2.Y)) return 1;
	else return 0;
}

int main() {
	game.Splash("Splash.txt");
	std::cin.get();
	game.Menu("Maps.txt", "Character.txt", "Connections.txt", "Objects.txt", "NPC.txt", "Item_DB.txt", "Dialogues.txt","Vendors.txt","Music.txt");
}