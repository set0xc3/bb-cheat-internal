#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class KeyBind
{
public:
	KeyBind() 
	{
		static bool one = true; m_keyCodes.insert(map<DWORD, string>::value_type(0x00, ""));
		if (one)
		{
			m_keyCodes.insert(map<DWORD, string>::value_type(0x01, "Mouse Left"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x02, "Mouse Right"));

			m_keyCodes.insert(map<DWORD, string>::value_type(0x30, "0"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x31, "1"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x32, "2"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x33, "3"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x34, "4"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x35, "5"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x36, "6"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x37, "7"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x38, "8"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x39, "9"));

			m_keyCodes.insert(map<DWORD, string>::value_type(0x41, "A"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x42, "B"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x43, "C"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x44, "D"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x45, "E"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x46, "F"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x47, "G"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x48, "H"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x49, "I"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x4A, "J"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x4B, "K"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x4C, "L"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x4D, "M"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x4E, "N"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x4F, "O"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x50, "P"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x51, "Q"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x52, "R"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x53, "S"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x54, "T"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x55, "U"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x56, "V"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x57, "W"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x58, "X"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x59, "Y"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0x5A, "Z"));

			m_keyCodes.insert(map<DWORD, string>::value_type(0xA0, "LSHIFT"));
			m_keyCodes.insert(map<DWORD, string>::value_type(0xA2, "LCONTROL"));

			one = false;
		}
	};
	~KeyBind() {};

	void Start();

	bool getStart() const;
	bool getHandler() const;
	string getKey() const;
	DWORD getKeyCode() const;
	map<DWORD, string> getKeyCodes() const;

	bool setStart(bool value);
	bool setHandler(bool value);
	string setKey(string value);
	void setKeyCode(DWORD value);

private:
	map<DWORD, string> m_keyCodes;
	bool m_isStart;
	bool m_isHandler;
	string m_key = "Mouse Left";
	DWORD m_keyCode = 0x01;
};
