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
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include "Skydome.h"
#include "BackGroundObject.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GameClearScene.h"
#include "BossEnemy.h"

#include <sstream>
#include <list>

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
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// フェード処理
	/// </summary>
	void Fade();

	/// <summary>
	/// ダメージ演出
	/// </summary>
	void DamageDirection();

	/// <summary>
	/// 敵の弾の追加
	/// </summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullets);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private:
	MyEngine::WinApp* winApp_;
	MyEngine::DirectXCommon* dxCommon_;
	MyEngine::Input* input_;
	
	// スプライト
	std::unique_ptr<MyEngine::Sprite> sprite_;
	std::unique_ptr<MyEngine::Sprite> hp_;
	std::unique_ptr<MyEngine::Sprite> hpBar_;
	std::unique_ptr<MyEngine::Sprite> hpBack_;
	std::unique_ptr<MyEngine::Sprite> ult_;
	std::unique_ptr<MyEngine::Sprite> cross_;
	std::unique_ptr<MyEngine::Sprite> damage_;
	std::unique_ptr<MyEngine::Sprite> white_;
	std::unique_ptr<MyEngine::Sprite> black_;
	std::array<std::unique_ptr<MyEngine::Sprite>, 6> num0_;
	std::array<std::unique_ptr<MyEngine::Sprite>, 10> number_;
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
	std::array<std::unique_ptr<MyEngine::Sprite>, 2> dotLine_;
	std::unique_ptr<MyEngine::Sprite> enemyHp_;
	std::unique_ptr<MyEngine::Sprite> enemyHpBar_;
	std::unique_ptr<MyEngine::Sprite> enemyHpBack_;
	MyEngine::SpriteCommon spriteCommon_;

	// パーティクル
	std::unique_ptr<MyEngine::ParticleManager> particleMan_;
	std::unique_ptr<MyEngine::ParticleManager> blackSmokeMan_;
	std::unique_ptr<MyEngine::Particle> particle_;
	std::unique_ptr<MyEngine::Particle> blackSmoke_;

	// ポストエフェクト
	std::unique_ptr<MyEngine::PostEffect> postEffect_;

	MyEngine::ImGuiManager* imGuiManager_ = nullptr;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<BackGroundObject> backGroundObj_;
	std::unique_ptr<Skydome> skydome_;
	// 衝突マネージャ
	CollisionManager* collisionMan_ = nullptr;
	// 敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	// 敵発生コマンド
	std::stringstream enemyPopCommands_;
	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	std::unique_ptr<Object3d> enemyEffect_;
	std::unique_ptr<Object3d> enemyEffectOne_;
	std::unique_ptr<Object3d> enemyEffectTwo_;
	std::unique_ptr<Object3d> enemyEffectThree_;
	std::unique_ptr<Object3d> enemyEffectFour_;
	std::unique_ptr<Model> enemyEffectModel_;

	std::list<std::unique_ptr<BossEnemy>> bossEnemy_;

	// 待機フラグ
	bool isWait_ = false;
	// 待機タイマー
	int waitTimer_ = 0;

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
	DirectX::XMFLOAT3 enemyHpPosition_ = { 1250,30,0 };
	DirectX::XMFLOAT2 enemyHpScale_ = { 500, 20 };
	DirectX::XMFLOAT3 enemyHpMove_ = { 10, 0, 0 };
	DirectX::XMFLOAT3 enemyHpBarPosition_ = { 1251,29,0 };
	DirectX::XMFLOAT3 enemyHpBackPosition_ = { 1251,30,0 };
	float enemyAlpha_ = 0.0f;
	bool isBossDamage_ = false;
	int bossDamageTimer_ = 0;
	DirectX::XMFLOAT3 enemyEffectPosition_ = { 0,0,0 };
	DirectX::XMFLOAT3 enemyEffectOnePosition_ = { 0,0,0 };
	DirectX::XMFLOAT3 enemyEffectTwoPosition_ = { 0,0,0 };
	DirectX::XMFLOAT3 enemyEffectThreePosition_ = { 0,0,0 };
	DirectX::XMFLOAT3 enemyEffectFourPosition_ = { 0,0,0 };
	DirectX::XMFLOAT3 enemyEffectScale_ = { 0.5f,0.5f,0.5f };
	DirectX::XMFLOAT3 enemyEffectOneScale_ = { 0.5f,0.5f,0.5f };
	DirectX::XMFLOAT3 enemyEffectTwoScale_ = { 0.5f,0.5f,0.5f };
	DirectX::XMFLOAT3 enemyEffectThreeScale_ = { 0.5f,0.5f,0.5f };
	DirectX::XMFLOAT3 enemyEffectFourScale_ = { 0.5f,0.5f,0.5f };
	bool isEnemyEffect_ = false;
	int enemyEffectTime_ = 0;

	bool isPushD_ = false;
	bool isPushA_ = false;

	int playerHp_ = 3;
	int time_ = 0;
	int overTimer_ = 0;
	bool isOver_ = false;

	// パーティクル
	float particleTime_ = 0;
	bool isParticle_ = false;
	bool isEnemyDeth_ = false;

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
	static const int mobEnemyScore_ = 100;

	// ポストエフェクト
	bool isRadialBlur_ = true;

	// 敵弾が発射する距離
	static const int fireDistance_ = 100;
	// 敵が消滅する距離
	static const int deadDistance_ = -20;

	DirectX::XMFLOAT3 po_ = { 0,0,0 };
	float pos_ = 0.0f;

	const int life_ = 20;
	const float start_scale_ = 0.6f;
	const float end_scale_ = 0.0f;
};
