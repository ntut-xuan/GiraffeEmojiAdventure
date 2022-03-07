#include "stdafx.h"
#include "GameCharacter.h"
#include "Material.h"
#include <cmath>
using namespace std;

namespace game_framework {

	Character::Character() {
		health = 1000;
		attack = 10;
		defence = 10;
		speed = 2;
		goldKeyNumber = 0;
		silverKeyNumber = 0;
		keyNumber = 1;
	}

	Character::Character(int _health, int _attack, int _defence, int _speed) {
		health = _health;
		attack = _attack;
		defence = _defence;
		speed = _speed;
	}

	void Character::setXY(int _x, int _y) {
		x = _x;
		y = _y;
	}

	int Character::getX() {
		return x;
	}

	int Character::getY() {
		return y;
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

	int Character::getGoldKeyNumber() {
		return goldKeyNumber;
	}

	int Character::getSilverKeyNumber() {
		return silverKeyNumber;
	}

	int Character::getKeyNumber() {
		return keyNumber;
	}

	void Character::changeKeyNumber(int keyCode, int amount) {
		if (keyCode == KEY) {
			keyNumber += amount;
		}
		if (keyCode == SILVER_KEY) {
			silverKeyNumber += amount;
		}
		if (keyCode == GOLD_KEY) {
			goldKeyNumber += amount;
		}
	}

	void Character::causeDamage(int damage) {
		health = (int)fmax(0, health - damage);
	}

}