#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>

using namespace std;


class Weapons
{
public:
	Weapons(int type_id, string name, int skade, int styrkemodifier, int holdbarhed, int price, int kills);
	~Weapons();
	int getType_id();
	string getName();
	int getSkade();
	int getStyrkemodifier();
	int getHoldbarhed();
	int getPrice();
	int getKills();
	void setHoldbarhed(int newHoldbarhed);
private:
	int type_id;
	string name;
	int skade;
	int styrkemodifier;
	int holdbarhed;
	int price;
	int kills;
};

#endif
