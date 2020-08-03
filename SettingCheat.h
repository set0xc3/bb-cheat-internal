#pragma once
#include "includes.h"

enum SKeyBool
{
    SKeyBool_ESP,
    SKeyBool_2DBBOX,
    SKeyBool_3DBOX,
    SKeyBool_NAME,
    SKeyBool_HEALTH,
    SKeyBool_DISTANCE,
    SKeyBool_AIMBOT,

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

enum SKeyInt
{
    SKeyInt_FOV,
    SKeyInt_BONE,
    SKeyInt_COLOR,
    SKeyInt_SMOOTH,

    SKeyInt_COUNT
};

class SettingCheat
{
public:
    struct Setting
    {
        bool arrayBool[SKeyBool_COUNT];
        Color arrayColor[SKeyColor_COUNT];
        int arrayInt[SKeyInt_COUNT];
    };
    Setting Setting;

public:

};
SettingCheat* settingCheat = new SettingCheat();

namespace NSCheat
{
    void Initialize();

    bool& GetBool(int id)
    {
        return settingCheat->Setting.arrayBool[id];
    }
    Color GetColor(int id) 
    {
        return settingCheat->Setting.arrayColor[id];
    }
    int& GetInt(int id)
    {
        return settingCheat->Setting.arrayInt[id];
    }
}

void NSCheat::Initialize()
{
    settingCheat->Setting.arrayBool[SKeyBool_ESP] = true;
    settingCheat->Setting.arrayBool[SKeyBool_2DBBOX] = false;
    settingCheat->Setting.arrayBool[SKeyBool_3DBOX] = true;
    settingCheat->Setting.arrayBool[SKeyBool_NAME] = true;
    settingCheat->Setting.arrayBool[SKeyBool_HEALTH] = true;
    settingCheat->Setting.arrayBool[SKeyBool_DISTANCE] = false;
    settingCheat->Setting.arrayBool[SKeyBool_AIMBOT] = true;

    settingCheat->Setting.arrayInt[SKeyInt_FOV] = 5;
    settingCheat->Setting.arrayInt[SKeyInt_BONE] = 2;
    settingCheat->Setting.arrayInt[SKeyInt_COLOR] = 0;
    settingCheat->Setting.arrayInt[SKeyInt_SMOOTH] = 10;

    settingCheat->Setting.arrayColor[SKeyColor_WHITE] = Color(255.0f, 255.0f, 255.0f, 255.0f);
    settingCheat->Setting.arrayColor[SKeyColor_YELLOW] = Color(255.0f, 255.0f, 255.0f, 0.0f);
    settingCheat->Setting.arrayColor[SKeyColor_GREEN] = Color(255.0f, 0.0f, 255.0f, 0.0f);
    settingCheat->Setting.arrayColor[SKeyColor_RED] = Color(255.0f, 255.0f, 0.0f, 0.0f);
    settingCheat->Setting.arrayColor[SKeyColor_BLUE] = Color(255.0f, 0.0f, 0.0f, 255.0f);
    settingCheat->Setting.arrayColor[SKeyColor_ORANGE] = Color(255.0f, 255.0f, 165.0f, 0.0f);
}
