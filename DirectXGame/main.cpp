/**
 * @file main.cpp
 * @brief メインのクラス
 * @author Yamashita Tubasa
 */

#include "MyGame.h"

// windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	std::unique_ptr<MyEngine::TYFramework> game = std::make_unique<MyGame>();

	game->Run();

	//DirectXCommon::ResourceLeakCheck();

	return 0;
}