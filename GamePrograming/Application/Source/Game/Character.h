/******************************************************
* Character.h	キャラクター
* 制作者：ユミタリオ
* 作成日：2025/01/21
* 最終更新日：2025/01/23
*******************************************************/
#pragma once

#include "DirectX/DirectX.h"


enum ANIM_STATE {

	/*****************
	* 通常ステート
	*****************/
	IDLE = 0,	// 待機状態
	MOVE,		// 移動
	JUMP,		// ジャンプ
	FALL,		// 落下
	LANDING,	// 着地
	
	/*****************
	* 割り込みステート
	*****************/
	HITSTOP,	// ヒットストップ
	BLOW,		// 吹っ飛び
	HAVETHINGS,	// モノを持つ
	THROW,		// モノを投げる
};


/******************************************
* それぞれのアニメーションを保存するテクスチャの構造体
******************************************/
struct ANIM_TEX{
	Texture idleTex;
	Texture moveTex;
	Texture jumpTex;
	Texture fallTex;
	Texture landingTex;

	Texture hitstopTex;
	Texture blowTex;
	Texture havethingsTex;
	Texture throwTex;
};


class Character {
public:
	Character();
	virtual ~Character();

	void Update();
	virtual void Draw(XMFLOAT2 Pos,XMFLOAT2 Size,float rotate);

	void SetInterruptFlag(bool flag);
	bool GetInterruptFlag();

	void SetAnimState(ANIM_STATE animState);
	ANIM_STATE GetAnimState();

	void SetStopAnim(bool isStop);

	virtual Texture ReplaceTex() = 0;
	virtual void ChangePetternUV(ANIM_STATE currentState) = 0; // 現在のステートのアニメーションに合わせてUVの枚数を変える

	void IsCharacterFacingLeft(bool isLeft);

protected:
	ANIM_STATE	m_currentState;		// 現在のステート
	Texture		m_currentTex;		// 現在セットされているテクスチャ

	XMFLOAT2	m_uv;
	float		m_uvNum;			// 何個目の画像を描画するか
	int			m_uvNumMax;			// 現在セットされているテクスチャの総枚数を保存する
	int			m_uvNumX;			// 画像横パターン数
	int			m_uvNumY;			// 画像縦パターン数
	float		m_animSpeed;		// どれくらいのスピードでアニメーションを回すか

	XMFLOAT2	m_texSize;			// 一区画の画像の幅を保存する
	bool		m_isLeft;			// 左方向を向いているか

private:
	bool		m_interruptFlag;	// 割り込みフラグ
	ANIM_STATE	m_oldState;			// 前のステート
	bool		m_stopAnim;			// アニメーションをストップさせる
};

