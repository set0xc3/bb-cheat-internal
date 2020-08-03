
#include "includes.h"
#include <FindPattern.h>
#include <Memory.h>
#include <Vectors.h>

enum KeyPattern
{
	Pattern_Entity,
	Pattern_PLH,
	Pattern_VWIK,
	Pattern_GUIInv,
	Pattern_Controll,
	Pattern_MainManager
};

enum KeyBone
{
	Bone_Head,
	Bone_Body,

	Bone_LArm1,
	Bone_LArm2,

	Bone_RArm1,
	Bone_RArm2,

	Bone_LLeg1,
	Bone_LLeg2,
	Bone_LLeg3,

	Bone_RLeg1,
	Bone_RLeg2,
	Bone_RLeg3
};

class CheatEngine
{
public:

	struct GetModule
	{
		DWORD dwGameAssembly;
		DWORD dwUnityPlayer;
	};
	GetModule GetModule;

	struct GetOffset
	{
		struct Engine
		{
			std::vector<unsigned int> Matrix = { 0x0, 0x8, 0xBC };
		};
		Engine Engine;

		struct World
		{
			std::vector<unsigned int> GameMode = { 0x5C, 0x218 };
		};
		World World;

		struct Local
		{
			//std::vector<unsigned int> Coords = { 0x5C, 0xFC };

			std::vector<unsigned int> Angle = { 0x5C, 0x3C };
			std::vector<unsigned int> Coords = { 0x0, 0x8, 0x3B4 }; // Camera
			std::vector<unsigned int> Death = { 0x5C, 0x13C, 0x148 }; // Death: (int)5 | Мертв
			std::vector<unsigned int> Team = { 0x5C, 0x13C, 0x14 };  // Death: (int)2 | Спектр
			std::vector<unsigned int> TeamZombie = { 0x5C, 0x13C, 0x2C };
			std::vector<unsigned int> Menu = { 0x5C, 0x13C, 0x10 }; // isMenu
		};
		Local Local;

		struct Entity
		{
			DWORD Dist = 0x10;

			std::vector<unsigned int> Base			= { 0x5C, 0xC, 0x0, 0x0 };
			std::vector<unsigned int> Team			= { 0x5C, 0xC, 0x0, 0x14 }; // Death: (int)2 | Спектр
			std::vector<unsigned int> TeamZombie		= { 0x5C, 0xC, 0x0, 0x2C };
			std::vector<unsigned int> Name			= { 0x5C, 0xC, 0x0, 0x18, 0xC };
			std::vector<unsigned int> NameLength		= { 0x5C, 0xC, 0x0, 0x18, 0x8 };
			std::vector<unsigned int> Coords			= { 0x5C, 0xC, 0x0, 0x44 };
			std::vector<unsigned int> IsEntity		= { 0x5C, 0xC, 0x0, 0x1F8 }; // IsEntity: (DWORD) != NULL | Не пусто - значит это локальный игрок
			std::vector<unsigned int> Health			= { 0x5C, 0xC, 0x0, 0x28 };
			std::vector<unsigned int> Death			= { 0x5C, 0xC, 0x0, 0x148 }; // Death: (int)5 | Мертв
			std::vector<unsigned int> Protection		= { 0x5C, 0xC, 0x0, 0x148 }; // Protection: (int)1 | Защита при старте
													
			std::vector<unsigned int> Head = { 0x104, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> Body = { 0x100, 0x8, 0x34, 0x60 };

			std::vector<unsigned int> LArm1 = { 0x108, 0x10, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> LArm2 = { 0x108, 0x14, 0x8, 0x34, 0x60 };

			std::vector<unsigned int> RArm1 = { 0x10C, 0x10, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> RArm2 = { 0x10C, 0x14, 0x8, 0x34, 0x60 };

			std::vector<unsigned int> LLeg1 = { 0x110, 0x10, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> LLeg2 = { 0x110, 0x14, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> LLeg3 = { 0x110, 0x18, 0x8, 0x34, 0x60 };

			std::vector<unsigned int> RLeg1 = { 0x114, 0x10, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> RLeg2 = { 0x114, 0x14, 0x8, 0x34, 0x60 };
			std::vector<unsigned int> RLeg3 = { 0x114, 0x18, 0x8, 0x34, 0x60 };
		};
		Entity Entity;
	};
	GetOffset GetOffset;

	struct GetPlayer
	{
		struct Player
		{
			ImVec2 SCoords;
		};
		Player Player;

		struct Entity
		{
			std::vector<ImVec2> BoneSCoords;
		};
		Entity Entity;
	};
	GetPlayer GetPlayer;

	class Pattern
	{
	public:
		bool isFind;
		DWORD address;
		int id;
		const char* module;
		const char* pattern;
		const char* mask;
	};

	std::vector<Pattern*> pattern;
	std::vector<Vector3> bones;

	void Shutdown();
	void Initialize();
	void Find(int id, char* module, char* pattern, char* mask, DWORD offset, bool ptr);
	DWORD GetAddress(int id);

	//Offsets GetModule();

	// World
	int GetGameMode();

	// Matrix
	float* GetMatrix();
	DWORD GetBaseMatrix();

	// Local
	DWORD GetLDeath();
	int GetLTeam();
	int GetLTeamZombie();
	int GetIsMenu();
	Vector3 GetDir();
	Vector3 GetAngle();
	Vector3 GetCoords();
	ImVec2& GetSCoords();

};
static CheatEngine* cheatEngine = new CheatEngine();


