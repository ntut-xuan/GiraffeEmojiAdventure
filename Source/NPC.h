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
		void loadData(int stage_id, int entity_x, int entity_y);
		int getVariable(int index);
		void setVariable(int index, int value);
		vector<vector<string>> getDialog();
		vector<string> getOption();
		vector<string> getSubOption();
		vector<char> getPosition();
		static int increaseCost1;
		static int increaseCost2;
	protected:
		int ID;
		string name;
		vector<int> variable = vector<int>(100);
		vector<vector<string>> dialog;
		vector<string> option;
		vector<string> suboption;
		vector<char> positions;
		int focusOption = 0;
	};
}