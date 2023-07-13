#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "ImGuiManager.h"
#include "GameTitleScene.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "PostEffect.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"

class GameScene
{
private:
	enum Scene {
		Title,
		Scene_1,
		GameClear,
		GameOver
	};
	Scene scene;

public:
	GameScene();
	~GameScene();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dXCommon"></param>
	void Initialize(Camera* camera, SpriteCommon& spriteCommon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera* camera);

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize();

	/// <summary>
	/// スプライト初期化
	/// </summary>
	void SpriteInitialize(SpriteCommon& spriteCommon);

	/// <summary>
	/// ゲームのリセット
	/// </summary>
	void GameReset();

	/// <summary>
	/// 当たり判定
	/// </summary>
	int CheckCollision(DirectX::XMFLOAT3 object, DirectX::XMFLOAT3 scale);

private:
	WinApp* winApp = nullptr;
	// 入力
	Input* input = nullptr;
	// モデル
	Model* Model[5];
	// オブジェクト
	Object3d* object3d[5];
	// スプライト
	Sprite* sprite = nullptr;
	Sprite* hp = nullptr;
	Sprite* hpBar = nullptr;
	Sprite* hpBack = nullptr;
	Sprite* ult = nullptr;
	Sprite* X = nullptr;
	Sprite* number[5];
	SpriteCommon spriteCommon_;

	// パーティクル
	ParticleManager* particleMan = nullptr;
	ParticleManager* particleMan1 = nullptr;
	Particle* particle = nullptr;
	Particle* particle1 = nullptr;
	//ParticleManager* particleMan1 = nullptr;

	DirectXCommon* dXCommon = nullptr;

	ImGuiManager* imGuiManager = nullptr;
	GameTitleScene* gTS = nullptr;
	Camera* camera_ = nullptr;
	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
	PostEffect* postEffect = nullptr;
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Skydome* skydome = nullptr;

	Vector3 position[5]{};
	Vector3 rotation[5]{};
	Vector3 scale[5]{};
	Vector3 eye[5]{};
	Vector3 target[5]{};
	Vector3 hpPosition = { 30,30,0 };
	Vector2 hpScale = { 500, 20 };
	Vector3 hpMove = { -1, 0, 0 };
	Vector3 hpBarPosition = { 29,29,0 };
	Vector3 hpBackPosition = { 30,30,0 };

	bool isPush_D = false;
	bool isPush_A = false;

	int playerHp = 3;
	int time = 0;
	float particleTime = 0;
	bool particl = false;
};
