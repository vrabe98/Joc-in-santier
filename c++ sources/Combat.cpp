#include <iostream>
#include <random>
#include <Windows.h>
#include "Character.h"
#include "Combat.h"


int Roll_d20() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution <int> distrib(ROLL_MIN, ROLL_MAX);
	return distrib(gen);
}

void Round(Character* attacker, Character* defender,int turn) {
	double attacker_roll, crit = 1, glance = 1, hit = 0,counter=1;
	int dualwield = attacker->Dualwield()+1;		//the do...while doesn't work well without this
	std::cout << "\n\nTurn " << turn;
	do {
		std::cout << "\nAttacker: " << attacker->GetName() << ". HP: " << attacker->GetHP() << ". Daune " << attacker->GetWeaponDmg(RHAND+dualwield-1).type << " mana curenta: " << attacker->GetWeaponDmg(RHAND+dualwield-1).damage << "\n";
		std::cout << "Defender: " << defender->GetName() << ". HP: " << defender->GetHP() << ". Daune " << defender->GetWeaponDmg(RHAND).type << " mana dreapta: " << defender->GetWeaponDmg(RHAND).damage << "\n" << std::endl;
		attacker_roll = Roll_d20() + (attacker->DEX() - 10.0) * 0.5 + attacker->Has1h() * 0.5 - defender->GetEvasion();
		if (attacker_roll < 0) attacker_roll = 0;
		if (attacker_roll > 20) attacker_roll = 20;
		std::cout << attacker->GetName() << " rolls " << attacker_roll << std::endl << std::endl;
		if (attacker_roll == 0) {
			std::cout << attacker->GetName() << " makes a bad mistake.\n";
			defender->SetDmgBonus();
			hit = 0;
		}
		else if (attacker_roll < 5) {
			std::cout << attacker->GetName() << " misses " << defender->GetName() << " by an inch.\n";
			hit = 0;
		}
		else if (attacker_roll < 10) {
			std::cout << attacker->GetName() << " almost misses, but somehow manages to scratch " << defender->GetName() << ".\n";
			glance = 0.5;
			hit = 1;
		}
		else if (attacker_roll < 19) {
			std::cout << attacker->GetName() << " hits " << defender->GetName() << ". Boring.\n";
			hit = 1;
		}
		else if ((attacker_roll == 19) || (attacker_roll == 20)) {
			std::cout << attacker->GetName() << " hits " << defender->GetName() << "'s weak spot. Blood flies everywhere! Eugh, I need to wash. Damn it, " << attacker->GetName() << " I'm wearing my new shirt!\n";
			hit = 1;
			crit = 1.5;
		}
		if (hit) {
			float damage = attacker->GetWeaponDmg(RHAND+dualwield-1).damage * (1.0f + 0.1f * (attacker->STR() - 10.0f)) * crit * glance * (1.0f + attacker->HasDmgBonus() * 0.2f);
			if (crit == 1) defender->GetDamaged(damage, attacker, 0, 0);
			else if (crit == 1.5) defender->GetDamaged(damage, attacker, 0, 1);
		}
		if (attacker->HasDmgBonus())	attacker->ResetDmgBonus();
		dualwield--;
	} while (dualwield);
}

int Combat(Character* me, Character* enemy) {
	int turn_number = 0,someonedied;
	double initiative_roll_mainchar, initiative_roll_enemy;
	Character* attacker=nullptr;
	Character* defender=nullptr;
	system("cls");
	initiative_roll_mainchar = Roll_d20() + (me->DEX()-10.0) * 0.25;
	initiative_roll_enemy = Roll_d20() + (enemy->DEX()-10.0) * 0.25;
	if (initiative_roll_mainchar >= initiative_roll_enemy) {
		std::cout << "Inamicul te-a vazut prea tarziu! Ataci primul.";
		attacker = me;
		defender = enemy;
	}
	else if (initiative_roll_mainchar < initiative_roll_enemy) {
		std::cout << "Inamicul te-a vazut inainte sa ataci! El va ataca primul.";
		attacker = enemy;
		defender = me;
	}
	Round(attacker, defender,turn_number);		//round 0 is mandatory
	someonedied = me->died() || enemy->died();
	while(!someonedied){
		char action;
		std::cout << "Mai vrei sa dai cu sabia-n dusman? Sau ti-a ajuns? (a-attack/x-exit)\n";
		std::cin >> action;
		if (action == 'a'&&(!someonedied)) {
			turn_number++;
			if (turn_number % 2) {
				Round(defender, attacker, turn_number);
			}
			else Round(attacker, defender, turn_number);
		}
		else if (action=='x'&&(!someonedied)){
			std::cout << "Lasul de tine n-a putut sa se bata nici macar " << turn_number + 1 << " runde. Rusine...\n";
			return 2;
		}
		std::cin.get();
		someonedied = me->died() || enemy->died();
	}
	std::cin.get();
	if (me->died()) return 0;
	else return 1;
}

void DeathScreen(Character* me,Character* killer) {
	system("cls");
	std::cout << "Cu o ultima lovitura, "<<killer->GetName()<<" il doboara pe "<<me->GetName()<<". Astfel, povestea celui din urma se sfarseste intr-un mod deosebit de tragic. Dc imi dau lacrimi? Sclav! Baga...baga un servetel aicea...\n Doamnelor si domnilor, va trebui acum sa imi iau ramas bun de la dumneavoastra.\n\nYOU DIED\n";
	std::cin.get();
	exit(1);
}