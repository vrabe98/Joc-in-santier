#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Quest.h"
#include "Game.h"

extern Game game;

std::string WordWrap(std::string text,int padding);

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

void Generic_quest::Load(std::ifstream& qstream) {
	std::string aux;
	qstream >> aux;
	qstream.ignore();
	getline(qstream, name, '\n');
	getline(qstream, description, '\n');
	qstream>>  num_objectives;
	qstream >> aux;
	if (aux != "OBJECTIVES:") {
		std::cout << "Quest file corrupted!";
		exit(1);
	}
	for (int i = 0; i < num_objectives; i++) {
		objectives[i] = new Quest_objective();
		objectives[i]->Load(qstream);
	}
	qstream >> aux;
	if (aux != ";;") {
		std::cout << "Quest file corrupted!";
		exit(2);
	}
}

Quest_flag::Quest_flag() {
	counter = 1;
}

void Quest_objective::Show() {
	std::cout << WordWrap("- "+description,0);

}

void Generic_quest::Show(int order_no) {
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

void Generic_quest::Refresh() {
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

void Chain_quest::Load(std::ifstream& qstream) {
	std::string aux;
	qstream >> aux;
	qstream.ignore();
	getline(qstream, name, '\n');
	getline(qstream, description, '\n');
	qstream >> nextquest_id;
	qstream >> num_objectives;
	qstream >> aux;
	if (aux != "OBJECTIVES:") {
		std::cout << "Quest file corrupted!";
		exit(1);
	}
	for (int i = 0; i < num_objectives; i++) {
		objectives[i] = new Quest_objective();
		objectives[i]->Load(qstream);
	}
	qstream >> aux;
	if (aux != ";;") {
		std::cout << "Quest file corrupted!";
		exit(2);
	}
}

void Chain_quest::Refresh() {
	int done = 1;
	if (status == IN_PROGRESS || status == ACTIVE) {
		for (int i = 0; i < num_objectives; i++) {
			objectives[i]->Refresh();
		}
		for (int i = 0; i < num_objectives && done == 1; i++) {
			done = objectives[i]->Finished();
		}
		if (done == 1) {
			status = FINISHED;
			game.Get_quest_by_ID(nextquest_id)->Take();
			game.Get_main_char()->Add_quest(game.Get_quest_by_ID(nextquest_id));
		}
	}
}