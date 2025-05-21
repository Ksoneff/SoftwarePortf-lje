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

// Equips weapon in the database dynamically while the game is running
void DatabaseCommunication::equipWeapon(Weapons* selectedWeapon, Hero& hero) {
    if (!db.isOpen()) {
        std::cerr << "equipWeapon() aborted: database is not open!" << std::endl;
        return;
    }

    if (selectedWeapon == nullptr) {
        std::cerr << "equipWeapon() aborted: selected weapon is null!" << std::endl;
        return;
    }

    int equippedWeaponId = -1;

    // Find the inventory index of selectedWeapon
    int inventorySlot = -1;
    const auto& weapons = hero.getWeapons();
    for (int i = 0; i < weapons.size(); ++i) {
        if (weapons[i] == selectedWeapon) {
            inventorySlot = i;
            break;
        }
    }

    // If new weapon, insert it first to assign a weapon_id
    if (selectedWeapon->getWeapon_id() == -1) {
        QSqlQuery insertWeaponQuery;
        insertWeaponQuery.prepare(R"(
            INSERT INTO Weapon (hero_id, type_id, inventorySlot, kills)
            VALUES (:hero_id, :type_id, :inventorySlot, :kills)
        )");
        insertWeaponQuery.bindValue(":hero_id", hero.getHeroID());
        insertWeaponQuery.bindValue(":type_id", selectedWeapon->getType_id());
        insertWeaponQuery.bindValue(":inventorySlot", inventorySlot);
        insertWeaponQuery.bindValue(":kills", selectedWeapon->getKills());

        if (!insertWeaponQuery.exec()) {
            qDebug() << "Failed to insert weapon:" << insertWeaponQuery.lastError().text();
            return;
        }

        equippedWeaponId = insertWeaponQuery.lastInsertId().toInt();
        selectedWeapon->setWeapon_id(equippedWeaponId); // Sync back to object
    }
    else {
        equippedWeaponId = selectedWeapon->getWeapon_id();
    }

    // Update the Hero table with the equipped weapon
    QSqlQuery updateHeroQuery;
    updateHeroQuery.prepare(R"(
        UPDATE Hero SET weapon_id = :weapon_id WHERE hero_id = :hero_id
    )");
    updateHeroQuery.bindValue(":weapon_id", equippedWeaponId);
    updateHeroQuery.bindValue(":hero_id", hero.getHeroID());

    if (!updateHeroQuery.exec()) {
        qDebug() << "Failed to update hero's equipped weapon:" << updateHeroQuery.lastError().text();
        return;
    }

    qDebug() << "Weapon equipped successfully with weapon_id:" << equippedWeaponId;
}

// Unequips weapon in database dynamically while the game is running
void DatabaseCommunication::unequipWeapon(Hero& hero) {
    if (!db.isOpen()) {
        std::cerr << "unequipWeapon() aborted: database is not open!" << std::endl;
        return;
    }

    if (hero.getSelectedWeapon() == nullptr) {
        std::cerr << "unequipWeapon() aborted: hero has no weapon equipped!" << std::endl;
        return;
    }

    // Update the Hero table with the equipped weapon
    QSqlQuery unequipWeaponQuery;

    unequipWeaponQuery.prepare(R"(
        UPDATE Hero SET weapon_id = :weapon_id WHERE hero_id = :hero_id
    )");
    unequipWeaponQuery.bindValue(":weapon_id", QVariant(QVariant::Int));
    unequipWeaponQuery.bindValue(":hero_id", hero.getHeroID());

    if (!unequipWeaponQuery.exec()) {
        qDebug() << "Failed to update hero's equipped weapon:" << unequipWeaponQuery.lastError().text();
        return;
    }

    qDebug() << "Weapon unequipped successfully" << endl;

}

// Inserts heroes weapons
void DatabaseCommunication::insertHeroWeapons(vector<Weapons*> heroWeapons, Hero& hero) {
    if (!db.isOpen()) {
        std::cerr << "insertHeroWeapons() aborted: database is not open!" << std::endl;
        return;
    }

    // Iterates over all weapons in heroes inventory
    for (int i = 0; i < heroWeapons.size(); ++i) {
        Weapons* w = heroWeapons[i];
        QSqlQuery weaponsQuery;

        bool isNew = (w->getWeapon_id() == -1);

        // Alternate way of inserting weapon based on if its new or not
        if (isNew) {
            weaponsQuery.prepare(R"(
                INSERT INTO Weapon (hero_id, type_id, inventorySlot, kills)
                VALUES (:hero_id, :type_id, :inventorySlot, :kills)
            )");
        } else {
            weaponsQuery.prepare(R"(
                INSERT OR REPLACE INTO Weapon (hero_id, type_id, inventorySlot, kills, weapon_id)
                VALUES (:hero_id, :type_id, :inventorySlot, :kills, :weapon_id)
            )");
            weaponsQuery.bindValue(":weapon_id", w->getWeapon_id());
        }

        // Always does this
        weaponsQuery.bindValue(":hero_id", hero.getHeroID());
        weaponsQuery.bindValue(":type_id", w->getType_id());
        weaponsQuery.bindValue(":inventorySlot", i);
        weaponsQuery.bindValue(":kills", w->getKills());

        if (!weaponsQuery.exec()) {
            qDebug() << "Failed to insert weapon:" << weaponsQuery.lastError().text();
            continue;
        }

        // If its a new weapon also gives it a new weapon id
        if (isNew) {
            int newId = weaponsQuery.lastInsertId().toInt();
            w->setWeapon_id(newId);
        }
    }
}

// Function used to insert hero
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
    int weaponId = hero.hasWeaponEquipped() && hero.getSelectedWeapon()
        ? hero.getSelectedWeapon()->getWeapon_id()
        : -1;

    // No weapon equipped
    if (weaponId == -1)
        heroQuery.bindValue(":weapon_id", QVariant(QVariant::Int));  // NULL

    // Hero has weapon equipped
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
    weaponsQuery.prepare("SELECT type_id, weapon_id, kills FROM weapon WHERE hero_id = :hero_id ORDER BY inventoryslot ASC");
    weaponsQuery.bindValue(":hero_id", hero_id);

    // Debugging start
    if (!weaponsQuery.exec()) {
        qDebug() << "weaponsQuery failed:" << weaponsQuery.lastError().text();
    }
    // Debugging end

    // If execution is succesful
    if (weaponsQuery.exec()) {
        // Loop through every weapon in the database
        while (weaponsQuery.next()) {
            int typeId = weaponsQuery.value(0).toInt();
            int weaponId = weaponsQuery.value(1).toInt();
            int weaponKills = weaponsQuery.value(2).toInt();

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
                    weaponKills,
                    weaponId
                );
                if (weaponId == equippedWeaponId) {
                    equippedWeapon = w;
                } else {
                    heroWeapons.push_back(w);
                }
            }
        }
    }

    // Return reconstructed hero
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

// This functions intended use is showcasing hero along with its weapons so that the user can make an informed decision when choosing to load a hero
void DatabaseCommunication::showHeroes() {
    // Checks if database is open
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    // Initiates a new query
    QSqlQuery heroQuery;
    if (!heroQuery.exec("SELECT hero_id, name, hp, lvl, xp, damage, gold, inventoryspace, equippedbonusdamage, weapon_id FROM Hero")) {
        qDebug() << "Failed to retrieve heroes:" << heroQuery.lastError().text();
        return;
    }

    // Iterates through the hero table in the database retrieving relevant info
    while (heroQuery.next()) {
        int hero_id = heroQuery.value(0).toInt();
        QString hero_qname = heroQuery.value(1).toString();
        string hero_name = hero_qname.toStdString();
        int hp = heroQuery.value(2).toInt();
        int lvl = heroQuery.value(3).toInt();
        int xp = heroQuery.value(4).toInt();
        int damage = heroQuery.value(5).toInt();
        int gold = heroQuery.value(6).toInt();
        int inventorySpace = heroQuery.value(7).toInt();
        int equippedBonus = heroQuery.value(8).toInt();
        int equippedWeaponId = heroQuery.value(9).isNull() ? -1 : heroQuery.value(9).toInt();

        // Prints hero to terminal
        cout << " " << endl;
        cout << "Hero_id: " << hero_id << " | Name: " << hero_name
            << " | HP: " << hp << " | Level: " << lvl << " | XP: " << xp
            << " | Damage: " << damage << " | Gold: " << gold
            << " | Inventory Space: " << inventorySpace
            << " | Equipped Bonus: " << equippedBonus
            << " | Weapon ID: " << equippedWeaponId << endl;
        cout << "Weapons currently in heroes inventory: " << endl;

        // Initiates new query to display hero weapons
        QSqlQuery heroWeaponsQuery1;
        heroWeaponsQuery1.prepare("SELECT weapon_id, type_id FROM Weapon WHERE hero_id = :hero_id");
        heroWeaponsQuery1.bindValue(":hero_id", hero_id);

        if (!heroWeaponsQuery1.exec()) {
            qDebug() << "Failed to retrieve weapons for hero_id" << hero_id << ":" << heroWeaponsQuery1.lastError().text();
            continue;
        }

        // Iterates through all weapons retrieving weapon_id and type_id for the heroes weapons
        while (heroWeaponsQuery1.next()) {
            int weapon_id = heroWeaponsQuery1.value(0).toInt();
            int type_id = heroWeaponsQuery1.value(1).toInt();

            cout << "-> Weapon_id: " << weapon_id
                << " | type_id: " << type_id << endl;

            // Uses the weaponType table because all weapons with same type_id share the same stats
            QSqlQuery weaponTypesQuery;
            weaponTypesQuery.prepare("SELECT name, skade, styrkemodifier, holbarhed FROM weaponType WHERE type_id = :type_id");
            weaponTypesQuery.bindValue(":type_id", type_id);

            // Prints the weapons with matching type_id, or like prints their stats
            if (weaponTypesQuery.exec() && weaponTypesQuery.next()) {
                QString weapon_qname = weaponTypesQuery.value(0).toString();
                string weapon_name = weapon_qname.toStdString();
                int skade = weaponTypesQuery.value(1).toInt();
                int styrkemodifier = weaponTypesQuery.value(2).toInt();
                int holdbarhed = weaponTypesQuery.value(3).toInt();
                cout << " | Name: " << weapon_name << " | Skade: " << skade << " | Styrkemodifier: " << styrkemodifier << " | Holdbarhed: " << holdbarhed << endl;
            }
        }
    }
}

// Call this function to show heroes in database in alphabetic order
void DatabaseCommunication::showHeroesABC() {
    // Check if database is open
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    // Prepare the heroABCQuery in the correct way to display the heroes in ascending alphebetical fasion
    QSqlQuery heroABCQuery;
    if (!heroABCQuery.exec("SELECT hero_id, name, hp, lvl, xp, damage, gold, inventoryspace, equippedbonusdamage, weapon_id FROM Hero ORDER BY name ASC")) {
        qDebug() << "Failed to retrieve heroes:" << heroABCQuery.lastError().text();
        return;
    }

    // Loops through every hero
    while (heroABCQuery.next())
    {
        int hero_id = heroABCQuery.value(0).toInt();
        QString hero_qname = heroABCQuery.value(1).toString();
        string hero_name = hero_qname.toStdString();
        int hp = heroABCQuery.value(2).toInt();
        int lvl = heroABCQuery.value(3).toInt();
        int xp = heroABCQuery.value(4).toInt();
        int damage = heroABCQuery.value(5).toInt();
        int gold = heroABCQuery.value(6).toInt();
        int inventorySpace = heroABCQuery.value(7).toInt();
        int equippedBonus = heroABCQuery.value(8).toInt();
        int equippedWeaponId = heroABCQuery.value(9).isNull() ? -1 : heroABCQuery.value(9).toInt();

        // Prints hero to terminal
        cout << " " << endl;
        cout << "Hero_id: " << hero_id << " | Name: " << hero_name
            << " | HP: " << hp << " | Level: " << lvl << " | XP: " << xp
            << " | Damage: " << damage << " | Gold: " << gold
            << " | Inventory Space: " << inventorySpace
            << " | Equipped Bonus: " << equippedBonus
            << " | Weapon ID: " << equippedWeaponId << endl;
    }
}

// Call this function to show how many kills each hero has
void DatabaseCommunication::showHeroKills() {
    // Check if database is open
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    // Prepare the heroKillsQuery, with the correct statement to select all heroes and their kills
    QSqlQuery heroKillsQuery;
    if (!heroKillsQuery.exec("SELECT hero_id, name, kills FROM Hero ORDER BY kills DESC")) {
        qDebug() << "Failed to retrieve heroes:" << heroKillsQuery.lastError().text();
        return;
    }

    while (heroKillsQuery.next())
    {
        int hero_id = heroKillsQuery.value(0).toInt();
        QString hero_qname = heroKillsQuery.value(1).toString();
        string hero_name = hero_qname.toStdString();
        int kills = heroKillsQuery.value(2).toInt();

        cout << " " << endl;
        cout << "Hero_id: " << hero_id << " | Name: " << hero_name << " | Kills: " << kills << endl;
    }
}

// Call this function to show how many kills every weapon has for a given hero
bool DatabaseCommunication::showHeroWeaponKills(char hID) {
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return false;
    }

    int hero_id = hID - '0';

    // Check if the hero exists
    QSqlQuery checkHeroQuery;
    checkHeroQuery.prepare("SELECT name FROM hero WHERE hero_id = :hero_id");
    checkHeroQuery.bindValue(":hero_id", hero_id);

    // Return false if incorrect hero id was chosen, or if exec failed
    if (!checkHeroQuery.exec() || !checkHeroQuery.next()) {
        qDebug() << "Hero ID" << hero_id << "does not exist.";
        return false;
    }


    QString heroName = checkHeroQuery.value(0).toString();
    cout << "Hero: " << heroName.toStdString() << endl;

    // Get all weapons for the hero along with kills and weapon type name
    QSqlQuery weaponKillsQuery;
    weaponKillsQuery.prepare(R"(SELECT wt.name, w.kills FROM weapon w JOIN weaponType wt ON w.type_id = wt.type_id WHERE w.hero_id = :hero_id ORDER BY w.kills DESC)");
    weaponKillsQuery.bindValue(":hero_id", hero_id);

    if (!weaponKillsQuery.exec()) {
        qDebug() << "Failed to retrieve weapons for hero_id" << hero_id << ":" << weaponKillsQuery.lastError().text();
        return false;
    }

    // Display all weapons
    bool hasWeapons = false;
    while (weaponKillsQuery.next()) {
        hasWeapons = true;
        QString weaponName = weaponKillsQuery.value(0).toString();
        int kills = weaponKillsQuery.value(1).toInt();

        cout << "  -> Weapon: " << weaponName.toStdString()
            << " | Kills: " << kills << endl;
    }

    if (!hasWeapons) {
        cout << "This hero has no weapons." << endl;
    }

    return true;
}

// Call this function to see which hero currently has the most kills for every weapon type
void DatabaseCommunication::showWeaponTypeKillsLeader() {
    // Check if database is open
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    // Prepare the weaponTypeKillsLeaderQuery 
    QSqlQuery weaponTypeKillsLeaderQuery;
    if (!weaponTypeKillsLeaderQuery.exec("SELECT h.hero_id, h.name, w.kills, w.type_id FROM weapon w JOIN hero h ON h.hero_id = w.hero_id ORDER BY w.type_id, w.kills DESC")) {
        qDebug() << "Failed to retrieve heroes:" << weaponTypeKillsLeaderQuery.lastError().text();
        return;
    }

    while (weaponTypeKillsLeaderQuery.next())
    {
        int hero_id = weaponTypeKillsLeaderQuery.value(0).toInt();
        QString hero_qname = weaponTypeKillsLeaderQuery.value(1).toString();
        string hero_name = hero_qname.toStdString();
        int kills = weaponTypeKillsLeaderQuery.value(2).toInt();
        int type_id = weaponTypeKillsLeaderQuery.value(3).toInt();

        cout << " " << endl;
        cout << "WeaponType_id: " << type_id << " |Hero_id: " << hero_id << " | Name: " << hero_name << " | Kills: " << kills << endl;
    }
}