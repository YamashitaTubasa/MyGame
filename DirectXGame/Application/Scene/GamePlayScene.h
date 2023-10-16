/**
 * @file GamePlayerScene.h
 * @brief ゲームプレイシーンのクラス
 * @author Yamashita Tubasa
 */

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
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "PostEffect.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "Skydome.h"
#include "BackGroundObject.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GameClearScene.h"

// 前方宣言
class PostEffect;
class CollisionManager;
class Player;

class GamePlayScene : public GameBaseScene
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
	GamePlayScene();
	~GamePlayScene();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dXCommon"></param>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// スプライト初期化
	/// </summary>
	void SpriteInitialize();

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
	
	// スプライト
	Sprite* sprite = nullptr;
	Sprite* hp = nullptr;
	Sprite* hpBar = nullptr;
	Sprite* hpBack = nullptr;
	Sprite* enemyHp = nullptr;
	Sprite* enemyHpBar = nullptr;
	Sprite* enemyHpBack = nullptr;
	Sprite* ult = nullptr;
	Sprite* X = nullptr;
	Sprite* damage = nullptr;
	Sprite* black_ = nullptr;
	Sprite* number[5];
	SpriteCommon spriteCommon_;

	// パーティクル
	ParticleManager* particleMan = nullptr;
	Particle* particle = nullptr;

	DirectXCommon* dXCommon = nullptr;

	ImGuiManager* imGuiManager = nullptr;
	Camera* camera = nullptr;
	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
	/*PostEffect* postEffect = nullptr;
	PostEffect* postEffect1 = nullptr;*/
	Player* player = nullptr;
	Player* pBullet = nullptr;
	Enemy* enemy = nullptr;
	BackGroundObject* backGroundObj = nullptr;
	Skydome* skydome = nullptr;
	// 衝突マネージャ
	CollisionManager* collisionMan = nullptr;
	// 敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;

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
	DirectX::XMFLOAT3 enemyHpPosition = { 1250,30,0 };
	DirectX::XMFLOAT2 enemyHpScale = { 500, 20 };
	DirectX::XMFLOAT3 enemyHpMove = { -1, 0, 0 };
	DirectX::XMFLOAT3 enemyHpBarPosition = { 1251,29,0 };
	DirectX::XMFLOAT3 enemyHpBackPosition = { 1251,30,0 };

	bool isPush_D = false;
	bool isPush_A = false;

	int playerHp = 3;
	int time = 0;
	float particleTime = 0;
	bool particl = false;
	bool isEnemyDeth = false;

	int count = 0;
	bool isDamage = false;
	int damageTime = 0;

	// アルファ値
	float bAlpha_ = 1.0f;

	// フェードアウト・インのフラグ
	bool isFadeIn = false;
	bool isFadeOut = true;

	// シーンフラグ
	bool isScene = false;
};
