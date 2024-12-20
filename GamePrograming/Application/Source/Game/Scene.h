/******************************************************
* Scene.h		シーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/01
* 最終更新日：2024/11/01
*******************************************************/
#pragma once
#include "Game/Camera.h"

/****************************************************
* シーンクラス
*****************************************************/
class Scene {
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetIsFinished() { m_isFinished = true; }
	bool GetIsFinished() const { return m_isFinished; }
protected:
	Camera* m_camera = nullptr;
private:
	bool m_isFinished = false;
};


