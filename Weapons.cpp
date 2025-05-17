#include "Weapons.h"

Weapons::Weapons(int t_id, string n, int s, int sm, int h, int p, int k) {
	type_id = t_id;
	name = n;
	skade = s;
	styrkemodifier = sm;
	holdbarhed = h;
	price = p;
	kills = k;
}

int Weapons::getType_id() {
	return type_id;
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

void Weapons::setHoldbarhed(int newHoldbarhed) {
	holdbarhed = newHoldbarhed;
}

Weapons::~Weapons() {}
