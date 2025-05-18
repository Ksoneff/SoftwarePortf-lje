#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>

using namespace std;


class Weapons
{
public:
	Weapons(int type_id, string name, int skade, int styrkemodifier, int holdbarhed, int price, int kills, int w_id = -1);
	~Weapons();
	int getType_id();
	int getWeapon_id();
	string getName();
	int getSkade();
	int getStyrkemodifier();
	int getHoldbarhed();
	int getPrice();
	int getKills();
	void incrementKills();
	void setWeapon_id(int);
	void setHoldbarhed(int newHoldbarhed);
private:
	int weapon_id = -1;
	int type_id;
	string name;
	int skade;
	int styrkemodifier;
	int holdbarhed;
	int price;
	int kills;
};

#endif
