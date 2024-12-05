/******************************************************
* Shield.h	シールド
* 制作者：ユミタリオ
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Shield : public ThrowObject {
public:
	Shield() = delete;
	Shield(float x, float y, float r);
	~Shield();

private:

};
