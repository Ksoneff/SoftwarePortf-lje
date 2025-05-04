#ifndef THESTRONGESTGOBLIN_H
#define THESTRONGESTGOBLIN_H

#include "Monster.h"

class TheStrongestGoblin : public Monster {
public:
    TheStrongestGoblin(string name, int hp, int damage, int xp);
    ~TheStrongestGoblin();
};

#endif
