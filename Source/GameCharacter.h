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
		int getGoldKeyNumber();
		int getSilverKeyNumber();
		int getKeyNumber();
		void causeDamage(int damage);
		void setXY(int x, int y);
		void changeKeyNumber(int keyCode, int amount);
	protected:
		int x, y; // 座標
		int health, attack, defence, speed; // 人物屬性
		int goldKeyNumber, silverKeyNumber, keyNumber;
	private:
		bool isDodged();
	};
}