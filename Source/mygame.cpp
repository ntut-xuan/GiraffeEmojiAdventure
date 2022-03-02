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

		vector<vector<int>> material_code = stage.getStageMaterial(current_stage);
		material_map = vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(11));
		vector<vector<int>> entity_code = stage.getStageEntity(current_stage);
		entity_map = vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(11));

		menuBitmap.LoadBitmapA("RES/menu.bmp", RGB(255, 255, 255));
		menuBitmap.SetTopLeft(78, 30);

		int start_x = 539;
		int start_y = 30;

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
					material_map[i][j].LoadBitmap("RES/wall.bmp");
					material_map[i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
				}
				else if (material_code[i][j] == 1) {
					material_map[i][j].LoadBitmap("RES/road.bmp");
					material_map[i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
				}
			}
		}

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 2) {
					entity_map[i][j].LoadBitmap("RES/chest.bmp", RGB(255, 255, 255));
					entity_map[i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
				}
				else if (entity_code[i][j] == 3) {
					entity_map[i][j].LoadBitmap("RES/ladder.bmp", RGB(255, 255, 255));
					entity_map[i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
				}
				else if (entity_code[i][j] == 4) {
					entity_map[i][j].LoadBitmap("RES/normal_door.bmp", RGB(255, 255, 255));
					entity_map[i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
				}
				else if (entity_code[i][j] == 5) {
					entity_map[i][j].LoadBitmap("RES/grape.bmp", RGB(255, 255, 255));
					entity_map[i][j].SetTopLeft(start_x + 77 * j, start_y + 77 * i);
				}
			}
		}

		characterBitmap.LoadBitmap("RES/character.bmp", RGB(255, 255, 255));
		characterBitmap.SetTopLeft(start_x + character.getY() * 77 , start_y + character.getX() * 77);
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
			if (material_code[character.getX() - 1][character.getY()] == 0 || entity_code[character.getX() - 1][character.getY()] == 4) {
				return;
			}
			character.setXY(character.getX() - 1, character.getY());
		}
		else if (nChar == KEY_LEFT) {
			if (material_code[character.getX()][character.getY() - 1] == 0 || entity_code[character.getX()][character.getY() - 1] == 4) {
				return;
			}
			character.setXY(character.getX(), character.getY() -1);
		}
		else if (nChar == KEY_RIGHT) {
			if (material_code[character.getX()][character.getY() + 1] == 0 || entity_code[character.getX()][character.getY() + 1] == 4) {
				return;
			}
			character.setXY(character.getX(), character.getY() + 1);
		}
		else if (nChar == KEY_DOWN) {
			if (material_code[character.getX() + 1][character.getY()] == 0 || entity_code[character.getX() + 1][character.getY()] == 4) {
				return;
			}
			character.setXY(character.getX() + 1, character.getY());
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
				material_map[i][j].ShowBitmap();
			}
		}
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == 0) continue;
				if (entity_code[i][j] == 1) continue;
				entity_map[i][j].ShowBitmap();
			}
		}
		
		characterBitmap.ShowBitmap();
		menuBitmap.ShowBitmap();

	}
}