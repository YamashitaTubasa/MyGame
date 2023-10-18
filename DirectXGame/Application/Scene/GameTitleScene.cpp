/**
 * @file GameTitleScene.cpp
 * @brief ゲームタイトルシーンのクラス
 * @author Yamashita Tubasa
 */

#include "GameTitleScene.h"

GameTitleScene::GameTitleScene()
{
}

GameTitleScene::~GameTitleScene()
{
	delete sprite_;
	delete title_;
	delete skydome;
	delete space_;
	delete object3d;
	delete model;
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
	spriteCommon_ = sprite_->SpriteCommonCreate();

	// スプライトの読み込み
	LoadSprite();

	// OBJモデルの読み込み
	model = new Model();
	model = Model::LoadFromOBJ("fighter");
	// オブジェクトの生成
	object3d = new Object3d();
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
	// 天球の回転処理
	skydome->RotateSky();

	// 自機の更新
	object3d->Update();

	// スプライトの更新
	title_->SpriteUpdate(title_, spriteCommon_);
	space_->SpriteUpdate(space_, spriteCommon_);
	inBlack_->SpriteUpdate(inBlack_, spriteCommon_);
	nowLoding_->SpriteUpdate(nowLoding_, spriteCommon_);
	/*for (auto& i : dot_) {
		i->SpriteUpdate(i, spriteCommon_);
	}*/
	for (int i = 0; i < 3; i++) {
		dot_[i]->SpriteUpdate(dot_[i], spriteCommon_);
	}
	
	// フェードイン・アウトの処理
	GameTitleScene::Fade();

	// Press SPACEの描画の点滅処理
	GameTitleScene::FlashSpace();

	// ロード中の点の点滅処理
	GameTitleScene::Load();

	// 自機の上下移動
	if (isPos == false) {
		pPos.y += 0.03f;
	}
	if (pPos.y >= -4.0f) {
		isPos = true;
	}
	if (isPos == true) {
		pPos.y -= 0.03f;
	}
	if (pPos.y <= -7.0f) {
		isPos = false;
	}
	object3d->SetPosition(pPos);
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		int a;
		a = 0;
	}

	// シーンの切り替え処理
	if (isFadeIn == false && isFadeOut == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			start = true;
		}
	}
	if (start) {
		startTimer++;
		pPos.z += 1.5f;
		pPos.y += 0.25f;
		eye.y -= 0.06f;

		object3d->SetPosition(pPos);
		object3d->SetEye(eye);

		isFadeIn = true;
	}
	if (startTimer >= 200) {
		// タイマーを初期値に戻す
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
		if (isSpace) {
			space_->SpriteDraw(spriteCommon_);
		}
	}

	// フェードインとフェードアウトの描画
	if (isFadeIn == true || isFadeOut == true) {
		inBlack_->SpriteDraw(spriteCommon_);
	}

	// NowLodingと点の描画
	if (isLoding_) {
		nowLoding_->SpriteDraw(spriteCommon_);
		/*for (auto& i : dot_) {
			i->SpriteDraw(spriteCommon_);
		}*/
		for (int i = 0; i < 3; i++) {
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
	space_->SetRotation(spaceRot_);
	space_->SetAlpha(sAlpha_);
	// スプライトの頂点バッファの転送
	space_->SpriteTransferVertexBuffer(space_, spriteCommon_, 2);

	//===== InBlackの描画 =====//
	inBlack_ = new Sprite();
	// テクスチャの読み込み
	inBlack_->LoadTexture(spriteCommon_, 3, L"Resources/Image/white.png");
	// スプライトの生成
	inBlack_->SpriteCreate(1280, 720, 3, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	inBlack_->SetColor({ 1,1,1,1 });
	inBlack_->SetPosition({ 0,0,0 });
	inBlack_->SetScale({ 1280,720 });
	inBlack_->SetRotation(0.0f);
	inBlack_->SetAlpha(bInAlpha_);
	// スプライトの頂点バッファの転送
	inBlack_->SpriteTransferVertexBuffer(inBlack_, spriteCommon_, 3);

	//===== ロードの描画 =====//
	nowLoding_ = new Sprite();
	// テクスチャの読み込み
	nowLoding_->LoadTexture(spriteCommon_, 4, L"Resources/Image/nowLoding.png");
	// スプライトの生成
	nowLoding_->SpriteCreate(1280, 720, 4, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	nowLoding_->SetColor({ 1,1,1,1 });
	nowLoding_->SetPosition(nowLodingPos_);
	nowLoding_->SetScale(nowLodingScale_);
	nowLoding_->SetRotation(0.0f);
	nowLoding_->SetAlpha(lodingAlpha_);
	// スプライトの頂点バッファの転送
	nowLoding_->SpriteTransferVertexBuffer(nowLoding_, spriteCommon_, 4);

	//===== 点の描画 =====//
	dot_[0] = new Sprite();
	// テクスチャの読み込み
	dot_[0]->LoadTexture(spriteCommon_, 5, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[0]->SpriteCreate(1280, 720, 5, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[0]->SetColor({ 1,1,1,1 });
	dot_[0]->SetPosition(dotPos_[0]);
	dot_[0]->SetScale(dotScale_);
	dot_[0]->SetRotation(0.0f);
	dot_[0]->SetAlpha(1.0f);
	// スプライトの頂点バッファの転送
	dot_[0]->SpriteTransferVertexBuffer(dot_[0], spriteCommon_, 5);

	dot_[1] = new Sprite();
	// テクスチャの読み込み
	dot_[1]->LoadTexture(spriteCommon_, 6, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[1]->SpriteCreate(1280, 720, 6, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[1]->SetColor({ 1,1,1,1 });
	dot_[1]->SetPosition(dotPos_[1]);
	dot_[1]->SetScale(dotScale_);
	dot_[1]->SetRotation(0.0f);
	dot_[1]->SetAlpha(1.0f);
	// スプライトの頂点バッファの転送
	dot_[1]->SpriteTransferVertexBuffer(dot_[1], spriteCommon_, 6);

	dot_[2] = new Sprite();
	// テクスチャの読み込み
	dot_[2]->LoadTexture(spriteCommon_, 7, L"Resources/Image/dot.png");
	// スプライトの生成
	dot_[2]->SpriteCreate(1280, 720, 7, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	// 色、座標、サイズ、回転角の設定
	dot_[2]->SetColor({ 1,1,1,1 });
	dot_[2]->SetPosition(dotPos_[2]);
	dot_[2]->SetScale(dotScale_);
	dot_[2]->SetRotation(0.0f);
	dot_[2]->SetAlpha(1.0f);
	// スプライトの頂点バッファの転送
	dot_[2]->SpriteTransferVertexBuffer(dot_[2], spriteCommon_, 7);
}

void GameTitleScene::Fade()
{
	// フェードアウト処理
	if (isFadeOut) {
		if (bInAlpha_ > 0.0f) {
			bInAlpha_ -= 0.01f;
		}
		if (bInAlpha_ <= 0.01f) {
			bInAlpha_ = 0.0f;
			isFadeOut = false;
		}
	}

	// フェードインの処理
	if (isFadeIn) {
		if (bInAlpha_ < 1.0f) {
			bInAlpha_ += 0.01f;
		}
	}
	inBlack_->SetAlpha(bInAlpha_);
}

void GameTitleScene::FlashSpace()
{
	if (isSpace) {
		spaceTimer++;
		if (spaceTimer >= 50 && spaceTimer < 100) {
			if (sAlpha_ > 0.0f) {
				sAlpha_ -= 0.05f;
			}
			if (sAlpha_ <= 0.0f) {
				sAlpha_ = 0.0f;
			}
		}
		if (spaceTimer >= 100 && spaceTimer < 150) {
			if (sAlpha_ < 1.0f) {
				sAlpha_ += 0.05f;
			}
			if (sAlpha_ >= 1.0f) {
				sAlpha_ = 1.0f;
			}
		}
		if (spaceTimer >= 150) {
			spaceTimer = 0;
		}
		space_->SetAlpha(sAlpha_);
	}
}

void GameTitleScene::Load()
{
	if (bInAlpha_ >= 1.0f) {
		isLoding_ = true;
	}
	if (isLoding_) {
		dotTimer_++;
		if (dotTimer_ >= 0 && dotTimer_ < 40) {
			isDot_[0] = true;
		}
		if (dotTimer_ >= 40 && dotTimer_ < 70) {
			isDot_[1] = true;
		}
		if (dotTimer_ >= 70 && dotTimer_ < 100) {
			isDot_[2] = true;
		}
		if (dotTimer_ >= 140) {
			for (int i = 0; i < 3; i++) {
				isDot_[i] = false;
			}
			dotTimer_ = 0;
		}
	}
}
