#include "DatabaseCommunication.h"
#include "Hero.h"
#include "Weapons.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

DatabaseCommunication::DatabaseCommunication(const QString& path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
}

bool DatabaseCommunication::open() {
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    qDebug() << "Database opened successfully.";
    return true;
}

void DatabaseCommunication::close() {
    if (db.isOpen()) {
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        qDebug() << "Database connection closed.";
    }
}

void DatabaseCommunication::insertHero(Hero& hero) {
    if (!db.isOpen()) return;

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO Hero (
            name, hp, lvl, xp, damage, gold,
            inventoryspace, equippedbonusdamage,
            kills, weapon_id
        )
        VALUES (
            :name, :hp, :lvl, :xp, :damage, :gold,
            :inventoryspace, :equippedbonusdamage,
            :kills, :weapon_id
        )
    )");

    query.bindValue(":name", QString::fromStdString(hero.getName()));
    query.bindValue(":hp", hero.getHP());
    query.bindValue(":lvl", hero.getLevel());
    query.bindValue(":xp", hero.getXP());
    query.bindValue(":damage", hero.getDamage());
    query.bindValue(":gold", hero.getGold());
    query.bindValue(":inventoryspace", hero.getRemainingInventorySpace());
    query.bindValue(":equippedbonusdamage", hero.getEquippedBonusDamage());
    query.bindValue(":kills", 0);

    int weaponId = hero.hasWeaponEquipped() && hero.getSelectedWeapon()
                   ? hero.getSelectedWeapon()->getWeapon_id()
                   : -1;
    if (weaponId == -1)
        query.bindValue(":weapon_id", QVariant(QVariant::Int));  // NULL
    else
        query.bindValue(":weapon_id", weaponId);

    if (!query.exec()) {
        qDebug() << "Failed to insert hero:" << query.lastError().text();
        return;
    }

    int heroId = query.lastInsertId().toInt();

    const auto& weapons = hero.getWeapons();
    for (int i = 0; i < weapons.size(); ++i) {
        QSqlQuery invQuery;
        invQuery.prepare(R"(
            INSERT INTO Inventory (hero_id, weapon_id, slot)
            VALUES (:hero_id, :weapon_id, :slot)
        )");
        invQuery.bindValue(":hero_id", heroId);
        invQuery.bindValue(":weapon_id", weapons[i]->getWeapon_id());
        invQuery.bindValue(":slot", i);
        invQuery.exec();
    }
}

Hero DatabaseCommunication::loadHero(int heroId) {
    if (!db.isOpen()) return Hero();

    QSqlQuery query;
    query.prepare("SELECT name, hp, lvl, xp, damage, gold, inventoryspace, equippedbonusdamage, weapon_id FROM Hero WHERE hero_id = :hero_id");
    query.bindValue(":hero_id", heroId);

    if (!query.exec() || !query.next()) return Hero();

    QString name = query.value(0).toString();
    int hp = query.value(1).toInt();
    int lvl = query.value(2).toInt();
    int xp = query.value(3).toInt();
    int damage = query.value(4).toInt();
    int gold = query.value(5).toInt();
    int inventorySpace = query.value(6).toInt();
    int equippedBonus = query.value(7).toInt();
    int equippedWeaponId = query.value(8).isNull() ? -1 : query.value(8).toInt();

    Weapons* equippedWeapon = nullptr;
    std::vector<Weapons*> heroWeapons;

    QSqlQuery inventoryQuery;
    inventoryQuery.prepare("SELECT weapon_id FROM Inventory WHERE hero_id = :hero_id ORDER BY slot ASC");
    inventoryQuery.bindValue(":hero_id", heroId);

    if (inventoryQuery.exec()) {
        while (inventoryQuery.next()) {
            int wpnId = inventoryQuery.value(0).toInt();

            QSqlQuery weaponQuery;
            weaponQuery.prepare("SELECT name, skade, styrkemodifier, holbarhed, price, kills FROM Weapon WHERE weapon_id = :weapon_id");
            weaponQuery.bindValue(":weapon_id", wpnId);

            if (weaponQuery.exec() && weaponQuery.next()) {
                Weapons* w = new Weapons(
                    wpnId,
                    weaponQuery.value(0).toString().toStdString(),
                    weaponQuery.value(1).toInt(),
                    weaponQuery.value(2).toInt(),
                    weaponQuery.value(3).toInt(),
                    weaponQuery.value(4).toInt(),
                    weaponQuery.value(5).toInt()
                );
                heroWeapons.push_back(w);
                if (wpnId == equippedWeaponId)
                    equippedWeapon = w;
            }
        }
    }

    return Hero(
        name.toStdString(),
        lvl,
        xp,
        hp,
        damage,
        gold,
        inventorySpace,
        equippedBonus,
        heroWeapons,
        equippedWeapon
    );
}

void DatabaseCommunication::insertWeapon(Weapons& weapon) {
    if (!db.isOpen()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO Weapon (weapon_id, name, skade, styrkemodifier, holbarhed, price, kills) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(weapon.getWeapon_id());
    query.addBindValue(QString::fromStdString(weapon.getName()));
    query.addBindValue(weapon.getSkade());
    query.addBindValue(weapon.getStyrkemodifier());
    query.addBindValue(weapon.getHoldbarhed());
    query.addBindValue(weapon.getPrice());
    query.addBindValue(0);
    query.exec();
}

std::vector<Weapons*> DatabaseCommunication::loadWeaponsForHero(int heroId) {
    std::vector<Weapons*> weapons;
    QSqlQuery query;

    query.prepare("SELECT w.weapon_id, w.name, w.skade, w.styrkemodifier, w.holbarhed, w.price, w.kills FROM weapon w JOIN Inventory i ON w.weapon_id = i.weapon_id WHERE i.hero_id = ?");
    query.addBindValue(heroId);

    if (query.exec()) {
        while (query.next()) {
            Weapons* w = new Weapons(
                query.value(0).toInt(),
                query.value(1).toString().toStdString(),
                query.value(2).toInt(),
                query.value(3).toInt(),
                query.value(4).toInt(),
                query.value(5).toInt(),
                query.value(6).toInt()
            );
            weapons.push_back(w);
        }
    }
    return weapons;
}
