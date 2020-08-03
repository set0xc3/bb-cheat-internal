
#include "CheatEngine.h"

//std::string narrow(const std::wstring& wstr)
//{
//	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
//	return convert.to_bytes(wstr);
//}

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

	return true;
}


void CheatEngine::Find(int id, char* module, char* pattern, char* mask, DWORD offset, bool ptr)
{
	Pattern* _pattern = new Pattern();
	_pattern->id = id;
	_pattern->module = module;
	_pattern->pattern = pattern;
	_pattern->mask = mask;

	if (ptr) _pattern->address = Memory::FindDMAAddy(::Pattern::FindPattern(module, pattern, mask) + offset, { 0x0 });
	else _pattern->address = ::Pattern::FindPattern(module, pattern, mask) + offset;

	if (!ValidatePointer(_pattern->address))
		_pattern->isFind = false;
	else
		_pattern->isFind = true;

	this->pattern.push_back(_pattern);
}

DWORD CheatEngine::GetAddress(int id)
{
	for (size_t i = 0; i < this->pattern.size(); i++)
	{
		if (id == this->pattern[i]->id) 
			return this->pattern[i]->address;
	}
}

//Offsets Offsets::GetModule()
//{
//	return Offsets();
//}

void CheatEngine::Shutdown()
{

}

void CheatEngine::Initialize()
{
	// GetModules
	this->GetModule.dwGameAssembly = (DWORD)GetModuleHandle("GameAssembly.dll");
	this->GetModule.dwUnityPlayer = (DWORD)GetModuleHandle("UnityPlayer.dll");

	// Entity
	this->Find(
		KeyPattern::Pattern_Entity,
		(char*)"GameAssembly.dll",
		(char*)"\x66\x0F\xD6\x46\x44",
		(char*)"xxxxx", 0x0, false);

	// PLH$$Audio_fs_water | PLH
	this->Find(
		KeyPattern::Pattern_PLH,
		(char*)"GameAssembly.dll",
		(char*)"\xA1\x00\x00\x00\x00\xF6\x80\xBF\x00\x00\x00\x02\x74\x14\x83\x78\x70\x00\x75\x0E\x50\xE8\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x83\xC4\x04\x8B\x40\x5C\x8B\x40\x1C\x85\xC0\x0F\x84\x00\x00\x00\x00\x3B\x70\x0C\x0F\x83",
		(char*)"x????xxxxxxxxxxxxxxxxx????x????xxxxxxxxxxxxx????xxxxx", 0x1, true);

	// Client$$recv_detonate | VWIK
	this->Find(
		KeyPattern::Pattern_VWIK,
		(char*)"GameAssembly.dll",
		(char*)"\xA1\x00\x00\x00\x00\xF6\x80\xBF\x00\x00\x00\x02\x74\x0F\x83\x78\x70\x00\x75\x09\x50\xE8\x00\x00\x00\x00\x83\xC4\x04\xF3\x0F\x10\x45\xF0",
		(char*)"x????xxxxxxxxxxxxxxxxx????xxxxxxxx", 0x1, true);

	// GUIInv$$_cctor | GUIInv
	this->Find(
		KeyPattern::Pattern_GUIInv,
		(char*)"GameAssembly.dll",
		(char*)"\xA1\x00\x00\x00\x00\x8B\x40\x5C\xC6\x00\x00\xA1\x00\x00\x00\x00\x8B\x40\x5C\xC6\x40\x01\x00\xA1\x00\x00\x00\x00\x8B\x40\x5C\xC6\x40\x02\x00\xA1\x00\x00\x00\x00\xF6\x80\xBF\x00\x00\x00\x02",
		(char*)"x????xxxxxxx????xxxxxxxx????xxxxxxxx????xxxxxxx", 0x1, true);

	// Controll$$UpdatePhysics | Controll
	this->Find(
		KeyPattern::Pattern_Controll,
		(char*)"GameAssembly.dll",
		(char*)"\xA1\x00\x00\x00\x00\x0F\x57\xC0\x66\x0F\xD6\x45\xE0",
		(char*)"x????xxxxxxxx", 0x1, true);

	// MainManager$$_cctor | MainManager
	this->Find(
		KeyPattern::Pattern_MainManager,
		(char*)"GameAssembly.dll",
		(char*)"\xA1\x00\x00\x00\x00\x8B\x40\x5C\xC7\x00\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x8B\x40\x5C\xC6\x40\x04\x00\xA1\x00\x00\x00\x00\x8B\x40\x5C\xC6\x40\x05\x00",
		(char*)"x????xxxxxxxxxx????xxxxxxxx????xxxxxxx", 0x1, true);

	this->bones.resize(20);
	this->GetPlayer.Entity.BoneSCoords.resize(20);
}

int CheatEngine::GetGameMode()
{
	static uintptr_t dwAddr = NULL;
	static bool b = true;
	if (b) {
		dwAddr = Memory::FindDMAAddy(this->GetAddress(KeyPattern::Pattern_Controll), this->GetOffset.World.GameMode);
		b = false;
	}

	return *(int*)dwAddr;
}

float* CheatEngine::GetMatrix()
{
	float* address = (float*)Memory::FindDMAAddy(this->GetModule.dwUnityPlayer + 0x1059AC0, this->GetOffset.Engine.Matrix);
	return address;
}

DWORD CheatEngine::GetBaseMatrix()
{
	DWORD address = Memory::FindDMAAddy(this->GetModule.dwUnityPlayer + 0x1059AC0, this->GetOffset.Engine.Matrix);
	return address;
}

DWORD CheatEngine::GetLDeath()
{
	static uintptr_t dwAddr = NULL;
	static bool b = true;
	if (b) {
		dwAddr = Memory::FindDMAAddy(this->GetAddress(KeyPattern::Pattern_Controll), this->GetOffset.Local.Death);
		b = false;
	}

	return *(int*)dwAddr;
}

int CheatEngine::GetLTeam()
{
	static uintptr_t dwAddr = NULL;
	static bool b = true;
	if (b) {
		dwAddr = Memory::FindDMAAddy(this->GetAddress(KeyPattern::Pattern_Controll), this->GetOffset.Local.Team);
		b = false;
	}

	return *(int*)dwAddr;
}

int CheatEngine::GetLTeamZombie()
{
	static uintptr_t dwAddr = NULL;
	static bool b = true;
	if (b) {
		dwAddr = Memory::FindDMAAddy(this->GetAddress(KeyPattern::Pattern_Controll), this->GetOffset.Local.TeamZombie);
		b = false;
	}

	return *(int*)dwAddr;
}

int CheatEngine::GetIsMenu()
{
	static uintptr_t dwAddr = NULL;
	static bool b = true;
	if (b) {
		dwAddr = Memory::FindDMAAddy(this->GetAddress(KeyPattern::Pattern_Controll), this->GetOffset.Local.Menu);
		b = false;
	}

	return *(int*)dwAddr;
}

Vector3 MatrixToDir(float matrix[16])
{
	return Vector3(matrix[2], matrix[6], matrix[10]);
}

Vector3 CheatEngine::GetDir()
{
	float* address = (float*)Memory::FindDMAAddy(this->GetModule.dwUnityPlayer + 0x1059AC0, this->GetOffset.Engine.Matrix);
	
	return MatrixToDir(address);
}

Vector3 CheatEngine::GetAngle()
{
	static uintptr_t dwAddr = NULL;
	static bool b = true;
	if (b) {
		dwAddr = Memory::FindDMAAddy(this->GetAddress(KeyPattern::Pattern_Controll), this->GetOffset.Local.Angle);
		b = false;
	}

	return *(Vector3*)dwAddr;
}

Vector3 CheatEngine::GetCoords()
{
	static uintptr_t dwAddr = NULL;
	dwAddr = Memory::FindDMAAddy(this->GetModule.dwUnityPlayer + 0x1059AC0, this->GetOffset.Local.Coords);

	return *(Vector3*)dwAddr;
}

ImVec2& CheatEngine::GetSCoords()
{
	return  this->GetPlayer.Player.SCoords;
}

