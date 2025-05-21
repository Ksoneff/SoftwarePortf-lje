#ifndef HERO_H
#define HERO_H

#include <string>
#include <vector>
#include "Weapons.h"

using namespace std;

class Hero
{
public:
	// Default constructers and destructor
	Hero();
	Hero(string, int, int, int, int, int, int, int equippedBonusDamage, vector<Weapons*> heroWeapons = {}, int kills = 0, Weapons* equipedWeapon = nullptr, int hero_id = -1);
	~Hero();

	// Get methods for private attributes
	int getHeroID();
	string getName();
	int getLevel();
	int getXP();
	int getHP();
	int getDamage();
	int getGold();
	int getKills();
	int getRemainingInventorySpace();
	int getEquippedBonusDamage();
	Weapons* getSelectedWeapon();
	vector<Weapons*>& getWeapons();

	// Set methods
	void setGold(int g);
	void setHeroID(int h_id);

	// Handling hero inventory
	void showInventory();
	void deleteInventorySlot(int choice);
	void addWeaponToInventory(Weapons* weapon);
	int showRemaingInventorySpace();

	// Handling equipped weapon
	void equipWeapon(int weaponChoice);
	void unequipWeapon();
	bool hasWeaponEquipped();
	void removeWeaponDurability();
	void deleteEquippedWeapon();

private:
	string name;
	int damage;
	int HP;
	int Level;
	int XP;
	int gold;
	int inventorySpace = 6;
	int equippedBonusDamage;
	int kills = 0;
	Weapons* selectedWeapon;
	int hero_id = -1; // Initialize to -1 so the database communications class knows when its a new hero

	vector<Weapons*> heroWeapons;
};

#endif

