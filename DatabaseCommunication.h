#ifndef DATABASECOMMUNICATION_H
#define DATABASECOMMUNICATION_H

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

    // For inserting, loading etc.
    void insertHero(Hero& hero);
    Hero loadHero(int heroId);
    void showHeroes();

    // For analyzing game
    void showHeroesABC();
    void showHeroKills();
    bool showHeroWeaponKills(char hero_id);
    void showWeaponTypeKillsLeader();

private:
    QSqlDatabase db;
};

#endif // DATABASECOMMUNICATION_H