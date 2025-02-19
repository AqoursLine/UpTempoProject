/******************************************************
* FieldManager.h	?t?B?[???h?Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/12
* ?ŏI?X?V???F2024/11/12
*******************************************************/
#pragma once

#include "Game/FieldObject.h"

class FieldManager {
public:
	FieldManager();
	~FieldManager();

	void Update();
	void Draw();

	void ReCreateField(float sizeTop, float sizeLeft, float sizeRight, float sizeButtom);

private:
	std::list<FieldObject*> m_fieldObjects;
};
