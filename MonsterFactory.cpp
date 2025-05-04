#include "MonsterFactory.h"

Monster* MonsterFactory::create(Type t) {
	switch (t) 
	{
	case HEST:
		return new Hest();
	case WEAKGOBLIN:
		return new WeakGoblin();
	case STRONGGOBLIN:
		return new StrongGoblin();
	case STRONGERGOBLIN:
		return new StrongerGoblin();
	case THESTRONGESTGOBLIN:
		return new TheStrongestGoblin();
	case APEKING:
		return new ApeKing();
	case UNICORN:
		return new Unicorn();
	case DRAGON:
		return new Dragon();
	}
}
