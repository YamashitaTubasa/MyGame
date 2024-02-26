/**
 * @file WinApp.cpp
 * @brief WindowsAPIに関するクラス
 * @author Yamashita Tubasa
 */

#include "WinApp.h"

#pragma warning(push)
#pragma warning(disable:4514)
#pragma warning(disable:4820)
#include <imgui_impl_win32.h>
#pragma warning(pop)

#pragma comment(lib,"winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

MyEngine::WinApp* MyEngine::WinApp::GetInstance()
{
	static WinApp instance;

	return &instance;
}

LRESULT MyEngine::WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	// メッセージで分岐
	switch (msg) {
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

void MyEngine::WinApp::Initialize()
{
	// ウィンドウクラスの設定
	//WNDCLASSEX w{};
	wc_.cbSize = sizeof(WNDCLASSEX);
	wc_.lpfnWndProc = (WNDPROC)WindowProc;    // ウィンドウプロシージャを設定
	wc_.lpszClassName = L"DirectXGame";       // ウィンドクラス名
	wc_.hInstance = GetModuleHandle(nullptr); // ウィンドハンドル
	wc_.hCursor = LoadCursor(NULL, IDC_ARROW);// カーソル指定

	// ウィンドクラスをOSに登録する
	RegisterClassEx(&wc_);
	// ウィンドサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc = { 0, 0, window_width, window_height };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(wc_.lpszClassName, // クラス名
		L"Annihilate",        // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,  // 標準的なウィンドウスタイル
		CW_USEDEFAULT,        // 標準X座標 (05に任せる)
		CW_USEDEFAULT,        // 標準Y座標 (05に任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr);

	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

void MyEngine::WinApp::Finalize()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wc_.lpszClassName, wc_.hInstance);
}

bool MyEngine::WinApp::ProcessMessage()
{
	MSG msg{};// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); //キー入力メッセージの処理
		DispatchMessage(&msg);  //プロシージャーにメッセージを送る
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}