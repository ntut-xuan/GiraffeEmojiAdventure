#include "stdafx.h"
#include "Monster.h"
#include "Material.h"
#include <cmath>
using namespace std;

namespace game_framework {

	Monster::Monster() {

	}

	Monster::Monster(int _ID, string _name, int _health, int _attack, int _defence, int _speed) {
		ID = _ID;
		name = _name;
		health = _health;
		attack = _attack;
		defence = _defence;
		speed = _speed;
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

	void Monster::causeDamage(int damage) {
		health = (int)fmax(0, health - damage);
	}

}