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
	int counter, id;
	std::string name;
public:
	Quest_flag();
	void Refresh();
	void Load(std::ifstream&);
	inline int Get_ID() {
		return id;
	}
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
};

class Quest
{
protected:
	int status,num_objectives;
	std::string description, name;
public:
	virtual void Show(int)=0;
	virtual void Load(std::ifstream&)=0;
	virtual void Refresh()=0;
	inline void Take() {
		status = IN_PROGRESS;
	}
};

class Generic_quest:public Quest {
protected:
	Quest_objective* objectives[MAX_OBJECTIVES];
public:
	void Show(int);
	void Load(std::ifstream&);
	void Refresh();
};

class Chain_quest:public Generic_quest {
	int nextquest_id;
public:
	void Load(std::ifstream&);
	void Refresh();
};

class Umbrella_quest:public Quest {

};

