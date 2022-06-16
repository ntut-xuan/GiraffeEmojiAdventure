#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <iostream>
#include <ddraw.h>
#include <time.h>
#include <string>
#include <fstream>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "MainFrm.h"
#include <thread>
#include <set>

#define element_width 77

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

	const int CGameStateRun::start_x = 469;
	const int CGameStateRun::start_y = 77;

	void ChangeFontLog(CDC* pDC, CFont* &fp, int size, string fontName, int weight = 500) {
		LOGFONT lf;
		CFont f;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = size;
		lf.lfWeight = weight;
		strcpy(lf.lfFaceName, fontName.c_str());
		f.CreateFontIndirect(&lf);
		fp = pDC->SelectObject(&f);
	}

	bool eventEntity(int entity_code) {
		bool flag = false;
		flag |= entity_code == DOOR || entity_code == SILVER_DOOR || entity_code == GOLD_DOOR;
		flag |= entity_code == KEY || entity_code == SILVER_KEY || entity_code == GOLD_KEY;
		return flag;
	}

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0, "Hi!");	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		logo.LoadBitmap(IDB_BACKGROUND);

		giraffe_photo.LoadBitmap("./RES/layout.bmp");
		giraffe_photo.SetTopLeft(0, 0);

		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{

	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);
		else if (nChar == KEY_ESC)
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{

	}

	void CGameStateInit::OnShow() {

		giraffe_photo.ShowBitmap();

	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	vector<vector<vector<int>>> CGameStateRun::stage_entity = vector<vector<vector<int>>>(50, vector<vector<int>>(11, vector<int>(11)));
	vector<vector<vector<int>>> CGameStateRun::stage_material = vector<vector<vector<int>>>(50, vector<vector<int>>(11, vector<int>(11)));
	vector<vector<vector<int>>> CGameStateRun::hidden_code = vector<vector<vector<int>>>(50, vector<vector<int>>(11, vector<int>(11)));
	Character CGameStateRun::character = Character();
	bool CGameStateRun::refresh_animation = false;
	char CGameStateRun::ending = '\0';
	int CGameStateRun::current_stage = 26;
	bool CGameStateRun::teleport_allow = false;

	CGameStateRun::CGameStateRun(CGame *g) : CGameState(g), STAGES(60)
	{

	}

	CGameStateRun::~CGameStateRun()
	{

	}

	void CGameStateRun::loadFixedEntityMap(int ID, vector<string> vec, int animation_time, bool animation_option) {
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fixed_entity_map[i][j][ID].LoadBitmapByString(vec);
				fixed_entity_map[i][j][ID].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
				if (animation_time) {
					fixed_entity_map[i][j][ID].SetAnimation(animation_time, animation_option);
				}
			}
		}
		//TRACE("%d, size=%d\n", ID, fixed_entity_map[0][0][ID].GetMovingBitmapFrame());
	}

	void CGameStateRun::loadFixedMaterialMap(int ID, vector<string> vec, int animation_time, bool animation_option) {
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fixed_material_map[i][j][ID].LoadBitmapByString(vec);
				fixed_material_map[i][j][ID].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
				if (animation_time) {
					fixed_material_map[i][j][ID].SetAnimation(animation_time, animation_option);
				}
			}
		}
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{

		ifstream in("Config/config.txt");
		ifstream monster_data_in("Config/monster_config.txt");

		int n;

		int count = 0;

		for (int r = 0; r < 5; r++) {
			in >> n;
			for (int i = 0; i < n; i++, count++) {
				string file_name;
				int ID;
				in >> file_name >> ID;
				if (r == 0) {
					int frame_count;
					in >> frame_count;
					normal_bitmap_map[ID] = file_name;
					vector<string> frame = { "RES/" + file_name + ".bmp" };
					for (int i = 0; i < frame_count - 1; i++) {
						frame.push_back("RES/" + file_name + to_string(2 + i) + ".bmp");
					}
					loadFixedEntityMap(ID, { frame }, 0, 0);
				}
				else if (r == 1) {
					functional_entity_bitmap_map[ID] = file_name;
					loadFixedEntityMap(ID, { "RES/" + file_name + ".bmp" }, 0, 0);
				}
				else if (r == 2) {
					string filename = "RES/" + file_name + ".bmp";
					string filename2 = "RES/" + file_name + "2.bmp";
					char* char_filename = (char*)filename.c_str();
					char* char_filename2 = (char*)filename2.c_str();
					loadFixedEntityMap(ID, { filename, filename2 }, 200, false);
					spyMenu.LoadMonsterBitmap(ID, { char_filename, char_filename2 });
					monster_map[ID].LoadBitmap({ char_filename, char_filename2 });
					monster_map[ID].SetAnimation(200, false);
					monster_map[ID].SetTopLeft(711 - 227, 417);
					monster_bitmap_map[ID] = file_name;
				}
				else if (r == 3) {
					string filename = "RES/" + file_name + ".bmp";
					string filename2 = "RES/" + file_name + "2.bmp";
					char* char_filename = (char*)filename.c_str();
					char* char_filename2 = (char*)filename2.c_str();
					loadFixedEntityMap(ID, { filename, filename2 }, 200, false);
					npc_map[ID].LoadBitmap({ char_filename, char_filename2 });
					npc_map[ID].SetAnimation(200, false);
					npc_map[ID].SetTopLeft(870 - 227, 161);
					npc_bitmap_map[ID] = file_name;
				}
				else if (r == 4) {
					block_bitmap_map[ID] = file_name;
					loadFixedEntityMap(ID, { "RES/" + file_name + ".bmp" }, 0, 0);
				}
				ShowInitProgress((int)(count * 0.5), "Loading Entity " + file_name);
			}
		}

		ShowInitProgress(70, "Loading Block Animation");

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fixed_animation_map[i][j][0].LoadBitmap({ "RES/blank_animation.bmp", "RES/lfa.bmp", "RES/lfa2.bmp", "RES/lfa3.bmp", "RES/lfa4.bmp", "RES/lfa5.bmp", "RES/blank_animation.bmp" }, RGB(255, 255, 255));
				fixed_animation_map[i][j][0].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
				fixed_animation_map[i][j][0].SetAnimation(0, true);
				fixed_animation_map[i][j][1].LoadBitmap({ "RES/blank_animation.bmp", "RES/lfai.bmp", "RES/lfai2.bmp", "RES/lfai3.bmp", "RES/lfai4.bmp", "RES/lfai5.bmp", "RES/blank_animation.bmp" }, RGB(255, 255, 255));
				fixed_animation_map[i][j][1].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
				fixed_animation_map[i][j][1].SetAnimation(0, true);
			}
		}

		ShowInitProgress(100, "Done!");

		map<string, int> special_event_number;
		special_event_number["RESPAWN_52"] = RESPAWN_52;
		special_event_number["RESPAWN_53"] = RESPAWN_53;
		special_event_number["ONE_SHOT_ATTACK"] = ONE_SHOT_ATTACK;
		special_event_number["IGNORE_DEFENCE"] = IGNORE_DEFENCE;
		special_event_number["CAUSE_WEEK"] = CAUSE_WEEK;
		special_event_number["POSION_ATTACK"] = POSION_ATTACK;
		special_event_number["HARD_HARD_HARD"] = HARD_HARD_HARD;
		special_event_number["MULTIPLE_ATTACK"] = MULTIPLE_ATTACK;
		special_event_number["DODGE_ATTACK"] = DODGE_ATTACK;

		monster_data_in >> n;
		for (int i = 0; i < n; i++) {
			string name;
			string special_event;
			int ID, health, attack, defence, speed, attack_count, exp, gold;
			monster_data_in >> ID >> name >> health >> attack >> defence >> speed >> attack_count >> exp >> gold >> special_event;
			Monster monster(ID, name, health, attack, defence, speed, attack_count, gold, exp, special_event_number[special_event]);
			monster_value[ID] = monster;
		}

		gameSystem.LoadBitmap({ "RES/menu.bmp" });

		loadFixedMaterialMap(WALL, { "RES/wall.bmp" }, 0, 0);
		loadFixedMaterialMap(ROAD, { "RES/road.bmp" }, 0, 0);
		loadFixedMaterialMap(LAVA, { "RES/lava.bmp", "RES/lava2.bmp" }, 200, false);
		loadFixedMaterialMap(WALL_SHINE, { "RES/wall_shine.bmp", "RES/wall_shine2.bmp" }, 200, false);
		loadFixedMaterialMap(WALL_SPECIAL, { "RES/wall_special.bmp" }, 200, false);
		loadFixedMaterialMap(WALL_B, { "RES/wall_b.bmp" }, 200, false);
		loadFixedMaterialMap(LAVA_ROAD, { "RES/lava_road.bmp", "RES/lava_road2.bmp" }, 200, false);

		/* Attack animation Load */

		enemyAttackAnimation[0].LoadBitmap({ "RES/attack_animation_1.bmp", "RES/attack_animation_2.bmp", "RES/attack_animation_3.bmp", "RES/empty_animation.bmp" }, RGB(0, 0, 0));
		enemyAttackAnimation[0].SetTopLeft(671 - 227, 377);
		enemyAttackAnimation[0].SetAnimation(100, true);

		characterAttackAnimation[0].LoadBitmap({ "RES/attack_animation_1.bmp", "RES/attack_animation_2.bmp", "RES/attack_animation_3.bmp", "RES/empty_animation.bmp" }, RGB(0, 0, 0));
		characterAttackAnimation[0].SetTopLeft(1427 - 227, 387);
		characterAttackAnimation[0].SetAnimation(100, true);

		/* Bitmap Menu Load */

		dialogMenu.LoadBitmap();
		dialogMenu.setNPCMap(npc_map);

		/* Character Attack Menu Load */

		characterAttackMenuBitMap.LoadBitmap({ "RES/character.bmp" }, RGB(0, 0, 0));
		characterAttackMenuBitMap.SetTopLeft(1467 - 227, 427);

		characterBitmap.LoadBitmap({ "RES/character.bmp", "RES/character2.bmp", "RES/character_21.bmp", "RES/character_22.bmp", "RES/character_31.bmp", "RES/character_32.bmp", "RES/character_41.bmp", "RES/character_42.bmp" }, RGB(0, 0, 0));
		characterBitmap.SetTopLeft(start_x + character.getY() * element_width, start_y + character.getX() * element_width);

		attackMenu.LoadBitmap({ "RES/attack_menu.bmp" }, RGB(0, 0, 0));
		attackMenu.SetTopLeft(656 - 227, 297);

		winningMenu.LoadBitmap({ "RES/WinPlatform.bmp" });
		winningMenu.SetTopLeft(656 - 227, 682);

		getDialogMenu.LoadBitmap({ "RES/getDialog.bmp" });
		getDialogMenu.SetTopLeft(656 - 227, 400);

		helpMenu.LoadBitmapA("RES/help.bmp");
		helpMenu.SetTopLeft(start_x, start_y);

		anyGateMenu.LoadBitmap({ "RES/anygate.bmp" });
		pogsMenu.LoadBitmap({ "RES/pogs.bmp" });

		/* Flying Menu Bitmap */

		flyingMenu.LoadBitmap();

		/* Spy Menu Bitmap */

		spyMenu.LoadBitmap();

		CAudio::Instance()->Load(AUDIO_BACKGROUND, "Audio/background.mp3");
		CAudio::Instance()->Load(AUDIO_BACKGROUND2, "Audio/background2.mp3");
		CAudio::Instance()->Load(AUDIO_WALK, "Audio/walk.mp3");
		CAudio::Instance()->Load(AUDIO_OPENDOOR, "Audio/opendoor.mp3");
		CAudio::Instance()->Load(AUDIO_ATTACK, "Audio/attack.mp3");
		CAudio::Instance()->Load(AUDIO_CATCH_ITEM, "Audio/catch_item.mp3");
		CAudio::Instance()->Load(AUDIO_WINNING_SOUND, "Audio/winning_sound.mp3");

	}

	void CGameStateRun::OnBeginState()
	{
		character = Character();

		current_stage = 26;
		temp_monster_x = NULL;
		temp_monster_y = NULL;
		dialogMenuing = false;
		inShopping = false;
		attackMenuing = false;
		dialogMenuing = false;
		flyingMenuing = false;
		spyMenuing = false;
		showAttackValue = true;
		enterStatus = false;
		turn = true;
		teleport_allow = false;
		temp_current_stage = 0;
		flyingMenu.setFlyingRange(FlyingMenu::TOP, 26);
		flyingMenu.setFlyingRange(FlyingMenu::DOWN, 26);
		character.setCharacterIndex(0);

		stage_entity = vector<vector<vector<int>>>(STAGES, vector<vector<int>>(11, vector<int>(11)));
		stage_material = vector<vector<vector<int>>>(STAGES, vector<vector<int>>(11, vector<int>(11)));
		hidden_code = vector<vector<vector<int>>>(STAGES, vector<vector<int>>(11, vector<int>(11)));
		refresh_animation = false;

		for (int i = 1; i <= 56; i++) {
			stage_entity[i] = stage.getStageEntity(i);
			stage_material[i] = stage.getStageMaterial(i);
		}

		vector<vector<int>> entity_code = stage_entity[current_stage];

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 1) {
					character.setXY(i, j);
				}
			}
		}

		flyingMenu.setFlyingSelect(current_stage);

		CAudio::Instance()->Play(AUDIO_BACKGROUND, true);

	}

	bool CGameStateRun::isDoor(int doorCode) {
		return doorCode == DOOR || doorCode == SILVER_DOOR || doorCode == GOLD_DOOR || doorCode == IRON_FENCE || doorCode == GEM_DOOR;
	}

	bool CGameStateRun::isKey(int keyCode) {
		return keyCode == KEY || keyCode == SILVER_KEY || keyCode == GOLD_KEY;
	}

	bool CGameStateRun::OpenDoor(int preCurrentStage, int x, int y, int doorCode) {

		vector<vector<int>> entity_ID = stage_entity[current_stage];

		bool doorSuccessOpen = true;

		if (doorCode == DOOR) {
			if (character.getKeyNumber() > 0) {
				doorSuccessOpen = true;
				character.changeKeyNumber(KEY, -1);
			}
		}
		if (doorCode == SILVER_DOOR) {
			if (character.getSilverKeyNumber() > 0) {
				doorSuccessOpen = true;
				character.changeKeyNumber(SILVER_KEY, -1);
			}
		}
		if (doorCode == GOLD_DOOR) {
			if (character.getGoldKeyNumber() > 0) {
				doorSuccessOpen = true;
				character.changeKeyNumber(GOLD_KEY, -1);
			}
		}
		if (doorCode == GEM_DOOR) {
			if (character.getItemCount(GEM_DIGGER) > 0) {
				doorSuccessOpen = true;
				character.setItemCount(GEM_DIGGER, character.getItemCount(GEM_DIGGER) - 1);
			}
		}
		if (doorCode == IRON_FENCE) {
			fixed_entity_map[x][y][doorCode].SetAnimation(1, true);
			fixed_entity_map[x][y][doorCode].ToggleAnimation(1);
			hidden_code[preCurrentStage][x][y] = 1;
			CAudio::Instance()->Play(AUDIO_OPENDOOR, false);
			return true;
		}

		if (doorSuccessOpen) {

			bool trigger = event.triggerTeleportIsExist(current_stage, x, y);

			if (!trigger) {
				fixed_entity_map[x][y][doorCode].SetAnimation(1, true);
				fixed_entity_map[x][y][doorCode].ToggleAnimation(1);
			}
			
			hidden_code[preCurrentStage][x][y] = 1;
			CAudio::Instance()->Play(AUDIO_OPENDOOR, false);

			return true;
		}
		return false;
	}

	bool CGameStateRun::GetKey(int x, int y, int keyCode) {
		if (keyCode == KEY) {
			hidden_code[current_stage][x][y] = 1;
			character.changeKeyNumber(KEY, 1);
			return true;
		}
		if (keyCode == SILVER_KEY) {
			hidden_code[current_stage][x][y] = 1;
			character.changeKeyNumber(SILVER_KEY, 1);
			return true;
		}
		if (keyCode == GOLD_KEY) {
			hidden_code[current_stage][x][y] = 1;
			character.changeKeyNumber(GOLD_KEY, 1);
			return true;
		}
		return false;
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		
		if (CGameStateRun::ending != '\0') {
			GotoGameState(GAME_STATE_OVER);
		}

		if (character.getHealth() <= 0) {
			if (character.getItemCount(CROSS) > 0) {
				character.setHealth(2000);
				character.setItemCount(CROSS, character.getItemCount(CROSS) - 1);
			}
			else {
				CGameStateOver::ending = 'B';
				GotoGameState(GAME_STATE_OVER);
			}
		}

		if (temp_current_stage != current_stage) {
			if (current_stage == 26) {
				CAudio::Instance()->Play(AUDIO_BACKGROUND, true);
				CAudio::Instance()->Stop(AUDIO_BACKGROUND2);
			}
			else if (temp_current_stage == 26 && current_stage != 26) {
				CAudio::Instance()->Play(AUDIO_BACKGROUND2, true);
				CAudio::Instance()->Stop(AUDIO_BACKGROUND);
			}
		}

		if (characterBitmap.GetSelectShowBitmap() / 2 != character.getCharacterIndex()) {
			characterBitmap.SelectShowBitmap(character.getCharacterIndex() * 2);
		}

		if (character.getHealth() == 0) {
			//GotoGameState(GAME_STATE_OVER);
		}

		floor_message = "主塔　";

		if (current_stage == 26) {
			floor_message += "入口";
		}
		else {
			floor_message += to_string(current_stage - 26) + "F";
		}

		characterBitmap.SetTopLeft(start_x + character.getY() * element_width, start_y + character.getX() * element_width);

		if (attackMenuing && (clock() - last_time) >= tempDelayCycle) {

			last_time = clock();

			if (character.getHealth() && monster.getHealth()) {

				int probabilityCauseDamage = rand() % 100;
				int probabilityCauseCriticalHit = rand() % 100;
				int probabilityCauseSkill = rand() % 100;

				int mo_health = monster.getHealth();
				int mo_ID = monster.getID();
				int mo_attack = monster.getAttack();
				int mo_defence = monster.getDefence();
				int mo_speed = monster.getSpeed();
				int mo_special = monster.getSpecialEvent();
				int mo_attack_count = monster.getAttackCount();

				int ch_health = character.getHealth();
				int ch_attack = character.getAttack();
				int ch_defence = mo_special == IGNORE_DEFENCE ? 0 : character.getDefence();
				int ch_speed = character.getSpeed();

				if (mo_special == ONE_SHOT_ATTACK) {
					if ((mo_ID == KNIGHT && probabilityCauseCriticalHit <= 30) || (mo_ID == IRON_KNIGHT_B && probabilityCauseCriticalHit <= 40) || (mo_ID == IRON_KNIGHT_A && probabilityCauseCriticalHit <= 50)) {
						mo_attack = ch_health + ch_defence + (rand() % 12345 + 12345) * (rand() % 12345 + 12345);
					}
				}

				if (turn) {
					attack_menu_animation_select = 1;
					int causeDamageValue;
					if (probabilityCauseDamage < mo_speed) {
						causeDamageValue = 0;
						causeDamageValueString = "Miss!";
					}
					else {
						causeDamageValue = max(1, ch_attack - mo_defence) * (probabilityCauseCriticalHit <= 2 ? 2 : 1);;
						causeDamageValueString = to_string(causeDamageValue);
					}
					monster.causeDamage(causeDamageValue);
					enemyAttackAnimation[0].ToggleAnimation(100);
					monsterAttackCount = mo_attack_count;
					tempDelayCycle = MENU_DELAY / (monster.getHealth() == 0 ? 1 : mo_attack_count);
					CAudio::Instance()->Play(AUDIO_ATTACK, false);
					turn = !turn;
				}
				else {
					attack_menu_animation_select = 0;
					int causeDamageValue;
					if (probabilityCauseDamage < ch_speed) {
						causeDamageValue = 0;
						causeDamageValueString = "Miss!";
					}
					else {
						causeDamageValue = max(0, mo_attack - ch_defence) * (probabilityCauseCriticalHit <= 2 ? 2 : 1);
						causeDamageValueString = to_string(causeDamageValue);
						if (probabilityCauseSkill <= 50) {
							if (monster.getSpecialEvent() == CAUSE_WEEK) {
								character.setStatus(CAUSE_WEEK);
							}
							if (monster.getSpecialEvent() == POSION_ATTACK) {
								character.setStatus(POSION_ATTACK);
							}
						}
					}
					character.causeDamage(causeDamageValue);
					characterAttackAnimation[0].ToggleAnimation(100);
					CAudio::Instance()->Play(AUDIO_ATTACK, false);
					if (--monsterAttackCount == 0) {
						turn = !turn;
						tempDelayCycle = MENU_DELAY;
						monsterAttackCount = mo_attack_count;
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
				CAudio::Instance()->Play(AUDIO_WINNING_SOUND, false);
				enterStatus = true;
				turn = false;
			}

		}

		gameSystem.setParameter(character, mouse_x, mouse_y, floor_message);
		anyGateMenu.setParameter(character);
		pogsMenu.setParameter(character);

	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_ENTER = 0x0D;
		const char KEY_D = 0x44;
		const char KEY_Q = 0x51;
		const char KEY_L = 0x4C;
		const char KEY_F = 0x46;
		const char KEY_R = 0x52;
		const char KEY_T = 0x54;
		const char KEY_W = 0x57;

		vector<vector<int>> material_code = stage_material[current_stage];
		vector<vector<int>> entity_code = stage_entity[current_stage];

		int x = character.getX();
		int y = character.getY();
		int faced_x = character.getFacedX();
		int faced_y = character.getFacedY();
		int preCurrentStage = current_stage;

		if (teleport_allow) {
			if (GetKeyState(VK_CONTROL) < 0) {
				if (nChar == KEY_UP) {
					current_stage += 1;
					return;
				}
				else if (nChar == KEY_DOWN) {
					current_stage -= 1;
					return;
				}
			}
		}

		if (nChar == KEY_Q) {
			if (!enterStatus && attackMenuing) {
				attackMenuing = false;
				showAttackValue = false;
				temp_monster_x = 0;
				temp_monster_y = 0;
				enterStatus = false;
				turn = true;
			}
			return;
		}

		if (nChar == KEY_R) {
			GotoGameState(GAME_STATE_INIT);
		}

		if (nChar == KEY_L && !attackMenuing && !dialogMenuing) {
			helpMenuing = !helpMenuing;
			return;
		}

		if (GetKeyState(VK_CONTROL) >= 0 && nChar == KEY_F && !(attackMenuing || dialogMenuing || helpMenuing || spyMenuing) && !(current_stage == 36 || current_stage > 45 || current_stage == 1) && character.getItemCount(FLY_ENABLE)) {
			flyingMenuing = !flyingMenuing;
			return;
		}

		if (nChar == KEY_D && !attackMenuing && !dialogMenuing && !helpMenuing && !flyingMenuing && character.getItemCount(HEART_MIRROR_ENABLE)) {
			spyMenuing = !spyMenuing;

			vector<vector<int>> entity_map = stage_entity[current_stage];
			map<int, bool> visited;
			vector<Monster> monster_vec;

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (monster_bitmap_map[entity_map[i][j]].length() == 0 || visited[entity_map[i][j]]) continue;
					visited[entity_map[i][j]] = true;
					monster_vec.push_back(monster_value[entity_map[i][j]]);
				}
			}

			spyMenu.setMonsterData({ monster_vec }, character);
			spyMenu.setPage(0);
			return;
		}

		if (nChar == KEY_T && !attackMenuing && !dialogMenuing && !helpMenuing && !flyingMenuing && !spyMenuing) {
			if (Event::triggerAnydoorRule(character)) {
				anyGateMenuing = !anyGateMenuing;
				return;
			}
		}

		if (nChar == KEY_W && !attackMenuing && !dialogMenuing && !helpMenuing && !flyingMenuing && !spyMenuing && !anyGateMenuing) {
			if (Event::triggerPOGSRule(character, monster_bitmap_map)) {
				pogsMenuing = !pogsMenuing;
				return;
			}
		}

		if (spyMenuing) {
			spyMenu.onKeyUp(nChar, nRepCnt, nFlags);
			return;
		}

		if (dialogMenuing || inShopping) {
			dialogMenu.onKeyUp(nChar, nRepCnt, nFlags, character, inShopping, enterStatus, dialogMenuing);
			return;
		}

		if (anyGateMenuing) {
			anyGateMenu.onKeyUp(nChar, nRepCnt, nFlags, character, enterStatus, anyGateMenuing);
			return;
		}

		if (pogsMenuing) {
			pogsMenu.onKeyUp(nChar, nRepCnt, nFlags, character, enterStatus, pogsMenuing, monster_bitmap_map);
			return;
		}

		if (enterStatus && nChar == KEY_ENTER) {
			if (attackMenuing) {
				event.triggerAfterAttack(current_stage, temp_monster_x, temp_monster_y, dialogMenu, enterStatus, dialogMenuing);
				hidden_code[current_stage][temp_monster_x][temp_monster_y] = 1;
				temp_monster_x = 0;
				temp_monster_y = 0;
				enterStatus = false;
				turn = true;
				enterStatus = false;
				attackMenuing = false;
			}
			if (getDialogMenuing) {
				getDialogMenuing = false;
				enterStatus = false;
			}
		}

		if (flyingMenuing) {
			int flying_select = flyingMenu.getFlyingSelect();
			int flying_top_range = flyingMenu.getFlyingRange(FlyingMenu::TOP);
			int flying_down_range = flyingMenu.getFlyingRange(FlyingMenu::DOWN);
			if (nChar == KEY_DOWN) {
				if (flying_select - 1 < flying_down_range) return;
				flyingMenu.setFlyingSelect(flying_select - 1);
			}
			else if (nChar == KEY_UP) {
				if (flying_select + 1 > flying_top_range) return;
				flyingMenu.setFlyingSelect(flying_select + 1);
			}
			if (nChar == KEY_ENTER) {
				vector<vector<int>> entity_stage_code = stage_entity[flying_select];
				pair<int, int> floor;
				if (current_stage < flying_select) {
					floor = flyingMenu.fetch_near_floor(flying_select, FlyingMenu::DOWN, entity_stage_code, { character.getX(), character.getY() });
				}
				else if (current_stage >= flying_select) {
					floor = flyingMenu.fetch_near_floor(flying_select, FlyingMenu::TOP, entity_stage_code, { character.getX(), character.getY() });
				}
				character.setXY(floor.first, floor.second);
				current_stage = flying_select;
				flyingMenuing = false;
				return;
			}
		}

		if (enterStatus || helpMenuing || flyingMenuing || spyMenuing) {
			return;
		}

		if (!attackMenuing) {
			if (nChar == KEY_UP) {
				x = max(0, character.getX() - 1);
				y = character.getY();
				character.setFacedDirection(0);
				CAudio::Instance()->Play(AUDIO_WALK, false);
			}
			else if (nChar == KEY_LEFT) {
				characterBitmap.SelectShowBitmap(character.getCharacterIndex() * 2);
				x = character.getX();
				y = max(0, character.getY() - 1);
				character.setFacedDirection(3);
				CAudio::Instance()->Play(AUDIO_WALK, false);
			}
			else if (nChar == KEY_RIGHT) {
				characterBitmap.SelectShowBitmap(character.getCharacterIndex() * 2 + 1);
				x = character.getX();
				y = min(10, character.getY() + 1);
				character.setFacedDirection(1);
				CAudio::Instance()->Play(AUDIO_WALK, false);
			}
			else if (nChar == KEY_DOWN) {
				x = min(10, character.getX() + 1);
				y = character.getY();
				character.setFacedDirection(2);
				CAudio::Instance()->Play(AUDIO_WALK, false);
			}
		}

		character.setFacedXY(faced_x, faced_y);

		if (character.getX() == x && character.getY() == y) {
			return;
		}

		if (material_code[x][y] == 0 || material_code[x][y] == WALL_SHINE || material_code[x][y] == WALL_SPECIAL || material_code[x][y] == LAVA || material_code[x][y] == WALL_B || block_bitmap_map[entity_code[x][y]].length() != 0) {
			return;
		}
		if (hidden_code[current_stage][x][y] == 0 && isDoor(entity_code[x][y]) && OpenDoor(preCurrentStage, x, y, entity_code[x][y]) == false) {
			return;
		}

		if (entity_code[x][y] == UP_STAIR) {
			current_stage++;
			flyingMenu.setFlyingRange(FlyingMenu::TOP, max(flyingMenu.getFlyingRange(FlyingMenu::TOP), current_stage));
			flyingMenu.setFlyingSelect(current_stage);
		}
		if (entity_code[x][y] == DOWN_STAIR) {
			current_stage--;
			flyingMenu.setFlyingRange(FlyingMenu::DOWN, min(flyingMenu.getFlyingRange(FlyingMenu::DOWN), current_stage));
			flyingMenu.setFlyingSelect(current_stage);
		}
		if (material_code[x][y] == LAVA_ROAD) {
			if (character.getItemCount(AMULET_OF_ICE) > 0) {
				fixed_animation_map[x][y][1].ToggleAnimation(50);
				CGameStateRun::stage_material[current_stage][x][y] = ROAD;
			}
			else {
				fixed_animation_map[x][y][0].ToggleAnimation(50);
				character.setHealth(character.getHealth() - 50);
			}
		}
		if (hidden_code[current_stage][x][y] == 0 && monster_bitmap_map[entity_code[x][y]].length() != 0) {

			attackMenuing = true;
			int ID = entity_code[x][y];

			monster = monster_value[ID];

			if (monster.getAttack() == -1) {
				monster.setAttack(character.getAttack());
			}
			if (monster.getDefence() == -1) {
				monster.setDefence(character.getDefence());
			}

			monsterAttackCount = monster.getAttackCount();
			tempDelayCycle = 200;
			last_time = clock();
			temp_monster_x = x;
			temp_monster_y = y;
			turn = 1;
			causeDamageValueString = "";
			return;
		}

		if (hidden_code[current_stage][x][y] == 0 && functional_entity_bitmap_map[entity_code[x][y]].length() != 0) {
			event.triggerGetItem(entity_code[x][y], character, temp_item_info);
			CAudio::Instance()->Play(AUDIO_CATCH_ITEM, false);
			enterStatus = true;
			getDialogMenuing = true;
			hidden_code[current_stage][x][y] = 1;
			return;
		}
		if (hidden_code[current_stage][x][y] == 0 && npc_bitmap_map[entity_code[x][y]].length() != 0) {
			if (entity_code[x][y] == SHOPKEEPER) {
				dialogMenuing = true;
				enterStatus = true;
				bool trigger = event.triggerSpecialNPCDialog(current_stage, x, y, NPC(SHOPKEEPER, "奸商"), character, dialogMenu, enterStatus, dialogMenuing);
				if (trigger == false) {
					dialogMenu.setNPC(NPC(SHOPKEEPER, "奸商"), current_stage, x, y, inShopping);
				}
			}
			else if (entity_code[x][y] == ELDER) {
				dialogMenuing = true;
				enterStatus = true;
				inShopping = false;
				bool trigger = event.triggerSpecialNPCDialog(current_stage, x, y, NPC(ELDER, "老人"), character, dialogMenu, enterStatus, dialogMenuing);
				if (trigger == false) {
					dialogMenu.setNPC(NPC(ELDER, "老人"), current_stage, x, y, inShopping);
				}
			}
			else if (entity_code[x][y] == SHOP_1_2) {
				dialogMenuing = true;
				enterStatus = true;
				dialogMenu.setNPC(NPC(SHOP_1_2, "貪婪之神"), current_stage, x, y, inShopping);
			}
			else if (entity_code[x][y] == SHOP_2_2) {
				dialogMenuing = true;
				enterStatus = true;
				dialogMenu.setNPC(NPC(SHOP_2_2, "戰鬥之神"), current_stage, x, y, inShopping);
			}
			else if (entity_code[x][y] == PRINCESS) {
				dialogMenuing = true;
				enterStatus = true; 
				inShopping = false;
				bool trigger = event.triggerSpecialNPCDialog(current_stage, x, y, NPC(PRINCESS, "公主"), character, dialogMenu, enterStatus, dialogMenuing);
				if (trigger == false) {
					dialogMenu.setNPC(NPC(PRINCESS, "公主"), current_stage, x, y, inShopping);
				}
			}
			else if (entity_code[x][y] == THIEF) {
				dialogMenuing = true;
				enterStatus = true;
				inShopping = false;
				bool trigger = event.triggerSpecialNPCDialog(current_stage, x, y, NPC(THIEF, "竊賊"), character, dialogMenu, enterStatus, dialogMenuing);
				if (trigger == false) {
					dialogMenu.setNPC(NPC(THIEF, "竊賊"), current_stage, x, y, inShopping);
				}
			}
			else if (entity_code[x][y] == FAIRY) {
				dialogMenuing = true;
				enterStatus = true;
				inShopping = false;
				bool trigger = event.triggerSpecialNPCDialog(current_stage, x, y, NPC(FAIRY, "妖精"), character, dialogMenu, enterStatus, dialogMenuing);
				if (trigger == false) {
					dialogMenu.setNPC(NPC(FAIRY, "妖精"), current_stage, x, y, inShopping);
				}
			}
			return;
		}

		if (eventEntity(entity_code[x][y])) {
			hidden_code[current_stage][x][y] = 1;
		}


		event.triggerDialogEvent(current_stage, x, y, dialogMenu, enterStatus, dialogMenuing);

		bool triggerTeleport = event.triggerTeleport(current_stage, x, y, character);
		bool triggerTeleportAfterShowBitmap = event.triggerTeleportAfterShowBitmap(current_stage, x, y, character);

		TRACE("%d %d\n", triggerTeleport, triggerTeleportAfterShowBitmap);

		if (triggerTeleport || triggerTeleportAfterShowBitmap) {
			return;
		}

		if ((character.getStatus() & POSION_ATTACK) > 0) {
			character.setHealth(character.getHealth() - 10);
		}

		character.setXY(x, y);
		character.setCurrentStage(current_stage);
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void CGameStateRun::OnShow() {

		gameSystem.onShow();

		vector<vector<int>> material_code = stage_material[current_stage];
		vector<vector<int>> entity_code = stage_entity[current_stage];

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fixed_material_map[i][j][material_code[i][j]].ShowBitmap();
			}
		}

		bool reset_frame = ((temp_current_stage != current_stage) || refresh_animation);

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 0) continue;
				if (entity_code[i][j] == 1) continue;
				if (fixed_entity_map[i][j][entity_code[i][j]].IsAnimationDone()) {
					if (hidden_code[current_stage][i][j] == 1) {
						fixed_entity_map[i][j][entity_code[i][j]].UnshowBitmap();
					}
					else {
						fixed_entity_map[i][j][entity_code[i][j]].ShowBitmap();
						if (reset_frame && !monster_bitmap_map[entity_code[i][j]].length()) {
							fixed_entity_map[i][j][entity_code[i][j]].SelectShowBitmap(0);
						}
					}
				}
				else {
					fixed_entity_map[i][j][entity_code[i][j]].ShowBitmap();
					if (reset_frame && !monster_bitmap_map[entity_code[i][j]].length()) {
						fixed_entity_map[i][j][entity_code[i][j]].SelectShowBitmap(0);
					}
				}
			}
		}

		if (reset_frame) {
			temp_current_stage = current_stage;
			refresh_animation = false;
			reset_frame = false; 
		}

		characterBitmap.ShowBitmap();

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				for (int k = 0; k < 2; k++) {
					fixed_animation_map[i][j][k].ShowBitmap();
				}
			}
		}

		if (attackMenuing) {
			attackMenu.ShowBitmap();
			characterAttackMenuBitMap.ShowBitmap();
			monster_map[monster.getID()].ShowBitmap();
			if (attack_menu_animation_select == 1) {
				enemyAttackAnimation[0].ShowBitmap();
			}
			else if (attack_menu_animation_select == 0) {
				characterAttackAnimation[0].ShowBitmap();
			}
		}

		if (attackMenuing && enterStatus) {
			winningMenu.ShowBitmap();
		}
		else {
			winningMenu.UnshowBitmap();
		}

		if (dialogMenuing) {
			dialogMenu.onShow();
		}

		if (getDialogMenuing) {
			getDialogMenu.ShowBitmap();
		}

		if (helpMenuing) {
			helpMenu.ShowBitmap();
		}

		if (flyingMenuing) {
			flyingMenu.onShow();
		}

		if (spyMenuing) {
			spyMenu.onShow();
		}

		if (anyGateMenuing) {
			anyGateMenu.onShow();
		}

		if (pogsMenuing) {
			pogsMenu.onShow();
		}

		ShowText();

	}

	vector<int> getShineVector() {
		vector<int> vec;
		for (int i = 80; i <= 250; i += 10) {
			vec.push_back(i);
		}
		for (int i = 250; i >= 80; i -= 10) {
			vec.push_back(i);
		}
		return vec;
	}

	void CGameStateRun::ShowText() {

		CDC *pDC = CDDraw::GetBackCDC();
		CFont *fp;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));

		/* Attack Menu Text Setup */

		if (attackMenuing) {

			ChangeFontLog(pDC, fp, 40, "Noto Sans TC");

			CTextDraw::print(pDC, 1167 - 227, 405, to_string(character.getHealth()).c_str());
			CTextDraw::print(pDC, 1167 - 227, 465, to_string(character.getAttack()).c_str());
			CTextDraw::print(pDC, 1167 - 227, 528, to_string(character.getDefence()).c_str());
			CTextDraw::print(pDC, 1167 - 227, 586, to_string(character.getSpeed()).c_str());

			ChangeFontLog(pDC, fp, 60, "Noto Sans TC");

			CTextDraw::print(pDC, 701 - 227, 330, monster.getName().c_str());

			ChangeFontLog(pDC, fp, 40, "Noto Sans TC");

			CTextDraw::print(pDC, 977 - 227, 405, to_string(monster.getHealth()).c_str());
			CTextDraw::print(pDC, 977 - 227, 465, to_string(monster.getAttack()).c_str());
			CTextDraw::print(pDC, 977 - 227, 528, to_string(monster.getDefence()).c_str());
			CTextDraw::print(pDC, 977 - 227, 586, to_string(monster.getSpeed()).c_str());

			if (showAttackValue == true) {

				ChangeFontLog(pDC, fp, 80, "Noto Sans TC");

				if (attack_menu_animation_select == 1) {
					pDC->SetTextColor(RGB(255, 255 - ((clock() - last_time) % (MENU_DELAY + 100)) * (255.0 / MENU_DELAY), 255 - ((clock() - last_time) % (MENU_DELAY + 100)) * (255.0 / MENU_DELAY)));
					CTextDraw::print(pDC, (int)(504 + (clock() - last_time) * (50.0 / MENU_DELAY)), (int)(437 + (clock() - last_time) * (50.0 / MENU_DELAY)), causeDamageValueString.c_str());
				}
				else if (attack_menu_animation_select == 0) {
					pDC->SetTextColor(RGB(255, 255 - ((clock() - last_time) % (MENU_DELAY + 100)) * (255.0 / MENU_DELAY), 255 - ((clock() - last_time) % (MENU_DELAY + 100)) * (255.0 / MENU_DELAY)));
					CTextDraw::print(pDC, (int)(1230 + (clock() - last_time) * (50.0 / MENU_DELAY)) - 20 * causeDamageValueString.length(), (int)(437 + (clock() - last_time) * (50.0 / MENU_DELAY)), causeDamageValueString.c_str());
				}

				if (!enterStatus) {
					pDC->SetTextColor(RGB(255, 255, 0));
					ChangeFontLog(pDC, fp, 50, "Noto Sans TC");
					CTextDraw::print(pDC, 1430 - 227, 630, string("撤退(Q)").c_str());
				}
			}

		}

		/* Get Dialog Menu Setup */
		if (getDialogMenuing) {
			pDC->SetTextColor(RGB(255, 255, 255));
			ChangeFontLog(pDC, fp, 50, "Noto Sans TC");
			CTextDraw::print(pDC, 786 - 227, 415, temp_item_info.c_str());
		}

		if (attackMenuing && enterStatus || getDialogMenuing || flyingMenuing ) {
			/* Make Shine Enter */
			vector<int> vec = getShineVector();
			int count = vec.size();
			pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));
			ChangeFontLog(pDC, fp, 50, "Noto Sans TC");
			if (attackMenuing) {
				CTextDraw::print(pDC, 1430 - 227, 630, string("- Enter -").c_str());
			}else if (getDialogMenuing) {
				CTextDraw::print(pDC, 1430 - 227, 415, string("- Enter -").c_str());
			}
			/* Place gold and exp */
			if (attackMenuing) {
				pDC->SetTextColor(RGB(255, 255, 255));
				ChangeFontLog(pDC, fp, 80, "Noto Sans TC", 800);
				CTextDraw::print(pDC, 1156 - 227, 688, to_string(monster.getExp()).c_str());
				CTextDraw::print(pDC, 1470 - 227, 688, to_string(monster.getGold()).c_str());
			}
		}

		

		/* Dialog Menu Text Setup */

		if (dialogMenuing) {
			dialogMenu.onShowText(pDC, fp, inShopping);
		}

		if (flyingMenuing) {
			flyingMenu.onShowText(pDC, fp);
		}

		if (spyMenuing) {
			spyMenu.onShowText(pDC, fp);
		}

		if (anyGateMenuing) {
			anyGateMenu.onShowText(pDC, fp);
		}

		if (pogsMenuing) {
			pogsMenu.onShowText(pDC, fp);
		}

		/* Show Dashboard */

		gameSystem.onShowText(pDC, fp);
		

		ChangeFontLog(pDC, fp, 60, "Noto Sans TC");
		pDC->SetTextColor(RGB(0, 0, 0));
		CTextDraw::print(pDC, 0, 930, (to_string(current_stage) + " " + to_string(character.getX()) + " " + to_string(character.getY())).c_str());

		CDDraw::ReleaseBackCDC();
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
		mouse_x = point.x;
		mouse_y = point.y;
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {  // 處理滑鼠的動作

	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{

	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{

	}

	char CGameStateOver::ending = '\0';

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{

	}

	void CGameStateOver::OnBeginState()
	{
		if (CGameStateOver::ending == 'B') {
			dialog = {
				{"你在與怪物的戰鬥中被擊敗了", "怪物踐踏著你的屍體", "國家陷入毀滅" },
				{"Bad End..."}
			};
		}

		if (CGameStateOver::ending == 'N') {
			dialog = {
				{ "這位勇士終於成功將公主救了出來", "魔塔也被毀滅了", "剩下的只是一堆石頭而已" },
				{"不過，卻比他預期中，", "花多了兩天才完成任務，照當初的約定", "他沒有領取任何獎金就離開這個國家了"},
				{"不久，這個國家傳出滅亡的消息", "兇手不知是誰，奇怪的是，", "找遍了整個國家都找不到公主的屍體"},
				{"究竟是逃走了", "還是被毀屍了？", "沒有人知道"},
				{"唯一知道的是，", "在魔塔遺址又有怪物出沒", "似乎是有漏網之魚"},
				{"無論如何，", "任務已完成，", "其他事都不用去想了"},
				{"Normal End..."}
			};
		}

		if (CGameStateOver::ending == 'T') {
			dialog = {
				{"這位勇士終於成功將公主救了出來", "魔塔也被毀滅了", "剩下的只是一堆石頭而已" },
				{"雖然，卻比他預期中，", "花多了兩天才完成任務"},
				{"不過國王在公主的說服下，", "獎金仍然是給了他"},
				{"接過獎金，", "這位勇士就離開這個國家了"},
				{"自此之後", "再沒有人見過他的蹤影"},
				{"究竟他是誰？", "究竟為何而來？", "至今依然是個謎"},
				{"總之，任務完成了，", "這個國家也回復一片和平的景象"},
				{"True End..."}
			};
		}
		current_dialog = 0;
	}

	void CGameStateOver::OnInit()
	{

		dialogMenu_B.LoadBitmap("RES/DialogMenu.bmp", RGB(0, 0, 0));
		dialogMenu_B.SetTopLeft(588 - 150, 507 - 250);

		dialogMenuCharacter_B.LoadBitmap("RES/character_bg.bmp", RGB(255, 255, 255));
		dialogMenuCharacter_B.SetTopLeft(643 - 150, 564 - 250);

		
	}

	void CGameStateOver::OnShow(){

		dialogMenu_B.ShowBitmap();
		dialogMenuCharacter_B.ShowBitmap();
		ShowText();

	}

	void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
		const char KEY_ENTER = 0x0D;

		if (nChar == KEY_ENTER) {
			current_dialog += 1;
			if (current_dialog == dialog.size()) {
				GotoGameState(GAME_STATE_INIT);
				current_dialog -= 1;
				CGameStateRun::ending = '\0';
				CGameStateOver::ending = '\0';
			}
		}
	}

	void CGameStateOver::ShowText() {

		CDC *pDC = CDDraw::GetBackCDC();
		CFont *fp;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));

		ChangeFontLog(pDC, fp, 60, "STZhongsong");

		CTextDraw::print(pDC, 1100 - 227 - 150, 540 - 250, "長頸鹿");

		ChangeFontLog(pDC, fp, 24, "STZhongsong");

		for (int i = 0; i < (int)dialog[current_dialog].size(); i++) {
			CTextDraw::print(pDC, 1010 - 227 - 150, 630 + i * 40 - 250, dialog[current_dialog][i].c_str());
		}

		vector<int> vec = getShineVector();
		int count = vec.size();
		pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));

		CTextDraw::print(pDC, 1293 - 227 - 150, 760 - 250, string("- Enter -").c_str());

		CDDraw::ReleaseBackCDC();
	}

}