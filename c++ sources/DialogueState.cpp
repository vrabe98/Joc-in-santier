#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "DialogueState.h"

void DialogueState::Load(std::ifstream& stream,int j) {
	std::string aux;
	if(!j) stream.ignore();
	getline(stream, text, '\n');
	stream >> num_options;
	stream.ignore();
	getline(stream, aux, '\n');
	if (aux == "DIALOGUE OPTIONS:") {
		for (int i = 0; i < num_options; i++) {
			options[i] = new DialogueOption;
			if (i != 0) stream.ignore();
			getline(stream, options[i]->text);
			stream >> options[i]->exit;
		}
	}
	else {
		printf("Dialogue file corrupted!");
		exit(1);
	}
	stream.ignore();
	getline(stream, aux, '\n');
	if (aux != ";;") {
		printf("Dialogue file corrupted!");
		exit(1);
	}
	for (int i = 0; i < num_options-1; i++) {
		DialogueState* diag = new DialogueState;
		diag->Load(stream,1);
		options[i]->next = diag;
	}
}

void DialogueState::Enter_dialogue(std::string NPCname) {
	int opt;
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "-------------------------------------------\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	std::cout << NPCname;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout<< "\n-------------------------------------------\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout <<NPCname<<": "<< text<<"\n\n";
	for (int i = 0; i < num_options; i++) {
		std::cout << "[";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		std::cout << i;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout<< "]. " << options[i]->text;
		if (options[i]->exit) std::cout << " (EXIT)\n";
		else std::cout << "\n";
	}
	std::cout << "\nYour choice: ";
	std::cin >> opt;
	DialogueState* next = options[opt]->next;
	if (next) next->Enter_dialogue(NPCname);
	else {
		system("cls");
		return;
	}
}