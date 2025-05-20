#ifndef MONSTER_H
#define MONSTER_H

#include <string>
using namespace std;

class Monster
{
public:
    // Constructer and destructer
    Monster();
    ~Monster();
    Monster(string, int, int, int);

    // Get methods for viewing private attributes
    string getName();
    int getHP();
    int getDamage();
    int getXP();

private:
    string name;
    int HP;
    int damage;
    int XP;
};

#endif

