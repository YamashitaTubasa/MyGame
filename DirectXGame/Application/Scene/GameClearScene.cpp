#include "GameClearScene.h"

using namespace DirectX;

GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
	delete sprite_;
	delete clear_;
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
	clear_->LoadTexture(spriteCommon_, 1, L"Resources/Image/clear.png");
	clear_->SpriteCreate(1280, 720, 1, spriteCommon_, XMFLOAT2(0.0f, 0.0f), false, false);
	clear_->SetColor(XMFLOAT4(1, 1, 1, 1));
	clear_->SetPosition(clearPos_);
	clear_->SetScale(clearScale_);
	clear_->SetRotation(0.0f);
	clear_->SpriteTransferVertexBuffer(clear_, spriteCommon_, 1);
	clear_->SpriteUpdate(clear_, spriteCommon_);

}

void GameClearScene::Update()
{
	// 十字キーの右を押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームプレイシーン（次シーン）を生成
		GameBaseScene* scene = new GameTitleScene();
		GameSceneManager::GetInstance()->SetNextScene(scene);
	}
}

void GameClearScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

	// スプライト描画前処理
	Sprite::PreDraw(cmdList, spriteCommon_);

	// タイトルの描画
	clear_->SpriteDraw(spriteCommon_);

	// スプライト描画後処理
	Sprite::PostDraw();
}

void GameClearScene::Finalize()
{
}
