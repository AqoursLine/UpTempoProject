/******************************************************
* Board.h	?Ŕ?
* ?????F?Z???o?\??
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2025/2/13
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
