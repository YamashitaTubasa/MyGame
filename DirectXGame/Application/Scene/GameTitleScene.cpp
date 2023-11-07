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
	delete sprite_;
	delete title_;
	delete skydome_;
	delete space_;
	delete object3d_;
	delete model_;
}

void GameTitleScene::Initialize()
{
	// DirectXCommonのインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();

	// 天球の初期化
	skydome_ = new Skydome();
	skydome_->Initialize();

	// スプライトの初期化
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate();

	// スプライトの読み込み
	LoadSprite();

	// OBJモデルの読み込み
	model_ = new Model();
	model_ = Model::LoadFromOBJ("fighter");
	// オブジェクトの生成
	object3d_ = new Object3d();
	object3d_ = Object3d::Create();
	// オブジェクトにモデルをセット
	object3d_->SetModel(model_);
	// オブジェクトの位置の設定
	object3d_->SetPosition(pPos_);
	object3d_->SetScale({5,5,5});
	object3d_->SetRotation(pRot_);
	object3d_->SetTarget(object3d_->GetPosition());
	object3d_->SetEye(eye_);
}

void GameTitleScene::Update()
{
	// 天球の更新
	skydome_->Update();
	// 天球の回転処理
	skydome_->RotateSky();

	// 自機の更新
	object3d_->Update();

	// スプライトの更新
	title_->SpriteUpdate(title_, spriteCommon_);
	space_->SpriteUpdate(space_, spriteCommon_);
	inBlack_->SpriteUpdate(inBlack_, spriteCommon_);
	nowLoading_->SpriteUpdate(nowLoading_, spriteCommon_);
	/*for (auto& i : dot_) {
		i->SpriteUpdate(i, spriteCommon_);
	}*/
	for (int i = 0; i < dotMax_; i++) {
		dot_[i]->SpriteUpdate(dot_[i], spriteCommon_);
	}
	
	// フェードイン・アウトの処理
	GameTitleScene::Fade();

	// Press SPACEの描画の点滅処理
	GameTitleScene::FlashSpace();

	// ロード中の点の点滅処理
	GameTitleScene::Load();

	// 自機の上下移動
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
	object3d_->SetPosition(pPos_);

	// シーンの切り替え処理
	if (isFadeIn_ == false && isFadeOut_ == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			start_ = true;
		}
	}
	if (start_) {
		PlayerRotation();
		startTimer_++;
		pPos_.z += sPlayerPosMoveZ_;
		pPos_.y += sPlayerPosMoveY_;
		eye_.y -= sPlayerEyeMove_;

		object3d_->SetPosition(pPos_);
		object3d_->SetEye(eye_);

		isFadeIn_ = true;
	}
	if (startTimer_ >= MaxStartTimer_) {
		// タイマーを初期値に戻す
		startTimer_ = defaultStartTimer_;

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
	skydome_->Draw();
	object3d_->Draw();

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
	//===== タイトル描画の初期化 =====//
	title_ = new Sprite();
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
	title_->SpriteTransferVertexBuffer(title_, spriteCommon_, titleNum);

	//===== SPACEの描画 =====//
	space_ = new Sprite();
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
	space_->SpriteTransferVertexBuffer(space_, spriteCommon_, spaceNum);

	//===== InBlackの描画 =====//
	inBlack_ = new Sprite();
	// テクスチャの読み込み
	inBlack_->LoadTexture(spriteCommon_, inBlackNum, L"Resources/Image/white.png");
	// スプライトの生成
	inBlack_->SpriteCreate(WinApp::window_width, WinApp::window_height, inBlackNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	inBlack_->SetColor({ 1,1,1,1 });
	inBlack_->SetPosition({ 0,0,0 });
	inBlack_->SetScale({ 1280,720 });
	inBlack_->SetRotation(0.0f);
	inBlack_->SetAlpha(bInAlpha_);
	// スプライトの頂点バッファの転送
	inBlack_->SpriteTransferVertexBuffer(inBlack_, spriteCommon_, inBlackNum);

	//===== ロードの描画 =====//
	nowLoading_ = new Sprite();
	// テクスチャの読み込み
	nowLoading_->LoadTexture(spriteCommon_, nowLoadingNum, L"Resources/Image/nowLoading.png");
	// スプライトの生成
	nowLoading_->SpriteCreate(WinApp::window_width, WinApp::window_height, nowLoadingNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	nowLoading_->SetColor({ 1,1,1,1 });
	nowLoading_->SetPosition(nowLodingPos_);
	nowLoading_->SetScale(nowLodingScale_);
	nowLoading_->SetRotation(0.0f);
	nowLoading_->SetAlpha(lodingAlpha_);
	// スプライトの頂点バッファの転送
	nowLoading_->SpriteTransferVertexBuffer(nowLoading_, spriteCommon_, nowLoadingNum);

	//===== 点の描画 =====//
	dot_[0] = new Sprite();
	// テクスチャの読み込み
	dot_[0]->LoadTexture(spriteCommon_, dotZeroNum, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[0]->SpriteCreate(WinApp::window_width, WinApp::window_height, dotZeroNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[0]->SetColor({ 1,1,1,1 });
	dot_[0]->SetPosition(dotPos_[0]);
	dot_[0]->SetScale(dotScale_);
	dot_[0]->SetRotation(0.0f);
	dot_[0]->SetAlpha(1.0f);
	// スプライトの頂点バッファの転送
	dot_[0]->SpriteTransferVertexBuffer(dot_[0], spriteCommon_, dotZeroNum);

	dot_[1] = new Sprite();
	// テクスチャの読み込み
	dot_[1]->LoadTexture(spriteCommon_, dotOneNum, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[1]->SpriteCreate(WinApp::window_width, WinApp::window_height, dotOneNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[1]->SetColor({ 1,1,1,1 });
	dot_[1]->SetPosition(dotPos_[1]);
	dot_[1]->SetScale(dotScale_);
	dot_[1]->SetRotation(0.0f);
	dot_[1]->SetAlpha(1.0f);
	// スプライトの頂点バッファの転送
	dot_[1]->SpriteTransferVertexBuffer(dot_[1], spriteCommon_, dotOneNum);

	dot_[2] = new Sprite();
	// テクスチャの読み込み
	dot_[2]->LoadTexture(spriteCommon_, dotTwoNum, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[2]->SpriteCreate(WinApp::window_width, WinApp::window_height, dotTwoNum, spriteCommon_, defaultAnchorpoint_, false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[2]->SetColor({ 1,1,1,1 });
	dot_[2]->SetPosition(dotPos_[2]);
	dot_[2]->SetScale(dotScale_);
	dot_[2]->SetRotation(0.0f);
	dot_[2]->SetAlpha(1.0f);
	// スプライトの頂点バッファの転送
	dot_[2]->SpriteTransferVertexBuffer(dot_[2], spriteCommon_, dotTwoNum);
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
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isEaseFlag_ = true;
		isRot_ = true;
		eFrame_ = defaultFrame_;
	}

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

	object3d_->SetPosition(pPos_);
	object3d_->SetRotation(pRot_);
}
