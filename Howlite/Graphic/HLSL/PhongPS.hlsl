#include "ConstantBuffer.hlsli"
#include "Texture.hlsli"

SamplerState TextureSampler;

// Light methods =======================================
float GetAttenuation(float DistanceToLight)
{
    return 1.0f / (LightKc + LightKl * DistanceToLight + LightKq * (DistanceToLight * DistanceToLight));
}

float3 GetDiffuse(const float3 Color, const float Intencity, const float Attenuation, const float3 N, const float3 L)
{
    return Color * Intencity * Attenuation * max(0.0f, dot(N, L));
}

float3 GetSpecular(const float3 Color, const float Intencity, const float Attenuation, const float3 R, const float3 V, const float SpecularIntencity, const float SpecularPower)
{
    return Color * Intencity * Attenuation * SpecularIntencity * pow(max(0.0f, dot(-R, V)), SpecularPower);
}
// =====================================================

struct ShaderInput
{
    float3 Fragment : FRAGMENT;
    float3 Normal : NORMAL;
    float2 Uv : UV;
};

float4 main(ShaderInput Input) : SV_TARGET
{
    if(NormalMapEnabled)
    {
        const float3 normalSample = NormalMap.Sample(TextureSampler, Input.Uv).xyz;
        Input.Normal.x = normalSample.x * 2.0f - 1.0f;
        Input.Normal.y = normalSample.y * 2.0f - 1.0f;
        Input.Normal.z = -normalSample.z;
    }

    const float3 L = normalize(LightPosition - Input.Fragment);
    const float3 N = normalize(Input.Normal);
    const float3 R = normalize(reflect(L, N));
    const float3 V = normalize(CameraPosition - Input.Fragment);

    const float attenuation = GetAttenuation(length(LightPosition - Input.Fragment));
    
    const float3 ambient = LightColor * 0.05f;
    const float3 diffuse = GetDiffuse(LightColor, LightIntencity, attenuation, N, L);
    const float3 specular = GetSpecular(LightColor, LightIntencity, attenuation, R, V, MaterialSpecularIntensity, MaterialSpecularPower);

    float3 material = MaterialColor; 
    
    if(DiffuseMapEnabled)
    {
        material = DiffuseMap.Sample(TextureSampler, Input.Uv).rgb;
    }

    return float4(saturate((diffuse + ambient) * material + specular), 1.0f);
}
