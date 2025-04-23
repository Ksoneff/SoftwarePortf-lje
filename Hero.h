#ifndef HERO_H
#define HERO_H

#include <string>

using namespace std;

class Hero
{
public:
	Hero();
	Hero(string, int, int, int, int);
	string getName();
	int getLevel();
	int getXP();
	int getHP();
	int getDamage();
	~Hero();
private:
	string name;
	int damage;
	int HP;
	int Level;
	int XP;
};

#endif

