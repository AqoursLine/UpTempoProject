/******************************************************
* CoffeCup.h	コーヒーカップ
* 制作者：ナカガワリオン
* 作成日：2024/12/29
* 最終更新日：2024/12/29
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class CoffeeCup : public ThrowObject {
public:
	CoffeeCup() = delete;
	CoffeeCup(float x, float y, float r);
	~CoffeeCup();

private:

};
