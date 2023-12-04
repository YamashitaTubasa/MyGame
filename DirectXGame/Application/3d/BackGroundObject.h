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

private:
	// モデル
	Model* buildingM_ = nullptr;
	Model* asphaltM_ = nullptr;
	Model* bridgeM_ = nullptr;
	Model* treeM_ = nullptr;
	Model* wheelM_ = nullptr;
	// モデルデータコンテナ
	std::map<std::string, Model*> models_;
	// オブジェクト
	std::vector<Object3d*> objects_;
	// レベルデータ
	LevelData* levelData_ = nullptr;

	Vector3 modelPos_ = { 0,0,0 };
	DirectX::XMFLOAT3 modelEye_ = { 0,0,-3 };
	DirectX::XMFLOAT3 modelTarget_ = { 0,0,0 };

	const XMFLOAT3 eye_ = { 0.0f,0.0f,-3.0f };
};

