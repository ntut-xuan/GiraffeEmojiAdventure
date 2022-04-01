/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

// #define UU_DEBUG
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

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

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
		//ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		logo.LoadBitmap(IDB_BACKGROUND);

		giraffe_photo.LoadBitmap("./RES/giraffe.bmp", RGB(255, 255, 255));
		giraffe_photo.SetTopLeft(600, 300);

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
			GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}

	void CGameStateInit::OnShow(){

		giraffe_photo.ShowBitmap();

		CDC *pDC = CDDraw::GetBackCDC();
		CFont f,*fp;

		time_t rawtime;
		time(&rawtime);

		f.CreatePointFont(960, "Noto Sans TC");	
		fp=pDC->SelectObject(&f);
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetTextColor(RGB(0,0,0));
		pDC->TextOut(400, 50, "長頸鹿爬塔");

		f.Detach();
		f.CreatePointFont(160, "Noto Sans TC");
		fp = pDC->SelectObject(&f);

		pDC->TextOut(650, 740, " - 按 Space 開始遊戲 - ");
		pDC->TextOut(20, 850, "按 Ctrl + Q 暫停遊戲");
		pDC->TextOut(20, 850, "按 Ctrl + Q 暫停遊戲");
		pDC->TextOut(1000, 850, ctime(&rawtime));

		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}								

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
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
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f,*fp;
		f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
		fp=pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(255,255,0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240,210,str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g) : CGameState(g), STAGES(10)
	{	
		current_stage = 0;
		hidden_map.clear();
	}

	CGameStateRun::~CGameStateRun()
	{
		
	}

	void CGameStateRun::OnBeginState()
	{

#ifdef UU_DEBUG 
		character.setAttack(998244353); character.setDefence(998244353); character.setHealth(998244353); character.setSpeed(998244353);
#endif 

		menuBitmap.LoadBitmapA("RES/menu.bmp", RGB(0, 0, 0));
		menuBitmap.SetTopLeft(239, 0);

			
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
				TRACE("%d\n", ID);
				if (r == 0) normal_bitmap_map[ID] = file_name;
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
				}else if (r == 3) {
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
			Monster monster(ID, name, health, attack, defence, speed);
			monster_value[ID] = monster;
		}

		for (int s = 0; s <= 5; s++) {
			vector<vector<int>> material_code = stage.getStageMaterial(s);
			vector<vector<int>> entity_code = stage.getStageEntity(s);

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (material_code[i][j] == WALL) {
						material_map[s][i][j].LoadBitmap("RES/wall.bmp");
						material_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (material_code[i][j] == ROAD) {
						material_map[s][i][j].LoadBitmap("RES/road.bmp");
						material_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (material_code[i][j] == LAVA) {
						material_map[s][i][j].LoadBitmap({ "RES/lava.bmp", "RES/lava2.bmp" });
						material_map[s][i][j].SetAnimation(5, false);
						material_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
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
						string filename = "RES/" + normal_bitmap_map[ID] + ".bmp";
						char* char_filename = (char*)filename.c_str();
						entity_map[s][i][j].LoadBitmap(char_filename);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (functional_entity_bitmap_map[ID].length() != 0) {
						string filename = "RES/" + functional_entity_bitmap_map[ID] + ".bmp";
						char* char_filename = (char*)filename.c_str();
						entity_map[s][i][j].LoadBitmap(char_filename);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (block_bitmap_map[ID].length() != 0) {
						string filename = "RES/" + block_bitmap_map[ID] + ".bmp";
						char* char_filename = (char*)filename.c_str();
						entity_map[s][i][j].LoadBitmap(char_filename);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else {
						if (monster_bitmap_map[ID].length() != 0) {
							string filename = "RES/" + monster_bitmap_map[ID] + ".bmp";
							string filename2 = "RES/" + monster_bitmap_map[ID] + "2.bmp";
							char* char_filename = (char*) filename.c_str();
							char* char_filename2 = (char*) filename2.c_str();
							entity_map[s][i][j].LoadBitmap({ char_filename, char_filename2 });
							entity_map[s][i][j].SetAnimation(5, false);
							entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
						}
						else if (npc_bitmap_map[ID].length() != 0) {
							string filename = "RES/" + npc_bitmap_map[ID] + ".bmp";
							string filename2 = "RES/" + npc_bitmap_map[ID] + "2.bmp";
							char* char_filename = (char*) filename.c_str();
							char* char_filename2 = (char*) filename2.c_str();
							entity_map[s][i][j].LoadBitmap({ char_filename, char_filename2 });
							entity_map[s][i][j].SetAnimation(5, false);
							entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
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
		characterAttackAnimation[0].SetTopLeft(1387, 387);
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
		characterAttackMenuBitMap.SetTopLeft(1427, 427);

		characterBitmap.LoadBitmap({ "RES/character.bmp", "RES/character2.bmp" }, RGB(255, 255, 255));
		characterBitmap.SetTopLeft(start_x + character.getY() * 77 , start_y + character.getX() * 77);

		attackMenu.LoadBitmap({ "RES/AttackPlatform.bmp" }, RGB(0, 0, 0));
		attackMenu.SetTopLeft(656, 297);

		winningMenu.LoadBitmap({ "RES/WinPlatform.bmp" }, RGB(0, 0, 0));
		winningMenu.SetTopLeft(656, 682);
			
	}

	bool CGameStateRun::isDoor(int doorCode) {
		return doorCode == DOOR || doorCode == SILVER_DOOR || doorCode == GOLD_DOOR;
	}

	bool CGameStateRun::isKey(int keyCode) {
		return keyCode == KEY || keyCode == SILVER_KEY || keyCode == GOLD_KEY;
	}

	bool CGameStateRun::OpenDoor(int x, int y, int doorCode) {
		if (doorCode == DOOR) {
			if (character.getKeyNumber() > 0) {
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(KEY, -1);
				return true;
			}
		}
		if (doorCode == SILVER_DOOR) {
			if (character.getSilverKeyNumber() > 0) {
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(SILVER_KEY, -1);
				return true;
			}
		}
		if (doorCode == GOLD_DOOR) {
			if (character.getGoldKeyNumber() > 0) {
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(GOLD_KEY, -1);
				return true;
			}
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

		if (character.getHealth() == 0) {
			GotoGameState(GAME_STATE_OVER);
		}

		floor_message = "主塔　";

		if (current_stage == 0) {
			floor_message += "入口";
		}
		else {
			floor_message += to_string(current_stage) + "F";
		}

		int start_x = 700;
		int start_y = 77;
		characterBitmap.SetTopLeft(start_x + character.getY() * 77, start_y + character.getX() * 77);

		if (attackMenuing && --tempDelayCycle <= 0) {
			if (character.getHealth() && monster.getHealth()) {
				if (turn) {
					tempCauseDamageValue = max(0, character.getAttack() - monster.getDefence());
					monster.causeDamage(tempCauseDamageValue);
					enemyAttackAnimation[0].SetAnimation(1, true);
				}
				else {
					tempCauseDamageValue = max(0, monster.getAttack() - character.getDefence());
					character.causeDamage(tempCauseDamageValue);
					characterAttackAnimation[0].SetAnimation(1, true);
				}
				turn = !turn;
				showAttackValue = true;
			}

			if (enterStatus == true) {
				showAttackValue = false;
			}

			if (monster.getHealth() == 0) {
				enterStatus = true;
				turn = false;
			}

			tempDelayCycle = MENU_DELAY_CYCLE;

		}

	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_ENTER = 0x0D;

		TRACE("%04X\n", nChar);

		if (inShopping) {
			if (nChar == KEY_DOWN) {
				tempSelect += 1;
				tempSelect %= (int) npc.getOption().size();
			} else if (nChar == KEY_UP) {
				tempSelect -= 1;
				tempSelect += (int) npc.getOption().size();
				tempSelect %= (int) npc.getOption().size();
			}
			else if (nChar == KEY_ENTER) {
				inShopping = false;
				enterStatus = false;
				dialogMenuing = false;
			}
			optionArrow.SetTopLeft(875, 464 + menuOptionGap * tempSelect);
			return;
		}

		vector<vector<int>> material_code = stage.getStageMaterial(current_stage);
		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		int x = character.getX();
		int y = character.getY();

		if (enterStatus && KEY_ENTER) {
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

		if (material_code[x][y] == 0) {
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
				npc = NPC(SHOPKEEPER, "商人");
				npc.loadData(current_stage, x, y);
			}
			else if (entity_code[x][y] == SHOP2) {
				dialogMenuing = true;
				enterStatus = true;
				inShopping = true;
				npc = NPC(SHOP2, "貪婪之神");
				npc.setVariable(0, 20);
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

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{

		current_stage = 0;

		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 1) {
					character.setXY(i, j);
				}
			}
		}

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
				if (hidden_code[current_stage][i][j] == 1) {
					entity_map[current_stage][i][j].UnshowBitmap();
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

			CDC *pDC = CDDraw::GetBackCDC();
			CFont f, *fp;

			f.CreatePointFont(300, "Noto Sans TC");
			fp = pDC->SelectObject(&f);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->TextOut(1167, 437, to_string(character.getHealth()).c_str());
			pDC->TextOut(1167, 503, to_string(character.getAttack()).c_str());
			pDC->TextOut(1167, 573, to_string(character.getDefence()).c_str());

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->TextOut(751, 360, monster.getName().c_str());
			pDC->TextOut(977, 437, to_string(monster.getHealth()).c_str());
			pDC->TextOut(977, 503, to_string(monster.getAttack()).c_str());
			pDC->TextOut(977, 573, to_string(monster.getDefence()).c_str());

			if (showAttackValue == true) {
				if (!turn) {
					f.Detach();
					f.CreatePointFont(400, "Noto Sans TC");
					fp = pDC->SelectObject(&f);

					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(RGB(255, 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE), 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE)));
					pDC->TextOut(781 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), 487 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), to_string(tempCauseDamageValue).c_str());
				}
				else {
					f.Detach();
					f.CreatePointFont(400, "Noto Sans TC");
					fp = pDC->SelectObject(&f);

					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(RGB(255, 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE), 0 + tempDelayCycle * (255 / MENU_DELAY_CYCLE)));
					pDC->TextOut(1507 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), 507 - tempDelayCycle * (25 / MENU_DELAY_CYCLE), to_string(tempCauseDamageValue).c_str());
				}
			}

			pDC->SelectObject(fp);
			CDDraw::ReleaseBackCDC();

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

			CDC *pDC = CDDraw::GetBackCDC();
			CFont f, *fp;

			f.CreatePointFont(450, "STZhongsong");
			fp = pDC->SelectObject(&f);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->TextOut(1100, 130, npc.getName().c_str());

			f.Detach();
			f.CreatePointFont(200, "Noto Sans TC");
			fp = pDC->SelectObject(&f);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255, 255, 255));

			vector<string> dialog = npc.getDialog();
			vector<string> option = npc.getOption();

			for (int i = 0; i < (int) dialog.size(); i++) {
				pDC->TextOut(1010, 210 + i * 40, dialog[i].c_str());
			}

			pDC->TextOutA(1293, 360, "-Enter-");

			if (inShopping) {

				f.Detach();
				f.CreatePointFont(260, "STZhongsong");
				fp = pDC->SelectObject(&f);

				for (int i = 0; i < (int)option.size(); i++) {
					pDC->TextOut(1000, 480 + i * menuOptionGap, option[i].c_str());
				}
			
			}

			pDC->SelectObject(fp);
			CDDraw::ReleaseBackCDC();

		}
		else {
			dialogMenu.UnshowBitmap();
		}

		CDC *pDC = CDDraw::GetBackCDC();
		CFont f, *fp;
		
		f.CreatePointFont(240, "Noto Sans TC");
		fp = pDC->SelectObject(&f);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(256 + 240, 140, character.getStatus().c_str());
		pDC->TextOut(256 + 250, 204, to_string(character.getLevel()).c_str());
		pDC->TextOut(256 + 250, 256, to_string(character.getHealth()).c_str());
		pDC->TextOut(256 + 250, 308, to_string(character.getAttack()).c_str());
		pDC->TextOut(256 + 250, 360, to_string(character.getDefence()).c_str());
		pDC->TextOut(256 + 250, 412, to_string(character.getSpeed()).c_str());
		pDC->TextOut(256 + 250, 460, to_string(character.getExp()).c_str());
		pDC->TextOut(256 + 790, 14, floor_message.c_str());

		f.Detach();
		f.CreatePointFont(300, "Noto Sans TC");
		fp = pDC->SelectObject(&f);

		pDC->TextOut(256 + 290, 615, to_string(character.getKeyNumber()).c_str());
		pDC->TextOut(256 + 290, 688, to_string(character.getSilverKeyNumber()).c_str());
		pDC->TextOut(256 + 290, 762, to_string(character.getGoldKeyNumber()).c_str());
		pDC->TextOut(256 + 290, 844, to_string(character.getCoin()).c_str());

		
		pDC->SelectObject(fp);
		CDDraw::ReleaseBackCDC();
	}



	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
		mouse_x = point.x;
		mouse_y = point.y;
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{

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
}