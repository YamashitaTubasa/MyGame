/**
 * @file Matrix4.h
 * @brief Matrix4クラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"
#include "Vector4.h"

#define PI 3.14159265359

class Matrix4
{
public:
	float m[4][4];

public:
	// コンストラクタ
	Matrix4();

	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	/// <summary>
	/// 形状タイプ取得
	/// </summary>
	/// <returns></returns>
	static Matrix4 Identity();

	/// <summary>
	/// 逆行列を求める
	/// </summary>
	/// <returns></returns>
	Matrix4 MakeInverse();

	/// <summary>
	/// 拡大縮小行列の設定
	/// </summary>
	/// <param name="s"></param>
	/// <returns></returns>
	Matrix4 Scale(const Vector3& s);
	Matrix4 Scale(const Vector4& s);

	/// <summary>
	/// 回転行列の設定
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	Matrix4 RotateX(float angle);
	Matrix4 RotateY(float angle);
	Matrix4 RotateZ(float angle);

	/// <summary>
	/// 平行移動行列の作成
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	Matrix4 Translate(const Vector3& t);
	Matrix4 Translate(const Vector4& t);

	/// <summary>
	/// 座標変換（ベクトルと行列の掛け算をする）
	/// </summary>
	/// <param name="v"></param>
	/// <param name="m"></param>
	/// <returns></returns>
	Vector3 Transform(const Vector3& v, const Matrix4& m);

	/// <summary>
	/// ビュー行列作成
	/// </summary>
	/// <param name="eye"></param>
	/// <param name="target"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	Matrix4 ViewMat(Vector3 eye, Vector3 target, Vector3 up);

	/// <summary>
	/// 射影行列
	/// </summary>
	/// <param name="fovAngleY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	Matrix4 ProjectionMat(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	/// <summary>
	/// 代入演算子オーバーロード
	/// </summary>
	/// <param name="m1"></param>
	/// <returns></returns>
	Matrix4& operator*=(const Matrix4& m1);

	Matrix4 operator*(const Matrix4& m1);
};

// 2項演算子オーバーロード
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);

// ラジアンに変換
float ConvertToRadians(float rotation);