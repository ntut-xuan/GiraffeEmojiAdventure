#include "GameCharacter.h"
#include "Monster.h"
#include "NPC.h"
#include "Stage.h"
#include "GameMenu.h"
#include "Spymenu.h"
#include "FlyingMenu.h"
#include "GameSystem.h"
#include "DialogMenu.h"
#include "AnyGateMenu.h"
#include "POGSMenu.h"
#include "Event.h"

namespace game_framework {

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap giraffe_photo;
		CMovingBitmap logo;								// csie的logo
	};

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		static const int start_x;
		static const int start_y;
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作

		static Character character;
		static vector<vector<vector<int>>> stage_entity;
		static vector<vector<vector<int>>> stage_material;
		static vector<vector<vector<int>>> hidden_code;
		static bool refresh_animation;
		static char ending;
		static int current_stage;
		static bool teleport_allow;

	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
		bool isDoor(int doorCode);
		bool isKey(int keyCode);
		bool OpenDoor(int preCurrentStage, int x, int y, int doorCode);
		bool GetKey(int x, int y, int keyCode);
		void loadFixedEntityMap(int ID, vector<string> vec, int animation_time, bool animation_option);
		void loadFixedMaterialMap(int ID, vector<string> vec, int animation_time, bool animation_option);
	private:
		const int		STAGES;	// 關卡的總數
		const int MENU_DELAY = 500;
		int mouse_x, mouse_y;
		int temp_current_stage = 0;
		int temp_monster_x, temp_monster_y;
		int current_shop_price = 20;
		int current_dialog = 0;
		long long tempDelayCycle = MENU_DELAY;
		int monsterAttackCount = 0;
		int attack_menu_animation_select = -1;
		std::string causeDamageValueString;
		bool getDialogMenuing = false;
		bool dialogMenuing = false;
		bool inShopping = false;
		bool attackMenuing = false;
		bool helpMenuing = false;
		bool flyingMenuing = false;
		bool spyMenuing = false;
		bool anyGateMenuing = false;
		bool pogsMenuing = false;
		bool showAttackValue = true;
		bool enterStatus = false;
		bool turn = true; // 玩家先手
		bool onMoveDone = false;
		bool onShowDone = true;
		int last_time;
		int show_value_time;
		std::string floor_message;
		std::string temp_item_info;

		Stage stage;
		Event event;
		CMovingBitmap characterBitmap;

		/* Attack Menu Object */
		CMovingBitmap attackMenu;
		CMovingBitmap characterAttackMenuBitMap;
		CMovingBitmap enemyAttackMenuBitmap;

		/* Winning Menu Object */
		CMovingBitmap winningMenu;
		CMovingBitmap hidden;

		/* Get Dialog Menu Object */
		CMovingBitmap getDialogMenu;

		/* Help Menu */
		CMovingBitmap helpMenu;

		/* Attack Animation Vector (enemy and character) */
		vector<CMovingBitmap> enemyAttackAnimation = vector<CMovingBitmap>(3);
		vector<CMovingBitmap> characterAttackAnimation = vector<CMovingBitmap>(3);

		Monster monster;
		NPC npc;
		map<int, int> bitmap_frame;
		map<int, Monster> monster_value;
		map<int, string> normal_bitmap_map;
		map<int, string> functional_entity_bitmap_map;
		map<int, string> monster_bitmap_map;
		map<int, string> npc_bitmap_map;
		map<int, string> block_bitmap_map;
		vector<CMovingBitmap> monster_map = vector<CMovingBitmap>(200);
		vector<CMovingBitmap> npc_map = vector<CMovingBitmap>(200);
		vector<vector<vector<CMovingBitmap>>> fixed_entity_map = vector<vector<vector<CMovingBitmap>>>(11, vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(200)));
		vector<vector<vector<CMovingBitmap>>> fixed_animation_map = vector<vector<vector<CMovingBitmap>>>(11, vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(5)));
		vector<vector<vector<CMovingBitmap>>> fixed_material_map = vector<vector<vector<CMovingBitmap>>>(11, vector<vector<CMovingBitmap>>(11, vector<CMovingBitmap>(200)));
		GameSystem gameSystem;
		SpyMenu spyMenu;
		FlyingMenu flyingMenu;
		DialogMenu dialogMenu;
		AnyGateMenu anyGateMenu;
		POGSMenu pogsMenu;

		void ShowText();
	};

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnInit();
		static char ending;
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void ShowText(); 
	private:
		int current_dialog = 0;
		vector<vector<string>> dialog;
		CMovingBitmap dialogMenu_B, dialogMenuCharacter_B;
		bool enterStatus = true;
		bool dialogMenuing = true;
	};

}