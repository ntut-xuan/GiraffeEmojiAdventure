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
		int x, y; // 座標
		int health, attack, defence, speed; // 人物屬性
	private:
		bool isDodged();
	};
}