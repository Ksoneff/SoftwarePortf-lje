#include "Controller.h"
#include "Hero.h"
#include "Hest.h"
#include "Monster.h"
#include "WeakGoblin.h"
#include "StrongGoblin.h"
#include "StrongerGoblin.h"
#include "TheStrongestGoblin.h"
#include "ApeKing.h"
#include "Unicorn.h"
#include "Dragon.h"
#include "CaveFactory.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime> // needed for time()
#include <cstdlib> // needed for srand() and rand()

Controller::Controller(){}

Controller::Controller(string n) {

	Hero h;
	Hero Ironman;
	Hero Spiderman;
	Hero Deadpool;
	Hero Batman;
	Hero Level5;
	char newGame;
	char action;
	string newHero;
	string oldHero;
	bool correctInput = false;

	// The flow of the game starts here
	cout << "(1) New Game (0) Load Game (2) Load Premade Hero: " << endl;

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
		h = Hero(newHero, 10, 1, 0, 2, 0);
	}

	if (newGame == '0') {
		cout << "Enter hero name to load: " << endl;
		cout << showHeroes() << endl;
		cin >> oldHero;
		h = loadGame(oldHero);
	}

	if (newGame != '0' && newGame != '1') {
		correctInput = false;
		while (!correctInput) {
			if (newGame == '2') {
				string heroChoice;
				cout << " " << endl;
				cout << "§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§" << endl;
				cout << "Load one of the following heroes: " << endl;
				Ironman = Hero("Ironman", 8, 1, 0, 6, 0);
				cout << " " << endl;
				cout << "Ironman: Strengths - Has cool tricks and can damage other oppenents heavily, Weakness - Has low HP " << endl;
				cout << "HP: " << Ironman.getHP() << "  Level: " << Ironman.getLevel() << "  XP: " << Ironman.getXP() << "  Damage: " << Ironman.getDamage() << "  Gold: " << Ironman.getGold() << endl;
				cout << " " << endl;
				Spiderman = Hero("Spiderman", 12, 1, 0, 3, 0);
				cout << " " << endl;
				cout << "Spiderman: This is a pretty average choice, no real strengths or weaknesses, just your friendly neighboorhod spiderman " << endl;
				cout << "HP: " << Spiderman.getHP() << "  Level: " << Spiderman.getLevel() << "  XP: " << Spiderman.getXP() << "  Damage: " << Spiderman.getDamage() << "  Gold: " << Spiderman.getGold() << endl;
				cout << " " << endl;
				Deadpool = Hero("Deadpool", 20, 1, 0, 1, 0);
				cout << " " << endl;
				cout << "Deadpool: Strength - Regenerates so starts out with double HP, Weakness - doesnt really damage too much " << endl;
				cout << "HP: " << Deadpool.getHP() << "  Level: " << Deadpool.getLevel() << "  XP: " << Deadpool.getXP() << "  Damage: " << Deadpool.getDamage() << "  Gold: " << Deadpool.getGold() << endl;
				cout << " " << endl;
				Batman = Hero("Batman", 10, 1, 0, 2, 500);
				cout << " " << endl;
				cout << "Batman: Strength - HE IS RICH, so you start the game with 500 gold " << endl;
				cout << "HP: " << Batman.getHP() << "  Level: " << Batman.getLevel() << "  XP: " << Batman.getXP() << "  Damage: " << Batman.getDamage() << "  Gold: " << Batman.getGold() << endl;
				cout << " " << endl;
				Level5 = Hero("Level5", 20, 5, 0, 6, 0);
				cout << " " << endl;
				cout << "Level5: This hero is mainly for testing, start at level 5, too instantly explore caves " << endl;
				cout << "HP: " << Level5.getHP() << "  Level: " << Level5.getLevel() << "  XP: " << Level5.getXP() << "  Damage: " << Level5.getDamage() << "  Gold: " << Level5.getGold() << endl;
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
				else if (heroChoice == Level5.getName()) {
					h = Level5;
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
	cout << "HP: " << h.getHP() << "  Level: " << h.getLevel() << "  XP: " << h.getXP() << "  Damage: " << h.getDamage() << "  Gold: " << h.getGold() << endl;
	cout << "--------------------------------------------------------------------" << endl;

	this_thread::sleep_for(chrono::seconds(5));

	bool quitGame = false;

	// While loop that runs indefinetly so that the player can battle as many foes as they want, ends when player writes 0 (to quit)
	while (!quitGame) {
		cout << " " << endl;
		cout << "Either choose to fight a wild monster (W), you may also inspect nearby caves (C), or save and exit (0): ";

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
			cin >> action;
			cout << " " << endl;
			cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << endl;
			cout << " " << endl;
			if (action == '1') {
				h = battleCave(h.getLevel(), h, cave1Monsters);
			}
			else if (action == '2') {
				h = battleCave(h.getLevel(), h, cave2Monsters);
			}
			else if (action == '3') {
			}
		}

		else if (action == 'C' && h.getLevel() < 5) {
			cout << " " << endl;
			cout << "ERROR: Your hero level is too low to explore caves. Fight more wild monsters to level up and unlock caves" << endl;
			cout << " " << endl;
		}

		else if (action == 'W') {

			char monsterChoice;

			// Objects of hero and all enemies
			Hest hest("Hest", 5, 1, 100);
			WeakGoblin g1("Weak Goblin", 4, 2, 200);
			StrongGoblin g2("Strong Goblin", 8, 3, 400);
			StrongerGoblin g3("Stronger Goblin", 10, 4, 500);
			TheStrongestGoblin g4("The Strongest Goblin", 15, 5, 800);
			ApeKing a("Ape King", 30, 5, 1000);
			Unicorn u("Unicorn", 5, 8, 1500);
			Dragon d("Dragon", 100, 10, 3000);

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
			cout << "" << endl;
		}

		else if (action == '0') {
			cout << "Thank you for playing!" << endl;
			cout << "Game saving..." << endl;
			saveGame(h); // Uses save game function to save game to save.txt file
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
	cout << "BUT BE CAREFUL if you accidently battle an enemy before you are ready and lose," << endl;
	cout << "your hero will die and you must start over..." << endl;
	cout << "Now you are ready to play, in order to win you must slay the almighty dragon and free the villagers" << endl;
	cout << "from its tyrani, may the force be with you." << endl;
	cout << "//////////////////////////////////////////////////////////////////////////////////////" << endl;
}

// showHeroes function return a list of all previously used and saved heroes
string Controller::showHeroes() {

	ifstream file("save.txt");

	string name;
	int level, xp, hp, damage, gold;
	string allNames;

	while (file >> name >> level >> xp >> hp >> damage >> gold) {
		allNames += name + "\n";
	}

	file.close();
	return allNames;
}

// saveGame function saves the game when user quits
void Controller::saveGame(Hero h) {

	string name;
	int hp, level, xp, damage, gold;

	// Opens function, writes all hero stats to save.txt and closes file
	ofstream file("save.txt", ios::app);

	file << h.getName() << " "
		<< h.getHP() << " "
		<< h.getLevel() << " "
		<< h.getXP() << " "
		<< h.getDamage() << " "
		<< h.getGold() << "\n";

	file.close();
}

// loadGame function used to load previously saved heroes, to continue quest
Hero Controller::loadGame(string nameToFind) {
	ifstream file("save.txt");

	string name;
	int level, xp, hp, damage, gold;

	while (file >> name >> hp >> level >> xp >> damage >> gold) {
		if (name == nameToFind) {
			return Hero(name, hp, level, xp, damage, gold);
		}
		else { // For handling incorrect load
			cout << "Error: Hero was not found, you have been given default hero" << endl; 
			return Hero("NoobMaster69", 10, 1, 0, 2, 0);
		}
	}

	file.close();
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
	int heroLevel = h.getLevel();
	int heroXP = h.getXP();
	int heroDamage = h.getDamage();
	int heroHP = h.getHP();
	int gold = h.getGold();

	heroXP += xp;

	int remainder = heroXP - (heroLevel * 1000);

	if (heroXP >= heroLevel * 1000) {
		heroXP = remainder;
		heroLevel += 1;
		heroDamage += 1;
		heroHP += 2;
	}

	Hero updatedHero(h.getName(), heroHP, heroLevel, heroXP, heroDamage, gold);
	return updatedHero;
}

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
				cout << "You have defeated " << monster->getName() << ", and receive XP: " << monster->getXP() << endl;
				++index;
				correctInput = true;
			}
			else {
				cout << "ERROR: Incorrect user input, please enter either 1 or 0 " << endl;
			}
		}
	}

	if (updatedHero.getLevel() < 5) {
		int heroGold = updatedHero.getGold() + 100;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold);
		cout << "Congrats you have defeated the cave and receive 100 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 7) {
		int heroGold = updatedHero.getGold() + 250;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold);
		cout << "Congrats you have defeated the cave and receive 250 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 9) {
		int heroGold = updatedHero.getGold() + 500;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold);
		cout << "Congrats you have defeated the cave and receive 500 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 11) {
		int heroGold = updatedHero.getGold() + 1000;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold);
		cout << "Congrats you have defeated the cave and receive 1000 gold" << endl;
		return h;
	}

	else if (updatedHero.getLevel() < 15) {
		int heroGold = updatedHero.getGold() + 2000;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold);
		cout << "Congrats you have defeated the cave and receive 2000 gold" << endl;
		return h;
	}
	else {
		int heroGold = updatedHero.getGold() + 10000;
		Hero h(updatedHero.getName(), updatedHero.getHP(), updatedHero.getLevel(), updatedHero.getXP(), updatedHero.getDamage(), heroGold);
		cout << "Congrats you have defeated the cave and receive 10000 gold" << endl;
		return h;
	}
}

Controller::~Controller(){}
