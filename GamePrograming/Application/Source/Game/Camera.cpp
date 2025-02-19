/******************************************************
* Camera.h		?J????
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/21
* ?ŏI?X?V???F2024/11/21
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Camera.h"
#include "Game/easing.h"

/****************************************************
* ?X?^?e?B?b?N?ϐ???????
*****************************************************/
bool Camera::m_isShake = false;
XMFLOAT2 Camera::m_offset = XMFLOAT2(0.0f, 0.0f);
XMFLOAT2 Camera::m_velocity = XMFLOAT2(0.0f, 0.0f);
int Camera::m_totalCount = 0;

/****************************************************
* ?J??????????
*****************************************************/
Camera::Camera() {
	m_scale = XMFLOAT2(1.0f, 1.0f);
	m_rot = 0.0f;
	m_pos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_time = 0;

	//?v???W?F?N?V?????}?g???N?X?ݒ?
	XMMATRIX projection;
	float right = SCREEN_WIDTH / m_scale.x;
	float buttom = SCREEN_HEIGHT / m_scale.y;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, right, buttom, 0.0f, 0.0f, 1.0f);
	D3D.SetProjectionMatrix(projection);

	//?r???[?}?g???N?X?ݒ?
	XMMATRIX view;
	view = XMMatrixTranslation(-m_pos.x, -m_pos.y, 0.0f) * XMMatrixRotationZ(-m_rot);
	D3D.SetViewMatrix(view);

	//?I?t?Z?b?g??????
	m_offset = XMFLOAT2(0.0f, 0.0f);
	m_isShake = false;
	m_frameCount = 0;
	m_velocity = XMFLOAT2(0.0f, 0.0f);
}

/****************************************************
* ?J?????I??
*****************************************************/
Camera::~Camera() {

}

/****************************************************
* ?J?????X?V
*****************************************************/
void Camera::Update() {
	if (m_isShake) {
		m_frameCount++;

		float time;

		time = m_frameCount / 60.0f;
		if (time > 1.0f) {
			time = 1.0f;
		}

		if (m_velocity.x != 0.0f) {
			m_velocity.x = Easing::OutElastic(time, 100.0f, 0.1f);
		}

		if (m_velocity.y != 0.0f) {
			m_velocity.y = Easing::OutElastic(time, 100.0f, 0.1f);
		}
		
		

		m_offset.x += m_velocity.x * m_reverseOffset;
		m_offset.y += m_velocity.y * m_reverseOffset;

		m_reverseOffset *= -1;

		//m_velocity.x *= -1;
		//m_velocity.y *= -1;

		if (m_frameCount >= 30) {
			m_isShake = false;
			m_offset = XMFLOAT2(0.0f, 0.0f);
			m_frameCount = 0;
		}
	}

}

/****************************************************
* ?J?????`??
*****************************************************/
void Camera::Draw() {
	//?v???W?F?N?V?????}?g???N?X?ݒ?
	XMMATRIX projection;
	float width = SCREEN_WIDTH / m_scale.x;
	float height = SCREEN_HEIGHT / m_scale.y;
	width *= 0.5;
	height *= 0.5f;
	projection = XMMatrixOrthographicOffCenterLH(-width, width, height, -height, 0.0f, 1.0f);

	D3D.SetProjectionMatrix(projection);

	//?r???[?}?g???N?X?ݒ?
	XMMATRIX view;
	view = XMMatrixTranslation(-m_pos.x - m_offset.x, -m_pos.y - m_offset.y, 0.0f) * XMMatrixRotationZ(-m_rot);
//	view = XMMatrixIdentity();
	D3D.SetViewMatrix(view);

}

/****************************************************
* ?J?????h?炷
*****************************************************/
void Camera::Shake(const XMFLOAT2& velocity, const int& totalCount) {
	m_offset = XMFLOAT2(0.0f, 0.0f);
	m_velocity = velocity;
	m_totalCount = totalCount;

	m_isShake = true;
}
