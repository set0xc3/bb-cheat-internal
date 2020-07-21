#pragma once

#include <includes.h>

#include "Common.h"
#include <imgui.h>
#include <Vectors.h>

typedef unsigned char uchar;
#pragma warning(disable : 4244)

#define ABS(x) ((x < 0) ? (-x) : (x))

#define TORAD(x) ((x) * 0.01745329252)

class Color
{
public:
	uchar A, R, G, B;

	Color()
	{
		A = R = G = B = 0;
	}

	Color(uchar A, uchar R, uchar G, uchar B)
	{
		this->A = A;
		this->R = R;
		this->G = G;
		this->B = B;
	}
};

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;

private:
	ID3D11DeviceContext* pContext;

protected:
	Renderer();
	~Renderer();

public:
	void Initialize(ID3D11DeviceContext* pContext);

	void BeginScene();
	void EndScene();

	float RenderText(const std::string& text, const ImVec2& position, float size, Color color, bool center = false);
	float GetWidth();
	float GetHeight();

	void RenderLine(const ImVec2& from, const ImVec2& to, Color color, float thickness = 1.0f);
	void RenderCircle(const ImVec2& position, float radius, Color color, float thickness = 1.0f, uint32_t segments = 16);
	void RenderCircleFilled(const ImVec2& position, float radius, Color color, uint32_t segments = 16);
	void RenderRect(const ImVec2& from, const ImVec2& to, Color color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	void RenderRectFilled(const ImVec2& from, const ImVec2& to, Color color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);
	void RenderName(const ImVec2& bot, const std::string name, Color color);
	void RenderHealth(const ImVec2& bot, int health);
	void Render2DBox(const ImVec2& top, const ImVec2& bot, Color color, float thickness = 1.0f);
	void Render3DBox(Vector3 top, Vector3 bot, int a, int width, int height, float viewMatrix[16], Color color);
	
	//void RenderImage(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, Color color);
	//void RenderImageRounded(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, Color color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);

private:
	ImFont* m_pFont;
};

#define g_sRenderer Renderer::GetInstance()