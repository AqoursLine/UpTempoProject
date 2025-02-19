/******************************************************
* Note.h	?m?[?g
* ?????F?J???}?^?g?E
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Note : public ThrowObject {
public:
	Note() = delete;
	Note(float x, float y, float r);
	~Note();

	void Update() override;

private:

};
