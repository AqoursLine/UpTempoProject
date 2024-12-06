/******************************************************
* Barrel.h	樽
* 制作者：イササトル
* 作成日：2024/12/06
* 最終更新日：2024/12/06
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Barrel : public ThrowObject {
public:
	Barrel() = delete;
	Barrel(float x, float y, float r);
	~Barrel();
private:

};
