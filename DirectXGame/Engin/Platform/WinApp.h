/**
 * @file WinApp.h
 * @brief WindowsAPIに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#pragma warning(push)
#pragma warning(disable:4668)
#pragma warning(disable:5039)
#include <Windows.h>
#pragma warning(pop)

class WinApp final
{
public: // 静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	// WindowsAPIの初期化
	void Initialize();

	// 終了
	void Finalize();

	// メッセージの処理
	bool ProcessMessage();

public: // Getter
	[[maybe_unused]] HWND GetHwnd() const { return hwnd_; }
	[[maybe_unused]] HINSTANCE GetHInstance() const { return wc_.hInstance; }

public:
	// インスタンスの取得
	static WinApp* GetInstance();

private:
	// コンストラクタ
	[[maybe_unused]] WinApp() {};
	// デストラクタ
	~WinApp() = default;
	// コピーコンストラクタの禁止
	WinApp(const WinApp&) = delete;
	// 代入演算子の禁止
	WinApp& operator=(const WinApp&) = delete;

public: // 定数
	// ウィンドウ横幅
	static const int window_width = 1280;
	// ウィンドウ縦幅
	static const int window_height = 720;

private:
	// ウィンドウハンドル
	HWND hwnd_ = nullptr;
	// ウィンドウクラスの設定
	WNDCLASSEX wc_{};
};