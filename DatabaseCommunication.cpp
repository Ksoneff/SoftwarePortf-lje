#include "DatabaseCommunication.h"
#include "Hero.h"
#include "Weapons.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <iostream>

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
        qDebug() << "Database connection closed.";
    }
}


void DatabaseCommunication::insertHero(Hero& hero) {

    if (!db.isOpen()) {
        std::cerr << "insertHero() aborted: database is not open!" << std::endl;
        return;
    }

    bool isNewHero = (hero.getHeroID() == -1);

    QString sql;

    if (isNewHero) {
        // Insert without hero_id (auto-generated)
        sql = R"(
            INSERT INTO Hero (
                name, hp, lvl, xp, damage, gold,
                inventoryspace, equippedbonusdamage, kills, weapon_id
            )
            VALUES (
                :name, :hp, :lvl, :xp, :damage, :gold,
                :inventoryspace, :equippedbonusdamage, :kills, :weapon_id
            )
        )";
    }
    else {
        // Replace existing hero using hero_id
        cout << "Replacing old hero with new updated hero" << endl;
        sql = R"(
            INSERT OR REPLACE INTO Hero (
                hero_id, name, hp, lvl, xp, damage, gold,
                inventoryspace, equippedbonusdamage, kills, weapon_id
            )
            VALUES (
                :hero_id, :name, :hp, :lvl, :xp, :damage, :gold,
                :inventoryspace, :equippedbonusdamage, :kills, :weapon_id
            )
        )";
    }

    QSqlQuery query;
    query.prepare(sql);

    if (!isNewHero)
        query.bindValue(":hero_id", hero.getHeroID());

    query.bindValue(":name", QString::fromStdString(hero.getName()));
    query.bindValue(":hp", hero.getHP());
    query.bindValue(":lvl", hero.getLevel());
    query.bindValue(":xp", hero.getXP());
    query.bindValue(":damage", hero.getDamage());
    query.bindValue(":gold", hero.getGold());
    query.bindValue(":inventoryspace", hero.getRemainingInventorySpace());
    query.bindValue(":equippedbonusdamage", hero.getEquippedBonusDamage());
    query.bindValue(":kills", hero.getKills());

    int weaponId = hero.hasWeaponEquipped() && hero.getSelectedWeapon()
        ? hero.getSelectedWeapon()->getWeapon_id()
        : -1;
    if (weaponId == -1)
        query.bindValue(":weapon_id", QVariant(QVariant::Int));  // NULL
    else
        query.bindValue(":weapon_id", weaponId);

    if (!query.exec()) {
        qDebug() << "Failed to insert/replace hero:" << query.lastError().text();
        return;
    }

    // If new hero, get the generated ID
    if (isNewHero) {
        int newHeroId = query.lastInsertId().toInt();
        hero.setHeroID(newHeroId);
    }

    // Handle Inventory: Replace entire inventory for this hero
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM Inventory WHERE hero_id = :hero_id");
    deleteQuery.bindValue(":hero_id", hero.getHeroID());
    deleteQuery.exec();  // Clean up old inventory if exists

    const auto& weapons = hero.getWeapons();
    for (int i = 0; i < weapons.size(); ++i) {
        QSqlQuery invQuery;
        invQuery.prepare(R"(
            INSERT INTO Inventory (hero_id, weapon_id, slot)
            VALUES (:hero_id, :weapon_id, :slot)
        )");
        invQuery.bindValue(":hero_id", hero.getHeroID());
        invQuery.bindValue(":weapon_id", weapons[i]->getWeapon_id());
        invQuery.bindValue(":slot", i);
        invQuery.exec();
    }
}

Hero DatabaseCommunication::loadHero(int heroId) {
    if (!db.isOpen()) return Hero();

    QSqlQuery query;
    query.prepare(R"(
        SELECT hero_id, name, hp, lvl, xp, damage, gold,
               inventoryspace, equippedbonusdamage, kills, weapon_id
        FROM Hero
        WHERE hero_id = :hero_id
    )");
    query.bindValue(":hero_id", heroId);

    if (!query.exec() || !query.next()) return Hero();

    int hero_id = query.value(0).toInt();
    QString name = query.value(1).isNull() ? "" : query.value(1).toString();
    int hp = query.value(2).toInt();
    int lvl = query.value(3).toInt();
    int xp = query.value(4).toInt();
    int damage = query.value(5).toInt();
    int gold = query.value(6).toInt();
    int inventorySpace = query.value(7).toInt();
    int equippedBonus = query.value(8).toInt();
    int kills = query.value(9).toInt();
    int equippedWeaponId = query.value(10).isNull() ? -1 : query.value(10).toInt();

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
        hp,
        lvl,
        xp,
        damage,
        gold,
        inventorySpace,
        equippedBonus,
        heroWeapons,
        kills,
        equippedWeapon,
        hero_id
    );
}

void DatabaseCommunication::showHeroes() {
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    QSqlQuery query;
    if (!query.exec("SELECT hero_id, name, hp, lvl, xp, damage, gold, inventoryspace, equippedbonusdamage, weapon_id FROM Hero")) {
        qDebug() << "Failed to retrieve heroes:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int hero_id = query.value(0).toInt();
        QString qname = query.value(1).toString();
        std::string name = qname.toStdString();
        int hp = query.value(2).toInt();
        int lvl = query.value(3).toInt();
        int xp = query.value(4).toInt();
        int damage = query.value(5).toInt();
        int gold = query.value(6).toInt();
        int inventorySpace = query.value(7).toInt();
        int equippedBonus = query.value(8).toInt();
        int equippedWeaponId = query.value(9).isNull() ? -1 : query.value(9).toInt();

        std::cout << "Hero_id: " << hero_id
            << " | Name: " << name
            << " | HP: " << hp
            << " | Level: " << lvl
            << " | XP: " << xp
            << " | Damage: " << damage
            << " | Gold: " << gold
            << " | Inventory Space: " << inventorySpace
            << " | Equipped Bonus: " << equippedBonus
            << " | Weapon ID: " << equippedWeaponId
            << "\n";
    }
}
