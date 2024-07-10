SamplerState smp : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> norm : register(t1);

// 頂点シェーダーの入力
struct PS_INPUT
{
    float4 Diffuse : COLOR0;         // ディフューズカラー
    float4 Specular : COLOR1;        // スペキュラカラー
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoords0 : TEXCOORD0;   // テクスチャ座標
    float4 svPosition : SV_POSITION; // 座標(プロジェクション空間)
    float4 Position : POSITION0;     // 座標(ワールド空間)
    float4 P : POSITION1;            // 座標(プロジェクション空間)
    float4 VPosition : POSITION2;    // 座標(ワールド空間 * ビュー空間)
};

// 頂点シェーダの出力
struct PS_OUTPUT
{
    float4 color : SV_Target0; // 色
    float4 depth : SV_Target1; // 深度値
    float4 normal : SV_Target2; // 法線情報
};


PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT outdata;
    
    outdata.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return outdata;
}