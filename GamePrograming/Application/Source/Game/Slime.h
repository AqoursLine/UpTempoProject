/******************************************************
* Slime.h	スライム
* 制作者：ユミタリオ
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Slime : public ThrowObject {
public:
	Slime() = delete;
	Slime(float x, float y, float r);
	~Slime();
	void HitPlayer(Player* p)override;

private:

};
