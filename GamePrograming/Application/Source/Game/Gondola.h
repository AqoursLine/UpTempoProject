/******************************************************
* Gondola.h	観覧車
* 制作者：イササトル
* 作成日：2025/01/17
* 最終更新日：2025/01/17
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Gondola : public ThrowObject {
public:
	Gondola() = delete;
	Gondola(float x, float y, float r, int angle, ThrowObject* wheel);
	~Gondola();

	void Draw() override;

	void HoldTiming() override;
private:
	XMFLOAT2 m_textureSize;

	b2Joint* m_revjoint = nullptr;
};


