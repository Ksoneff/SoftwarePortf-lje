#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Hero.h"
#include "Monster.h"
#include "WeakGoblin.h"
#include "StrongGoblin.h"
#include "Hest.h"
#include "StrongerGoblin.h"
#include "TheStrongestGoblin.h"
#include "ApeKing.h"
#include "Unicorn.h"
#include "Dragon.h"
#include "Weapons.h"
#include <string>
#include <vector>

using namespace std;

class Controller
{
public:
	Controller();
	Controller(string n);
	void showRules();
	string showHeroes();
	void saveGame(Hero);
	Hero loadGame(string);
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
};

#endif

