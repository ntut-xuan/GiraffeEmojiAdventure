#pragma once
#include "stdafx.h"

using namespace std;

namespace game_framework {
	class AnyGateMenu : public GameMenu {
	public:
		AnyGateMenu() = default;
		void onShow();
		void setParameter(Character _character);
		bool onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags, Character &character, bool &enterStatus, bool &anyGateMenuing);
		void LoadBitmap(vector<char*> vec);
		void onShowText(CDC *pDC, CFont* &fp);
	private:
		CMovingBitmap menu;
		Character character;
	};
}