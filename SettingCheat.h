#pragma once
#include "includes.h"

enum SKeyBool
{
    SKeyBool_ENABLED,
    SKeyBool_ESP,
    SKeyBool_2DBBOX,
    SKeyBool_3DBOX,
    SKeyBool_NAME,
    SKeyBool_HEALTH,
    SKeyBool_DISTANCE,
    SKeyBool_COUNT
};

enum SKeyColor
{
    SKeyColor_WHITE,
    SKeyColor_YELLOW,
    SKeyColor_RED,
    SKeyColor_GREEN,
    SKeyColor_BLUE,
    SKeyColor_ORANGE,
    SKeyColor_COUNT
};

class SettingCheat
{
public:
    struct Setting
    {
        bool arraybool[SKeyBool_COUNT];
        Color arrayColor[SKeyColor_COUNT];
    };
    Setting Setting;

public:

};
SettingCheat* settingCheat = new SettingCheat();

namespace NSCheat
{
    int CurrColor = 0;

    void Initialize();
    bool& GetBool(int id);
    Color GetColor();
}

void NSCheat::Initialize()
{
    settingCheat->Setting.arraybool[SKeyBool_ENABLED] = true;
    settingCheat->Setting.arraybool[SKeyBool_ESP] = true;
    settingCheat->Setting.arraybool[SKeyBool_2DBBOX] = false;
    settingCheat->Setting.arraybool[SKeyBool_3DBOX] = true;
    settingCheat->Setting.arraybool[SKeyBool_NAME] = true;
    settingCheat->Setting.arraybool[SKeyBool_HEALTH] = true;
    settingCheat->Setting.arraybool[SKeyBool_DISTANCE] = true;

    settingCheat->Setting.arrayColor[SKeyColor_WHITE] = Color(255.0f, 255.0f, 255.0f, 255.0f);
    settingCheat->Setting.arrayColor[SKeyColor_YELLOW] = Color(255.0f, 255.0f, 255.0f, 0.0f);
    settingCheat->Setting.arrayColor[SKeyColor_GREEN] = Color(255.0f, 0.0f, 255.0f, 0.0f);
    settingCheat->Setting.arrayColor[SKeyColor_RED] = Color(255.0f, 255.0f, 0.0f, 0.0f);
    settingCheat->Setting.arrayColor[SKeyColor_BLUE] = Color(255.0f, 0.0f, 0.0f, 255.0f);
    settingCheat->Setting.arrayColor[SKeyColor_ORANGE] = Color(255.0f, 255.0f, 165.0f, 0.0f);
}

bool& NSCheat::GetBool(int id)
{
    return settingCheat->Setting.arraybool[id];
}

Color NSCheat::GetColor()
{
    return settingCheat->Setting.arrayColor[CurrColor];
}

