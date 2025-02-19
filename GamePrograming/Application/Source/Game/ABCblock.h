/******************************************************
* ABCblock.h	ABCblock
* 制作者：イササトル
* 作成日：2025/01/17
* 最終更新日：2025/01/17
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Abcblock : public ThrowObject {
public:
	Abcblock() = delete;
	Abcblock(float x, float y, float r);
	~Abcblock();
private:

};

