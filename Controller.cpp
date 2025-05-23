#include "Controller.h"
#include "Hero.h"
#include "Monster.h"
#include "CaveFactory.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime> // needed for time()
#include <cstdlib> // needed for srand() and rand()
#include "DatabaseCommunication.h"

Controller::Controller() {}

Controller::Controller(string n) {
	// Open database
	dbc.open();

	// The flow of the game starts here
	bool running = true;

	// Loops while running is true
	while (running) {
		cout << "(0) Load Game (1) New Game (2) Analyze Game Statistics (q) to quit" << endl;

		// First choice the player makes is tracked with this char: gameChoice
		char gameChoice;

		// A boolean used to track wether or not the user input is correct
		bool correctInput = false;

		while (!correctInput) {
			cin >> gameChoice;

			if (gameChoice == '0' || gameChoice == '1' || gameChoice == '2' || gameChoice == 'q') {
				correctInput = true;
			}
			else {
				cout << "Error: Incorrect user input. Please enter 0, 1, 2 or q to quit." << endl;
			}
		}

		if (gameChoice == '1') {
			cout << "Create new hero, enter name without any spaces: " << endl;
			string newHero;
			cin >> newHero;
			h = Hero(newHero, 10, 1, 0, 2, 0, 5, 0);
			h.setHeroID(-1); // Mark as new hero
			running = false; // Exit after hero creation
		}

		else if (gameChoice == '0') {
			int hero_id;
			cout << "Enter hero_id to load: " << endl;
			dbc.showHeroes();
			cin >> hero_id;
			h = loadGame(hero_id);
			running = false; // Exit after loading
		}

		else if (gameChoice == '2') {
			analyzeGame();
		}

		else if (gameChoice == 'q') {
			exit(0);
		}
	}


	// Uses showRules() function to show rules
	showRules();
	this_thread::sleep_for(chrono::seconds(20)); // For pausing to read rules

	cout << "" << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "Current hero stats are: " << endl;
	cout << "HP: " << h.getHP() << "  Level: " << h.getLevel() << "  XP: " << h.getXP() << "  Damage: " << h.getDamage() << "  Gold: " << h.getGold() << "  Inventory Space: " << h.showRemaingInventorySpace() << "  Kills:  " << h.getKills() << endl;
	cout << "--------------------------------------------------------------------" << endl;

	this_thread::sleep_for(chrono::seconds(2));

	// Boolean value that is used to track the main part of the game
	bool quitGame = false;

	// While loop that runs indefinetly so that the player can battle as many foes as they want, ends when player writes 0 (to quit)
	while (!quitGame) {

		// First screen the player seen after having either started a new game or loaded an old one
		cout << " " << endl;
		cout << "Choose one of the following options: " << endl;
		cout << "1. To fight a wild monster (W)" << endl;
		cout << "	2. To search nearby terrain for caves (C)" << endl;
		cout << "		3. To enter the armory (A)" << endl;
		cout << "			4. To manage equiped weapons and see hero inventory (E) " << endl;
		cout << "				5. To save and exit (0) " << endl;

		// Create object of the cave factory class, and initialize a vector of monsters (a cave)
		CaveFactory cave1;
		CaveFactory cave2;
		vector<Monster*> cave1Monsters;
		vector<Monster*> cave2Monsters;

		// Action tracks the players action
		char action;
		cin >> action;

		// Action 'C' Lead to fighting a cave, only accesible if hero level is <= 5
		if (action == 'C' && h.getLevel() >= 5) {
			cout << " " << endl;
			cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << endl;
			cout << "You have chosen to search the area for nearby caves " << endl;
			cout << "Loading terrain";
			this_thread::sleep_for(chrono::seconds(1));
			cout << ".";
			this_thread::sleep_for(chrono::seconds(1));
			cout << ".";
			this_thread::sleep_for(chrono::seconds(1));
			cout << ".";
			this_thread::sleep_for(chrono::seconds(1));
			cout << ".";
			this_thread::sleep_for(chrono::seconds(1));
			cout << ".";
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Your hero found two caves" << endl;
			cout << " " << endl;
			cout << "Cave 1: " << endl;
			cout << " " << endl;
			cave1Monsters = cave1.createCave(h.getLevel());
			cave1.printCaveMonsters(cave1Monsters);
			cout << "Cave 2: " << endl;
			cout << " " << endl;
			cave2Monsters = cave2.createCave(h.getLevel());
			cave2.printCaveMonsters(cave2Monsters);
			cout << " " << endl;
			cout << "Now you can choose between three options: " << endl;
			cout << "1. Choose to battle cave 1 " << endl;
			cout << "2. Choose to battle cave 2 " << endl;
			cout << "3. Choose to abandon caves and roam the lands until you encounter a wild monster instead " << endl;
			cout << " " << endl;
			cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << endl;
			cout << " " << endl;

			bool inputOK = false;
			while (!inputOK)
			{
				cin >> action;
				// Choice of action again leads to three scenarios
				if (action != '1' && action != '2' && action != '3')
				{
					cout << "ERROR: incorrect user input" << endl;
				}

				// Battle cave 1
				else if (action == '1')
				{
					h = battleCave(h.getLevel(), cave1Monsters);
					inputOK = true;
				}

				// Battle cave 2
				else if (action == '2')
				{
					h = battleCave(h.getLevel(), cave2Monsters);
					inputOK = true;
				}

				// Leave caves and return to main menu
				else if (action == '3')
				{
					inputOK = true;
				}
			}
		}

		// Handles incorrect user choice, when hero level is too low to fight caves
		else if (action == 'C' && h.getLevel() < 5) {
			cout << " " << endl;
			cout << "ERROR: Your hero level is too low to explore caves. Fight more wild monsters to level up and unlock caves" << endl;
			cout << " " << endl;
		}

		// Action 'A' leads to armory
		else if (action == 'A') {
			char weaponChoice;
			cout << " " << endl;
			cout << "x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>" << endl;
			cout << "Welcome to the armory, here you can buy various weapons, which will help you on your quest" << endl;
			cout << " " << endl;
			cout << "Weapons in the armory are: " << endl;
			// Uses createArmory to initialize the armory
			vector<Weapons*> weaponsInArmory = createArmory();
			seeWeaponsInArmory(weaponsInArmory);
			cout << " " << endl;
			bool inArmory = true;

			// While loop that runs while player remains in the armory
			while (inArmory)
			{
				cout << "Choose weapon to buy by index (e.g., 0, 1, 2), or press - to exit: "
					<< "        Hero Gold : " << h.getGold() << endl;

				cin >> weaponChoice;

				if (weaponChoice == '-') {
					cout << "You have chosen to exit the armory, hope to see you again soon :)" << endl;
					inArmory = false;
				}
				else if (weaponChoice >= '0' && weaponChoice < '0' + weaponsInArmory.size()) {
					int index = weaponChoice - '0';
					buyWeapon(index, h.getGold(), weaponsInArmory);
				}
				else {
					cout << "ERROR: Invalid input. Please enter a valid index digit or '-' to exit." << endl;
				}
			}

			// To avoid any pointer issues, the armory is then swiftly deleted
			for (Weapons* w : weaponsInArmory)
			{
				delete w;
			}
			cout << "x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>" << endl;
		}

		// If action is 'W' the player has chosen to fight wild monsters
		else if (action == 'W') {

			char monsterChoice;

			// Objects of hero and all enemies
			Monster hest("Hest", 5, 1, 100);
			Monster g1("Weak Goblin", 4, 2, 200);
			Monster g2("Strong Goblin", 8, 3, 400);
			Monster g3("Stronger Goblin", 10, 4, 500);
			Monster g4("The Strongest Goblin", 15, 5, 800);
			Monster a("Ape King", 30, 5, 1000);
			Monster u("Unicorn", 5, 8, 1500);
			Monster d("Dragon", 100, 10, 3000);

			// Vector with different monsters
			vector<Monster*> enemies;
			enemies.push_back(&hest);
			enemies.push_back(&g1);
			enemies.push_back(&g2);
			enemies.push_back(&g3);
			enemies.push_back(&g4);
			enemies.push_back(&a);
			enemies.push_back(&u);
			enemies.push_back(&d);

			// Show all monsters
			cout << " " << endl;
			cout << "Monster stats are: " << endl;
			cout << "1. Hest:                 5. The Strongest Goblin:" << endl;
			cout << "HP: " << hest.getHP() << "  Damage: " << hest.getDamage() << "  XP: " << hest.getXP();
			cout << "     HP: " << g4.getHP() << "  Damage: " << g4.getDamage() << "  XP: " << g4.getXP() << endl << endl;

			cout << "2. Weak Goblin:          6. Ape King:" << endl;
			cout << "HP: " << g1.getHP() << "  Damage: " << g1.getDamage() << "  XP: " << g1.getXP();
			cout << "     HP: " << a.getHP() << "  Damage: " << a.getDamage() << "  XP: " << a.getXP() << endl << endl;

			cout << "3. Strong Goblin:        7. Unicorn:" << endl;
			cout << "HP: " << g2.getHP() << "  Damage: " << g2.getDamage() << "  XP: " << g2.getXP();
			cout << "     HP: " << u.getHP() << "  Damage: " << u.getDamage() << "  XP: " << u.getXP() << endl << endl;

			cout << "4. Stronger Goblin:      8. ALMIGHTY DRAGON:" << endl;
			cout << "HP: " << g3.getHP() << "  Damage: " << g3.getDamage() << "  XP: " << g3.getXP();
			cout << "     HP: " << d.getHP() << "  Damage: " << d.getDamage() << "  XP: " << d.getXP() << endl << endl;
			cout << " " << endl;
			cout << "Choose a monster to fight by index: " << endl;
			cin >> monsterChoice;

			// fightMonster returns an updated hero after the fight is done
			h = fightMonster(monsterChoice, enemies);

			// If the hero has a weapon equipped, a certain amount of durability is removed
			if (h.hasWeaponEquipped()) {
				h.removeWeaponDurability();
			}

			this_thread::sleep_for(chrono::seconds(1));

			// Displayes heroes new stats, after the battle
			cout << "" << endl;
			cout << "--------------------------------------------------------------------" << endl;
			cout << "The battle is over, and your heros stats now are: " << endl;
			cout << "--------------------------------------------------------------------" << endl;
			cout << "" << endl;
			cout << "Name:     " << h.getName() << endl;
			cout << "HP:       " << h.getHP() << endl;
			cout << "Level:    " << h.getLevel() << endl;
			cout << "XP:       " << h.getXP() << endl;
			cout << "Damage:   " << h.getDamage() << endl;
			cout << "Gold:     " << h.getGold() << endl;
			cout << "Inventory:" << h.getRemainingInventorySpace() << endl;
			cout << "" << endl;
		}

		// Action 'E' means equip/unequip
		else if (action == 'E') {

			// Boolean used to ensure correct user selection
			bool correctWeaponChoice = false;

			cout << " " << endl;
			cout << "????????????????????????????????????????????????????????????????????" << endl;
			cout << "Here you may equip/unequip purchased weapons." << endl;
			cout << "By equiping a weapon your heroes damage will be affected." << endl;
			cout << "When equiping a weapon, the previously equiped weapon will be unequiped" << endl;
			cout << " " << endl;
			cout << "Choose by index which of your weapons you wish to equip or choose 'q' to unequip current weapon. If you wish to return to main menu (x): " << endl;
			cout << " " << endl;

			// Shows heroes current inventory
			h.showInventory();


			while (!correctWeaponChoice)
			{
				char weaponChoice;
				int herosWeaponsLength = h.getWeapons().size();
				cin >> weaponChoice;

				// Equips the weapon which the user has selected
				if (weaponChoice >= '0' && weaponChoice < '0' + herosWeaponsLength) {
					int index = weaponChoice - '0';

					dbc.open();

					// If a weapon is currently equipped, remove it from DB
					if (h.hasWeaponEquipped()) {
						dbc.unequipWeapon(h);
					}

					// Equip new weapon in memory
					h.equipWeapon(index);

					// Update DB with newly equipped weapon
					dbc.equipWeapon(h.getSelectedWeapon(), h);

					dbc.close();

					correctWeaponChoice = true;
				}

				// Unequips current weapon
				else if (weaponChoice == 'q') {
					dbc.open();
					h.unequipWeapon();
					dbc.unequipWeapon(h);
					dbc.close();
					correctWeaponChoice = true;
				}

				// Exits the loop without doing anything
				else if (weaponChoice == 'x') {
					correctWeaponChoice = true;
				}
				else {
					cout << "ERROR: Invalid weapon choice" << endl;
				}
			}

			// Shows new and updated hero stats
			cout << " " << endl;
			cout << "Your hero stats now are: " << endl;
			cout << "Name:     " << h.getName() << endl;
			cout << "HP:       " << h.getHP() << endl;
			cout << "Level:    " << h.getLevel() << endl;
			cout << "XP:       " << h.getXP() << endl;
			cout << "Damage:   " << h.getDamage() << endl;
			cout << "Gold:     " << h.getGold() << endl;
			cout << "Inventory:" << h.getRemainingInventorySpace() << endl;
			cout << "" << endl;
		}

		// Lastly the game is saved if the action is 0
		else if (action == '0') {
			cout << "Thank you for playing!" << endl;
			cout << "Game saving..." << endl;
			saveGame(h); // Uses save game function to save game to database
			cout << "Game saved succesfully!" << endl;
			quitGame = true;
		}
		else {
			cout << "ERROR: please enter valid input" << endl;
		}
	}
}

// This function shows the rules of the game
void Controller::showRules() {
	cout << "//////////////////////////////////////////////////////////////////////////////////////" << endl;
	cout << "Welcome traveler, i hope you are ready to embark on an awesome quest!" << endl;
	cout << " " << endl;
	cout << "My name is gandalf, and i will be your leader on this quest." << endl;
	cout << "In order to survive you will need to listen very carefully..." << endl;
	cout << " " << endl;
	cout << "On this quest you will encounter various monsters. These monsters have different stats." << endl;
	cout << "The monsters all have HP(health), Damage(amount they hurt hero per turn), and XP which is " << endl;
	cout << "the amount of XP you will recieve for defeating them." << endl;
	cout << "When you defeat a monster you will recieve XP, collect enough XP to level up." << endl;
	cout << "Each level up grants 2 additional hitpoints for your hero. In addition you heroes damage increases by 1 " << endl;
	cout << "Leveling up and improving your heroes stats will let you battle tougher and tougher enemies. " << endl;
	cout << "Along your road you may encounter caves. Caves hold great treasure which you must discover on your own." << endl;
	cout << "However be careful as you venture deep in to the caves, monsters become stronger and stronger." << endl;
	cout << "The risk of caves might be worth it because with gold you can visit the armory and buy awesome weapons." << endl;
	cout << "Remember even with all powerfull weapons you need to be very CAREFUL." << endl;
	cout << "If you accidently battle an enemy before you are ready and lose," << endl;
	cout << "your hero will die and you must start over..." << endl;
	cout << "Now you are ready to play, in order to win you must slay the almighty dragon and free the villagers" << endl;
	cout << "from its tyrani, may the force be with you." << endl;
	cout << "//////////////////////////////////////////////////////////////////////////////////////" << endl;
}

// saveGame function saves the game when user quits
void Controller::saveGame(Hero& h) {
	// Checks if the database is open
	if (!dbc.open()) {
		std::cerr << "Failed to open database during save!" << std::endl;
		return;
	}

	// Uses insert hero function from the database communication class and close function to close the database
	dbc.insertHeroWeapons(h.getWeapons(), h);
	dbc.insertHero(h);
	dbc.close();
}

// loadGame function used to load previously saved heroes, to continue quest
Hero Controller::loadGame(int hero_id) {
	// Assigns the chosen hero to the private attribute of the controller class
	h = dbc.loadHero(hero_id);
	dbc.close();
	return h; // Returns the loaded hero
}

// fightMonster function, handles hero vs monster battles
Hero Controller::fightMonster(char n, vector<Monster*> monsters) {

	// Iterates over the monsters vector
	for (int i = 0; i < monsters.size(); i++) {

		// Monster stats
		int monsterHealth = monsters[i]->getHP();
		string monsterName = monsters[i]->getName();
		int monsterXP = monsters[i]->getXP();

		int heroHealth = h.getHP();
		Hero updatedHero;

		// If n (the monster choice) is equal to i + 1 (because of vector 0 indexing), then fight the selected monster
		if (n == i + 1 + '0') {
			// Fight until either the hero or the monster is dead
			while (monsterHealth > 0 && heroHealth > 0) {
				heroHealth -= monsters[i]->getDamage();
				monsterHealth -= h.getDamage();
				cout << "Hero Health: " << heroHealth << endl;
				cout << "Monster health: " << monsterHealth << endl;
			}
			// Happens if hero wins
			if (monsterHealth <= 0) {
				cout << "You have defeated: " << monsterName << " you recieve " << monsterXP << " XP points " << endl;
				updatedHero = updateLevel(monsterXP);
				return updatedHero;
			}
			// Happens if monster wins
			else {
				cout << "You have been defeated by the monster" << endl;
				cout << "Game over :-(" << endl;
				exit(0);
			}
		}
	}
	cout << "Choice doesnt match" << endl;
	return h;

}

// updateLevel function used to update hero level based on experience points gained
Hero Controller::updateLevel(int xp) {
	vector<Weapons*> hWeapons = h.getWeapons();
	int heroLevel = h.getLevel();
	int heroXP = h.getXP();
	int heroDamage = h.getDamage();
	int heroHP = h.getHP();
	int gold = h.getGold();
	int inventorySpace = h.getRemainingInventorySpace() + hWeapons.size(); // Needed because of the way that i calculate inventory space
	int equippedBonusDamage = h.getEquippedBonusDamage();

	heroXP += xp;

	int heroKills = h.getKills() + 1;

	// Increments the heroes weapons kills
	if (h.hasWeaponEquipped()) {
		Weapons* equippedWeapon = h.getSelectedWeapon();
		equippedWeapon->incrementKills();
	}

	int remainder = heroXP - (heroLevel * 1000);

	if (heroXP >= heroLevel * 1000) {
		heroXP = remainder;
		heroLevel += 1;
		heroDamage += 1;
		heroHP += 2;
	}

	// Returns a modified hero to fight monster function which returns it to the main program, which then assigns it to the private attribute h
	Hero updatedHero(h.getName(), heroHP, heroLevel, heroXP, heroDamage, gold, inventorySpace, equippedBonusDamage, h.getWeapons(), heroKills, h.getSelectedWeapon(), h.getHeroID());
	return updatedHero;
}

// This function is used to battle caves
Hero Controller::battleCave(int heroLvl, vector<Monster*> caveMonsters) {

	CaveFactory c;
	Hero updatedHero;
	int heroGold;

	// This ensures that the caves are generated at random for every time the player playes
	srand(static_cast<unsigned>(time(0)));

	cout << "You have chosen to battle your way through this cave" << endl;
	cout << " " << endl;
	c.printCaveMonsters(caveMonsters);

	int index = 0;
	char action;

	// For loop that iterates thorugh the entire cave
	for (Monster* monster : caveMonsters) {
		cout << "\nNext monster appears: " << monster->getName() << endl;
		cout << "Press 1 to fight or 0 to save and exit: " << endl;
		bool correctInput = false;
		while (!correctInput) {
			cin >> action;
			// Player can choose to save game and exit
			if (action == '0') {
				saveGame(h);
				correctInput = true;
			}
			// What happens if choice is to fight the monster
			else if (action == '1') {
				vector<Monster*> singleMonster = { monster };
				updatedHero = fightMonster('1', singleMonster);
				if (h.hasWeaponEquipped()) {
					h.removeWeaponDurability();
				}
				++index;
				correctInput = true;
			}
			else {
				cout << "ERROR: Incorrect user input, please enter either 1 or 0 " << endl;
			}
		}
	}


	// Return an updated hero based on the level of the cave, also give the hero gold
	if (updatedHero.getLevel() < 7) {
		int heroGold = updatedHero.getGold() + 250;

		h = Hero(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 2, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 250 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 9) {
		int heroGold = updatedHero.getGold() + 500;
		h = Hero(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 4, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 500 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 11) {
		int heroGold = updatedHero.getGold() + 1000;

		h = Hero(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 5, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 1000 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 15) {
		int heroGold = updatedHero.getGold() + 2000;

		h = Hero(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 6, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 2000 gold" << endl;
		return h;
	}
	else {
		int heroGold = updatedHero.getGold() + 10000;

		h = Hero(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 6, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 10000 gold" << endl;
		return h;
	}
}

// This function is used to set up an armory which is an array 
vector<Weapons*> Controller::createArmory() {
	vector<Weapons*> weapons;

	weapons.push_back(new Weapons(0, "Wooden Sword", 2, 0, 10, 100, 0));
	weapons.push_back(new Weapons(1, "Bronze Sword", 2, 2, 10, 200, 0));
	weapons.push_back(new Weapons(2, "Iron Sword", 4, 2, 15, 500, 0));
	weapons.push_back(new Weapons(3, "Gold Sword", 8, 2, 6, 750, 0));
	weapons.push_back(new Weapons(4, "Emerald Sword", 5, 2, 20, 1000, 0));
	weapons.push_back(new Weapons(5, "Diamond Sword", 5, 3, 20, 5000, 0));
	weapons.push_back(new Weapons(6, "Netherite Sword", 6, 3, 30, 10000, 0));
	weapons.push_back(new Weapons(7, "The Almighty God Slayer", 20, 5, 100, 50000, 0));

	return weapons;
}

// Call this to display the weapons currently in the armory
void Controller::seeWeaponsInArmory(vector<Weapons*> weapons) {
	for (size_t i = 0; i < weapons.size(); ++i) {
		Weapons* weapon = weapons[i];
		cout << i << ". "
			<< "Name: " << weapon->getName()
			<< "  Damage: " << weapon->getSkade()
			<< "  Modifier: " << weapon->getStyrkemodifier()
			<< "  Durability: " << weapon->getHoldbarhed()
			<< "  Price: " << weapon->getPrice() << " gold" << endl;
	}
}

// Call this to buy a specific weapon from the armory
void Controller::buyWeapon(char choice, int heroGold, vector<Weapons*>& weapons) {
	// Convert weaponchoice to an int
	int index = choice;

	// The chosen weapon is the index of the given weapons array
	Weapons* weapon = weapons[index];

	// The hero doesnt have enough gold
	if (weapon->getPrice() > heroGold) {
		cout << "You do not have enough gold to purchase this weapon. Battle caves to get more gold and come back later." << endl;
		return;
	}

	// Heroes new amount of gold is equal to the amount he had before minus what the weapon cost
	int newGold = heroGold - weapon->getPrice();
	h.setGold(newGold);

	// Display weapon choice
	cout << "\nYou have selected the weapon: " << weapon->getName() << endl;

	// Checks inventory space
	if (h.getRemainingInventorySpace() <= 0) {
		int action;
		cout << "You do not have enough inventory space. Choose a weapon to discard (by index) or select (-1) to cancel purchase: " << endl;
		h.showInventory();
		cin >> action;
		// Choice is to replace purchased weapon with one in the heroes inventory
		if (action != -1) {
			h.deleteInventorySlot(action);
			h.addWeaponToInventory(weapon);
			cout << weapon->getName() << " has been added to your hero's inventory." << endl;
		}
		// Purchase cancelled
		else {
			cout << "Purchase canceled." << endl;
			int refund = weapon->getPrice();
			newGold += refund;
			h.setGold(newGold);
			return;
		}
	}
	else {
		h.addWeaponToInventory(weapon);
		cout << "The weapon has been added to your hero's inventory." << endl;
		cout << "Thank you for your purchace! " << endl;
	}

	// Display remaining inventory space
	cout << "Remaining inventory space: " << h.getRemainingInventorySpace() << endl;
	cout << "Current hero inventory: " << endl;
	h.showInventory();
}

// Function used to analyze the games database
void Controller::analyzeGame() {
	bool analyze = true;

	// Runs as long as choice isnt 0
	while (analyze)
	{
		cout << " " << endl;
		cout << "==============================================================================" << endl;
		cout << "You have chosen to analyze the game. You now have 4 options: " << endl;
		cout << "1. Display heroes in alphebetical order A - Z " << endl;
		cout << "	2. Display all heroes along with their kills " << endl;
		cout << "		3. See how many kills each weapon has for a given hero " << endl;
		cout << "			4. For each weapontype display which hero has the most kills with it " << endl;
		cout << "				0. To stop analyzing and return to the main menu " << endl;
		cout << "==============================================================================" << endl;
		char analyzeChoice;
		cin >> analyzeChoice;

		bool correctInput1 = false;

		// While correctInput1 is true. This part is a bit tricky because there are quite a few choices within choices
		while (!correctInput1)
		{
			if (analyzeChoice != '1' && analyzeChoice != '2' && analyzeChoice != '3' && analyzeChoice != '4' && analyzeChoice != '0') {
				cout << " " << endl;
				cout << "ERROR: Incorrect user input, please enter a valid index" << endl;
				correctInput1 = true;
			}

			// First option for game anlyzing
			else if (analyzeChoice == '1') {
				dbc.showHeroesABC();
				correctInput1 = true;
			}
			// Second option
			else if (analyzeChoice == '2') {
				dbc.showHeroKills();
				correctInput1 = true;
			}
			// Third option
			else if (analyzeChoice == '3') {
				bool correctInput2 = false;
				while (!correctInput2)
				{
					cout << " " << endl;
					cout << "You have chosen option (3), please select by hero_id, which hero you would like to examine: " << endl;
					cout << " " << endl;

					dbc.showHeroes();
					char hero_id;
					cin >> hero_id;
					bool success = dbc.showHeroWeaponKills(hero_id);
					if (!success) {
						cout << "ERROR: Incorrect user input, please select a valid hero_id" << endl;
					}
					else {
						correctInput2 = true;
					}
				}
				correctInput1 = true;
			}
			// Fourth option
			else if (analyzeChoice == '4') {
				dbc.showWeaponTypeKillsLeader();
				correctInput1 = true;
			}
			// User has chosen to exit analyze section
			else if (analyzeChoice == '0') {
				analyze = false;
				correctInput1 = true;
			}
		}
	}
}


Controller::~Controller() {}

