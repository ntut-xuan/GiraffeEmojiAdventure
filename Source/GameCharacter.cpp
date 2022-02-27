#include "stdafx.h"
#include "GameCharacter.h"
#include <cmath>
using namespace std;

namespace game_framework {

	Character::Character() {
		health = 1000;
		attack = 10;
		defence = 10;
		speed = 2;
	}

	Character::Character(int _health, int _attack, int _defence, int _speed) {
		health = _health;
		attack = _attack;
		defence = _defence;
		speed = _speed;
	}

	int Character::getHealth() {
		return health;
	}

	int Character::getAttack() {
		return attack;
	}

	int Character::getDefence() {
		return defence;
	}

	int Character::getSpeed() {
		return speed;
	}

	void Character::causeDamage(int damage) {
		health = (int) fmin(0, health - damage);
	}

}