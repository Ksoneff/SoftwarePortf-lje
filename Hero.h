#ifndef HERO_H
#define HERO_H

#include <string>
#include <vector>
#include "Weapons.h"

using namespace std;

class Hero
{
public:
	Hero();
	Hero(string, int, int, int, int, int, int, int equippedBonusDamage, vector<Weapons*> heroWeapons = {}, Weapons* equipedWeapon = nullptr);
	string getName();
	int getLevel();
	int getXP();
	int getHP();
	int getDamage();
	int getGold();
	void setGold(int g);
	int getRemainingInventorySpace();
	void showInventory();
	void deleteInventorySlot(int choice);
	void addWeaponToInventory(Weapons* weapon);
	void equipWeapon(int weaponChoice);
	void unequipWeapon();
	bool hasWeaponEquipped();
	void removeWeaponDurability();
	void deleteEquippedWeapon();
	int getEquippedBonusDamage();
	Weapons* getSelectedWeapon();
	vector<Weapons*>& getWeapons();
	~Hero();
private:
	string name;
	int damage;
	int HP;
	int Level;
	int XP;
	int gold;
	int inventorySpace = 6;
	int equippedBonusDamage;
	Weapons* selectedWeapon;

	vector<Weapons*> heroWeapons;
};

#endif

