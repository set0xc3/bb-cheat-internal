#pragma once

#include <includes.h>

#include <tchar.h>
#include <intrin.h>
#include <sha256.h>

__forceinline bool DetourExit(void* src, void* dst, int len)
{
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(src, 0x90, len);

	uintptr_t relativeAddress = ((uintptr_t)dst - (uintptr_t)src) - 5;

	*(BYTE*)src = 0xE9;
	*(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(src, len, curProtection, &temp);

	return true;
}

__forceinline char* TrampHookExit(BYTE* src, BYTE* dst, const intptr_t len)
{
	// Make sure the length is greater than 5
	if (len < 5) return 0;

	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	void* gateway = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//Write the stolen bytes into the gateway
	memcpy(gateway, src, len);

	// Get the gateway to destination addy
	intptr_t  gatewayRelativeAddr = ((intptr_t)src - (intptr_t)gateway) - 5;

	// Add the jmp opcode to the end of the gateway
	*(char*)((intptr_t)gateway + len) = 0xE9;

	// Add the address to the jmp
	*(intptr_t*)((intptr_t)gateway + len + 1) = gatewayRelativeAddr;

	// Perform the detour
	DetourExit(src, dst, len);

	return (char*)gateway;
}

__forceinline void PlaceJMPExit(BYTE* Address, DWORD jumpTo, DWORD length = 5)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	//give that address read and write permissions and store the old permissions at oldProtection
	VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// Calculate the "distance" we're gonna have to jump - the size of the JMP instruction
	dwRelAddr = (DWORD)(jumpTo - (DWORD)Address) - 5;

	// Write the JMP opcode @ our jump position...
	*Address = 0xE9;

	// Write the offset to where we're gonna jump
	//The instruction will then become JMP ff002123 for example
	*((DWORD*)(Address + 0x1)) = dwRelAddr;

	// Overwrite the rest of the bytes with NOPs
	//ensuring no instruction is Half overwritten(To prevent any crashes)
	for (DWORD x = 0x5; x < length; x++)
		*(Address + x) = 0x90;

	// Restore the default permissions
	VirtualProtect(Address, length, dwOldProtect, &dwBkup);
}

__forceinline void GetHWID()
{
	//First part gets the HDD informations
	TCHAR volumeName[MAX_PATH + 1] = { 0 };
	TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;
	if (GetVolumeInformation(
		_T("\\"),
		volumeName,
		ARRAYSIZE(volumeName),
		&serialNumber,
		&maxComponentLen,
		&fileSystemFlags,
		fileSystemName,
		ARRAYSIZE(fileSystemName)))
	{

	}
	//Second part gets the computer name
	TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName) / sizeof(computerName[0]);
	if (GetComputerName(
		computerName,
		&size))
	{
		//std::cout << "Computer Name: " << computerName << std::endl;
	}
	//Third part gets the CPU Hash
	int cpuinfo[4] = { 0, 0, 0, 0 }; //EAX, EBX, ECX, EDX
	__cpuid(cpuinfo, 0);
	char16_t hash = 0;
	char16_t* ptr = (char16_t*)(&cpuinfo[0]);
	for (char32_t i = 0; i < 8; i++)
		hash += ptr[i];

	std::string HWID = std::to_string(serialNumber) + fileSystemName + std::to_string(hash);


	if (SHA256::hashString(HWID) != "aacd4c669d13ca69dc9d09cd4a64be2b3a0f51ad7ec86dee5a014200502ede03")
	{	
		DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + (DWORD)(1+rand() % 0x1000000);

		_asm {
			jmp bAddr
		}

		exit(-1);
	}
}
