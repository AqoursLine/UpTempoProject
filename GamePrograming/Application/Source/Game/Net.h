#pragma once

#include "Game/GameObject.h"

class Net :public GameObject {

private:
	XMFLOAT2 m_pos;   // ��
	XMFLOAT2 m_sight; // �Ə�
	XMFLOAT2 m_size;  // �Ԃ̃T�C�Y

	b2Body* m_body = nullptr;

	float m_rot;
	Texture m_tex;


	bool m_IsThrow;   // �l�b�g���������Ă��邩�ǂ���
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