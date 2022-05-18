#include "TestTerrein.hlsli"
HSInput mainVS(VSInput In)
{
    HSInput result = (HSInput) 0;
    float4x4 mtxWVP = mul(sceneConstants.world, sceneConstants.viewProj);

    result.Position = In.Position;
    result.UV = In.UV;
    return result;
}