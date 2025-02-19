/******************************************************
* Kokeshi.h	コケシ
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Kokeshi : public ThrowObject {
public:
	Kokeshi() = delete;
	Kokeshi(float x, float y, float r);
	~Kokeshi();
private:

};


