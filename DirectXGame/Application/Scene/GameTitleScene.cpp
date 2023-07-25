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

void GameTitleScene::Initialize(SpriteCommon& spriteCommon)
{
	// DirectXCommonのインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();

	// スプライト
	sprite_ = new Sprite();
	spriteCommon_ = sprite_->SpriteCommonCreate(1280, 720);
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// タイトル
	title_ = new Sprite();
	title_->LoadTexture(spriteCommon_, 1, L"Resources/Image/title.png");
	title_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	title_->SetColor(XMFLOAT4(1, 1, 1, 1));
	title_->SetPosition(titlePos_);
	title_->SetScale(titleScale_);
	title_->SetRotation(0.0f);
	title_->SpriteTransferVertexBuffer(title_, spriteCommon, 1);
	title_->SpriteUpdate(title_, spriteCommon_);

}

void GameTitleScene::Update()
{
	// 十字キーの右を押したら
	if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		// ゲームプレイシーン（次シーン）を生成
		GameBaseScene* scene = new GamePlayScene();
		GameSceneManager::GetInstance()->SetNextScene(scene);
	}
}

void GameTitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	// タイトルの描画
	title_->SpriteDraw(cmdList, spriteCommon_);
	
	// スプライト描画後処理
	Sprite::PostDraw();

}

void GameTitleScene::Finalize()
{
}
