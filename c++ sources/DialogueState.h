#ifndef DIALOGUESTATE_H
#define DIALOGUESTATE_H

#define MAX_OPT 10
#define EXIT 1
#define NON_EXIT 0

class DialogueState;

class DialogueOption
{
public:
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
	int num_options;
	DialogueOption* options[MAX_OPT];
public:
	DialogueState() {
		text = "";
		num_options = MAX_OPT;
		for (int i = 0; i < MAX_OPT; i++) {
			options[i] = nullptr;
		}
	}
	void Load(std::ifstream&,int);
	void Enter_dialogue(std::string);
};

#endif;
