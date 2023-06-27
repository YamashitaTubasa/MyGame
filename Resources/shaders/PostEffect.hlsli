cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
	bool isBlur;
	float alpha;
};

// 頂点シェーダーからの出力構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};