Texture2D diffuseTexture;
SamplerState samplerState;

float4 main(float2 uv : UV) : SV_Target
{
    return diffuseTexture.Sample(samplerState, uv);
}