#include <iostream>
#include <Windows.h>
#include "Game.h"

Game game;

int main() {
	game.Splash("Splash.txt");
	std::cin.get();
	game.Menu("Maps.txt", "Character.txt", "Connections.txt", "Objects.txt", "NPC.txt", "Item_DB.txt", "Dialogues.txt","Vendors.txt","Music.txt");
}