#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "Event.h"
#include <string>
#include <vector>

namespace game_framework {

	vector<vector<vector<int>>> Event::visited = vector<vector<vector<int>>>(50, vector<vector<int>>(11, vector<int>(11)));

	void Event::triggerDialogEvent(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 26 && x == 8 && y == 5 && !visited[current_stage][x][y]) {
			dialogMenu.setTemporaryDialog(DialogMenu::DOWN, { "�N�O�o�̤F", "��i�h..." }, enterStatus, dialogMenuing);
			visited[current_stage][x][y] = true;
		}
	}
	void Event::triggerAfterDialog(int current_stage, int x, int y) {
		if (current_stage == 46 && x == 0 && y == 5) {
			if (CGameStateRun::hidden_code[current_stage][0][5] == 0) {
				CGameStateRun::stage_material[current_stage][3][5] = WALL;
				CGameStateRun::stage_entity[current_stage][0][5] = FAKE_PRINCESS;
			}
			else {
				CGameStateRun::stage_material[current_stage][3][5] = ROAD;
				CGameStateRun::stage_entity[current_stage][0][0] = KEY;
				CGameStateRun::stage_entity[current_stage][0][1] = RED_POTION;
				CGameStateRun::stage_entity[current_stage][0][2] = RED_POTION;
				CGameStateRun::stage_entity[current_stage][0][3] = RED_POTION;
				CGameStateRun::stage_entity[current_stage][0][5] = HOE;
				CGameStateRun::stage_entity[current_stage][0][7] = RED_POTION;
				CGameStateRun::stage_entity[current_stage][0][8] = RED_POTION;
				CGameStateRun::stage_entity[current_stage][0][9] = RED_POTION;
				CGameStateRun::stage_entity[current_stage][0][10] = KEY;
				CGameStateRun::stage_entity[current_stage][1][0] = SILVER_KEY;
				CGameStateRun::stage_entity[current_stage][1][1] = SAPPHIRE;
				CGameStateRun::stage_entity[current_stage][1][2] = SAPPHIRE;
				CGameStateRun::stage_entity[current_stage][1][3] = SAPPHIRE;
				CGameStateRun::stage_entity[current_stage][1][7] = RUBY;
				CGameStateRun::stage_entity[current_stage][1][8] = RUBY;
				CGameStateRun::stage_entity[current_stage][1][9] = RUBY;
				CGameStateRun::stage_entity[current_stage][1][10] = SILVER_KEY;
				CGameStateRun::stage_entity[current_stage][4][0] = DOWN_STAIR;
				CGameStateRun::stage_entity[current_stage][4][10] = DOWN_STAIR;
				CGameStateRun::hidden_code[current_stage][0][5] = 0;
			}
		}
	}
	void Event::triggerGetItem(int ID, Character &character, string &temp_item_info) {
		if (ID == RUBY) {
			character.setAttack(character.getAttack() + 2);
			temp_item_info = "���_�ۡA�����O + 2";
		}
		if (ID == SAPPHIRE) {
			character.setDefence(character.getDefence() + 2);
			temp_item_info = "���_�ۡA���m�O + 2";
		}
		if (ID == EMERALD) {
			character.setSpeed(character.getSpeed() + 2);
			temp_item_info = "���_�ۡA�ӱ� + 2";
		}
		if (ID == RED_POTION) {
			character.setHealth(character.getHealth() + 150);
			temp_item_info = "���Ĥ��A��q + 150";
		}
		if (ID == BLUE_POTION) {
			character.setHealth(character.getHealth() + 400);
			temp_item_info = "���Ĥ��A��q + 400";
		}
		if (ID == KEY) {
			character.changeKeyNumber(KEY, 1);
			temp_item_info = "���_��";
		}
		if (ID == SILVER_KEY) {
			character.changeKeyNumber(SILVER_KEY, 1);
			temp_item_info = "���_��";
		}
		if (ID == GOLD_KEY) {
			character.changeKeyNumber(GOLD_KEY, 1);
			temp_item_info = "���_��";
		}
		if (ID == FIRE_DRINK) {
			temp_item_info = "���s�A�^�_�I�z���A";
		}
		if (ID == ANTI_POTION) {
			temp_item_info = "�ܬr���A�^�_�r���A";
		}
		if (ID == JUMP_WING) {
			character.setLevel(character.getLevel() + 1);
			temp_item_info = "�D�i���l�A���Ŵ��ɤ@";
		}
		if (ID == SHIELD_D) {
			character.setDefence(character.getDefence() + 7);
			temp_item_info = "�֭����ޡA���m�O + 7";
		}
		if (ID == GEM_DIGGER) {
			temp_item_info = "�_�۾S�Y";
			character.setItemCount(GEM_DIGGER, character.getItemCount(GEM_DIGGER) + 1);
		}
		if (ID == COIN_200) {
			character.setCoin(character.getCoin() + 200);
			temp_item_info = "���� 200 ��";
		}
		if (ID == SWORD_C) {
			character.setAttack(character.getAttack() + 16);
			temp_item_info = "���C�A�����O + 16";
		}
		if (ID == SHIELD_C) {
			character.setAttack(character.getDefence() + 22);
			temp_item_info = "�����ޡA���m�O + 22";
		}
		if (ID == MAGIC_KEY) {
			character.changeKeyNumber(KEY, 1);
			character.changeKeyNumber(SILVER_KEY, 1);
			character.changeKeyNumber(GOLD_KEY, 1);
			temp_item_info = "�]�k���͡A�Ҧ���ͼ� + 1";
		}
		if (ID == SWORD_D) {
			character.setAttack(character.getAttack() + 8);
			temp_item_info = "�K�C�A�����O + 8";
		}
		if (ID == HOE) {
			temp_item_info = "�S�Y";
			character.setItemCount(HOE, character.getItemCount(HOE) + 1);
		}
	}
	void Event::triggerAfterAttack(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 41 && x == 3 && y == 5) {
			CGameStateRun::hidden_code[current_stage][1][4] = 1;
			CGameStateRun::hidden_code[current_stage][1][5] = 1;
			CGameStateRun::hidden_code[current_stage][1][6] = 1;
			CGameStateRun::hidden_code[current_stage][2][4] = 1;
			CGameStateRun::hidden_code[current_stage][2][5] = 1;
			CGameStateRun::hidden_code[current_stage][2][6] = 1;
			CGameStateRun::hidden_code[current_stage][3][4] = 1;
			CGameStateRun::hidden_code[current_stage][3][6] = 1;
		}
		if (current_stage == 46 && x == 0 && y == 5) {
			CGameStateRun::hidden_code[current_stage][3][5] = 1;
			dialogMenu.setNPCTemporaryDialog({ "�u�������D�N�b�𪺳̩���", "�����ƴN�ӱϥL�a�I" }, enterStatus, dialogMenuing, NPC(PRINCESS, "���D"));
		}
	}
	bool Event::triggerSpecialNPCDialog(int current_stage, int x, int y, NPC npc, Character character, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 28 && x == 1 && y == 5 && character.getItemCount(HOE) > 0) {
			dialogMenu.setNPCTemporaryDialog({ "�x�I�o���N�O�ڪ��S�Y�ܡI", "���§A���ڧ�^�ӡI", "���ڴ��A���}", "�q���a�U���q�D�a�I" }, enterStatus, dialogMenuing, NPC(THIEF, "�s��"));
			CGameStateRun::hidden_code[current_stage][1][5] = 1;
			CGameStateRun::stage_entity[26][1][2] = THIEF;
			CGameStateRun::stage_entity[26][1][1] = DOWN_STAIR;
			CGameStateRun::stage_material[26][1][1] = ROAD;
			return true;
		}
		return false;
	}
}