#include "ConstantBuffer.hlsli"

struct Output
{
    float3 position    : Position;
    float3 normal      : Normal;
    float4 position_CS : SV_Position;
};

Output main(float3 position : Position, float3 normal : Normal)
{
    float4 position_WS = mul(float4(position, 1.0f), ModelMatrix);
    float4 position_VS = mul(position_WS, ViewMatrix);
    float4 position_CS = mul(position_VS, ProjectionMatrix);
    Output output;
    output.position_CS = position_CS;
    output.position = (float3)position_WS;
    output.normal = mul(normal, (float3x3)ModelMatrix);
    return output;
}