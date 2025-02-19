#pragma once
/******************************************************
* Houki.h	箒
* 制作者： イササトル
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Houki : public ThrowObject {
public:
	Houki() = delete;
	Houki(float x, float y, float r);
	~Houki();

	void Update() override;
	bool downFirst=true;
private:

};


