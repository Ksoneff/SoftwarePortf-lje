#ifndef MONSTER_H
#define MONSTER_H

#include <string>
using namespace std;

class Monster
{
public:
    Monster();
    Monster(string, int, int, int);
    virtual string getName();
    virtual int getHP();
    virtual int getDamage();
    virtual int getXP();
    ~Monster();

protected:
    string name;
    int HP;
    int damage;
    int XP;
};

#endif

