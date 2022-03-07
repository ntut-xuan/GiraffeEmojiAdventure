#pragma once
#include <string>
using namespace std;

namespace game_framework {
	class Monster {
	public:
		Monster();
		Monster(string name, int health, int attack, int defence, int speed);
		string getName();
		int getHealth();
		int getAttack();
		int getDefence();
		void causeDamage(int damage);
	protected:
		string name;
		int health, attack, defence, speed; // ¤Hª«ÄÝ©Ê
	private:
		bool isDodged();
	};
}