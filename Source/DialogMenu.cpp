#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "NPC.h"
#include "GameCharacter.h"
#include "mygame.h"
#include "Event.h"
#include "DialogMenu.h"
#include "GameMenu.h"
#include <string>
#include <vector>

namespace game_framework {

	void DialogMenu::LoadBitmap() {

		dialogMenu_T.LoadBitmap("RES/DialogMenu.bmp", RGB(0, 0, 0));
		dialogMenu_T.SetTopLeft(815 - 227, 107);

		dialogMenu_B.LoadBitmap("RES/DialogMenu.bmp", RGB(0, 0, 0));
		dialogMenu_B.SetTopLeft(815 - 227, 507);

		dialogMenu_C.LoadBitmap({ "RES/getDialog.bmp" });
		dialogMenu_C.SetTopLeft(656 - 227, 400);

		dialogMenuCharacter_B.LoadBitmap("RES/character_bg.bmp", RGB(255, 255, 255));
		dialogMenuCharacter_B.SetTopLeft(643, 564);

		optionMenu.LoadBitmap("RES/OptionMenu1.bmp", RGB(0, 0, 0));
		optionMenu.SetTopLeft(815 - 227, 414);

		optionArrow.LoadBitmap("RES/optionArrow.bmp", RGB(0, 0, 0));
		optionArrow.SetTopLeft(875 - 277, 454);

	}

	void DialogMenu::setNPC(NPC _npc, int _current_stage, int _x, int _y, bool &inShopping) {
		npc = _npc;
		npc.loadData(_current_stage, _x, _y);

		TRACE("%d %d %d\n", npc.getDialog().size(), npc.getOption().size(), npc.getSubOption().size());
		inShopping = npc.getOption().size();

		dialog = npc.getDialog();
		position = npc.getPosition();
		option = npc.getOption();
		suboption = npc.getSubOption();
		
		menuOptionGap = 100;
		current_stage = _current_stage;
		x = _x;
		y = _y;

		TRACE("%d %d %d\n", current_stage, x, y);

		current_dialog = 0;
	}

	void DialogMenu::setTemporaryDialog(POSITION position_flag, vector<string> _dialog, bool &enterStatus, bool &dialogMenuing) {
		dialog = { _dialog };
		current_dialog = 0;
		if (position_flag == TOP) {
			position = { 'T' };
		}
		if (position_flag == DOWN) {
			position = { 'B' };
		}
		if (position_flag == CENTER) {
			position = { 'C' };
		}
		enterStatus = true;
		dialogMenuing = true;
	}

	void DialogMenu::setNPCTemporaryDialog(vector<string> _dialog, bool &enterStatus, bool &dialogMenuing, NPC _npc) {
		dialog = { _dialog };
		position = { 'T' };
		npc = _npc;
		current_dialog = 0;
		enterStatus = true;
		dialogMenuing = true;
	}

	void DialogMenu::setNPCTemporaryDialog(vector<vector<string>> _dialog, vector<char> _position, bool &enterStatus, bool &dialogMenuing, NPC _npc) {
		dialog = _dialog;
		position = { _position };
		npc = _npc;
		current_dialog = 0;
		enterStatus = true;
		dialogMenuing = true;
		option = vector<string>(0);
	}

	void DialogMenu::setNPCMap(vector<CMovingBitmap> vec) {
		npc_map = vec;
	}

	bool DialogMenu::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags, Character &character, bool &inShopping, bool &enterStatus, bool &dialogMenuing) {

		const char KEY_UP = 0x26; // keyboard?W?b?Y
		const char KEY_DOWN = 0x28; // keyboard?U?b?Y
		const char KEY_ENTER = 0x0D;

		if (inShopping) {

			if (nChar == KEY_DOWN) {
				tempSelect += 1;
				tempSelect %= (int) option.size();
			}
			else if (nChar == KEY_UP) {
				tempSelect -= 1;
				tempSelect += (int) option.size();
				tempSelect %= (int) option.size();
			}
			else if (nChar == KEY_ENTER) {
				Event::triggerShopping(current_stage, x, y, character, tempSelect, inShopping, enterStatus, dialogMenuing);
				if (tempSelect == option.size() - 1) {
					inShopping = false;
					enterStatus = false;
					dialogMenuing = false;
				}
				dialog = npc.getDialog();
			}
			optionArrow.SetTopLeft(875 - 277, 454 + menuOptionGap * tempSelect);
			return false;
		}

		if (dialogMenuing && nChar == KEY_ENTER) {
			if (dialogMenuing && !inShopping) {
				if (current_dialog < (int) dialog.size() - 1) {
					current_dialog += 1;
				}
				else {
					current_dialog = 0;
					enterStatus = false;
					dialogMenuing = false;
					Event::triggerAfterDialog(current_stage, x, y, character);
					return true;
				}
			}
		}

		return false;

	}

	void DialogMenu::onShow() {

		if (position[current_dialog] == 'T') {

			dialogMenu_T.ShowBitmap();
			npc_map[npc.getID()].ShowBitmap();

		}

		else if (position[current_dialog] == 'B') {

			dialogMenu_B.ShowBitmap();
			dialogMenuCharacter_B.ShowBitmap();

		}

		else if (position[current_dialog] == 'C') {

			dialogMenu_C.ShowBitmap();

		}

		if ((int) option.size()) {
			optionMenu.ShowBitmap();
			optionArrow.ShowBitmap();
		}

	}

	void DialogMenu::onShowText(CDC *pDC, CFont* &fp, bool inShopping) {

		pDC->SetTextColor(RGB(255, 255, 255));

		int	offset_x = CDDraw::IsFullScreen() ? 0 : (1920 - SIZE_X) / 2;

		ChangeFontLog(pDC, fp, 60, "STZhongsong");

		int nameLength = npc.getName().length() > 3 ? npc.getName().length() : 0;

		if (position[current_dialog] == 'T') {
			CTextDraw::print(pDC, 1100 - 227 - nameLength * 5, 130, npc.getName().c_str());
		}
		else if (position[current_dialog] == 'B') {
			CTextDraw::print(pDC, 1100 - 227, 540, "???V??");
		}

		ChangeFontLog(pDC, fp, 30, "STZhongsong");

		for (int i = 0; i < (int)dialog[current_dialog].size(); i++) {
			if (position[current_dialog] == 'T') {
				CTextDraw::print(pDC, 1010 - 227, 210 + i * 40, dialog[current_dialog][i].c_str());
			}
			else if (position[current_dialog] == 'B') {
				CTextDraw::print(pDC, 1010 - 227, 630 + i * 40, dialog[current_dialog][i].c_str());
			}
			else if (position[current_dialog] == 'C') {
				ChangeFontLog(pDC, fp, 50, "Noto Sans TC");
				CTextDraw::print(pDC, 786 - 227, 415, dialog[current_dialog][0].c_str());
			}
		}

		if (inShopping) {
			ChangeFontLog(pDC, fp, 36, "Noto Sans TC");
			for (int i = 0; i < (int)option.size(); i++) {
				CTextDraw::print(pDC, 1000 - 227, 480 + i * menuOptionGap, option[i].c_str());
			}
			pDC->SetTextColor(RGB(0, 255, 0));
			ChangeFontLog(pDC, fp, 24, "Noto Sans TC");
			for (int i = 0; i < (int)suboption.size(); i++) {
				CTextDraw::print(pDC, 1200 - 227, 490 + i * menuOptionGap, suboption[i].c_str());
			}
		}

		vector<int> vec = getShineVector();
		int count = vec.size();
		pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));

		if (position[current_dialog] == 'T') {
			CTextDraw::print(pDC, 1293 - 227, 360, string("- Enter -").c_str());
		}
		else if (position[current_dialog] == 'B') {
			CTextDraw::print(pDC, 1293 - 227, 760, string("- Enter -").c_str());
		}
		else if (position[current_dialog] == 'C') {
			CTextDraw::print(pDC, 1430 - 227, 415, string("- Enter -").c_str());
		}

	}
}