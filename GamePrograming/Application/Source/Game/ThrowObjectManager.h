/******************************************************
* ThrowObjectManager.h	投げるオブジェクト管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class ThrowObjectManager {
public:
	ThrowObjectManager();
	~ThrowObjectManager();

	void Update();
	void Draw();

	void PushLotteryObject(ThrowObject* pPushObjects); // リストの最後尾にモノを追加する

private:
	std::list<ThrowObject*> m_throwObjects;     // フェーズに登場しているモノ
	std::vector<ThrowObject*> m_lotteryObjects; // フェーズに登場するモノの種類の数（抽選用）

	float m_currentFrame;
	
};
