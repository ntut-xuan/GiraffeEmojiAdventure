#include "stdafx.h"
#include "AttackEvent.h"
#include "Material.h"
#include <cmath>
using namespace std;

namespace game_framework {
	void AttackEvent::attack_status(bool attackEvent, Character &character, Monster &monster) {
		last_time = clock();
		if (character.getHealth() && monster.getHealth()) {
			if (turn) {
				attack_menu_animation_select = 1;
				int probabilityCauseDamage = rand() % 100;
				int causeDamageValue;
				if (probabilityCauseDamage < monster.getSpeed()) {
					causeDamageValue = 0;
					causeDamageValueString = "Miss!";
				}
				else {
					causeDamageValue = max(1, character.getAttack() - monster.getDefence());
					causeDamageValueString = to_string(causeDamageValue);
				}

				monster.causeDamage(causeDamageValue);
				enemyAttackAnimation[0].ToggleAnimation(100);
				monsterAttackCount = monster.getAttackCount();
				tempDelayCycle = MENU_DELAY / monster.getAttackCount();
				if (!monster.getHealth()) {
					tempDelayCycle = MENU_DELAY;
				}
				turn = !turn;
			}
			else {
				attack_menu_animation_select = 0;
				int probabilityCauseDamage = rand() % 100;
				int causeDamageValue;
				if (probabilityCauseDamage < character.getSpeed()) {
					causeDamageValue = 0;
					causeDamageValueString = "Miss!";
				}
				else {
					causeDamageValue = max(0, monster.getAttack() - character.getDefence());
					causeDamageValueString = to_string(causeDamageValue);
				}
				character.causeDamage(causeDamageValue);
				characterAttackAnimation[0].ToggleAnimation(100);
				if (--monsterAttackCount == 0) {
					turn = !turn;
					tempDelayCycle = MENU_DELAY;
					monsterAttackCount = monster.getAttackCount();
				}
			}
			showAttackValue = true;
		}

		if (enterStatus) {
			showAttackValue = false;
		}

		if (!enterStatus && monster.getHealth() == 0) {
			character.setExp(character.getExp() + monster.getExp());
			character.setCoin(character.getCoin() + monster.getGold());
			enterStatus = true;
			turn = false;
		}
	}
}