/******************************************************
* Ferriswheel.h	観覧車
* 制作者：イササトル
* 作成日：2025/01/17
* 最終更新日：2025/01/17
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Ferriswheel : public ThrowObject {
public:
	Ferriswheel() = delete;
	Ferriswheel(float x, float y, float r);
	~Ferriswheel();
private:

};
