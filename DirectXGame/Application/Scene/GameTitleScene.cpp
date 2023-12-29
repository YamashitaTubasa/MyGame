/**
 * @file GameTitleScene.cpp
 * @brief ゲームタイトルシーンのクラス
 * @author Yamashita Tubasa
 */

#include "GameTitleScene.h"

#include "EasingManager.h"

GameTitleScene::GameTitleScene()
{
}

GameTitleScene::~GameTitleScene()
{
}

void GameTitleScene::Initialize()
{
	// DirectXCommonのインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();

	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// スプライトの読み込み
	LoadSprite();

	// OBJモデルの読み込み
	playerModel_ = Model::LoadFromOBJ("fighter");
	// オブジェクトの生成
	player_ = Object3d::Create();
	// オブジェクトにモデルをセット
	player_->SetModel(playerModel_.get());
	// オブジェクトの位置の設定
	player_->SetPosition(pPos_);
	player_->SetScale(pScale_);
	player_->SetRotation(pRot_);
	player_->SetTarget(player_->GetPosition());
	player_->SetEye(eye_);
}

void GameTitleScene::Update()
{
	// 天球の更新
	skydome_->Update();
	// 天球の回転処理
	skydome_->RotateSky();

	// 自機の更新
	player_->Update();

	// スプライトの更新
	title_->SpriteUpdate(title_.get(), spriteCommon_);
	space_->SpriteUpdate(space_.get(), spriteCommon_);
	inBlack_->SpriteUpdate(inBlack_.get(), spriteCommon_);
	nowLoading_->SpriteUpdate(nowLoading_.get(), spriteCommon_);
	/*for (auto& i : dot_) {
		i->SpriteUpdate(i, spriteCommon_);
	}*/
	for (int i = 0; i < dotMax_; i++) {
		dot_[i]->SpriteUpdate(dot_[i].get(), spriteCommon_);
	}
	
	// フェードイン・アウトの処理
	GameTitleScene::Fade();

	// Press SPACEの描画の点滅処理
	GameTitleScene::FlashSpace();

	// ロード中の点の点滅処理
	GameTitleScene::Load();

	// 自機の移動
	if (isPos_ == false) {
		pPos_.y += playerPosYMove_;
	}
	if (pPos_.y >= playerPosYMin_) {
		isPos_ = true;
	}
	if (isPos_ == true) {
		pPos_.y -= playerPosYMove_;
	}
	if (pPos_.y <= playerPosYMax_) {
		isPos_ = false;
	}
	player_->SetPosition(pPos_);

	// シーンの切り替え処理
	GameTitleScene::SceneChange();

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		GameSceneManager::GetInstance()->ChangeScene("PLAY");
	}
	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		GameSceneManager::GetInstance()->ChangeScene("OVER");
	}
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

#endif
}

void GameTitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

#pragma region オブジェクトの描画

	// オブジェクトの前処理
	Object3d::PreDraw(cmdList);

	//=== オブジェクトの描画 ===//
	skydome_->Draw();
	player_->Draw();

	// オブジェクトの後処理
	Object3d::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	//=== スプライトの描画 ===//
	if (!start_) {
		title_->SpriteDraw(spriteCommon_);
		if (isSpace_) {
			space_->SpriteDraw(spriteCommon_);
		}
	}

	// フェードインとフェードアウトの描画
	if (isFadeIn_ == true || isFadeOut_ == true) {
		inBlack_->SpriteDraw(spriteCommon_);
	}

	// NowLodingと点の描画
	if (isLoding_) {
		nowLoading_->SpriteDraw(spriteCommon_);
		/*for (auto& i : dot_) {
			i->SpriteDraw(spriteCommon_);
		}*/
		for (int i = 0; i < dotMax_; i++) {
			if (isDot_[i]) {
				dot_[i]->SpriteDraw(spriteCommon_);
			}
		}
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameTitleScene::Finalize()
{
}

void GameTitleScene::LoadSprite()
{
	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate();

	//===== タイトル描画の初期化 =====//
	title_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	title_->LoadTexture(spriteCommon_, titleNum, L"Resources/Image/title.png");
	// スプライトの生成
	title_->SpriteCreate(WinApp::window_width, WinApp::window_height, titleNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	title_->SetColor(titleColor_);
	title_->SetPosition(titlePos_);
	title_->SetScale(titleScale_);
	title_->SetRotation(titleRot_);
	// スプライトの頂点バッファの転送
	title_->SpriteTransferVertexBuffer(title_.get(), spriteCommon_, titleNum);

	//===== SPACEの描画 =====//
	space_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	space_->LoadTexture(spriteCommon_, spaceNum, L"Resources/Image/pressSpace1.png");
	// スプライトの生成
	space_->SpriteCreate(WinApp::window_width, WinApp::window_height, spaceNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	space_->SetColor(spaceColor_);
	space_->SetPosition(spacePos_);
	space_->SetScale(spaceScale_);
	space_->SetRotation(spaceRot_);
	space_->SetAlpha(sAlpha_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_.get(), spriteCommon_, spaceNum);

	//===== InBlackの描画 =====//
	inBlack_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	inBlack_->LoadTexture(spriteCommon_, inBlackNum, L"Resources/Image/white.png");
	// スプライトの生成
	inBlack_->SpriteCreate(WinApp::window_width, WinApp::window_height, inBlackNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	inBlack_->SetColor(inBlackColor_);
	inBlack_->SetPosition(inBlackPosition_);
	inBlack_->SetScale(inBlackScale_);
	inBlack_->SetAlpha(bInAlpha_);
	// スプライトの頂点バッファの転送
	inBlack_->SpriteTransferVertexBuffer(inBlack_.get(), spriteCommon_, inBlackNum);

	//===== ロードの描画 =====//
	nowLoading_ = std::make_unique<Sprite>();
	// テクスチャの読み込み
	nowLoading_->LoadTexture(spriteCommon_, nowLoadingNum, L"Resources/Image/nowLoading.png");
	// スプライトの生成
	nowLoading_->SpriteCreate(WinApp::window_width, WinApp::window_height, nowLoadingNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	nowLoading_->SetColor(nowLoadingColor_);
	nowLoading_->SetPosition(nowLoadingPos_);
	nowLoading_->SetScale(nowLoadingScale_);
	nowLoading_->SetAlpha(lodingAlpha_);
	// スプライトの頂点バッファの転送
	nowLoading_->SpriteTransferVertexBuffer(nowLoading_.get(), spriteCommon_, nowLoadingNum);

	//===== 点の描画 =====//
	dot_[0] = std::make_unique<Sprite>();
	// テクスチャの読み込み
	dot_[0]->LoadTexture(spriteCommon_, dotZeroNum, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[0]->SpriteCreate(WinApp::window_width, WinApp::window_height, dotZeroNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[0]->SetColor(dotColor_[0]);
	dot_[0]->SetPosition(dotPos_[0]);
	dot_[0]->SetScale(dotScale_[0]);
	dot_[0]->SetAlpha(dotAlpha_[0]);
	// スプライトの頂点バッファの転送
	dot_[0]->SpriteTransferVertexBuffer(dot_[0].get(), spriteCommon_, dotZeroNum);

	dot_[1] = std::make_unique<Sprite>();
	// テクスチャの読み込み
	dot_[1]->LoadTexture(spriteCommon_, dotOneNum, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[1]->SpriteCreate(WinApp::window_width, WinApp::window_height, dotOneNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[1]->SetColor(dotColor_[1]);
	dot_[1]->SetPosition(dotPos_[1]);
	dot_[1]->SetScale(dotScale_[1]);
	dot_[1]->SetAlpha(dotAlpha_[1]);
	// スプライトの頂点バッファの転送
	dot_[1]->SpriteTransferVertexBuffer(dot_[1].get(), spriteCommon_, dotOneNum);

	dot_[2] = std::make_unique<Sprite>();
	// テクスチャの読み込み
	dot_[2]->LoadTexture(spriteCommon_, dotTwoNum, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[2]->SpriteCreate(WinApp::window_width, WinApp::window_height, dotTwoNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[2]->SetColor(dotColor_[2]);
	dot_[2]->SetPosition(dotPos_[2]);
	dot_[2]->SetScale(dotScale_[2]);
	dot_[2]->SetAlpha(dotAlpha_[2]);
	// スプライトの頂点バッファの転送
	dot_[2]->SpriteTransferVertexBuffer(dot_[2].get(), spriteCommon_, dotTwoNum);
}

void GameTitleScene::Fade()
{
	// フェードアウト処理
	if (isFadeOut_) {
		if (bInAlpha_ > bFadeMin_) {
			bInAlpha_ -= bFadeMove_;
		}
		if (bInAlpha_ <= bFadeMove_) {
			bInAlpha_ = bFadeMin_;
			isFadeOut_ = false;
		}
	}

	// フェードインの処理
	if (isFadeIn_) {
		if (bInAlpha_ < bFadeMax_) {
			bInAlpha_ += bFadeMove_;
		}
	}
	inBlack_->SetAlpha(bInAlpha_);
}

void GameTitleScene::FlashSpace()
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

void GameTitleScene::Load()
{
	if (bInAlpha_ >= bInAlphaMax_) {
		isLoding_ = true;
	}
	if (isLoding_) {
		dotTimer_++;
		if (dotTimer_ >= dotTimerMin_ && dotTimer_ < dotTimerOne_) {
			isDot_[0] = true;
		}
		if (dotTimer_ >= dotTimerOne_ && dotTimer_ < dotTimerTwo_) {
			isDot_[1] = true;
		}
		if (dotTimer_ >= dotTimerTwo_ && dotTimer_ < dotTimerThree_) {
			isDot_[2] = true;
		}
		if (dotTimer_ >= dotTimerMax_) {
			for (int i = 0; i < dotMax_; i++) {
				isDot_[i] = false;
			}
			dotTimer_ = defaultDotTimer_;
		}
	}
}

void GameTitleScene::PlayerRotation()
{
	// ブースト時の回転処理
	isEaseFlag_ = true;
	isRot_ = true;
	
	if (isRot_) {
		if (isEaseFlag_) {
			eFrame_++;
		}

		// 自機の回転
		pRot_.z = EasingManager::EaseOutQuintP(eFrame_, eStartPos_, eEndPos_, eTotalTime_);

		// 回転最大時の値の初期化
		if (pRot_.z == playerMaxRotation_) {
			pRot_.z = playerMinRotation_;
			isRot_ = false;
		}
	}

	player_->SetPosition(pPos_);
	player_->SetRotation(pRot_);
}

void GameTitleScene::SceneChange()
{
	if (isFadeIn_ == false && isFadeOut_ == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			start_ = true;
		}
	}
	if (start_) {
		GameTitleScene::PlayerRotation();
		startTimer_++;
		pPos_.z += sPlayerPosMoveZ_;
		pPos_.y += sPlayerPosMoveY_;
		eye_.y -= sPlayerEyeMove_;

		player_->SetPosition(pPos_);
		player_->SetEye(eye_);

		isFadeIn_ = true;
	}
	if (startTimer_ >= MaxStartTimer_) {
		// タイマーを初期値に戻す
		startTimer_ = defaultStartTimer_;

		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}
