#pragma once
#include "includes.h"

namespace Memory
{
	template<typename T>
	bool ValidatePointer(T lpAddress);
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	void Ret(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
}
