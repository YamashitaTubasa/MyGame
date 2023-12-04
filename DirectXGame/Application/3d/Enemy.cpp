/**
 * @file Enemy.cpp
 * @brief 敵の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#include "Enemy.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Input.h"

#include <fstream>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete enemyO3_;
	delete enemyO1_;
	delete enemyO2_;
	delete enemyO4_;
	delete enemyM_;
}

Enemy* Enemy::Create(Model* model)
{
	// 3Dオブジェクトのインスタンス生成
	Enemy* instance = new Enemy();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルをセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Enemy::Initialize()
{
	if (!Object3d::Initialize()) 
	{
		return false;
	}

	// OBJからモデルデータを読み込む
	enemyM_ = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	enemyO3_ = Object3d::Create();
	enemyO1_ = Object3d::Create();
	enemyO2_ = Object3d::Create();
	enemyO4_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	enemyO3_->SetModel(enemyM_);
	enemyO1_->SetModel(enemyM_);
	enemyO2_->SetModel(enemyM_);
	enemyO4_->SetModel(enemyM_);
	enemyO3_->SetCollider(new SphereCollider);
	// 3Dオブジェクトの位置を指定
	position_[0] = { 0,0,0 };
	rotation_[0] = { 0,180,0 };
	enemyO3_->SetPosition(position_[0]);
	enemyO3_->SetScale({ 5, 5, 5 });
	enemyO3_->SetRotation(rotation_[0]);

	enemyO1_->SetPosition({ 10,1,10 });
	enemyO1_->SetScale({ 5, 5, 5 });
	enemyO1_->SetRotation(rotation_[0]);
	enemyO2_->SetPosition({ -8,-1,20 });
	enemyO2_->SetScale({ 5, 5, 5 });
	enemyO2_->SetRotation(rotation_[0]);
	enemyO4_->SetPosition({ 0,0,30 });
	enemyO4_->SetScale({ 5, 5, 5 });
	enemyO4_->SetRotation(rotation_[0]);

	// 敵発生データの読み込み
	LoadEnemyPopData();

	return true;
}

void Enemy::Update()
{
	// 敵の更新
	enemyO3_->Update();
	enemyO1_->Update();
	enemyO2_->Update();
	enemyO4_->Update();
	
	// 敵を発生
	UpdateEnemyPopCommands();
}

void Enemy::Draw()
{
	// 敵の描画
	if (!enemyDamage_) {
		enemyO3_->Draw();
	}
	if (!enemyDamage01_) {
		enemyO1_->Draw();
	}
	if (!enemyDamage02_) {
		enemyO2_->Draw();
	}
	if (!enemyDamage03_) {
		enemyO4_->Draw();
	}
}

void Enemy::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void Enemy::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWait_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands_, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			SetPosition({ x,y,z });
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}
