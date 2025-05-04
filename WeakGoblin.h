#ifndef WEAKGOBLIN_H
#define WEAKGOBLIN_H

#include "Monster.h"

class WeakGoblin : public Monster {
public:
    WeakGoblin(string name, int hp, int damage, int xp);
    ~WeakGoblin();
};

#endif
