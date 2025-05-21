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
	// Standard constructers and destructer
	Controller();
	~Controller();
	Controller(string n);

	// Basic functions
	void showRules();
	void saveGame(Hero& h);
	Hero loadGame(int);

	// Functions used in battles
	Hero fightMonster(char, vector<Monster*>);
	Hero updateLevel(int);
	Hero battleCave(int heroLvl, vector<Monster*> caveMonsters);

	// Functions for weapon handling
	vector<Weapons*> createArmory();
	void seeWeaponsInArmory(vector<Weapons*> weapons);
	void buyWeapon(char choice, int heroGold, vector<Weapons*>& weapons);

	// Function used to analyze the game
	void analyzeGame();

private:
	Hero h;
	string monster;

	// Object of the database communication class, which takes a path to where the database is located
	DatabaseCommunication dbc = DatabaseCommunication("/home/sammy/SoftwarePortf-lje/game.db"); // set path here
};

#endif

