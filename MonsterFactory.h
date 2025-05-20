#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include <string>

// Uses monsterTpye and Monster class in create method
#include "MonsterType.h"
#include "Monster.h"

using namespace std;

class MonsterFactory
{
public:
	Monster* create(Type t); // Factory method
};

#endif

