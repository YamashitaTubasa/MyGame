#pragma once

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#define MaxCountrollers 4  
#define MaxVibration 65535

#include "WinApp.h"

#include <windows.h>
#include <wrl.h>
#include <dinput.h>
#include <dwrite.h>
#include <wchar.h>
#include <wrl/client.h>
#include <random>
#include <xinput.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment (lib, "xinput.lib")

/// <summary>
/// 入力
/// </summary>
class Input final
{
public:
	struct CountrolerState
	{
		XINPUT_STATE state; // コントローラーの状態の取得
		XINPUT_VIBRATION vibration;  // バイブレーション
		//bool Connected;
	};
	CountrolerState GamePad;

public:
	// namespace
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	// 初期化
	void Initialize(WinApp* winApp);

	// 更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// </param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <reutrns>トリガーか</params>
	bool TriggerKey(BYTE keyNumber);

public:
	// インスタンスの取得
	static Input* GetInstance();

private:
	// コンストラクタ
	Input() = default;
	// デストラクタ
	~Input() = default;
	// コピーコンストラクタの禁止
	Input(const Input&) = delete;
	// 代入演算子の禁止
	Input& operator=(const Input&) = delete;

private: // メンバ変数
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;
	// 全キーの状態
	BYTE key[256] = {};
	// 前回の全キーの状態
	BYTE keyPre[256] = {};
	// WindowsAPI
	WinApp* winApp = nullptr;
};