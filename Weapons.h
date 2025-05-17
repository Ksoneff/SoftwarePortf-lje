#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>

using namespace std;


class Weapons
{
public:
	Weapons(int w_id, int type_id, string name, int skade, int styrkemodifier, int holdbarhed, int price, int kills);
	~Weapons();
	int getType_id();
	int getWeapon_id();
	string getName();
	int getSkade();
	int getStyrkemodifier();
	int getHoldbarhed();
	int getPrice();
	int getKills();
	void setWeapon_id(int);
	void setHoldbarhed(int newHoldbarhed);
private:
	int weapon_id;
	int type_id;
	string name;
	int skade;
	int styrkemodifier;
	int holdbarhed;
	int price;
	int kills;
};

#endif
