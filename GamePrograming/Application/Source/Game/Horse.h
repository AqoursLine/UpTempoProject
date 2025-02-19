/******************************************************
* Horse.h	???܁[
* ?????Fmurayama
* ?쐬???F2024/01/23
* ?ŏI?X?V???F
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
	bool m_fixed = true;//??????Œ肵?Ă???
	bool m_front;
	bool first=true;
	Texture m_tex2;
};
