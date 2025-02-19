/******************************************************
* Camera.h		?J????
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/21
* ?ŏI?X?V???F2024/11/21
*******************************************************/
#pragma once

class Camera {
public:
	Camera();
	~Camera();

	void Update();
	void Draw();

	static void Shake(const XMFLOAT2& velocity, const int& totalCount);
private:
	XMFLOAT2 m_pos;
	XMFLOAT2 m_scale;
	float m_rot;

	float m_time;

	//?h?炷?悤?ϐ?
	static bool m_isShake;
	static XMFLOAT2 m_offset;
	static XMFLOAT2 m_velocity;
	static int m_totalCount;
	int m_frameCount = 0;
	int m_reverseOffset = 1;
};
