#pragma once
#include "stdafx.h"

using namespace std;

namespace game_framework {
	class GameSystem : public GameMenu {
	public:
		GameSystem() = default;
		void onShow();
		void setParameter(Character _character, int _mouse_x, int _mouse_y, string _floor_message);
		void LoadBitmap(vector<char*> vec);
		void onShowText(CDC *pDC, CFont* &fp);
	private:
		CMovingBitmap menu;
		Character character;
		int mouse_x;
		int mouse_y;
		string floor_message;
	};
}