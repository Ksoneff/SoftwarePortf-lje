#include "MonsterFactory.h"
#include "Hest.h"
#include "WeakGoblin.h"
#include "StrongGoblin.h"
#include "StrongerGoblin.h"
#include "TheStrongestGoblin.h"
#include "ApeKing.h"
#include "Unicorn.h"
#include "Dragon.h"


Monster* MonsterFactory::create(Type t) {
	int hp, damage, xp;

	xp = 0;

	switch (t) 
	{
	case HEST:
		hp = 4 + rand() % 5; // 4 - 8
		damage = 1 + rand() % 3; // 1 - 4
		if (hp == 8 && damage == 4) {
			xp == 1500;
			return new Hest("The most INSANE Hest EVER", hp, damage, xp);
		}
		if (hp > 6 && damage > 2) {
			xp = 250;
			return new Hest("bonusXP Hest", hp, damage, xp);
		} else {
			xp = 100;
			return new Hest("Hest", hp, damage, xp);
		}

	case WEAKGOBLIN:
		hp = 2 + rand() % 5; // 2 - 6
		damage = 2 + rand() % 3; // 2 - 5
		if (hp == 6 && damage == 5) {
			xp = 2000;
			return new WeakGoblin("The most INSANE Weak Goblin EVER", hp, damage, xp);
		}
		else if (hp > 4 && damage > 3) {
			xp = 500;
			return new WeakGoblin("BonusXP Weak Goblin", hp, damage, xp);
		} else {
			xp = 200;
			return new WeakGoblin("Weak Goblin", hp, damage, xp);
		}

	case STRONGGOBLIN:
		hp = 6 + rand() % 6; // 6 - 12
		damage = 2 + rand() % 3; // 2 - 5
		if (hp == 12 && damage == 5) {
			xp = 2500;
			return new StrongGoblin("The most INSANE Strong Goblin EVER", hp, damage, xp);
		}
		else if (hp > 8 && damage > 3) {
			xp = 750;
			return new StrongGoblin("BonusXP Strong Goblin", hp, damage, xp);
		} else {
			xp = 400;
			return new StrongGoblin("Strong Goblin", hp, damage, xp);
		}

	case STRONGERGOBLIN:
		hp = 7 + rand() % 7; // 7 - 14
		damage = 3 + rand() % 3; // 3 - 6
		if (hp == 14 && damage == 6) {
			xp = 3500;
			return new StrongerGoblin("The most INSANE Stronger Goblin EVER", hp, damage, xp);
		}
		else if (hp > 11 && damage > 4) {
			xp = 1000;
			return new StrongerGoblin("BonusXP Stronger Goblin", hp, damage, xp);
		} else {
			xp = 500;
			return new StrongerGoblin("Stronger Goblin", hp, damage, xp);
		}

	case THESTRONGESTGOBLIN:
		hp = 12 + rand() % 6; // 12 - 18
		damage = 4 + rand() % 4; // 4 -8
		if (hp == 18 && damage == 8) {
			xp = 4000;
			return new TheStrongestGoblin("The most INSANE The Strongest Goblin EVER", hp, damage, xp);
		}
		else if (hp > 15 && damage > 5) {
			xp = 1500;
			return new TheStrongestGoblin("BonusXP The Strongest Goblin", hp, damage, xp);
		} else {
			xp = 800;
			return new TheStrongestGoblin("The Strongest Goblin", hp, damage, xp);
		}

	case APEKING:
		hp = 25 + rand() % 10; // 25 - 35
		damage = 4 + rand() % 4; // 4 - 8
		if (hp == 35 && damage == 8) {
			xp = 5000;
			return new ApeKing("The most INSANE Ape King EVER", hp, damage, xp);
		} 
		else if (hp > 30 && damage > 5) {
			xp = 2000;
			return new ApeKing("BonusXP Ape King", hp, damage, xp);
		} else {
			xp = 1000;
			return new ApeKing("Ape King", hp, damage, xp);
		}
	case UNICORN:
		hp = 4 + rand() % 4; // 4 - 8
		damage = 6 + rand() % 6; // 6 - 12
		if (hp == 8 && damage == 12) {
			xp = 6000;
			return new Unicorn("The most INSANE and BEAUTIFUL Unicorn EVER", hp, damage, xp);
		}
		else if (hp > 5 && damage > 8) {
			xp = 3000;
			return new Unicorn("BonusXP Unicorn", hp, damage, xp);
		} else {
			xp = 1500;
			return new Unicorn("Unicorn", hp, damage, xp);
		}
	case DRAGON:
		hp = 90 + rand() % 30; // 90 - 120
		damage = 8 + rand() % 6; // 8 - 14
		if (hp == 120 && damage == 14) {
			xp = 25000;
			return new Dragon("The MOST INSANE AND ALMIGHTY Dragon EVER", hp, damage, xp);
		}
		else if (hp > 110 && damage > 10) {
			xp = 6000;
			return new Dragon("BonusXP Dragon", hp, damage, xp);
		} else {
			xp = 3000;
			return new Dragon("Dragon", hp, damage, xp);
		}
	}
}
