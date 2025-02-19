/******************************************************
* R_Scaffold.h	???X?|?[???p?̑???
* ?????Fmurayama
* ?쐬???F2024/02/12
* ?ŏI?X?V???F
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class R_Scaffold : public ThrowObject {
public:
	R_Scaffold() = delete;
	R_Scaffold(float x, float y, float r,int pnum);
	~R_Scaffold();
	void Update() override;
	void HoldTiming() override;

	void OnCollisionEnter(GameObject* collision)override;
	void OnCollisionExit(GameObject* collision) override;
private:

	bool m_isCollision;//?Փ˂??Ă?
	int m_cnt;
	bool m_fixed = true;
	int m_resPNum;
	bool first = true;
};
