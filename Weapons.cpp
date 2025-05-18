#include "Weapons.h"

Weapons::Weapons(int t_id, string n, int s, int sm, int h, int p, int k, int w_id) {
	type_id = t_id;
	name = n;
	skade = s;
	styrkemodifier = sm;
	holdbarhed = h;
	price = p;
	kills = k;
	weapon_id = w_id;
}

int Weapons::getType_id() {
	return type_id;
}

int Weapons::getWeapon_id() {
	return weapon_id;
}

int Weapons::getHoldbarhed() {
	return holdbarhed;
}

int Weapons::getSkade() {
	return skade;
}

int Weapons::getStyrkemodifier() {
	return styrkemodifier;
}

string Weapons::getName() {
	return name;
}

int Weapons::getPrice() {
	return price;
}

int Weapons::getKills() {
	return kills;
}

void Weapons::setWeapon_id(int w_id) {
	weapon_id = w_id;
}

void Weapons::setHoldbarhed(int newHoldbarhed) {
	holdbarhed = newHoldbarhed;
}

void Weapons::incrementKills() {
	kills = getKills() + 1;
}

Weapons::~Weapons() {}
