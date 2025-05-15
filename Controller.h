#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Hero.h"
#include "Monster.h"
#include "Weapons.h"
#include "DatabaseCommunication.h"
#include <string>
#include <vector>

using namespace std;

class Controller
{
public:
	Controller();
	Controller(string n);
	void showRules();
	void saveGame(Hero);
	Hero loadGame(int);
	Hero fightMonster(char, Hero, vector<Monster*>);
	Hero updateLevel(int, Hero);
	Hero battleCave(int heroLvl, Hero h, vector<Monster*> caveMonsters);
	vector<Weapons*> createArmory();
	void seeWeaponsInArmory(vector<Weapons*> weapons);
	void buyWeapon(char choice, int heroGold, vector<Weapons*>& weapons, Hero& h);
	~Controller();

private:
	string hero;
	string monster;
	DatabaseCommunication dbc = DatabaseCommunication("/home/sammy/SoftwarePortf-lje/game.db"); // set path here

};

#endif

