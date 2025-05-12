#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>

using namespace std;


class Weapons
{
public:
	Weapons(string name, int skade, int styrkemodifier, int holdbarhed, int price);
	~Weapons();
	string getName();
	int getSkade();
	int getStyrkemodifier();
	int getHoldbarhed();
	int getPrice();

private:
	string name;
	int skade;
	int styrkemodifier;
	int holdbarhed;
	int price;
};

#endif
