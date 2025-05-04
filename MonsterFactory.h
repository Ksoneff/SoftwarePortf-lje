#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include <string>
#include "MonsterType.h"
#include "Monster.h"

using namespace std;

class MonsterFactory
{
public:
	Monster* create(Type t); // Factory method
};

#endif

