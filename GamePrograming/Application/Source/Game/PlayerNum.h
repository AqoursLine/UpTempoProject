#pragma once

class Player;

class PlayerNum {
public:
	PlayerNum();
	~PlayerNum();

	void Update();
	void Draw();

	void SetTexture(Player* pPlayer, bool isCPU, int pNum);
private:
	Texture m_tex;

	Player* m_player = nullptr;

	float m_alpha = 1.0f;

	int m_frameCnt = 0;

	bool m_existsThis = true;
};
