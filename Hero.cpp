#include "Hero.h"
#include <iostream>

Hero::Hero() {}

Hero::Hero(string n, int hp, int lvl, int xp, int d, int g, int is, int equippedBonusD, vector<Weapons*> heroweapons, int k, Weapons* equipedWeapon, int h_id) {
	name = n;
	HP = hp;
	Level = lvl;
	XP = xp;
	damage = d;
	gold = g;
	inventorySpace = is;
	selectedWeapon = equipedWeapon;
	heroWeapons = heroweapons;
	kills = k;
	equippedBonusDamage = equippedBonusD;
	hero_id = h_id;
}

int Hero::getHeroID() {
	return hero_id;
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

int Hero::getKills() {
	return kills;
}

int Hero::getRemainingInventorySpace() {
	return inventorySpace - heroWeapons.size();
}

// Displays heroes inventory
void Hero::showInventory() {
	// For every weapon in the heroes inventory, displays it or if there are none, doesnt display any
	for (size_t i = 0; i < heroWeapons.size(); ++i) {
		if (heroWeapons[i] != nullptr) {
			cout << "Weapon at inventory index: " << i << " is: " << heroWeapons[i]->getName() << endl;
		}
		else {
			cout << "Weapon at inventory index: " << i << " is: nullptr (invalid pointer)" << endl;
		}
	}
}

// Deletes weapon from a specific inventory slot, used in armory
void Hero::deleteInventorySlot(int choice) {
	if (choice >= 0 && choice < heroWeapons.size()) {

		heroWeapons.erase(heroWeapons.begin() + choice);

		cout << "Weapon removed from inventory slot " << choice << "." << endl;
	}
	else {
		cout << "Invalid inventory slot." << endl;
	}
}

// Adds a weapon to heroes inventory, used in armory in controller
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

// Equips a weapon
void Hero::equipWeapon(int weaponChoice) {

	// This check is nescissary, unequips previous weapon if hero has one already equipped
	if (selectedWeapon != nullptr) {
		unequipWeapon();
	}

	// Increments heroes damage based on equipped weapon
	selectedWeapon = heroWeapons[weaponChoice];
	equippedBonusDamage = selectedWeapon->getSkade() + (selectedWeapon->getStyrkemodifier() * damage);
	damage += equippedBonusDamage;

	// Information
	cout << "Equipped weapon: " << selectedWeapon->getName() << endl;
	cout << "New hero damage: " << damage << " (+" << equippedBonusDamage << " from weapon)" << endl;
}


// Unequips heroes current weapon
void Hero::unequipWeapon() {
	// Nescissary check
	if (selectedWeapon == nullptr) {
		cout << "No weapon is currently equipped." << endl;
		return;
	}

	selectedWeapon->setWeapon_id(-1);

	// Removes previously applied damage. This is also why hero has a private attribute: equippedBonusDamage
	cout << "Unequipping weapon: " << selectedWeapon->getName() << endl;
	cout << "Removing bonus damage: " << equippedBonusDamage << endl;
	damage -= equippedBonusDamage;
	equippedBonusDamage = 0;
	selectedWeapon = nullptr;
}

// Boolean function for checking if the hero has a weapon equipped
bool Hero::hasWeaponEquipped() {
	return equippedBonusDamage != 0;
}

// Simple function that removes the weapon durability and deletes it if durabillity becomes 0
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

// Deletes heroes equipped weapon, used in the remove Weapon durability function if heroes weapon breaks
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

void Hero::setHeroID(int h_id) {
	hero_id = h_id;
}



Hero::~Hero() {}
