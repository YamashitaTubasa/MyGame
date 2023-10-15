/**
 * @file main.cpp
 * @brief メインのクラス
 * @author Yamashita Tubasa
 */

#include "MyGame.h"

// windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	TYFramework* game = new MyGame();

	game->Run();

	delete game;

	//DirectXCommon::ResourceLeakCheck();

	return 0;
}