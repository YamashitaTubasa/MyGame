#pragma once

#include "GameBaseScene.h"

/// <summary>
/// シーン管理
/// </summary>
class GameSceneManager final
{
private: // メンバ関数
	// コンストラクタ
	GameSceneManager() = default;
	// デストラクタ
	~GameSceneManager();

public: // メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Destroy();

public:
	// インスタンスの作成
	static GameSceneManager* GetInstance();

private:
	// コピーコンストラクタを無効にする
	GameSceneManager(const GameSceneManager&) = delete;
	// 代入演算子を無効にする
	GameSceneManager& operator=(const GameSceneManager&) = delete;

public: // メンバ関数
	// 次シーン予約
	void SetNextScene(GameBaseScene* nextScene) { nextScene_ = nextScene; }

private: // メンバ変数
	// 今のシーン(実行中シーン)
	GameBaseScene* scene_ = nullptr;
	// 次シーン
	GameBaseScene* nextScene_ = nullptr;
};

