/**
 * @file GamePlayerScene.cpp
 * @brief ゲームプレイシーンのクラス
 * @author Yamashita Tubasa
 */

#include "GamePlayScene.h"

#include "Player.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

using namespace DirectX;
using namespace std;

GamePlayScene::GamePlayScene()
{
}

GamePlayScene::~GamePlayScene()
{
	delete skydome_;
	delete player_;
	delete sprite_;
	delete camera_;
	delete fbxObject_;
	delete fbxModel_;
	delete hp_;
	delete hpBar_;
	delete hpBack_;
	delete enemyHp_;
	delete enemyHpBar_;
	delete enemyHpBack_;
	delete ult_;
	delete X_;
	delete damage_;
	delete particle_;
	delete particleMan_;
	delete blackSmokeMan_;
	delete blackSmoke_;
	delete enemy_;
	for (int i = 0; i < 5; i++) {
		delete number_[i];
	}
	delete backGroundObj_;
}

void GamePlayScene::Initialize()
{
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	collisionMan_ = CollisionManager::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->Initialize();

	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(camera_);
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	
	// カメラの注視点をセット
	camera_->SetTarget(target_[0]);
	camera_->SetEye(eye_[0]);
	camera_->SetDistance(8.0f);

	// 自キャラ生成
	player_ = new Player();
	player_ = Player::Create();
	player_->SetCollider(new SphereCollider);

	// 敵キャラ生成
	enemy_ = new Enemy();
	enemy_ = Enemy::Create();
	enemy_->SetCollider(new SphereCollider);

	// 背景のオブジェクトの初期化
	backGroundObj_ = new BackGroundObject();
	backGroundObj_->Initialize();

	// 天球の初期化
	skydome_ = new Skydome();
	skydome_->Initialize();

	// OBJの名前を指定してモデルデータを読み込む
	particle_ = Particle::LoadFromOBJ("bombEffect.png");
	blackSmoke_ = Particle::LoadFromOBJ("bomb.png");
	// パーティクルの生成
	particleMan_ = ParticleManager::Create();
	blackSmokeMan_ = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルセットする
	particleMan_->SetModel(particle_);
	blackSmokeMan_->SetModel(blackSmoke_);
	
	// スプライトの初期化
	SpriteInitialize();
}

void GamePlayScene::Update()
{
	count_++;

	// シーン切り替え
	if (player_->GetPositon().z >= 150) {
		isClearFadeIn_ = true;
	}
	if (player_->GetIsGameOverStaging()) {
		isOverFadeIn_ = true;
	}
	if (isClearScene_) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	enemy_->SetDamage(player_->GetDamage());
	enemy_->SetDamage01(player_->GetDamage01());
	enemy_->SetDamage02(player_->GetDamage02());
	enemy_->SetDamage03(player_->GetDamage03());

	// カメラの更新
	camera_->Update();

	// 自キャラの更新
	player_->Update();

	// 敵キャラ更新
	enemy_->Update();
	// デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy_) {
		return enemy_->GetIsDead();
	});

	// 背景のオブジェクトの更新
	backGroundObj_->Update();

	// 天球の更新
	skydome_->Update();

	/*if (CheckCollision(player->GetpBulletP(), enemy->GetPosition())) {
		isEnemyDeth = true;
	}*/

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
	hp_->SpriteTransferVertexBuffer(hp_, spriteCommon_, 1);
	hp_->SpriteUpdate(hp_, spriteCommon_);
	enemyHp_->SetScale(enemyHpScale_);
	enemyHp_->SpriteTransferVertexBuffer(enemyHp_, spriteCommon_, 7);
	enemyHp_->SpriteUpdate(enemyHp_, spriteCommon_);

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

	// ダメージの処理
	if (isDamage_) {
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

		if (overTimer_ >= 50) {

			isOverFadeIn_ = true;
			overTimer_ = 0;
		}
	}
	if (isOverScene_) {
		// ゲームオーバー（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("OVER");
	}

	// パーティクルの更新
	blackSmokeMan_->Update();

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

#pragma region 3Dオブジェクトの描画

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブジェクトの描画
	skydome_->Draw();
	player_->Draw();
	if (!isEnemyDeth_) {
		enemy_->Draw();
	}

	// 背景のオブジェクトの描画
	backGroundObj_->Draw();

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

#pragma endregion 

#pragma region パーティクルの描画

	// パーティクル描画前処理
	ParticleManager::PreDraw(cmdList);

	// パーティクルの描画
	blackSmokeMan_->Draw();
	player_->Effect();

	// パーティクル描画後処理
	ParticleManager::PostDraw();

#pragma endregion

#pragma region スプライト描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	if (player_->GetIsStartStaging() == false && player_->GetIsGameClearStaging() == false && 
		player_->GetIsGameOverStaging() == false) {

		// HPバーの描画
		hpBar_->SpriteDraw(spriteCommon_);
		// HPの背景描画
		hpBack_->SpriteDraw(spriteCommon_);
		// HPの描画
		hp_->SpriteDraw(spriteCommon_);
	}
	// ULTの描画
	//ult->SpriteDraw(spriteCommon_);
	// Xの描画
	//X->SpriteDraw(spriteCommon_);
	// 0の描画
	//number[0]->SpriteDraw(spriteCommon_);
	
	// 1の描画
	//number[1]->SpriteDraw(spriteCommon_);
	
	// 敵のHPバーの描画
	//enemyHpBar->SpriteDraw(spriteCommon_);
	// 敵のHPの背景描画
	//enemyHpBack->SpriteDraw(spriteCommon_);
	// 敵のHPの描画
	//enemyHp->SpriteDraw(spriteCommon_);
	// ダメージの描画
	if (isDamage_) {
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
	Sprite::PostDraw();

#pragma endregion

	// ImGui描画
	//imGuiManager->Draw(dXCommon);
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::SpriteInitialize()
{
	// スプライト
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// HP
	hp_ = new Sprite();
	hp_->LoadTexture(spriteCommon_, 1, L"Resources/Image/hp.png");
	hp_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hp_->SetColor(XMFLOAT4(1, 1, 1, 1));
	hp_->SetPosition(hpPosition_);
	hp_->SetScale(hpScale_);
	hp_->SetRotation(0.0f);
	hp_->SpriteTransferVertexBuffer(hp_, spriteCommon_, 1);
	hp_->SpriteUpdate(hp_, spriteCommon_);
	// HPバー
	hpBar_ = new Sprite();
	hpBar_->LoadTexture(spriteCommon_, 2, L"Resources/Image/hpBar.png");
	hpBar_->SpriteCreate(1280, 720, 2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBar_->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBar_->SetPosition(hpBarPosition_);
	hpBar_->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	hpBar_->SetRotation(0.0f);
	hpBar_->SpriteTransferVertexBuffer(hpBar_, spriteCommon_, 2);
	hpBar_->SpriteUpdate(hpBar_, spriteCommon_);
	// HP背景
	hpBack_ = new Sprite();
	hpBack_->LoadTexture(spriteCommon_, 3, L"Resources/Image/hpBack.png");
	hpBack_->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBack_->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBack_->SetPosition(hpBackPosition_);
	hpBack_->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hpBack_->SetRotation(0.0f);
	hpBack_->SpriteTransferVertexBuffer(hpBack_, spriteCommon_, 3);
	hpBack_->SpriteUpdate(hpBack_, spriteCommon_);
	// ULT
	ult_ = new Sprite();
	ult_->LoadTexture(spriteCommon_, 4, L"Resources/Image/ult.png");
	ult_->SpriteCreate(1280, 720, 4, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	ult_->SetColor(XMFLOAT4(1, 1, 1, 1));
	ult_->SetPosition({ 1010,610, 0 });
	ult_->SetScale({ 1600 * 0.05, 1600 * 0.05 });
	ult_->SetRotation(0.0f);
	ult_->SpriteTransferVertexBuffer(ult_, spriteCommon_, 4);
	ult_->SpriteUpdate(ult_, spriteCommon_);
	// X
	X_ = new Sprite();
	X_->LoadTexture(spriteCommon_, 5, L"Resources/Image/x.png");
	X_->SpriteCreate(1280, 720, 5, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	X_->SetColor(XMFLOAT4(1, 1, 1, 1));
	X_->SetPosition({ 1120,630, 0 });
	X_->SetScale({ 64.0f * 0.7f, 64.0f * 0.7f });
	X_->SetRotation(0.0f);
	X_->SpriteTransferVertexBuffer(X_, spriteCommon_, 5);
	X_->SpriteUpdate(X_, spriteCommon_);
	// 0
	number_[0] = new Sprite();
	number_[0]->LoadTexture(spriteCommon_, 10, L"Resources/Image/0.png");
	number_[0]->SpriteCreate(1280, 720, 10, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[0]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[0]->SetPosition({ 1200, 620, 0 });
	number_[0]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[0]->SetRotation(0.0f);
	number_[0]->SpriteTransferVertexBuffer(number_[0], spriteCommon_, 10);
	number_[0]->SpriteUpdate(number_[0], spriteCommon_);
	// 1
	number_[1] = new Sprite();
	number_[1]->LoadTexture(spriteCommon_, 6, L"Resources/Image/1.png");
	number_[1]->SpriteCreate(1280, 720, 6, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[1]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[1]->SetPosition({ 1200, 620, 0 });
	number_[1]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[1]->SetRotation(0.0f);
	number_[1]->SpriteTransferVertexBuffer(number_[1], spriteCommon_, 6);
	number_[1]->SpriteUpdate(number_[1], spriteCommon_);
	// 敵のHP
	enemyHp_ = new Sprite();
	enemyHp_->LoadTexture(spriteCommon_, 7, L"Resources/Image/enemyHp.png");
	enemyHp_->SpriteCreate(1280, 720, 7, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHp_->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHp_->SetPosition(enemyHpPosition_);
	enemyHp_->SetScale(enemyHpScale_);
	enemyHp_->SetRotation(0.0f);
	enemyHp_->SpriteTransferVertexBuffer(enemyHp_, spriteCommon_, 7);
	enemyHp_->SpriteUpdate(enemyHp_, spriteCommon_);
	// 敵のHPバー
	enemyHpBar_ = new Sprite();
	enemyHpBar_->LoadTexture(spriteCommon_, 8, L"Resources/Image/enemyHpBar.png");
	enemyHpBar_->SpriteCreate(1280, 720, 8, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHpBar_->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHpBar_->SetPosition(enemyHpBarPosition_);
	enemyHpBar_->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	enemyHpBar_->SetRotation(0.0f);
	enemyHpBar_->SpriteTransferVertexBuffer(enemyHpBar_, spriteCommon_, 8);
	enemyHpBar_->SpriteUpdate(enemyHpBar_, spriteCommon_);
	// 敵のHP背景
	enemyHpBack_ = new Sprite();
	enemyHpBack_->LoadTexture(spriteCommon_, 9, L"Resources/Image/enemyHpBack.png");
	enemyHpBack_->SpriteCreate(1280, 720, 9, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHpBack_->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHpBack_->SetPosition(enemyHpBackPosition_);
	enemyHpBack_->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	enemyHpBack_->SetRotation(0.0f);
	enemyHpBack_->SpriteTransferVertexBuffer(enemyHpBack_, spriteCommon_, 9);
	enemyHpBack_->SpriteUpdate(enemyHpBack_, spriteCommon_);
	// ダメージ
	damage_ = new Sprite();
	damage_->LoadTexture(spriteCommon_, 11, L"Resources/Image/damage.png");
	damage_->SpriteCreate(1280, 720, 11, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	damage_->SetColor(XMFLOAT4(1, 1, 1, 1));
	damage_->SetPosition({ 0.0f, 0.0f, 0.0f});
	damage_->SetScale(XMFLOAT2(1280 * 1, 720 * 1));
	damage_->SetRotation(0.0f);
	damage_->SpriteTransferVertexBuffer(damage_, spriteCommon_, 11);
	damage_->SpriteUpdate(damage_, spriteCommon_);

	//===== Whiteの描画 =====//
	white_ = new Sprite();
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
	white_->SpriteTransferVertexBuffer(white_, spriteCommon_, 12);

	//===== Blackの描画 =====//
	black_ = new Sprite();
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
	black_->SpriteTransferVertexBuffer(black_, spriteCommon_, 13);
	black_->SpriteUpdate(black_, spriteCommon_);
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
