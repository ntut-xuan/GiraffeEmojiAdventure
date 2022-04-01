#include "stdafx.h"
#include "GameCharacter.h"
#include "Material.h"
#include <cmath>
using namespace std;

namespace game_framework {

	Character::Character() {
		level = 1;
		exp = 0;
		coin = 0;
		health = 1000;
		attack = 10;
		defence = 10;
		speed = 2;
		goldKeyNumber = 1;
		silverKeyNumber = 1;
		keyNumber = 1;
		status = "¥¿±`";
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

	int Character::getLevel() {
		return level;
	}

	int Character::getCoin() {
		return coin;
	}

	int Character::getExp() {
		return exp;
	}

	int Character::getSpeed() {
		return speed;
	}

	string Character::getStatus() {
		return status;
	}

	void Character::setLevel(int _level) {
		level = _level;
	}

	void Character::setCoin(int _coin) {
		coin = _coin;
	}

	void Character::setExp(int _exp) {
		exp = _exp;
	}

	void Character::setSpeed(int _speed) {
		speed = _speed;
	}

	void Character::setHealth(int _health) {
		health = _health;
	}

	void Character::setAttack(int _attack) {
		attack = _attack;
	}

	void Character::setDefence(int _defence) {
		defence = _defence;
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
		health = max(0, health - damage);
	}

}