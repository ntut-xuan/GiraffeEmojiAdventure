#include "stdafx.h"
#include "NPC.h"
#include "Material.h"
#include <cmath>
#include <fstream>

namespace game_framework {

	NPC::NPC() {

	}

	NPC::NPC(int _ID, string _name, vector<string> _dialog) {
		ID = _ID;
		name = _name;
		dialog = std::vector<string>(_dialog.begin(), _dialog.end());
	}

	NPC::NPC(int _ID, string _name) {
		ID = _ID;
		name = _name;
	}

	void NPC::loadData(int stage_id, int entity_x, int entity_y) {
		string fileName = "DialogData/Dialog_" + to_string(stage_id) + "_" + to_string(entity_x) + "_" + to_string(entity_y) + ".txt";
		ifstream in(fileName);
		string s;
		int dialogCount;
		in >> dialogCount;
		getline(in, s);
		for (int i = 0; i < dialogCount; i++) {
			getline(in, s);
			for (int j = 0; j < (int) variable.size(); j++) {
				int index = s.find("{" + to_string(j) + "}");
				if (index == -1) continue;
				s.replace(s.find("{" + to_string(j) + "}"), 3, to_string(variable[j]));
			}
			dialog.push_back(s);
		}
		int selectCount;
		in >> selectCount;
		getline(in, s);
		for (int i = 0; i < selectCount; i++) {
			getline(in, s);
			option.push_back(s);
		}
	}

	vector<string> NPC::getDialog() {
		return dialog;
	}

	vector<string> NPC::getOption() {
		return option;
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