/******************************************************
* Bear.h	熊ぬいぐるみ
* 制作者：ミヤタジョウジ
* 作成日：2024/11/15
* 最終更新日：2024/11/15
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Bear : public ThrowObject {
public:
	Bear() = delete;
	Bear(float x, float y, float r);
	~Bear();

private:

};
