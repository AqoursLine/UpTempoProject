#pragma once
#include "Game/Player.h"


class PlayerManager
{
public:
	PlayerManager(int phaze);
	void Finalize();

	void Update();
	void Draw();
	void CreatePlayer(XMFLOAT2 pos, int pnum);

private:
	static std::list<Player*> m_players;
	int m_phazeNo;
};
