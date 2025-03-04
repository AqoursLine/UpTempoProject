#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/StageObject.h"
#include "Game/Camera.h"

void StageObject::Update() {
	if (m_hp <= 0 && first) {
		m_fixed = false;
		m_body->SetType(b2_dynamicBody);
		b2Fixture* fixture = (m_body)->GetFixtureList();
		fixture->SetDensity(m_density	);//重すぎて飛ばないから0.
		fixture->SetFriction(0.3f);
		fixture->SetRestitution(0.0f);
		m_body->ResetMassData();
		SetTag("ThrowObject");
		first = false;

	}
	if (m_fixed) {
	} else {
		ThrowObject::Update();
	}
}

void StageObject::OnCollisionEnter(GameObject* collision) {
	if (m_fixed) {
		if (collision->CompareTag("ThrowObject") && ((ThrowObject*)collision)->GetIsThrow()) {
			AddDamage();
			((ThrowObject*)collision)->SetIsThrow(false);
			((ThrowObject*)collision)->SetIsDeleteStandBy(true);
		}
	} else {
		ThrowObject::OnCollisionEnter(collision);
	}
}

void StageObject::AddDamage() {
	m_hp--;
	Camera::Shake(XMFLOAT2(10.0f, -10.0f), 30);
}

