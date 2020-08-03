
#include "includes.h"

#include "Renderer.h"
#include "Memory.h"
#include "SettingImgui.h"
#include "CheatEngine.h"
#include <Vectors.h>
#include <Hooks_reclass.h>
#include <SettingCheat.h>
#include <HWID.h>
#include <AimBot.h>
#include <nMath.h>
#include <KeyBind.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no colors needed


#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")
#pragma comment(lib, "msdmo.lib")
#pragma comment(lib, "Strmiids.lib")

#pragma execution_character_set("utf-8")

class RaycastHit
{
	Vector3 pos; // offset: 0x0
	DWORD id;	// id: 0x28
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// HOOKS
typedef bool(__cdecl* Raycast)(Vector3 pos, Vector3 dir, void* hit, float dist);
typedef int(__cdecl* sub_105CA9E0)(int a1);

// HOOKS Send
void* sendPTR;
GetServerTime oGetServerTime = nullptr;
WRITE_LONG oWRITE_LONG = nullptr;
WRITE_FLOAT oWRITE_FLOAT = nullptr;
BEGIN_WRITE oBEGIN_WRITE = nullptr;
WRITE_BYTE oWRITE_BYTE = nullptr;
END_WRITE oEND_WRITE = nullptr;
Send oSend = nullptr;
send_attack osend_attack = nullptr;

// HOOKS
Raycast oRaycast = nullptr;
sub_105CA9E0 osub_105CA9E0 = nullptr;

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


// AimBot
class AimBot
{
protected:
	AimBot() { angles_.resize(40); diffs_.resize(40); }
	~AimBot() {  }

	bool isActive_ = false;
	int id_;
	float minDiff_;
	std::vector<Vector3> angles_;
	Vector3 pos_;
	std::vector<float> diffs_;
	PlayerData* entsptr_;

public:
	
};

class AimBotDeffault : AimBot
{
protected:
	struct local
	{
		Vector3 pos;
		Vector3 angles;
	};
	local local;

	struct entity
	{
		Vector3 pos;
	};
	entity entity;

public:
	bool isTarget = false;
	bool isValiad = false;

	int getId();
	float getMinDiff();
	Vector3 getPos();

	void clear();
	void calcDiff(int i);
	void Target(Vector3 localAngle, Vector3 localPos, int id)
	{
		Vector3 bone = nEntity::GetCoordsBones(entsptrAD, id, 1);
		this->setLocalAngle(localAngle);
		this->setLocalPos(localPos);
		this->setEntityPos(bone);
		this->calcDiff(id);
	}

	void setIsActive(bool isActive);
	void setMinDiff(float minDiff);
	void setLocalPos(Vector3 pos);
	void setLocalAngle(Vector3 angle);
	void setEntityPos(Vector3 pos);

	float getFov();
	bool getIsActive();
	Vector3 getAngles();

	Vector3 getSmoothAngles(Vector3 entiytAng, Vector3 localAng)
	{
		return entiytAng - localAng;
	}

	PlayerData* getPlayerData();
};
AimBotDeffault* aimbotDeffault = new AimBotDeffault();


void AimBotDeffault::setIsActive(bool isActive)
{
	this->isActive_ = isActive;
}

void AimBotDeffault::clear()
{
	for (size_t i = 0; i < 40; i++)
	{
		this->diffs_[i] = 0;
		this->angles_[i] = Vector3();
	}
}

void AimBotDeffault::calcDiff(int i)
{
	this->diffs_[i] = Distance(CalcAngle(this->entity.pos, this->local.pos), this->local.angles);
	if (this->diffs_[i] < this->minDiff_) 
	{
		if (nEntity::GetDeath(entsptrAD) == 5)
		{
			aimbotDeffault->isValiad = false;
			return void();
		}
		if (cheatEngine->GetGameMode() != 2 && cheatEngine->GetGameMode() != 5 && cheatEngine->GetGameMode() != 8)
		{
			if (nEntity::GetTeam(entsptrAD) == cheatEngine->GetLTeam())
			{
				aimbotDeffault->isValiad = false;
				return void();
			}
		}

		if (cheatEngine->GetGameMode() == 5)
		{
			if (nEntity::GetTeamZombie(entsptrAD) == cheatEngine->GetLTeamZombie())
			{
				aimbotDeffault->isValiad = false;
				return void();
			}
		}

		aimbotDeffault->isValiad = true;

		this->id_ = i;
		this->pos_ = this->entity.pos;
		this->minDiff_ = this->diffs_[i];
		this->entsptr_ = entsptrAD;
		this->angles_[i] = CalcAngle(this->entity.pos, this->local.pos);
	}
}

void AimBotDeffault::setMinDiff(float minDiff)
{
	minDiff_ = minDiff;
}

void AimBotDeffault::setLocalPos(Vector3 pos)
{
	local.pos = pos;
}

void AimBotDeffault::setLocalAngle(Vector3 angles)
{
	local.angles = angles;
}

void AimBotDeffault::setEntityPos(Vector3 pos)
{
	entity.pos = pos;
}

float AimBotDeffault::getFov()
{
	return this->minDiff_;
}

bool AimBotDeffault::getIsActive()
{
	return this->isActive_;
}

Vector3 AimBotDeffault::getAngles()
{
	return this->angles_[this->getId()];
}

PlayerData* AimBotDeffault::getPlayerData()
{
	return this->entsptr_;
}

int AimBotDeffault::getId()
{
	return id_;
}

float AimBotDeffault::getMinDiff()
{
	return minDiff_;
}

Vector3 AimBotDeffault::getPos()
{
	return pos_;
}


ImVec2 hitSPos;
Vector3 hitPos;

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

KeyBind keyBind;
__forceinline LRESULT CALLBACK WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_IsCheat)
	{
		if (keyBind.getStart())
		{
			keyBind.setHandler(true);

			if (uMsg == WM_KEYUP || uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
			{
				keyBind.setKeyCode(wParam);
				keyBind.setKey(keyBind.getKeyCodes()[wParam]);
				keyBind.setStart(false);
				keyBind.setHandler(false);

				if (keyBind.getKeyCodes()[wParam] == "")
					keyBind.setKey("?");
			}

			if (uMsg == WM_KEYUP && wParam == VK_ESCAPE)
			{
				keyBind.setKeyCode(0x0);
				keyBind.setKey("No Key");
				keyBind.setStart(false);
				keyBind.setHandler(false);
			}			
		}

		if (uMsg == WM_KEYUP && wParam == VK_DELETE)
		{
			g_ShowImgui ^= 1;
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

bool ishkPresent = false;

char hitInfo2;
__forceinline bool __cdecl hkRaycast(Vector3 pos, Vector3 dir, void* hit, float dist)
{
	return oRaycast(pos, dir, hit, dist);
}
__forceinline DWORD WINAPI AimBotThread(HMODULE hModule)
{
	while (true)
	{
		static bool isMenu = false;
		if (ishkPresent == true && cheatEngine->GetIsMenu() != -1 && cheatEngine->GetLTeam() != 2)
		{
			if (isMenu == true) {
				isMenu = false;
				Sleep(500);
			}

			DWORD offset = 0x10;
			aimbotDeffault->setMinDiff(10000.0f);
			for (size_t i = 0; i < 40; i++)
			{
				if (cheatEngine->GetIsMenu() == -1) 
				{ 
					break;
				}

				DWORD entirtptr = Memory::FindDMAAddy(cheatEngine->GetAddress(KeyPattern::Pattern_PLH), { 0x5C, 0xC, offset, 0x0 });
				if (entirtptr != NULL) entsptrAD = (PlayerData*)entirtptr;
				else 
				{
					offset += 0x4; 
					continue;
				}

				if (nEntity::GetTeam(entsptr) == 2 || 
					nEntity::GetDeath(entsptrAD) == 5)
				{ 
					offset += 0x4; 
					continue;
				}

				if (cheatEngine->GetGameMode() != 2 && cheatEngine->GetGameMode() != 5 && cheatEngine->GetGameMode() != 8)
				{
					if (nEntity::GetTeam(entsptrAD) == cheatEngine->GetLTeam())
					{
						offset += 0x4;
						continue;
					}
				}

				if (cheatEngine->GetGameMode() == 5)
				{
					if (nEntity::GetTeamZombie(entsptrAD) == cheatEngine->GetLTeamZombie())
					{
						offset += 0x4; continue;
					}
				}

				// Aimbot
				{
					if (aimbotDeffault->getIsActive() && aimbotDeffault->isTarget)
						entsptrAD = aimbotDeffault->getPlayerData();

					Vector3 localAngle = cheatEngine->GetAngle();
					Vector3 localPos = cheatEngine->GetCoords();

					if (NSCheat::GetInt(SKeyInt::SKeyInt_BONE) == 0)
						aimbotDeffault->Target(localAngle, localPos, KeyBone::Bone_Head);

					if (NSCheat::GetInt(SKeyInt::SKeyInt_BONE) == 1)
						aimbotDeffault->Target(localAngle, localPos, KeyBone::Bone_Body);

					if (NSCheat::GetInt(SKeyInt::SKeyInt_BONE) == 2)
					{
						for (size_t b = 0; b < 2; b++)
						{
							if (aimbotDeffault->getIsActive() && aimbotDeffault->isTarget)
								if (aimbotDeffault->getId() != b) continue;
							
							aimbotDeffault->Target(localAngle, localPos, b);
						}
					}
	
					if (entirtptr != NULL) entsptrAD = (PlayerData*)entirtptr;
				}

				offset += 0x4;
			}

		
			if (NSCheat::GetBool(SKeyBool::SKeyBool_AIMBOT) && GetAsyncKeyState(keyBind.getKeyCode()) && 1)
			{
				if (aimbotDeffault->isValiad)
				{
					Vector3 smoothAngles;
					smoothAngles = cheatEngine->GetAngle();
					smoothAngles += Clamp(Norm(aimbotDeffault->getSmoothAngles(aimbotDeffault->getAngles(), cheatEngine->GetAngle()))) / (NSCheat::GetInt(SKeyInt::SKeyInt_SMOOTH) + 1);

					static uintptr_t dwAddr = NULL;
					static bool b = true;
					if (b) {
						dwAddr = Memory::FindDMAAddy(cheatEngine->GetAddress(KeyPattern::Pattern_Controll), cheatEngine->GetOffset.Local.Angle);
						b = false;
					}

					if (aimbotDeffault->getFov() < NSCheat::GetInt(SKeyInt::SKeyInt_FOV) || aimbotDeffault->getFov() < -NSCheat::GetInt(SKeyInt::SKeyInt_FOV))
					{
						*(Vector3*)dwAddr = smoothAngles;

						aimbotDeffault->isTarget = true;
					}
					else if (aimbotDeffault->isTarget)
					{
						*(Vector3*)dwAddr = smoothAngles;
					}
					aimbotDeffault->setIsActive(true);
				}
				else 
				{
					aimbotDeffault->isTarget = false;
					aimbotDeffault->setIsActive(false);
				}
			}
			else 
			{
				aimbotDeffault->isTarget = false;
				aimbotDeffault->isValiad = false;
				aimbotDeffault->setIsActive(false);
			}
		}
		else {
			isMenu = true;
			aimbotDeffault->setIsActive(false);
		}

		Sleep(1);
	}
}

__forceinline HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{// Игровой поток

	

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

		ishkPresent = true;
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

			
			{
				static bool alpha_preview = true;
				static bool alpha_half_preview = false;
				static bool drag_and_drop = true;
				static bool options_menu = true;
				static bool hdr = false;
				ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

				// ESP
				{
					static const char* Coloritems[] = { "WHITE", "YELLOW", "RED", "GREEN", "BLUE", "ORANGE" };

					ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f));
					ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));

					ImGui::Begin("ESP", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

					ImGui::Checkbox("Enable", &NSCheat::GetBool(SKeyBool::SKeyBool_ESP));
					ImGui::Checkbox("2DBox", &NSCheat::GetBool(SKeyBool::SKeyBool_2DBBOX));
					ImGui::Checkbox("3DBox", &NSCheat::GetBool(SKeyBool::SKeyBool_3DBOX));
					ImGui::Checkbox("Name", &NSCheat::GetBool(SKeyBool::SKeyBool_NAME));
					ImGui::Checkbox("Health", &NSCheat::GetBool(SKeyBool::SKeyBool_HEALTH));
					ImGui::Checkbox("Distance", &NSCheat::GetBool(SKeyBool::SKeyBool_DISTANCE));
			
					ImGui::Combo("Color", &NSCheat::GetInt(SKeyInt::SKeyInt_COLOR), Coloritems, IM_ARRAYSIZE(Coloritems));

					ImGui::End();
				}
			
				// AimBot
				{
					ImGui::SetNextWindowPos(ImVec2(220.0f, 10.0f));
					ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));

					// State
					static const char*	Bone_items[] = { "HEAD", "BODY", "HEAD+BODY" };

					static int	Fov_min = 0, Fov_max = 360;
					static int	Smooth_min = 0, Smooth_max = 100;

					ImGui::Begin("AimBot", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

					ImGui::Checkbox("Enable", &NSCheat::GetBool(SKeyBool::SKeyBool_AIMBOT));


					if (ImGui::Button(("Aim Key [ " + keyBind.getKey() + " ]").c_str())) // Deffault: Mouse Left, No Key
					{
						if (!keyBind.getHandler()) keyBind.Start();
					}

					ImGui::SliderScalar("Smooth", ImGuiDataType_S16, &NSCheat::GetInt(SKeyInt::SKeyInt_SMOOTH), &Smooth_min, &Smooth_max, "%d");
					ImGui::SliderScalar("Fov", ImGuiDataType_S16, &NSCheat::GetInt(SKeyInt::SKeyInt_FOV), &Fov_min, &Fov_max, "%d");
					
					ImGui::Combo("Bone", &NSCheat::GetInt(SKeyInt::SKeyInt_BONE), Bone_items, IM_ARRAYSIZE(Bone_items));
					

					ImGui::End();
				}
			}
			
		}

		// Drawing
		if (NSCheat::GetBool(SKeyBool::SKeyBool_ESP))
		{
			Renderer::GetInstance().BeginScene(); // BEGIN

			if (cheatEngine->GetIsMenu() != -1)
			{
				nEntity::GetOffset() = 0x10;
				for (size_t i = 0; i < 40; i++)
				{
					if (cheatEngine->GetIsMenu() == -1) { break; }

					DWORD entirtptr = Memory::FindDMAAddy(cheatEngine->GetAddress(KeyPattern::Pattern_PLH), { 0x5C, 0xC, nEntity::GetOffset(), 0x0 });
					if (entirtptr != NULL) entsptr = (PlayerData*)entirtptr;
					else { nEntity::GetOffset() += 0x4; continue; }

					if (nEntity::GetTeam(entsptr) == 2 ||
						nEntity::GetDeath(entsptr) == 5 ||
						nEntity::GetDeath(entsptr) == -1) { nEntity::GetOffset() += 0x4; continue; }

					if (cheatEngine->GetGameMode() != 2 && cheatEngine->GetGameMode() != 5 && cheatEngine->GetGameMode() != 8)
						if (nEntity::GetTeam(entsptr) == cheatEngine->GetLTeam()) { nEntity::GetOffset() += 0x4; continue; }

					if (cheatEngine->GetGameMode() == 5)
						if (nEntity::GetTeamZombie(entsptr) == cheatEngine->GetLTeamZombie()) { nEntity::GetOffset() += 0x4; continue; }

					Vector3 Coords = nEntity::GetCoords(entsptr); Coords.y -= 2.0f; Coords.z -= 0.7f;
					Vector3 CoordsHead = nEntity::GetCoordsBones(entsptr, KeyBone::Bone_Head, 0); CoordsHead.y += 0.5f;


					if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), cheatEngine->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) { nEntity::GetOffset() += 0x4; continue; }
					if (!WorldToScreenOpenGL(CoordsHead, nEntity::GetSCoordsBones(KeyBone::Bone_Head), cheatEngine->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) { nEntity::GetOffset() += 0x4; continue; }
									
					// Drawing 3DBox
					if (NSCheat::GetBool(SKeyBool::SKeyBool_3DBOX))
					{
						Renderer::GetInstance().Render3DBox(CoordsHead, Coords, 0, 1, 0, cheatEngine->GetMatrix(), NSCheat::GetColor(NSCheat::GetInt(SKeyInt::SKeyInt_COLOR)));
					}
					
					// Drawing 2DBox
					if (NSCheat::GetBool(SKeyBool::SKeyBool_2DBBOX))
					{
						Renderer::GetInstance().Render2DBox(nEntity::GetSCoordsBones(KeyBone::Bone_Head), nEntity::GetSCoords(), NSCheat::GetColor(NSCheat::GetInt(SKeyInt::SKeyInt_COLOR)));
					}

					// Drawing Distance
					if (NSCheat::GetBool(SKeyBool::SKeyBool_DISTANCE))
					{
						Vector3 Coords = nEntity::GetCoords(entsptr); Coords.y -= 2.4f;
						if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), cheatEngine->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) { nEntity::GetOffset() += 0x4; continue; }

						std::string buffAsStdStr = std::to_string((int)Distance(cheatEngine->GetCoords(), nEntity::GetCoords(entsptr)));
						Renderer::GetInstance().RenderText(buffAsStdStr+"m", nEntity::GetSCoords(), 12, NSCheat::GetColor(NSCheat::GetInt(SKeyInt::SKeyInt_COLOR)), true);
					}

					// Drawing Name
					if (NSCheat::GetBool(SKeyBool::SKeyBool_NAME))
					{
						Vector3 Coords = nEntity::GetCoords(entsptr); Coords.y += 1.0f;
						if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), cheatEngine->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) { nEntity::GetOffset() += 0x4; continue; }
						Renderer::GetInstance().RenderName(nEntity::GetSCoords(), nEntity::GetName(entsptr), NSCheat::GetColor(NSCheat::GetInt(SKeyInt::SKeyInt_COLOR)));
					}

					// Drawing Health
					if (NSCheat::GetBool(SKeyBool::SKeyBool_HEALTH))
					{
						Vector3 Coords = nEntity::GetCoords(entsptr); Coords.y += 1.0f;
						if (!WorldToScreenOpenGL(Coords, nEntity::GetSCoords(), cheatEngine->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) { nEntity::GetOffset() += 0x4; continue; }
						Renderer::GetInstance().RenderHealth(nEntity::GetSCoords(), nEntity::GetHealth(entsptr));
					}

					// Drawing Head Ponter
					{
						//if (!WorldToScreenOpenGL(nEntity::GetCoordsBones()[KeyBone::Head], nEntity::GetSCoordsBones()[KeyBone::Head], offsets->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) continue;
						//Renderer::GetInstance().RenderCircleFilled(nEntity::GetSCoordsBones()[KeyBone::Head], 10, Color(255, 255, 0, 0), 10);
					}

					nEntity::GetOffset() += 0x4;
				}


				/*ImVec2 spos;
				if (WorldToScreenOpenGL(aimbotDeffault->getPos(), spos, cheatEngine->GetMatrix(), Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()))
					if (aimbotDeffault->getFov() < NSCheat::GetInt(SKeyInt::SKeyInt_FOV) || aimbotDeffault->getFov() < -NSCheat::GetInt(SKeyInt::SKeyInt_FOV)) Renderer::GetInstance().RenderLine(ImVec2(Renderer::GetInstance().GetWidth() / 2, Renderer::GetInstance().GetHeight() / 2), spos, Color(255, 255, 255, 0));*/

				Renderer::GetInstance().RenderCircle(ImVec2(Renderer::GetInstance().GetWidth() / 2, Renderer::GetInstance().GetHeight() / 2), (NSCheat::GetInt(SKeyInt::SKeyInt_FOV)*10), Color(255, 255, 0, 0), 1, 50);
			}

			Renderer::GetInstance().EndScene(); // END
		}

		// Rendering
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	//system("cls");

	//void* hitInfo = 0;
	//if (oRaycast(cheatEngine->GetCoords(), cheatEngine->GetDir(), &hitInfo, 1024.00f) == true)
	//{

	//}
	//spdlog::info(cheatEngine->GetDir().x);
	//spdlog::info(cheatEngine->GetDir().y);
	//spdlog::info(cheatEngine->GetDir().z);
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

__forceinline void __cdecl hksend_attack(int thisClient, Vector3 pos, int time, int AttackData)
{
	//if(sendPTR == nullptr) return osend_attack(thisClient, pos, time, AttackData);
	//
	//oBEGIN_WRITE(0xF5, 0x4, 0);

	//// PosLocalPlayer
	//oWRITE_FLOAT(nEntity::GetCoords().x); // x
	//oWRITE_FLOAT(nEntity::GetCoords().y); // y
	//oWRITE_FLOAT(nEntity::GetCoords().z); // z
	//oWRITE_LONG(oGetServerTime(0)); // time oGetServerTime()

	//oWRITE_BYTE(0, 0); // vid
	//oWRITE_BYTE(1, 0); // hitbox

	//// PosHitBox
	//oWRITE_FLOAT(nEntity::GetCoords().x); // x
	//oWRITE_FLOAT(nEntity::GetCoords().y); // y
	//oWRITE_FLOAT(nEntity::GetCoords().z); // z

	//system("cls");
	//std::cout << "[+] Time: " << time << std::endl;

	//std::cout << "\n" << time << std::endl;

	//std::cout << "[+] sendPTR: " << sendPTR << std::endl;
	//std::cout << "[+] X: " << cheatEngine->GetCoords().x << std::endl;
	//std::cout << "[+] Y: " << cheatEngine->GetCoords().y << std::endl;
	//std::cout << "[+] Z: " << cheatEngine->GetCoords().z << std::endl;
	//std::cout << "[+] Time: " << oGetServerTime(0) << std::endl;
	//std::cout << "[+] X: " << nEntity::GetCoords().x << std::endl;
	//std::cout << "[+] Y: " << nEntity::GetCoords().y << std::endl;
	//std::cout << "[+] Z: " << nEntity::GetCoords().z << std::endl;

	//oEND_WRITE(0);

	//oSend(sendPTR);


	return osend_attack(thisClient, pos, time, AttackData);
}



__forceinline bool __cdecl hksub_105CA9E0(int a1)
{
	return osub_105CA9E0(a1);
}

__forceinline void ConsoleSetup()
{
	// With this trick we'll be able to print content to the console, and if we have luck we could get information printed by the game.
	AllocConsole();
	SetConsoleTitle("[+] Hooking DirectX 11 by Niemand");
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	freopen("CONIN$", "r", stdin);
}

__forceinline DWORD WINAPI MainThread(HMODULE hModule)
{// Main поток

	//ConsoleSetup();

	{
		DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x6FB700;
		oRaycast = (Raycast)bAddr;
	}

	{
		//DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x6FB700;
		//oRaycast = (Raycast)TrampHook32((BYTE*)bAddr, (BYTE*)hkRaycast, 10);
	}

	{
		//DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x5CA9E0;
		//osub_105CA9E0 = (sub_105CA9E0)TrampHook32((BYTE*)bAddr, (BYTE*)hksub_105CA9E0, 10);
	}

	//Use the default logger (stdout, multi-threaded, colored)
	//spdlog::info("info, {}!", "World");
	//spdlog::error("error, {}!", "World");
	//spdlog::trace("trace, {}!", "World");
	//spdlog::debug("debug, {}!", "World");
	//spdlog::warn("warn, {}!", "World");
	//spdlog::critical("critical, {}!", "World");

	//int crash = *(volatile int*)nullptr;

	NSCheat::Initialize();
	cheatEngine->Initialize();
	nEntity::Initialize();
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

	//DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x6D8F30;
	//oUpdate = (_Update)TrampHook32((BYTE*)bAddr, (BYTE*)hkUpdate, 11);

	//{
	//	DWORD b1 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37EC45; // push edi
	//	DWORD b2 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37EC4E; // push esi
	//	DWORD b3 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37EC4F; // call GameAssembly.dll + 0x378860
	//	DWORD b4 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37EC54; // add esp,08
	//	Memory::Nop((BYTE*)b1, 1); // push edi
	//	Memory::Nop((BYTE*)b2, 1); // push esi
	//	Memory::Nop((BYTE*)b3, 5); // call GameAssembly.dll + 0x378860	
	//	Memory::Nop((BYTE*)b4, 3); // add esp,08

	//	DWORD b5 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37ECC3; // push 00
	//	DWORD b6 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37ECC9; // push esi
	//	DWORD b7 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37ECCA; // call GameAssembly.dll + 0x378F70
	//	DWORD b8 = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x37ECCF; // add esp,08
	//	Memory::Nop((BYTE*)b5, 2); // push 00
	//	Memory::Nop((BYTE*)b6, 1); // push esi
	//	Memory::Nop((BYTE*)b7, 5); // call GameAssembly.dll + 0x378F70
	//	Memory::Nop((BYTE*)b8, 3); // add esp,08
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x6F7560;
	//	oGetServerTime = (GetServerTime)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x63E2F0;
	//	oWRITE_LONG = (WRITE_LONG)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x63E0B0;
	//	oWRITE_FLOAT = (WRITE_FLOAT)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x63CFB0;
	//	oBEGIN_WRITE = (BEGIN_WRITE)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x63DFE0;
	//	oWRITE_BYTE = (WRITE_BYTE)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x63D250;
	//	oEND_WRITE = (END_WRITE)bAddr;
	//}

	//{
	//	DWORD bAddr = Memory::FindDMAAddy((DWORD)GetModuleHandle("GameAssembly.dll") + 0xB358A0, { 0x5C, 0x0, 0x10, 0x0 });
	//	sendPTR = (void*)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x5E5A30;
	//	oSend = (Send)bAddr;
	//}

	//{
	//	DWORD bAddr = (DWORD)GetModuleHandle("GameAssembly.dll") + 0x5EDE30;
	//	osend_attack = (send_attack)TrampHook32((BYTE*)bAddr, (BYTE*)hksend_attack, 10);
	//}

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

__forceinline BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		GetHWID();

		DisableThreadLibraryCalls(hMod);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hMod, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AimBotThread, hMod, NULL, NULL);
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
