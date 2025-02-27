#pragma once

#include "Game/ThrowObject.h"

class FerrisWheel : public ThrowObject {
public:
	FerrisWheel() = delete;
	FerrisWheel(float x, float y, float r);
	~FerrisWheel();

	void Update() override {};
	void Draw() override;

	b2Body* GetBody() const { return m_body; }
private:
	Texture m_poleTexture;
};

