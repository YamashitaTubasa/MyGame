#include "Enemy.h"

#include "CollisionManager.h"
#include "SphereCollider.h"

#include <fstream>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	// 3Dオブジェクト解放
	delete enemyO3;
	// 3Dモデル解放
	delete enemyM;
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
	enemyM = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	enemyO3 = Object3d::Create();
	enemyO1 = Object3d::Create();
	enemyO2 = Object3d::Create();
	enemyO4 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	enemyO3->SetModel(enemyM);
	enemyO1->SetModel(enemyM);
	enemyO2->SetModel(enemyM);
	enemyO4->SetModel(enemyM);
	enemyO3->SetCollider(new SphereCollider);
	// 3Dオブジェクトの位置を指定
	position[0] = { 0,0,0 };
	rotation[0] = { 0,180,0 };
	enemyO3->SetPosition(position[0]);
	enemyO3->SetScale({ 5, 5, 5 });
	enemyO3->SetRotation(rotation[0]);

	enemyO1->SetPosition({ 10,1,10 });
	enemyO1->SetScale({ 5, 5, 5 });
	enemyO1->SetRotation(rotation[0]);
	enemyO2->SetPosition({ -8,-1,20 });
	enemyO2->SetScale({ 5, 5, 5 });
	enemyO2->SetRotation(rotation[0]);
	enemyO4->SetPosition({ 0,0,30 });
	enemyO4->SetScale({ 5, 5, 5 });
	enemyO4->SetRotation(rotation[0]);

	// 敵発生データの読み込み
	LoadEnemyPopData();

	return true;
}

void Enemy::Update()
{
	// 敵の更新
	enemyO3->Update();
	enemyO1->Update();
	enemyO2->Update();
	enemyO4->Update();
	
	// 敵を発生
	UpdateEnemyPopCommands();
}

void Enemy::Draw()
{
	// 敵の描画
	enemyO3->Draw();
	enemyO1->Draw();
	enemyO2->Draw();
	enemyO4->Draw();
}

void Enemy::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void Enemy::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
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
			isWait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}
