#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>

using namespace std;


class Weapons
{
public:
	Weapons(int weapon_id, string name, int skade, int styrkemodifier, int holdbarhed, int price, int kills);
	~Weapons();
	int getWeapon_id();
	string getName();
	int getSkade();
	int getStyrkemodifier();
	int getHoldbarhed();
	int getPrice();
	int getKills();
	void setHoldbarhed(int newHoldbarhed);
private:
	int weapon_id;
	string name;
	int skade;
	int styrkemodifier;
	int holdbarhed;
	int price;
	int kills;
};

#endif
