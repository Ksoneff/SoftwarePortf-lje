#ifndef DRAGON_H
#define DRAGON_H

#include "Monster.h"

class Dragon : public Monster {
public:
	Dragon(string name, int hp, int damage, int xp);
	~Dragon();
};

#endif
