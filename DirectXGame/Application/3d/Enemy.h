#pragma once

#include "Object3d.h"
#include "Model.h"

/// <summary>
/// 敵クラス
/// </summary>
class Enemy : public Object3d
{
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

public: // メンバ関数
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns></returns>
	static Enemy* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // ゲッター
	DirectX::XMFLOAT3 GetPosition() const { return position[0]; }

private:
	// オブジェクト
	Object3d* enemyO3 = nullptr;
	// モデル
	Model* enemyM = nullptr;

	DirectX::XMFLOAT3 position[5]{};
	DirectX::XMFLOAT3 rotation[5]{};
	DirectX::XMFLOAT3 scale[5]{};
	DirectX::XMFLOAT3 eye[5]{};
	DirectX::XMFLOAT3 target[5]{};
};

