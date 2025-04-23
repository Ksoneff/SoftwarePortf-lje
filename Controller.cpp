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
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

Controller::Controller(){}

Controller::Controller(string n) {

	// Objects of hero and all enemies
	Hero h;
	Hest hest;
	WeakGoblin g1;
	StrongGoblin g2;
	StrongerGoblin g3;
	TheStrongestGoblin g4;
	ApeKing a;
	Unicorn u;
	Dragon d;

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

	char newGame;
	char action;
	string newHero;
	string oldHero;
	bool correctInput = false;

	// The flow of the game starts here
	cout << "(1) New Game (0) Load Game: " << endl;

	while (!correctInput) { // While loop checking for icorrect user input
		cin >> newGame;
		if (newGame == '0' || newGame == '1') {
			correctInput = true;
		}
		else {
			cout << "Error incorrect user input, please enter either 0 or 1" << endl;
			correctInput = false;
		}
	}

	if (newGame == '1') {
		cout << "Create new hero, enter name without any spaces: " << endl;
		cin >> newHero;
		h = Hero(newHero, 10, 1, 0, 2);
	}

	else {
		cout << "Enter hero name to load: " << endl;
		cout << showHeroes() << endl;
		cin >> oldHero;
		h = loadGame(oldHero);
	}

	// Uses showRules() function to show rules
	showRules();

	this_thread::sleep_for(chrono::seconds(40)); // For pausing to read rules, taken from chatGBT :)

	bool quitGame = false;

	cout << "" << endl;
	cout << "" << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "Current hero stats are: " << endl;
	cout << "HP: " << h.getHP() << "  Level: " << h.getLevel() << "  XP: " << h.getXP() << "  Damage: " << h.getDamage() << endl;
	cout << "--------------------------------------------------------------------" << endl;

	this_thread::sleep_for(chrono::seconds(5));

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

	this_thread::sleep_for(chrono::seconds(5));

	// While loop that runs indefinetly so that the player can battle as many foes as they want, ends when player writes 0 (to quit)
	while (!quitGame) {
		cout << "Choose monster to fight (by index number), or type: '0' to save and exit: ";

		correctInput = false;

		cin >> action;

		while (!correctInput) {

			if (action != '0' && action != '1' && action != '2' && action != '3' && action != '4' && action != '5' && action != '6' && action != '7' && action != '8') {
				cout << "No monster has this index, please enter either 0 to quit or number 1-8 to choose monster" << endl;
				cin >> action;
			}

			else if (action == '0') {
				cout << "Thank you for playing!" << endl;
				cout << "Game saving..." << endl;
				saveGame(h); // Uses save game function to save game to save.txt file
				cout << "Game saved succesfully!" << endl;
				exit(0); // exits program instantly
			}

			else {
				correctInput = true;
			}
		}

		h = fightMonster(action, h, enemies);

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
		cout << "" << endl;
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
	int level, xp, hp, damage;
	string allNames;

	while (file >> name >> level >> xp >> hp >> damage) {
		allNames += name + "\n";
	}

	file.close();
	return allNames;
}

// saveGame function saves the game when user quits
void Controller::saveGame(Hero h) {

	// Opens function, writes all hero stats to save.txt and closes file
	ofstream file("save.txt", ios::app);

	file << h.getName() << " "
		<< h.getHP() << " "
		<< h.getLevel() << " "
		<< h.getXP() << " "
		<< h.getDamage() << "\n";

	file.close();
}

// loadGame function used to load previously saved heroes, to continue quest
Hero Controller::loadGame(string nameToFind) {
	ifstream file("save.txt");

	string name;
	int level, xp, hp, damage;

	while (file >> name >> hp >> level >> xp >> damage) {
		if (name == nameToFind) {
			return Hero(name, hp, level, xp, damage);
		}
		else { // For handling incorrect load
			cout << "Error: Hero was not found, you have been given default hero" << endl; 
			return Hero("NoobMaster69", 10, 1, 0, 2);
		}
	}

	file.close();
}

// fightMonster function, handles hero vs monster battles
Hero Controller::fightMonster(char n, Hero h, vector<Monster*> monsters) {

	for (int i = 0; i < monsters.size(); i++) {

		int monsterHealth = monsters[i]->getHP();
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
				cout << "You have defeated hest, you recieve 100 XP points! " << endl;
				updatedHero = updateLevel(monsters[i]->getXP(), h);
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

	heroXP += xp;

	int remainder = heroXP - (heroLevel * 1000);

	if (heroXP >= heroLevel * 1000) {
		heroXP = remainder;
		heroLevel += 1;
		heroDamage += 1;
		heroHP += 2;
	}

	Hero updatedHero(h.getName(), heroHP, heroLevel, heroXP, heroDamage);
	return updatedHero;
}

Controller::~Controller(){}
