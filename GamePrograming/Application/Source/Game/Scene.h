/******************************************************
* Scene.h		シーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/01
* 最終更新日：2024/11/01
*******************************************************/
#pragma once
#include "Game/Camera.h"
#include "Game/Movie.h"

/****************************************************
* シーンクラス
*****************************************************/
class Scene {
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	const bool GetIsFinished() const { return m_isFinished; }
	const bool GetISEnd() const { return m_isEnd; }

protected:
	Camera* m_camera = nullptr;
	bool m_isFinished = false;
	bool m_isEnd = false;
};


