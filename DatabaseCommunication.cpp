#include "DatabaseCommunication.h"
#include "Hero.h"
#include "Weapons.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <iostream>

// Constructer that initiates the database communication
DatabaseCommunication::DatabaseCommunication(const QString& path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
}

// Used to open the database which was initialized in the constructer
bool DatabaseCommunication::open() {
    // Error check to see if database was openned correctly
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    qDebug() << "Database opened successfully.";
    return true;
}

// Closes connection to database
void DatabaseCommunication::close() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed.";
    }
}

// Function used to insert hero aswell as heroes inventory/weapons
void DatabaseCommunication::insertHero(Hero& hero) {

    if (!db.isOpen()) {
        std::cerr << "insertHero() aborted: database is not open!" << std::endl;
        return;
    }

    bool isNewHero = (hero.getHeroID() == -1); // For tracking wether or not a hero is already in the database

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

    // New query for inserting into database hero table
    QSqlQuery heroQuery;
    heroQuery.prepare(sql);

    // Only get hero id from hero object if hero is already in the database
    if (!isNewHero)
    {
        heroQuery.bindValue(":hero_id", hero.getHeroID());
    }

    // Insert hero values
    heroQuery.bindValue(":name", QString::fromStdString(hero.getName()));
    heroQuery.bindValue(":hp", hero.getHP());
    heroQuery.bindValue(":lvl", hero.getLevel());
    heroQuery.bindValue(":xp", hero.getXP());
    heroQuery.bindValue(":damage", hero.getDamage());
    heroQuery.bindValue(":gold", hero.getGold());
    heroQuery.bindValue(":inventoryspace", hero.getRemainingInventorySpace());
    heroQuery.bindValue(":equippedbonusdamage", hero.getEquippedBonusDamage());
    heroQuery.bindValue(":kills", hero.getKills());


    // Get equipped weapon type id
    int weaponTypeId = hero.hasWeaponEquipped() && hero.getSelectedWeapon()
        ? hero.getSelectedWeapon()->getType_id()
        : -1;

    // No weapon equipped
    if (weaponTypeId == -1)
        heroQuery.bindValue(":weapon_id", QVariant(QVariant::Int));  // NULL

    // Hero has weapon equipped
    else
        heroQuery.bindValue(":weapon_id", weaponTypeId);

    if (!heroQuery.exec()) {
        qDebug() << "Failed to insert/replace hero:" << heroQuery.lastError().text();
        return;
    }

    // If new hero, get the generated ID
    if (isNewHero) {
        int newHeroId = heroQuery.lastInsertId().toInt();
        hero.setHeroID(newHeroId);
    }

    // Handle Weapons: Replace all previous weapons for this hero, with new weapons
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM Weapons WHERE hero_id = :hero_id");
    deleteQuery.bindValue(":hero_id", hero.getHeroID());
    deleteQuery.exec();  // Clean up old inventory if exists



    // Add hero weapons to weapon table
    const auto& weapons = hero.getWeapons();
    for (int i = 0; i < weapons.size(); ++i) {
        // For handling if a weapon is new or not
        bool isNewWeapon = (weapons[i]->getWeapon_id() == -1); 
        QString sqlW;

        // Inserting without weapon_id will be auto generated later
        if (isNewWeapon)
        {
            sqlW = R"(
                INSERT INTO Weapon (hero_id, type_id, inventorySlot, kills)
                VALUES (:hero_id, :type_id, :inventorySlot, :kills)
            )";
        }
        // Insert with old weapon_id
        else 
        {
            sqlW = R"(
                INSERT INTO Weapon (weapon_id, hero_id, type_id, inventorySlot, kills)
                VALUES (:weapon_id, :hero_id, :type_id, :inventorySlot, :kills)
            )";
        }

        QSqlQuery weaponsQuery;
        weaponsQuery.prepare(sqlW);
        
        // Only if its an old weapon with updated kills or inventory stats
        if (!isNewWeapon) 
        {
            weaponsQuery.bindValue(":weapon_id", weapons[i]->getWeapon_id());
        }

        // Always do
        weaponsQuery.bindValue(":hero_id", hero.getHeroID());
        weaponsQuery.bindValue(":type_id", weapons[i]->getType_id());
        weaponsQuery.bindValue(":inventorySlot", i);
        weaponsQuery.bindValue(":kills", weapons[i]->getKills());
        weaponsQuery.exec();

        // Create new weaponID
        if (isNewWeapon) 
        {
            int newWeaponId = weaponsQuery.lastInsertId().toInt();
            weapons[i]->setWeapon_id(newWeaponId);
        }
    }
}

// Function used to load hero from the database
Hero DatabaseCommunication::loadHero(int heroId) {
    // Check if database is open
    if (!db.isOpen()) return Hero();

    // Initiate the hero query
    QSqlQuery heroQuery;
    heroQuery.prepare(R"(
        SELECT hero_id, name, hp, lvl, xp, damage, gold,
               inventoryspace, equippedbonusdamage, kills, weapon_id
        FROM Hero
        WHERE hero_id = :hero_id
    )");

    heroQuery.bindValue(":hero_id", heroId);


    if (!heroQuery.exec() || !heroQuery.next()) return Hero();

    // Extracts nescisarry information and adds it to variables
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

    // Variables created to insert into hero object
    Weapons* equippedWeapon = nullptr;
    vector<Weapons*> heroWeapons;

    // Initiates weaponsQuery to extract weapons that go along with selected hero
    QSqlQuery weaponsQuery;
    weaponsQuery.prepare("SELECT type_id, weapon_id FROM Weapons WHERE hero_id = :hero_id ORDER BY inventorySlot ASC");
    weaponsQuery.bindValue(":hero_id", heroId);

    // If execution is succesful
    if (weaponsQuery.exec()) {
        // Loop through every weapon in the database
        while (weaponsQuery.next()) {
            int typeId = weaponsQuery.value(0).toInt();
            int weaponId = weaponsQuery.value(1).toInt();
            int weaponKills = weaponsQuery.value(3).toInt();

            QSqlQuery weaponTypeQuery;
            weaponTypeQuery.prepare("SELECT name, skade, styrkemodifier, holbarhed, price FROM weaponType WHERE type_id = :type_id");
            weaponTypeQuery.bindValue(":type_id", typeId);

            if (weaponTypeQuery.exec() && weaponTypeQuery.next()) {
                Weapons* w = new Weapons(
                    weaponId,
                    typeId,
                    weaponTypeQuery.value(0).toString().toStdString(),
                    weaponTypeQuery.value(1).toInt(),
                    weaponTypeQuery.value(2).toInt(),
                    weaponTypeQuery.value(3).toInt(),
                    weaponTypeQuery.value(4).toInt(),
                    weaponKills
                );
                heroWeapons.push_back(w);
                if (weaponId == equippedWeaponId)
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

// showHeroes intended use is displaying hero - and stats, weapons etc. so that the user can make an informed decision when choosing a hero
void DatabaseCommunication::showHeroes() {
    // Checks to ensure the database is open
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }


    QSqlQuery heroQuery;
    QSqlQuery heroWeaponsQuery;
    if (!heroQuery.exec("SELECT hero_id, name, hp, lvl, xp, damage, gold, inventoryspace, equippedbonusdamage, weapon_id FROM Hero")) {
        qDebug() << "Failed to retrieve heroes:" << heroQuery.lastError().text();
        return;
    }

    if (!heroWeaponsQuery.exec("SELECT weapon_id, type_id, name, skade, styrkemodifier, holdbarhed, price FROM Weapon WHERE hero_id = :hero_id")) {
        qDebug() << "Failed to retrieve hero weapons:" << heroWeaponsQuery.lastError().text();
        return;
    }

    while (heroQuery.next()) {
        int hero_id = heroQuery.value(0).toInt();
        QString qname = heroQuery.value(1).toString();
        std::string name = qname.toStdString();
        int hp = heroQuery.value(2).toInt();
        int lvl = heroQuery.value(3).toInt();
        int xp = heroQuery.value(4).toInt();
        int damage = heroQuery.value(5).toInt();
        int gold = heroQuery.value(6).toInt();
        int inventorySpace = heroQuery.value(7).toInt();
        int equippedBonus = heroQuery.value(8).toInt();
        int equippedWeaponId = heroQuery.value(9).isNull() ? -1 : heroQuery.value(9).toInt();

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