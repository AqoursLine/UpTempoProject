#pragma once
#include "Game/FieldObject.h"

class Corner : public FieldObject {
public:
	Corner(const XMFLOAT2& pos, float rot, const XMFLOAT2& ceilingSize, const XMFLOAT2& wallSize, int uvNum, const FIELD_DIRECTION fieldDirection);
	~Corner();


private:

};


