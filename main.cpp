
#include "includes.h"

#include "Renderer.h"
#include "Memory.h"
#include "SettingImgui.h"
#include "CheatEngine.h"
#include <Vectors.h>
#include <Hooks_reclass.h>
#include <SettingCheat.h>
#include <HWID.h>

#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")
#pragma comment(lib, "msdmo.lib")
#pragma comment(lib, "Strmiids.lib")


#pragma execution_character_set("utf-8")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main D3D11 Objects
HWND window = NULL;
WNDPROC oWndProc = NULL;
ID3D11Device* pDevice = NULL;
IDXGISwapChain* pSwapChain = NULL;
IDXGISwapChainPresent oPresent = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView = NULL;

// Boolean
BOOL g_bInitialised = TRUE;
bool g_IsCheat = TRUE;
bool g_IsWndProc = TRUE;
bool g_ShowImgui = false;
bool g_ShowImguiESP = TRUE;
bool g_Show_Demo_Window = TRUE;
bool g_Show_Another_Window = TRUE;

bool Detour32(void* src, void* dst, int len)
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

char* TrampHook32(BYTE* src, BYTE* dst, const intptr_t len)
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
	Detour32(src, dst, len);

	return (char*)gateway;
}

void PlaceJMP(BYTE* Address, DWORD jumpTo, DWORD length = 5)
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

bool WorldToScreenOpenGL(Vector3 pos, ImVec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

Vector3 Subtract(Vector3 src, Vector3 dst)
{
	Vector3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

float Magnitude(Vector3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float Distance(Vector3 src, Vector3 dst)
{
	Vector3 diff = Subtract(src, dst);
	diff = diff;
	return Magnitude(diff);
}

LRESULT CALLBACK WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_IsCheat)
	{
		if (uMsg == WM_KEYUP)
		{
			if (wParam == VK_DELETE)
			{
				g_ShowImgui ^= 1;
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		POINT mPos;
		GetCursorPos(&mPos);
		ScreenToClient(window, &mPos);
		ImGui::GetIO().MousePos.x = mPos.x;
		ImGui::GetIO().MousePos.y = mPos.y;
		ImGui::GetIO().MouseDrawCursor = g_ShowImgui;

		if (g_ShowImgui)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			return true;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{// Игровой поток

	if (GetAsyncKeyState(VK_F6) & 1)
	{
		system("cls");
	}

	// Initialize
	if (g_bInitialised)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			// Инициализация Direct3D | ImGui
			{
				pDevice->GetImmediateContext(&pContext);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				window = sd.OutputWindow;

				// ImGui
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // ImGuiConfigFlags_NoMouseCursorChange

				// ImGui
				ImGui_ImplWin32_Init(window);
				ImGui_ImplDX11_Init(pDevice, pContext);
				ImGui::GetIO().ImeWindowHandle = window;
				
				Renderer::GetInstance().Initialize(pContext);

				// Set OriginalWndProcHandler to the Address of the Original WndProc function
				oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			}		
		
			// Установка области просмотра
			{
				D3D11_VIEWPORT viewport;
				ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;
				viewport.Width = Renderer::GetInstance().GetWidth();
				viewport.Height = Renderer::GetInstance().GetHeight();

				pContext->RSSetViewports(1, &viewport);
			}
		
			g_bInitialised = false;
		}	
		else return oPresent(pSwapChain, SyncInterval, Flags);

		NSCheat::Initialize();
		cheatEngine->Initialize();
		//SettingImgui::Initialize();

		//entityhook
		//DWORD entityAddy = offsets->pattern[KeyPattern::Entity]->_address;
		//EntlistJmpBack = entityAddy + 0x5;
		//PlaceJMP((BYTE*)entityAddy, (DWORD)entityhook, 5);

		//std::cout << std::hex << "[+] dwGameAssembly: " << offsets->GetModule.dwGameAssembly << std::endl;
		//std::cout << std::hex << "[+] dwUnityPlayer: " << offsets->GetModule.dwUnityPlayer << std::endl;

		//std::cout << std::hex << "\t[+] Entity: " << offsets->pattern[KeyPattern::Entity]->_address << std::endl;

		//std::cout << std::hex << "\t[+] PLH: " << offsets->GetAddress(KeyPattern::PLH) << std::endl;
		//std::cout << std::hex << "\t[+] VWIK: " << offsets->GetAddress(KeyPattern::VWIK) << std::endl;
		//std::cout << std::hex << "\t[+] GUIInv: " << offsets->GetAddress(KeyPattern::GUIInv) << std::endl;
		//std::cout << std::hex << "\t[+] Controll: " << offsets->GetAddress(KeyPattern::Controll) << std::endl;
		//std::cout << std::hex << "\t[+] MainManager: " << offsets->GetAddress(KeyPattern::MainManager) << std::endl;

		//PtrTest();
	}

	// ImGui - Update
	if (g_IsCheat)
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool greetings = true;
		if (greetings)
		{
			ImGui::Begin("title", &greetings, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
			ImGui::Text("Cheat loaded, press DELETE for menu");
			ImGui::End();

			if (g_ShowImgui)
			{
				greetings = false;
			}		
		}

		// Menu
		if (g_ShowImgui)
		{
			//ImGui::ShowDemoWindow(&g_Show_Demo_Window);

			// Window [Hello, world!]
			{
	
				static ImVec4 color = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
				static bool alpha_preview = true;
				static bool alpha_half_preview = false;
				static bool drag_and_drop = true;
				static bool options_menu = true;
				static bool hdr = false;
				ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

				ImGui::SetNextWindowSize(ImVec2(200.f, 200.f));

				ImGui::Begin("ESP", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

				ImGui::Checkbox("Enabled", &NSCheat::GetBool(SKeyBool::SKeyBool_ENABLED)); 
				ImGui::Checkbox("2DBox", &NSCheat::GetBool(SKeyBool::SKeyBool_2DBBOX)); 
				ImGui::Checkbox("3DBox", &NSCheat::GetBool(SKeyBool::SKeyBool_3DBOX)); 
				ImGui::Checkbox("Name", &NSCheat::GetBool(SKeyBool::SKeyBool_NAME)); 
				ImGui::Checkbox("Health", &NSCheat::GetBool(SKeyBool::SKeyBool_HEALTH));
				ImGui::Checkbox("Distance", &NSCheat::GetBool(SKeyBool::SKeyBool_DISTANCE));

				const char* items[] = { "WHITE", "YELLOW", "RED", "GREEN", "BLUE", "ORANGE" };
				static int item_current = 0;
				ImGui::Combo("Color", &NSCheat::CurrColor, items, IM_ARRAYSIZE(items));

				ImGui::End();	
			}
			
		}

		// Drawing
		if (NSCheat::GetBool(SKeyBool::SKeyBool_ENABLED))
		{
			Renderer::GetInstance().BeginScene(); // BEGIN

			if (cheatEngine->GetIsMenu() != -1)
			{
				nEntity::GetOffset() = 0x10;
				for (size_t i = 0; i < 40; i++)
				{
					//DWORD entirtptr = Memory::FindDMAAddy(Module::GameAssembly + 0xA782D8, { 0x5C, 0x8, Entity::GetOffset(), 0x0 });
					if (cheatEngine->GetIsMenu() == -1) break;

					//if (!nEntity::GetEntity() || nEntity::GetTeam() == 2 || nEntity::GetDeath() == 5) continue;

					//if (offsets->GetGameMode() != 2 && offsets->GetGameMode() != 5 && offsets->GetGameMode() != 8)
					//	if (nEntity::GetTeam() == offsets->GetLTeam()) continue;

					//if (offsets->GetGameMode() == 5)
					//	if (nEntity::GetTeamZombie() == offsets->GetLTeamZombie())
					//		continue;

					//if (nEntity::GetCoords() == Vector3(0, 0, 0)) continue;
					//
					//if (nEntity::GetCoordsBones()[KeyBone::Head] == Vector3(0, 0, 0)) continue;
					//if (nEntity::GetCoordsBones()[KeyBone::Body] == Vector3(0, 0, 0)) continue;

					//Vector3 CoordsHead = nEntity::GetCoordsBones()[KeyBone::Head]; CoordsHead.y += 0.5f;
					//Vector3 Coords = nEntity::GetCoords(); Coords.y -= 2.0f; Coords.z -= 0.7f;

					//if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
					//if (!WorldToScreenOpenGL(CoordsHead, nEntity::GetSCoordsBones()[KeyBone::Head], offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
									
					// Drawing 3DBox
					if (NSCheat::GetBool(SKeyBool::SKeyBool_3DBOX))
					{
						//Renderer::GetInstance().Render3DBox(CoordsHead, Coords, 0, 1, 0, offsets->GetMatrix(), NSCheat::GetColor());
					}
					
					// Drawing 2DBox
					if (NSCheat::GetBool(SKeyBool::SKeyBool_2DBBOX))
					{
						//Renderer::GetInstance().Render2DBox(nEntity::GetSCoordsBones()[KeyBone::Head], nEntity::GetSCoords(), NSCheat::GetColor());
					}

					// Drawing Distance
					if (NSCheat::GetBool(SKeyBool::SKeyBool_DISTANCE))
					{
						//Vector3 Coords = nEntity::GetCoords(); Coords.y -= 2.4f;
						//if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
						//

						//std::string buffAsStdStr = std::to_string((int)Distance(offsets->GetCoords(), nEntity::GetCoords()));
						//Renderer::GetInstance().RenderText(buffAsStdStr+"m", nEntity::GetSCoords(), 12, NSCheat::GetColor(), true);
					}

					// Drawing Name
					if (NSCheat::GetBool(SKeyBool::SKeyBool_NAME))
					{
						//Vector3 Coords = nEntity::GetCoords(); Coords.y += 1.0f;
						//if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
						//Renderer::GetInstance().RenderName(nEntity::GetSCoords(), nEntity::GetName(), NSCheat::GetColor());
					}

					// Drawing Health
					if (NSCheat::GetBool(SKeyBool::SKeyBool_HEALTH))
					{
						//Vector3 Coords = nEntity::GetCoords(); Coords.y += 1.0f;
						//if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
						//Renderer::GetInstance().RenderHealth(nEntity::GetSCoords(), nEntity::GetHealth());
					}

					// Drawing Head Ponter
					{
						//if (!WorldToScreenOpenGL(nEntity::GetCoordsBones()[KeyBone::Head], nEntity::GetSCoordsBones()[KeyBone::Head], offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
						//Renderer::GetInstance().RenderCircleFilled(nEntity::GetSCoordsBones()[KeyBone::Head], 10, Color(255, 255, 0, 0), 10);
					}

					nEntity::GetOffset() += 0x4;
				}
			}
			Renderer::GetInstance().EndScene(); // END
		}

		// Rendering
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

void ConsoleSetup()
{
	// With this trick we'll be able to print content to the console, and if we have luck we could get information printed by the game.
	AllocConsole();
	SetConsoleTitle("[+] Hooking DirectX 11 by Niemand");
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	freopen("CONIN$", "r", stdin);

	std::cout << "\t[+] Offsets:" << std::endl;
}

DWORD WINAPI MainThread(HMODULE hModule)
{// Main поток

	//ConsoleSetup();

	bool init_hook = false;
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{
		kiero::bind(8, (void**)&oPresent, hkPresent);

		init_hook = true;
	}

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			g_IsCheat = false;

			FreeLibraryAndExitThread(hModule, 0);
		}
		Sleep(1);
	}
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//GetHWID();

		DisableThreadLibraryCalls(hMod);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hMod, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();

		SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::GetIO().Fonts = NULL;

		// Hooks
		//Memory::Patch((BYTE*)offsets->pattern[KeyPattern::Entity]->_address, (BYTE*)offsets->pattern[KeyPattern::Entity]->_pattern, 5);

		break;
	}
	return TRUE;
}
