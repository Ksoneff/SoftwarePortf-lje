#include "Hero.h"

Hero::Hero(){}

Hero::Hero(string n, int hp, int lvl, int xp, int d, int g) {
	name = n;
	HP = hp;
	Level = lvl;
	XP = xp;
	damage = d;
	gold = g;
}

string Hero::getName() {
	return name;
}

int Hero::getLevel() {
	return Level;
}

int Hero::getHP() {
	return HP;
}

int Hero::getXP() {
	return XP;
}

int Hero::getDamage() {
	return damage;
}

int Hero::getGold() {
	return gold;
}

Hero::~Hero(){}

