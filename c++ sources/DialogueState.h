#ifndef DIALOGUESTATE_H
#define DIALOGUESTATE_H

#define MAX_OPT 10
#define EXIT 1
#define NON_EXIT 0

#include "Quest.h"

class DialogueState;

struct DialogueOption
{
	std::string text;
	int exit;
	DialogueState* next;
	DialogueOption() {
		text = "";
		exit = EXIT;
		next = nullptr;
	}
};

class DialogueState
{
	std::string text;
	int num_options,num_flags;
	Quest_flag* flags[MAX_FLAGS];		//flags that can be set by reaching the dialogue state
	Quest* quest;						//the quest that will be started when reaching the dialogue state, only one
	DialogueOption* options[MAX_OPT];
public:
	DialogueState() {
		num_flags = 0;
		quest = nullptr;
		text = "";
		num_options = MAX_OPT;
		for (int i = 0; i < MAX_OPT; i++) {
			options[i] = nullptr;
		}
	}
	void SetFlags();
	void Load(std::ifstream&,int);
	void Enter_dialogue(std::string);
};

#endif;
