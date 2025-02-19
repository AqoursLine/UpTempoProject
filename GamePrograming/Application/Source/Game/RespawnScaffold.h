/******************************************************
* R_Scaffold.h	リスポーン用の足場
* 制作者：murayama
* 作成日：2024/02/12
* 最終更新日：
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

	bool m_isCollision;//衝突してる？
	int m_cnt;
	bool m_fixed = true;
	int m_resPNum;
	bool first = true;

};
