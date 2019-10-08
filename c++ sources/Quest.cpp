#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Quest.h"
#include "Game.h"

extern Game game;

std::string WordWrap(std::string text,int padding);

void Quest_objective::UpdateState() {
	if (state == OBJECTIVE_COMPLETE) return;
	else {

	}
}

void Quest_flag::Load(std::ifstream& qstream) {
	qstream >> name;
	qstream >> counter;
}

void Quest_objective::Load(std::ifstream& qstream) {
	qstream.ignore();
	getline(qstream, description, '\n');
	qstream >> num_flags;
	for (int i = 0; i < num_flags; i++) {
		needed_flags[i] = new Quest_flag();
		needed_flags[i]->Load(qstream);
	}
}

void Quest::Load(std::ifstream& qstream) {
	std::string aux;
	getline(qstream, name, '\n');
	getline(qstream, description, '\n');
	qstream>>  num_objectives;
	qstream >> aux;
	if (aux == "OBJECTIVES:") {
		for (int i = 0; i < num_objectives; i++) {
			objectives[i] = new Quest_objective();
			objectives[i]->Load(qstream);
		}
	}
	else {
		std::cout << "Quest/dialogue file corrupted!";
		exit(1);
	}
	qstream >> aux;
	if (aux != ";;") {
		std::cout << "Quest/dialogue file corrupted!";
		exit(1);
	}
}

Quest_flag::Quest_flag() {
	counter = 1;
}

void Quest_objective::Show() {
	std::cout << WordWrap("- "+description,0);

}

void Quest::Show(int order_no) {
	std::string aux;
	std::cout << "[";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << order_no;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "]. ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "Title: ";
	aux = "Title: ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << WordWrap(name+"\n",aux.length());
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "Description: ";
	aux = "Description: ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << WordWrap(description,aux.length());
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nStatus: ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	switch (status) {
	case IN_PROGRESS: std::cout << "IN PROGRESS";
		break;
	case ACTIVE: std::cout << "ACTIVE";
		break;
	case FINISHED: std::cout << "FINISHED";
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "\nOBJECTIVES: \n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	for (int i = 0; i < num_objectives; i++) {
		objectives[i]->Show();
	}
}

void Quest_flag::Refresh() {
	if (counter > 0) {
		std::map<std::string, Quest_flag>* mapa = game.Get_unclaimed_flags_map();
		std::map<std::string, Quest_flag>::iterator iter = mapa->find(name);
		if (iter != mapa->end()) {
			Quest_flag flag = iter->second;
			counter -= flag.Get_counter();
			if (counter < 0) counter = 0;
		}
	}
}

void Quest_objective::Refresh() {
	int ndone = 0;		//reverse of "done", means done when 0
	if (state == OBJECTIVE_IN_PROGRESS) {
		for (int i = 0; i < num_flags; i++) {
			needed_flags[i]->Refresh();
		}
		for (int i = 0; i < num_flags&&ndone==0; i++) {
			ndone = needed_flags[i]->Get_counter();
		}
		if (!ndone) state = OBJECTIVE_COMPLETE;
	}
}

void Quest::Refresh() {
	int done = 1;
	if (status == IN_PROGRESS || status == ACTIVE) {
		for (int i = 0; i < num_objectives; i++) {
			objectives[i]->Refresh();
		}
		for (int i = 0; i < num_objectives&&done==1; i++) {
			done = objectives[i]->Finished();
		}
		if (done == 1) status = FINISHED;
	}
}