#include <string>
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
		int getLevel();
		int getSpeed();
		int getCoin();
		int getExp();
		int getGoldKeyNumber();
		int getSilverKeyNumber();
		int getKeyNumber();
		std::string getStatus();
		void causeDamage(int damage);
		void setXY(int x, int y);
		void setHealth(int health);
		void setAttack(int attack);
		void setDefence(int defence);
		void setSpeed(int speed);
		void setLevel(int level);
		void setCoin(int coin);
		void setExp(int exp);
		void changeKeyNumber(int keyCode, int amount);
	protected:
		int x, y; // 座標
		int level, health, attack, defence, speed, coin, exp; // 人物屬性
		int goldKeyNumber, silverKeyNumber, keyNumber;
		std::string status;
	};
}