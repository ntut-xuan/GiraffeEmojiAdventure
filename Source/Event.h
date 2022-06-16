#pragma once
namespace game_framework {
	class Event {
	public:
		Event();
		static void triggerDialogEvent(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing);
		static void triggerAfterDialog(int current_stage, int x, int y, Character &character);
		static void triggerGetItem(int ID, Character &character, string &temp_item_info);
		static void triggerAfterAttack(int current_stage, int x, int y, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing);
		static bool triggerSpecialNPCDialog(int current_stage, int x, int y, NPC npc, Character &character, DialogMenu &dialogMenu, bool &enterStatus, bool &dialogMenuing);
		static bool triggerTeleport(int &current_stage, int x, int y, Character &character);
		static bool triggerTeleportAfterShowBitmap(int &current_stage, int x, int y, Character &character);
		static bool triggerTeleportIsExist(int current_stage, int x, int y);
		static bool triggerAnydoorRule(Character character);
		static bool triggerPOGSRule(Character character, map<int,string> monster_map);
		static bool triggerShopping(int current_stage, int x, int y, Character &character, int tempSelect, bool &inShopping, bool &enterStatus, bool &dialogMenuing);
		static vector<vector<vector<int>>> visited;	
	};
}