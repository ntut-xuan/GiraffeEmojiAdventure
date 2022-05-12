#pragma once
#include <string>
#include "GameCharacter.h"
#include "Monster.h"
using namespace std;

namespace game_framework {
	class AttackEvent {
	public:
		void attack_status(bool attackEvent, Character &character, Monster &monster);
		void player_winning_status();
		void monster_winning_status();
	private:
	};
}