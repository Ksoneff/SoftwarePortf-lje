#include "Weapons.h"

Weapons::Weapons(string n, int s, int sm, int h, int p) {
	name = n;
	skade = s;
	styrkemodifier = sm;
	holdbarhed = h;
	price = p;
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

Weapons::~Weapons() {}
