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
#include "SpyMenu.h"
#include <string>
#include <vector>

namespace game_framework {

	void SpyMenu::LoadBitmap() {
		spyMenu.LoadBitmap("RES/spy_menu.bmp");
		spyMenu.SetTopLeft(CGameStateRun::start_x, CGameStateRun::start_y);
		for (int i = 0; i < 3; i++) {
			spyMenuSquare[i].LoadBitmap("RES/spy_menu_square.bmp");
			spyMenuSquare[i].SetTopLeft(519 - 9, 200 - 19 + i * 250);
		}
		rightarrow.LoadBitmap({ "RES/arrow_right_enable.bmp", "RES/arrow_right_disable.bmp" }, RGB(255, 255, 255));
		rightarrow.SetTopLeft(1230, 835);
		leftarrow.LoadBitmap({"RES/arrow_left_enable.bmp", "RES/arrow_left_disable.bmp" }, RGB(255, 255, 255));
		leftarrow.SetTopLeft(1150, 835);
	}

	void SpyMenu::onShow() {
		spyMenu.ShowBitmap();
		for (int i = spyMenuPage * 3; i < min(spyMenuPage * 3 + 3, (int)spyMenuMonsterData.size()); i++) {
			spyMenuSquare[i % 3].ShowBitmap();
			spyMenuIcon[i % 3][spyMenuMonsterData[i].getID()].ShowBitmap();
		}
		if (spyMenuPage < (((int)spyMenuMonsterData.size() - 1) / 3)) {
			rightarrow.ShowBitmap();
		}
		if (spyMenuPage != 0) {
			leftarrow.ShowBitmap();
		}
		
	}

	void SpyMenu::setPage(int page) {
		
		page = min((int)spyMenuMonsterData.size() / 3, page);
		spyMenuPage = page;
	}

	void SpyMenu::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
		if (nChar == VK_LEFT) {
			spyMenuPage -= 1;
			spyMenuPage = max(0, spyMenuPage);
		}
		if (nChar == VK_RIGHT) {
			spyMenuPage += 1;
			spyMenuPage = min((((int)spyMenuMonsterData.size() - 1) / 3), spyMenuPage);
		}
	}

	void SpyMenu::setMonsterData(vector<Monster> _monsterData, Character character) {
		vector<int> visited = vector<int>(200);
		vector<Monster> fixed;
		for (int i = 0; i < (int) _monsterData.size(); i++) {
			if (visited[_monsterData[i].getID()]) continue;
			visited[_monsterData[i].getID()] = true;
			if (_monsterData[i].getDefence() == -1) {
				_monsterData[i].setDefence(character.getDefence());
			}
			if (_monsterData[i].getAttack() == -1) {
				_monsterData[i].setAttack(character.getAttack());
			}
			fixed.push_back(_monsterData[i]);
		}
		TRACE("%d\n", fixed.size());
		spyMenuMonsterData = fixed;
	}

	void SpyMenu::LoadMonsterBitmap(int ID, vector<char*> vec) {
		spyMenuIcon[0][ID].LoadBitmap(vec);
		spyMenuIcon[1][ID].LoadBitmap(vec);
		spyMenuIcon[2][ID].LoadBitmap(vec);
		spyMenuIcon[0][ID].SetAnimation(200, false);
		spyMenuIcon[1][ID].SetAnimation(200, false);
		spyMenuIcon[2][ID].SetAnimation(200, false);
		spyMenuIcon[0][ID].SetTopLeft(519, 192);
		spyMenuIcon[1][ID].SetTopLeft(519, 440);
		spyMenuIcon[2][ID].SetTopLeft(519, 690);
	}

	void SpyMenu::onShowText(CDC *pDC, CFont* &fp) {

		int	offset_x = CDDraw::IsFullScreen() ? 0 : (1920 - SIZE_X) / 2;

		for (int i = 0; i < min(3, (int)(spyMenuMonsterData.size() - spyMenuPage * 3)); i++) {

			ChangeFontLog(pDC, fp, 48, "STZhongsong");
			pDC->SetTextColor(RGB(255, 255, 255));
			CTextDraw::print(pDC, 510, 115 + i * 250, "名稱");
			CTextDraw::print(pDC, 910, 115 + i * 250, "特殊技能");

			ChangeFontLog(pDC, fp, 28, "STZhongsong");
			CTextDraw::print(pDC, 620, 185 + i * 250, "體力");
			CTextDraw::print(pDC, 770, 185 + i * 250, "攻擊力");
			CTextDraw::print(pDC, 970, 185 + i * 250, "防禦力");
			CTextDraw::print(pDC, 1170, 185 + i * 250, "敏捷");

			CTextDraw::print(pDC, 620, 265 + i * 250, "攻擊次數");
			CTextDraw::print(pDC, 820, 265 + i * 250, "Exp");
			CTextDraw::print(pDC, 1020, 265 + i * 250, "Gold");

			ChangeFontLog(pDC, fp, 36, "STZhongsong");
			CTextDraw::print(pDC, 615, 125 + i * 250, spyMenuMonsterData[spyMenuPage * 3 + i].getName().c_str());
			pDC->SetTextColor(RGB(255, 165, 95));
			CTextDraw::print(pDC, 1115, 125 + i * 250, spyMenuMonsterData[spyMenuPage * 3 + i].getSpecialEventName().c_str());

			ChangeFontLog(pDC, fp, 36, "Noto Sans", 800);

			pDC->SetTextColor(RGB(0, 255, 0));
			CTextDraw::print(pDC, 620, 215 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getHealth()).c_str());

			pDC->SetTextColor(RGB(255, 0, 0));
			CTextDraw::print(pDC, 770, 215 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getAttack()).c_str());

			pDC->SetTextColor(RGB(255, 255, 160));
			CTextDraw::print(pDC, 970, 215 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getDefence()).c_str());

			pDC->SetTextColor(RGB(160, 160, 255));
			CTextDraw::print(pDC, 1170, 215 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getSpeed()).c_str());

			pDC->SetTextColor(RGB(255, 100, 100));
			CTextDraw::print(pDC, 620, 295 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getAttackCount()).c_str());

			pDC->SetTextColor(RGB(0, 255, 255));
			CTextDraw::print(pDC, 820, 295 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getExp()).c_str());

			pDC->SetTextColor(RGB(255, 215, 0));
			CTextDraw::print(pDC, 1020, 295 + i * 250, to_string(spyMenuMonsterData[spyMenuPage * 3 + i].getGold()).c_str());
		}

		ChangeFontLog(pDC, fp, 30, "Noto Sans TC", 800);

		vector<int> vec = getShineVector();
		int count = vec.size();
		pDC->SetTextColor(RGB(vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count], vec[CSpecialEffect::GetCurrentTimeCount() % count]));

		ChangeFontLog(pDC, fp, 30, "STZhongsong");
		CTextDraw::print(pDC, 1150, 880, "- Quit(D) -");
	}
}