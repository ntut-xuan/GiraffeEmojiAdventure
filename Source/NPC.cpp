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

	NPC::NPC(int _ID, string _name, vector<string> _dialog) {
		ID = _ID;
		name = _name;
		dialog = std::vector<string>(_dialog.begin(), _dialog.end());
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
		int dialogCount;
		in >> dialogCount;
		getline(in, s);
		for (int i = 0; i < dialogCount; i++) {
			getline(in, s);
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
		vector<string> replacementDialog = dialog;
		for (int i = 0; i < (int) dialog.size(); i++) {
			if (replacementDialog[i].find("{IC1}") != -1) {
				replacementDialog[i] = replacementDialog[i].replace(replacementDialog[i].find("{IC1}"), 5, to_string(NPC::increaseCost1));
			}
			if (replacementDialog[i].find("{IC2}") != -1) {
				replacementDialog[i] = replacementDialog[i].replace(replacementDialog[i].find("{IC2}"), 5, to_string(NPC::increaseCost2));
			}
		}
		return replacementDialog;
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