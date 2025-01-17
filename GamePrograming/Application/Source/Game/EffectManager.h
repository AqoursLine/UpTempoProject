// EffectManager.h
// 
// 
// -------------------------------
//使い方
// EffectManager.hをインクルードし、 EffectManagerの変数を作りInit() Update() Draw()を置いてから 
// CreateEffect()を呼べばエフェクトを生成出来る
// 引数には	(エフェクトのタイプ,描画する座標,大きさ,角度,描画する時間,) が必要。		時間のみ省略可能で、省略した場合は画像の総パターン数が入る
// 
// 
//エフェクトの追加について
// １：neum内のEffectMaxの上に	"エフェクトのタイプ,"	で追加
// 
// ２：m_textures[エフェクトのタイプ].Load("ファイルのパス");	を追加
// 
// ３：CreateEffect()のswitch文のdefultの上に
//	case エフェクトのタイプ:
//		m_Effects.push_back(new Effect(m_textures[エフェクトのタイプ], pos, size, rot, time, 画像の横パターン数, 縦パターン数));
//		break;
// 


#pragma once
#include "Game/Effect.h"
enum EffectType
{
	TestEffect0,
	TestEffect1,
	ObjectHitOther, // モノが他のモノに当たる時
	Jump,			// ジャンプ
	AirJump,		//　空中ジャンプ
	PlayerHitWall,	// プレイヤーが外枠にぶつかる
	PlayerBlow,		// プレイヤーが吹っ飛ぶ時の

	EffectMax		//これ絶対最後
};

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void Update();
	void Draw();
	static void CreateEffect(EffectType type, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time = 0);


private:
	static std::list<Effect*> m_Effects;
	static Texture m_textures[EffectMax];

	//たまにエラーを吐くstaticメンバ変数にした時点から発生

};

