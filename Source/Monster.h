#pragma once
#include <string>
using namespace std;

namespace game_framework {
	class Monster {
	public:
		Monster();
		Monster(int ID, string name, int health, int attack, int defence, int speed, int attack_count, int gold, int exp);
		string getName();
		int getHealth();
		int getAttack();
		int getDefence();
		int getSpeed();
		int getID();
		int getAttackCount();
		int getGold();
		int getExp();
		void causeDamage(int damage);
	protected:
		string name;
		int ID, health, attack, defence, speed, attack_count, gold, exp; // ¤Hª«ÄÝ©Ê
	private:
		bool isDodged();
	};
}