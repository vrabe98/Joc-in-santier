#include <iostream>
#include <Windows.h>
#include "Game.h"

Game game;

int main() {
	game.Load("Maps.txt", "Character.txt","Connections.txt","Objects.txt","NPC.txt","Item_DB.txt");
	game.Play();
}