struct HSInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

struct SceneParameters
{
    float4 cameraPos;
    float4x4 world;
    float4x4 viewProj;
    float4 tessRange;
};

struct HSParameters
{
    float tessFactor[4] : SV_TessFactor;
    float insideFactor[2] : SV_InsideTessFactor;
};


struct DSInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
};

struct VSInput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};



ConstantBuffer<SceneParameters> sceneConstants : register(b0);
Texture2D texHeightMap : register(t0);
Texture2D texNormalMap : register(t1);
sampler mapSampler : register(s0);