#include "MonsterFactory.h"


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
			return new Monster("The most INSANE Hest EVER", hp, damage, xp);
		}
		if (hp > 6 && damage > 2) {
			xp = 250;
			return new Monster("bonusXP Hest", hp, damage, xp);
		} else {
			xp = 100;
			return new Monster("Hest", hp, damage, xp);
		}

	case WEAKGOBLIN:
		hp = 2 + rand() % 5; // 2 - 6
		damage = 2 + rand() % 3; // 2 - 5
		if (hp == 6 && damage == 5) {
			xp = 2000;
			return new Monster("The most INSANE Weak Goblin EVER", hp, damage, xp);
		}
		else if (hp > 4 && damage > 3) {
			xp = 500;
			return new Monster("BonusXP Weak Goblin", hp, damage, xp);
		} else {
			xp = 200;
			return new Monster("Weak Goblin", hp, damage, xp);
		}

	case STRONGGOBLIN:
		hp = 6 + rand() % 6; // 6 - 12
		damage = 2 + rand() % 3; // 2 - 5
		if (hp == 12 && damage == 5) {
			xp = 2500;
			return new Monster("The most INSANE Strong Goblin EVER", hp, damage, xp);
		}
		else if (hp > 8 && damage > 3) {
			xp = 750;
			return new Monster("BonusXP Strong Goblin", hp, damage, xp);
		} else {
			xp = 400;
			return new Monster("Strong Goblin", hp, damage, xp);
		}

	case STRONGERGOBLIN:
		hp = 7 + rand() % 7; // 7 - 14
		damage = 3 + rand() % 3; // 3 - 6
		if (hp == 14 && damage == 6) {
			xp = 3500;
			return new Monster("The most INSANE Stronger Goblin EVER", hp, damage, xp);
		}
		else if (hp > 11 && damage > 4) {
			xp = 1000;
			return new Monster("BonusXP Stronger Goblin", hp, damage, xp);
		} else {
			xp = 500;
			return new Monster("Stronger Goblin", hp, damage, xp);
		}

	case THESTRONGESTGOBLIN:
		hp = 12 + rand() % 6; // 12 - 18
		damage = 4 + rand() % 4; // 4 -8
		if (hp == 18 && damage == 8) {
			xp = 4000;
			return new Monster("The most INSANE The Strongest Goblin EVER", hp, damage, xp);
		}
		else if (hp > 15 && damage > 5) {
			xp = 1500;
			return new Monster("BonusXP The Strongest Goblin", hp, damage, xp);
		} else {
			xp = 800;
			return new Monster("The Strongest Goblin", hp, damage, xp);
		}

	case APEKING:
		hp = 25 + rand() % 10; // 25 - 35
		damage = 4 + rand() % 4; // 4 - 8
		if (hp == 35 && damage == 8) {
			xp = 5000;
			return new Monster("The most INSANE Ape King EVER", hp, damage, xp);
		} 
		else if (hp > 30 && damage > 5) {
			xp = 2000;
			return new Monster("BonusXP Ape King", hp, damage, xp);
		} else {
			xp = 1000;
			return new Monster("Ape King", hp, damage, xp);
		}
	case UNICORN:
		hp = 4 + rand() % 4; // 4 - 8
		damage = 6 + rand() % 6; // 6 - 12
		if (hp == 8 && damage == 12) {
			xp = 6000;
			return new Monster("The most INSANE and BEAUTIFUL Unicorn EVER", hp, damage, xp);
		}
		else if (hp > 5 && damage > 8) {
			xp = 3000;
			return new Monster("BonusXP Unicorn", hp, damage, xp);
		} else {
			xp = 1500;
			return new Monster("Unicorn", hp, damage, xp);
		}
	case DRAGON:
		hp = 90 + rand() % 30; // 90 - 120
		damage = 8 + rand() % 6; // 8 - 14
		if (hp == 120 && damage == 14) {
			xp = 25000;
			return new Monster("The MOST INSANE AND ALMIGHTY Dragon EVER", hp, damage, xp);
		}
		else if (hp > 110 && damage > 10) {
			xp = 6000;
			return new Monster("BonusXP Dragon", hp, damage, xp);
		} else {
			xp = 3000;
			return new Monster("Dragon", hp, damage, xp);
		}
	}
}
