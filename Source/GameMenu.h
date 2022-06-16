#pragma once
#include <string>
#include <vector>
using namespace std;

namespace game_framework {
	class GameMenu {
	public:
		void ChangeFontLog(CDC* pDC, CFont* &fp, int size, string fontName, int weight = 500) {
			LOGFONT lf;
			CFont f;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = size;
			lf.lfWeight = weight;
			strcpy(lf.lfFaceName, fontName.c_str());
			f.CreateFontIndirectA(&lf);
			fp = pDC->SelectObject(&f);
		};
		vector<int> getShineVector() {
			vector<int> vec;
			for (int i = 80; i <= 250; i += 10) {
				vec.push_back(i);
				vec.push_back(i);
			}
			for (int i = 250; i >= 80; i -= 10) {
				vec.push_back(i);
				vec.push_back(i);
			}
			return vec;
		};
	};
};