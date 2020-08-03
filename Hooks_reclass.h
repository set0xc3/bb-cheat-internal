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


class Controll
{
public:


}; 
Controll* localptr;

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
PlayerData* entsptrAD;

namespace nEntity
{
	DWORD offset = 0x10;

	ImVec2 SCoords;
	std::vector<Vector3> CoordsBones;
	std::vector<ImVec2> SCoordsBone;

	std::vector<Vector3> CoordsBones2;

	void Initialize();
	void SetBase(PlayerData* entsptr, DWORD ptr);
	DWORD& GetOffset();
	DWORD GetIsEntity(PlayerData* entsptr);

	Vector3 GetBone(PlayerData* entsptr, int bone, int id);
	Vector3 GetCoords(PlayerData* entsptr);
	Vector3 GetCoordsBones(PlayerData* entsptr, int bone, int id);

	ImVec2& GetSCoords();
	ImVec2& GetSCoordsBones(int bone);

	bool GetEntity(PlayerData* entsptr);

	int GetTeam(PlayerData* entsptr);
	int GetTeamZombie(PlayerData* entsptr);
	int GetDeath(PlayerData* entsptr);
	int GetHealth(PlayerData* entsptr);

	const std::string GetName(PlayerData* entsptr);
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



bool nEntity::GetEntity(PlayerData* entsptr)
{
	if (entsptr == NULL)
	{
		return false;
	}
	else return true;
}

void nEntity::Initialize()
{
	SCoordsBone.resize(12);
	CoordsBones.resize(12);
	CoordsBones2.resize(12);
}

void nEntity::SetBase(PlayerData* entsptr, DWORD ptr)
{
	entsptr = (PlayerData*)ptr;
}

DWORD& nEntity::GetOffset()
{
	return nEntity::offset;
}

DWORD nEntity::GetIsEntity(PlayerData* entsptr)
{
	if (entsptr == nullptr) return NULL;

	return entsptr->localPlayer;
}

int nEntity::GetTeam(PlayerData* entsptr)
{
	if (entsptr == nullptr) return -1;

	return entsptr->team;
}

int nEntity::GetTeamZombie(PlayerData* entsptr)
{
	if (entsptr == nullptr) return -1;

	return entsptr->teamZombie;
}

Vector3 nEntity::GetCoords(PlayerData* entsptr)
{
	if (entsptr == nullptr) return Vector3(0, 0, 0);

	return entsptr->pos;
}

int nEntity::GetDeath(PlayerData* entsptr)
{
	if (entsptr == nullptr) return -1;

	return entsptr->death;
}

int nEntity::GetHealth(PlayerData* entsptr)
{
	if (entsptr == nullptr) return -1;

	return entsptr->health;
}

const std::string nEntity::GetName(PlayerData* entsptr)
{
	if(entsptr == nullptr) return "";

	if (entsptr->string18ptr != NULL)
	{
		WCHAR* txt = (WCHAR*)entsptr->string18ptr->name;

		return (narrow(txt));
	}
}


Vector3 nEntity::GetBone(PlayerData* entsptr, int bone, int id)
{
	if (entsptr == nullptr) return Vector3(0, 0, 0);

	if (bone == KeyBone::Bone_Head)
	{
		if (entsptr->Head == nullptr || 
			entsptr->Head->N00000D89 == nullptr ||
			entsptr->Head->N00000D89->N00000DA8 == nullptr) return Vector3(0, 0, 0);

		if (id == 0) return CoordsBones[bone] = entsptr->Head->N00000D89->N00000DA8->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->Head->N00000D89->N00000DA8->Coords;
	}

	if (bone == KeyBone::Bone_Body)
	{
		if (entsptr->Body == nullptr ||
			entsptr->Body->N00000D1F == nullptr ||
			entsptr->Body->N00000D1F->N00000D3E == nullptr) return Vector3(0, 0, 0);
			
		if (id == 0) return CoordsBones[bone] = entsptr->Body->N00000D1F->N00000D3E->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->Body->N00000D1F->N00000D3E->Coords;
	}

	if (bone == KeyBone::Bone_LArm1)
	{
		if (entsptr->LArm == nullptr ||
			entsptr->LArm->N00000DD8 == nullptr ||
			entsptr->LArm->N00000DD8->N00000DEA == nullptr ||
			entsptr->LArm->N00000DD8->N00000DEA->N00000E1D == nullptr) return Vector3(0, 0, 0);

		if (id == 0) return CoordsBones[bone] = entsptr->LArm->N00000DD8->N00000DEA->N00000E1D->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->LArm->N00000DD8->N00000DEA->N00000E1D->Coords;
	}

	if (bone == KeyBone::Bone_LArm2)
	{
		if (entsptr->LArm == nullptr ||
			entsptr->LArm->N00000DD9 == nullptr ||
			entsptr->LArm->N00000DD9->N00000DFE == nullptr ||
			entsptr->LArm->N00000DD9->N00000DFE->N00000E56 == nullptr) return Vector3(0, 0, 0);

		if (id == 0) return CoordsBones[bone] = entsptr->LArm->N00000DD9->N00000DFE->N00000E56->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->LArm->N00000DD9->N00000DFE->N00000E56->Coords;
	}

	if (bone == KeyBone::Bone_RArm1)
	{
		if (entsptr->RArm == nullptr || 
			entsptr->RArm->N00000ECA == nullptr ||
			entsptr->RArm->N00000ECA->N00000EDC == nullptr ||
			entsptr->RArm->N00000ECA->N00000EDC->N00000F0F == nullptr) return Vector3(0, 0, 0);

		if (id == 0) return CoordsBones[bone] = entsptr->RArm->N00000ECA->N00000EDC->N00000F0F->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->RArm->N00000ECA->N00000EDC->N00000F0F->Coords;
	}

	if (bone == KeyBone::Bone_RArm2)
	{
		if (entsptr->RArm == nullptr ||
			entsptr->RArm->N00000ECB == nullptr ||
			entsptr->RArm->N00000ECB->N00000EF0 == nullptr ||
			entsptr->RArm->N00000ECB->N00000EF0->N00000F48 == nullptr) return Vector3(0, 0, 0);

		if (id == 0) return CoordsBones[bone] = entsptr->RArm->N00000ECB->N00000EF0->N00000F48->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->RArm->N00000ECB->N00000EF0->N00000F48->Coords;
	}

	if (bone == KeyBone::Bone_LLeg1)
	{
		if (entsptr->LLeg == nullptr ||
			entsptr->LLeg->N00000F78 == nullptr ||
			entsptr->LLeg->N00000F78->N00000F9E == nullptr ||
			entsptr->LLeg->N00000F78->N00000F9E->N00000FE5 == nullptr) return Vector3(0, 0, 0);

		if (id == 0) return CoordsBones[bone] = entsptr->LLeg->N00000F78->N00000F9E->N00000FE5->Coords;
		if (id == 1) return CoordsBones2[bone] = entsptr->LLeg->N00000F78->N00000F9E->N00000FE5->Coords;
	}

	if (bone == KeyBone::Bone_LLeg2)
	{
		if (entsptr->LLeg == nullptr ||
			entsptr->LLeg->N00000F79 == nullptr ||
			entsptr->LLeg->N00000F79->N00000FB2 == nullptr ||
			entsptr->LLeg->N00000F79->N00000FB2->N0000101E == nullptr) return Vector3(0, 0, 0);

		if (id == 0) CoordsBones[bone] = entsptr->LLeg->N00000F79->N00000FB2->N0000101E->Coords;
		if (id == 1) CoordsBones2[bone] = entsptr->LLeg->N00000F79->N00000FB2->N0000101E->Coords;

		return CoordsBones[bone];
	}

	if (bone == KeyBone::Bone_LLeg3)
	{
		if (entsptr->LLeg == nullptr ||
			entsptr->LLeg->N00000F7A == nullptr ||
			entsptr->LLeg->N00000F7A->N00000FC6 == nullptr ||
			entsptr->LLeg->N00000F7A->N00000FC6->N00001057 == nullptr) return Vector3(0, 0, 0);

		if (id == 0) CoordsBones[bone] = entsptr->LLeg->N00000F7A->N00000FC6->N00001057->Coords;
		if (id == 1) CoordsBones2[bone] = entsptr->LLeg->N00000F7A->N00000FC6->N00001057->Coords;

		return CoordsBones[bone];
	}

	if (bone == KeyBone::Bone_RLeg1)
	{
		if (entsptr->RLeg == nullptr ||
			entsptr->RLeg->N00000F8C == nullptr ||
			entsptr->RLeg->N00000F8C->N00001085 == nullptr ||
			entsptr->RLeg->N00000F8C->N00001085->N000010CC == nullptr) return Vector3(0, 0, 0);

		if (id == 0) CoordsBones[bone] = entsptr->RLeg->N00000F8C->N00001085->N000010CC->Coords;
		if (id == 1) CoordsBones2[bone] = entsptr->RLeg->N00000F8C->N00001085->N000010CC->Coords;

		return CoordsBones[bone];
	}

	if (bone == KeyBone::Bone_RLeg2)
	{
		if (entsptr->RLeg == nullptr ||
			entsptr->RLeg->N00000F8D == nullptr ||
			entsptr->RLeg->N00000F8D->N00001099 == nullptr ||
			entsptr->RLeg->N00000F8D->N00001099->N00001106 == nullptr) return Vector3(0, 0, 0);

		if (id == 0) CoordsBones[bone] = entsptr->RLeg->N00000F8D->N00001099->N00001106->Coords;
		if (id == 1) CoordsBones2[bone] = entsptr->RLeg->N00000F8D->N00001099->N00001106->Coords;

		return CoordsBones[bone];
	}

	if (bone == KeyBone::Bone_RLeg3)
	{
		if (entsptr->RLeg == nullptr ||
			entsptr->RLeg->N00000F8E == nullptr ||
			entsptr->RLeg->N00000F8E->N000010AD == nullptr ||
			entsptr->RLeg->N00000F8E->N000010AD->N0000113F == nullptr) return Vector3(0, 0, 0);

		if (id == 0) CoordsBones[bone] = entsptr->RLeg->N00000F8E->N000010AD->N0000113F->Coords;
		if (id == 1) CoordsBones2[bone] = entsptr->RLeg->N00000F8E->N000010AD->N0000113F->Coords;

		return CoordsBones[bone];
	}
}

Vector3 nEntity::GetCoordsBones(PlayerData* entsptr, int bone, int id)
{										 
	return GetBone(entsptr, bone, id);
}

ImVec2& nEntity::GetSCoords()
{
	return SCoords;
}

ImVec2& nEntity::GetSCoordsBones(int bone)
{
	return SCoordsBone[bone];
}

