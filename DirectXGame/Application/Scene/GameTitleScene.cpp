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

	// スプライトの初期化
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(1280, 720);

	//===== タイトル描画の初期化 =====//
	title_ = new Sprite();
	// テクスチャの読み込み
	title_->LoadTexture(spriteCommon_, 1, L"Resources/Image/title.png");
	// スプライトの生成
	title_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	title_->SetColor(titleColor_);
	title_->SetPosition(titlePos_);
	title_->SetScale(titleScale_);
	title_->SetRotation(titleRot_);
	// スプライトの頂点バッファの転送
	title_->SpriteTransferVertexBuffer(title_, spriteCommon_, 1);
}

void GameTitleScene::Update()
{
	// スプライトの更新
	title_->SpriteUpdate(title_, spriteCommon_);

	// シーンの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void GameTitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

#pragma region スプライトの描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	//=== スプライトの描画 ===//
	title_->SpriteDraw(spriteCommon_);
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameTitleScene::Finalize()
{
}
