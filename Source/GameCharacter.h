#include <string>
#include <map>
#pragma once
namespace game_framework {
	class Character {
	public:
		Character();
		Character(int health, int attack, int defence, int speed);
		int getX();
		int getY();
		int getFacedX();
		int getFacedY();
		int getCurrentStage();
		int getFacedDirection();
		int getHealth();
		int getAttack();
		int getRawAttack();
		int getDefence();
		int getRawDefence();
		int getLevel();
		int getSpeed();
		int getCoin();
		int getExp();
		int getGoldKeyNumber();
		int getSilverKeyNumber();
		int getKeyNumber();
		int getStatus();
		int getCharacterIndex() {
			return character_index;
		}
		std::string getStatusString();
		void causeDamage(int damage);
		void setXY(int x, int y);
		void setFacedXY(int x, int y);
		void setFacedDirection(int direction);
		void setCurrentStage(int current_stage);
		void setHealth(int health);
		void setAttack(int attack);
		void setDefence(int defence);
		void setSpeed(int speed);
		void setLevel(int level);
		void setCoin(int coin);
		void setExp(int exp);
		void levelup();
		void setItemCount(int item, int count);
		void setCharacterIndex(int index) {
			character_index = index;
		}
		void setStatus(int status);
		int getItemCount(int item);
		void changeKeyNumber(int keyCode, int amount);
	protected:
		int x, y;
		int faced_x, faced_y;
		int faced_direction;
		int current_stage;
		int level, health, attack, defence, speed, coin, exp, status;
		int goldKeyNumber, silverKeyNumber, keyNumber;
		int character_index = 0;
		std::string statusString;
		std::map<int, int> itemCount;
	};
}