#pragma once

#include "Game/ThrowObject.h"

class StageObject : public ThrowObject {
public:
	StageObject() = delete;
	StageObject(float x, float y, float r) : ThrowObject(x, y, r) {}
	virtual ~StageObject() = default;

	void Update() override;
	void OnCollisionEnter(GameObject* collision) override;

	virtual void AddDamage();

protected:
	int m_hp = 1;		//固定が解除されるまで何度耐えるか
	float m_density = 1.0f;	//重いモノの調整用

private:
	bool m_fixed = true;//固定されている状態かどうか
	bool first = true;	//最初の１回だけ

};

