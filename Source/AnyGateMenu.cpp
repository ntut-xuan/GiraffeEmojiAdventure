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

	void AnyGateMenu::onShow() {
		menu.ShowBitmap();
	}
	
	void AnyGateMenu::LoadBitmap(vector<char*> vec) {
		menu.LoadBitmap(vec);
		menu.SetTopLeft(810, 580);
	}

	void AnyGateMenu::setParameter(Character _character) {
		character = _character;
	}

	void AnyGateMenu::onShowText(CDC *pDC, CFont* &fp) {

		ChangeFontLog(pDC, fp, 60, "STZhongsong");

		pDC->TextOut(920, 620, "任意門");


		pDC->TextOut(1105, 620, ((string)("X " + to_string(character.getItemCount(ANY_GATE)))).c_str());

		ChangeFontLog(pDC, fp, 36, "STZhongsong");

		pDC->TextOut(967, 725, "→ 確定");


		ChangeFontLog(pDC, fp, 48, "STZhongsong");

		vector<int> vec = getShineVector();
		int count = vec.size();
		pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));

		pDC->TextOut(1095, 717, "-Enter-");
		pDC->TextOut(965, 815, "-Quit-(T)");

	}

	bool AnyGateMenu::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags, Character &character, bool &enterStatus, bool &anyGateMenuing) {
		const char KEY_ENTER = 0x0D;
		if (nChar == KEY_ENTER) {
			
			int current_stage = character.getCurrentStage();
			int x = character.getX();
			int y = character.getY();
			int faced_direction = character.getFacedDirection();

			TRACE("ori = %d - %d, %d\n", faced_direction, x, y);

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

			TRACE("calc = %d - %d, %d\n", faced_direction, x, y);

			CGameStateRun::hidden_code[current_stage][x][y] = 0;
			CGameStateRun::stage_entity[current_stage][x][y] = DOOR;
			CGameStateRun::stage_material[current_stage][x][y] = ROAD;
			CGameStateRun::refresh_animation = true;
			character.setItemCount(ANY_GATE, character.getItemCount(ANY_GATE) - 1);
			anyGateMenuing = false;
			enterStatus = false;
			
		}
		return true;
	}
}