#include "stdafx.h"
#include "NPC.h"
#include "Material.h"
#include <cmath>
#include <fstream>

namespace game_framework {

	int NPC::increaseCost1 = 20;
	int NPC::increaseCost2 = 50;

	NPC::NPC() {

	}

	NPC::NPC(int _ID, string _name) {
		ID = _ID;
		name = _name;
	}

	int NPC::getVariable(int index) {
		return variable[index];
	}

	void NPC::loadData(int stage_id, int entity_x, int entity_y) {
		dialog.clear();
		option.clear();
		string fileName = "DialogData/Dialog_" + to_string(stage_id) + "_" + to_string(entity_x) + "_" + to_string(entity_y) + ".txt";
		ifstream in(fileName);
		string s;

		TRACE("%d\n", in.good());

		if (!in.good()) {
			dialog.push_back({ "Dialog_" + to_string(stage_id) + "_" + to_string(entity_x) + "_" + to_string(entity_y) + " 對話檔不存在.jpg" });
			positions.push_back('T');
			return;
		}

		int dialogCount;
		in >> dialogCount;
		for (int i = 0; i < dialogCount; i++) {
			int dialogLineCount;
			in >> dialogLineCount;
			char position;
			in >> position;
			vector<string> temp_dialog;
			getline(in, s);
			for (int j = 0; j < dialogLineCount; j++) {
				getline(in, s);
				temp_dialog.push_back(s);
			}
			dialog.push_back(temp_dialog);
			positions.push_back(position);
		}

		int selectCount;
		in >> selectCount;
		getline(in, s);
		for (int i = 0; i < selectCount; i++) {
			getline(in, s);
			option.push_back(s);
		}
		int subOptionCount;
		in >> subOptionCount;
		getline(in, s);
		for (int i = 0; i < subOptionCount; i++) {
			getline(in, s);
			suboption.push_back(s);
		}
	}

	vector<vector<string>> NPC::getDialog() {
		vector<vector<string>> replacementDialog = dialog;
		for (int i = 0; i < (int) replacementDialog.size(); i++){
			for (int j = 0; j < (int) replacementDialog[i].size(); j++) {
				if (replacementDialog[i][j].find("{IC1}") != -1) {
					replacementDialog[i][j] = replacementDialog[i][j].replace(replacementDialog[i][j].find("{IC1}"), 5, to_string(NPC::increaseCost1));
				}
				if (replacementDialog[i][j].find("{IC2}") != -1) {
					replacementDialog[i][j] = replacementDialog[i][j].replace(replacementDialog[i][j].find("{IC2}"), 5, to_string(NPC::increaseCost2));
				}
			}
		}
		return replacementDialog;
	}

	vector<string> NPC::getOption() {
		return option;
	}

	vector<string> NPC::getSubOption() {
		return suboption;
	}

	vector<char> NPC::getPosition() {
		return positions;
	}
	
	void NPC::setVariable(int index, int value) {
		variable[index] = value;
	}

	string NPC::getName() {
		return name;
	}

	int NPC::getID() {
		return ID;
	}
}