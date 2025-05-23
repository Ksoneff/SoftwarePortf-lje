#ifndef DATABASECOMMUNICATION_H
#define DATABASECOMMUNICATION_H

// Query libraries for communicating with sql database
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <vector>
#include "Hero.h"
#include "Weapons.h"

class DatabaseCommunication
{
public:
    // Basic nescisarry functions
    DatabaseCommunication(const QString& path);
    bool open();
    void close();

    // For inserting, loading, equipping, unequipping etc.
    void equipWeapon(Weapons* selectedWeapon, Hero& hero);
    void unequipWeapon(Hero& hero);
    void insertHeroWeapons(vector<Weapons*> heroWeapons, Hero& hero);
    void insertHero(Hero& hero);
    Hero loadHero(int heroId);

    // Shows heroes
    void showHeroes();

    // For analyzing game
    void showHeroesABC();
    void showHeroKills();
    bool showHeroWeaponKills(char hero_id);
    void showWeaponTypeKillsLeader();

private:
    QSqlDatabase db;
};

#endif