#pragma once
namespace game_framework {
	class DialogMenu : public GameMenu {
	public:
		enum POSITION { TOP, CENTER, DOWN };
		static POSITION topdown;
		DialogMenu() = default;
		void onShow();
		void LoadBitmap();
		bool onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags, Character &character, bool &inShopping, bool &enterStatus, bool &dialogMenuing);
		void setNPC(NPC npc, int current_stage, int x, int y, bool &inShopping);
		void setNPCMap(vector<CMovingBitmap> vec);
		void onShowText(CDC *pDC, CFont* &fp, bool inShopping);
		int tempSelect = 0;
		const int OPTION_GAP = 150;
		int menuOptionGap = 120;
		int current_dialog = 0;
		void setNPCTemporaryDialog(vector<string> dialog, bool &enterStatus, bool &dialogMenuing, NPC _npc);
		void setNPCTemporaryDialog(vector<vector<string>>, vector<char> _position, bool &enterStatus, bool &dialogMenuing, NPC _npc);
		void setTemporaryDialog(POSITION position, vector<string> dialog, bool &enterStatus, bool &dialogMenuing);
	private:

		int current_stage;
		int x;
		int y;

		vector<CMovingBitmap> npc_map;

		vector<vector<string>> dialog;
		vector<char> position;
		vector<string> option;
		vector<string> suboption;

		CMovingBitmap dialogMenu_T;
		CMovingBitmap dialogMenu_B;
		CMovingBitmap dialogMenu_C;
		CMovingBitmap dialogMenuCharacter_B;
		CMovingBitmap optionMenu;
		CMovingBitmap optionArrow;
		NPC npc;
	};
}