/******************************************************
* Scene.h		?V?[???Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/01
* ?ŏI?X?V???F2024/11/01
*******************************************************/
#pragma once
#include "Game/Camera.h"

/****************************************************
* ?V?[???N???X
*****************************************************/
class Scene {
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	const bool GetIsFinished() const { return m_isFinished; }
	const bool GetIsEnd() const { return m_isEnd; }

protected:
	Camera* m_camera = nullptr;
	bool m_isFinished = false;
	bool m_isEnd = false;
};
