// Created with ReClass.NET 1.2 by KN4CK3R

#include <includes.h>
#include <Vectors.h>

template<typename T>
bool ValidatePointer(T lpAddress)
{
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T size = VirtualQuery(reinterpret_cast<LPVOID>(lpAddress), &mbi, sizeof(MEMORY_BASIC_INFORMATION));

	if (size == 0)
		return false;

	if (mbi.Protect & PAGE_NOACCESS)
		return false;

	if (mbi.Protect & PAGE_GUARD)
		return false;

	if (lpAddress == NULL)
		return false;

	return true;
}

std::string narrow(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	return convert.to_bytes(wstr);
}

int ix;
DWORD EntityObjStart = 0x0;
DWORD EntlistJmpBack = 0x0;
bool alreadyThere = false;

class PlayerData
{
public:
	char pad_0000[16]; //0x0000
	int32_t id; //0x0010
	int32_t team; //0x0014
	class string18* string18ptr; //0x0018
	char pad_001C[12]; //0x001C
	int32_t health; //0x0028
	int32_t teamZombie; //0x002C
	char pad_0030[20]; //0x0030
	Vector3 pos; //0x0044
	char pad_0050[176]; //0x0050
	class RigidbodyBody* Body; //0x0100
	class RigidbodyHead* Head; //0x0104
	class RigidbodyLArm* LArm; //0x0108
	class RigidbodyRArm* RArm; //0x010C
	class RigidbodyLLeg* LLeg; //0x0110
	class RigidbodyRLeg* RLeg; //0x0114
	char pad_0118[48]; //0x0118
	int32_t death; //0x0148
	char pad_014C[172]; //0x014C
	uint32_t localPlayer; //0x01F8
	char pad_01FC[612]; //0x01FC
}; //Size: 0x0460
PlayerData* entsptr;

namespace nEntity
{
	DWORD offset = 0x10;

	ImVec2 SCoords;
	std::vector<Vector3> bones;
	std::vector<ImVec2> SCoordsBone;

	DWORD& GetOffset();
	DWORD GetIsEntity();

	Vector3 GetCoords();
	std::vector<Vector3> GetCoordsBones();

	ImVec2& GetSCoords();
	std::vector<ImVec2>& GetSCoordsBones();

	bool GetEntity();

	int GetTeam();
	int GetTeamZombie();
	int GetDeath();
	int GetHealth();

	const std::string GetName();
};

class string18
{
public:
	char pad_0000[12]; //0x0000
	WCHAR* name[32]; //0x000C
	char pad_0010[82]; //0x0010
}; //Size: 0x0062

class RigidbodyBody
{
public:
	char pad_0000[8]; //0x0000
	class N00000D30* N00000D1F; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000D30
{
public:
	char pad_0000[52]; //0x0000
	class N00000D45* N00000D3E; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000D45
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class RigidbodyHead
{
public:
	char pad_0000[8]; //0x0000
	class N00000D9A* N00000D89; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000D9A
{
public:
	char pad_0000[52]; //0x0000
	class N00000DAE* N00000DA8; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000DAE
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class RigidbodyLArm
{
public:
	char pad_0000[16]; //0x0000
	class N00000DE7* N00000DD8; //0x0010
	class N00000DFB* N00000DD9; //0x0014
	char pad_0018[44]; //0x0018
}; //Size: 0x0044

class N00000DE7
{
public:
	char pad_0000[8]; //0x0000
	class N00000E0F* N00000DEA; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000DFB
{
public:
	char pad_0000[8]; //0x0000
	class N00000E48* N00000DFE; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000E0F
{
public:
	char pad_0000[52]; //0x0000
	class N00000E23* N00000E1D; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000E23
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N00000E48
{
public:
	char pad_0000[52]; //0x0000
	class N00000E5C* N00000E56; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000E5C
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class RigidbodyRArm
{
public:
	char pad_0000[16]; //0x0000
	class N00000ED9* N00000ECA; //0x0010
	class N00000EED* N00000ECB; //0x0014
	char pad_0018[44]; //0x0018
}; //Size: 0x0044

class N00000ED9
{
public:
	char pad_0000[8]; //0x0000
	class N00000F01* N00000EDC; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000EED
{
public:
	char pad_0000[8]; //0x0000
	class N00000F3A* N00000EF0; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000F01
{
public:
	char pad_0000[52]; //0x0000
	class N00000F15* N00000F0F; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000F15
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N00000F3A
{
public:
	char pad_0000[52]; //0x0000
	class N00000F4E* N00000F48; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000F4E
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class RigidbodyLLeg
{
public:
	char pad_0000[16]; //0x0000
	class N00000F9B* N00000F78; //0x0010
	class N00000FAF* N00000F79; //0x0014
	class N00000FC3* N00000F7A; //0x0018
	char pad_001C[40]; //0x001C
}; //Size: 0x0044

class RigidbodyRLeg
{
public:
	char pad_0000[16]; //0x0000
	class N00001082* N00000F8C; //0x0010
	class N00001096* N00000F8D; //0x0014
	class N000010AA* N00000F8E; //0x0018
	char pad_001C[40]; //0x001C
}; //Size: 0x0044

class N00000F9B
{
public:
	char pad_0000[8]; //0x0000
	class N00000FD7* N00000F9E; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000FAF
{
public:
	char pad_0000[8]; //0x0000
	class N00001010* N00000FB2; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000FC3
{
public:
	char pad_0000[8]; //0x0000
	class N00001049* N00000FC6; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00000FD7
{
public:
	char pad_0000[52]; //0x0000
	class N00000FEB* N00000FE5; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00000FEB
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N00001010
{
public:
	char pad_0000[52]; //0x0000
	class N00001024* N0000101E; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00001024
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N00001049
{
public:
	char pad_0000[52]; //0x0000
	class N0000105D* N00001057; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N0000105D
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N00001082
{
public:
	char pad_0000[8]; //0x0000
	class N000010BE* N00001085; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N00001096
{
public:
	char pad_0000[8]; //0x0000
	class N000010F8* N00001099; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N000010AA
{
public:
	char pad_0000[8]; //0x0000
	class N00001131* N000010AD; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044

class N000010BE
{
public:
	char pad_0000[52]; //0x0000
	class N000010D2* N000010CC; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N000010D2
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N000010F8
{
public:
	char pad_0000[52]; //0x0000
	class N0000110C* N00001106; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N0000110C
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C

class N00001131
{
public:
	char pad_0000[52]; //0x0000
	class N00001145* N0000113F; //0x0034
	char pad_0038[12]; //0x0038
}; //Size: 0x0044

class N00001145
{
public:
	char pad_0000[96]; //0x0000
	Vector3 Coords; //0x0060
	char pad_006C[32]; //0x006C
}; //Size: 0x008C



bool nEntity::GetEntity()
{
	if (entsptr == NULL)
	{
		return false;
	}
	else return true;
}

DWORD& nEntity::GetOffset()
{
	return nEntity::offset;
}

DWORD nEntity::GetIsEntity()
{
	return entsptr->localPlayer;
}

int nEntity::GetTeam()
{
	return entsptr->team;
}

int nEntity::GetTeamZombie()
{
	return entsptr->teamZombie;
}

Vector3 nEntity::GetCoords()
{
	return entsptr->pos;
}

int nEntity::GetDeath()
{
	return entsptr->death;
}

int nEntity::GetHealth()
{

	return entsptr->health;
}

const std::string nEntity::GetName()
{
	if (entsptr != NULL)
	{
		if (entsptr->string18ptr != NULL)
		{
			WCHAR* txt = (WCHAR*)entsptr->string18ptr->name;

			return (narrow(txt));
		}
	}
}


Vector3 GetBone(int bone, PlayerData* entsptr)
{
	if (entsptr != NULL)
	{
		if (bone == KeyBone::Head)
		{
			if (entsptr != NULL && entsptr->Head != NULL && entsptr->Head->N00000D89 != NULL && entsptr->Head->N00000D89->N00000DA8 != NULL)
			{
				return entsptr->Head->N00000D89->N00000DA8->Coords;
			}
			else entsptr = NULL;
		}

		if (bone == KeyBone::Body)
		{
			if (entsptr != NULL && entsptr->Body != NULL && entsptr->Body->N00000D1F != NULL && entsptr->Body->N00000D1F->N00000D3E != NULL)
			{
				return entsptr->Body->N00000D1F->N00000D3E->Coords;
			}
			else entsptr = NULL;
		}

		if (bone == KeyBone::LArm1)
		{
			if (entsptr != NULL && entsptr->LArm != NULL && entsptr->LArm->N00000DD8 != NULL && entsptr->LArm->N00000DD8->N00000DEA != NULL)
			{
				return entsptr->LArm->N00000DD8->N00000DEA->N00000E1D->Coords;
			}
		}

		if (bone == KeyBone::LArm2)
		{
			if (entsptr != NULL && entsptr->LArm != NULL && entsptr->LArm->N00000DD9 != NULL && entsptr->LArm->N00000DD9->N00000DFE != NULL)
			{
				return entsptr->LArm->N00000DD9->N00000DFE->N00000E56->Coords;
			}
		}

		if (bone == KeyBone::RArm1)
		{
			if (entsptr != NULL && entsptr->RArm != NULL && entsptr->RArm->N00000ECA != NULL && entsptr->RArm->N00000ECA->N00000EDC != NULL)
			{
				return entsptr->RArm->N00000ECA->N00000EDC->N00000F0F->Coords;
			}
		}

		if (bone == KeyBone::RArm2)
		{
			if (entsptr != NULL && entsptr->RArm != NULL && entsptr->RArm->N00000ECB != NULL && entsptr->RArm->N00000ECB->N00000EF0 != NULL)
			{
				return entsptr->RArm->N00000ECB->N00000EF0->N00000F48->Coords;
			}
		}

		if (bone == KeyBone::LLeg1)
		{
			if (entsptr != NULL && entsptr->LLeg != NULL && entsptr->LLeg->N00000F78 != NULL && entsptr->LLeg->N00000F78->N00000F9E != NULL)
			{
				return entsptr->LLeg->N00000F78->N00000F9E->N00000FE5->Coords;
			}
		}

		if (bone == KeyBone::LLeg2)
		{
			if (entsptr != NULL && entsptr->LLeg != NULL && entsptr->LLeg->N00000F79 != NULL && entsptr->LLeg->N00000F79->N00000FB2 != NULL)
			{
				return entsptr->LLeg->N00000F79->N00000FB2->N0000101E->Coords;
			}
		}

		if (bone == KeyBone::LLeg3)
		{
			if (entsptr != NULL && entsptr->LLeg != NULL && entsptr->LLeg->N00000F7A != NULL && entsptr->LLeg->N00000F7A->N00000FC6 != NULL)
			{
				return entsptr->LLeg->N00000F7A->N00000FC6->N00001057->Coords;
			}
		}

		if (bone == KeyBone::RLeg1)
		{
			if (entsptr != NULL && entsptr->RLeg != NULL && entsptr->RLeg->N00000F8C != NULL && entsptr->RLeg->N00000F8C->N00001085 != NULL)
			{
				return entsptr->RLeg->N00000F8C->N00001085->N000010CC->Coords;
			}
		}

		if (bone == KeyBone::RLeg2)
		{
			if (entsptr != NULL && entsptr->RLeg != NULL && entsptr->RLeg->N00000F8D != NULL && entsptr->RLeg->N00000F8D->N00001099 != NULL)
			{
				return entsptr->RLeg->N00000F8D->N00001099->N00001106->Coords;
			}
		}

		if (bone == KeyBone::RLeg3)
		{
			if (entsptr != NULL && entsptr->RLeg != NULL && entsptr->RLeg->N00000F8E != NULL && entsptr->RLeg->N00000F8E->N000010AD != NULL)
			{
				return entsptr->RLeg->N00000F8E->N000010AD->N0000113F->Coords;
			}
		}
	}
}

std::vector<Vector3> nEntity::GetCoordsBones()
{
	bones[KeyBone::Head] = GetBone(KeyBone::Head, entsptr);
	bones[KeyBone::Body] = GetBone(KeyBone::Body, entsptr);

	//bones[KeyBone::LArm1] = GetBone(KeyBone::LArm1, ents[id]);
	//bones[KeyBone::LArm2] = GetBone(KeyBone::LArm2, ents[id]);

	//bones[KeyBone::RArm1] = GetBone(KeyBone::RArm1, ents[id]);
	//bones[KeyBone::RArm2] = GetBone(KeyBone::RArm2, ents[id]);

	//bones[KeyBone::LLeg1] = GetBone(KeyBone::LLeg1, ents[id]);
	//bones[KeyBone::LLeg2] = GetBone(KeyBone::LLeg2, ents[id]);
	//bones[KeyBone::LLeg3] = GetBone(KeyBone::LLeg3, ents[id]);

	//bones[KeyBone::RLeg1] = GetBone(KeyBone::RLeg1, ents[id]);
	//bones[KeyBone::RLeg2] = GetBone(KeyBone::RLeg2, ents[id]);
	//bones[KeyBone::RLeg3] = GetBone(KeyBone::RLeg3, ents[id]);
											 
	return bones;
}

ImVec2& nEntity::GetSCoords()
{
	return SCoords;
}

std::vector<ImVec2>& nEntity::GetSCoordsBones()
{
	return SCoordsBone;
}

