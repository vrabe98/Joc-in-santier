#include <iostream>
#include <Windows.h>
#include "Game.h"

Game game;

int main() {
	game.Splash("Splash.txt");
	std::cin.get();
	game.Menu();
}