#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float count_ = 10;
    float luminance = 1.0;
    float strength_ = 1.0;

    float4 col = tex.Sample(smp, input.uv) * color;

    if (isBlur == true)
    {
        float2 symmetryUv = input.uv - 0.5;
        float distance = length(symmetryUv);
        float factory = strength_ / count_ * distance;

        for(int i = 0; i < count_; i++){
            float uvOffset = 1 - factory * i;
            col.rgb += tex.Sample(smp, symmetryUv * uvOffset + 0.5);
        }

        col.rgb /= count_;
        col.a = 1;
    }
    
    return float4(col.rgb * luminance, alpha);
}