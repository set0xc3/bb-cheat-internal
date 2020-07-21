
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

//D3D11 HOOK DEFINITIONS
typedef HRESULT(__stdcall* IDXGISwapChainPresent) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
