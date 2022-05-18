#include "TestTerrein.hlsli"

float CalcTessFactor(float4 v)
{
    float dist = length(mul(v, sceneConstants.world).xyz - sceneConstants.cameraPos.xyz);
    float tessNear = sceneConstants.tessRange.x;
    float tessFar = sceneConstants.tessRange.y;

    const float MaxTessFactor = 32.0;
    float val = MaxTessFactor - (MaxTessFactor - 1) * (dist - tessNear) / (tessFar - tessNear);
    val = max(1, min(MaxTessFactor, val));
    return val;
}
float CalcNormalBias(float3 p, float3 n)
{
  //const float normalThreshold = 0.34; // 約70度.
  //const float normalThreshold = 0.5; // 約60度.
    const float normalThreshold = 0.85; // 約60度.
    float3 camPos = sceneConstants.cameraPos.xyz;
    float3 fromCamera = normalize(p - camPos);
    float cos2 = dot(n, fromCamera);
    cos2 *= cos2;
    float normalFactor = 1.0 - cos2;
    float bias = max(normalFactor - normalThreshold, 0) / (1.0 - normalThreshold);
    return bias * clamp(sceneConstants.tessRange.z, 0, 64);
}

HSParameters ComputePatchConstants(InputPatch<HSInput, 4> patch)
{
  // 分割用パラメータを決定する.
    HSParameters outParams;

  { // 各エッジの中点を求め、その点とカメラの距離で係数を計算する.
    // この中点で法線を取得し、その向きでさらに係数を補正する
        float4 v[4];
        float3 n[4];
        int indices[][2] =
        {
            { 2, 0 },
            { 0, 1 },
            { 1, 3 },
            { 2, 3 }
        };

        for (int i = 0; i < 4; ++i)
        {
            int idx0 = indices[i][0];
            int idx1 = indices[i][1];
            v[i] = 0.5 * (patch[idx0].Position + patch[idx1].Position);
            float2 uv = 0.5 * (patch[idx0].UV + patch[idx1].UV);
            n[i] = texNormalMap.SampleLevel(mapSampler, uv, 0).xyz;
            n[i] = normalize(n[i] - 0.5);
        }

        outParams.tessFactor[0] = CalcTessFactor(v[0]);
        outParams.tessFactor[2] = CalcTessFactor(v[2]);
        outParams.tessFactor[0] += CalcNormalBias(v[0].xyz, n[0].xyz);
        outParams.tessFactor[2] += CalcNormalBias(v[2].xyz, n[2].xyz);
        outParams.insideFactor[0] = 0.5 * (outParams.tessFactor[0] + outParams.tessFactor[2]);

        outParams.tessFactor[1] = CalcTessFactor(v[1]);
        outParams.tessFactor[3] = CalcTessFactor(v[3]);
        outParams.tessFactor[1] += CalcNormalBias(v[1].xyz, n[1].xyz);
        outParams.tessFactor[3] += CalcNormalBias(v[3].xyz, n[3].xyz);
        outParams.insideFactor[1] = 0.5 * (outParams.tessFactor[1] + outParams.tessFactor[3]);
    }
  
    return outParams;
}
[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ComputePatchConstants")]
DSInput mainHS(InputPatch<HSInput, 4> inPatch, uint pointID : SV_OutputControlPointID)
{
    DSInput result;
    result.Position = inPatch[pointID].Position;
    result.UV = inPatch[pointID].UV;
    return result;
}