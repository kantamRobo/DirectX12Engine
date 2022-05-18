#include "TestTerrein.hlsli"

[domain("quad")]
PSInput mainDS(HSParameters In, float2 loc : SV_DomainLocation, const OutputPatch<DSInput, 4> patch)
{
    PSInput result;
    float4x4 mtxWVP = mul(sceneConstants.world, sceneConstants.viewProj);

    float3 p0 = lerp(patch[0].Position, patch[1].Position, loc.x).xyz;
    float3 p1 = lerp(patch[2].Position, patch[3].Position, loc.x).xyz;
    float3 pos = lerp(p0, p1, loc.y);

    float2 c0 = lerp(patch[0].UV, patch[1].UV, loc.x);
    float2 c1 = lerp(patch[2].UV, patch[3].UV, loc.x);
    float2 uv = lerp(c0, c1, loc.y);

    float height = texHeightMap.SampleLevel(mapSampler, uv, 0).x;
    pos.y = height * 30;

    float3 n = normalize((texNormalMap.SampleLevel(mapSampler, uv, 0).xyz - 0.5));
    result.Normal = n;
    result.UV = uv;
    result.Position = mul(float4(pos.xyz, 1), mtxWVP);

    return result;
}
