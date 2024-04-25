/**
 * @file FPSFixed.cpp
 * @brief FPSに関するクラス
 * @author Yamashita Tubasa
 */

#include "FPSFixed.h"

#include <windows.h>
#include "WinApp.h"

void MyEngine::FPSFixed::InitializeFixFPS() {
	// 現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void MyEngine::FPSFixed::UpdateFixFPS() {
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在の時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小名スリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void MyEngine::FPSFixed::CountFPS()
{
	static DWORD p_t = timeGetTime(); // 前回の時間
	static int frame_count = 0;       // フレームカウント
	DWORD now_time = timeGetTime();   // 今回のフレームの時間

	frame_count++; // フレーム数をカウントする

	// 経過時間が１秒を超えたらカウントと時間をリセット
	if (now_time - p_t >= 1000) {
		p_t = now_time;
		frame_count = 0;
	}
	/*if (frame_count == 35) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"35");
	}
	if (frame_count == 36) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"36");
	}
	if (frame_count == 37) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"37");
	}
	if (frame_count == 38) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"38");
	}
	if (frame_count == 39) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"39");
	}
	if (frame_count == 40) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"40");
	}
	if (frame_count == 41) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"41");
	}
	if (frame_count == 42) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"42");
	}
	if (frame_count == 43) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"43");
	}
	if (frame_count == 44) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"44");
	}
	if (frame_count == 45) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"45");
	}
	if (frame_count == 46) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"46");
	}
	if (frame_count == 47) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"47");
	}
	if (frame_count == 48) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"48");
	}
	if (frame_count == 49) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"49");
	}
	if (frame_count == 50) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"50");
	}
	if (frame_count > 50) {
		SetWindowText(WinApp::GetInstance()->GetHwnd(), L"60");
	}*/
}
