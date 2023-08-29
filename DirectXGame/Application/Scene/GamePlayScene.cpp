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
	delete skydomeM;
	delete skydomeO3;
	delete player;
	delete sprite;
	delete camera;
	delete fbxObject;
	delete hp;
	delete hpBar;
	delete hpBack;
	delete enemyHp;
	delete enemyHpBar;
	delete enemyHpBack;
	delete ult;
	delete X;
	delete particle;
	delete particle1;
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

	//// ポストエフェクト
	//postEffect = new PostEffect();
	//postEffect->Initialize();
	//postEffect1 = new PostEffect();
	//postEffect1->Initialize();

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

	// 背景のオブジェクト
	backGroundObj = new BackGroundObject();
	backGroundObj->Initialize();

	// OBJの名前を指定してモデルデータを読み込む
	particle = Particle::LoadFromOBJ("bombEffect.png");
	//particle1 = Particle::LoadFromOBJ("effect2.png");
	// パーティクルの生成
	particleMan = ParticleManager::Create();
	//particleMan1 = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan->SetModel(particle);
	//particleMan1->SetModel(particle1);

	// オブジェクトの初期化
	ObjectInitialize();
	
	// スプライトの初期化
	SpriteInitialize();

	// パーティクルの初期化
	//ParticleInitialize();

	// FBXアニメーションの実行
	fbxObject->PlayAnimation();
}

void GamePlayScene::Update()
{
	count++;

	if (player->GetPositon().z >= 150) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	// カメラの更新
	camera->Update();

	// FBXオブジェクトの更新
	fbxObject->Update();

	// ポストエフェクト
	//postEffect->SetBlur(true);

	// 自キャラの更新
	player->Update();

	// 敵キャラ更新
	enemy->Update();

	// 背景のオブジェクトの更新
	backGroundObj->Update();

	if (input->TriggerKey(DIK_O)) {
		DirectX::XMFLOAT3 a = player->GetpBulletP();
	}

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

	// オブジェクトの更新
	ObjectUpdate();
	
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
		particleMan->Execution(particle, 0.0f, 0.0f, 0.5f, 20, 0.9f, 0.0f);
		//particleMan1->Execution(particle1, 6.0f, 0.0f, 0.0f, 20, 1.0f, 0.0f);
	}

	if (input->PushKey(DIK_I)) {
		//hpPosition.x += 1;
		hpScale.x -= 1;
	}
	hp->SetPosition(hpPosition);
	hp->SetScale(hpScale);
	hp->SpriteTransferVertexBuffer(hp, spriteCommon_, 1);
	hp->SpriteUpdate(hp, spriteCommon_);

	if (player->GetHp() == 9) {
		if (enemyHpScale.x >= 300) {
			enemyHpScale.x -= 5;
		}
	}
	if (player->GetHp() == 8) {
		if (enemyHpScale.x >= 100) {
			enemyHpScale.x -= 5;
		}
	}
	if (player->GetHp() == 7) {
		if (enemyHpScale.x >= 0) {
			enemyHpScale.x -= 5;
		}
	}
	if (player->GetHp() == 6) {
		if (enemyHpScale.x >= 100) {
			enemyHpScale.x -= 2;
		}
	}
	if (player->GetHp() == 5) {
		if (enemyHpScale.x >= 0) {
			enemyHpScale.x -= 2;
		}
	}
	if (player->GetHp() == 4) {
		if (enemyHpScale.x >= 150) {
			enemyHpScale.x -= 2;
		}
	}
	if (player->GetHp() == 3) {
		if (enemyHpScale.x >= 100) {
			enemyHpScale.x -= 2;
		}
	}
	if (player->GetHp() == 2) {
		if (enemyHpScale.x >= 50) {
			enemyHpScale.x -= 2;
		}
	}
	if (player->GetHp() == 1) {
		if (enemyHpScale.x >= 0) {
			enemyHpScale.x -= 2;
		}
	}

	enemyHp->SetScale(enemyHpScale);
	enemyHp->SpriteTransferVertexBuffer(enemyHp, spriteCommon_, 7);
	enemyHp->SpriteUpdate(enemyHp, spriteCommon_);

	if (input->PushKey(DIK_I)) {
		isDamage = true;
	}
	if (isDamage) {
		damageTime++;
		if (hpScale.x >= 50) {
			hpScale.x -= 5;
		}
	}
	if (damageTime >= 20) {
		isDamage = false;
		damageTime = 0;
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
	skydomeO3->Draw();
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
	ult->SpriteDraw(spriteCommon_);
	// Xの描画
	X->SpriteDraw(spriteCommon_);
	// 0の描画
	if (player->GetIsBullet() == 0) {
		number[0]->SpriteDraw(spriteCommon_);
	}
	// 1の描画
	if (player->GetIsBullet() == 1) {
		number[1]->SpriteDraw(spriteCommon_);
	}
	// 敵のHPバーの描画
	enemyHpBar->SpriteDraw(spriteCommon_);
	// 敵のHPの背景描画
	enemyHpBack->SpriteDraw(spriteCommon_);
	// 敵のHPの描画
	enemyHp->SpriteDraw(spriteCommon_);
	// ダメージの描画
	if (isDamage) {
		damage->SpriteDraw(spriteCommon_);
	}

	// スプライト描画後処理
	Sprite::PostDraw();

	#pragma endregion

	// ImGui描画
	//imGuiManager->Draw(dXCommon);

}

void GamePlayScene::Finalize()
{
	// オブジェクトの解放
	ObjectFinalize();
}

void GamePlayScene::ObjectInitialize()
{
	// OBJからモデルデータを読み込む
	skydomeM = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクト生成
	skydomeO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	skydomeO3->SetModel(skydomeM);
	// 3Dオブジェクトの位置を指定
	skydomeO3->SetPosition({ 0,30,0 });
	skydomeO3->SetScale({ 300, 300, 300 });
	skydomeO3->SetRotation({0,0,0});
}

void GamePlayScene::ObjectUpdate()
{
	// 3Dオブジェクト更新
	skydomeO3->Update();
}

void GamePlayScene::ObjectFinalize()
{
}

void GamePlayScene::SpriteInitialize()
{
	// スプライト
	sprite = new Sprite();
	spriteCommon_ = sprite->SpriteCommonCreate(1280, 720);
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
}

void GamePlayScene::ParticleInitialize()
{
	//for (int i = 0; i < 100; i++) {
	//	// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
	//	const float md_pos = 10.0f;
	//	XMFLOAT3 pos{};
	//	float posx = -5;
	//	pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + posx;
	//	pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
	//	// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
	//	const float md_vel = 0.1f;
	//	XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	// 重力に見立ててYのみ[-0.001f,0]でランダム分布
	//	XMFLOAT3 acc{};
	//	const float md_acc = 0.001f;
	//	acc.y = (float)rand() / RAND_MAX * md_acc;

	//	// 追加
	//	particleMan->Add(20, pos, vel, acc, 1.0f, 0.0);
	//}
}

void GamePlayScene::ParticleUpdate()
{
	//// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//{
	//	if (input->PushKey(DIK_W)) { ParticleManager::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input->PushKey(DIK_S)) { ParticleManager::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input->PushKey(DIK_D)) { ParticleManager::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input->PushKey(DIK_A)) { ParticleManager::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
	//}

	//particleMan->Update();
}

void GamePlayScene::GameReset()
{
	// 3Dオブジェクトの位置を指定
	position[0] = { -20,-5,0 };
	rotation[0] = { 0,90,0 };
	object3d[0]->SetPosition(position[0]);
	object3d[0]->SetScale({ 5, 5, 5 });
	object3d[0]->SetRotation(rotation[0]);
	position[1] = { 0,5,50 };
	object3d[1]->SetPosition(position[1]);
	object3d[1]->SetRotation({ 0, 90, 0 });
	object3d[1]->SetScale({ 5,5,5 });

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
