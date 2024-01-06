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
}

void GameClearScene::Initialize()
{
	dxCommon_ = MyEngine::DirectXCommon::GetInstance();

	// スプライトの読み込み
	GameClearScene::LoadSprite();

	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
}

void GameClearScene::Update()
{
	// シーンの切り替え処理
	GameClearScene::SceneChange();

	// 天球の更新
	skydome_->Update();

	// スプライトの更新
	clear_->SpriteUpdate(clear_.get(), spriteCommon_);
	space_->SpriteUpdate(space_.get(), spriteCommon_);
	black_->SpriteUpdate(black_.get(), spriteCommon_);

	// フェードアウト処理
	GameClearScene::Fade();

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
	MyEngine::Sprite::PreDraw(cmdList, spriteCommon_);

	//=== スプライトの描画 ===//
	clear_->SpriteDraw(spriteCommon_);
	if (isSpace_) {
		space_->SpriteDraw(spriteCommon_);
	}
	if (isFadeIn_ == true || isFadeOut_ == true) {
		black_->SpriteDraw(spriteCommon_);
	}

	// スプライト描画後処理
	MyEngine::Sprite::PostDraw();

#pragma endregion
}

void GameClearScene::Finalize()
{
}

void GameClearScene::FlashSpace()
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

void GameClearScene::Fade()
{
	if (isFadeOut_) {
		if (bAlpha_ > blackFadeMin_) {
			bAlpha_ -= blackFadeMove_;
		}
		if (bAlpha_ <= blackFadeMove_) {
			bAlpha_ = blackFadeMin_;
			isFadeOut_ = false;
		}
	}

	// フェードインの処理
	if (isFadeIn_) {
		if (bAlpha_ < blackFadeMax_) {
			bAlpha_ += blackFadeMove_;
		}
		if (bAlpha_ >= blackFadeMax_) {
			bAlpha_ = blackFadeMax_;
			isScene_ = true;
		}
	}
	black_->SetAlpha(bAlpha_);
}

void GameClearScene::SceneChange()
{
	timer_++;

	// SPACEを押したら
	if (isFadeIn_ == false && isFadeOut_ == false) {
		if (MyEngine::Input::GetInstance()->TriggerKey(DIK_SPACE) || timer_ >= changeSceneTimer_) {
			isFadeIn_ = true;
		}
	}

	if (isScene_ == true) {
		// ゲームタイトルシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClearScene::LoadSprite()
{
	// スプライト
	sprite_ = std::make_unique<MyEngine::Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	MyEngine::PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// タイトル
	clear_ = std::make_unique<MyEngine::Sprite>();
	clear_->LoadTexture(spriteCommon_, titleNum, L"Resources/Image/gameClear.png");
	clear_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_height, titleNum, spriteCommon_, defaultAnchorpoint_, false, false);
	clear_->SetColor(clearColor_);
	clear_->SetPosition(clearPos_);
	clear_->SetScale(clearScale_);
	clear_->SpriteTransferVertexBuffer(clear_.get(), spriteCommon_, titleNum);

	//===== SPACEの描画 =====//
	space_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	space_->LoadTexture(spriteCommon_, spaceNum, L"Resources/Image/pressSpace1.png");
	// スプライトの生成
	space_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_height, spaceNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	space_->SetColor(spaceColor_);
	space_->SetPosition(spacePos_);
	space_->SetScale(spaceScale_);
	space_->SetAlpha(sAlpha_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_.get(), spriteCommon_, spaceNum);

	//===== Blackの描画 =====//
	black_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, blackNum, L"Resources/Image/white.png");
	// スプライトの生成
	black_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_height, blackNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor(blackColor_);
	black_->SetPosition(blackPos_);
	black_->SetScale(blackScale_);
	black_->SetAlpha(bAlpha_);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_.get(), spriteCommon_, blackNum);
}
