#include "CaveFactory.h"
#include <iostream>

vector<Monster*> CaveFactory::createCave(int heroLvl) {
    vector<Monster*> cave;
    MonsterFactory factory;

    if (heroLvl < 5) {
        cout << "You are not ready to explore the vast caves of moria yet. Reach level 5 first." << endl;
        return cave;
    }

    else if (heroLvl < 7) {
        cave.push_back(factory.create(HEST));
        cave.push_back(factory.create(HEST));
        cave.push_back(factory.create(WEAKGOBLIN));
    }
    else if (heroLvl < 9) {
        cave.push_back(factory.create(HEST));
        cave.push_back(factory.create(STRONGGOBLIN));
        cave.push_back(factory.create(WEAKGOBLIN));
        cave.push_back(factory.create(STRONGERGOBLIN));
        cave.push_back(factory.create(STRONGERGOBLIN));
    }
    else if (heroLvl < 11) {
        cave.push_back(factory.create(WEAKGOBLIN));
        cave.push_back(factory.create(STRONGERGOBLIN));
        cave.push_back(factory.create(STRONGERGOBLIN));
        cave.push_back(factory.create(THESTRONGESTGOBLIN));
        cave.push_back(factory.create(HEST));
        cave.push_back(factory.create(THESTRONGESTGOBLIN));
    }
    else if (heroLvl < 15) {
        cave.push_back(factory.create(STRONGGOBLIN));
        cave.push_back(factory.create(STRONGERGOBLIN));
        cave.push_back(factory.create(THESTRONGESTGOBLIN));
        cave.push_back(factory.create(UNICORN));
        cave.push_back(factory.create(HEST));
        cave.push_back(factory.create(THESTRONGESTGOBLIN));
        cave.push_back(factory.create(APEKING));
    }
    else {
        cave.push_back(factory.create(THESTRONGESTGOBLIN));
        cave.push_back(factory.create(APEKING));
        cave.push_back(factory.create(UNICORN));
        cave.push_back(factory.create(THESTRONGESTGOBLIN));
        cave.push_back(factory.create(UNICORN));
        cave.push_back(factory.create(APEKING));
        cave.push_back(factory.create(DRAGON));
    }
    return cave;
}

void CaveFactory::printCaveMonsters(const vector<Monster*>& monsters) {
    cout << "Monsters in the cave:" << endl;
    for (Monster* m : monsters) {
        cout << "- " << m->getName() << " (HP: " << m->getHP()
            << ", Damage: " << m->getDamage()
            << ", XP: " << m->getXP() << ")" << endl;
    }
}
