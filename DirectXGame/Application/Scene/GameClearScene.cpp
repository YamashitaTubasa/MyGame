#include "GameClearScene.h"

using namespace DirectX;

GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
	delete sprite_;
	delete clear_;
	delete space_;
	delete skydome_;
}

void GameClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	// スプライト
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(1280, 720);
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// タイトル
	clear_ = new Sprite();
	clear_->LoadTexture(spriteCommon_, 1, L"Resources/Image/clear2.png");
	clear_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	clear_->SetColor(XMFLOAT4(1, 1, 1, 1));
	clear_->SetPosition(clearPos_);
	clear_->SetScale(clearScale_);
	clear_->SetRotation(0.0f);
	clear_->SpriteTransferVertexBuffer(clear_, spriteCommon_, 1);
	clear_->SpriteUpdate(clear_, spriteCommon_);

	//===== SPACEの描画 =====//
	space_ = new Sprite();
	// テクスチャの読み込み
	space_->LoadTexture(spriteCommon_, 2, L"Resources/Image/space2.png");
	// スプライトの生成
	space_->SpriteCreate(1280, 720, 2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	space_->SetColor(spaceColor_);
	space_->SetPosition(spacePos_);
	space_->SetScale(spaceScale_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_, spriteCommon_, 2);

	// 天球の初期化
	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GameClearScene::Update()
{
	// SPACEを押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームタイトルシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// 天球の更新
	skydome_->Update();

	// スプライトの更新
	space_->SpriteUpdate(space_, spriteCommon_);

	// Press SPACEの描画の点滅処理
	spaceTimer++;
	if (spaceTimer >= 50 && spaceTimer <= 99) {
		isSpace = false;
	}
	if (spaceTimer >= 100) {
		isSpace = true;
		spaceTimer = 0;
	}
}

void GameClearScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

#pragma region オブジェクトの描画

	// オブジェクトの前処理
	Object3d::PreDraw(cmdList);

	//=== オブジェクトの描画 ===//
	skydome_->Draw();

	// オブジェクトの後処理
	Object3d::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	//=== スプライトの描画 ===//
	clear_->SpriteDraw(spriteCommon_);
	if (isSpace) {
		space_->SpriteDraw(spriteCommon_);
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameClearScene::Finalize()
{
}
