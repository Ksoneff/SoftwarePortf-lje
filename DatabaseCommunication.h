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
    DatabaseCommunication(const QString& path);
    bool open();
    void close();

    void insertHero(Hero& hero);
    Hero loadHero(int heroId);

    void insertWeapon(Weapons& weapon);
    std::vector<Weapons*> loadWeaponsForHero(int heroId);

private:
    QSqlDatabase db;
};

#endif // DATABASECOMMUNICATION_H