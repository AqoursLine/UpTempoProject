#pragma once

#include "Game/GameObject.h"

class Net :public GameObject {

private:
	XMFLOAT2 m_pos;   // 網
	XMFLOAT2 m_sight; // 照準
	XMFLOAT2 m_size;  // 網のサイズ

	b2Body* m_body = nullptr;

	float m_rot;
	Texture m_tex;


	bool m_IsThrow;   // ネットが投げられているかどうか
public:
	Net();
	~Net();

	void Update() override;
	void Draw() override;

	void ThrowNet();

	XMFLOAT2 GetNetPos() { return m_pos; }
	void SetNetPos(XMFLOAT2 Pos);
	bool IsThrow() { return m_IsThrow; }
	void SetThrow(bool NetCondition) { m_IsThrow = NetCondition; }

	void VelReset();
	void OnCollisionEnter(GameObject* collision) override;
	
};