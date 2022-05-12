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
			dialogMenu.setTemporaryDialog(DialogMenu::DOWN, { "就是這裡了", "攻進去..." }, enterStatus, dialogMenuing);
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
			temp_item_info = "紅寶石，攻擊力 + 2";
		}
		if (ID == SAPPHIRE) {
			character.setDefence(character.getDefence() + 2);
			temp_item_info = "藍寶石，防禦力 + 2";
		}
		if (ID == EMERALD) {
			character.setSpeed(character.getSpeed() + 2);
			temp_item_info = "綠寶石，敏捷 + 2";
		}
		if (ID == RED_POTION) {
			character.setHealth(character.getHealth() + 150);
			temp_item_info = "紅藥水，血量 + 150";
		}
		if (ID == BLUE_POTION) {
			character.setHealth(character.getHealth() + 400);
			temp_item_info = "藍藥水，血量 + 400";
		}
		if (ID == KEY) {
			character.changeKeyNumber(KEY, 1);
			temp_item_info = "黃鑰匙";
		}
		if (ID == SILVER_KEY) {
			character.changeKeyNumber(SILVER_KEY, 1);
			temp_item_info = "藍鑰匙";
		}
		if (ID == GOLD_KEY) {
			character.changeKeyNumber(GOLD_KEY, 1);
			temp_item_info = "紅鑰匙";
		}
		if (ID == FIRE_DRINK) {
			temp_item_info = "火酒，回復衰弱狀態";
		}
		if (ID == ANTI_POTION) {
			temp_item_info = "抗毒劑，回復毒狀態";
		}
		if (ID == JUMP_WING) {
			character.setLevel(character.getLevel() + 1);
			temp_item_info = "躍進之翼，等級提升一";
		}
		if (ID == SHIELD_D) {
			character.setDefence(character.getDefence() + 7);
			temp_item_info = "皮革之盾，防禦力 + 7";
		}
		if (ID == GEM_DIGGER) {
			temp_item_info = "寶石鋤頭";
			character.setItemCount(GEM_DIGGER, character.getItemCount(GEM_DIGGER) + 1);
		}
		if (ID == COIN_200) {
			character.setCoin(character.getCoin() + 200);
			temp_item_info = "金幣 200 塊";
		}
		if (ID == SWORD_C) {
			character.setAttack(character.getAttack() + 16);
			temp_item_info = "長劍，攻擊力 + 16";
		}
		if (ID == SHIELD_C) {
			character.setAttack(character.getDefence() + 22);
			temp_item_info = "光之盾，防禦力 + 22";
		}
		if (ID == MAGIC_KEY) {
			character.changeKeyNumber(KEY, 1);
			character.changeKeyNumber(SILVER_KEY, 1);
			character.changeKeyNumber(GOLD_KEY, 1);
			temp_item_info = "魔法之匙，所有鎖匙數 + 1";
		}
		if (ID == SWORD_D) {
			character.setAttack(character.getAttack() + 8);
			temp_item_info = "鐵劍，攻擊力 + 8";
		}
		if (ID == HOE) {
			temp_item_info = "鋤頭";
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
			dialogMenu.setNPCTemporaryDialog({ "真正的公主就在塔的最底部", "有本事就來救他吧！" }, enterStatus, dialogMenuing, NPC(PRINCESS, "公主"));
		}
	}
	bool Event::triggerSpecialNPCDialog(int current_stage, int x, int y, NPC npc, Character character, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 28 && x == 1 && y == 5 && character.getItemCount(HOE) > 0) {
			dialogMenu.setNPCTemporaryDialog({ "咦！這不就是我的鋤頭嗎！", "謝謝你替我找回來！", "讓我替你打開", "通往地下的通道吧！" }, enterStatus, dialogMenuing, NPC(THIEF, "盜賊"));
			CGameStateRun::hidden_code[current_stage][1][5] = 1;
			CGameStateRun::stage_entity[26][1][2] = THIEF;
			CGameStateRun::stage_entity[26][1][1] = DOWN_STAIR;
			CGameStateRun::stage_material[26][1][1] = ROAD;
			return true;
		}
		return false;
	}
}