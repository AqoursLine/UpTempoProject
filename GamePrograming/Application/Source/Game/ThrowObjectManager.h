/******************************************************
* ThrowObjectManager.h	???????u?W?F?N?g?Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/12
* ?ŏI?X?V???F2024/11/12
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"
#include "Game/StageObjectManager.h"

static constexpr int SPAWN_OBJECT_MAX = 5;

enum THROWOBJECT_ID {

	// ?P???u?W?F?N?g
	KOKESHI = 0,
	BEAR,
	WOODENBOX,

	// ?Q?[??
	APPLE,
	COIN,
	SLIME,
	SWORD,
	SHIELD,
	MOON,
	SCAFFOLD,
	BOARD,
	CLOUD,
	GRASS,

	// ????
	NOTE,
	PC,
	HOUKI,
	PLATFORM,
	TEACHER,

	// ?C
	SHELL,
	BARREL,
	CORAL,
	ANCHOR,
	WHALE,

	//?V???n
	ABCBLOCK,
	BALLOON,
	CLOWN,
	COFFEECUP,
	FERRISWHEEL,
	MERRYGOROUNDBEAR,
	HORSEFRONT,
	HORSEBACK,

};

class ThrowObjectManager {
public:
	ThrowObjectManager();
	~ThrowObjectManager();

	void Update();
	void Draw();

	void PushLotteryObject(const THROWOBJECT_ID& ObjectID);
	static void PushRespawnScaffold(float x,float y,int pnum);
private:
	static std::list<ThrowObject*> m_throwObjects;
	std::vector<THROWOBJECT_ID> m_lotteryObjects; // ?t?F?[?Y?ɓo?ꂷ?郂?m?̎??̐??i???I?p?j

	float m_currentFrame = 0.0f;

	bool m_standby = false;

	XMFLOAT2 m_spwnPos[SPAWN_OBJECT_MAX];
	int m_spawnNum = 0;
	int m_spawnTime = 120;//???̕???????40?ȉ??̒l?ɏo???Ȃ?

	StageObjectManager m_stageObjectManager;
};
