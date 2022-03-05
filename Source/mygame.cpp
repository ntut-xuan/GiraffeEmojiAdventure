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
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

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

		giraffe_photo.LoadBitmap("RES/giraffe.bmp", RGB(255, 255, 255));
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
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
		//
		// �}�l���J���
		//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �̲׶i�׬�100%
		//
		ShowInitProgress(100);
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
		current_stage = 1;
	}

	CGameStateRun::~CGameStateRun()
	{
		
	}

	void CGameStateRun::OnBeginState()
	{

		menuBitmap.LoadBitmapA("RES/menu.bmp", RGB(255, 255, 255));
		menuBitmap.SetTopLeft(78, 30);

		int start_x = 539;
		int start_y = 30;

		for (int s = 0; s < STAGES; s++) {
			vector<vector<int>> material_code = stage.getStageMaterial(s);
			vector<vector<int>> entity_code = stage.getStageEntity(s);

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (entity_code[i][j] == 1) {
						character.setXY(i, j);
					}
				}
			}

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (material_code[i][j] == 0) {
						material_map[s][i][j].LoadBitmap("RES/wall.bmp");
						material_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
						hidden_map[s][i][j].LoadBitmap("RES/wall.bmp");
						hidden_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (material_code[i][j] == 1) {
						material_map[s][i][j].LoadBitmap("RES/road.bmp");
						material_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
						hidden_map[s][i][j].LoadBitmap("RES/road.bmp");
						hidden_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
				}
			}

			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (entity_code[i][j] == BOX) {
						entity_map[s][i][j].LoadBitmap("RES/chest.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
					else if (entity_code[i][j] == LADDER) {
						entity_map[s][i][j].LoadBitmap("RES/ladder.bmp", RGB(255, 255, 255));
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
					else if (entity_code[i][j] == GREEN) {
						entity_map[s][i][j].LoadBitmap("RES/green.bmp", RGB(255, 255, 255));
						entity_map[s][i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
					}
				}
			}

		}

		characterBitmap.LoadBitmap("RES/character.bmp", RGB(255, 255, 255));
		characterBitmap.SetTopLeft(start_x + character.getY() * 77 , start_y + character.getX() * 77);

		health.LoadBitmap();
		health.SetInteger(character.getHealth());
		health.SetTopLeft(312, 169);

		attack.LoadBitmap();
		attack.SetInteger(character.getAttack());
		attack.SetTopLeft(312, 268);

		defence.LoadBitmap();
		defence.SetInteger(character.getDefence());
		defence.SetTopLeft(312, 373);

		keyNumber.LoadBitmapA();
		keyNumber.SetInteger(character.getKeyNumber());
		keyNumber.SetTopLeft(193, 707);

		silverKeyNumber.LoadBitmapA();
		silverKeyNumber.SetInteger(character.getSilverKeyNumber());
		silverKeyNumber.SetTopLeft(304, 707);

		goldKeyNumber.LoadBitmapA();
		goldKeyNumber.SetInteger(character.getGoldKeyNumber());
		goldKeyNumber.SetTopLeft(417, 707);
			
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
				hidden_code[x][y] = 1;
				character.changeKeyNumber(KEY, -1);
				keyNumber.Add(character.getKeyNumber() - keyNumber.GetInteger());
				return true;
			}
		}
		if (doorCode == SILVER_DOOR) {
			if (character.getSilverKeyNumber() > 0) {
				hidden_code[x][y] = 1;
				character.changeKeyNumber(SILVER_KEY, -1);
				silverKeyNumber.Add(character.getSilverKeyNumber() - silverKeyNumber.GetInteger());
				return true;
			}
		}
		if (doorCode == GOLD_DOOR) {
			if (character.getGoldKeyNumber() > 0) {
				hidden_code[x][y] = 1;
				character.changeKeyNumber(GOLD_KEY, -1);
				goldKeyNumber.Add(character.getGoldKeyNumber() - goldKeyNumber.GetInteger());
				return true;
			}
		}
		return false;
	}

	bool CGameStateRun::GetKey(int x, int y, int keyCode) {
		if (keyCode == KEY) {
			hidden_code[x][y] = 1;
			character.changeKeyNumber(KEY, 1);
			keyNumber.Add(1);
			return true;
		}
		if (keyCode == SILVER_KEY) {
			hidden_code[x][y] = 1;
			character.changeKeyNumber(SILVER_KEY, 1);
			silverKeyNumber.Add(1);
			return true;
		}
		if (keyCode == GOLD_KEY) {
			hidden_code[x][y] = 1;
			character.changeKeyNumber(GOLD_KEY, 1);
			goldKeyNumber.Add(1);
			return true;
		}
		return false;
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{	
		int start_x = 534;
		int start_y = 30;
		characterBitmap.SetTopLeft(start_x + character.getY() * 77, start_y + character.getX() * 77);
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
	
		vector<vector<int>> material_code = stage.getStageMaterial(current_stage);
		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);

		if (nChar == KEY_UP) {
			int x = character.getX() - 1;
			int y = character.getY();
			if (material_code[x][y] == 0) {
				return;
			}
			if (hidden_code[x][y] == 0 && isDoor(entity_code[x][y]) && OpenDoor(x, y, entity_code[x][y]) == false) {
				return;
			}
			if (entity_code[x][y] == LADDER) {
				current_stage++;
			}
			if (isKey(entity_code[x][y])) {
				GetKey(x, y, entity_code[x][y]);
			}
			if(eventEntity(entity_code[x][y])) {
				hidden_code[x][y] = 1;
			}
			character.setXY(x, y);
		}
		else if (nChar == KEY_LEFT) {
			int x = character.getX();
			int y = character.getY() - 1;
			if (material_code[x][y] == 0) {
				return;
			}
			if (hidden_code[x][y] == 0 && isDoor(entity_code[x][y]) && OpenDoor(x, y, entity_code[x][y]) == false) {
				return;
			}
			if (entity_code[x][y] == LADDER) {
				current_stage++;
			}
			if (isKey(entity_code[x][y])) {
				GetKey(x, y, entity_code[x][y]);
			}
			if (eventEntity(entity_code[x][y])) {
				hidden_code[x][y] = 1;
			}
			character.setXY(x, y);
		}
		else if (nChar == KEY_RIGHT) {
			int x = character.getX();
			int y = character.getY() + 1;
			if (material_code[x][y] == 0) {
				return;
			}
			if (hidden_code[x][y] == 0 && isDoor(entity_code[x][y]) && OpenDoor(x, y, entity_code[x][y]) == false) {
				return;
			}
			if (entity_code[x][y] == LADDER) {
				current_stage++;
			}
			if (isKey(entity_code[x][y])) {
				GetKey(x, y, entity_code[x][y]);
			}
			if (eventEntity(entity_code[x][y])) {
				hidden_code[x][y] = 1;
			}
			character.setXY(x, y);
		}
		else if (nChar == KEY_DOWN) {
			int x = character.getX() + 1;
			int y = character.getY();
			if (material_code[x][y] == 0) {
				return;
			}
			if (hidden_code[x][y] == 0 && isDoor(entity_code[x][y]) && OpenDoor(x, y, entity_code[x][y]) == false) {
				return;
			}
			if (entity_code[x][y] == LADDER) {
				current_stage++;
			}
			if (isKey(entity_code[x][y])) {
				GetKey(x, y, entity_code[x][y]);
			}
			if(eventEntity(entity_code[x][y])){
				hidden_code[x][y] = 1;
			}
			character.setXY(x, y);
		}
	
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
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{

	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{

	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
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
				if (hidden_code[i][j] == 1) {
					material_map[current_stage][i][j].ShowBitmap();
				}
				else {
					entity_map[current_stage][i][j].ShowBitmap();
				}
			}
		}
		
		characterBitmap.ShowBitmap();
		menuBitmap.ShowBitmap();
		health.ShowBitmap();
		attack.ShowBitmap();
		defence.ShowBitmap();
		keyNumber.ShowBitmap();
		silverKeyNumber.ShowBitmap();
		goldKeyNumber.ShowBitmap();
	}
}