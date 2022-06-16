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
#include "AnyGateMenu.h"
#include <string>
#include <vector>
namespace game_framework {

	void POGSMenu::onShow() {
		menu.ShowBitmap();
	}

	void POGSMenu::LoadBitmap(vector<char*> vec) {
		menu.LoadBitmap(vec);
		menu.SetTopLeft(810, 580);
	}

	void POGSMenu::setParameter(Character _character) {
		character = _character;
	}

	void POGSMenu::onShowText(CDC *pDC, CFont* &fp) {

		ChangeFontLog(pDC, fp, 60, "STZhongsong");

		pDC->TextOut(860, 620, "¯«¼C¤§µý");

		int POGS_count = character.getItemCount(POGS);

		pDC->TextOut(1105, 620, ((string)("X " + to_string(POGS_count))).c_str());

		ChangeFontLog(pDC, fp, 36, "STZhongsong");

		pDC->TextOut(967, 725, "¡÷ ½T©w");


		ChangeFontLog(pDC, fp, 48, "STZhongsong");

		vector<int> vec = getShineVector();
		int count = vec.size();
		pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));

		pDC->TextOut(1095, 717, "-Enter-");
		pDC->TextOut(965, 815, "-Quit-(W)");

	}

	bool POGSMenu::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags, Character &character, bool &enterStatus, bool &pogsMenuing, map<int, string> monster_map) {
		const char KEY_ENTER = 0x0D;
		if (nChar == KEY_ENTER) {

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

			CGameStateRun::stage_entity[current_stage][x][y] = GREEN_SLIME;
			CGameStateRun::refresh_animation = true;
			character.setItemCount(POGS, character.getItemCount(POGS) - 1);
			pogsMenuing = false;
			enterStatus = false;

		}
		return true;
	}
}