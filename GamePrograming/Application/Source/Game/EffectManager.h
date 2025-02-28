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
//動くエフェクトについて
// 動くが秒数が経ったら消える場合はcreateMoveEffectでtimeまで指定するboolとintは指定しない
// pos,rotはcreateMoveEffectを呼ぶ所で元の変数を更新することで変更する
// 
//動き時間で消えないエフェクトについて
// 時間で消えないエフェクトにはcreateMoveEffectの引数を最後まで指定する timeは何も影響しないはずなので適当でいい
// bool*でエフェクトの寿命を管理するのでbool*の引数に渡した実体がfalseになるとエフェクトは消える
// switchframeが10の場合10フレームで画像が切り替わるはず
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
	SpawnEffect,	// オブジェクトの発生
	ThingsSpawn,	// モノが出現する瞬間に生成されるエフェクト
	BuffEffect,		// バフエフェクト
	DebuffEffect,	// デバフエフェクト
	WallDesEffect,	//壁にプレイヤーがヒットした際
	BlowEffect,
	SmashEffect1P,
	SmashEffect2P,
	SmashEffect3P,
	SmashEffect4P,
	SmashEffectCPU,
	
	EffectMax		//これ絶対最後
};

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void Update();
	void Draw();
	static void CreateEffect(EffectType type, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time = 0,XMFLOAT4 col = { 1.0f,1.0f,1.0f,1.0f });
	static void CreateMoveEffect(EffectType type, XMFLOAT2* pos, XMFLOAT2 size, float* rot, float time = 0, XMFLOAT4 col = {1.0f,1.0f,1.0f,1.0f}
		,bool* loopflag = nullptr,int switchframe = 1);


private:
	static std::list<Effect*> m_Effects;
	static Texture m_textures[EffectMax];

	//たまにエラーを吐くstaticメンバ変数にした時点から発生

};



