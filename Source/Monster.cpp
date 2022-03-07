#include "stdafx.h"
#include "Monster.h"
#include "Material.h"
#include <cmath>
using namespace std;

namespace game_framework {

	Monster::Monster() {

	}

	Monster::Monster(string _name, int _health, int _attack, int _defence, int _speed) {
		name = _name;
		health = _health;
		attack = _attack;
		defence = _defence;
		speed = _speed;
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