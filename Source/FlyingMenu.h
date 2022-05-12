#pragma once
#include "stdafx.h"
using namespace std;

namespace game_framework {
	class FlyingMenu : public GameMenu {
	public:
		enum TOPDOWN { TOP, DOWN };
		static TOPDOWN topdown;
		FlyingMenu() = default;
		void onShow();
		void LoadBitmap();
		int getFlyingRange(TOPDOWN setting);
		int getFlyingSelect();
		void setFlyingRange(TOPDOWN setting, int value);
		void setFlyingSelect(int flyingSelect);
		void onShowText(CDC *pDC, CFont* &fp);
		pair<int, int> fetch_near_floor(TOPDOWN up_or_down, vector<vector<int>> entity_code, pair<int, int> character_location);
	private:
		CMovingBitmap flyingMenu;
		CMovingBitmap arrow_up_enable;
		CMovingBitmap arrow_up_disable;
		CMovingBitmap arrow_down_enable;
		CMovingBitmap arrow_down_disable;
		bool flying_enable = false;
		int flying_top_range = 0;
		int flying_buttom_range = 0;
		int flying_select;
	};
}