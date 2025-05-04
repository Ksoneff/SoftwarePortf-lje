#ifndef UNICORN_H
#define UNICORN_H

#include "Monster.h"

class Unicorn : public Monster {
public:
	Unicorn(string name, int hp, int damage, int xp);
	~Unicorn();
};

#endif
