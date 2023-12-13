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
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	// スプライトの読み込み
	GameOverScene::LoadSprite();

	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
}

void GameOverScene::Update()
{
	// シーンの切り替え処理
	GameOverScene::SceneChange();

	// 天球の更新
	skydome_->Update();

	// スプライトの更新
	black_->SpriteUpdate(black_.get(), spriteCommon_);
	over_->SpriteUpdate(over_.get(), spriteCommon_);
	space_->SpriteUpdate(space_.get(), spriteCommon_);
	fBlack_->SpriteUpdate(fBlack_.get(), spriteCommon_);

	// Press SPACEの点滅処理
	GameOverScene::FlashSpace();

	// フェード処理
	GameOverScene::Fade();
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
	
	if (isSpace_ == true) {
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
		if (spaceTimer_ >= spaceTimerOne_ && spaceTimer_ < spaceTimerTwo_) {
			if (sAlpha_ > sAlphaMin_) {
				sAlpha_ -= sAlphaMove_;
			}
			if (sAlpha_ <= sAlphaMin_) {
				sAlpha_ = sAlphaMin_;
			}
		}
		if (spaceTimer_ >= spaceTimerTwo_ && spaceTimer_ < spaceTimerMax_) {
			if (sAlpha_ < sAlphaMax_) {
				sAlpha_ += sAlphaMove_;
			}
			if (sAlpha_ >= sAlphaMax_) {
				sAlpha_ = sAlphaMax_;
			}
		}
		if (spaceTimer_ >= spaceTimerMax_) {
			spaceTimer_ = spaceTimerMin_;
		}
		space_->SetAlpha(sAlpha_);
	}
}

void GameOverScene::Fade()
{
	// フェードアウト処理
	if (isFadeOut_) {
		if (fBlackAlpha_ > fBlackFadeMin_) {
			fBlackAlpha_ -= fBlackFadeMove_;
		}
		if (fBlackAlpha_ <= fBlackFadeMove_) {
			fBlackAlpha_ = fBlackFadeMin_;
			isFadeOut_ = false;
		}
	}

	// フェードインの処理
	if (isFadeIn_) {
		if (fBlackAlpha_ < fBlackFadeMax_) {
			fBlackAlpha_ += fBlackFadeMove_;
		}
		if (fBlackAlpha_ >= fBlackFadeMax_) {
			fBlackAlpha_ = fBlackFadeMax_;
			isScene_ = true;
		}
	}
	fBlack_->SetAlpha(fBlackAlpha_);
}

void GameOverScene::SceneChange()
{
	timer_++;

	// 十字キーの右を押したら
	if (isFadeIn_ == false && isFadeOut_ == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || timer_ >= changeSceneTimer_) {
			isFadeIn_ = true;
		}
	}

	if (isScene_ == true) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::LoadSprite()
{
	// スプライト
	sprite_ = std::make_unique<Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	//===== 背景黒の描画 =====//
	black_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, blackNum, L"Resources/Image/black.png");
	// スプライトの生成
	black_->SpriteCreate(WinApp::window_width, WinApp::window_height, blackNum, spriteCommon_, blackAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor(blackColor_);
	black_->SetPosition(blackPos_);
	black_->SetScale(blackScale_);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_.get(), spriteCommon_, blackNum);

	//===== ゲームオーバー描画 =====//
	over_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	over_->LoadTexture(spriteCommon_, overNum, L"Resources/Image/gameOver.png");
	// スプライトの生成
	over_->SpriteCreate(WinApp::window_width, WinApp::window_height, overNum, spriteCommon_, overAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	over_->SetColor(overColor_);
	over_->SetPosition(overPos_);
	over_->SetScale(overScale_);
	// スプライトの頂点バッファの転送
	over_->SpriteTransferVertexBuffer(over_.get(), spriteCommon_, overNum);

	//===== SPACEの描画 =====//
	space_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	space_->LoadTexture(spriteCommon_, spaceNum, L"Resources/Image/pressSpace2.png");
	// スプライトの生成
	space_->SpriteCreate(WinApp::window_width, WinApp::window_height, spaceNum, spriteCommon_, spaceAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	space_->SetColor(spaceColor_);
	space_->SetPosition(spacePos_);
	space_->SetScale(spaceScale_);
	space_->SetAlpha(sAlpha_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_.get(), spriteCommon_, spaceNum);

	//===== Blackの描画 =====//
	fBlack_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	fBlack_->LoadTexture(spriteCommon_, fBlackNum, L"Resources/Image/black.png");
	// スプライトの生成
	fBlack_->SpriteCreate(WinApp::window_width, WinApp::window_width, fBlackNum, spriteCommon_, fBlackAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	fBlack_->SetColor(fBlackColor_);
	fBlack_->SetPosition(fBlackPos_);
	fBlack_->SetScale(fBlackScale_);
	fBlack_->SetAlpha(fBlackAlpha_);
	// スプライトの頂点バッファの転送
	fBlack_->SpriteTransferVertexBuffer(fBlack_.get(), spriteCommon_, fBlackNum);
}
