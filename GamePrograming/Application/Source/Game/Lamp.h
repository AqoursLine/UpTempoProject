/******************************************************
* Lamp.h	蛍光灯
* 制作者：murayama
* 作成日：2024/01/23
* 最終更新日：
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Lamp : public ThrowObject {
public:
	Lamp() = delete;
	Lamp(float x, float y, float r,bool left);
	~Lamp();
	void Update() override;
	void OnCollisionEnter(GameObject* collision) override;

private:

	bool m_fixed = true;//固定されている状態かどうか
	int m_hp;			//固定が解除されるまで何度耐えるか
	bool first = true;	//最初の１回だけ

};



