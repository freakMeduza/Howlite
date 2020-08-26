#include "ConstantBuffer.hlsli"

float4 main() : SV_Target
{
    return float4(MaterialColor, 1.0f);
}