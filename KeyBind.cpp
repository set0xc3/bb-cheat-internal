#include <KeyBind.h>


bool KeyBind::setStart(bool value)
{
	return m_isStart = value;
}

bool KeyBind::setHandler(bool value)
{
	return m_isHandler = value;
}

string KeyBind::setKey(string value)
{
	return m_key = value;
}

void KeyBind::setKeyCode(DWORD value)
{
	m_keyCode = value;
}

void KeyBind::Start()
{
	m_isStart = true;
}

bool KeyBind::getStart() const
{
	return m_isStart;
}

bool KeyBind::getHandler() const
{
	return m_isHandler;
}

map<DWORD, string> KeyBind::getKeyCodes() const
{
	return m_keyCodes;
}

string KeyBind::getKey() const
{
	return m_key;
}

DWORD KeyBind::getKeyCode() const
{
	return m_keyCode;
}
