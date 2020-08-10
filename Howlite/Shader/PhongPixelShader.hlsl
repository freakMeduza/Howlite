cbuffer TransformBuffer : register(b0)
{
    matrix ModelMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
    float3 CameraPosition;
};

cbuffer LightBuffer : register(b1)
{
    float3 LightPosition;
    float3 LightColor;
    float LightIntencity;
    float LightKc;
    float LightKl;
    float LightKq;
};

cbuffer ColorBuffer : register(b2)
{
    float3 MaterialColor;
    float MaterialSpecularPower;
    float MaterialSpecularIntensity;
};

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

float4 main(float3 position : Position, float3 normal : Normal) : SV_Target
{
    const float3 L = normalize(LightPosition - position);
    const float3 N = normalize(normal);
    const float3 R = normalize(reflect(L, N));
    const float3 V = normalize(CameraPosition - position);

    const float attenuation = GetAttenuation(length(LightPosition - position));
    
    const float3 ambient = LightColor * 0.05f;
    const float3 diffuse = GetDiffuse(LightColor, LightIntencity, attenuation, N, L);
    const float3 specular = GetSpecular(LightColor, LightIntencity, attenuation, R, V, MaterialSpecularIntensity, MaterialSpecularPower);
    
    return float4(saturate(diffuse + ambient + specular) * MaterialColor, 1.0f);
}
