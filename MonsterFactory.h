#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include <string>
#include "Hest.h"
#include "Monster.h"
#include "WeakGoblin.h"
#include "StrongGoblin.h"
#include "StrongerGoblin.h"
#include "TheStrongestGoblin.h"
#include "ApeKing.h"
#include "Unicorn.h"
#include "Dragon.h"

enum Type { HEST, WEAKGOBLIN, STRONGGOBLIN, STRONGERGOBLIN, THESTRONGESTGOBLIN, APEKING, UNICORN, DRAGON };

class MonsterFactory
{
public:
	Monster* create(Type t); // Factory method
};

#endif
