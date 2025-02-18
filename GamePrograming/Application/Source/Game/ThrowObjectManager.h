/******************************************************
* ThrowObjectManager.h	投げるオブジェクト管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

static constexpr int SPAWN_OBJECT_MAX = 5;

enum THROWOBJECT_ID {

	// 恒常オブジェクト
	KOKESHI = 0,
	BEAR,
	WOODENBOX,

	// ゲーム
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

	// 教室
	NOTE,
	PC,
	HOUKI,
	PLATFORM,
	TEACHER,

	// 海
	SHELL,
	BARREL,
	CORAL,
	ANCHOR,
	WHALE,

	//遊園地
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
	std::vector<THROWOBJECT_ID> m_lotteryObjects; // フェーズに登場するモノの種類の数（抽選用）

	float m_currentFrame = 0.0f;

	bool m_standby = false;

	XMFLOAT2 m_spwnPos[SPAWN_OBJECT_MAX];
	int m_spawnNum = 0;
	int m_spawnTime = 120;//この方式だと40以下の値に出来ない
};
