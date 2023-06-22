#pragma once
#include "ParticleManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

class GameTitleScene 
{
public:
	GameTitleScene();
	~GameTitleScene();

public:
	void Initialize();

	void Update();

	void Draw(DirectXCommon* dXCommon);

private:
	ParticleManager* particleMan1 = nullptr;
	Input* input = nullptr;
	DirectXCommon* dXCommon = nullptr;
};
