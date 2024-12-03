#pragma once
#include "framework.h"
#include "DirectX/DirectX.h"

class Effect
{
	Texture& m_tex;//テクスチャ


	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;

	float m_drawTime;//描画する時間(f)//intでよさそう
	int m_frameCount;//カウント用

	int m_imagePattern;//画像の総パターン数
	int m_uvX;//画像横パターン数
	int m_uvY;//画像縦パターン数

	bool isUse;//使用フラグ

public:
	//でふぉこんどうする？t
	Effect(Texture& tex, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time, int uvx, int uvy, int pattern = 0);

	~Effect();

	void Update(void);
	void Draw(void);
	bool GetUse() { return isUse; };
};

