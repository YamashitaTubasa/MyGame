/**
 * @file BackGroundObject.h
 * @brief 背景モデルを設置するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"
#include "LevelLoader.h"

#include <DirectXMath.h>
#include <map>

class BackGroundObject
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	BackGroundObject();
	~BackGroundObject();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// サークルの座標取得
	const DirectX::XMFLOAT3& GetWheelPosition() const { return wheelPosition_; }

private:
	// モデルデータコンテナ
	std::map<std::string, std::unique_ptr<Model>> models_;
	// オブジェクト
	std::vector<std::unique_ptr<Object3d>> objects_;
	// レベルデータ
	LevelData* levelData_ = nullptr;

	XMFLOAT3 modelPos_ = { 0,0,0 };
	XMFLOAT3 modelEye_ = { 0,0,-3 };
	XMFLOAT3 modelTarget_ = { 0,0,0 };

	const XMFLOAT3 eye_ = { 0.0f,0.0f,-3.0f };

	XMFLOAT3 wheelPosition_ = { 0,0,0 };
};

