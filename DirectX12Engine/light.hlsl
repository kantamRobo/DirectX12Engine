	
#include "SimpleDeg.hlsli"
cbuffer DiretionLightCB :register(b1)
{
	float3 dirDirection; //ライトの方向
	float3 dirColor;	//ライトのカラー
	float3 eyePos //視点の位置
		float3 ambientLight //アンビエントライト

		float3 groundcolor;
	float3 skyColor;
	float3 groundNormal;
}

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float4 normal);
float3 CalcLigFromDirectionLight(VSOutput in_PS);

Texture2D<float4> g_texture :register(t0);

sampler g_sampler :register(s0);
//ランバート拡散反射光を計算する
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する
	float t = dot(normal, lightDirection) * -1.0f;

	//内積の値を0以上の値にする

	t = max(0.0f, t);

	return lightColor * t;
}

//Phong鏡面反射光の計算する

float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	float3 regVec = reflect(lightDirection, normal);

	//光が当たったサーフェスから視点に伸びるベクトルを求める
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	// 鏡面反射の強さを求める
	float t = dot(refVec, toEye);

	// 鏡面反射の強さを0以上の数値にする
	t = max(0.0f, t);

	// 鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	// 鏡面反射光を求める
	return lightColor * t;
}

/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromDirectionLight(VSOutput psIn)
{
	// ディレクションライトによるLambert拡散反射光を計算する
	float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

	// ディレクションライトによるPhong鏡面反射光を計算する
	float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, psIn.worldPos, psIn.normal);
	return diffDirection + specDirection;
}