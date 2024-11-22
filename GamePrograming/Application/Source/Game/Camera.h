/******************************************************
* Camera.h		カメラ
* 制作者：ミヤタジョウジ
* 作成日：2024/11/21
* 最終更新日：2024/11/21
*******************************************************/
#pragma once

class Camera {
public:
	Camera();
	~Camera();

	void Update();
	void Draw();
private:
	XMFLOAT2 m_pos;
	XMFLOAT2 m_scale;
	float m_rot;

	float m_velocity;
	float m_time;
};

