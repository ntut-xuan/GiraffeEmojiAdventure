/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <iostream>
#include <ddraw.h>
#include <time.h>
#include <string>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "MainFrm.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
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

		f.CreatePointFont(960, "Noto Sans TC");	
		fp=pDC->SelectObject(&f);
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetTextColor(RGB(0,0,0));
		pDC->TextOut(400, 50, "���V������");

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

		menuBitmap.LoadBitmapA("RES/menu.bmp", RGB(255, 255, 255));
		menuBitmap.SetTopLeft(239, 77);

		int start_x = 700;
		int start_y = 77;

		for (int s = 0; s < STAGES; s++) {
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
					if (entity_code[i][j] == BOX) {
						entity_map[s][i][j].LoadBitmap("RES/chest.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == UP_STAIR) {
						entity_map[s][i][j].LoadBitmap("RES/upstair.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == DOOR) {
						entity_map[s][i][j].LoadBitmap("RES/door.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SILVER_DOOR) {
						entity_map[s][i][j].LoadBitmap("RES/silverdoor.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == GOLD_DOOR) {
						entity_map[s][i][j].LoadBitmap("RES/golddoor.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == GRAPE) {
						entity_map[s][i][j].LoadBitmap("RES/grape.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == GOLD_KEY) {
						entity_map[s][i][j].LoadBitmap("RES/goldkey.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SILVER_KEY) {
						entity_map[s][i][j].LoadBitmap("RES/sliverkey.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == KEY) {
						entity_map[s][i][j].LoadBitmap("RES/key.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SUSHI) {
						entity_map[s][i][j].LoadBitmap("RES/sushi.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == GREEN_SLIME) {

						monster_map[GREEN_SLIME].LoadBitmap({ "RES/green.bmp", "RES/green2.bmp" }, RGB(0, 0, 0));
						monster_map[GREEN_SLIME].SetAnimation(5, false);
						monster_map[GREEN_SLIME].SetTopLeft(711, 417);

						entity_map[s][i][j].LoadBitmap({ "RES/green.bmp", "RES/green2.bmp" }, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == DOWN_STAIR) {
						entity_map[s][i][j].LoadBitmap("RES/downstair.bmp", RGB(0, 0, 0));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == RED_SLIME) {

						monster_map[RED_SLIME].LoadBitmap({ "RES/red.bmp", "RES/red2.bmp" }, RGB(0, 0, 0));
						monster_map[RED_SLIME].SetAnimation(5, false);
						monster_map[RED_SLIME].SetTopLeft(711, 417);

						entity_map[s][i][j].LoadBitmap({ "RES/red.bmp", "RES/red2.bmp" }, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == BAT) {

						monster_map[BAT].LoadBitmap({ "RES/bat.bmp", "RES/bat2.bmp" }, RGB(0, 0, 0));
						monster_map[BAT].SetAnimation(5, false);
						monster_map[BAT].SetTopLeft(711, 417);

						entity_map[s][i][j].LoadBitmap({ "RES/bat.bmp", "RES/bat2.bmp" }, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == ZOMBIE) {

						monster_map[ZOMBIE].LoadBitmap({ "RES/zombie.bmp", "RES/zombie2.bmp" }, RGB(0, 0, 0));
						monster_map[ZOMBIE].SetAnimation(5, false);
						monster_map[ZOMBIE].SetTopLeft(711, 417);

						entity_map[s][i][j].LoadBitmap({ "RES/zombie.bmp", "RES/zombie2.bmp" }, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SKELETON) {

						monster_map[SKELETON].LoadBitmap({ "RES/skeleton.bmp", "RES/skeleton2.bmp" }, RGB(0, 0, 0));
						monster_map[SKELETON].SetAnimation(5, false);
						monster_map[SKELETON].SetTopLeft(711, 417);

						entity_map[s][i][j].LoadBitmap({ "RES/skeleton.bmp", "RES/skeleton2.bmp" }, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SHOPKEEPER) {

						npc_map[SHOPKEEPER].LoadBitmap({ "RES/shopkeeper.bmp", "RES/shopkeeper2.bmp" }, RGB(0, 0, 0));
						npc_map[SHOPKEEPER].SetAnimation(5, false);
						npc_map[SHOPKEEPER].SetTopLeft(870, 161);

						entity_map[s][i][j].LoadBitmap({ "RES/shopkeeper.bmp", "RES/shopkeeper2.bmp" }, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SHOP1) {

						npc_map[SHOP1].LoadBitmap("RES/shop1.bmp", RGB(0, 0, 0));
						npc_map[SHOP1].SetAnimation(5, false);
						npc_map[SHOP1].SetTopLeft(870, 161);

						entity_map[s][i][j].LoadBitmap("RES/shop1.bmp", RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SHOP2) {
						npc_map[SHOP2].LoadBitmap({ "RES/shop2.bmp", "RES/shop22.bmp" }, RGB(0, 0, 0));
						npc_map[SHOP2].SetAnimation(5, false);
						npc_map[SHOP2].SetTopLeft(870, 161);

						entity_map[s][i][j].LoadBitmap({"RES/shop2.bmp", "RES/shop22.bmp"}, RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == SHOP3) {
						npc_map[SHOP3].LoadBitmap("RES/shop3.bmp", RGB(0, 0, 0));
						npc_map[SHOP3].SetAnimation(5, false);
						npc_map[SHOP3].SetTopLeft(870, 161);

						entity_map[s][i][j].LoadBitmap("RES/shop3.bmp", RGB(0, 0, 0));
						entity_map[s][i][j].SetAnimation(5, false);
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
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

		/* Panel Value Load */

		health.LoadBitmap();
		health.SetInteger(character.getHealth());
		health.SetTopLeft(473, 216);

		attack.LoadBitmap();
		attack.SetInteger(character.getAttack());
		attack.SetTopLeft(473, 315);

		defence.LoadBitmap();
		defence.SetInteger(character.getDefence());
		defence.SetTopLeft(473, 420);

		attackMenu.LoadBitmap({ "RES/AttackPlatform.bmp" }, RGB(0, 0, 0));
		attackMenu.SetTopLeft(656, 297);

		winningMenu.LoadBitmap({ "RES/WinPlatform.bmp" }, RGB(0, 0, 0));
		winningMenu.SetTopLeft(656, 682);

		keyNumber.LoadBitmapA();
		keyNumber.SetInteger(character.getKeyNumber());
		keyNumber.SetTopLeft(354, 754);

		silverKeyNumber.LoadBitmapA();
		silverKeyNumber.SetInteger(character.getSilverKeyNumber());
		silverKeyNumber.SetTopLeft(465, 754);

		goldKeyNumber.LoadBitmapA();
		goldKeyNumber.SetInteger(character.getGoldKeyNumber());
		goldKeyNumber.SetTopLeft(578, 754);
			
	}

	bool CGameStateRun::isDoor(int doorCode) {
		return doorCode == DOOR || doorCode == SILVER_DOOR || doorCode == GOLD_DOOR;
	}

	bool CGameStateRun::isEnemy(int enemyCode) {
		return enemyCode == GREEN_SLIME || enemyCode == RED_SLIME || enemyCode == BAT || enemyCode == ZOMBIE || enemyCode == SKELETON;
	}

	bool CGameStateRun::isKey(int keyCode) {
		return keyCode == KEY || keyCode == SILVER_KEY || keyCode == GOLD_KEY;
	}

	bool CGameStateRun::isNPC(int entityCode) {
		return entityCode == SHOPKEEPER || entityCode == SHOP1 || entityCode == SHOP2 || entityCode == SHOP3;
	}

	bool CGameStateRun::OpenDoor(int x, int y, int doorCode) {
		if (doorCode == DOOR) {
			if (character.getKeyNumber() > 0) {
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(KEY, -1);
				keyNumber.Add(character.getKeyNumber() - keyNumber.GetInteger());
				return true;
			}
		}
		if (doorCode == SILVER_DOOR) {
			if (character.getSilverKeyNumber() > 0) {
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(SILVER_KEY, -1);
				silverKeyNumber.Add(character.getSilverKeyNumber() - silverKeyNumber.GetInteger());
				return true;
			}
		}
		if (doorCode == GOLD_DOOR) {
			if (character.getGoldKeyNumber() > 0) {
				hidden_code[current_stage][x][y] = 1;
				character.changeKeyNumber(GOLD_KEY, -1);
				goldKeyNumber.Add(character.getGoldKeyNumber() - goldKeyNumber.GetInteger());
				return true;
			}
		}
		return false;
	}

	bool CGameStateRun::GetKey(int x, int y, int keyCode) {
		if (keyCode == KEY) {
			hidden_code[current_stage][x][y] = 1;
			character.changeKeyNumber(KEY, 1);
			keyNumber.Add(1);
			return true;
		}
		if (keyCode == SILVER_KEY) {
			hidden_code[current_stage][x][y] = 1;
			character.changeKeyNumber(SILVER_KEY, 1);
			silverKeyNumber.Add(1);
			return true;
		}
		if (keyCode == GOLD_KEY) {
			hidden_code[current_stage][x][y] = 1;
			character.changeKeyNumber(GOLD_KEY, 1);
			goldKeyNumber.Add(1);
			return true;
		}
		return false;
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{	

		if (character.getHealth() == 0) {
			GotoGameState(GAME_STATE_OVER);
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
					health.Add(character.getHealth() - health.GetInteger());
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

		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
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
		if (hidden_code[current_stage][x][y] == 0 && isEnemy(entity_code[x][y])) {
			attackMenuing = true;
			if (entity_code[x][y] == GREEN_SLIME) {
				monster = Monster(GREEN_SLIME, "���v�ܩi", 40, 18, 1, 0);
			}
			else if (entity_code[x][y] == RED_SLIME) {
				monster = Monster(RED_SLIME, "����v�ܩi", 50, 20, 4, 0);
			}
			else if (entity_code[x][y] == BAT) {
				monster = Monster(BAT, "����", 52, 32, 2, 4);
			}
			else if (entity_code[x][y] == SKELETON) {
				monster = Monster(SKELETON, "�u�\", 95, 70, 0, 3);
			}
			else if (entity_code[x][y] == ZOMBIE) {
				monster = Monster(ZOMBIE, "�L��", 190, 90, 33, 2);
			}
			temp_monster_x = x;
			temp_monster_y = y;
			return;
		}
		if (hidden_code[current_stage][x][y] == 0 && isNPC(entity_code[x][y])) {
			if (entity_code[x][y] == SHOPKEEPER) {
				dialogMenuing = true;
				enterStatus = true;
				npc = NPC(SHOPKEEPER, "�ӤH");
				npc.loadData(x, y);
			}
			else if (entity_code[x][y] == SHOP2) {
				dialogMenuing = true;
				enterStatus = true;
				inShopping = true;
				npc = NPC(SHOP2, "�g������");
				npc.setVariable(0, 20);
				npc.loadData(x, y);
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
		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
	}

	void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
	{

		current_stage = 1;

		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 1) {
					character.setXY(i, j);
				}
			}
		}

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
		health.ShowBitmap(false);
		attack.ShowBitmap(false);
		defence.ShowBitmap(false);
		keyNumber.ShowBitmap(true);
		silverKeyNumber.ShowBitmap(true);
		goldKeyNumber.ShowBitmap(true);

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
	}
}