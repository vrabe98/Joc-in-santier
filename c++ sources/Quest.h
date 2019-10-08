#pragma once

#define NOT_TAKEN 0
#define IN_PROGRESS 1
#define ACTIVE 2
#define RECENTLY_FINISHED 3
#define FINISHED 4

#define OBJECTIVE_IN_PROGRESS 0
#define OBJECTIVE_COMPLETE 1

#define MAX_FLAGS 100
#define MAX_OBJECTIVES 100

class Quest_flag {
	int counter;
	std::string name;
public:
	Quest_flag();
	void Refresh();
	void Load(std::ifstream&);
	inline std::string Get_name() {
		return name;
	}
	inline int Get_counter() {
		return counter;
	}
	void Increment_counter() {
		counter++;
	}
};

class Quest_objective {
	int state, num_flags;
	std::string description;
	Quest_flag* needed_flags[MAX_FLAGS];
public:
	void Load(std::ifstream&);
	inline int Finished() {
		return state;
	}
	void Show();
	void Refresh();
	void UpdateState();
};

class Quest
{
	int status,num_objectives;
	std::string description, name;
	Quest_objective* objectives[MAX_OBJECTIVES];
public:
	void Show(int);
	void Load(std::ifstream&);
	void Refresh();
	inline void Take() {
		status = IN_PROGRESS;
	}
};

