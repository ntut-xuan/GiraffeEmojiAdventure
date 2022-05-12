#include "stdafx.h"
#include "Monster.h"
#include "Material.h"
#include <cmath>
#include <map>
using namespace std;

namespace game_framework {

	Monster::Monster() {

	}

	Monster::Monster(int _ID, string _name, int _health, int _attack, int _defence, int _speed, int _attack_count, int _gold, int _exp, int _special_event) {
		ID = _ID;
		name = _name;
		health = _health;
		attack = _attack;
		defence = _defence;
		speed = _speed;
		attack_count = _attack_count;
		gold = _gold;
		exp = _exp;
		special_event = _special_event;
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

	int Monster::getSpecialEvent() {
		return special_event;
	}

	string Monster::getSpecialEventName() {
		map<int, string> specialEventName;
		specialEventName[N] = "無";
		specialEventName[RESPAWN_53] = "不死重生";
		specialEventName[RESPAWN_52] = "不死重生";
		specialEventName[ONE_SHOT_ATTACK] = "一擊必殺";
		specialEventName[IGNORE_DEFENCE] = "防禦無視";
		specialEventName[CAUSE_WEEK] = "虛弱造成";
		specialEventName[POSION_ATTACK] = "毒攻擊";
		specialEventName[HARD_HARD_HARD] = "變硬";;
		specialEventName[MULTIPLE_ATTACK] = "多段攻擊";
		specialEventName[DODGE_ATTACK] = "被閃避時攻擊";
		return specialEventName[special_event];
	}

	void Monster::setAttack(int _attack) {
		attack = _attack;
	}

	void Monster::setDefence(int _defence) {
		defence = _defence;
	}

	void Monster::causeDamage(int damage) {
		health = (int)fmax(0, health - damage);
	}

}