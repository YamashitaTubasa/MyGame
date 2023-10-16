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
	delete skydome;
	delete player;
	delete sprite;
	delete camera;
	delete fbxObject;
	delete fbxModel;
	delete hp;
	delete hpBar;
	delete hpBack;
	delete enemyHp;
	delete enemyHpBar;
	delete enemyHpBack;
	delete ult;
	delete X;
	delete damage;
	delete particle;
	delete particleMan;
	delete enemy;
	for (int i = 0; i < 5; i++) {
		delete number[i];
	}
	delete backGroundObj;
}

void GamePlayScene::Initialize()
{
	winApp = WinApp::GetInstance();
	input = Input::GetInstance();
	dXCommon = DirectXCommon::GetInstance();
	collisionMan = CollisionManager::GetInstance();

	// カメラ
	camera = new Camera();
	camera->Initialize();

	// デバイスをセット
	FbxObject3d::SetDevice(dXCommon->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(camera);
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	// FBXの3Dオブジェクト生成とモデルのセット
	fbxObject = new FbxObject3d();
	fbxObject->Initialize();
	// モデル名を指定してファイル読み込み
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	// FBXオブジェクトにFBXモデルを割り当てる
	fbxObject->SetModel(fbxModel);
	// スケール、回転、座標
	fbxObject->SetRotation({ 0,90,0 });

	// カメラの注視点をセット
	target[0] = { 0,2.5f,0 };
	eye[0] = { 0,0,-10 };
	camera->SetTarget(target[0]);
	camera->SetEye(eye[0]);
	camera->SetDistance(8.0f);

	// 自キャラ生成
	player = new Player();
	player = Player::Create();
	player->SetCollider(new SphereCollider);

	// 敵キャラ生成
	enemy = new Enemy();
	enemy = Enemy::Create();
	enemy->SetCollider(new SphereCollider);

	// 背景のオブジェクトの初期化
	backGroundObj = new BackGroundObject();
	backGroundObj->Initialize();

	// 天球の初期化
	skydome = new Skydome();
	skydome->Initialize();

	// OBJの名前を指定してモデルデータを読み込む
	particle = Particle::LoadFromOBJ("bombEffect.png");
	// パーティクルの生成
	particleMan = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルセットする
	particleMan->SetModel(particle);
	
	// スプライトの初期化
	SpriteInitialize();

	// FBXアニメーションの実行
	fbxObject->PlayAnimation();
}

void GamePlayScene::Update()
{
	count++;

	if (player->GetPosition().z >= 90) {
		isFadeIn = true;
	}

	// シーン切り替え
	if (player->GetPositon().z >= 150) {
		isFadeIn = true;
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	// カメラの更新
	camera->Update();

	// FBXオブジェクトの更新
	fbxObject->Update();

	// 自キャラの更新
	player->Update();

	// 敵キャラ更新
	enemy->Update();
	// デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy_) {
		return enemy_->GetIsDead();
	});

	// 背景のオブジェクトの更新
	backGroundObj->Update();

	// 天球の更新
	skydome->Update();

	/*if (CheckCollision(player->GetpBulletP(), enemy->GetPosition())) {
		isEnemyDeth = true;
	}*/

	if (input->PushKey(DIK_RIGHT)) {
		eye[0].x += 0.5;
	}
	if (input->PushKey(DIK_LEFT)) {
		eye[0].x -= 0.5;
	}
	if (input->PushKey(DIK_UP)) {
		eye[0].y += 0.5;
	}
	if (input->PushKey(DIK_DOWN)) {
		eye[0].y -= 0.5;
	}
	camera->SetEye(eye[0]);
	
	if (input->TriggerKey(DIK_U)) {
		particl = true;
	}
	if (particl == true) {
		particleTime++;
	}
	if (particleTime >= 10) { 
		particl = false; 
		particleTime = 0; 
	}
	if (particl == true) {
		// パーティクルの実行
		//particleMan->Execution(particle, 0.0f, 0.0f, 0.5f, 20, 0.9f, 0.0f);
		//particleMan1->Execution(particle1, 6.0f, 0.0f, 0.0f, 20, 1.0f, 0.0f);
	}

	if (player->GetIsHp()) {
		if (hpScale.x >= 0) {
			hpScale.x -= 15;
		}
	}
	if (player->GetHp() == 8) {
		if (hpScale.x >= 0) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 7) {
		if (hpScale.x >= 0) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 6) {
		if (hpScale.x >= 100) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 5) {
		if (hpScale.x >= 0) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 4) {
		if (hpScale.x >= 150) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 3) {
		if (hpScale.x >= 100) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 2) {
		if (hpScale.x >= 50) {
			hpScale.x -= 10;
		}
	}
	if (player->GetHp() == 1) {
		if (hpScale.x >= 0) {
			hpScale.x -= 10;
		}
	}
	hp->SetPosition(hpPosition);
	hp->SetScale(hpScale);
	hp->SpriteTransferVertexBuffer(hp, spriteCommon_, 1);
	hp->SpriteUpdate(hp, spriteCommon_);
	enemyHp->SetScale(enemyHpScale);
	enemyHp->SpriteTransferVertexBuffer(enemyHp, spriteCommon_, 7);
	enemyHp->SpriteUpdate(enemyHp, spriteCommon_);

	// フェードアウト処理
	if (isFadeOut) {
		if (bAlpha_ > 0.0f) {
			bAlpha_ -= 0.01f;
		}
		if (bAlpha_ <= 0.01f) {
			bAlpha_ = 0.0f;
			isFadeOut = false;
		}
	}

	// フェードインの処理
	if (isFadeIn) {
		if (bAlpha_ < 1.0f) {
			bAlpha_ += 0.01f;
		}
		if (bAlpha_ > 1.0f) {
			bAlpha_ = 1.0f;
			isScene = true;
		}
	}
	black_->SetAlpha(bAlpha_);

	if (input->PushKey(DIK_I)) {
		isDamage = true;
	}
	if (isDamage) {
		damageTime++;
		if (hpScale.x >= 0) {
			hpScale.x -= 5;
		}
	}
	if (damageTime >= 20) {
		isDamage = false;
		damageTime = 0;
	}

	if (hpScale.x <= 0) {
		isFadeIn = true;
	}
	if (isScene) {
		// ゲームオーバー（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("OVER");
	}

	// パーティクルの更新
	particleMan->Update();
	//particleMan1->Update();

	// 全ての衝突をチェック
	collisionMan->CheckAllCollisions();
}

void GamePlayScene::Draw()
{
	// コマンドライン取得
	ID3D12GraphicsCommandList* cmdList = dXCommon->GetCommandList();

	#pragma region 3Dオブジェクトの描画

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブジェクトの描画
	skydome->Draw();
	player->Draw();
	if (!isEnemyDeth) {
		enemy->Draw();
	}

	// 背景のオブジェクトの描画
	backGroundObj->Draw();

	// FBX3Dオブジェクトの描画
	//fbxObject->Draw(dXCommon->GetCommandList());

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	#pragma endregion 

	#pragma region パーティクルの描画

	// パーティクル描画前処理
	ParticleManager::PreDraw(cmdList);

	// パーティクルの描画
	particleMan->Draw();
	//particleMan1->Draw();
	player->Effect();

	// パーティクル描画後処理
	ParticleManager::PostDraw();

	#pragma endregion

	#pragma region スプライト描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	// HPバーの描画
	hpBar->SpriteDraw(spriteCommon_);
	// HPの背景描画
	hpBack->SpriteDraw(spriteCommon_);
	// HPの描画
	hp->SpriteDraw(spriteCommon_);
	// ULTの描画
	//ult->SpriteDraw(spriteCommon_);
	// Xの描画
	//X->SpriteDraw(spriteCommon_);
	// 0の描画
	if (player->GetIsBullet() == 0) {
		//number[0]->SpriteDraw(spriteCommon_);
	}
	// 1の描画
	if (player->GetIsBullet() == 1) {
		//number[1]->SpriteDraw(spriteCommon_);
	}
	// 敵のHPバーの描画
	//enemyHpBar->SpriteDraw(spriteCommon_);
	// 敵のHPの背景描画
	//enemyHpBack->SpriteDraw(spriteCommon_);
	// 敵のHPの描画
	//enemyHp->SpriteDraw(spriteCommon_);
	// ダメージの描画
	if (isDamage) {
		damage->SpriteDraw(spriteCommon_);
	}
	// 黒
	if (isFadeIn == true || isFadeOut == true) {
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
	sprite = new Sprite();
	spriteCommon_ = sprite->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite->SpriteCreateGraphicsPipeline();

	// HP
	hp = new Sprite();
	hp->LoadTexture(spriteCommon_, 1, L"Resources/Image/hp.png");
	hp->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hp->SetColor(XMFLOAT4(1, 1, 1, 1));
	hp->SetPosition(hpPosition);
	hp->SetScale(hpScale);
	hp->SetRotation(0.0f);
	hp->SpriteTransferVertexBuffer(hp, spriteCommon_, 1);
	hp->SpriteUpdate(hp, spriteCommon_);
	// HPバー
	hpBar = new Sprite();
	hpBar->LoadTexture(spriteCommon_, 2, L"Resources/Image/hpBar.png");
	hpBar->SpriteCreate(1280, 720, 2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBar->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBar->SetPosition(hpBarPosition);
	hpBar->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	hpBar->SetRotation(0.0f);
	hpBar->SpriteTransferVertexBuffer(hpBar, spriteCommon_, 2);
	hpBar->SpriteUpdate(hpBar, spriteCommon_);
	// HP背景
	hpBack = new Sprite();
	hpBack->LoadTexture(spriteCommon_, 3, L"Resources/Image/hpBack.png");
	hpBack->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBack->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBack->SetPosition(hpBackPosition);
	hpBack->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hpBack->SetRotation(0.0f);
	hpBack->SpriteTransferVertexBuffer(hpBack, spriteCommon_, 3);
	hpBack->SpriteUpdate(hpBack, spriteCommon_);
	// ULT
	ult = new Sprite();
	ult->LoadTexture(spriteCommon_, 4, L"Resources/Image/ult.png");
	ult->SpriteCreate(1280, 720, 4, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	ult->SetColor(XMFLOAT4(1, 1, 1, 1));
	ult->SetPosition({ 1010,610, 0 });
	ult->SetScale({ 1600 * 0.05, 1600 * 0.05 });
	ult->SetRotation(0.0f);
	ult->SpriteTransferVertexBuffer(ult, spriteCommon_, 4);
	ult->SpriteUpdate(ult, spriteCommon_);
	// X
	X = new Sprite();
	X->LoadTexture(spriteCommon_, 5, L"Resources/Image/x.png");
	X->SpriteCreate(1280, 720, 5, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	X->SetColor(XMFLOAT4(1, 1, 1, 1));
	X->SetPosition({ 1120,630, 0 });
	X->SetScale({ 64.0f * 0.7f, 64.0f * 0.7f });
	X->SetRotation(0.0f);
	X->SpriteTransferVertexBuffer(X, spriteCommon_, 5);
	X->SpriteUpdate(X, spriteCommon_);
	// 0
	number[0] = new Sprite();
	number[0]->LoadTexture(spriteCommon_, 10, L"Resources/Image/0.png");
	number[0]->SpriteCreate(1280, 720, 10, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number[0]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number[0]->SetPosition({ 1200, 620, 0 });
	number[0]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number[0]->SetRotation(0.0f);
	number[0]->SpriteTransferVertexBuffer(number[0], spriteCommon_, 10);
	number[0]->SpriteUpdate(number[0], spriteCommon_);
	// 1
	number[1] = new Sprite();
	number[1]->LoadTexture(spriteCommon_, 6, L"Resources/Image/1.png");
	number[1]->SpriteCreate(1280, 720, 6, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number[1]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number[1]->SetPosition({ 1200, 620, 0 });
	number[1]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number[1]->SetRotation(0.0f);
	number[1]->SpriteTransferVertexBuffer(number[1], spriteCommon_, 6);
	number[1]->SpriteUpdate(number[1], spriteCommon_);
	// 敵のHP
	enemyHp = new Sprite();
	enemyHp->LoadTexture(spriteCommon_, 7, L"Resources/Image/enemyHp.png");
	enemyHp->SpriteCreate(1280, 720, 7, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHp->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHp->SetPosition(enemyHpPosition);
	enemyHp->SetScale(enemyHpScale);
	enemyHp->SetRotation(0.0f);
	enemyHp->SpriteTransferVertexBuffer(enemyHp, spriteCommon_, 7);
	enemyHp->SpriteUpdate(enemyHp, spriteCommon_);
	// 敵のHPバー
	enemyHpBar = new Sprite();
	enemyHpBar->LoadTexture(spriteCommon_, 8, L"Resources/Image/enemyHpBar.png");
	enemyHpBar->SpriteCreate(1280, 720, 8, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHpBar->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHpBar->SetPosition(enemyHpBarPosition);
	enemyHpBar->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	enemyHpBar->SetRotation(0.0f);
	enemyHpBar->SpriteTransferVertexBuffer(enemyHpBar, spriteCommon_, 8);
	enemyHpBar->SpriteUpdate(enemyHpBar, spriteCommon_);
	// 敵のHP背景
	enemyHpBack = new Sprite();
	enemyHpBack->LoadTexture(spriteCommon_, 9, L"Resources/Image/enemyHpBack.png");
	enemyHpBack->SpriteCreate(1280, 720, 9, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHpBack->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHpBack->SetPosition(enemyHpBackPosition);
	enemyHpBack->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	enemyHpBack->SetRotation(0.0f);
	enemyHpBack->SpriteTransferVertexBuffer(enemyHpBack, spriteCommon_, 9);
	enemyHpBack->SpriteUpdate(enemyHpBack, spriteCommon_);
	// ダメージ
	damage = new Sprite();
	damage->LoadTexture(spriteCommon_, 11, L"Resources/Image/damage.png");
	damage->SpriteCreate(1280, 720, 11, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	damage->SetColor(XMFLOAT4(1, 1, 1, 1));
	damage->SetPosition({ 0.0f, 0.0f, 0.0f});
	damage->SetScale(XMFLOAT2(1280 * 1, 720 * 1));
	damage->SetRotation(0.0f);
	damage->SpriteTransferVertexBuffer(damage, spriteCommon_, 11);
	damage->SpriteUpdate(damage, spriteCommon_);

	//===== Blackの描画 =====//
	black_ = new Sprite();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, 12, L"Resources/Image/black.png");
	// スプライトの生成
	black_->SpriteCreate(1280, 720, 12, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor({ 1,1,1,1 });
	black_->SetPosition({ 0,0,0 });
	black_->SetScale({ 1280,720 });
	black_->SetRotation(0.0f);
	black_->SetAlpha(bAlpha_);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_, spriteCommon_, 12);
}

void GamePlayScene::GameReset()
{
	playerHp = 3;
	time = 0;
}

bool GamePlayScene::CheckCollision(const DirectX::XMFLOAT3& object, const DirectX::XMFLOAT3& object1)
{
	if(std::pow((object1.x - object.x),2.0f) + std::pow((object1.y - object.y),2.0f) + 
		std::pow((object1.z - object.z), 2.0f) <= std::pow((1 + 1), 2.0f)){
		return true;
	}

	return true;
}
