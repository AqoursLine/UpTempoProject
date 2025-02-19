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

	static void Shake(const XMFLOAT2& velocity, const int& totalCount);
private:
	XMFLOAT2 m_pos;
	XMFLOAT2 m_scale;
	float m_rot;

	float m_time;

	//揺らすよう変数
	static bool m_isShake;
	static XMFLOAT2 m_offset;
	static XMFLOAT2 m_velocity;
	static int m_totalCount;
	int m_frameCount = 0;
	int m_reverseOffset = 1;
};



