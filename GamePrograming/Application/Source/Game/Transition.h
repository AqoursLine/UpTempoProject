#pragma once

#include "DirectX/DirectX.h"


//******************************
// 《使い方》
//******************************
//
// まずTextureを用意して、画像をロード。
// 以下の情報をコンストラクタに引数を渡してぶち込め！
// （テクスチャのディレクトリ、ポジション、サイズ、回転、横方向のアニメーション数、縦方向のアニメーション数、横方向のアニメーション数、総アニメーション数、コマ送りスピード）
//
// あとはUpdateとDrawを呼び出せばOKだ。
// アニメーションが終わったら、つまりメンバ変数であるm_animFinishedがtrueになったら次のシーンに行けばいい。
// if(IsAnimFinished()) 次のシーンへ
// ******************************************************************************************************************************

class Transition {

public:
	Transition() = delete;
	Transition(const std::wstring& filename, XMFLOAT2 pos, XMFLOAT2 size, float rot, int uvNumX, int uvNumY, int animFrameMax, float animSpeed, bool isLoop);
	~Transition() = default;

	void Update();
	void Draw() const;

	void SetTexture(const std::wstring& filename);
	bool IsAnimFinished() const { return m_animFinished; }

	void SetPos(XMFLOAT2 Pos) { m_pos = Pos; } // 後でpos変えたくなった時に。
	void SetSize(XMFLOAT2 Size) { m_size = Size; }

	void ResetUV();

private:
	Texture		m_tex;
	XMFLOAT2	m_pos;
	XMFLOAT2	m_size;
	float		m_rot;

	// 以下アニメーション関連
	float		m_uvNum;		// 現在のコマ数（何個目のコマを表示しているかってこと）floatにしているのは速さを細かく調整できるようにするため
	bool		m_animFinished; // アニメーションが終わったか！？
	XMFLOAT2	m_uv;			// 現在のUV
	int			m_animFrameMax;	// 現在セットされているテクスチャの総枚数を保存する
	int			m_uvNumX;		// 画像横パターン数
	int			m_uvNumY;		// 画像縦パターン数
	float		m_animSpeed;	// コマ送りの速さ
	XMFLOAT2	m_texSize;		// 一区画の画像の幅を保存する
	bool		m_isLoop;		// ループさせるか
};

