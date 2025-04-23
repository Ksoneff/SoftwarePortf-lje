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
	~Controller();

private:
	vector<Hero> heroes;
	string hero;
	string monster;
};

#endif
