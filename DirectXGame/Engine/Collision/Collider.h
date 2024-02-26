/**
 * @file Collider.h
 * @brief 衝突判定オブジェクト
 * @author Yamashita Tubasa
 */

#pragma once

class Collider
{
public:
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision();

	// 半径を取得
	float GetRadius() const { return radius_; }
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }

private:
	// 衝突半径
	float radius_ = 1.0f;
};

