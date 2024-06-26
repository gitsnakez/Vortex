/// Copyright (C) 2024 Roman Sivkov - All Rights Reserved.
/// You may use, distribute and modify this code under the
/// terms of the MIT License
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files(the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions :
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// For more information contact snakezfortress04@gmail.com

#pragma once

#include "api.h"
#include <Windows.h>
#include "Level.h"
#include "Model.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Matrix4x4.h"
#include "Shader.h"
#include "Prerequisites.h"
#include <future>
#include <thread>
#include <atomic>

EXTERN API void OnUpdateViewport(Viewport* vp, float deltaTime);
EXTERN API void UpdateMovementParameters(Viewport* vp, float forward, float rightward);
EXTERN API void UpdateRotateParameters(Viewport* vp, float camx, float camy);
EXTERN API void SetFullscreenMode(Viewport* vp, bool isFullScr, int winWidth, int winHeight);
EXTERN API void SetClientName(Viewport* vp, std::wstring client);
EXTERN API void LoadLevelAPI(LPCWSTR filename, Viewport* vp);

EXTERN API class Viewport
{
public:
	API Viewport(void* hWnd, void* engine, bool isBorder);
	API ~Viewport();

	// Scene
	void Update();
	void Render();
	void UpdateCamera();
	void UpdateLight();

	// Level
	void LoadLevel(LPCWSTR filename);
	void UpdateLevel();
	API void UnloadLevel();

	API void ShowGraph(const int* value);
	void DrawGraph();
	void DrawBuildInfo();
	API void ShowPos(const int* value);
	void DrawPos();
	API void ShowHud(const int* value);
	void DrawHud();

	API void ReloadAssets();

	API void GoToZeroPos();
	API void SetWireframeMode(bool value);

	API void SetVSync(bool value);

	// Inherited via Window
	API void OnCreate();
	API void OnUpdate();
	API void OnResize(int width, int height);
	API void OnChangeLocation(int x, int y);
	API void OnDestroy();
	API void OnEnter();
	API void OnLeave();

	//API
	void RecreateSwapChain();

#ifdef _USRDLL

	UINT winw;
	UINT winh;

	UINT fullScr_winw;
	UINT fullScr_winh;
#endif

	UINT normal_winw;
	UINT normal_winh;

	bool m_vsync = true;

public:
	HWND _hWnd;
	GraphicsEngine* _enginePtr = nullptr;
	SwapChainPtr m_swap_chain;
	IndexBufferPtr m_ib;

	bool m_is_running = true;
	#if _STEAM
	std::wstring* ClientName = new std::wstring(L"NULL");
	#endif

	LevelPtr _level;
#pragma region GUI Text
	FontPtr _pGeneralFont;
	FontPtr _pHudFont;
	FontPtr _pWatermarkFont;
#pragma endregion

	Vector4D m_light_position;
	
	bool is_play = false;
	bool keyz = false;
	bool is_fullscr = false;
	bool keyflscr = false;

	#if _DEBUG
	bool isShowGraph = true;
	bool isShowPos = false;
	bool isShowHud = false;
	#elif NDEBUG
	bool isShowGraph = false;
	bool isShowPos = false;
	bool isShowHud = true;
	#endif

	std::thread *levelLoadThread;
	std::future<void> *levelLoadFuture;
	float m_scale = 1;

	float m_delta_time = 0;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_speed = 1;

	float m_vertical_constraint = 1.5f;

	float world_constraint = 50;

	float cam_x;
	float cam_y;

	float m_light_rot_y = 0;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	float m_sensitivity = 0.1f;

	float m_time = 0.0f;
	float m_light_radius = 4.0f;
	float m_attenuation = 2.0f;
	bool m_anim = true;

	Matrix4x4 m_world_camera;
	Matrix4x4 m_view_camera;
	Matrix4x4 m_proj_camera;
};