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
		status = 0;
		statusString = "正常";
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

	int Character::getFacedX() {
		return faced_x;
	}

	int Character::getFacedY() {
		return faced_y;
	}

	int Character::getFacedDirection() {
		return faced_direction;
	}

	int Character::getCurrentStage() {
		return current_stage;
	}

	int Character::getHealth() {
		return health;
	}

	int Character::getRawAttack() {
		return attack;
	}

	int Character::getRawDefence() {
		return defence;
	}

	int Character::getAttack() {
		return status & CAUSE_WEEK ? attack/2 : attack;
	}

	int Character::getDefence() {
		return status & CAUSE_WEEK ? defence/2 : defence;
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

	int Character::getStatus() {
		return status;
	}

	string Character::getStatusString() {
		return statusString;
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

	void Character::setStatus(int _status) {
		int prestatus = status;
		_status > 0 ? status |= _status : status += _status;
		if (status == CAUSE_WEEK) {
			statusString = "衰弱";
		}
		if (status == POSION_ATTACK) {
			statusString = "中毒";
		}
		if (status == (POSION_ATTACK | CAUSE_WEEK)) {
			statusString = "衰弱 + 中毒";
		}
		if (status == 0) {
			statusString = "正常";
		}
	}

	void Character::setFacedXY(int x, int y) {
		faced_x = x;
		faced_y = y;
	}

	void Character::setFacedDirection(int direction) {
		faced_direction = direction;
	}

	void Character::setCurrentStage(int _current_stage) {
		current_stage = _current_stage;
	}

	void Character::levelup() {
		level += 1;
		health += 250;
		attack += 3;
		defence += 3;
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

	void Character::setItemCount(int item, int count){
		itemCount[item] = count;
	}

	int Character::getItemCount(int item) {
		return itemCount[item];
	}

}