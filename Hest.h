#ifndef HEST_H
#define HEST_H

#include "Monster.h"

class Hest : public Monster {
public:
    Hest(string name, int hp, int damage, int xp);
    ~Hest();
};

#endif


