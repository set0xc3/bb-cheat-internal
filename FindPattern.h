#include "includes.h"

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))


namespace Pattern
{
	void MsgBoxAddy(DWORD addy);
	void PlaceJMP(BYTE* Address, DWORD jumpTo, DWORD length);
	MODULEINFO GetModuleInfo(char* szModule);
	DWORD FindPattern(char* module, char* pattern, char* mask);
	char* InScan(char* pattern, char* mask, char* begin, unsigned int size);
	char* PatternScan(char* pattern, char* mask);
}