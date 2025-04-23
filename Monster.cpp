#include "Monster.h"

Monster::Monster() {}

Monster::Monster(string n, int hp, int d, int xp) {
    name = n;
    HP = hp;
    damage = d;
    XP = xp;
}

Monster::~Monster() {}

string Monster::getName() {
    return name;
}

int Monster::getHP() {
    return HP;
}

int Monster::getDamage() {
    return damage;
}

int Monster::getXP() {
    return XP;
}
