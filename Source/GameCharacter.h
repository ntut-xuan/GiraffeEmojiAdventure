#pragma once
namespace game_framework {
	class Character {
	public:
		Character();
		Character(int health, int attack, int defence, int speed);
		int getX();
		int getY();
		int getHealth();
		int getAttack();
		int getDefence();
		int getSpeed();
		void causeDamage(int damage);
		void setXY(int x, int y);
	protected:
		int x, y; // �y��
		int health, attack, defence, speed; // �H���ݩ�
	private:
		bool isDodged();
	};
}