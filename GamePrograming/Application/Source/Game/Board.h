/******************************************************
* Board.h	看板
* 制作者：センバソラ
* 作成日：2024/12/05
* 最終更新日：2025/2/13
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Board : public ThrowObject {
public:
	Board() = delete;
	Board(float x, float y, float r);
	~Board();

private:

};


