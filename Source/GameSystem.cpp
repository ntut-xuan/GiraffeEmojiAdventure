#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameCharacter.h"
#include "GameMenu.h"
#include "GameSystem.h"
#include <string>
#include <vector>

using namespace std;

namespace game_framework {

	void GameSystem::setParameter(Character _character, int _mouse_x, int _mouse_y, string _floor_message) {
		character = _character;
		mouse_x = _mouse_x;
		mouse_y = _mouse_y;
		floor_message = _floor_message;
	}

	void GameSystem::LoadBitmap(vector<char*> vec) {
		menu.LoadBitmapA(vec, RGB(0, 0, 0));
		menu.SetTopLeft(0, 0);
	}

	void GameSystem::onShow() {
		menu.ShowBitmap();
	}

	void GameSystem::onShowText(CDC *pDC, CFont* &fp) {

		ChangeFontLog(pDC, fp, 48, "Noto Sans TC");

		pDC->SetTextColor(RGB(255, 255, 255));

		if (character.getStatusString().length() > 5) {
			ChangeFontLog(pDC, fp, 36, "Noto Sans TC");
			pDC->TextOut(220, 142, character.getStatusString().c_str());
		}
		else {
			pDC->TextOut(249, 140, character.getStatusString().c_str());
		}


		ChangeFontLog(pDC, fp, 48, "Noto Sans TC");
		pDC->TextOut(256 + 250 - 227, 204, to_string(character.getLevel()).c_str());
		pDC->TextOut(256 + 250 - 227, 256, to_string(character.getHealth()).c_str());
		pDC->TextOut(256 + 250 - 227, 308, to_string(character.getAttack()).c_str());
		pDC->TextOut(256 + 250 - 227, 360, to_string(character.getDefence()).c_str());
		pDC->TextOut(256 + 250 - 227, 412, to_string(character.getSpeed()).c_str());
		pDC->TextOut(256 + 250 - 227, 460, to_string(character.getExp()).c_str());
		pDC->TextOut(256 + 790 - 227, 14, floor_message.c_str());

		ChangeFontLog(pDC, fp, 60, "Noto Sans TC", 800);

		pDC->TextOut(256 + 290 - 227, 615, to_string(character.getKeyNumber()).c_str());
		pDC->TextOut(256 + 290 - 227, 688, to_string(character.getSilverKeyNumber()).c_str());
		pDC->TextOut(256 + 290 - 227, 762, to_string(character.getGoldKeyNumber()).c_str());
		pDC->TextOut(256 + 290 - 227, 844, to_string(character.getCoin()).c_str());

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(0, 0, (to_string(mouse_x) + " " + to_string(mouse_y)).c_str());
		pDC->TextOut(1120, 935, " - Press L - ");
	}
}