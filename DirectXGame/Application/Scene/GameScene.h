#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "ImGuiManager.h"
#include "Vector3.h"
#include "GameTitleScene.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "PostEffect.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerBullet.h"

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
	void Initialize(SpriteCommon& spriteCommon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize();

	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	/// <param name="dXCommon"></param>
	void ObjectInitialize();

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// オブジェクトの解放
	/// </summary>
	void ObjectFinalize();

	/// <summary>
	/// スプライト初期化
	/// </summary>
	void SpriteInitialize(SpriteCommon& spriteCommon);

	// パーティクルの初期化
	void ParticleInitialize();

	void ParticleUpdate();

	/// <summary>
	/// ゲームのリセット
	/// </summary>
	void GameReset();

	/// <summary>
	/// 当たり判定
	/// </summary>
	bool CheckCollision(const DirectX::XMFLOAT3& object, const DirectX::XMFLOAT3& object1);

private:
	WinApp* winApp = nullptr;
	// 入力
	Input* input = nullptr;
	// モデル
	Model* enemeyM = nullptr;
	Model* skydomeM = nullptr;
	Model* Model[5];
	// オブジェクト
	Object3d* enemyO3 = nullptr;
	Object3d* skydomeO3 = nullptr;
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
	Camera* camera = nullptr;
	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
	PostEffect* postEffect = nullptr;
	Player* player = nullptr;
	Player* pBullet = nullptr;
	Enemy* enemy = nullptr;

	DirectX::XMFLOAT3 position[5]{};
	DirectX::XMFLOAT3 rotation[5]{};
	DirectX::XMFLOAT3 scale[5]{};
	DirectX::XMFLOAT3 eye[5]{};
	DirectX::XMFLOAT3 target[5]{};
	DirectX::XMFLOAT3 hpPosition = { 30,30,0 };
	DirectX::XMFLOAT2 hpScale = { 500, 20 };
	DirectX::XMFLOAT3 hpMove = { -1, 0, 0 };
	DirectX::XMFLOAT3 hpBarPosition = { 29,29,0 };
	DirectX::XMFLOAT3 hpBackPosition = { 30,30,0 };

	bool isPush_D = false;
	bool isPush_A = false;

	int playerHp = 3;
	int time = 0;
	float particleTime = 0;
	bool particl = false;
	bool isEnemyDeth = false;
};
