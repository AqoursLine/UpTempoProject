/******************************************************
* ScaffoldBase.h	看板
* 制作者：センバソラ
* 作成日：2024/12/05
* 最終更新日：2025/2/13
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class ScaffoldBase : public ThrowObject {
public:
	ScaffoldBase() = delete;
	ScaffoldBase(float x, float y, float r,int pattern = 1);
	~ScaffoldBase();

	void Update() override;
	void Draw() override {};
	void OnCollisionEnter(GameObject* collision) override {};
	b2Body* GetBody() const { return m_body; }
	float m_moveDir = 1;
	int m_pattern = 1;//良くない
private:

};
