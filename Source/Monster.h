#pragma once
#include <string>
using namespace std;

namespace game_framework {
	class Monster {
	public:
		Monster();
		Monster(int ID, string name, int health, int attack, int defence, int speed, int attack_count, int gold, int exp, int special_event);
		string getName();
		int getHealth();
		int getAttack();
		int getDefence();
		int getSpeed();
		int getID();
		int getAttackCount();
		int getGold();
		int getExp();
		int getSpecialEvent();
		string getSpecialEventName();
		void setAttack(int attack);
		void setDefence(int defence);
		void causeDamage(int damage);
	protected:
		string name;
		int special_event;
		int ID, health, attack, defence, speed, attack_count, gold, exp; // ¤Hª«ÄÝ©Ê
	private:
		bool isDodged();
	};
}