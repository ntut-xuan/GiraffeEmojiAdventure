#pragma once
#include "stdafx.h"

using namespace std;

namespace game_framework {
	class SpyMenu : public GameMenu {
	public:
		SpyMenu() = default;
		void onShow();
		void LoadBitmap();
		void LoadMonsterBitmap(int ID, vector<char*> vec);
		void setMonsterData(vector<Monster> monsterData, Character character);
		void setPage(int page);
		void onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void onShowText(CDC *pDC, CFont* &fp);
	private:
		CMovingBitmap spyMenu;
		CMovingBitmap rightarrow, leftarrow;
		int spyMenuPage = 0;
		vector<Monster> spyMenuMonsterData;
		vector<CMovingBitmap> spyMenuSquare = vector<CMovingBitmap>(3);
		vector<vector<CMovingBitmap>> spyMenuIcon = vector<vector<CMovingBitmap>>(3, vector<CMovingBitmap>(1000));
	};
}