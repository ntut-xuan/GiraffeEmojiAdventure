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
		specialEventName[N] = "�L";
		specialEventName[RESPAWN_53] = "��������";
		specialEventName[RESPAWN_52] = "��������";
		specialEventName[ONE_SHOT_ATTACK] = "�@������";
		specialEventName[IGNORE_DEFENCE] = "���m�L��";
		specialEventName[CAUSE_WEEK] = "��z�y��";
		specialEventName[POSION_ATTACK] = "�r����";
		specialEventName[HARD_HARD_HARD] = "�ܵw";;
		specialEventName[MULTIPLE_ATTACK] = "�h�q����";
		specialEventName[DODGE_ATTACK] = "�Q�{�׮ɧ���";
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