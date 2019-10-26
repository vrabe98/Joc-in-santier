#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "Game.h"

int Compare_coord(COORD coord1, COORD coord2);

int Game::Check_NPC(int auxint, std::string aux) {
	return npcs[auxint]->CheckName(aux);
}

void Game::Load_maps(std::ifstream& map_stream) {
	std::string aux;
	for (int i = 0; i < 5; i++) {				//skip file format specifiers at the top
		getline(map_stream, aux, '\n');
	}
	getline(map_stream, aux, '\n');
	if (aux == "Number of maps:") {
		map_stream >> num_maps;
	}
	map_stream >> aux;
	if (aux != ";;") {
		printf("Map file corrupted!");
		exit(1);
	}
	for (int i = 0; i < num_maps; i++) {
		maps[i].Load(map_stream);
	}
}

void Game::Load_MainCharacter(std::ifstream& character_stream) {
	int map_id = 0, x = 0, y = 0, inventory_size = 0, str = 0,dex=0,con=0,cha=0,num_items=0;
	float currency = 0.0;
	char name[100];
	Item* inventory[MAX_ITEMS];
	std::string aux;
	getline(character_stream, aux, '\n');
	if (aux == "Starting map ID:") {
		character_stream >> map_id;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Starting coordinates:") {
		character_stream >> y >> x;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Starting cash:") {
		character_stream >> currency;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Inventory size:") {
		character_stream >> inventory_size;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Strength:") {
		character_stream >> str;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Dexterity:") {
		character_stream >> dex;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Constitution:") {
		character_stream >> con;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Charisma:") {
		character_stream >> cha;
	}
	character_stream.ignore();
	getline(character_stream, aux, '\n');
	if (aux == "Items:") {
		while (!character_stream.eof()) {
			int id;
			num_items++;
			character_stream >> id;
			inventory[num_items-1] = item_db[id];
		}
	}
	printf("What's your name? ");
	std::cin.ignore(INT_MAX, '\n');
	std::cin.getline(name,100);
	std::string nume(name);
	main_character = new Character(x, y, &maps[map_id],inventory_size,str,dex,con,cha,currency,nume,inventory,num_items);
}

void Game::Load_connections(std::ifstream& conn_stream) {
	std::string aux;
	for (int i = 0; i < 6; i++) {				//skip file format specifiers at the top
		getline(conn_stream, aux, '\n');
	}
	conn_stream >> num_conn;
	conn_stream >> aux;
	if (aux != ";;") {
		printf("Connections file corrupted!");
		exit(1);
	}
	for (int i = 0; i < num_conn; i++) {
		con[i].Load(conn_stream,maps);
	}
}

void Game::Load_objects(std::ifstream& obj_stream) {
	std::string aux;
	int obj_num;
	for (int i = 0; i < 8; i++) {				//skip file format specifiers at the top
		getline(obj_stream, aux, '\n');
	}
	obj_stream >> obj_num;
	obj_stream >> aux;
	if (aux != ";;") {
		printf("Objects file corrupted!");
		exit(1);
	}
	for (int i = 0; i < obj_num; i++) {
		std::string type;
		int map;
		obj_stream >> type;
		if (type == "Generic") {
			Generic_object* obj = new Generic_object;
			obj->Load(obj_stream,nullptr);
			map = obj->Get_map();
			maps[map].Bind_object(obj);
		}
		else if (type == "Container") {
			Container* obj = new Container;
			obj->Load(obj_stream,item_db);
			map = obj->Get_map();
			maps[map].Bind_object(obj);
		}
	}
}

void Game::Load_npcs(std::ifstream& npc_stream) {
	std::string aux;
	for (int i = 0; i < 13; i++) {				//skip file format specifiers at the top
		getline(npc_stream, aux, '\n');
	}
	npc_stream >> num_chars;
	npc_stream >> aux;
	if (aux != ";;") {
		printf("NPC file corrupted!");
		exit(1);
	}
	for (int i = 0; i < num_chars; i++) {
		npcs[i] = new NPC;
		npc_stream.ignore();				//ignores the character ID
		npc_stream.ignore();				//ignores the newline after the character ID
		npcs[i]->Load(npc_stream, maps,item_db);
	}
}

void Game::Load_item_db(std::ifstream& item_db_stream) {
	std::string aux;
	for (int i = 0; i < 19; i++) {				//skip file format specifiers at the top
		getline(item_db_stream, aux, '\n');
	}
	item_db_stream >> num_items;
	item_db_stream >> aux;
	if (aux != ";;") {
		printf("Item database corrupted!");
		exit(1);
	}
	for (int i = 0; i < num_items; i++) {
		std::string type;
		item_db_stream >> type;
		if (type == "Weapon") {
			item_db[i] = new Weapon();
		}
		else if (type == "Armor") {
			item_db[i] = new Armor();
		}
		else if (type == "Generic") {
			item_db[i] = new Generic_item();
		}
		else if (type == "Shield") {
			item_db[i] = new Shield();
		}
		item_db[i]->Load(item_db_stream);
	}
}

void Game::Load_vendors(std::ifstream& vendor_str) {
	std::string aux;
	for (int i = 0; i < 9; i++) {				//skip file format specifiers at the top
		getline(vendor_str, aux, '\n');
	}
	vendor_str >> num_vendors;
	vendor_str >> aux;
	if (aux != ";;") {
		printf("Vendor file corrupted!");
		exit(1);
	}
	for (int i = num_chars; i < num_chars + num_vendors; i++) {
		npcs[i] = new Vendor();
		vendor_str.ignore();				//ignores the character ID
		vendor_str.ignore();				//ignores the newline after the character ID
		npcs[i]->Load(vendor_str, maps, item_db);
	}
}

void Game::Load_dialogues(std::ifstream& dialogue_stream) {
	std::string aux;
	for (int i = 0; i < 29; i++) {				//skip file format specifiers at the top
		getline(dialogue_stream, aux, '\n');
	}
	for (int i = 0; i < num_chars+num_vendors; i++) {
		DialogueState* root = new DialogueState;
		std::string _aux;
		int auxint;
		dialogue_stream >> auxint;
		dialogue_stream.ignore();
		getline(dialogue_stream, _aux, '\n');
		if (Check_NPC(auxint, _aux)) {
			dialogue_stream >> _aux;
			if (_aux == ";;") {
				root->Load(dialogue_stream,0);
			}
			else {
				printf("Dialogue file corrupted!");
				exit(1);
			}
		}
		else {
			printf("Dialogue file corrupted!");
			exit(1);
		}
		npcs[i]->Bind_dialogue_root(root);
	}
}

void Game::Load_start_quest(std::ifstream& quest_stream) {
	std::string aux;
	for (int i = 0; i < 14; i++) {				//skip file format specifiers at the top
		getline(quest_stream, aux, '\n');
	}
	main_character->Load_startquest(quest_stream);
}

void Game::Load(std::string maps_file, std::string character_file,std::string conn_file,std::string obj_file,std::string npc_file,std::string item_db_file,std::string dialogue_file,std::string vendor_file,std::string startquest_file) {
	std::ifstream map_stream(maps_file, std::ifstream::in);
	std::ifstream character_stream(character_file, std::ifstream::in);
	std::ifstream connections_stream(conn_file, std::ifstream::in);
	std::ifstream objects_stream(obj_file, std::ifstream::in);
	std::ifstream npc_stream(npc_file, std::ifstream::in);
	std::ifstream item_db_stream(item_db_file, std::ifstream::in);
	std::ifstream dialogue_stream(dialogue_file, std::ifstream::in);
	std::ifstream vendor_stream(vendor_file, std::ifstream::in);
	std::ifstream startquest_stream(startquest_file, std::ifstream::in);
	Load_maps(map_stream);
	Load_item_db(item_db_stream);
	Load_objects(objects_stream);
	Load_MainCharacter(character_stream);
	Load_connections(connections_stream);
	Load_npcs(npc_stream);
	Load_vendors(vendor_stream);
	Load_dialogues(dialogue_stream);
	Load_start_quest(startquest_stream);		//loads the starting quest
}

void Game::Play() {
	while (1) {
		fflush(stdin);
		main_character->RefreshQuests();
		main_character->Draw();
		for (int i = 0; i < num_chars+num_vendors; i++) {
			npcs[i]->Draw(main_character->current_map,i+1);
		}
		printf(" ");
		main_character->Move();
	}
}

NPC* Game::GetNPC(COORD coordonate, int map_id) {
	for (int i = 0; i < num_chars+num_vendors; i++) {
		if (npcs[i]->CheckNPC(coordonate, map_id)) return npcs[i];
	}
	return nullptr;
}

Connection Game::Search(COORD coord, int id_mapa) {
	Connection invalid;
	for (int i = 0; i < num_conn; i++) {
		if ((con[i]._entry.mapa.map_id == id_mapa) && Compare_coord(coord, con[i]._entry.coordonate)) {
			return con[i];
		}
		else if ((con[i]._exit.mapa.map_id == id_mapa) && Compare_coord(coord, con[i]._exit.coordonate)) {
			return con[i];
		}
	}
	invalid.valid = 0;
	return invalid;
}

void Game::Enter_connection() {
	Connection conn = Search(main_character->coordonate, main_character->current_map->map_id);
	if (conn.valid) {
		if (main_character->current_map->map_id == conn._entry.mapa.map_id) {
			main_character->Change_map(&maps[conn._exit.mapa.map_id],conn._exit.coordonate);
		}
		else if (main_character->current_map->map_id == conn._exit.mapa.map_id) {
			if (conn.two_way) {
				main_character->Change_map(&maps[conn._entry.mapa.map_id],conn._entry.coordonate);
			}
			else {
				printf("The other map cannot be reached! The connection is one-way!");
			}
		}
		main_character->map_change_attempt = 1;
	}
	Sleep(70);
}

void Game::Splash(std::string splashscreen_file) {
	std::ifstream stream(splashscreen_file);
	std::string splash((std::istreambuf_iterator<char>(stream)),
		(std::istreambuf_iterator<char>()));
	std::cout << splash;
}

void Game::Menu(std::string maps_file, std::string character_file, std::string conn_file, std::string obj_file, std::string npc_file, std::string item_db_file, std::string dialogue_file,std::string vendor_file,std::string music_file,std::string quest_file) {
	int opt;
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "-------------------------------------------\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	std::cout << "MENIU";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "\n-------------------------------------------\n\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "[";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "1";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "]. Intra in joc\n";
	std::cout << "[";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "2";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "]. Iesi, cat timp mai poti\n\n";
	std::cout << "Ce alegi? ";
	std::cin >> opt;
	if (opt == 1) {
		std::string music;
		std::ifstream mus(music_file, std::ifstream::in);
		Load(maps_file,character_file,conn_file,obj_file,npc_file,item_db_file,dialogue_file,vendor_file,quest_file);
		getline(mus, music, '\n');
		PlaySound(TEXT(music.c_str()), NULL, SND_FILENAME|SND_ASYNC);
		Play();
	}
	else return;
}

void Game::Set_flag(Quest_flag flag_unset){
	std::map<std::string, Quest_flag>::iterator iter = unclaimed_flags.find(flag_unset.Get_name());
	if (iter == unclaimed_flags.end()) {
		unclaimed_flags[flag_unset.Get_name()] = flag_unset;
	}
	else {
		Quest_flag found = iter->second;
		found.Increment_counter();
		unclaimed_flags[found.Get_name()] = found;
	}
}