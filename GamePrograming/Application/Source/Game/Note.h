/******************************************************
* Note.h	ノート
* 制作者：カワマタトウ
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Note : public ThrowObject {
public:
	Note() = delete;
	Note(float x, float y, float r);
	~Note();

private:

};
