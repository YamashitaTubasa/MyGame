/**
 * @file GameOverScene.cpp
 * @brief ゲームオーバーシーンのクラス
 * @author Yamashita Tubasa
 */

#include "GameOverScene.h"

using namespace DirectX;

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
	delete sprite_;
	delete over_;
	delete black_;
	delete space_;
	delete skydome_;
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	// スプライト
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	//===== 背景黒の描画 =====//
	black_ = new Sprite();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, 0, L"Resources/Image/black.png");
	// スプライトの生成
	black_->SpriteCreate(1280, 720, 0, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor(XMFLOAT4(1, 1, 1, 1));
	black_->SetPosition({ 0,0,0 });
	black_->SetScale({ 1280,720 });
	black_->SetRotation(0.0f);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_, spriteCommon_, 0);
	black_->SpriteUpdate(black_, spriteCommon_);

	//===== ゲームオーバー描画 =====//
	over_ = new Sprite();
	// テクスチャの読み込み
	over_->LoadTexture(spriteCommon_, 1, L"Resources/Image/gameOver.png");
	// スプライトの生成
	over_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	over_->SetColor(XMFLOAT4(1, 1, 1, 1));
	over_->SetPosition(overPos_);
	over_->SetScale(overScale_);
	over_->SetRotation(0.0f);
	// スプライトの頂点バッファの転送
	over_->SpriteTransferVertexBuffer(over_, spriteCommon_, 1);
	over_->SpriteUpdate(over_, spriteCommon_);

	//===== SPACEの描画 =====//
	space_ = new Sprite();
	// テクスチャの読み込み
	space_->LoadTexture(spriteCommon_, 2, L"Resources/Image/pressSpace2.png");
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
	fBlack_ = new Sprite();
	// テクスチャの読み込み
	fBlack_->LoadTexture(spriteCommon_, 3, L"Resources/Image/black.png");
	// スプライトの生成
	fBlack_->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	fBlack_->SetColor({ 1,1,1,1 });
	fBlack_->SetPosition({ 0,0,0 });
	fBlack_->SetScale({ 1280,720 });
	fBlack_->SetRotation(0.0f);
	fBlack_->SetAlpha(bAlpha_);
	// スプライトの頂点バッファの転送
	fBlack_->SpriteTransferVertexBuffer(fBlack_, spriteCommon_, 3);

	// 天球の初期化
	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GameOverScene::Update()
{
	timer_++;

	// 十字キーの右を押したら
	if (isFadeIn_ == false && isFadeOut_ == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || timer_ >= 500) {
			isFadeIn_ = true;
		}
	}

	if (isScene_ == true) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// 天球の更新
	skydome_->Update();

	// スプライトの更新
	black_->SpriteUpdate(black_, spriteCommon_);
	space_->SpriteUpdate(space_, spriteCommon_);
	fBlack_->SpriteUpdate(black_, spriteCommon_);

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
			isScene_ = true;
		}
	}
	fBlack_->SetAlpha(bAlpha_);

	// Press SPACEの描画の点滅処理
	GameOverScene::FlashSpace();
}

void GameOverScene::Draw()
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

	// タイトルの描画
	black_->SpriteDraw(spriteCommon_);
	over_->SpriteDraw(spriteCommon_);
	if (isSpace_) {
		space_->SpriteDraw(spriteCommon_);
	}
	if (isFadeIn_ == true || isFadeOut_ == true) {
		fBlack_->SpriteDraw(spriteCommon_);
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameOverScene::Finalize()
{
}

void GameOverScene::FlashSpace()
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
