#pragma once

#include <includes.h>

#include <tchar.h>
#include <intrin.h>
#include <sha256.h>

void GetHWID()
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
		//exit(-1);
	}
}
