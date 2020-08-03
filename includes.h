
// Standard imports
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <Psapi.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include <dxgi.h>
#include <unordered_set>
#include <string.h>
#include <sstream>
#include <locale>
#include <codecvt>
#include <chrono>
#include <thread>

#include <Vectors.h>

// Kiero imports
#include "kiero/kiero.h"

// DX11 imports
#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX11tex.h>
#pragma comment(lib, "d3d11.lib")

// ImGUI imports
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


#define PI 3.1415927f//3.14159265358979323846

//D3D11 HOOK DEFINITIONS
typedef HRESULT(__stdcall* IDXGISwapChainPresent) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

// HOOKS Send
typedef int(__cdecl* GetServerTime)(int a1);
typedef void(__cdecl* WRITE_LONG)(int a1);
typedef void(__cdecl* WRITE_FLOAT)(int a1);
typedef void(__cdecl* BEGIN_WRITE)(char proto, int packetid, int a3);
typedef void(__cdecl* WRITE_BYTE)(int a1, int a2);
typedef void(__cdecl* END_WRITE)(int a1);
typedef void(__cdecl* Send)(void* thisClient);
typedef void(__cdecl* send_attack)(int thisClient, Vector3 pos, int time, int AttackData);
