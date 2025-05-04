#ifndef APEKING_H
#define APEKING_H

#include "Monster.h"

class ApeKing : public Monster {
public:
	ApeKing(string name, int hp, int damage, int xp);
	~ApeKing();
};

#endif
