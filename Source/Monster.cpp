#include "stdafx.h"
#include "Monster.h"
#include "Material.h"
#include <cmath>
using namespace std;

namespace game_framework {

	Monster::Monster() {

	}

	Monster::Monster(int _ID, string _name, int _health, int _attack, int _defence, int _speed, int _attack_count, int _gold, int _exp) {
		ID = _ID;
		name = _name;
		health = _health;
		attack = _attack;
		defence = _defence;
		speed = _speed;
		attack_count = _attack_count;
		gold = _gold;
		exp = _exp;
	}

	int Monster::getID() {
		return ID;
	}

	string Monster::getName() {
		return name;
	}

	int Monster::getHealth() {
		return health;
	}

	int Monster::getAttack() {
		return attack;
	}

	int Monster::getDefence() {
		return defence;
	}

	int Monster::getSpeed() {
		return speed;
	}

	int Monster::getExp() {
		return exp;
	}

	int Monster::getGold() {
		return gold;
	}

	int Monster::getAttackCount() {
		return attack_count;
	}

	void Monster::causeDamage(int damage) {
		health = (int)fmax(0, health - damage);
	}

}