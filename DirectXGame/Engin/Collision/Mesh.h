#pragma once

#include <vector>

class Mesh
{
public:
	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<VectorPosNormalUv>& GetVertices() { return vertices; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices; }
};

