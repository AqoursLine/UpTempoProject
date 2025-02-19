/******************************************************
* Camera.h		カメラ
* 制作者：ミヤタジョウジ
* 作成日：2024/11/21
* 最終更新日：2024/11/21
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Camera.h"
#include "Game/easing.h"

/****************************************************
* スタティック変数初期化
*****************************************************/
bool Camera::m_isShake = false;
XMFLOAT2 Camera::m_offset = XMFLOAT2(0.0f, 0.0f);
XMFLOAT2 Camera::m_velocity = XMFLOAT2(0.0f, 0.0f);
int Camera::m_totalCount = 0;

/****************************************************
* カメラ初期化
*****************************************************/
Camera::Camera() {
	m_scale = XMFLOAT2(1.0f, 1.0f);
	m_rot = 0.0f;
	m_pos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_time = 0;

	//プロジェクションマトリクス設定
	XMMATRIX projection;
	float right = SCREEN_WIDTH / m_scale.x;
	float buttom = SCREEN_HEIGHT / m_scale.y;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, right, buttom, 0.0f, 0.0f, 1.0f);
	D3D.SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixTranslation(-m_pos.x, -m_pos.y, 0.0f) * XMMatrixRotationZ(-m_rot);
	D3D.SetViewMatrix(view);

	//オフセット初期化
	m_offset = XMFLOAT2(0.0f, 0.0f);
	m_isShake = false;
	m_frameCount = 0;
	m_velocity = XMFLOAT2(0.0f, 0.0f);
}

/****************************************************
* カメラ終了
*****************************************************/
Camera::~Camera() {

}

/****************************************************
* カメラ更新
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
* カメラ描画
*****************************************************/
void Camera::Draw() {
	//プロジェクションマトリクス設定
	XMMATRIX projection;
	float width = SCREEN_WIDTH / m_scale.x;
	float height = SCREEN_HEIGHT / m_scale.y;
	width *= 0.5;
	height *= 0.5f;
	projection = XMMatrixOrthographicOffCenterLH(-width, width, height, -height, 0.0f, 1.0f);

	D3D.SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixTranslation(-m_pos.x - m_offset.x, -m_pos.y - m_offset.y, 0.0f) * XMMatrixRotationZ(-m_rot);
//	view = XMMatrixIdentity();
	D3D.SetViewMatrix(view);

}

/****************************************************
* カメラ揺らす
*****************************************************/
void Camera::Shake(const XMFLOAT2& velocity, const int& totalCount) {
	m_offset = XMFLOAT2(0.0f, 0.0f);
	m_velocity = velocity;
	m_totalCount = totalCount;

	m_isShake = true;
}


