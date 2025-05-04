#ifndef CAVEFACTORY_H
#define CAVEFACTORY_H

#include <vector>
#include "Monster.h"
#include "MonsterType.h"
#include "MonsterFactory.h"

using namespace std;

class CaveFactory {
public:
    vector<Monster*> createCave(int heroLevel);
    void printCaveMonsters(const vector<Monster*>& monsters);
};

#endif
