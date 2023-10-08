#include "MyGame.h"

// windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	TYFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}