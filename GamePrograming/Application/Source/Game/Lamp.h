/******************************************************
* Lamp.h	?u????
* ?????Fmurayama
* ?쐬???F2024/01/23
* ?ŏI?X?V???F
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Lamp : public ThrowObject {
public:
	Lamp() = delete;
	Lamp(float x, float y, float r,bool left);
	~Lamp();
	void Update() override;
	void OnCollisionEnter(GameObject* collision) override;

private:

	bool m_fixed = true;//?Œ肳???????Ԃ??ǂ???
	int m_hp;			//?Œ肪????????܂ŉ??x?ς??邩
	bool first = true;	//?ŏ??̂P?񂾂?

};
