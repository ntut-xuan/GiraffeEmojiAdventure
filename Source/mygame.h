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
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap giraffe_photo;
		CMovingBitmap logo;								// csie��logo
	};

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		static const int start_x;
		static const int start_y;
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@

		static Character character;
		static vector<vector<vector<int>>> stage_entity;
		static vector<vector<vector<int>>> stage_material;
		static vector<vector<vector<int>>> hidden_code;
		static bool refresh_animation;
		static char ending;
		static int current_stage;
		static bool teleport_allow;

	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		bool isDoor(int doorCode);
		bool isKey(int keyCode);
		bool OpenDoor(int preCurrentStage, int x, int y, int doorCode);
		bool GetKey(int x, int y, int keyCode);
		void loadFixedEntityMap(int ID, vector<string> vec, int animation_time, bool animation_option);
		void loadFixedMaterialMap(int ID, vector<string> vec, int animation_time, bool animation_option);
	private:
		const int		STAGES;	// ���d���`��
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
		bool turn = true; // ���a����
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
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnInit();
		static char ending;
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void ShowText(); 
	private:
		int current_dialog = 0;
		vector<vector<string>> dialog;
		CMovingBitmap dialogMenu_B, dialogMenuCharacter_B;
		bool enterStatus = true;
		bool dialogMenuing = true;
	};

}