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

    QSqlQuery heroQuery;
    heroQuery.prepare(sql);

    if (!isNewHero)
        heroQuery.bindValue(":hero_id", hero.getHeroID());

    heroQuery.bindValue(":name", QString::fromStdString(hero.getName()));
    heroQuery.bindValue(":hp", hero.getHP());
    heroQuery.bindValue(":lvl", hero.getLevel());
    heroQuery.bindValue(":xp", hero.getXP());
    heroQuery.bindValue(":damage", hero.getDamage());
    heroQuery.bindValue(":gold", hero.getGold());
    heroQuery.bindValue(":inventoryspace", hero.getRemainingInventorySpace());
    heroQuery.bindValue(":equippedbonusdamage", hero.getEquippedBonusDamage());
    heroQuery.bindValue(":kills", hero.getKills());

    int weaponId = hero.hasWeaponEquipped() && hero.getSelectedWeapon()
        ? hero.getSelectedWeapon()->getWeapon_id()
        : -1;
    if (weaponId == -1)
        heroQuery.bindValue(":weapon_id", QVariant(QVariant::Int));  // NULL
    else
        heroQuery.bindValue(":weapon_id", weaponId);

    if (!heroQuery.exec()) {
        qDebug() << "Failed to insert/replace hero:" << heroQuery.lastError().text();
        return;
    }

    // If new hero, get the generated ID
    if (isNewHero) {
        int newHeroId = heroQuery.lastInsertId().toInt();
        hero.setHeroID(newHeroId);
    }

    // Handle Inventory: Replace entire inventory for this hero
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM Inventory WHERE hero_id = :hero_id");
    deleteQuery.bindValue(":hero_id", hero.getHeroID());
    deleteQuery.exec();  // Clean up old inventory if exists

    const auto& weapons = hero.getWeapons();
    for (int i = 0; i < weapons.size(); ++i) {
        QSqlQuery weaponsQuery;
        weaponsQuery.prepare(R"(
            INSERT INTO Weapon (weapon_id, hero_id, type_id, inventorySlot, kills)
            VALUES (:weapon_id, :hero_id, :type_id, :inventorySlot, :kills)
        )");
        weaponsQuery.bindValue(":weapon_id", weapons[i]->getWeapon_id());
        weaponsQuery.bindValue(":hero_id", hero.getHeroID());
        weaponsQuery.bindValue(".type_id", weapons[i]->getType_id());
        weaponsQuery.bindValue(":inventorySlot", i);
        weaponsQuery.bindValue(":kills", weapons[i]->getKills());
        weaponsQuery.exec();
    }
}

Hero DatabaseCommunication::loadHero(int heroId) {
    if (!db.isOpen()) return Hero();

    QSqlQuery heroQuery;
    heroQuery.prepare(R"(
        SELECT hero_id, name, hp, lvl, xp, damage, gold,
               inventoryspace, equippedbonusdamage, kills, weapon_id
        FROM Hero
        WHERE hero_id = :hero_id
    )");
    heroQuery.bindValue(":hero_id", heroId);

    if (!heroQuery.exec() || !heroQuery.next()) return Hero();

    int hero_id = heroQuery.value(0).toInt();
    QString name = heroQuery.value(1).isNull() ? "" : heroQuery.value(1).toString();
    int hp = heroQuery.value(2).toInt();
    int lvl = heroQuery.value(3).toInt();
    int xp = heroQuery.value(4).toInt();
    int damage = heroQuery.value(5).toInt();
    int gold = heroQuery.value(6).toInt();
    int inventorySpace = heroQuery.value(7).toInt();
    int equippedBonus = heroQuery.value(8).toInt();
    int kills = heroQuery.value(9).toInt();
    int equippedWeaponId = heroQuery.value(10).isNull() ? -1 : heroQuery.value(10).toInt();

    Weapons* equippedWeapon = nullptr;
    std::vector<Weapons*> heroWeapons;

    QSqlQuery inventoryQuery;
    inventoryQuery.prepare("SELECT weapon_id FROM Weapons WHERE hero_id = :hero_id ORDER BY slot ASC");
    inventoryQuery.bindValue(":hero_id", heroId);

    if (inventoryQuery.exec()) {
        while (inventoryQuery.next()) {
            int typeId = inventoryQuery.value(0).toInt();

            QSqlQuery weaponTypeQuery;
            weaponTypeQuery.prepare("SELECT name, skade, styrkemodifier, holbarhed, price FROM weaponType WHERE type_id = :type_id");
            weaponTypeQuery.bindValue(":type_id", typeId);

            if (weaponTypeQuery.exec() && weaponTypeQuery.next()) {
                Weapons* w = new Weapons(
                    typeId,
                    weaponTypeQuery.value(0).toString().toStdString(),
                    weaponTypeQuery.value(1).toInt(),
                    weaponTypeQuery.value(2).toInt(),
                    weaponTypeQuery.value(3).toInt(),
                    weaponTypeQuery.value(4).toInt(),
                    weaponTypeQuery.value(5).toInt()
                );
                heroWeapons.push_back(w);
                if (typeId == equippedWeaponId)
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

// Show heroes also needs some work
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