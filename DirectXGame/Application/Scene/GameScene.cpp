#include "GameScene.h"

using namespace DirectX;
using namespace std;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete particleMan;
	delete particleMan1;
	delete fbxModel;
	delete fbxObject;
	delete player;
}

void GameScene::Initialize(SpriteCommon& spriteCommon)
{
	winApp = WinApp::GetInstance();
	input = Input::GetInstance();
	dXCommon = DirectXCommon::GetInstance();
	postEffect = PostEffect::GetInstance();

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
	fbxModel = new FbxModel();
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
	player->Initialize();

	// OBJの名前を指定してモデルデータを読み込む
	particle = Particle::LoadFromOBJ("Resources/Image/effect1.png");
	particle1 = Particle::LoadFromOBJ("Resources/Image/effect2.png");
	// パーティクルの生成
	particleMan = ParticleManager::Create();
	particleMan1 = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan->SetModel(particle);
	particleMan1->SetModel(particle1);

	// オブジェクトの初期化
	ObjectInitialize();
	// スプライト
	sprite = new Sprite();
	//sprite = make_shared<Sprite>(100, { 0.0f,0.0f }, { 500.0f,500.0f }, { 1,1,1,1 }, { 0.0f,0.0f }, false, false);
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
	hp->SpriteTransferVertexBuffer(hp, spriteCommon, 1);
	hp->SpriteUpdate(hp, spriteCommon_);
	// HPバー
	hpBar = new Sprite();
	hpBar->LoadTexture(spriteCommon_, 2, L"Resources/Image/hpBar.png");
	hpBar->SpriteCreate(1280, 720, 2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBar->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBar->SetPosition(hpBarPosition);
	hpBar->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	hpBar->SetRotation(0.0f);
	hpBar->SpriteTransferVertexBuffer(hpBar, spriteCommon, 2);
	hpBar->SpriteUpdate(hpBar, spriteCommon_);
	// HP背景
	hpBack = new Sprite();
	hpBack->LoadTexture(spriteCommon_, 3, L"Resources/Image/hpBack.png");
	hpBack->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBack->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBack->SetPosition(hpBackPosition);
	hpBack->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hpBack->SetRotation(0.0f);
	hpBack->SpriteTransferVertexBuffer(hpBack, spriteCommon, 3);
	hpBack->SpriteUpdate(hpBack, spriteCommon_);
	// ULT
	ult = new Sprite();
	ult->LoadTexture(spriteCommon_, 4, L"Resources/Image/ult.png");
	ult->SpriteCreate(1280, 720, 4, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	ult->SetColor(XMFLOAT4(1, 1, 1, 1));
	ult->SetPosition({ 1000,620, 0 });
	ult->SetScale({ 1600 * 0.05, 1600 * 0.05});
	ult->SetRotation(0.0f);
	ult->SpriteTransferVertexBuffer(ult, spriteCommon, 4);
	ult->SpriteUpdate(ult, spriteCommon_);
	// X
	X = new Sprite();
	X->LoadTexture(spriteCommon_, 5, L"Resources/Image/x.png");
	X->SpriteCreate(1280, 720, 5, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	X->SetColor(XMFLOAT4(1, 1, 1, 1));
	X->SetPosition({ 1100,630, 0 });
	X->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	X->SetRotation(0.0f);
	X->SpriteTransferVertexBuffer(X, spriteCommon, 5);
	X->SpriteUpdate(X, spriteCommon_);
	// 1
	number[1] = new Sprite();
	number[1]->LoadTexture(spriteCommon_, 6, L"Resources/Image/1.png");
	number[1]->SpriteCreate(1280, 720, 6, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number[1]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number[1]->SetPosition({ 1200,630, 0 });
	number[1]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number[1]->SetRotation(0.0f);
	number[1]->SpriteTransferVertexBuffer(number[1], spriteCommon, 6);
	number[1]->SpriteUpdate(number[1], spriteCommon_);

	//// パーティクルの初期化
	//ParticleInitialize();

	// FBXアニメーションの実行
	fbxObject->PlayAnimation();
}

void GameScene::Update()
{
	// カメラの更新
	camera->Update();

	// FBXオブジェクトの更新
	fbxObject->Update();

	// ポストエフェクト
	postEffect->SetBlur(false);

	// 自キャラの更新
	player->Update();

	// gTSの更新
	//gTS->Update();

	static char buf[256]{};
	static float f = 0.0f;

	/*ImGui::Text("Hello%d", 123);
	if (ImGui::Button("Save")) {
		imGuiManager->MySaveFunction();

		ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
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
	// スプライトの更新
	SpriteUpdate();
	
	if (input->TriggerKey(DIK_U)) {
		particl = true;
	}
	if (particl == true) {
		particleTime++;
	}
	if (particleTime >= 10) { particl = false; particleTime = 0; }
	if (particl == true) {
		// パーティクルの初期化
		/*particleMan->Execution(particle, -10);
		particleMan1->Execution(particle1, 10);*/
		particleMan->Execution(particle, -6.0f, 0.0f, 0.0f, 20, 1.0f, 0.0f);
		particleMan1->Execution(particle1, 6.0f, 0.0f, 0.0f, 20, 1.0f, 0.0f);
	}

	if (input->PushKey(DIK_I)) {
		//hpPosition.x += 1;
		hpScale.x -= 1;
	}
	hp->SetPosition(hpPosition);
	hp->SetScale(hpScale);
	hp->SpriteTransferVertexBuffer(hp, spriteCommon_, 1);
	hp->SpriteUpdate(hp, spriteCommon_);

	// パーティクルの更新
	particleMan->Update();
	particleMan1->Update();
}

void GameScene::Draw()
{
	// コマンドライン取得
	ID3D12GraphicsCommandList* cmdList = dXCommon->GetCommandList();

#pragma region 3Dオブジェクトの描画

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブジェクトの描画
	playerO3->Draw();
	bulletO3->Draw();
	skydomeO3->Draw();
	player->Draw();

	// FBX3Dオブジェクトの描画
	//fbxObject->Draw(dXCommon->GetCommandList());

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 

#pragma region パーティクルの描画

	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	particleMan->Draw();
	particleMan1->Draw();

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

#pragma endregion

#pragma region スプライト描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	// HPバーの描画
	hpBar->SpriteDraw(cmdList, spriteCommon_);
	// HPの背景描画
	hpBack->SpriteDraw(cmdList, spriteCommon_);
	// HPの描画
	hp->SpriteDraw(cmdList, spriteCommon_);
	// ULTの描画
	ult->SpriteDraw(cmdList, spriteCommon_);
	// Xの描画
	X->SpriteDraw(cmdList, spriteCommon_);
	// 1の描画
	number[1]->SpriteDraw(cmdList, spriteCommon_);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Finalize()
{
	// オブジェクトの解放
	ObjectFinalize();

	// スプライトの解放
	SpriteFinalize();
}

void GameScene::ObjectInitialize() 
{
	// OBJからモデルデータを読み込む
	playerM = Model::LoadFromOBJ("fighter");
	bulletM = Model::LoadFromOBJ("bullet");
	skydomeM = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクト生成
	playerO3 = Object3d::Create();
	bulletO3 = Object3d::Create();
	skydomeO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3->SetModel(playerM);
	bulletO3->SetModel(bulletM);
	skydomeO3->SetModel(skydomeM);
	// 3Dオブジェクトの位置を指定
	position[0] = { 0,-3,-35 };
	rotation[0] = { 0,0,0 };
	playerO3->SetPosition(position[0]);
	playerO3->SetScale({ 5, 5, 5 });
	playerO3->SetRotation(rotation[0]);
	//object3d[0]->SetEye(eye[0]);

	position[1] = { 0,0,50 };
	bulletO3->SetPosition(position[1]);
	bulletO3->SetScale({ 5,5,5 });
	bulletO3->SetRotation({ 0, 90, 0 });

	skydomeO3->SetPosition({ 0,30,0 });
	skydomeO3->SetScale({ 100, 100, 100 });
	skydomeO3->SetRotation({0,0,0});
}

void GameScene::ObjectUpdate()
{
	// 3Dオブジェクト更新
	playerO3->Update();
	bulletO3->Update();
	skydomeO3->Update();

	playerO3->SetPosition(position[0]);
	playerO3->SetRotation(rotation[0]);
	//object3d[0]->SetEye(eye[0]);
	bulletO3->SetPosition(position[1]);

	/*if (input->PushKey(DIK_RIGHT)){
		eye[0].x += 0.5;
	}
	if (input->PushKey(DIK_LEFT)) {
		eye[0].x -= 0.5;
	}*/

	if (input->PushKey(DIK_W)) {
		position[0].y += 0.3f;
	}
	if (input->PushKey(DIK_A)) {
		position[0].x -= 0.3f;
	}
	if (input->PushKey(DIK_S)) {
		position[0].y -= 0.3f;
	}
	if (input->PushKey(DIK_D)) {
		position[0].x += 0.3f;
	}

	position[1].z -= 1;
	if (position[1].z < -100) {
		position[1].z = 50;
	}
}

void GameScene::ObjectFinalize()
{
	// 3Dオブジェクト解放
	delete playerO3;
	delete bulletO3;
	delete skydomeO3;
	// 3Dモデル解放
	delete playerM;
	delete bulletM;
	delete skydomeM;
}

void GameScene::SpriteInitialize()
{
	
}

void GameScene::SpriteUpdate()
{
}

void GameScene::SpriteFinalize()
{
	// スプライト解放
	delete sprite;
	sprite = nullptr;
}

void GameScene::ParticleInitialize()
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

void GameScene::ParticleUpdate()
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

void GameScene::GameReset()
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

int GameScene::CheckCollision(XMFLOAT3 position, XMFLOAT3 scale) {
	////オブジェクトの座標
	//float objLeftX = position.x - scale.x;
	//float objRightX = position.x + scale.x;
	//float objTopY = position.y + scale.y;
	//float objBottomY = position.y - scale.y;
	//float objFrontZ = position.z - scale.z;
	//float objBZ = position.z + scale.z;
	////プレイヤーの座標
	//float playerLeftX = object3d[0]->GetPosition().x - object3d[0]->GetScale().x;
	//float playerRightX = object3d[0]->GetPosition().x + object3d[0]->GetScale().x;
	//float playerTopY = object3d[0]->GetPosition().y + object3d[0]->GetScale().y;
	//float playerBottomY = object3d[0]->GetPosition().y - object3d[0]->GetScale().y;
	//float playerFrontZ = object3d[0]->GetPosition().z - object3d[0]->GetScale().z;
	//float playerBZ = object3d[0]->GetPosition().z + object3d[0]->GetScale().z;

	//if (objLeftX < playerRightX && playerLeftX < objRightX) {
	//	if (objBottomY < playerTopY && playerBottomY < objTopY) {
	//		if (objFrontZ < playerBZ && playerFrontZ < objBZ) {
	//			return true;
	//		}
	//	}
	//}
	return true;
}
