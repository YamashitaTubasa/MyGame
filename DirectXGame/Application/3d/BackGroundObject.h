#pragma once

#include "Object3d.h"
#include "Model.h"
#include "LevelLoader.h"

#include <DirectXMath.h>
#include <map>

/// <summary>
/// 背景モデル
/// </summary>
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
	Model* buildingM = nullptr;
	Model* asphaltM = nullptr;
	Model* bridgeM = nullptr;
	Model* treeM = nullptr;
	Model* wheelM = nullptr;
	// モデルデータコンテナ
	std::map<std::string, Model*> models;
	// オブジェクト
	std::vector<Object3d*> objects;
	// レベルデータ
	LevelData* levelData = nullptr;

	XMFLOAT3 modelPos = { 0,0,0 };
	XMFLOAT3 modelEye = { 0,0,-3 };
	XMFLOAT3 modelTarget = { 0,0,0 };
};

