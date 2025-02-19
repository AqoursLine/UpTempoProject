/******************************************************
* Coin.h	コイン
* 制作者：ユミタリオ
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Coin : public ThrowObject {
public:
	Coin() = delete;
	Coin(float x, float y, float r);
	~Coin();

private:

};

