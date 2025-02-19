/******************************************************
* Horse.h	うまー
* 制作者：murayama
* 作成日：2024/01/23
* 最終更新日：
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Horse : public ThrowObject {
public:
	Horse() = delete;
	Horse(float x, float y, float r,bool front);
	~Horse();
	void Update() override;
	void HoldTiming() override;

private:
	int m_turn = -1;
	int m_cnt;
	bool m_fixed = true;//動きを固定してるよん
	bool m_front;
	bool first=true;
	Texture m_tex2;
};

