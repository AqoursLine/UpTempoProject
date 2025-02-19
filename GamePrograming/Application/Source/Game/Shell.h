/******************************************************
* Shell.h	貝
* 制作者：仙波空
* 作成日：2024/12/06
* 最終更新日：2024/12/06
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Shell : public ThrowObject {
public:
	Shell() = delete;
	Shell(float x, float y, float r);
	~Shell();

private:

};

