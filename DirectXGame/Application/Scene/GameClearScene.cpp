/**
 * @file GameClearScene.cpp
 * @brief クリアシーンのクラス
 * @author Yamashita Tubasa
 */

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
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// タイトル
	clear_ = new Sprite();
	clear_->LoadTexture(spriteCommon_, 1, L"Resources/Image/gameClear.png");
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
	space_->LoadTexture(spriteCommon_, 2, L"Resources/Image/pressSpace.png");
	// スプライトの生成
	space_->SpriteCreate(1280, 720, 2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	space_->SetColor(spaceColor_);
	space_->SetPosition(spacePos_);
	space_->SetScale(spaceScale_);
	space_->SetAlpha(sAlpha_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_, spriteCommon_, 2);

	//===== Blackの描画 =====//
	black_ = new Sprite();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, 3, L"Resources/Image/white.png");
	// スプライトの生成
	black_->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor({ 1,1,1,1 });
	black_->SetPosition({ 0,0,0 });
	black_->SetScale({ 1280,720 });
	black_->SetRotation(0.0f);
	black_->SetAlpha(bAlpha_);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_, spriteCommon_, 3);

	// 天球の初期化
	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GameClearScene::Update()
{
	timer_++;

	// SPACEを押したら
	if (isFadeIn_ == false && isFadeOut_ == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || timer_ >= 500) {
			// ゲームタイトルシーン（次シーン）を生成
			GameSceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}

	// 天球の更新
	skydome_->Update();

	// スプライトの更新
	space_->SpriteUpdate(space_, spriteCommon_);
	black_->SpriteUpdate(black_, spriteCommon_);

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
	if (isFadeIn_) {
		if (bAlpha_ < 1.0f) {
			bAlpha_ += 0.01f;
		}
		if (bAlpha_ >= 1.0f) {
			bAlpha_ = 1.0f;
			isFadeIn_ = false;
			isScene_ = true;
		}
	}
	black_->SetAlpha(bAlpha_);

	// Press SPACEの描画の点滅処理
	GameClearScene::FlashSpace();
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
	if (isFadeIn_ == true || isFadeOut_ == true) {
		black_->SpriteDraw(spriteCommon_);
	}
	if (isSpace_) {
		space_->SpriteDraw(spriteCommon_);
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameClearScene::Finalize()
{
}

void GameClearScene::FlashSpace()
{
	if (isSpace_) {
		spaceTimer_++;
		if (spaceTimer_ >= 50 && spaceTimer_ < 100) {
			if (sAlpha_ > 0.0f) {
				sAlpha_ -= 0.05f;
			}
			if (sAlpha_ <= 0.0f) {
				sAlpha_ = 0.0f;
			}
		}
		if (spaceTimer_ >= 100 && spaceTimer_ < 150) {
			if (sAlpha_ < 1.0f) {
				sAlpha_ += 0.05f;
			}
			if (sAlpha_ >= 1.0f) {
				sAlpha_ = 1.0f;
			}
		}
		if (spaceTimer_ >= 150) {
			spaceTimer_ = 0;
		}
		space_->SetAlpha(sAlpha_);
	}
}
