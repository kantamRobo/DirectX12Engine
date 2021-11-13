/*!
 * @brief ディズニーベースの物理ベースシェーダ
 */
/*
 ///////////////////////////////////////////////////
 // 定数
 ///////////////////////////////////////////////////
static const int NUM_DIRECTIONAL_LIGHT = 4; // ディレクションライトの本数
static const float PI = 3.1415926f;         // π

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// ディレクションライト
struct DirectionalLight
{
    float3 direction;   // ライトの方向
    float4 color;       // ライトの色
};

// ライト用の定数バッファー
cbuffer LightCb : register(b1)
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 eyePos;          // カメラの視点
    float specPow;          // スペキュラの絞り
    float3 ambientLight;    // 環境光
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;  // モデルの頂点座標
    float3 normal : NORMAL; // 法線
    float3 tangent  : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0;  // UV座標
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL;         // 法線
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0;          // uv座標
    float3 worldPos : TEXCOORD1;    // ワールド空間でのピクセルの座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

// 各種マップにアクセスするための変数を追加
Texture2D<float4> g_albedo : register(t0);           // アルベドマップ
Texture2D<float4> g_normalMap : register(t1);        // 法線マップ
Texture2D<float4> g_metallicSmoothMap : register(t2); // メタリックスムースマップ。rにメタリック、aにスムース

// サンプラーステート
sampler g_sampler : register(s0);


float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_nornal.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
    
    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y
        + normal * binSpaceNormal.z;
    
    return newNormal;
}

//ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 =m*m:
    float D = 1.0f / (4.0f * m2 + t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

//フレネルを計算 Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    return f0 + (1 - f0) * pow(1 - u, 5);
}

//Cook-Torranceモデルの鏡面反射

float CookTorranceSpecular(float3 L, float3 V, float3 N, float metalic)
{
    float microfacet = 0.76f;

    //金属度を垂直入射の時のフレネル反射率として扱う
    //金属度が高いほどフレネル反社は大きくなる
    float t0 = metalic;

    //ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float H = normalize(L + V);

    //各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    //D項をSchlicj近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

    //G項を求める
    float m = PI * NdotV * NdotH;

    //ここまで求めた値を利用して、Cool-Torranceモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}

//フレネル反射を考慮した拡散反射を計算

//フレネル反射は、光が物体の方面で反射する減少のこと

//一方拡散反射は光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光の事である。
//つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときには、拡散反射が小さくなる

//N =法線
//L 光源に向かうベクトル 光の方向と逆向きのベクトル
//V 視線に向かうベクトル 
//粗さ。0~1の範囲

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    //step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
    //光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    //粗さは0.5で固定
    float roughness = 0.5f;

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    //光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
    float dotLH = saturate(dot(L, H));

    //光源に向かうベクトルとハーフベクトル
    //光と並行に入射したときの拡散反射量を求めている
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * dotLH * roughness;

    //法線と視点に向かうベクトルを利用して拡散反射率を求める
    float fotNV = saturate(dot(N, V));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    //法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を乗算して最終的な反射率を求めている。PI
    //で除算しているのは正規化を行うため。
    return (FL * FV * energyFactor);
   
}

float4 PSMain(VSOutput psIn) :SV_Target0
{
    float3 normal = GetNormal(psIn.normal,psIn.tangent,psIn.biNormal,psIn.uv);

    //アルベドカラー、スペキュラカラー,金属度、滑らかさをサンプリングする
    //アルベドカラー(拡散反射光)
    float4 specColor = albedoColor;
    //金属度
    float metalic = g_metalicSmoothMap.Sample(g_sampler, psIn.uv).r;

    //滑らかさ
    float metalic = g_metalicSmoothMap.Sample(g_sampler, psIn.uv).a;

    //自然に向かって伸びるベクトルを計算する
    float3 toEye = normalize(eyePos - psIn.worldPos);

    float lig = 0;
    for (int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++)
    {
        //シンプルなディズニーベースの拡散反射を実装する
        //フレネル反射を考慮した拡散反射を計算
        float fiffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight[ligNo].direction, toEye);

        //正規化Lambert拡散反射を求める
        float NdotL = saturate(dot(normal, -directionalLight[ligNo].direction));
        float3 lambertDiffuse = directionalLight[ligNo].color * NdotL / PI;

        //最終的な拡散反射光を計算する。
        float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

        //Cook-Torranceモデルを利用した鏡面反射率を計算する
        //Cook-Torranceモデルの鏡面反射率を計算する
        float3 spec = CookTorranceSpecular(
            -directionLight[ligNo].direction, toEye, normal, smooth)
            * directionalLight[ligNo].color;

        //金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
        //スペキュラカラーの強さを鏡面反射率として扱う
        spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metalic);

    }

    //環境光による底上げ
    lig += ambientLight * albedoColor;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    return finalColor;
}

*/