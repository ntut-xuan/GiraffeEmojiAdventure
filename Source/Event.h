#pragma once
namespace game_framework {
	class Event {
	public:
		static void triggerDialogEvent(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing);
		static void triggerAfterDialog(int current_stage, int x, int y);
		static void triggerGetItem(int ID, Character &character, string &temp_item_info);
		static void triggerAfterAttack(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing);
		static bool triggerSpecialNPCDialog(int current_stage, int x, int y, NPC npc, Character &character, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing);
		static bool triggerTeleport(int &current_stage, int x, int y, Character &character);
	private:
		static vector<vector<vector<int>>> visited;
	};
}