#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "Stage.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace game_framework {

	Stage::Stage() {}

	vector<vector<int>> Stage::getStageMaterial(int stage) {
		string fileName = "StageData/Stage" + to_string(stage) + "_Material.txt";
		ifstream in(fileName);
		vector<vector<int>> temp(11, vector<int>(11));
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				int a;
				in >> a;
				std::cout << a << std::endl;
				temp[i][j] = a;
			}
		}
		map = temp;
		return map;
	}

	vector<vector<int>> Stage::getStageEntity(int stage) {
		string fileName = "StageData/Stage" + to_string(stage) + "_Entity.txt";
		ifstream in(fileName);
		vector<vector<int>> temp(11, vector<int>(11));
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				int a;
				in >> a;
				std::cout << a << std::endl;
				temp[i][j] = a;
			}
		}
		map = temp;
		return map;
	}
}