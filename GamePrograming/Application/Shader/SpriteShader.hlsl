//0番のテクスチャスロットを使用する
Texture2D g_texture : register(t0);
//0番のサンプラスロットを使用する
SamplerState g_sampler : register(s0);

//頂点シェーダーから出力するデータ
struct VSOutput
{
	float4 Pos : SV_Position;	//頂点の座標(射影座標系)
	float2 UV : TEXCOORD0;		//UV座標
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
