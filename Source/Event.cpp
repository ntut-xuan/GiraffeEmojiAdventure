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

	vector<vector<vector<int>>> Event::visited = vector<vector<vector<int>>>(60, vector<vector<int>>(11, vector<int>(11)));

	Event::Event() {
		visited = vector<vector<vector<int>>>(60, vector<vector<int>>(11, vector<int>(11)));
	}

	void Event::triggerDialogEvent(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 26 && x == 8 && y == 5 && !visited[current_stage][x][y]) {
			dialogMenu.setTemporaryDialog(DialogMenu::DOWN, { "�N�O�o�̤F", "��i�h..." }, enterStatus, dialogMenuing);
			visited[current_stage][x][y] = true;
		}
		if (current_stage == 51 && x == 8 && y == 5 && !visited[current_stage][x][y] && CGameStateRun::stage_entity[current_stage][9][5] == SELF_MONSTER) {
			dialogMenu.setNPCTemporaryDialog({
				{"�A�O...�O��...�H"},
				{"�n�ϥγ�...�����n���j�j���믫�O...", "�ҥH�A�n�����ѧA�ۤv������믫�O�I", "�ӧa�I�I"} },
				{ 'B', 'T' }, enterStatus, dialogMenuing, NPC(SELF_NPC, "�ۤv"));
			visited[current_stage][x][y] = true;
		}
		if (current_stage == 1 && x == 5 && y == 5 && !visited[current_stage][x][y]) {
			dialogMenu.setNPCTemporaryDialog({
				{"�ݨӧA���B�𮼤�����", "���M���F�o��", "���L�o�̬O�A�����I�F"},
				{"�o�̷�M�O���I�I", "�]���ڷ|���ѧA�ñϨ����D�I�I"},
				{"�A���G�٥�����..."},
				{{"�ڰ��F���ѧA�~"}, {"���٭n���դ���H"}},
				{"��F", "�@�M�۶��a"} },
				{ 'T', 'B', 'T', 'B', 'T' }, enterStatus, dialogMenuing, NPC(VAMPIRE_NPC, "�]���@��ǲ�"));
			visited[current_stage][x][y] = true;
		}
		if (current_stage == 1 && x == 9 && y == 5 && !visited[current_stage][x][y]) {
			dialogMenu.setNPCTemporaryDialog({
				{"�S�O�����I", "�i�c�I�I"},
				{"...�I�I", "�ڳ��M�|�鵹�A�̳o�ǤH���I�I"},
				{"�u�ǧA�t�ޤӮt�F�I", "�ֻ��a�I", "�u�������D�b���̡I"},
				{"�ڤ��|���I�����Ʀۤv�h��I", "���L�A�Y�ϧA���F", "�]�@�w�Ϥ���L���I���������I"}},
				{ 'B', 'T', 'B', 'T'}, enterStatus, dialogMenuing, NPC(MONSTER, "�]������"));
			visited[current_stage][x][y] = true;
			CGameStateRun::stage_entity[1][0][5] = UP_STAIR;
			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (i == 0 && j == 5) {
						CGameStateRun::stage_entity[2][i][j] = DOWN_STAIR;
						CGameStateRun::stage_material[2][i][j] = ROAD;
						continue;
					}
					if (i == 0 && j == 6) {
						CGameStateRun::stage_entity[2][i][j] = 0;
						CGameStateRun::stage_material[2][i][j] = ROAD;
						continue;
					}
					CGameStateRun::stage_entity[2][i][j] = 0;
					CGameStateRun::stage_material[2][i][j] = LAVA;
				}
			}
		}
		if (current_stage == 2 && x == 0 && y == 6 && !visited[current_stage][x][y]) {
			dialogMenu.setNPCTemporaryDialog({
				{"�V�|�I�I", "��B���O�����I�I", "�ݨӪ����٦����D�٥��o�{"} },
				{ 'B' }, enterStatus, dialogMenuing, NPC(SELF_NPC, "���V��"));
			visited[current_stage][x][y] = true;
		}
	}
	void Event::triggerAfterDialog(int current_stage, int x, int y, Character &character) {
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
		if (current_stage == 19 && x == 7 && y == 3) {
			CGameStateRun::hidden_code[current_stage][x][y] = 1;
			character.setItemCount(ANY_GATE, 10);
		}
		if (current_stage == 1 && x == 9 && y == 5 && CGameStateRun::stage_entity[1][0][5] != UP_STAIR) {
			CGameStateRun::ending = 'N';
			CGameStateOver::ending = 'N';
		}
		if (current_stage == 54 && x == 0 && y == 5) {
			CGameStateRun::ending = 'T';
			CGameStateOver::ending = 'T';
		}
		if (current_stage == 9 && x == 10 && y == 0) {
			character.setItemCount(SPECIAL_KEY_B18F, 1);
		}
		if (current_stage == 31 && x == 10 && y == 5) {
			character.setItemCount(HEART_MIRROR_ENABLE, 1);
		}
		if (current_stage == 34 && x == 7 && y == 9) {
			character.setItemCount(FLY_ENABLE, 1);
		}
	}
	void Event::triggerGetItem(int ID, Character &character, string &temp_item_info) {
		if (ID == RUBY) {
			character.setAttack(character.getRawAttack() + 2);
			temp_item_info = "���_�ۡA�����O + 2";
		}
		if (ID == SAPPHIRE) {
			character.setDefence(character.getRawDefence() + 2);
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
			if ((character.getStatus() & CAUSE_WEEK) > 0) {
				character.setStatus(-CAUSE_WEEK);
			}
		}
		if (ID == ANTI_POTION) {
			temp_item_info = "�ܬr���A�^�_�r���A";
			if ((character.getStatus() & POSION_ATTACK) > 0) {
				character.setStatus(-POSION_ATTACK);
			}
		}
		if (ID == JUMP_WING) {
			character.levelup();
			temp_item_info = "�D�i���l�A���Ŵ��ɤ@";
		}
		if (ID == SHIELD_D) {
			character.setDefence(character.getRawDefence() + 7);
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
		if (ID == SWORD_D) {
			character.setAttack(character.getRawAttack() + 16);
			temp_item_info = "���C�A�����O + 16";
		}
		if (ID == SHIELD_C) {
			character.setAttack(character.getRawDefence() + 22);
			temp_item_info = "�����ޡA���m�O + 22";
		}
		if (ID == MAGIC_KEY) {
			character.changeKeyNumber(KEY, 1);
			character.changeKeyNumber(SILVER_KEY, 1);
			character.changeKeyNumber(GOLD_KEY, 1);
			temp_item_info = "�]�k���͡A�Ҧ���ͼ� + 1";
		}
		if (ID == SWORD_E) {
			character.setAttack(character.getRawAttack() + 8);
			temp_item_info = "�K�C�A�����O + 8";
		}
		if (ID == HOE) {
			temp_item_info = "�S�Y";
			character.setItemCount(HOE, character.getItemCount(HOE) + 1);
		}
		if (ID == SWORD_C) {
			temp_item_info = "�_�ۼC�A�����O + 30";
			character.setAttack(character.getRawAttack() + 30);
		}
		if (ID == AMULET_OF_ICE) {
			temp_item_info = "�B���@��";
			character.setItemCount(AMULET_OF_ICE, character.getItemCount(AMULET_OF_ICE) + 1);
		}
		if (ID == CROSS) {
			temp_item_info = "�Q�r�[�A����_����O 2000";
			character.setItemCount(CROSS, character.getItemCount(CROSS) + 1);
		}
		if (ID == HOLY_CROSS_SHIELD) {
			temp_item_info = "�t���Q�r�ޡA���m�O + 35";
			character.setDefence(character.getRawDefence() + 35);
		}
		if (ID == YONG_VERI) {
			temp_item_info = "�^���¬ï]����";
			character.setItemCount(YONG_VERI, character.getItemCount(YONG_VERI) + 1);
			CGameStateRun::stage_material[51][1][0] = WALL_SPECIAL;
			CGameStateRun::stage_material[51][1][10] = WALL_SPECIAL;
			CGameStateRun::stage_entity[51][9][5] = SELF_MONSTER;
		}
		if (ID == BAR_VERI) {
			temp_item_info = "�^���p�۪���";
			character.setItemCount(BAR_VERI, character.getItemCount(BAR_VERI) + 1);
			CGameStateRun::stage_material[51][1][10] = WALL_SPECIAL;
			CGameStateRun::stage_material[51][1][5] = WALL_SPECIAL;
			CGameStateRun::stage_entity[51][9][5] = SELF_MONSTER;
		}
		if (ID == XIAN_VERI) {
			temp_item_info = "�^�����_�۪���";
			character.setItemCount(XIAN_VERI, character.getItemCount(XIAN_VERI) + 1);
			CGameStateRun::stage_material[51][1][0] = WALL_SPECIAL;
			CGameStateRun::stage_material[51][1][5] = WALL_SPECIAL;
			CGameStateRun::stage_entity[51][9][5] = SELF_MONSTER;
		}
		if (ID == SHIELD_A) {
			temp_item_info = "�i�̤��ޡA���m�O + 50";
			character.setDefence(character.getRawDefence() + 50);
		}
		if (ID == HOLY_WATER) {
			temp_item_info = "�t���A��O�⭿";
			character.setHealth(character.getHealth() * 2);
		}
		if (ID == PROVE_OF_GOD_SWORD) {
			temp_item_info = "���C����";
			character.setItemCount(POGS, 10);
		}
	}
	void Event::triggerAfterAttack(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 16 && x == 8 && y == 5) {
			CGameStateRun::hidden_code[current_stage][8][4] = 1;
			CGameStateRun::hidden_code[current_stage][8][5] = 1;
			CGameStateRun::hidden_code[current_stage][8][6] = 1;
			CGameStateRun::hidden_code[current_stage][7][4] = 1;
			CGameStateRun::hidden_code[current_stage][7][5] = 1;
			CGameStateRun::hidden_code[current_stage][7][6] = 1;
			CGameStateRun::hidden_code[current_stage][6][4] = 1;
			CGameStateRun::hidden_code[current_stage][6][5] = 1;
			CGameStateRun::hidden_code[current_stage][6][6] = 1;
		}
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
		if (current_stage == 55 && x == 5 && y == 5) {
			CGameStateRun::hidden_code[current_stage][3][4] = 1;
			CGameStateRun::hidden_code[current_stage][3][5] = 1;
			CGameStateRun::hidden_code[current_stage][3][6] = 1;
			CGameStateRun::hidden_code[current_stage][4][4] = 1;
			CGameStateRun::hidden_code[current_stage][4][5] = 1;
			CGameStateRun::hidden_code[current_stage][4][6] = 1;
			CGameStateRun::hidden_code[current_stage][5][4] = 1;
			CGameStateRun::hidden_code[current_stage][5][6] = 1;
		}
		if (current_stage == 46 && x == 0 && y == 5) {
			CGameStateRun::hidden_code[current_stage][3][5] = 1;
			dialogMenu.setNPCTemporaryDialog({ "�u�������D�N�b�𪺳̩���", "�����ƴN�ӱϥL�a�I" }, enterStatus, dialogMenuing, NPC(PRINCESS, "���D"));
		}
	}
	bool Event::triggerSpecialNPCDialog(int current_stage, int x, int y, NPC npc, Character &character, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 28 && x == 1 && y == 5 && character.getItemCount(HOE) > 0) {
			dialogMenu.setNPCTemporaryDialog({ "�x�I�o���N�O�ڪ��S�Y�ܡI", "���§A���ڧ�^�ӡI", "���ڴ��A���}", "�q���a�U���q�D�a�I" }, enterStatus, dialogMenuing, NPC(THIEF, "�s��"));
			CGameStateRun::hidden_code[current_stage][1][5] = 1;
			CGameStateRun::stage_entity[26][1][2] = THIEF;
			CGameStateRun::stage_entity[26][1][1] = DOWN_STAIR;
			CGameStateRun::stage_material[26][1][1] = ROAD;
			return true;
		}
		if (current_stage == 40 && x == 0 && y == 1 && (character.getItemCount(XIAN_VERI) > 0 || character.getItemCount(BAR_VERI) > 0 || character.getItemCount(YONG_VERI) > 0)) {
			string message1 = "";
			string message2 = "";
			vector<string> message3 = { "" };
			vector<string> message4 = { "" };
			string message5 = "";
			if (character.getItemCount(BAR_VERI) > 0) {
				message1 = "��...�o���_�ۡA�o�Ϊ�...";
				message2 = "�����F�F�I�O�i�̤��ҡI";
				message3 = { "�i...�i�̤��ҡI�H" };
				message4 = { "�S���I���F�o�ӦA�t�X�W�T��", "�A���������ƴN�|�W��", "���԰��Q�����Q���ڡI" };
				message5 = "�i�̤���";
				character.setCharacterIndex(2);
			}
			if (character.getItemCount(YONG_VERI) > 0) {
				message1 = "���A�O�¬ï]�I";
				message2 = "���άݤF�I�O�Q�̤��ҡI";
				message3 = { "�Q�̤��ҡH", "���O����H" };
				message4 = { "�A���D���i�u�ֹ�~�u�n�֦��L", "�A�N�i�H�ϼu��⪺����", "�ϥL�]�P�ɨ���", "�X�{���v�ᰪ�A�n�n�n���ΩO�I" };
				message5 = "�Q�̤���";
				character.setCharacterIndex(1);
			}
			if (character.getItemCount(XIAN_VERI) > 0) {
				message1 = "��...�O�@���öQ���p��...";
				message2 = "�٦�...�I�I�����F�F�I�O��̤��ҡI";
				message3 = { "��̤��ҡI�H" };
				message4 = { "��I���F���A�N�i�H�N���", "�������Ʀ��A����O", "���M�X�{���v�����A�����٦����[��O", "���@�w���|�O�A�b�԰���^�_�ֶq��O" };
				message5 = "��̤���";
				character.setCharacterIndex(3);
			}
			dialogMenu.setNPCTemporaryDialog({
				{"�ѧB�A�ЧA�ݬݳo��"},
				{message1, message2},
				message3,
				message4,
				{ message5 } },
				{ 'B', 'T', 'B', 'T', 'C' }, enterStatus, dialogMenuing, NPC(ELDER, "�ѤH")
				);
			return true;
		}
		if (current_stage == 24 && x == 5 && y == 5) {
			if (character.getItemCount(SPECIAL_KEY_17F) == 0) {
				dialogMenu.setNPCTemporaryDialog({
					{"�}�ȩO��O�ӱϤ��D���a�H", "���o�h���᪺�Ǫ��|�ܱo��j", "�H�A�{�ɪ���O", "�O����Ĺ���F��"},
					{"�O�ܡH", "������H"},
					{"������٬O��ĳ�A�� 17F ���_��", "�o�ӧA���h�a", "�����_����A���O�q�N�|�j�W", "��ɦA�ӧ�ڡ�"},
					{"�������͡A�i���} 17F �����K�q�D�H" } },
					{ 'T', 'B', 'T', 'C' }, enterStatus, dialogMenuing, NPC(FAIRY, "����")
					);
				character.setItemCount(SPECIAL_KEY_17F, 1);
				return true;
			}
			if (character.getCharacterIndex() > 0 && !visited[current_stage][x][y]) {
				dialogMenu.setNPCTemporaryDialog({
					{ "�ڮ���F�I", "�O�q�u���W�[�F���֡I", "���§A�I" },
					{ "�����¡�", "��A�Ϧ^�F���D�~�¡�", "����A�[�o�a�I" } },
					{ 'B', 'T' }, enterStatus, dialogMenuing, NPC(FAIRY, "����"));
				visited[current_stage][x][y] = true;
				CGameStateRun::stage_entity[current_stage][0][0] = DOWN_STAIR;
				return true;
			}
		}
		if (current_stage == 8 && x == 7 && y == 3 && character.getItemCount(SPECIAL_KEY_B18F) && !visited[current_stage][x][y]) {
			dialogMenu.setNPCTemporaryDialog({
					{"�ڡA���쯵�K�q�D", "�ڰO�o 10F �O����", "���L�n�����������N���I"} },
				{ 'T', }, enterStatus, dialogMenuing, NPC(SHOPKEEPER, "�l��"));
			character.setItemCount(SPECIAL_KEY_10F, 1);
			visited[current_stage][x][y] = true;
			return true;
		}
		if (current_stage == 12 && x == 10 && y == 2 && character.getItemCount(POGS)) {
			dialogMenu.setNPCTemporaryDialog({
					{"�ڰڡA�O���C�����I", "�u�n�A�b�Ǫ��e�ϥΡA", "���򥦴N�|�ܦ��̮z�����v�ܩi"},
					{"�����Ʀ���", "�Ҽ{�M���~�ϥΰڡI"} },
				{ 'T', 'T' }, enterStatus, dialogMenuing, NPC(SHOPKEEPER, "�l��"));
			return true;
		}
		return false;	
	}

	bool Event::triggerTeleport(int &current_stage, int x, int y, Character &character) {
		if (current_stage == 55 && x == 10 && y == 5) {
			current_stage = 4;
			character.setXY(0, 10);
			return true;
		}
		if (current_stage == 47 && x == 10 && y == 5) {
			current_stage = 43;
			character.setXY(0, 2);
			return true;
		}
		if (current_stage == 52 && x == 10 && y == 5) {
			current_stage = 17;
			character.setXY(0, 4);
			return true;
		}
		if (current_stage == 56 && x == 10 && y == 5) {
			current_stage = 36;
			character.setXY(1, 0);
			return true;
		}
		if (current_stage == 53 && x == 10 && y == 5) {
			current_stage = 9;
			character.setXY(10, 10);
			return true;
		}
		return false;
	}

	bool Event::triggerTeleportAfterShowBitmap(int &current_stage, int x, int y, Character &character) {
		if (current_stage == 43 && x == 0 && y == 2 && character.getItemCount(SPECIAL_KEY_17F) > 0) {
			current_stage = 47;
			character.setXY(10, 5);
			return true;
		}
		if (current_stage == 4 && x == 0 && y == 10) {
			current_stage = 55;
			character.setXY(10, 5);
			return true;
		}
		if (current_stage == 17 && x == 0 && y == 4) {
			current_stage = 52;
			character.setXY(10, 5);
			return true;
		}
		if (current_stage == 36 && x == 1 && y == 0 && character.getItemCount(SPECIAL_KEY_10F)) {
			current_stage = 56;
			character.setXY(10, 5);
			return true;
		}
		if (current_stage == 9 && x == 10 && y == 10) {
			current_stage = 53;
			character.setXY(10, 5);
			return true;
		}
		if (current_stage == 1 && x == 10 && y == 5) {
			current_stage = 54;
			character.setXY(10, 5);
			return true;
		}
		return false;
	}

	bool Event::triggerTeleportIsExist(int current_stage, int x, int y) {
		if (current_stage == 43 && x == 0 && y == 2) {
			return true;
		}
		if (current_stage == 4 && x == 0 && y == 10) {
			return true;
		}
		if (current_stage == 55 && x == 10 && y == 5) {
			return true;
		}
		if (current_stage == 17 && x == 0 && y == 4) {
			return true;
		}
		if (current_stage == 36 && x == 1 && y == 0) {
			return true;
		}
		if (current_stage == 9 && x == 10 && y == 10) {
			return true;
		}
		if (current_stage == 1 && x == 10 && y == 5) {
			return true;
		}
		return false;
	}
	
	bool Event::triggerAnydoorRule(Character character) {

		int current_stage = character.getCurrentStage();
		int x = character.getX();
		int y = character.getY();
		int faced_direction = character.getFacedDirection();

		if (faced_direction == 0) {
			x -= 1;
		}
		if (faced_direction == 1) {
			y += 1;
		}
		if (faced_direction == 2) {
			x += 1;
		}
		if (faced_direction == 3) {
			y -= 1;
		}

		if (x < 0 || x > 10 || y < 0 || y > 10) {
			return false;
		}

		if (character.getItemCount(ANY_GATE) == 0) {
			return false;
		}

		if ((CGameStateRun::stage_material[current_stage][x][y] == ROAD || CGameStateRun::stage_material[current_stage][x][y] == LAVA_ROAD) && !CGameStateRun::stage_entity[current_stage][x][y]){
			return true;
		}

		vector<vector<int>> specificBlock = { {19, 7, 1}, {17, 0, 2}, {17, 0, 4}, {36, 5, 1}, {36, 4, 1}, {36, 3, 1}, {36, 2, 1}, {36, 1, 1}, {36, 1, 0}, {9, 8, 10}, {9, 9, 10}, {1, 10, 5} };

		for (vector<int> p : specificBlock) {
			if (p[0] == current_stage && p[1] == x && p[2] == y) {
				return true;
			}
		}

		return false;
	}

	bool Event::triggerPOGSRule(Character character, map<int, string> monster_map) {

		int current_stage = character.getCurrentStage();
		int x = character.getX();
		int y = character.getY();
		int faced_direction = character.getFacedDirection();

		if (faced_direction == 0) {
			x -= 1;
		}
		if (faced_direction == 1) {
			y += 1;
		}
		if (faced_direction == 2) {
			x += 1;
		}
		if (faced_direction == 3) {
			y -= 1;
		}

		if (x < 0 || x > 10 || y < 0 || y > 10) {
			return false;
		}

		TRACE("%d\n", character.getItemCount(POGS));

		if (character.getItemCount(POGS) == 0) {
			return false;
		}

		if (CGameStateRun::hidden_code[current_stage][x][y] == 1) {
			return false;
		}

		if (CGameStateRun::stage_entity[current_stage][x][y] == OCTOPUS_8) {
			return false;
		}

		if (CGameStateRun::stage_entity[current_stage][x][y] == DRAGON_8) {
			return false;
		}

		if (monster_map[CGameStateRun::stage_entity[current_stage][x][y]].length() != 0 || CGameStateRun::stage_entity[current_stage][x][y] == PRINCESS) {
			return true;
		}

		return true;
	}

	bool Event::triggerShopping(int current_stage, int x, int y, Character &character, int tempSelect, bool &inShopping, bool &enterStatus, bool &dialogMenuing) {
		if (current_stage == 29 && x == 7 && y == 3) {
			if (character.getCoin() >= NPC::increaseCost1) {
				if (tempSelect == 0) {
					character.setHealth(character.getHealth() + 500);
					character.setCoin(character.getCoin() - NPC::increaseCost1);
					NPC::increaseCost1 += 1;
				}
				else if (tempSelect == 1) {
					character.setAttack(character.getRawAttack() + 3);
					character.setCoin(character.getCoin() - NPC::increaseCost1);
					NPC::increaseCost1 += 1;
				}
				else if (tempSelect == 2) {
					character.setDefence(character.getRawDefence() + 3);
					character.setCoin(character.getCoin() - NPC::increaseCost1);
					NPC::increaseCost1 += 1;
				}
			}
			return true;
		}
		if (current_stage == 30 && x == 3 && y == 6) {
			if (tempSelect == 0) {
				character.setHealth(character.getHealth() + 200);
				character.setCoin(character.getCoin() - 15);
				CGameStateRun::stage_entity[30][3][6] = 0;
				inShopping = false;
				enterStatus = false;
				dialogMenuing = false;
			}
		}
		if (current_stage == 31 && x == 0 && y == 7) {
			if (tempSelect == 0) {
				character.setAttack(character.getRawAttack() + 4);
				character.setCoin(character.getCoin() - 40);
				CGameStateRun::stage_entity[31][0][7] = 0;
				inShopping = false;
				enterStatus = false;
				dialogMenuing = false;
			}
		}
		if (current_stage == 33 && x == 9 && y == 2) {
			
			if (tempSelect == 0) {
				if (character.getExp() >= 70) {
					character.levelup();
					character.setExp(character.getExp() - 70);
				}
			}
			else if (tempSelect == 1) {
				if (character.getExp() >= 20) {
					character.setAttack(character.getRawAttack() + 1);
					character.setExp(character.getExp() - 20);
				}
			}
			else if (tempSelect == 2) {
				if (character.getExp() >= 20) {
					character.setDefence(character.getRawDefence() + 2);
					character.setExp(character.getExp() - 20);
				}
			}
		}
		if (current_stage == 21 && x == 7 && y == 9) {
			if (character.getCoin() >= NPC::increaseCost2) {
				if (tempSelect == 0) {
					character.setHealth(character.getHealth() + 800);
					character.setCoin(character.getCoin() - NPC::increaseCost2);
					NPC::increaseCost2 += 1;
				}
				else if (tempSelect == 1) {
					character.setAttack(character.getRawAttack() + 6);
					character.setCoin(character.getCoin() - NPC::increaseCost2);
					NPC::increaseCost2 += 1;
				}
				else if (tempSelect == 2) {
					character.setDefence(character.getRawDefence() + 6);
					character.setCoin(character.getCoin() - NPC::increaseCost2);
					NPC::increaseCost2 += 1;
				}
			}
		}
		if (current_stage == 19 && x == 3 && y == 2) {
			if (tempSelect == 0) {
				if (character.getExp() >= 190) {
					character.levelup();
					character.levelup();
					character.levelup();
					character.setExp(character.getExp() - 70);
				}
			}
			else if (tempSelect == 1) {
				if (character.getExp() >= 50) {
					character.setDefence(character.getRawAttack() + 3);
					character.setExp(character.getExp() - 20);
				}
			}
			else if (tempSelect == 2) {
				if (character.getExp() >= 50) {
					character.setDefence(character.getRawDefence() + 5);
					character.setExp(character.getExp() - 20);
				}
			}
		}
		if (current_stage == 12 && x == 10 && y == 6) {
			if (character.getCoin() >= 150) {
				if (tempSelect == 0) {
					character.setItemCount(ANY_GATE, character.getItemCount(ANY_GATE) + 10);
					character.setCoin(character.getCoin() - 150);
				}
			}
		}
		return false;
	}
}