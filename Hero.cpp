#include "Hero.h"
#include <iostream>

Hero::Hero(){}

Hero::Hero(string n, int hp, int lvl, int xp, int d, int g, int is) {
	name = n;
	HP = hp;
	Level = lvl;
	XP = xp;
	damage = d;
	gold = g;
	inventorySpace = is;
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

void Hero::setGold(int g) {
	gold = g;
}

int Hero::getRemainingInventorySpace() {
	return inventorySpace - heroWeapons.size();
}

void Hero::showInventory() {
	for (size_t i = 0; i < heroWeapons.size(); ++i) 
	{
		cout << "Weapon at inventory index: " << i << " is:  " << heroWeapons[i]->getName() << endl;
	}
}

void Hero::deleteInventorySlot(int choice) {
	if (choice >= 0 && choice < heroWeapons.size()) {

		heroWeapons.erase(heroWeapons.begin() + choice);

		cout << "Weapon removed from inventory slot " << choice << "." << endl;
	}
	else {
		cout << "Invalid inventory slot." << endl;
	}
}


void Hero::addWeaponToInventory(Weapons* weapon) {
	if (heroWeapons.size() < inventorySpace) {
		heroWeapons.push_back(weapon);
	}
	else {
		cout << "Inventory is full. Cannot add " << weapon->getName() << "." << endl;
	}
}

vector<Weapons*>& Hero::getWeapons() {
	return heroWeapons;
}

Hero::~Hero(){}
