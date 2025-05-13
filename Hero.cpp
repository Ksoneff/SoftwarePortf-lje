#include "Hero.h"
#include <iostream>

Hero::Hero(){}

Hero::Hero(string n, int hp, int lvl, int xp, int d, int g, int is, int equippedBonusD, vector<Weapons*> heroweapons, Weapons* equipedWeapon) {
	name = n;
	HP = hp;
	Level = lvl;
	XP = xp;
	damage = d;
	gold = g;
	inventorySpace = is;
	selectedWeapon = equipedWeapon;
	heroWeapons = heroweapons;
	equippedBonusDamage = equippedBonusD;
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
	for (size_t i = 0; i < heroWeapons.size(); ++i) {
		if (heroWeapons[i] != nullptr) {
			cout << "Weapon at inventory index: " << i << " is: " << heroWeapons[i]->getName() << endl;
		}
		else {
			cout << "Weapon at inventory index: " << i << " is: nullptr (invalid pointer)" << endl;
		}
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
		Weapons* newWeapon = new Weapons(*weapon);
		heroWeapons.push_back(newWeapon);
	}
	else {
		cout << "Inventory is full. Cannot add " << weapon->getName() << "." << endl;
	}
}

vector<Weapons*>& Hero::getWeapons() {
	return heroWeapons;
}

void Hero::equipWeapon(int weaponChoice) {
	if (selectedWeapon != nullptr) {
		unequipWeapon();
	}

	selectedWeapon = heroWeapons[weaponChoice];
	equippedBonusDamage = selectedWeapon->getSkade() + (selectedWeapon->getStyrkemodifier() * damage);
	damage += equippedBonusDamage;

	cout << "Equipped weapon: " << selectedWeapon->getName() << endl;
	cout << "New hero damage: " << damage << " (+" << equippedBonusDamage << " from weapon)" << endl;
}


void Hero::unequipWeapon() {
	cout << "Unequipping weapon: " << selectedWeapon->getName() << endl;
	cout << "Removing bonus damage: " << equippedBonusDamage << endl;
	damage -= equippedBonusDamage;
	equippedBonusDamage = 0;
	selectedWeapon = nullptr;
}

bool Hero::hasWeaponEquipped() {
	return equippedBonusDamage != 0;
}

void Hero::removeWeaponDurability() {
	int holdbarhed = selectedWeapon->getHoldbarhed() - 2;
	if (holdbarhed < 0) holdbarhed = 0; // Avoid negative durability

	if (holdbarhed == 0) {
		cout << " " << endl;
		cout << "You weapon durability is = 0, and it has broken" << endl;
		cout << " " << endl;
		deleteEquippedWeapon();
		unequipWeapon();
		return;
	}
	selectedWeapon->setHoldbarhed(holdbarhed);
}

void Hero::deleteEquippedWeapon() {
	int index = 0;
	for (size_t i = 0; i < heroWeapons.size(); ++i) 
	{
		if (selectedWeapon->getName() == heroWeapons[i]->getName())
		{
			break;
		}
		index++;
	}
	heroWeapons.erase(heroWeapons.begin() + index);
}

int Hero::getEquippedBonusDamage() {
	return equippedBonusDamage;
}

Weapons* Hero::getSelectedWeapon() {
	return selectedWeapon;
}



Hero::~Hero(){}
