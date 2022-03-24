#pragma once
#include <string>
#include <vector>
using namespace std;

namespace game_framework {
	class NPC {
	public:
		NPC();
		NPC(int ID, string name);
		NPC(int ID, string name, vector<string> dialog);
		string getName();
		int getID();
		void loadData(int entity_x, int entity_y);
		void setVariable(int index, int value);
		vector<string> getDialog();
		vector<string> getOption();
	protected:
		int ID;
		string name;
		vector<int> variable = vector<int>(100);
		vector<string> dialog;
		vector<string> option;
		int focusOption = 0;
	};
}