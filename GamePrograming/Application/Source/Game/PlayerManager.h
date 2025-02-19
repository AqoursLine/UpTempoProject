#pragma once
#include "Game/Player.h"


class PlayerManager
{
public:
	PlayerManager() = delete;
	PlayerManager(int phaze);
	~PlayerManager();

	void Update();
	void Draw();
	void CreatePlayer(XMFLOAT2 pos, int pnum);

private:
	static std::list<Player*> m_players;
};


