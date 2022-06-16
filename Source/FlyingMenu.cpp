#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameCharacter.h"
#include "GameMenu.h"
#include "mygame.h"
#include "FlyingMenu.h"
#include <string>
#include <vector>

namespace game_framework {

	pair<int, int> FlyingMenu::fetch_near_floor(int target_stage, TOPDOWN up_or_down, vector<vector<int>> entity_code, pair<int, int> character_location) {
		double distance_up = 20020625;
		double distance_down = 20020625;
		pair<int, int> up_stair_location = { -1, -1 };
		pair<int, int> down_stair_location = { -1, -1 };
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (entity_code[i][j] == UP_STAIR) {
					double temp_distance = (character_location.first - i) * (character_location.first - i) + (character_location.second - j) * (character_location.second - j);
					if (temp_distance < distance_up) {
						distance_up = temp_distance;
						up_stair_location = { i, j };
					}
				}
				else if (entity_code[i][j] == DOWN_STAIR) {
					double temp_distance = (character_location.first - i) * (character_location.first - i) + (character_location.second - j) * (character_location.second - j);
					if (temp_distance < distance_down) {
						distance_down = temp_distance;
						down_stair_location = { i, j };
					}
				}
			}
		}
		if (target_stage == 36) {
			return { 7, 1 };
		}
		if (target_stage == 41) {
			return { 10,  5 };
		}
		if (target_stage == 46) {
			return { 9,  5 };
		}
		if (up_or_down == TOP) {
			return up_stair_location.first == -1 ? down_stair_location : up_stair_location;
		}
		else {
			return down_stair_location.first == -1 ? up_stair_location : down_stair_location;
		}
	}

	void FlyingMenu::LoadBitmap() {

		flyingMenu.LoadBitmap("RES/flying_menu.bmp");
		flyingMenu.SetTopLeft(CGameStateRun::start_x, CGameStateRun::start_y);

		arrow_up_enable.LoadBitmap("RES/arrow_up_enable.bmp", RGB(255, 255, 255));
		arrow_up_enable.SetTopLeft(815, 250);

		arrow_down_enable.LoadBitmap("RES/arrow_down_enable.bmp", RGB(255, 255, 255));
		arrow_down_enable.SetTopLeft(815, 650);

		arrow_up_disable.LoadBitmap("RES/arrow_up_disable.bmp", RGB(255, 255, 255));
		arrow_up_disable.SetTopLeft(815, 250);

		arrow_down_disable.LoadBitmap("RES/arrow_down_disable.bmp", RGB(255, 255, 255));
		arrow_down_disable.SetTopLeft(815, 650);
	}

	int FlyingMenu::getFlyingRange(TOPDOWN setting) {
		return setting == TOP ? flying_top_range: flying_buttom_range;
	}

	int FlyingMenu::getFlyingSelect() {
		return flying_select;
	}

	void FlyingMenu::setFlyingRange(TOPDOWN setting, int value) {
		setting == TOP ? flying_top_range = value : flying_buttom_range = value;
		TRACE("%d %d\n", flying_top_range, flying_buttom_range);
	}

	void FlyingMenu::setFlyingSelect(int _flyingSelect) {
		flying_select = _flyingSelect;
	}

	void FlyingMenu::onShow() {
		flyingMenu.ShowBitmap();
		if (flying_select == flying_top_range) {
			//arrow_up_disable.ShowBitmap();
		}
		else {
			arrow_up_enable.ShowBitmap();
		}
		if (flying_select == flying_buttom_range) {
			//arrow_down_disable.ShowBitmap();
		}
		else {
			arrow_down_enable.ShowBitmap();
		}
	}

	void FlyingMenu::onShowText(CDC *pDC, CFont* &fp) {
		ChangeFontLog(pDC, fp, 130, "STZhongsong");

		pDC->SetTextColor(RGB(255, 255, 255));
		CTextDraw::print(pDC, 850, 435, to_string(flying_select - 26).c_str());

		vector<int> vec = getShineVector();
		int count = (int)vec.size();
		pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));
		ChangeFontLog(pDC, fp, 50, "STZhongsong");

		CTextDraw::print(pDC, 1050, 835, "- Enter -");
		CTextDraw::print(pDC, 1050, 735, "- Quit(F) -");
	}
}