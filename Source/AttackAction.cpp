#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "NPC.h"
#include "GameCharacter.h"
#include "Monster.h"
#include "mygame.h"
#include "AttackAction.h"
#include <string>
#include <vector>

namespace game_framework {
	string makeString(int current_stage, int x, int y) {
		return (to_string(current_stage).length() == 1 ? "0" + to_string(current_stage) : to_string(current_stage)) + (to_string(x).length() == 1 ? "0" + to_string(x) : to_string(x)) + (to_string(y).length() == 1 ? "0" + to_string(y) : to_string(y));
	}
	void AttackAction::attackAction(int current_stage, int x, int y, Character &character, Monster &Monster) {
		//TODO: move attack action to here
	}
	void AttackAction::afterAttack(int current_stage, int x, int y) {
		string str = makeString(current_stage, x, y);
		if (str == "46_00_05") {

		}
	}
}