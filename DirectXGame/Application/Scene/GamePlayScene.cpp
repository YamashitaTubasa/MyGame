/** 
 * @file GamePlayerScene.cpp
 * @brief ゲームプレイシーンのクラス
 * @author Yamashita Tubasa
 */

#include "GamePlayScene.h"

#include "Player.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include <cmath>

using namespace DirectX;
using namespace std;

GamePlayScene::GamePlayScene()
{
}

GamePlayScene::~GamePlayScene()
{
}

void GamePlayScene::Initialize()
{
	winApp_ = MyEngine::WinApp::GetInstance();
	input_ = MyEngine::Input::GetInstance();
	dxCommon_ = MyEngine::DirectXCommon::GetInstance();
	collisionMan_ = CollisionManager::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	// カメラの注視点をセット
	camera_->SetTarget(target_[0]);
	camera_->SetEye(eye_[0]);
	camera_->SetDistance(8.0f);

	// 自キャラ生成
	player_ = std::make_unique<Player>();
	player_ = Player::Create();
	player_->SetCollider(new SphereCollider);

	// 敵発生データの読み込み
	LoadEnemyPopData();

	// 背景のオブジェクトの初期化
	backGroundObj_ = std::make_unique<BackGroundObject>();
	backGroundObj_->Initialize();

	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// OBJの名前を指定してモデルデータを読み込む
	particle_ = MyEngine::Particle::LoadFromOBJ("bombEffect.png");
	blackSmoke_ = MyEngine::Particle::LoadFromOBJ("bomb.png");
	// パーティクルの生成
	particleMan_ = MyEngine::ParticleManager::Create();
	blackSmokeMan_ = MyEngine::ParticleManager::Create();
	// パーティクルマネージャーにパーティクルセットする
	particleMan_->SetModel(particle_.get());
	blackSmokeMan_->SetModel(blackSmoke_.get());

	// ポストエフェクト
	postEffect_ = std::make_unique<MyEngine::PostEffect>();
	postEffect_->Initialize(L"Resources/shaders/PostEffectRadialBlurPS.hlsl");

	// スプライトの初期化
	SpriteInitialize();
}

void GamePlayScene::Update()
{
	// シーン切り替え
	if (player_->GetIsBoss() == true) {
		isClearFadeIn_ = true;
	}
	if (player_->GetIsGameOverStaging()) {
		isOverFadeIn_ = true;
	}
	if (isClearScene_) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	// カメラの更新
	camera_->Update();

	// 自キャラの更新
	player_->Update();

	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();

	// 敵キャラ更新
	for (std::unique_ptr<Enemy>& enemys : enemys_) {
		// 自機と敵が一定の距離になったら敵の弾を発射する
		if ((enemys->GetPosition().z - player_->GetPosition().z) <= fireDistance_) {
			enemys->Fire();
		}
		// 自機と敵が一定の距離になったら敵を消滅させる
		if ((enemys->GetPosition().z - player_->GetPosition().z) <= deadDistance_) {
			enemys->SetIsDead(true);
		}
		enemys->Update();
	}
	// デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemys) {
		if (enemys->GetIsDead()) {
			return true;
		};
		return false;
	});
	// 敵の弾の更新
	for (std::unique_ptr<EnemyBullet>& enemyBullets : enemyBullets_) {
		enemyBullets->Update();
	}
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullets) {
		if (enemyBullets->GetIsDead()) {
			return true;
		}
		return false;
	});

	// 背景のオブジェクトの更新
	backGroundObj_->Update();

	// 天球の更新
	skydome_->Update();

	// 当たり判定
	CheckAllCollisions();

	if (player_->GetIsGameClearStaging() == true) {
		isRadialBlur_ = true;
	}
	if (isRadialBlur_ == true) {
		postEffect_->SetIsPostE(true);
	}
	else {
		postEffect_->SetIsPostE(false);
	}

	if (input_->PushKey(DIK_RIGHT)) {
		eye_[0].x += 0.5;
	}
	if (input_->PushKey(DIK_LEFT)) {
		eye_[0].x -= 0.5;
	}
	if (input_->PushKey(DIK_UP)) {
		eye_[0].y += 0.5;
	}
	if (input_->PushKey(DIK_DOWN)) {
		eye_[0].y -= 0.5;
	}
	camera_->SetEye(eye_[0]);
	
	if (player_->GetIsHp()) {
		if (hpScale_.x >= 0) {
			hpScale_.x -= hpMove_.x;
			isDamage_ = true;
		}
	}
	hp_->SetPosition(hpPosition_);
	hp_->SetScale(hpScale_);
	hp_->SpriteTransferVertexBuffer(hp_.get(), spriteCommon_, 1);
	hp_->SpriteUpdate(hp_.get(), spriteCommon_);

	// フェード処理
	Fade();

	// ダメージの処理
	DamageDirection();

	if (isOverScene_) {
		// ゲームオーバー（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("OVER");
	}

	// パーティクルの更新
	blackSmokeMan_->Update();
	particleMan_->Update();

	// 全ての衝突をチェック
	collisionMan_->CheckAllCollisions();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_T)) {
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
	if (input_->TriggerKey(DIK_O)) {
		GameSceneManager::GetInstance()->ChangeScene("OVER");
	}
	if (input_->TriggerKey(DIK_C)) {
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

#endif
}

void GamePlayScene::Draw()
{
	// コマンドライン取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

	// 描画前処理
	postEffect_->PreDraw(cmdList);
#pragma region 3Dオブジェクトの描画

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブジェクトの描画
	skydome_->Draw();
	player_->Draw();
	for (std::unique_ptr<Enemy>& enemys : enemys_) {
		enemys->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& enemyBullets : enemyBullets_) {
		enemyBullets->Draw();
	}

	// 背景のオブジェクトの描画
	backGroundObj_->Draw();

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

#pragma endregion 

#pragma region パーティクルの描画

	// パーティクル描画前処理
	MyEngine::ParticleManager::PreDraw(cmdList);

	// パーティクルの描画
	particleMan_->Draw();
	blackSmokeMan_->Draw();
	player_->Effect();

	// パーティクル描画後処理
	MyEngine::ParticleManager::PostDraw();

#pragma endregion

#pragma region スプライト描画

	player_->SpriteDraw();

	// スプライト描画前処理
	MyEngine::Sprite::PreDraw(cmdList, spriteCommon_);

	if (player_->GetIsStartStaging() == false &&
		player_->GetIsGameClearStaging() == false && 
		player_->GetIsGameOverStaging() == false) {

		// HPバーの描画
		hpBar_->SpriteDraw(spriteCommon_);
		// HPの背景描画
		hpBack_->SpriteDraw(spriteCommon_);
		// HPの描画
		hp_->SpriteDraw(spriteCommon_);
		// 操作方法の描画
		w_->SpriteDraw(spriteCommon_);
		a_->SpriteDraw(spriteCommon_);
		s_->SpriteDraw(spriteCommon_);
		d_->SpriteDraw(spriteCommon_);
		move_->SpriteDraw(spriteCommon_);
		spin_->SpriteDraw(spriteCommon_);
		x_->SpriteDraw(spriteCommon_);
		space_->SpriteDraw(spriteCommon_);
		attack_->SpriteDraw(spriteCommon_);
		// ULTの描画
		ult_->SpriteDraw(spriteCommon_);
		cross_->SpriteDraw(spriteCommon_);
		num0_[5]->SpriteDraw(spriteCommon_);
		for (int i = 0; i < 2; i++) {
			dotLine_[i]->SpriteDraw(spriteCommon_);
		}
	}
	// 数字の描画
	if (player_->GetIsStartStaging() == false && 
		player_->GetIsGameOverStaging() == false 
		&& player_->GetIsBossStaging() == false) 
	{
		num0_[0]->SpriteDraw(spriteCommon_);
		num0_[1]->SpriteDraw(spriteCommon_);
		num0_[3]->SpriteDraw(spriteCommon_);
		num0_[4]->SpriteDraw(spriteCommon_);
		if (score_ == 0) {
			num0_[2]->SpriteDraw(spriteCommon_);
		}
		if (score_ == 100) {
			number_[1]->SpriteDraw(spriteCommon_);
		}
		if (score_ == 200) {
			number_[2]->SpriteDraw(spriteCommon_);
		}
		if (score_ == 300) {
			number_[3]->SpriteDraw(spriteCommon_);
		}
		if (score_ == 400) {
			number_[4]->SpriteDraw(spriteCommon_);
		}
	}
	// ダメージの描画
	if (isDamage_ == true || player_->GetIsBd() == true) {
		damage_->SpriteDraw(spriteCommon_);
	}
	// 黒
	if (isClearFadeIn_ == true || isFadeOut_ == true) {
		white_->SpriteDraw(spriteCommon_);
	}
	if (isOverFadeIn_ == true) {
		black_->SpriteDraw(spriteCommon_);
	}

	// スプライト描画後処理
	MyEngine::Sprite::PostDraw();
#pragma endregion
	// 描画後処理
	postEffect_->PostDraw(cmdList);

	// 描画前処理
	dxCommon_->PreDraw();

	// ポストエフェクト描画
	postEffect_->Draw(cmdList);

	// 描画後処理
	dxCommon_->PostDraw();

	// ImGui描画
	//imGuiManager->Draw(dXCommon);
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::SpriteInitialize()
{
	// スプライト
	sprite_ = std::make_unique<MyEngine::Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	MyEngine::PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// HP
	hp_ = std::make_unique<MyEngine::Sprite>();
	hp_->LoadTexture(spriteCommon_, 1, L"Resources/Image/hp.png");
	hp_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hp_->SetColor(XMFLOAT4(1, 1, 1, 1));
	hp_->SetPosition(hpPosition_);
	hp_->SetScale(hpScale_);
	hp_->SetRotation(0.0f);
	hp_->SpriteTransferVertexBuffer(hp_.get(), spriteCommon_, 1);
	hp_->SpriteUpdate(hp_.get(), spriteCommon_);
	// HPバー
	hpBar_ = std::make_unique<MyEngine::Sprite>();
	hpBar_->LoadTexture(spriteCommon_, 2, L"Resources/Image/hpBar.png");
	hpBar_->SpriteCreate(1280, 720, 2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBar_->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBar_->SetPosition(hpBarPosition_);
	hpBar_->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	hpBar_->SetRotation(0.0f);
	hpBar_->SpriteTransferVertexBuffer(hpBar_.get(), spriteCommon_, 2);
	hpBar_->SpriteUpdate(hpBar_.get(), spriteCommon_);
	// HP背景
	hpBack_ = std::make_unique<MyEngine::Sprite>();
	hpBack_->LoadTexture(spriteCommon_, 3, L"Resources/Image/hpBack.png");
	hpBack_->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBack_->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBack_->SetPosition(hpBackPosition_);
	hpBack_->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hpBack_->SetRotation(0.0f);
	hpBack_->SpriteTransferVertexBuffer(hpBack_.get(), spriteCommon_, 3);
	hpBack_->SpriteUpdate(hpBack_.get(), spriteCommon_);
	// ULT
	ult_ = std::make_unique<MyEngine::Sprite>();
	ult_->LoadTexture(spriteCommon_, 4, L"Resources/Image/ult.png");
	ult_->SpriteCreate(1280, 720, 4, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	ult_->SetColor(XMFLOAT4(1, 1, 1, 1));
	ult_->SetPosition({ 1040,615, 0 });
	ult_->SetScale({ 1600 * 0.05, 1600 * 0.05 });
	ult_->SetRotation(0.0f);
	ult_->SpriteTransferVertexBuffer(ult_.get(), spriteCommon_, 4);
	ult_->SpriteUpdate(ult_.get(), spriteCommon_);
	// cross
	cross_ = std::make_unique<MyEngine::Sprite>();
	cross_->LoadTexture(spriteCommon_, 5, L"Resources/Image/cross.png");
	cross_->SpriteCreate(1280, 720, 5, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	cross_->SetColor(XMFLOAT4(1, 1, 1, 1));
	cross_->SetPosition({ 1140,635, 0 });
	cross_->SetScale({ 64.0f * 0.6f, 64.0f * 0.6f });
	cross_->SetRotation(0.0f);
	cross_->SpriteTransferVertexBuffer(cross_.get(), spriteCommon_, 5);
	cross_->SpriteUpdate(cross_.get(), spriteCommon_);
	// 0
	for (int i = 0; i < 6; i++) {
		num0_[i] = std::make_unique<MyEngine::Sprite>();
		num0_[i]->LoadTexture(spriteCommon_, 10, L"Resources/Image/0.png");
		num0_[i]->SpriteCreate(1280, 720, 10, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
		num0_[i]->SetColor(XMFLOAT4(1, 1, 1, 1));
		num0_[i]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
		num0_[i]->SetRotation(0.0f);
		num0_[i]->SpriteTransferVertexBuffer(num0_[i].get(), spriteCommon_, 10);
		num0_[i]->SpriteUpdate(num0_[i].get(), spriteCommon_);
	}
	num0_[0]->SetPosition({ 1040, 30, 0 });
	num0_[1]->SetPosition({ 1080, 30, 0 });
	num0_[2]->SetPosition({ 1120, 30, 0 });
	num0_[3]->SetPosition({ 1160, 30, 0 });
	num0_[4]->SetPosition({ 1200, 30, 0 });
	num0_[5]->SetPosition({ 1200, 625,0 });
	num0_[5]->SetScale({ 64.0f * 0.8f, 64.0f * 0.8f });
	for(int i = 0; i < 6; i++){
		num0_[i]->SpriteUpdate(num0_[i].get(), spriteCommon_);
	}
	// 1
	number_[1] = std::make_unique<MyEngine::Sprite>();
	number_[1]->LoadTexture(spriteCommon_, 6, L"Resources/Image/1.png");
	number_[1]->SpriteCreate(1280, 720, 6, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[1]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[1]->SetPosition({ 1120, 30, 0 });
	number_[1]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[1]->SetRotation(0.0f);
	number_[1]->SpriteTransferVertexBuffer(number_[1].get(), spriteCommon_, 6);
	number_[1]->SpriteUpdate(number_[1].get(), spriteCommon_);
	// 2
	number_[2] = std::make_unique<MyEngine::Sprite>();
	number_[2]->LoadTexture(spriteCommon_, 23, L"Resources/Image/2.png");
	number_[2]->SpriteCreate(1280, 720, 23, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[2]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[2]->SetPosition({ 1120, 30, 0 });
	number_[2]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[2]->SetRotation(0.0f);
	number_[2]->SpriteTransferVertexBuffer(number_[2].get(), spriteCommon_, 23);
	number_[2]->SpriteUpdate(number_[2].get(), spriteCommon_);
	// 3
	number_[3] = std::make_unique<MyEngine::Sprite>();
	number_[3]->LoadTexture(spriteCommon_, 24, L"Resources/Image/3.png");
	number_[3]->SpriteCreate(1280, 720, 24, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[3]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[3]->SetPosition({ 1120, 30, 0 });
	number_[3]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[3]->SetRotation(0.0f);
	number_[3]->SpriteTransferVertexBuffer(number_[3].get(), spriteCommon_, 24);
	number_[3]->SpriteUpdate(number_[3].get(), spriteCommon_);
	// 4
	number_[4] = std::make_unique<MyEngine::Sprite>();
	number_[4]->LoadTexture(spriteCommon_, 25, L"Resources/Image/4.png");
	number_[4]->SpriteCreate(1280, 720, 25, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[4]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[4]->SetPosition({ 1120, 30, 0 });
	number_[4]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[4]->SetRotation(0.0f);
	number_[4]->SpriteTransferVertexBuffer(number_[4].get(), spriteCommon_, 25);
	number_[4]->SpriteUpdate(number_[4].get(), spriteCommon_);
	// 5
	number_[5] = std::make_unique<MyEngine::Sprite>();
	number_[5]->LoadTexture(spriteCommon_, 26, L"Resources/Image/5.png");
	number_[5]->SpriteCreate(1280, 720, 26, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[5]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[5]->SetPosition({ 1200, 620, 0 });
	number_[5]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[5]->SetRotation(0.0f);
	number_[5]->SpriteTransferVertexBuffer(number_[5].get(), spriteCommon_, 26);
	number_[5]->SpriteUpdate(number_[5].get(), spriteCommon_);
	// 6
	number_[6] = std::make_unique<MyEngine::Sprite>();
	number_[6]->LoadTexture(spriteCommon_, 27, L"Resources/Image/6.png");
	number_[6]->SpriteCreate(1280, 720, 27, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[6]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[6]->SetPosition({ 1200, 620, 0 });
	number_[6]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[6]->SetRotation(0.0f);
	number_[6]->SpriteTransferVertexBuffer(number_[6].get(), spriteCommon_, 27);
	number_[6]->SpriteUpdate(number_[6].get(), spriteCommon_);
	// 7
	number_[7] = std::make_unique<MyEngine::Sprite>();
	number_[7]->LoadTexture(spriteCommon_, 28, L"Resources/Image/7.png");
	number_[7]->SpriteCreate(1280, 720, 28, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[7]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[7]->SetPosition({ 1200, 620, 0 });
	number_[7]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[7]->SetRotation(0.0f);
	number_[7]->SpriteTransferVertexBuffer(number_[7].get(), spriteCommon_, 28);
	number_[7]->SpriteUpdate(number_[7].get(), spriteCommon_);
	// 8
	number_[8] = std::make_unique<MyEngine::Sprite>();
	number_[8]->LoadTexture(spriteCommon_, 29, L"Resources/Image/8.png");
	number_[8]->SpriteCreate(1280, 720, 29, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[8]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[8]->SetPosition({ 1200, 620, 0 });
	number_[8]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[8]->SetRotation(0.0f);
	number_[8]->SpriteTransferVertexBuffer(number_[8].get(), spriteCommon_, 29);
	number_[8]->SpriteUpdate(number_[8].get(), spriteCommon_);
	// 9
	number_[9] = std::make_unique<MyEngine::Sprite>();
	number_[9]->LoadTexture(spriteCommon_, 30, L"Resources/Image/9.png");
	number_[9]->SpriteCreate(1280, 720, 30, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[9]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[9]->SetPosition({ 1120, 30, 0 });
	number_[9]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[9]->SetRotation(0.0f);
	number_[9]->SpriteTransferVertexBuffer(number_[9].get(), spriteCommon_, 30);
	number_[9]->SpriteUpdate(number_[9].get(), spriteCommon_);
	// ダメージ
	damage_ = std::make_unique<MyEngine::Sprite>();
	damage_->LoadTexture(spriteCommon_, 11, L"Resources/Image/damage.png");
	damage_->SpriteCreate(1280, 720, 11, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	damage_->SetColor(XMFLOAT4(1, 1, 1, 1));
	damage_->SetPosition({ 0.0f, 0.0f, 0.0f});
	damage_->SetScale(XMFLOAT2(1280 * 1, 720 * 1));
	damage_->SetRotation(0.0f);
	damage_->SpriteTransferVertexBuffer(damage_.get(), spriteCommon_, 11);
	damage_->SpriteUpdate(damage_.get(), spriteCommon_);

	//===== Whiteの描画 =====//
	white_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	white_->LoadTexture(spriteCommon_, 12, L"Resources/Image/white.png");
	// スプライトの生成
	white_->SpriteCreate(1280, 720, 12, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	white_->SetColor({ 1,1,1,1 });
	white_->SetPosition({ 0,0,0 });
	white_->SetScale({ 1280,720 });
	white_->SetRotation(0.0f);
	white_->SetAlpha(bAlpha_);
	// スプライトの頂点バッファの転送
	white_->SpriteTransferVertexBuffer(white_.get(), spriteCommon_, 12);

	//===== Blackの描画 =====//
	black_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, 13, L"Resources/Image/black.png");
	// スプライトの生成
	black_->SpriteCreate(1280, 720, 13, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor({ 1,1,1,1 });
	black_->SetPosition({ 0,0,0 });
	black_->SetScale({ 1280,720 });
	black_->SetRotation(0.0f);
	black_->SetAlpha(bAlpha_);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_.get(), spriteCommon_, 13);
	black_->SpriteUpdate(black_.get(), spriteCommon_);

	// W
	w_ = std::make_unique<MyEngine::Sprite>();
	w_->LoadTexture(spriteCommon_, 14, L"Resources/Image/w1.png");
	w_->SpriteCreate(1280, 720, 14, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	w_->SetColor(XMFLOAT4(1, 1, 1, 1));
	w_->SetPosition({ 70,510, 0 });
	w_->SetScale(XMFLOAT2(64 * 0.45f, 64 * 0.45f));
	w_->SetRotation(0.0f);
	w_->SpriteTransferVertexBuffer(w_.get(), spriteCommon_, 14);
	w_->SpriteUpdate(w_.get(), spriteCommon_);
	// a
	a_ = std::make_unique<MyEngine::Sprite>();
	a_->LoadTexture(spriteCommon_, 15, L"Resources/Image/a1.png");
	a_->SpriteCreate(1280, 720, 15, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	a_->SetColor(XMFLOAT4(1, 1, 1, 1));
	a_->SetPosition({ 30,560, 0 });
	a_->SetScale(XMFLOAT2(64 * 0.45f, 64 * 0.45f));
	a_->SetRotation(0.0f);
	a_->SpriteTransferVertexBuffer(a_.get(), spriteCommon_, 15);
	a_->SpriteUpdate(a_.get(), spriteCommon_);
	// s
	s_ = std::make_unique<MyEngine::Sprite>();
	s_->LoadTexture(spriteCommon_, 16, L"Resources/Image/s1.png");
	s_->SpriteCreate(1280, 720, 16, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	s_->SetColor(XMFLOAT4(1, 1, 1, 1));
	s_->SetPosition({ 70,560, 0 });
	s_->SetScale(XMFLOAT2(64 * 0.45f, 64 * 0.45f));
	s_->SetRotation(0.0f);
	s_->SpriteTransferVertexBuffer(s_.get(), spriteCommon_, 16);
	s_->SpriteUpdate(s_.get(), spriteCommon_);
	// d
	d_ = std::make_unique<MyEngine::Sprite>();
	d_->LoadTexture(spriteCommon_, 17, L"Resources/Image/d1.png");
	d_->SpriteCreate(1280, 720, 17, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	d_->SetColor(XMFLOAT4(1, 1, 1, 1));
	d_->SetPosition({ 115,560, 0 });
	d_->SetScale(XMFLOAT2(64 * 0.45f, 64 * 0.45f));
	d_->SetRotation(0.0f);
	d_->SpriteTransferVertexBuffer(d_.get(), spriteCommon_, 17);
	d_->SpriteUpdate(d_.get(), spriteCommon_);
	// move
	move_ = std::make_unique<MyEngine::Sprite>();
	move_->LoadTexture(spriteCommon_, 18, L"Resources/Image/move1.png");
	move_->SpriteCreate(1280, 720, 18, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	move_->SetColor(XMFLOAT4(1, 1, 1, 1));
	move_->SetPosition({ 155,540, 0 });
	move_->SetScale(XMFLOAT2(200 * 0.35f, 64 * 0.35f));
	move_->SetRotation(0.0f);
	move_->SpriteTransferVertexBuffer(move_.get(), spriteCommon_, 18);
	move_->SpriteUpdate(move_.get(), spriteCommon_);
	// spin
	spin_ = std::make_unique<MyEngine::Sprite>();
	spin_->LoadTexture(spriteCommon_, 19, L"Resources/Image/spin1.png");
	spin_->SpriteCreate(1280, 720, 19, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	spin_->SetColor(XMFLOAT4(1, 1, 1, 1));
	spin_->SetPosition({ 150,610, 0 });
	spin_->SetScale(XMFLOAT2(300 * 0.35f, 64 * 0.35f));
	spin_->SetRotation(0.0f);
	spin_->SpriteTransferVertexBuffer(spin_.get(), spriteCommon_, 19);
	spin_->SpriteUpdate(spin_.get(), spriteCommon_);
	// u
	u_ = std::make_unique<MyEngine::Sprite>();
	u_->LoadTexture(spriteCommon_, 20, L"Resources/Image/u.png");
	u_->SpriteCreate(1280, 720, 20, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	u_->SetColor(XMFLOAT4(1, 1, 1, 1));
	u_->SetPosition({ 70,610, 0 });
	u_->SetScale(XMFLOAT2(64 * 0.45f, 64 * 0.45f));
	u_->SetRotation(0.0f);
	u_->SpriteTransferVertexBuffer(u_.get(), spriteCommon_, 20);
	u_->SpriteUpdate(u_.get(), spriteCommon_);
	// space
	space_ = std::make_unique<MyEngine::Sprite>();
	space_->LoadTexture(spriteCommon_, 21, L"Resources/Image/space1.png");
	space_->SpriteCreate(1280, 720, 21, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	space_->SetColor(XMFLOAT4(1, 1, 1, 1));
	space_->SetPosition({ 40,660, 0 });
	space_->SetScale(XMFLOAT2(200 * 0.45f, 64 * 0.45f));
	space_->SetRotation(0.0f);
	space_->SpriteTransferVertexBuffer(space_.get(), spriteCommon_, 21);
	space_->SpriteUpdate(space_.get(), spriteCommon_);
	// attack
	attack_ = std::make_unique<MyEngine::Sprite>();
	attack_->LoadTexture(spriteCommon_, 22, L"Resources/Image/attack1.png");
	attack_->SpriteCreate(1280, 720, 22, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	attack_->SetColor(XMFLOAT4(1, 1, 1, 1));
	attack_->SetPosition({ 145,660, 0 });
	attack_->SetScale(XMFLOAT2(210 * 0.35f, 64 * 0.35f));
	attack_->SetRotation(0.0f);
	attack_->SpriteTransferVertexBuffer(attack_.get(), spriteCommon_, 22);
	attack_->SpriteUpdate(attack_.get(), spriteCommon_);
	// 点線
	for (int i = 0; i < 2; i++) {
		dotLine_[i] = std::make_unique<MyEngine::Sprite>();
		dotLine_[i]->LoadTexture(spriteCommon_, 31, L"Resources/Image/circleDot1.png");
		dotLine_[i]->SpriteCreate(1280, 720, 31, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
		dotLine_[i]->SetColor(XMFLOAT4(1, 1, 1, 1));
		dotLine_[i]->SetScale(XMFLOAT2(64 * 4.0f, 10 * 1.0f));
		dotLine_[i]->SetRotation(0.0f);
	}
	dotLine_[0]->SetPosition({ 20,595, 0 });
	dotLine_[1]->SetPosition({ 20,645, 0 });
	//dotLine_[2]->SetPosition({ 20,685, 0 });
	for (int i = 0; i < 2; i++) {
		dotLine_[i]->SpriteTransferVertexBuffer(dotLine_[i].get(), spriteCommon_, 31);
		dotLine_[i]->SpriteUpdate(dotLine_[i].get(), spriteCommon_);
	}
	// X
	x_ = std::make_unique<MyEngine::Sprite>();
	x_->LoadTexture(spriteCommon_, 38, L"Resources/Image/x1.png");
	x_->SpriteCreate(1280, 720, 38, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	x_->SetColor(XMFLOAT4(1, 1, 1, 1));
	x_->SetPosition({ 70,610, 0 });
	x_->SetScale({ 64.0f * 0.45f, 64.0f * 0.45f });
	x_->SetRotation(0.0f);
	x_->SpriteTransferVertexBuffer(x_.get(), spriteCommon_, 38);
	x_->SpriteUpdate(x_.get(), spriteCommon_);
}

void GamePlayScene::GameReset()
{
	playerHp_ = 3;
	time_ = 0;
}

bool GamePlayScene::CheckCollision(const DirectX::XMFLOAT3& object, const DirectX::XMFLOAT3& object1)
{
	if(std::pow((object1.x - object.x),2.0f) + std::pow((object1.y - object.y),2.0f) + 
		std::pow((object1.z - object.z), 2.0f) <= std::pow((1 + 1), 2.0f)){
		return true;
	}

	return true;
}

void GamePlayScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWait_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands_, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			std::unique_ptr<Enemy> newEnemys = std::make_unique<Enemy>();
			newEnemys->Initialize({x,y,z});
			newEnemys->SetGamePlayScene(this);
			enemys_.push_back(std::move(newEnemys));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GamePlayScene::Fade()
{
	// フェードアウト処理
	if (isFadeOut_) {
		if (bAlpha_ > 0.0f) {
			bAlpha_ -= 0.01f;
		}
		if (bAlpha_ <= 0.01f) {
			bAlpha_ = 0.0f;
			isFadeOut_ = false;
		}
	}

	// フェードインの処理
	if (isOverFadeIn_) {
		if (bAlpha_ < 1.0f) {
			bAlpha_ += 0.01f;
		}
		if (bAlpha_ > 1.0f) {
			bAlpha_ = 1.0f;
			isOverScene_ = true;
		}
	}
	if (isClearFadeIn_) {
		if (bAlpha_ < 1.0f) {
			bAlpha_ += 0.01f;
		}
		if (bAlpha_ > 1.0f) {
			bAlpha_ = 1.0f;
			isClearScene_ = true;
		}
	}
	white_->SetAlpha(bAlpha_);
	black_->SetAlpha(bAlpha_);
}

void GamePlayScene::DamageDirection()
{
	if (isDamage_ == true || player_->GetIsBd() == true) {
		damageTime_++;
		player_->Shake();
		if (hpScale_.x >= 0) {
			hpScale_.x -= 5;
		}
	}
	if (damageTime_ >= 20) {
		isDamage_ = false;
		damageTime_ = 0;
	}

	if (hpScale_.x <= 0) {

		overTimer_++;
		player_->SetIsGameOverStaging(true);

		if (overTimer_ >= 50) {

			isOverFadeIn_ = true;
			overTimer_ = 0;
		}
	}
}

void GamePlayScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullets)
{
	// 敵の弾の登録
	enemyBullets_.push_back(std::move(enemyBullets));
}

void GamePlayScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自キャラ弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets_ = player_->GetBullets();

#pragma region 自キャラと敵キャラの当たり判定

	// 自キャラの座標
	posA.x = player_->GetPosition().x;
	posA.y = player_->GetPosition().y;
	posA.z = player_->GetPosition().z;

	// 自キャラと敵キャラ全ての当たり判定
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		// 敵キャラ弾の座標
		posB.x = enemy->GetPosition().x;
		posB.y = enemy->GetPosition().y;
		posB.z = enemy->GetPosition().z;

		// 座標AとBの距離を求める
		Vector3 direction = posA - posB;
		const float radius = 1.0f;

		// 球と球の交差判定
		if ((std::pow((posA.x - posB.x), 2.0f) + std::pow((posA.y - posB.y), 2.0f) + std::pow((posA.z - posB.z), 2.0f)) <= std::pow((radius + radius), 2.0f)) {
		}
	}

#pragma endregion

#pragma region 自キャラと敵キャラ弾の当たり判定

	// 自キャラの座標
	posA.x = player_->GetPosition().x;
	posA.y = player_->GetPosition().y;
	posA.z = player_->GetPosition().z;

	// 自キャラと敵キャラ弾全ての当たり判定
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		// 敵キャラ弾の座標
		posB.x = bullet->GetPosition().x;
		posB.y = bullet->GetPosition().y;
		posB.z = bullet->GetPosition().z;

		// 座標AとBの距離を求める
		Vector3 direction = posA - posB;
		const float radius = 1.0f;

		// 球と球の交差判定
		if ((std::pow((posA.x - posB.x), 2.0f) + std::pow((posA.y - posB.y), 2.0f) + std::pow((posA.z - posB.z), 2.0f)) <= std::pow((radius + radius), 2.0f)) {
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自キャラ弾と敵キャラの当たり判定

	// 自キャラ弾の座標
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
		posA.x = playerBullet->GetPosition().x;
		posA.y = playerBullet->GetPosition().y;
		posA.z = playerBullet->GetPosition().z;

		// 自キャラ弾と敵キャラ全ての当たり判定
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			// 敵キャラ弾の座標
			posB.x = enemy->GetPosition().x;
			posB.y = enemy->GetPosition().y;
			posB.z = enemy->GetPosition().z;

			// 座標AとBの距離を求める
			Vector3 direction = posA - posB;
			const float radius = 1.0f;

			// 球と球の交差判定
			if ((std::pow((posA.x - posB.x), 2.0f) + std::pow((posA.y - posB.y), 2.0f) + std::pow((posA.z - posB.z), 2.0f)) <= std::pow((radius + radius), 2.0f)) {
				playerBullet->OnCollision();
				enemy->OnCollision();
				score_ += mobEnemyScore_;
				
				for (int i = 0; i < 50; i++) {
					// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
					const float md_pos = 0.5f;
					XMFLOAT3 pos{};
					pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + enemy->GetPosition().x;
					pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + enemy->GetPosition().y;
					pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + enemy->GetPosition().z;
					// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
					const float md_vel = 0.1f;
					XMFLOAT3 vel{};
					vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
					vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
					vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
					// 重力に見立ててYのみ[-0.001f,0]でランダム分布
					XMFLOAT3 acc{};
					const float md_acc = 0.001f;
					acc.y = (float)rand() / RAND_MAX * md_acc;

					// 追加
					particle_->Add(life_, pos, vel, acc, start_scale_, end_scale_);
				}
			}
		}
	}

#pragma endregion

#pragma region 自キャラ弾と敵キャラ弾の当たり判定

	// 自キャラ弾の座標
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
		posA.x = playerBullet->GetPosition().x;
		posA.y = playerBullet->GetPosition().y;
		posA.z = playerBullet->GetPosition().z;
		
		// 自キャラ弾と敵キャラ弾全ての当たり判定
		for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
			// 敵キャラ弾の座標
			posB.x = bullet->GetPosition().x;
			posB.y = bullet->GetPosition().y;
			posB.z = bullet->GetPosition().z;

			// 座標AとBの距離を求める
			Vector3 direction = posA - posB;
			const float radius = 1.0f;

			// 球と球の交差判定
			if ((std::pow((posA.x - posB.x), 2.0f) + std::pow((posA.y - posB.y), 2.0f) + std::pow((posA.z - posB.z), 2.0f)) <= std::pow((radius + radius), 2.0f)) {
				playerBullet->OnCollision();
 				bullet->OnCollision();
			}
		}
	}

#pragma endregion
}
