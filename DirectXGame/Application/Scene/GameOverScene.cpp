/**
 * @file GameOverScene.cpp
 * @brief ゲームオーバーシーンのクラス
 * @author Yamashita Tubasa
 */

#include "GameOverScene.h"

using namespace DirectX;

GameOverScene::GameOverScene(){}

GameOverScene::~GameOverScene(){}

void GameOverScene::Initialize()
{
	dxCommon_ = MyEngine::DirectXCommon::GetInstance();

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

	// タイマー
	if (timer_ > 0 && timer_ < 60) {
		isTimer_[10] = true;
	}
	else if (timer_ > 60 && timer_ < 120) {
		isTimer_[10] = false;
		isTimer_[9] = true;
	}
	else if (timer_ > 120 && timer_ < 180) {
		isTimer_[9] = false;
		isTimer_[8] = true;
	}
	else if (timer_ > 180 && timer_ < 240) {
		isTimer_[8] = false;
		isTimer_[7] = true;
	}
	else if (timer_ > 240 && timer_ < 300) {
		isTimer_[7] = false;
		isTimer_[6] = true;
	}
	else if (timer_ > 300 && timer_ < 360) {
		isTimer_[6] = false;
		isTimer_[5] = true;
	}
	else if (timer_ > 360 && timer_ < 420) {
		isTimer_[5] = false;
		isTimer_[4] = true;
	}
	else if (timer_ > 420 && timer_ < 480) {
		isTimer_[4] = false;
		isTimer_[3] = true;
	}
	else if (timer_ > 480 && timer_ < 540) {
		isTimer_[3] = false;
		isTimer_[2] = true;
	}
	else if (timer_ > 540 && timer_ < 600) {
		isTimer_[2] = false;
		isTimer_[1] = true;
		number_[1]->SetPosition({ 70, 630, 0 });
		number_[1]->SpriteUpdate(number_[1].get(), spriteCommon_);
	}
	else if (timer_ > 600) {
		isTimer_[1] = false;
		isTimer_[0] = true;
	}
}

void GameOverScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

	// 描画前処理
	dxCommon_->PreDraw();

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

	// タイトルの描画
	black_->SpriteDraw(spriteCommon_);
	over_->SpriteDraw(spriteCommon_);
	
	if (isSpace_ == true) {
		space_->SpriteDraw(spriteCommon_);
	}

	// タイマーの描画
	if (isTimer_[10] == false) {
		num0_[0]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[0] || isTimer_[10]) {
		num0_[1]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[1]) {
		number_[1]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[2]) {
		number_[2]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[3]) {
		number_[3]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[4]) {
		number_[4]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[5]) {
		number_[5]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[6]) {
		number_[6]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[7]) {
		number_[7]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[8]) {
		number_[8]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[9]) {
		number_[9]->SpriteDraw(spriteCommon_);
	}
	if (isTimer_[10]) {
		number_[1]->SpriteDraw(spriteCommon_);
	}
	if (isFadeIn_ == true || isFadeOut_ == true) {
		fBlack_->SpriteDraw(spriteCommon_);
	}

	// スプライト描画後処理
	MyEngine::Sprite::PostDraw();

#pragma endregion

	// 描画後処理
	dxCommon_->PostDraw();
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
		if (MyEngine::Input::GetInstance()->TriggerKey(DIK_SPACE) || timer_ >= changeSceneTimer_) {
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
	sprite_ = std::make_unique<MyEngine::Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	MyEngine::PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	//===== 背景黒の描画 =====//
	black_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	black_->LoadTexture(spriteCommon_, blackNum, L"Resources/Image/black.png");
	// スプライトの生成
	black_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_height, blackNum, spriteCommon_, blackAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	black_->SetColor(blackColor_);
	black_->SetPosition(blackPos_);
	black_->SetScale(blackScale_);
	// スプライトの頂点バッファの転送
	black_->SpriteTransferVertexBuffer(black_.get(), spriteCommon_, blackNum);

	//===== ゲームオーバー描画 =====//
	over_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	over_->LoadTexture(spriteCommon_, overNum, L"Resources/Image/gameOver.png");
	// スプライトの生成
	over_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_height, overNum, spriteCommon_, overAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	over_->SetColor(overColor_);
	over_->SetPosition(overPos_);
	over_->SetScale(overScale_);
	// スプライトの頂点バッファの転送
	over_->SpriteTransferVertexBuffer(over_.get(), spriteCommon_, overNum);

	//===== SPACEの描画 =====//
	space_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	space_->LoadTexture(spriteCommon_, spaceNum, L"Resources/Image/pressSpace2.png");
	// スプライトの生成
	space_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_height, spaceNum, spriteCommon_, spaceAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	space_->SetColor(spaceColor_);
	space_->SetPosition(spacePos_);
	space_->SetScale(spaceScale_);
	space_->SetAlpha(sAlpha_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_.get(), spriteCommon_, spaceNum);

	//===== Blackの描画 =====//
	fBlack_ = std::make_unique<MyEngine::Sprite>();
	// テクスチャの読み込み
	fBlack_->LoadTexture(spriteCommon_, fBlackNum, L"Resources/Image/black.png");
	// スプライトの生成
	fBlack_->SpriteCreate(MyEngine::WinApp::window_width, MyEngine::WinApp::window_width, fBlackNum, spriteCommon_, fBlackAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	fBlack_->SetColor(fBlackColor_);
	fBlack_->SetPosition(fBlackPos_);
	fBlack_->SetScale(fBlackScale_);
	fBlack_->SetAlpha(fBlackAlpha_);
	// スプライトの頂点バッファの転送
	fBlack_->SpriteTransferVertexBuffer(fBlack_.get(), spriteCommon_, fBlackNum);

	// 0
	for (int i = 0; i < MaxNum; i++) {
		num0_[i] = std::make_unique<MyEngine::Sprite>();
		num0_[i]->LoadTexture(spriteCommon_, number0, L"Resources/Image/whiteRed0.png");
		num0_[i]->SpriteCreate(1280, 720, number0, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
		num0_[i]->SetColor(XMFLOAT4(1, 1, 1, 1));
		num0_[i]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
		num0_[i]->SetRotation(0.0f);
		num0_[i]->SpriteTransferVertexBuffer(num0_[i].get(), spriteCommon_, number0);
		num0_[i]->SpriteUpdate(num0_[i].get(), spriteCommon_);
	}
	num0_[0]->SetPosition({ 30, 630, 0 });
	num0_[1]->SetPosition({ 70, 630, 0 });
	for (int i = 0; i < MaxNum; i++) {
		num0_[i]->SpriteUpdate(num0_[i].get(), spriteCommon_);
	}
	// 1
	number_[1] = std::make_unique<MyEngine::Sprite>();
	number_[1]->LoadTexture(spriteCommon_, number1, L"Resources/Image/whiteRed1.png");
	number_[1]->SpriteCreate(1280, 720, number1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[1]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[1]->SetPosition({ 30, 630, 0 });
	number_[1]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[1]->SetRotation(0.0f);
	number_[1]->SpriteTransferVertexBuffer(number_[1].get(), spriteCommon_, number1);
	number_[1]->SpriteUpdate(number_[1].get(), spriteCommon_);
	// 2
	number_[2] = std::make_unique<MyEngine::Sprite>();
	number_[2]->LoadTexture(spriteCommon_, number2, L"Resources/Image/whiteRed2.png");
	number_[2]->SpriteCreate(1280, 720, number2, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[2]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[2]->SetPosition({ 70, 630, 0 });
	number_[2]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[2]->SetRotation(0.0f);
	number_[2]->SpriteTransferVertexBuffer(number_[2].get(), spriteCommon_, number2);
	number_[2]->SpriteUpdate(number_[2].get(), spriteCommon_);
	// 3
	number_[3] = std::make_unique<MyEngine::Sprite>();
	number_[3]->LoadTexture(spriteCommon_, number3, L"Resources/Image/whiteRed3.png");
	number_[3]->SpriteCreate(1280, 720, number3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[3]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[3]->SetPosition({ 70, 630, 0 });
	number_[3]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[3]->SetRotation(0.0f);
	number_[3]->SpriteTransferVertexBuffer(number_[3].get(), spriteCommon_, number3);
	number_[3]->SpriteUpdate(number_[3].get(), spriteCommon_);
	// 4
	number_[4] = std::make_unique<MyEngine::Sprite>();
	number_[4]->LoadTexture(spriteCommon_, number4, L"Resources/Image/whiteRed4.png");
	number_[4]->SpriteCreate(1280, 720, number4, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[4]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[4]->SetPosition({ 70, 630, 0 });
	number_[4]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[4]->SetRotation(0.0f);
	number_[4]->SpriteTransferVertexBuffer(number_[4].get(), spriteCommon_, number4);
	number_[4]->SpriteUpdate(number_[4].get(), spriteCommon_);
	// 5
	number_[5] = std::make_unique<MyEngine::Sprite>();
	number_[5]->LoadTexture(spriteCommon_, number5, L"Resources/Image/whiteRed5.png");
	number_[5]->SpriteCreate(1280, 720, number5, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[5]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[5]->SetPosition({ 70, 630, 0 });
	number_[5]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[5]->SetRotation(0.0f);
	number_[5]->SpriteTransferVertexBuffer(number_[5].get(), spriteCommon_, number5);
	number_[5]->SpriteUpdate(number_[5].get(), spriteCommon_);
	// 6
	number_[6] = std::make_unique<MyEngine::Sprite>();
	number_[6]->LoadTexture(spriteCommon_, number6, L"Resources/Image/whiteRed6.png");
	number_[6]->SpriteCreate(1280, 720, number6, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[6]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[6]->SetPosition({ 70, 630, 0 });
	number_[6]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[6]->SetRotation(0.0f);
	number_[6]->SpriteTransferVertexBuffer(number_[6].get(), spriteCommon_, number6);
	number_[6]->SpriteUpdate(number_[6].get(), spriteCommon_);
	// 7
	number_[7] = std::make_unique<MyEngine::Sprite>();
	number_[7]->LoadTexture(spriteCommon_, number7, L"Resources/Image/whiteRed7.png");
	number_[7]->SpriteCreate(1280, 720, number7, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[7]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[7]->SetPosition({ 70, 630, 0 });
	number_[7]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[7]->SetRotation(0.0f);
	number_[7]->SpriteTransferVertexBuffer(number_[7].get(), spriteCommon_, number7);
	number_[7]->SpriteUpdate(number_[7].get(), spriteCommon_);
	// 8
	number_[8] = std::make_unique<MyEngine::Sprite>();
	number_[8]->LoadTexture(spriteCommon_, number8, L"Resources/Image/whiteRed8.png");
	number_[8]->SpriteCreate(1280, 720, number8, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[8]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[8]->SetPosition({ 70, 630, 0 });
	number_[8]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[8]->SetRotation(0.0f);
	number_[8]->SpriteTransferVertexBuffer(number_[8].get(), spriteCommon_, number8);
	number_[8]->SpriteUpdate(number_[8].get(), spriteCommon_);
	// 9
	number_[9] = std::make_unique<MyEngine::Sprite>();
	number_[9]->LoadTexture(spriteCommon_, number9, L"Resources/Image/whiteRed9.png");
	number_[9]->SpriteCreate(1280, 720, number9, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	number_[9]->SetColor(XMFLOAT4(1, 1, 1, 1));
	number_[9]->SetPosition({ 70, 630, 0 });
	number_[9]->SetScale({ 64.0f * 0.9f, 64.0f * 0.9f });
	number_[9]->SetRotation(0.0f);
	number_[9]->SpriteTransferVertexBuffer(number_[9].get(), spriteCommon_, number9);
	number_[9]->SpriteUpdate(number_[9].get(), spriteCommon_);
}
