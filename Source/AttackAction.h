#pragma once
namespace game_framework {
	class AttackAction {
	public:
		void attackAction(int current_stage, int x, int y, Character &character, Monster &Monster);
		void afterAttack(int current_stage, int x, int y);
	};
}