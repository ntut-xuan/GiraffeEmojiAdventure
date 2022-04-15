/*
 * mygame.h: ���ɮ��x�C��������class��interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "GameCharacter.h"
#include "Monster.h"
#include "NPC.h"
#include "Stage.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap giraffe_photo;
		CMovingBitmap logo;								// csie��logo
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		bool isDoor(int doorCode);
		bool isKey(int keyCode);
		bool OpenDoor(int x, int y, int doorCode);
		bool GetKey(int x, int y, int keyCode);
	private:

		const int		STAGES;	// ���d���`��
		const int MENU_DELAY_CYCLE = 10;
		int mouse_x, mouse_y;
		int temp_monster_x, temp_monster_y;
		int current_stage;
		int current_shop_price = 20;
		int tempDelayCycle = 0;
		std::string causeDamageValueString;
		bool getDialogMenuing = false;
		bool dialogMenuing = false;
		bool inShopping = false;
		bool attackMenuing = false;
		bool showAttackValue = true;
		bool enterStatus = false;
		bool turn = true; // ���a����
		bool teleport_allow = false;
		std::string floor_message;
		std::string temp_item_info;

		Stage stage;
		Character character;
		CMovingBitmap menuBitmap;
		CMovingBitmap characterBitmap;

		/* Attack Menu Object */
		CMovingBitmap attackMenu;
		CMovingBitmap characterAttackMenuBitMap;
		CMovingBitmap enemyAttackMenuBitmap;

		/* Winning Menu Object */
		CMovingBitmap winningMenu;
		CMovingBitmap hidden;

		/* Dialog Menu Object */
		CMovingBitmap dialogMenu;
		CMovingBitmap dialogMenuCharacter;

		/* Shoping Menu Object */
		int tempSelect = 0;
		const int OPTION_GAP = 150;
		int menuOptionGap = 120;
		CMovingBitmap optionMenu;
		CMovingBitmap optionArrow;

		/* Get Dialog Menu Object */
		CMovingBitmap getDialogMenu;

		/* Attack Animation Vector (enemy and character) */
		vector<CMovingBitmap> enemyAttackAnimation = vector<CMovingBitmap>(3);
		vector<CMovingBitmap> characterAttackAnimation = vector<CMovingBitmap>(3);

		Monster monster;
		NPC npc;
		map<int, int> bitmap_frame;
		map<int, Monster> monster_value;
		map<int, string> normal_bitmap_map;
		map<int, string> functional_entity_bitmap_map;
		map<int, string> monster_bitmap_map;
		map<int, string> npc_bitmap_map;
		map<int, string> block_bitmap_map;
		vector<CMovingBitmap> monster_map = vector<CMovingBitmap>(200);
		vector<CMovingBitmap> npc_map = vector<CMovingBitmap>(200);
		vector<vector<vector<int>>> hidden_code = vector<vector<vector<int>>>(STAGES, vector<vector<int>>(11, vector<int>(11)));
		//vector<vector<vector<CMovingBitmap>>> hidden_map = vector<vector<vector<CMovingBitmap>>>(STAGES, vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(11)));
		vector<vector<vector<CMovingBitmap>>> material_map = vector<vector<vector<CMovingBitmap>>>(STAGES, vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(11)));
		vector<vector<vector<CMovingBitmap>>> entity_map = vector<vector<vector<CMovingBitmap>>>(STAGES, vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(11)));

		void ShowText();
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		int counter;	// �˼Ƥ��p�ƾ�
	};

}