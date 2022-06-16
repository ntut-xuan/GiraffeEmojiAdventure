#pragma once
#include "stdafx.h"

using namespace std;

namespace game_framework {
	class POGSMenu : public GameMenu {
	public:
		POGSMenu() = default;
		void onShow();
		void setParameter(Character _character);
		bool onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags, Character &character, bool &enterStatus, bool &anyGateMenuing, map<int, string> monster_map);
		void LoadBitmap(vector<char*> vec);
		void onShowText(CDC *pDC, CFont* &fp);
	private:
		CMovingBitmap menu;
		Character character;
	};
}