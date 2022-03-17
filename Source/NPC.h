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
		vector<string> getDialog();
		vector<string> getSelect();
	protected:
		int ID;
		string name;
		vector<string> dialog;
		vector<string> option;
		int focusOption = 0;
	};
}