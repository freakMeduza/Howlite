#include "ConstantBuffer.hlsli"

float4 main(float3 position : Position) : SV_Position
{
    float4 result = float4(position, 1.0f);
    result = mul(result, ModelMatrix);
    result = mul(result, ViewMatrix);
    result = mul(result, ProjectionMatrix);
    return result;
}