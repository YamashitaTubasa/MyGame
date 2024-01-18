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
	MyEngine::WinApp* winApp_ = nullptr;
	MyEngine::DirectXCommon* dxCommon_ = nullptr;
	// 入力
	MyEngine::Input* input_ = nullptr;
	
	// スプライト
	std::unique_ptr<MyEngine::Sprite> sprite_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> hp_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> hpBar_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> hpBack_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> ult_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> cross_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> damage_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> white_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> black_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> num0_[6];
	std::unique_ptr<MyEngine::Sprite> number_[10];
	std::unique_ptr<MyEngine::Sprite> w_;
	std::unique_ptr<MyEngine::Sprite> whiteW_;
	std::unique_ptr<MyEngine::Sprite> a_;
	std::unique_ptr<MyEngine::Sprite> whiteA_;
	std::unique_ptr<MyEngine::Sprite> s_;
	std::unique_ptr<MyEngine::Sprite> whiteS_;
	std::unique_ptr<MyEngine::Sprite> d_;
	std::unique_ptr<MyEngine::Sprite> whiteD_;
	std::unique_ptr<MyEngine::Sprite> u_;
	std::unique_ptr<MyEngine::Sprite> whiteU_;
	std::unique_ptr<MyEngine::Sprite> space_;
	std::unique_ptr<MyEngine::Sprite> x_;
	std::unique_ptr<MyEngine::Sprite> whiteX_;
	std::unique_ptr<MyEngine::Sprite> whiteSpace_;
	std::unique_ptr<MyEngine::Sprite> move_;
	std::unique_ptr<MyEngine::Sprite> spin_;
	std::unique_ptr<MyEngine::Sprite> attack_;
	std::unique_ptr<MyEngine::Sprite> dotLine_[4];
	MyEngine::SpriteCommon spriteCommon_;

	// パーティクル
	std::unique_ptr<MyEngine::ParticleManager> particleMan_ = nullptr;
	std::unique_ptr<MyEngine::ParticleManager> blackSmokeMan_ = nullptr;
	std::unique_ptr<MyEngine::Particle> particle_ = nullptr;
	std::unique_ptr<MyEngine::Particle> blackSmoke_ = nullptr;

	MyEngine::ImGuiManager* imGuiManager_ = nullptr;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<BackGroundObject> backGroundObj_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// 衝突マネージャ
	CollisionManager* collisionMan_ = nullptr;
	// 敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;

	DirectX::XMFLOAT3 position_[5]{};
	DirectX::XMFLOAT3 rotation_[5]{};
	DirectX::XMFLOAT3 scale_[5]{};
	std::vector<DirectX::XMFLOAT3> eye_ = { { 0,0,-10 } };
	std::vector<DirectX::XMFLOAT3> target_ = { {0.0f, 2.5f, 0.0f} };
	DirectX::XMFLOAT3 hpPosition_ = { 30,30,0 };
	DirectX::XMFLOAT2 hpScale_ = { 500, 20 };
	DirectX::XMFLOAT3 hpMove_ = { 10, 0, 0 };
	DirectX::XMFLOAT3 hpBarPosition_ = { 29,29,0 };
	DirectX::XMFLOAT3 hpBackPosition_ = { 30,30,0 };

	bool isPushD_ = false;
	bool isPushA_ = false;

	int playerHp_ = 3;
	int time_ = 0;
	int overTimer_ = 0;
	bool isOver_ = false;

	// パーティクル
	float particleTime_ = 0;
	bool particl_ = false;
	bool isEnemyDeth_ = false;

	int count_ = 0;

	// ダメージ
	bool isDamage_ = false;
	int damageTime_ = 0;

	// アルファ値
	float bAlpha_ = 1.0f;

	// フェードアウト・インのフラグ
	bool isOverFadeIn_ = false;
	bool isClearFadeIn_ = false;
	bool isFadeOut_ = true;

	// シーンフラグ
	bool isClearScene_ = false;
	bool isOverScene_ = false;

	// スコア
	bool isScore_ = false;
	int score_ = 0;

};
