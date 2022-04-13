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

#define element_width 77
#define UU_DEBUG 0

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	/////////////////////////////////////////////////////////////////////////////

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
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		//ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
		//
		// �}�l���J���
		//
		logo.LoadBitmap(IDB_BACKGROUND);

		giraffe_photo.LoadBitmap("./RES/giraffe.bmp", RGB(255, 255, 255));
		giraffe_photo.SetTopLeft(600, 300);

		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
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
			GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo �����C������k
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// �����C��
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
	}

	void CGameStateInit::OnShow(){

		giraffe_photo.ShowBitmap();

		CDC *pDC = CDDraw::GetBackCDC();
		CFont f,*fp;

		time_t rawtime;
		time(&rawtime);

		f.CreatePointFont(560, "Noto Sans TC");	
		fp=pDC->SelectObject(&f);
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetTextColor(RGB(0,0,0));
		pDC->TextOut(400, 50, "�]��]���ʭ��٨S���n.jpg�^");

		f.Detach();
		f.CreatePointFont(160, "Noto Sans TC");
		fp = pDC->SelectObject(&f);

		pDC->TextOut(650, 740, " - �� Space �}�l�C�� - ");
		pDC->TextOut(20, 850, "�� Ctrl + Q �Ȱ��C��");
		pDC->TextOut(20, 850, "�� Ctrl + Q �Ȱ��C��");
		pDC->TextOut(1000, 850, ctime(&rawtime));

		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}								

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g)
	: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f,*fp;
		f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
		fp=pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(255,255,0));
		char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240,210,str);
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g) : CGameState(g), STAGES(50)
	{

	}

	CGameStateRun::~CGameStateRun()
	{
		
	}

	void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
	{

		menuBitmap.LoadBitmapA("RES/menu.bmp", RGB(0, 0, 0));
		menuBitmap.SetTopLeft(237, 0);

		int start_x = 700;
		int start_y = 77;

		ifstream in("Config/config.txt");
		ifstream monster_data_in("Config/monster_config.txt");

		int n;

		for (int r = 0; r < 5; r++) {
			in >> n;
			for (int i = 0; i < n; i++) {
				string file_name;
				int ID;
				in >> file_name >> ID;
				if (r == 0) {
					int frame_count;
					in >> frame_count;
					normal_bitmap_map[ID] = file_name;
					bitmap_frame[ID] = frame_count;
				}
				else if (r == 1) {
					functional_entity_bitmap_map[ID] = file_name;
				}
				else if (r == 2) {
					string filename = "RES/" + file_name + ".bmp";
					string filename2 = "RES/" + file_name + "2.bmp";
					char* char_filename = (char*)filename.c_str();
					char* char_filename2 = (char*)filename2.c_str();
					monster_map[ID].LoadBitmap({ char_filename, char_filename2 });
					monster_map[ID].SetAnimation(5, false);
					monster_map[ID].SetTopLeft(711, 417);
					monster_bitmap_map[ID] = file_name;
				}
				else if (r == 3) {
					string filename = "RES/" + file_name + ".bmp";
					string filename2 = "RES/" + file_name + "2.bmp";
					char* char_filename = (char*)filename.c_str();
					char* char_filename2 = (char*)filename2.c_str();
					npc_map[ID].LoadBitmap({ char_filename, char_filename2 });
					npc_map[ID].SetAnimation(5, false);
					npc_map[ID].SetTopLeft(870, 161);
					npc_bitmap_map[ID] = file_name;
				}
				else if (r == 4) {
					block_bitmap_map[ID] = file_name;
				}
			}
		}

		monster_data_in >> n;
		for (int i = 0; i < n; i++) {
			string name;
			int ID, health, attack, defence, speed, attack_count, exp, gold;
			monster_data_in >> ID >> name >> health >> attack >> defence >> speed >> attack_count >> exp >> gold;
			Monster monster(ID, name, health, attack, defence, speed, attack_count, gold, exp);
			monster_value[ID] = monster;
		}

		for (int s = 26; s <= 46; s++) {
			vector<vector<int>> material_code = stage.getStageMaterial(s);
			vector<vector<int>> entity_code = stage.getStageEntity(s);

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (material_code[i][j] == WALL) {
						material_map[s][i][j].LoadBitmap("RES/wall.bmp");
						material_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else if (material_code[i][j] == ROAD) {
						material_map[s][i][j].LoadBitmap("RES/road.bmp");
						material_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else if (material_code[i][j] == LAVA) {
						material_map[s][i][j].LoadBitmap({ "RES/lava.bmp", "RES/lava2.bmp" });
						material_map[s][i][j].SetAnimation(5, false);
						material_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else if (material_code[i][j] == WALL_SHINE) {
						material_map[s][i][j].LoadBitmap({ "RES/wall_shine.bmp", "RES/wall_shine2.bmp" });
						material_map[s][i][j].SetAnimation(5, false);
						material_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else if (material_code[i][j] == WALL_SPECIAL) {
						material_map[s][i][j].LoadBitmap({ "RES/wall_special.bmp" });
						material_map[s][i][j].SetAnimation(5, false);
						material_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
				}
			}

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					int ID = entity_code[i][j];
					if (ID == 0 || ID == 1) {
						continue;
					}
					if (normal_bitmap_map[ID].length() != 0) {
						vector<string> frames = {};
						for (int i = 0; i < bitmap_frame[ID]; i++) {
							string filename = "RES/" + normal_bitmap_map[ID] + (i == 0 ? "" : to_string(i + 1)) + ".bmp";
							frames.push_back(filename);
						}
						entity_map[s][i][j].LoadBitmapByString(frames, RGB(0, 0, 0));
						entity_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else if (functional_entity_bitmap_map[ID].length() != 0) {
						string filename = "RES/" + functional_entity_bitmap_map[ID] + ".bmp";
						char* char_filename = (char*)filename.c_str();
						entity_map[s][i][j].LoadBitmap(char_filename);
						entity_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else if (block_bitmap_map[ID].length() != 0) {
						string filename = "RES/" + block_bitmap_map[ID] + ".bmp";
						char* char_filename = (char*)filename.c_str();
						entity_map[s][i][j].LoadBitmap(char_filename);
						entity_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
					}
					else {
						if (monster_bitmap_map[ID].length() != 0) {
							string filename = "RES/" + monster_bitmap_map[ID] + ".bmp";
							string filename2 = "RES/" + monster_bitmap_map[ID] + "2.bmp";
							char* char_filename = (char*)filename.c_str();
							char* char_filename2 = (char*)filename2.c_str();
							entity_map[s][i][j].LoadBitmap({ char_filename, char_filename2 });
							entity_map[s][i][j].SetAnimation(5, false);
							entity_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
						}
						else if (npc_bitmap_map[ID].length() != 0) {
							string filename = "RES/" + npc_bitmap_map[ID] + ".bmp";
							string filename2 = "RES/" + npc_bitmap_map[ID] + "2.bmp";
							char* char_filename = (char*)filename.c_str();
							char* char_filename2 = (char*)filename2.c_str();
							entity_map[s][i][j].LoadBitmap({ char_filename, char_filename2 });
							entity_map[s][i][j].SetAnimation(5, false);
							entity_map[s][i][j].SetTopLeft(start_x + element_width * j, start_y + element_width * i);
						}
						else {
							GAME_ASSERT(false, "ID NOT EXIST!");
						}
					}
				}
			}
		}

		/* Attack animation Load */

		enemyAttackAnimation[0].LoadBitmap({ "RES/attack_animation_1.bmp", "RES/attack_animation_2.bmp", "RES/attack_animation_3.bmp", "RES/empty_animation.bmp" }, RGB(0, 0, 0));
		enemyAttackAnimation[0].SetTopLeft(671, 377);
		enemyAttackAnimation[0].SetAnimation(2, true);

		characterAttackAnimation[0].LoadBitmap({ "RES/attack_animation_1.bmp", "RES/attack_animation_2.bmp", "RES/attack_animation_3.bmp", "RES/empty_animation.bmp" }, RGB(0, 0, 0));
		characterAttackAnimation[0].SetTopLeft(1427, 387);
		characterAttackAnimation[0].SetAnimation(2, true);

		/* Bitmap Menu Load */

		dialogMenu.LoadBitmap("RES/DialogMenu.bmp", RGB(0, 0, 0));
		dialogMenu.SetTopLeft(815, 107);

		optionMenu.LoadBitmap("RES/OptionMenu1.bmp", RGB(0, 0, 0));
		optionMenu.SetTopLeft(815, 414);

		optionArrow.LoadBitmap("RES/optionArrow.bmp", RGB(0, 0, 0));
		optionArrow.SetTopLeft(875, 464);

		/* Character Attack Menu Load */

		characterAttackMenuBitMap.LoadBitmap({ "RES/character.bmp" }, RGB(255, 255, 255));
		characterAttackMenuBitMap.SetTopLeft(1467, 427);

		characterBitmap.LoadBitmap({ "RES/character.bmp", "RES/character2.bmp" }, RGB(255, 255, 255));
		characterBitmap.SetTopLeft(start_x + character.getY() * element_width, start_y + character.getX() * element_width);

		attackMenu.LoadBitmap({ "RES/attack_menu.bmp" }, RGB(0, 0, 0));
		attackMenu.SetTopLeft(656, 297);

		winningMenu.LoadBitmap({ "RES/WinPlatform.bmp" });
		winningMenu.SetTopLeft(656, 682);

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
		showAttackValue = true;
		enterStatus = false;
		turn = true;
		teleport_allow = false;
		
#ifdef UU_DEBUG 
		character.setAttack(35);
		character.setDefence(0);
		character.setHealth(1);
		character.setSpeed(0);
		character.setCoin(998244353);
		character.changeKeyNumber(KEY, 23766);
		character.changeKeyNumber(SILVER_KEY, 23766);
		character.changeKeyNumber(GOLD_KEY, 23766);
		teleport_allow = true;
#endif 

		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 1) {
					character.setXY(i, j);
				}
			}
		}

	}

	bool CGameStateRun::isDoor(int doorCode) {
		return doorCode == DOOR || doorCode == SILVER_DOOR || doorCode == GOLD_DOOR || doorCode == IRON_FENCE;
	}

	bool CGameStateRun::isKey(int keyCode) {
		return keyCode == KEY || keyCode == SILVER_KEY || keyCode == GOLD_KEY;
	}

	bool CGameStateRun::OpenDoor(int x, int y, int doorCode) {
		if (doorCode == DOOR) {
			if (character.getKeyNumber() > 0) {
				entity_map[current_stage][x][y].ToggleAnimation(1);
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(KEY, -1);
				return true;
			}
		}
		if (doorCode == SILVER_DOOR) {
			if (character.getSilverKeyNumber() > 0) {
				entity_map[current_stage][x][y].ToggleAnimation(1);
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(SILVER_KEY, -1);
				return true;
			}
		}
		if (doorCode == GOLD_DOOR) {
			if (character.getGoldKeyNumber() > 0) {
				entity_map[current_stage][x][y].ToggleAnimation(1);
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(GOLD_KEY, -1);
				return true;
			}
		}
		if (doorCode == IRON_FENCE) {
			entity_map[current_stage][x][y].ToggleAnimation(1);
			hidden_code[current_stage][x][y] = 1;
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

	void CGameStateRun::OnMove()							// ���ʹC������
	{	

		if (character.getHealth() == 0) {
			GotoGameState(GAME_STATE_OVER);
		}

		floor_message = "�D��@";

		if (current_stage == 26) {
			floor_message += "�J�f";
		}
		else {
			floor_message += to_string(current_stage-26) + "F";
		}

		int start_x = 700;
		int start_y = 77;

		characterBitmap.SetTopLeft(start_x + character.getY() * element_width, start_y + character.getX() * element_width);

		if (attackMenuing && --tempDelayCycle <= 0) {
			if (character.getHealth() && monster.getHealth()) {
				if (turn) {
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
					enemyAttackAnimation[0].SetAnimation(1, true);
				}
				else {
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
					characterAttackAnimation[0].SetAnimation(1, true);
				}
				showAttackValue = true;
				turn = !turn;
			}

			if (enterStatus == true) {
				showAttackValue = false;
			}

			if (showAttackValue && monster.getHealth() == 0) {
				character.setExp(character.getExp() + monster.getExp());
				character.setCoin(character.getCoin() + monster.getGold());
				enterStatus = true;
				turn = false;
			}

			tempDelayCycle = MENU_DELAY_CYCLE;

		}

	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
		const char KEY_ENTER = 0x0D;
		const char KEY_Q = 0x51;

		vector<vector<int>> material_code = stage.getStageMaterial(current_stage);
		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		int x = character.getX();
		int y = character.getY();

		if (teleport_allow) {
			if (GetKeyState(VK_CONTROL) < 0) {
				if (nChar == KEY_UP) {
					current_stage += 1;
					return;
				}
				else {
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
				return;
			}
		}

		if (inShopping) {
			if (nChar == KEY_DOWN) {
				tempSelect += 1;
				tempSelect %= (int)npc.getOption().size();
			}
			else if (nChar == KEY_UP) {
				tempSelect -= 1;
				tempSelect += (int)npc.getOption().size();
				tempSelect %= (int)npc.getOption().size();
			}
			else if (nChar == KEY_ENTER) {
				TRACE("%d\n", tempSelect);
				if (character.getCoin() >= NPC::increaseCost1) {
					if (tempSelect == 0) {
						character.setHealth(character.getHealth() + 500);
						character.setCoin(character.getCoin() - current_shop_price);
					}
					else if (tempSelect == 1) {
						character.setAttack(character.getAttack() + 3);
						character.setCoin(character.getCoin() - current_shop_price);
					}
					else if (tempSelect == 2) {
						character.setDefence(character.getDefence() + 3);
						character.setCoin(character.getCoin() - current_shop_price);
					}
					NPC::increaseCost1 += 1;
				}
				if (tempSelect == 3) {
					inShopping = false;
					enterStatus = false;
					dialogMenuing = false;
				}
				return;
			}
			optionArrow.SetTopLeft(875  , 464 + menuOptionGap * tempSelect);
			return;
		}
		if (enterStatus && nChar == KEY_ENTER) {
			if (attackMenuing) {
				hidden_code[current_stage][temp_monster_x][temp_monster_y] = 1;
				temp_monster_x = 0;
				temp_monster_y = 0;
				enterStatus = false;
				turn = true;
			}
			enterStatus = false;
			attackMenuing = false;
			dialogMenuing = false;
			return;
		}
		if (!attackMenuing) {
			if (nChar == KEY_UP) {
				x = max(0, character.getX() - 1);
				y = character.getY();
			}
			else if (nChar == KEY_LEFT) {
				characterBitmap.SelectShowBitmap(0);
				x = character.getX();
				y = max(0, character.getY() - 1);
			}
			else if (nChar == KEY_RIGHT) {
				characterBitmap.SelectShowBitmap(1);
				x = character.getX();
				y = min(10, character.getY() + 1);
			}
			else if (nChar == KEY_DOWN) {
				x = min(10, character.getX() + 1);
				y = character.getY();
			}
		}
		if (material_code[x][y] == 0 || material_code[x][y] == WALL_SHINE || material_code[x][y] == WALL_SPECIAL) {
			return;
		}
		if (hidden_code[current_stage][x][y] == 0 && isDoor(entity_code[x][y]) && OpenDoor(x, y, entity_code[x][y]) == false) {
			return;
		}
		if (entity_code[x][y] == UP_STAIR) {
			current_stage++;
		}
		if (entity_code[x][y] == DOWN_STAIR) {
			current_stage--;
		}
		if (hidden_code[current_stage][x][y] == 0 && isKey(entity_code[x][y])) {
			GetKey(x, y, entity_code[x][y]);
		}
		if (hidden_code[current_stage][x][y] == 0 && monster_bitmap_map[entity_code[x][y]].length() != 0) {
			attackMenuing = true;
			int ID = entity_code[x][y];
			monster = monster_value[ID];
			temp_monster_x = x;
			temp_monster_y = y;
			turn = 1;
			return;
		}
		if (hidden_code[current_stage][x][y] == 0 && functional_entity_bitmap_map[entity_code[x][y]].length() != 0) {
			if (entity_code[x][y] == RUBY) {
				character.setAttack(character.getAttack() + 2);
			}
			if (entity_code[x][y] == SAPPHIRE) {
				character.setDefence(character.getDefence() + 2);
			}
			if (entity_code[x][y] == EMERALD) {
				character.setSpeed(character.getSpeed() + 2);
			}
			if (entity_code[x][y] == RED_POTION) {
				character.setHealth(character.getHealth() + 150);
			}
			if (entity_code[x][y] == BLUE_POTION) {
				character.setHealth(character.getHealth() + 400);
			}
			hidden_code[current_stage][x][y] = 1;
		}
		if (hidden_code[current_stage][x][y] == 0 && npc_bitmap_map[entity_code[x][y]].length() != 0) {
			if (entity_code[x][y] == SHOPKEEPER) {
				dialogMenuing = true;
				enterStatus = true;
				npc = NPC(SHOPKEEPER, "�ӤH");
				npc.loadData(current_stage, x, y);
			}
			else if (entity_code[x][y] == SHOP2) {
				dialogMenuing = true;
				enterStatus = true;
				inShopping = true;
				npc = NPC(SHOP2, "�g������");
				npc.loadData(current_stage, x, y);
				menuOptionGap = OPTION_GAP - 10 * npc.getOption().size();
			}
			return;
		}
		if (eventEntity(entity_code[x][y])) {
			hidden_code[current_stage][x][y] = 1;
		}
		character.setXY(x, y);
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void CGameStateRun::OnShow() {

		vector<vector<int>> material_code = stage.getStageMaterial(current_stage);
		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				material_map[current_stage][i][j].ShowBitmap();
			}
		}
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 0) continue;
				if (entity_code[i][j] == 1) continue;
				if (entity_map[current_stage][i][j].IsAnimationDone()) {
					if (hidden_code[current_stage][i][j] == 1) {
						entity_map[current_stage][i][j].UnshowBitmap();
					} else {
						entity_map[current_stage][i][j].ShowBitmap();
					}
				}
				else {
					entity_map[current_stage][i][j].ShowBitmap();
				}
			}
		}
		
		characterBitmap.ShowBitmap();
		menuBitmap.ShowBitmap();

		if (attackMenuing) {
			attackMenu.ShowBitmap();
			characterAttackMenuBitMap.ShowBitmap();
			monster_map[monster.getID()].ShowBitmap();
			if (!turn) {
				enemyAttackAnimation[0].ShowBitmap();
			}
			else {
				characterAttackAnimation[0].ShowBitmap();
			}
		}
		else {
			menuBitmap.UnshowBitmap();
		}

		if (attackMenuing && enterStatus) {
			winningMenu.ShowBitmap();
		}
		else {
			winningMenu.UnshowBitmap();
		}

		if (dialogMenuing) {

			dialogMenu.ShowBitmap();

			if (inShopping) {
				optionMenu.ShowBitmap();
				optionArrow.ShowBitmap();
			}

			npc_map[npc.getID()].ShowBitmap();
		}
		else {
			dialogMenu.UnshowBitmap();
		}

		ShowText();

	}

	void CGameStateRun::ShowText() {

		CDC *pDC = CDDraw::GetBackCDC();
		CFont *fp;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));

		/* Attack Menu Text Setup */

		if (attackMenuing) {

			ChangeFontLog(pDC, fp, 40, "Noto Sans TC");

			pDC->TextOut(1167, 405, to_string(character.getHealth()).c_str());
			pDC->TextOut(1167, 465, to_string(character.getAttack()).c_str());
			pDC->TextOut(1167, 528, to_string(character.getDefence()).c_str());
			pDC->TextOut(1167, 586, to_string(character.getSpeed()).c_str());

			ChangeFontLog(pDC, fp, 60, "Noto Sans TC");

			pDC->TextOut(701, 330, monster.getName().c_str());

			ChangeFontLog(pDC, fp, 40, "Noto Sans TC");

			pDC->TextOut(977, 405, to_string(monster.getHealth()).c_str());
			pDC->TextOut(977, 465, to_string(monster.getAttack()).c_str());
			pDC->TextOut(977, 528, to_string(monster.getDefence()).c_str());
			pDC->TextOut(977, 586, to_string(monster.getSpeed()).c_str());

			if (showAttackValue == true) {

				ChangeFontLog(pDC, fp, 80, "Noto Sans TC");

				if (!turn) {
					pDC->SetTextColor(RGB(255, 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE), 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE)));
					pDC->TextOut(781 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), 487 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), causeDamageValueString.c_str());
				}
				else {
					pDC->SetTextColor(RGB(255, 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE), 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE)));
					pDC->TextOut(1507 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), 507 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), causeDamageValueString.c_str());
				}

				if (!enterStatus) {
					pDC->SetTextColor(RGB(255, 255, 0));
					ChangeFontLog(pDC, fp, 50, "Noto Sans TC");
					pDC->TextOut(1430, 630, string("�M�h(Q)").c_str());
				}
			}

		}

		if (attackMenuing && enterStatus || dialogMenuing) {
			/* Make Shine Enter */
			vector<int> vec;
			for (int i = 80; i <= 250; i += 10) {
				vec.push_back(i);
			}
			for (int i = 250; i >= 80; i -= 10) {
				vec.push_back(i);
			}
			int count = vec.size();
			pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));
			ChangeFontLog(pDC, fp, 50, "Noto Sans TC");
			if (!dialogMenuing) {
				pDC->TextOut(1430, 630, string("- Enter -").c_str());
			}
			else {
				pDC->TextOut(1293, 360, string("- Enter -").c_str());
			}
			/* Place gold and exp */
			if (attackMenuing) {
				pDC->SetTextColor(RGB(255, 255, 255));
				ChangeFontLog(pDC, fp, 80, "Noto Sans TC", 800);
				pDC->TextOut(1156, 688, to_string(monster.getExp()).c_str());
				pDC->TextOut(1470, 688, to_string(monster.getGold()).c_str());
			}
		}

		

		/* Dialog Menu Text Setup */

		if (dialogMenuing) {

			pDC->SetTextColor(RGB(255, 255, 255));

			vector<string> dialog = npc.getDialog();
			vector<string> option = npc.getOption();

			ChangeFontLog(pDC, fp, 60, "STZhongsong");

			pDC->TextOut(1100, 130, npc.getName().c_str());

			ChangeFontLog(pDC, fp, 30, "STZhongsong");

			for (int i = 0; i < (int)dialog.size(); i++) {
				pDC->TextOut(1010, 210 + i * 40, dialog[i].c_str());
			}

			if (inShopping) {
				ChangeFontLog(pDC, fp, 36, "Noto Sans TC");
				for (int i = 0; i < (int)option.size(); i++) {
					pDC->TextOut(1000, 480 + i * menuOptionGap, option[i].c_str());
				}
			}
		}

		/* Show Dashboard */

		ChangeFontLog(pDC, fp, 48, "Noto Sans TC");

		pDC->SetTextColor(RGB(255, 255, 255));

		pDC->TextOut(256 + 240, 140, character.getStatus().c_str());

		pDC->TextOut(256 + 250, 204, to_string(character.getLevel()).c_str());
		pDC->TextOut(256 + 250, 256, to_string(character.getHealth()).c_str());
		pDC->TextOut(256 + 250, 308, to_string(character.getAttack()).c_str());
		pDC->TextOut(256 + 250, 360, to_string(character.getDefence()).c_str());
		pDC->TextOut(256 + 250, 412, to_string(character.getSpeed()).c_str());
		pDC->TextOut(256 + 250, 460, to_string(character.getExp()).c_str());
		pDC->TextOut(256 + 790, 14, floor_message.c_str());

		ChangeFontLog(pDC, fp, 60, "Noto Sans TC", 800);

		pDC->TextOut(256 + 290, 615, to_string(character.getKeyNumber()).c_str());
		pDC->TextOut(256 + 290, 688, to_string(character.getSilverKeyNumber()).c_str());
		pDC->TextOut(256 + 290, 762, to_string(character.getGoldKeyNumber()).c_str());
		pDC->TextOut(256 + 290, 844, to_string(character.getCoin()).c_str());

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(0, 0, (to_string(mouse_x) + " " + to_string(mouse_y)).c_str());

		CDDraw::ReleaseBackCDC();
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
		mouse_x = point.x;
		mouse_y = point.y;
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{

	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{

	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{

	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{

	}
}