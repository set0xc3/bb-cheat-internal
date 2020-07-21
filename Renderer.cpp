
#include <includes.h>
#include "Renderer.h"

#include <Fonts/Font.h>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool WorldToScreenOpenGL(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight)
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


void Renderer::Initialize(ID3D11DeviceContext* pContext)
{
	this->pContext = pContext;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromMemoryTTF(g_fFont, sizeof(g_fFont), 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	m_pFont = io.Fonts->AddFontFromMemoryTTF(g_fFont, sizeof(g_fFont), 32.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

void Renderer::BeginScene()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
}

void Renderer::EndScene()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

float Renderer::RenderText(const std::string& text, const ImVec2& position, float size, Color color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = color.A & 0xff;
	float r = color.R & 0xff;
	float g = color.G & 0xff;
	float b = color.B & 0xff;

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			//window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			//window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			//window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->DrawList->AddText(m_pFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}
		else
		{
			window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->DrawList->AddText(m_pFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

float Renderer::GetWidth()
{
	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->pContext->RSGetViewports(&nvp, &vp);
	return vp.Width;
}

float Renderer::GetHeight()
{
	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->pContext->RSGetViewports(&nvp, &vp);
	return vp.Height;
}

void Renderer::RenderLine(const ImVec2& from, const ImVec2& to, Color color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = color.A & 0xff;
	float r = color.R & 0xff;
	float g = color.G & 0xff;
	float b = color.B & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
}

void Renderer::RenderCircle(const ImVec2& position, float radius, Color color, float thickness, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = color.A & 0xff;
	float r = color.R & 0xff;
	float g = color.G & 0xff;
	float b = color.B & 0xff;

	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
}

void Renderer::RenderCircleFilled(const ImVec2& position, float radius, Color color, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = color.A & 0xff;
	float r = color.R & 0xff;
	float g = color.G & 0xff;
	float b = color.B & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
}

void Renderer::RenderRect(const ImVec2& from, const ImVec2& to, Color color, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = color.A & 0xff;
	float r = color.R & 0xff;
	float g = color.G & 0xff;
	float b = color.B & 0xff;

	window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
}

void Renderer::RenderRectFilled(const ImVec2& from, const ImVec2& to, Color color, float rounding, uint32_t roundingCornersFlags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = color.A & 0xff;
	float r = color.R & 0xff;
	float g = color.G & 0xff;
	float b = color.B & 0xff;

	window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
}

void Renderer::RenderName(const ImVec2& bot, const std::string name, Color color)
{
	ImVec2 t1;
	t1.x = bot.x;
	t1.y = bot.y - 6.0f;
	Renderer::GetInstance().RenderText(name, t1, 10, color, true);
}

void Renderer::RenderHealth(const ImVec2& bot, int health)
{	
	// Inside - bg
	{
		ImVec2 t1, tr;
		t1.x = bot.x - 12.0f;
		t1.y = bot.y - 6.0f;

		tr.x = bot.x + 12.0f;
		tr.y = bot.y - 6.0f - 2.0f;

		RenderRectFilled(t1, tr, Color(255 / 2, 0, 0, 0)); // top
	}

	// Health - bg //
	{
		ImVec2 t1, tr;

		float t = 1.0f - (min(max(100, 0), health) / 100.0f);

		t1.x = bot.x - 12.0f;
		t1.y = bot.y - 6.0f;

		int background_height = (bot.x - 12) - (bot.x + 12);
		int health_bar_height = background_height * t;

		tr.x = bot.x - 12.0f + (health_bar_height - background_height);
		tr.y = bot.y - 6.0f - 2.0f;

		RenderRectFilled(t1, tr, Color(255, 0, 255, 0)); // top
	}

	// Outside
	{
		ImVec2 t1, tr;
		t1.x = bot.x - 12.0f;
		t1.y = bot.y - 6.0f;

		tr.x = bot.x + 12.0f;
		tr.y = bot.y - 6.0f - 2.0f;

		RenderRect(t1, tr, Color(255, 0, 0, 0)); // top
	}
}

void Renderer::Render2DBox(const ImVec2& top, const ImVec2& bot, Color color, float thickness)
{
	// #1
	{
		float Width = ABS(bot.y - top.y) / 3;
		
		ImVec2 t1, tr;
		t1.x = top.x - Width;
		tr.x = top.x + Width;
		t1.y = tr.y = top.y;

		ImVec2 b1, br;
		b1.x = top.x - Width;
		br.x = top.x + Width;
		b1.y = br.y = bot.y;

		//RenderRect(t1, br, color, 10);

		RenderLine(t1, tr, color, thickness); // top
		RenderLine(b1, br, color, thickness); // bottom
		RenderLine(b1, t1, color, thickness); // left
		RenderLine(br, tr, color, thickness); // right
	}

	// #2
	{
		//int Height = ABS(bot.y - top.y) / 3;
		//ImVec2 t1, tr;
		//t1.x = top.x - Height;
		//tr.x = top.x + Height;
		//t1.y = tr.y = top.y;

		//ImVec2 b1, br;
		//b1.x = bot.x - Height;
		//br.x = bot.x + Height;
		//b1.y = br.y = bot.y;

		//RenderLine(t1, tr, color, thickness);
		//RenderLine(b1, br, color, thickness);
		//RenderLine(t1, b1, color, thickness);
		//RenderLine(tr, br, color, thickness);
	}
}

ImVec2 ToImVec2(Vector3 vector3)
{
	ImVec2 imVec2;
	imVec2.x = vector3.x;
	imVec2.y = vector3.y;

	return imVec2;
}

void Renderer::Render3DBox(Vector3 top, Vector3 bot, int a, int width, int height, float viewMatrix[16], Color color)
{
	float box = ABS(top.y - bot.y);

	float z3D = 1.4f;

	Vector3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;


	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.y = bot.y + (sin(TORAD(a + 45)) * height) + box;

	b2.x = bot.x + (cos(TORAD(a + 135)) * width);
	b2.y = bot.y + (sin(TORAD(a + 135)) * height) + box;

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.y = bot.y + (sin(TORAD(a + 225)) * height);

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.y = bot.y + (sin(TORAD(a + 315)) * height);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + z3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + z3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + z3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + z3D;

	Vector3 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	if (WorldToScreenOpenGL(b1, b1_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(b2, b2_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(b3, b3_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(b4, b4_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(t1, t1_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(t2, t2_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(t3, t3_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight()) && 
		WorldToScreenOpenGL(t4, t4_2, viewMatrix, Renderer::GetInstance().GetWidth(), Renderer::GetInstance().GetHeight())) {

		// col
		RenderLine(ToImVec2(t1_2), ToImVec2(b1_2), color);
		RenderLine(ToImVec2(t2_2), ToImVec2(b2_2), color);
		RenderLine(ToImVec2(t3_2), ToImVec2(b3_2), color);
		RenderLine(ToImVec2(t4_2), ToImVec2(b4_2), color);

		// top base
		RenderLine(ToImVec2(t1_2), ToImVec2(t2_2), color);
		RenderLine(ToImVec2(t2_2), ToImVec2(t3_2), color);
		RenderLine(ToImVec2(t3_2), ToImVec2(t4_2), color);
		RenderLine(ToImVec2(t4_2), ToImVec2(t1_2), color);

		// bottom base
		RenderLine(ToImVec2(b1_2), ToImVec2(b2_2), color);
		RenderLine(ToImVec2(b2_2), ToImVec2(b3_2), color);
		RenderLine(ToImVec2(b3_2), ToImVec2(b4_2), color);
		RenderLine(ToImVec2(b4_2), ToImVec2(b1_2), color);
	}							
}

//void Renderer::RenderImage(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, Color color)
//{
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//
//	float a = color.A & 0xff;
//	float r = color.R & 0xff;
//	float g = color.G & 0xff;
//	float b = color.B & 0xff;
//
//	window->DrawList->AddImage(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
//}
//
//void Renderer::RenderImageRounded(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, Color color, float rounding, uint32_t roundingCornersFlags)
//{
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//
//	float a = color.A & 0xff;
//	float r = color.R & 0xff;
//	float g = color.G & 0xff;
//	float b = color.B & 0xff;
//
//	window->DrawList->AddImageRounded(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
//}


