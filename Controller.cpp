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
	dbc.open();
	Hero h;
	Hero Ironman;
	Hero Spiderman;
	Hero Deadpool;
	Hero Batman;
	Hero Tester;
	char newGame;
	char action;
	string newHero;
	string oldHero;
	bool correctInput = false;

	// The flow of the game starts here
	cout << "(0) Load Game (1) New Game (2) Load Premade Hero: " << endl;

	while (!correctInput) { // While loop checking for icorrect user input
		cin >> newGame;
		if (newGame == '0' || newGame == '1' || newGame == '2') {
			correctInput = true;
		}
		else {
			cout << "Error incorrect user input, please enter either 0, 1 or 2" << endl;
			correctInput = false;
		}
	}

	if (newGame == '1') {
		cout << "Create new hero, enter name without any spaces: " << endl;
		cin >> newHero;
		h = Hero(newHero, 10, 1, 0, 2, 0, 5, 0);
		h.setHeroID(-1); // So that my database nows that its a new hero
	}

	if (newGame == '0') {
		int hero_id;
		cout << "Enter hero_id to load: " << endl;
		dbc.showHeroes();
		cin >> hero_id;
		h = loadGame(hero_id);
	}

	if (newGame != '0' && newGame != '1') {
		correctInput = false;
		while (!correctInput) {
			if (newGame == '2') {
				string heroChoice;
				cout << " " << endl;
				cout << "§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§" << endl;
				cout << "Load one of the following heroes: " << endl;
				Ironman = Hero("Ironman", 8, 1, 0, 6, 0, 10, 0);
				Ironman.setHeroID(-1);
				cout << " " << endl;
				cout << "Ironman: Strengths - Has cool tricks and can damage other oppenents heavily, also has additional inventory space for extra gadgets. Weakness - Has low HP " << endl;
				cout << "HP: " << Ironman.getHP() << "  Level: " << Ironman.getLevel() << "  XP: " << Ironman.getXP() << "  Damage: " << Ironman.getDamage() << "  Gold: " << Ironman.getGold() << "  Inventory Space: " << Ironman.getRemainingInventorySpace() << endl;
				cout << " " << endl;
				Spiderman = Hero("Spiderman", 12, 1, 0, 3, 0, 5, 0);
				Spiderman.setHeroID(-1);
				cout << " " << endl;
				cout << "Spiderman: This is a pretty average choice, no real strengths or weaknesses, just your friendly neighboorhod spiderman " << endl;
				cout << "HP: " << Spiderman.getHP() << "  Level: " << Spiderman.getLevel() << "  XP: " << Spiderman.getXP() << "  Damage: " << Spiderman.getDamage() << "  Gold: " << Spiderman.getGold() << "  Inventory Space: " << Spiderman.getRemainingInventorySpace() << endl;
				cout << " " << endl;
				Deadpool = Hero("Deadpool", 20, 1, 0, 1, 0, 5, 0);
				Deadpool.setHeroID(-1);
				cout << " " << endl;
				cout << "Deadpool: Strength - Regenerates so starts out with double HP, Weakness - doesnt really damage too much " << endl;
				cout << "HP: " << Deadpool.getHP() << "  Level: " << Deadpool.getLevel() << "  XP: " << Deadpool.getXP() << "  Damage: " << Deadpool.getDamage() << "  Gold: " << Deadpool.getGold() << "  Inventory Space: " << Deadpool.getRemainingInventorySpace() << endl;
				cout << " " << endl;
				Batman = Hero("Batman", 10, 1, 0, 2, 500, 10, 0);
				Batman.setHeroID(-1);
				cout << " " << endl;
				cout << "Batman: Strength - HE IS RICH, so you start the game with 500 gold. And like ironman he has room for extra gadgets " << endl;
				cout << "HP: " << Batman.getHP() << "  Level: " << Batman.getLevel() << "  XP: " << Batman.getXP() << "  Damage: " << Batman.getDamage() << "  Gold: " << Batman.getGold() << "  Inventory Space: " << Batman.getRemainingInventorySpace() << endl;
				cout << " " << endl;
				Tester = Hero("Tester", 20, 5, 0, 6, 10000, 5, 0);
				Tester.setHeroID(-1);
				cout << " " << endl;
				cout << "Tester: This hero is mainly for testing, start at level 5, too instantly explore caves, 10000 gold to buy weapons" << endl;
				cout << "HP: " << Tester.getHP() << "  Level: " << Tester.getLevel() << "  XP: " << Tester.getXP() << "  Damage: " << Tester.getDamage() << "  Gold: " << Tester.getGold() << "  Inventory Space: " << Tester.getRemainingInventorySpace() << endl;
				cout << " " << endl;
				cout << "§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§" << endl;
				cin >> heroChoice;
				if (heroChoice == Ironman.getName()) {
					h = Ironman;
					correctInput = true;
				}
				else if (heroChoice == Spiderman.getName()) {
					h = Spiderman;
					correctInput = true;
				}
				else if (heroChoice == Deadpool.getName()) {
					h = Deadpool;
					correctInput = true;
				}
				else if (heroChoice == Batman.getName()) {
					h = Batman;
					correctInput = true;
				}
				else if (heroChoice == Tester.getName()) {
					h = Tester;
					correctInput = true;
				}
				else {
					cout << "Incorrect user input, please choose a hero by writing the correct name: " << endl;
				}
			}
		}
	}


	// Uses showRules() function to show rules
	showRules();
	this_thread::sleep_for(chrono::seconds(1)); // For pausing to read rules, taken from chatGBT, i mean found chrono and stuff there:)

	cout << "" << endl;
	cout << "" << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "Current hero stats are: " << endl;
	cout << "HP: " << h.getHP() << "  Level: " << h.getLevel() << "  XP: " << h.getXP() << "  Damage: " << h.getDamage() << "  Gold: " << h.getGold() << "  Inventory Space: " << h.getRemainingInventorySpace() << "  Kills:  " << h.getKills() << endl;
	cout << "--------------------------------------------------------------------" << endl;

	this_thread::sleep_for(chrono::seconds(5));

	bool quitGame = false;

	// While loop that runs indefinetly so that the player can battle as many foes as they want, ends when player writes 0 (to quit)
	while (!quitGame) {
		cout << " " << endl;
		cout << "Choose one of the following options: " << endl;
		cout << "1. To fight a wild monster (W)" << endl;
		cout << "	2. To search nearby terrain for caves (C)" << endl;
		cout << "		3. To enter the armory (A)" << endl;
		cout << "			4. To manage equiped weapons and see hero inventory (E) " << endl;
		cout << "				5. To save and exit (0) " << endl;

		CaveFactory cave1;
		CaveFactory cave2;
		vector<Monster*> cave1Monsters;
		vector<Monster*> cave2Monsters;

		cin >> action;

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
				if (action != '1' && action != '2' && action != '3')
				{
					cout << "ERROR: incorrect user input" << endl;
				}

				else if (action == '1')
				{
					h = battleCave(h.getLevel(), h, cave1Monsters);
					inputOK = true;
				}
				else if (action == '2')
				{
					h = battleCave(h.getLevel(), h, cave2Monsters);
					inputOK = true;
				}
				else if (action == '3')
				{
					inputOK = true;
				}
			}
		}

		else if (action == 'C' && h.getLevel() < 5) {
			cout << " " << endl;
			cout << "ERROR: Your hero level is too low to explore caves. Fight more wild monsters to level up and unlock caves" << endl;
			cout << " " << endl;
		}

		else if (action == 'A') {
			char weaponChoice;
			cout << " " << endl;
			cout << "x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>" << endl;
			cout << "Welcome to the armory, here you can buy various weapons, which will help you on your quest" << endl;
			cout << " " << endl;
			cout << "Weapons in the armory are: " << endl;
			vector<Weapons*> weaponsInArmory = createArmory();
			seeWeaponsInArmory(weaponsInArmory);
			cout << " " << endl;
			bool inArmory = true;

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
					buyWeapon(index, h.getGold(), weaponsInArmory, h);
				}
				else {
					cout << "ERROR: Invalid input. Please enter a valid index digit or '-' to exit." << endl;
				}
			}

			for (Weapons* w : weaponsInArmory)
			{
				delete w;
			}
			cout << "x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>x<>" << endl;
		}

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

			h = fightMonster(monsterChoice, h, enemies);
			if (h.hasWeaponEquipped()) {
				h.removeWeaponDurability();
			}

			this_thread::sleep_for(chrono::seconds(2));

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

		else if (action == 'E') {
			bool correctWeaponChoice = false;
			cout << " " << endl;
			cout << "????????????????????????????????????????????????????????????????????" << endl;
			cout << "Here you may equip/unequip purchased weapons." << endl;
			cout << "By equiping a weapon your heroes damage will be affected." << endl;
			cout << "When equiping a weapon, the previously equiped weapon will be unequiped" << endl;
			cout << " " << endl;
			cout << "Choose by index which of your weapons you wish to equip or choose 'q' to unequip current weapon. If you wish to return to main menu (x): " << endl;
			cout << " " << endl;
			
			h.showInventory();


			while (!correctWeaponChoice)
			{
				char weaponChoice;
				int herosWeaponsLength = h.getWeapons().size();
				cin >> weaponChoice;

				if (weaponChoice >= '0' && weaponChoice < '0' + herosWeaponsLength) {
					int index = weaponChoice - '0';
					h.equipWeapon(index);
					correctWeaponChoice = true;
				}
				else if (weaponChoice == 'q') {
					h.unequipWeapon();
					correctWeaponChoice = true;
				}
				else if (weaponChoice == 'x') {
					correctWeaponChoice = true;
				}
				else {
					cout << "ERROR: Invalid weapon choice" << endl;
				}
			}

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
void Controller::saveGame(Hero h) {
	if (!dbc.open()) {
		std::cerr << "Failed to open database during save!" << std::endl;
		return;
	}

	dbc.insertHero(h);
	dbc.close();
}


// loadGame function used to load previously saved heroes, to continue quest
Hero Controller::loadGame(int hero_id) {
	Hero h = dbc.loadHero(hero_id);
	dbc.close();
	return h;
}

// fightMonster function, handles hero vs monster battles
Hero Controller::fightMonster(char n, Hero h, vector<Monster*> monsters) {

	for (int i = 0; i < monsters.size(); i++) {

		int monsterHealth = monsters[i]->getHP();
		string monsterName = monsters[i]->getName();
		int monsterXP = monsters[i]->getXP();
		int heroHealth = h.getHP();
		Hero updatedHero;

		if (n == i + 1 + '0') {
			while (monsterHealth > 0 && heroHealth > 0) {
				heroHealth -= monsters[i]->getDamage();
				monsterHealth -= h.getDamage();
				cout << "Hero Health: " << heroHealth << endl;
				cout << "Monster health: " << monsterHealth << endl;
			}
			if (monsterHealth <= 0) {
				cout << "You have defeated: " << monsterName << " you recieve " << monsterXP << " XP points " << endl;
				updatedHero = updateLevel(monsterXP, h);
				return updatedHero;
			}
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
Hero Controller::updateLevel(int xp, Hero h) {
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

	int remainder = heroXP - (heroLevel * 1000);

	if (heroXP >= heroLevel * 1000) {
		heroXP = remainder;
		heroLevel += 1;
		heroDamage += 1;
		heroHP += 2;
	}

	Hero updatedHero(h.getName(), heroHP, heroLevel, heroXP, heroDamage, gold, inventorySpace, equippedBonusDamage, h.getWeapons(), heroKills, h.getSelectedWeapon(), h.getHeroID());
	return updatedHero;
}

// This function is used to battle caves
Hero Controller::battleCave(int heroLvl, Hero hero, vector<Monster*> caveMonsters) {

	CaveFactory c;
	Hero updatedHero;
	int heroGold;

	srand(static_cast<unsigned>(time(0)));

	cout << "You have chosen to battle your way through this cave" << endl;
	cout << " " << endl;
	c.printCaveMonsters(caveMonsters);

	int index = 0;
	char action;

	for (Monster* monster : caveMonsters) {
		cout << "\nNext monster appears: " << monster->getName() << endl;
		cout << "Press 1 to fight or 0 to save and exit: " << endl;
		bool correctInput = false;
		while (!correctInput) {
			cin >> action;
			if (action == '0') {
				saveGame(hero);
				correctInput = true;
			}
			else if (action == '1') {
				vector<Monster*> singleMonster = { monster };
				updatedHero = fightMonster('1', hero, singleMonster);
				if (hero.hasWeaponEquipped()) {
					hero.removeWeaponDurability();
				}
				++index;
				correctInput = true;
			}
			else {
				cout << "ERROR: Incorrect user input, please enter either 1 or 0 " << endl;
			}
		}
	}



	if (updatedHero.getLevel() < 7) {
		int heroGold = updatedHero.getGold() + 250;

		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 2, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 250 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 9) {
		int heroGold = updatedHero.getGold() + 500;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 4, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 500 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 11) {
		int heroGold = updatedHero.getGold() + 1000;

		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 5, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 1000 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 15) {
		int heroGold = updatedHero.getGold() + 2000;

		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 6, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
		cout << "Congrats you have defeated the cave and receive 2000 gold" << endl;
		return h;
	}
	else {
		int heroGold = updatedHero.getGold() + 10000;

		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold, updatedHero.getRemainingInventorySpace(), updatedHero.getEquippedBonusDamage(), updatedHero.getWeapons(), updatedHero.getKills() + 6, updatedHero.getSelectedWeapon(), updatedHero.getHeroID());
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
void Controller::buyWeapon(char choice, int heroGold, vector<Weapons*>& weapons, Hero& h) {
	int index = choice;

	Weapons* weapon = weapons[index];

	if (weapon->getPrice() > heroGold) {
		cout << "You do not have enough gold to purchase this weapon. Battle caves to get more gold and come back later." << endl;
		return;
	}

	int newGold = heroGold - weapon->getPrice();
	h.setGold(newGold);

	cout << "\nYou have selected the weapon: " << weapon->getName() << endl;

	if (h.getRemainingInventorySpace() <= 0) {
		int action;
		cout << "You do not have enough inventory space. Choose a weapon to discard (by index) or select (-1) to cancel purchase: " << endl;
		h.showInventory();
		cin >> action;
		if (action != -1) {
			h.deleteInventorySlot(action);
			h.addWeaponToInventory(weapon);
			cout << weapon->getName() << " has been added to your hero's inventory." << endl;
		}
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

	cout << "Remaining inventory space: " << h.getRemainingInventorySpace() << endl;
	cout << "Current hero inventory: " << endl;
	h.showInventory();
}



Controller::~Controller() {}

