/**
 * @file Matrix4.cpp
 * @brief Matrix4クラス
 * @author Yamashita Tubasa
 */

#include "Matrix4.h"
#include "Vector3.h"
#include <cmath> // sin cos

Matrix4::Matrix4()
{
	float result[4][4]
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}
}

Matrix4::Matrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03,
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13,
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23,
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
}

// 単位行列を求める
Matrix4 Matrix4::Identity()
{
	static const Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// 逆行列を求める
Matrix4 Matrix4::MakeInverse()
{
	Matrix4 temp;
	float mat[4][8] = { 0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = m[i][j];
		}
	}

	mat[0][4] = 1;
	mat[1][5] = 1;
	mat[2][6] = 1;
	mat[3][7] = 1;

	for (int n = 0; n < 4; n++) {
		//最大の絶対値を探索する(とりあえず対象成分を最大と仮定しておく)
		float max = abs(mat[n][n]);
		int maxIndex = n;

		for (int i = n + 1; i < 4; i++) {
			if (abs(mat[i][n]) > max) {
				max = abs(mat[i][n]);
				maxIndex = i;
			}
		}

		//最大の絶対値が0だったら逆行列は求められない
		if (abs(mat[maxIndex][n]) <= 0.000001f) {
			return temp; //とりあえず単位行列返しちゃう
		}

		//入れ替え
		if (n != maxIndex) {
			for (int i = 0; i < 8; i++) {
				float f = mat[maxIndex][i];
				mat[maxIndex][i] = mat[n][i];
				mat[n][i] = f;
			}
		}

		//掛けたら1になる値を算出
		float mul = 1 / mat[n][n];

		//掛ける
		for (int i = 0; i < 8; i++) {
			mat[n][i] *= mul;
		}

		//他全部0にする
		for (int i = 0; i < 4; i++) {
			if (n == i) {
				continue;
			}

			float mul1 = -mat[i][n];

			for (int j = 0; j < 8; j++) {
				mat[i][j] += mat[n][j] * mul1;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			temp.m[i][j] = mat[i][j + 4];
		}
	}

	*this = temp;

	return *this;
}


// 拡大縮小行列を求める
Matrix4 Matrix4::Scale(const Vector3& s) 
{
	Matrix4 result
	{
		s.x , 0.0f, 0.0f, 0.0f,
		0.0f, s.y , 0.0f, 0.0f,
		0.0f, 0.0f, s.z , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

Matrix4 Matrix4::Scale(const Vector4& s)
{
	Matrix4 result
	{
		s.x , 0.0f, 0.0f, 0.0f,
		0.0f, s.y , 0.0f, 0.0f,
		0.0f, 0.0f, s.z , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// x軸まわりの回転行列を求める
Matrix4 Matrix4::RotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos , sin , 0.0f,
		0.0f, -sin, cos , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

// y軸まわりの回転行列を求める
Matrix4 Matrix4::RotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos , 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin , 0.0f, cos , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// z軸まわりの回転を行列を求める
Matrix4 Matrix4::RotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos , sin , 0.0f, 0.0f,
		-sin, cos , 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

// 平行移動行列の設定
Matrix4 Matrix4::Translate(const Vector3& t)
{
	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x , t.y , t.z , 1.0f,
	};

	return result;
}

Matrix4 Matrix4::Translate(const Vector4& t)
{
	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x , t.y , t.z , 1.0f,
	};

	return result;
}

// 座標変換（ベクトルと行列の掛け算）を行うTransform関数を作成する。（透視変換にも対応している）
Vector3 Matrix4::Transform(const Vector3& v, const Matrix4& m1)
{
	float w = v.x * m1.m[0][3] + v.y * m1.m[1][3] + v.z * m1.m[2][3] + m1.m[3][3];

	Vector3 result
	{
		(v.x * m1.m[0][0] + v.y * m1.m[1][0] + v.z * m1.m[2][0] + m1.m[3][0]) / w,
		(v.x * m1.m[0][1] + v.y * m1.m[1][1] + v.z * m1.m[2][1] + m1.m[3][1]) / w,
		(v.x * m1.m[0][2] + v.y * m1.m[1][2] + v.z * m1.m[2][2] + m1.m[3][2]) / w
	};

	return result;
}

// ビュー行列の作成
Matrix4 Matrix4::ViewMat(Vector3 eye, Vector3 target, Vector3 up)
{
	Vector3 zaxis = target - eye;
	zaxis.Normalize();
	Vector3 xaxis = up.Cross(zaxis);
	xaxis.Normalize();
	Vector3 yaxis = zaxis.Cross(xaxis);
	yaxis.Normalize();

	Matrix4 LookAt = {
		xaxis.x,	xaxis.y,	xaxis.z,	0,
		yaxis.x,	yaxis.y,	yaxis.z,	0,
		zaxis.x,	zaxis.y,	zaxis.z,	0,
		eye.x,		eye.y,		eye.z,		1
	};

	LookAt.MakeInverse();

	*this = LookAt;

	return *this;
}

// 射影行列の作成
Matrix4 Matrix4::ProjectionMat(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	float h = 1 / tanf(fovAngleY / 2);
	float w = h / aspectRatio;
	float a = farZ / (farZ - nearZ);
	float b = (-nearZ * farZ) / (farZ - nearZ);

	Matrix4 perspectiveFovLH = {
		w,		 0,		 0,		 0,
		0,		 h,		 0,		 0,
		0,		 0,		 a,		 1,
		0,		 0,		 b,		 0
	};

	*this = perspectiveFovLH;

	return *this;
}


Matrix4 Matrix4::operator*(const Matrix4& m1)
{
	return *this *= m1;
}

// 代入演算子オーバーロード
Matrix4& Matrix4::operator*=(const Matrix4& m1)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float Total = 0.0f;
			for (int k = 0; k < 4; k++) {
				Total += m[i][k] * m1.m[k][j];
			}
			result.m[i][j] = Total;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result.m[i][j];
		}
	}

	return *this;
}

// 2項演算子* のオーバーロード関数(行列と行列の積)
const Matrix4 operator *(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
}

// 2項演算子* のオーバーロード関数(ベクトルと行列の積)
const Vector3 operator *(const Vector3& v, const Matrix4& m)
{
	float W = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / W,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / W,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / W
	};
	return result;
}

float ConvertToRadians(float rotation)
{
	float radians = static_cast<float>(rotation * (PI / 180.0f));

	return radians;
}