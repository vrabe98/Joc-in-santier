#include <iostream>
#include <Windows.h>
#include "Music.h"
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

std::string WordWrap(std::string text,int padding) {						/*Introduce spatii suplimentare in string, pentru ca toate cuvintele
																sa fie afisate pe o linie fiecare, nu separate*/
	CONSOLE_SCREEN_BUFFER_INFO info_consola;
	HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(consola, &info_consola);
	COORD size = info_consola.dwMaximumWindowSize;
	int width = size.X;
	text.insert(0, padding, ' ');
	int k = text.length() / width;									//Fereastra consolei e de width de caractere, default.
	for (int i = 1; i <= k; i++, k = text.length() / width) {
		if (((text[width * i - 1] != ' ') || (text[width * i] != ' ')) && (text[width * i + 1] != '\n')) {
			int char_back = 0;
			do {
				char_back++;
				if (text[width * i - char_back - 1] == ' ') {
					for (int cont = 0; cont < char_back; cont++) {
						text.insert(width * i - char_back - 1, " ");
					}
					char_back = 0;
					break;
				}
			} while (text[width * i - char_back - 1] != ' ');
		}
	}
	text.erase(0, padding);
	return text;
}

int main(int argc,char* argv[]) {
	game.Splash("Splash.txt");
	std::cin.get();
	game.Menu("Maps.txt", "Character.txt", "Connections.txt", "Objects.txt", "NPC.txt", "Item_DB.txt", "Dialogues.txt","Vendors.txt","Quests.txt");
}