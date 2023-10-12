#pragma once

#include "Vector3.h"
#include "ImGuiManager.h"

#include <variant>
#include <map>
#include <string>

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables
{
public:
	// インスタンの取得
	static GlobalVariables* GetInstance();

private:
	// コンストラクタ
	GlobalVariables() = default;
	// デストラクタ
	~GlobalVariables() = default;
	// コピーコンストラクタの禁止
	GlobalVariables(const GlobalVariables&) = delete;
	// 代入演算子の禁止
	GlobalVariables& operator=(const GlobalVariables&) = delete;

public:
	// 項目
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};

public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

public:
	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

private:
	// 全データ
	std::map<std::string, Group> datas_;
};

