#ifndef STRONGERGOBLIN_H
#define STRONGERGOBLIN_H

#include "Monster.h"

class StrongerGoblin : public Monster {
public:
	StrongerGoblin(string name, int hp, int damage, int xp);
	~StrongerGoblin();
};

#endif
