//0番のテクスチャスロットを使用する
Texture2D g_texture : register(t0);
//0番のサンプラスロットを使用する
SamplerState g_sampler : register(s0);

//頂点シェーダーから出力するデータ
struct VSOutput
{
    float4 Pos : SV_Position; //頂点の座標(射影座標系)
    float2 UV : TEXCOORD0; //UV座標
    float4 Diffuse : COLOR0;
};

//マトリクスバッファ
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
}

//=====================================================
// 頂点シェーダー
//=====================================================
VSOutput VS(float4 pos : POSITION, float2 uv : TEXUV, float4 diffuse : COLOR)
{
    VSOutput Out;
    //頂点座標を出力
    Out.Pos = mul(pos, WorldViewProjection);
	//頂点のUV座標を、何も加工せずそのまま出力
    Out.UV = uv;
	//頂点のカラーを出力
    Out.Diffuse = diffuse;
    return Out;
}

//=====================================================
// ピクセルシェーダー
//=====================================================
float4 PS(VSOutput In) : SV_target0
{
	//テクスチャから色を取得
    float4 color = g_texture.Sample(g_sampler, In.UV);
	
	//テクスチャに色を乗算
    color *= In.Diffuse;
	
	//テクスチャの色を出力
    return color;
}

//=====================================================
// シルエットピクセルシェーダー
//=====================================================
float4 SilhouettePS(VSOutput In) : SV_target0
{
	//テクスチャから色を取得
    float4 color = g_texture.Sample(g_sampler, In.UV);

	//テクスチャのα値からマスクを作成
    float mask = step(0.1f, color.a);
	
	//マスク如何で使用する色を指定
    color = lerp(float4(0.0f, 0.0f, 0.0f, 0.0f), In.Diffuse, mask);
	
	//テクスチャの色を出力
    return color;
}

//=====================================================
// 動画用ピクセルシェーダー
//=====================================================

float3 YUVToRGB(float3 yuv)
{
  // BT.601 coefs
    static const float3 yuvCoef_r = { 1.164f, 0.000f, 1.596f };
    static const float3 yuvCoef_g = { 1.164f, -0.392f, -0.813f };
    static const float3 yuvCoef_b = { 1.164f, 2.017f, 0.000f };
    yuv -= float3(0.0625f, 0.5f, 0.5f);
    return saturate(float3(
    dot(yuv, yuvCoef_r),
    dot(yuv, yuvCoef_g),
    dot(yuv, yuvCoef_b)
    ));
}

float4 MoviePS(VSOutput input) : SV_Target
{
    float y = g_texture.Sample(g_sampler, float2(input.UV.x, input.UV.y * 0.5)).r;
    float u = g_texture.Sample(g_sampler, float2(input.UV.x * 0.5, 0.50 + input.UV.y * 0.25)).r;
    float v = g_texture.Sample(g_sampler, float2(input.UV.x * 0.5, 0.75 + input.UV.y * 0.25)).r;
    return float4(YUVToRGB(float3(y, u, v)), 1.f);
}