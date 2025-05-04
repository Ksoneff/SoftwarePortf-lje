#ifndef STRONGGOBLIN_H
#define STRONGGOBLIN_H

#include "Monster.h"

class StrongGoblin : public Monster {
public:
	StrongGoblin(string name, int hp, int damage, int xp);
	~StrongGoblin();
};

#endif
