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

private:
	std::list<ThrowObject*> m_throwObjects;
	
};
