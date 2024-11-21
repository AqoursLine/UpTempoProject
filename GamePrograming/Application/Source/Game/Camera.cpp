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

/****************************************************
* カメラ初期化
*****************************************************/
Camera::Camera() {
	m_scale = XMFLOAT2(1.0f, 1.0f);
	m_rot = 0.0f;
	m_pos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_velocity = 0.1f;
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
	//m_scale.x += m_velocity;
	//m_scale.y += m_velocity;
	m_rot += m_velocity;
	m_time += GAMESYS.GetDletaTime();
	if (m_time >= 1) {
		m_velocity *= -1;
		m_time = 0.0f;
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
	view = XMMatrixTranslation(-m_pos.x, -m_pos.y, 0.0f) * XMMatrixRotationZ(-m_rot);
//	view = XMMatrixIdentity();
	D3D.SetViewMatrix(view);

}
