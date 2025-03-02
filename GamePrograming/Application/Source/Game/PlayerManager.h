#pragma once
#include "Game/Player.h"


class PlayerManager
{
public:
	PlayerManager() = delete;
	PlayerManager(int phaze);
	~PlayerManager();


	void PhysicsUpdate();
	void Update();
	void Draw();

	void CharacterUpdate();

	void CreatePlayer(XMFLOAT2 pos, int pnum);
	void CreateCPU(XMFLOAT2 pos, int pnum);

	void PlayerSmashSE();

private:
	static std::list<Player*> m_players;

	int m_soundSmash; // 残機がなくなったら即デリートされるため、プレイヤーの方で最後の撃墜音がならない。それを解消するためのやつ。
};


