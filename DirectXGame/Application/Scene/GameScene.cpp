#include "GameScene.h"

using namespace DirectX;
using namespace std;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete particleMan;
	delete fbxModel;
	delete fbxObject;
	/*delete particleMan1;*/
}

void GameScene::Initialize(SpriteCommon& spriteCommon)
{
	winApp = WinApp::GetInstance();
	input = Input::GetInstance();
	dXCommon = DirectXCommon::GetInstance();

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

	// OBJの名前を指定してモデルデータを読み込む
	particle = ParticleM::LoadFromOBJ("Resources/Image/effect1.png");
	particle1 = ParticleM::LoadFromOBJ("Resources/Image/effect2.png");
	// パーティクルの生成
	particleMan = ParticleManager::Create();
	particleMan1 = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan->SetModel(particle);
	particleMan1->SetModel(particle1);

	// gTs
	/*gTS = new GameTitleScene();
	gTS->Initialize();*/
	
	// オブジェクトの初期化
	ObjectInitialize();
	// スプライトの初期化
	//SpriteInitialize(spriteCommon);
	// スプライト
	sprite = new Sprite();
	//sprite = make_shared<Sprite>(100, { 0.0f,0.0f }, { 500.0f,500.0f }, { 1,1,1,1 }, { 0.0f,0.0f }, false, false);
	spriteCommon_ = sprite->SpriteCommonCreate(dXCommon->GetDevice(), 1280, 720);
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite->SpriteCreateGraphicsPipeline(dXCommon->GetDevice());

	// HP
	hp = new Sprite();
	hp->LoadTexture(spriteCommon_, 1, L"Resources/Image/hp.png", dXCommon->GetDevice());
	hp->SpriteCreate(dXCommon->GetDevice(), 1280, 720, 1, spriteCommon, XMFLOAT2(0.0f, 0.0f), false, false);
	hp->SetColor(XMFLOAT4(1, 1, 1, 1));
	hp->SetPosition(XMFLOAT3(30, 30, 0));
	hp->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hp->SetRotation(0.0f);
	hp->SpriteTransferVertexBuffer(hp, spriteCommon, 1);
	hp->SpriteUpdate(hp, spriteCommon_);
	// HPバー
	hpBar = new Sprite();
	hpBar->LoadTexture(spriteCommon_, 2, L"Resources/Image/hpBar.png", dXCommon->GetDevice());
	hpBar->SpriteCreate(dXCommon->GetDevice(), 1280, 720, 2, spriteCommon, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBar->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBar->SetPosition(XMFLOAT3(30, 30, 0));
	hpBar->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hpBar->SetRotation(0.0f);
	hpBar->SpriteTransferVertexBuffer(hpBar, spriteCommon, 2);
	hpBar->SpriteUpdate(hpBar, spriteCommon_);
	// HP背景
	hpBack = new Sprite();
	hpBack->LoadTexture(spriteCommon_, 3, L"Resources/Image/hpBack.png", dXCommon->GetDevice());
	hpBack->SpriteCreate(dXCommon->GetDevice(), 1280, 720, 3, spriteCommon, XMFLOAT2(0.0f, 0.0f), false, false);
	hpBack->SetColor(XMFLOAT4(1, 1, 1, 1));
	hpBack->SetPosition(XMFLOAT3(30, 30, 0));
	hpBack->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	hpBack->SetRotation(0.0f);
	hpBack->SpriteTransferVertexBuffer(hpBack, spriteCommon, 3);
	hpBack->SpriteUpdate(hpBack, spriteCommon_);

	//// パーティクルの初期化
	//ParticleInitialize();

	// FBXアニメーションの実行
	fbxObject->PlayAnimation();
}

void GameScene::Update()
{
	// ImGui受付開始
	imGuiManager->Begin();

	// 入力の更新
	input->Update();

	// カメラの更新
	camera->Update();

	// FBXオブジェクトの更新
	fbxObject->Update();

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
		particleMan->Execution(particle, 0);
	}

	// パーティクルの更新
	particleMan->Update();

	// ImGui受付終了
	imGuiManager->End();
}

void GameScene::Draw(SpriteCommon& spriteCommon)
{
#pragma region 3Dオブジェクトの描画

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dXCommon->GetCommandList());

	// 3Dオブジェクトの描画
	/*for (int i = 0; i < 5; i++) {
		object3d[i]->Draw();
	}*/
	object3d[0]->Draw();

	// FBX3Dオブジェクトの描画
	//fbxObject->Draw(dXCommon->GetCommandList());

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 

#pragma region パーティクルの描画

	// コマンドリストの取得
	//ID3D12GraphicsCommandList* cmdList = dXCommon->GetCommandList();

	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw(dXCommon->GetCommandList());

	// 3Dオブクジェクトの描画
	particleMan->Draw();
	//particleMan1->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

#pragma endregion

#pragma region スプライト描画

	Sprite::PreDraw(dXCommon->GetCommandList(), spriteCommon_);

	// HPバーの描画
	hpBar->SpriteDraw(dXCommon->GetCommandList(), spriteCommon_, dXCommon->GetDevice(), hpBar->vbView);
	// HPの背景描画
	hpBack->SpriteDraw(dXCommon->GetCommandList(), spriteCommon_, dXCommon->GetDevice(), hpBack->vbView);
	// HPの描画
	hp->SpriteDraw(dXCommon->GetCommandList(), spriteCommon_, dXCommon->GetDevice(), hp->vbView);

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
	Model[0] = Model::LoadFromOBJ("fighter", "effect1.png");
	//Model[0]->LoadTexture("effect1.png");
	Model[1] = Model::LoadFromOBJ("ironSphere", "ironShpere/ironSphere.png");
	//Model[2] = Model::LoadFromOBJ("skydome", "skydome/skydome.jpg");
	// 3Dオブジェクト生成
	for (int i = 0; i < 5; i++) {
		object3d[i] = Object3d::Create();
	}
	// オブジェクトにモデルをひも付ける
	object3d[0]->SetModel(Model[0]);
	object3d[1]->SetModel(Model[1]);
	object3d[2]->SetModel(Model[2]);
	// 3Dオブジェクトの位置を指定
	position[0] = { 0,-5,-35 };
	rotation[0] = { 0,0,0 };
	object3d[0]->SetPosition(position[0]);
	object3d[0]->SetScale({ 5, 5, 5 });
	object3d[0]->SetRotation(rotation[0]);
	//object3d[0]->SetEye(eye[0]);

	position[1] = { 0,0,50 };
	object3d[1]->SetPosition(position[1]);
	object3d[1]->SetScale({ 5,5,5 });
	object3d[1]->SetRotation({ 0, 90, 0 });

	object3d[2]->SetPosition({ 0,-40,0 });
	object3d[2]->SetScale({ 100, 100, 100 });
	object3d[2]->SetRotation({0,100,20});
}

void GameScene::ObjectUpdate()
{
	// 3Dオブジェクト更新
	for (int i = 0; i < 5; i++) {
		object3d[i]->Update();
	}

	object3d[0]->SetPosition(position[0]);
	object3d[0]->SetRotation(rotation[0]);
	//object3d[0]->SetEye(eye[0]);
	object3d[1]->SetPosition(position[1]);

	/*if (input->PushKey(DIK_RIGHT)){
		eye[0].x += 0.5;
	}
	if (input->PushKey(DIK_LEFT)) {
		eye[0].x -= 0.5;
	}*/

	if (input->PushKey(DIK_W)) {
		position[0].y += 0.4f;
	}
	if (input->PushKey(DIK_A)) {
		position[0].x -= 0.4f;
	}
	if (input->PushKey(DIK_S)) {
		position[0].y -= 0.4f;
	}
	if (input->PushKey(DIK_D)) {
		position[0].x += 0.4f;
	}

	position[1].z -= 1;
	if (position[1].z < -100) {
		position[1].z = 50;
	}

	// プレイヤーと鉄球の当たり判定
	/*if (CheckCollision(object3d[1]->GetPosition(), object3d[1]->GetScale()) == true) {
		playerHp -= 1;
	}*/
	/*if (playerHp == 0) {
		scene = GameOver;
	}*/
}

void GameScene::ObjectFinalize()
{
	// 3Dオブジェクト解放
	for (int i = 0; i < 5; i++) {
		delete object3d[i];
	}
	// 3Dモデル解放
	for (int i = 0; i < 5; i++) {
		delete Model[i];
	}
}

void GameScene::SpriteInitialize(SpriteCommon& spriteCommon)
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
