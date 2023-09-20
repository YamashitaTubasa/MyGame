#include "GameTitleScene.h"

using namespace DirectX;

GameTitleScene::GameTitleScene()
{
}

GameTitleScene::~GameTitleScene()
{
	delete sprite_;
	delete title_;
}

void GameTitleScene::Initialize()
{
	// DirectXCommonのインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();

	// 天球の初期化
	skydome = new Skydome();
	skydome->Initialize();

	// スプライトの初期化
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(1280, 720);

	//===== タイトル描画の初期化 =====//
	title_ = new Sprite();
	// テクスチャの読み込み
	title_->LoadTexture(spriteCommon_, 1, L"Resources/Image/title2.png");
	// スプライトの生成
	title_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	title_->SetColor(titleColor_);
	title_->SetPosition(titlePos_);
	title_->SetScale(titleScale_);
	title_->SetRotation(titleRot_);
	// スプライトの頂点バッファの転送
	title_->SpriteTransferVertexBuffer(title_, spriteCommon_, 1);

	// OBJモデルの読み込み
	model = Model::LoadFromOBJ("fighter");
	// オブジェクトの生成
	object3d = Object3d::Create();
	// オブジェクトにモデルをセット
	object3d->SetModel(model);
	// オブジェクトの位置の設定
	object3d->SetPosition(pPos);
	object3d->SetScale({5,5,5});
	object3d->SetRotation({0,0,0});
	object3d->SetTarget(object3d->GetPosition());
	object3d->SetEye(eye);
}

void GameTitleScene::Update()
{
	// 天球の更新
	skydome->Update();

	// 自機の更新
	object3d->Update();

	// スプライトの更新
	title_->SpriteUpdate(title_, spriteCommon_);

	// シーンの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		start = true;
	}
	if (start) {
		startTimer++;
		pPos.z += 1.5f;
		pPos.y += 0.25f;
		eye.y -= 0.06f;

		object3d->SetPosition(pPos);
		object3d->SetEye(eye);
	}
	if (startTimer >= 200) {
		start = false;
		startTimer = 0;
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void GameTitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

#pragma region オブジェクトの描画

	// オブジェクトの前処理
	Object3d::PreDraw(cmdList);

	//=== オブジェクトの描画 ===//
	skydome->Draw();
	object3d->Draw();

	// オブジェクトの後処理
	Object3d::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	//=== スプライトの描画 ===//
	if (!start) {
		title_->SpriteDraw(spriteCommon_);
	}
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameTitleScene::Finalize()
{
}
