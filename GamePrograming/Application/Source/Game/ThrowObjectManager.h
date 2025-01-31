/******************************************************
* ThrowObjectManager.h	投げるオブジェクト管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

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
	HORSE,

};

class ThrowObjectManager {
public:
	ThrowObjectManager();
	~ThrowObjectManager();

	void Update();
	void Draw();

	void PushLotteryObject(const THROWOBJECT_ID& ObjectID);

private:
	std::list<ThrowObject*> m_throwObjects;
	std::vector<THROWOBJECT_ID> m_lotteryObjects; // フェーズに登場するモノの種類の数（抽選用）

	float m_currentFrame = 0.0f;

};
