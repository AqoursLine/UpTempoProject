/******************************************************
* GameObject.h		プレイヤー
* 制作者：ミヤタジョウジ
* 作成日：2024/11/06
* 最終更新日：2024/11/06
*******************************************************/
#pragma once

/****************************************************
* ゲームオブジェクトクラス
*****************************************************/
class GameObject {
public:
	GameObject() {}
	virtual ~GameObject() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void OnCollisionEnter(GameObject* collision) {}
	virtual void OnCollisionExit(GameObject* collision) {}

	void SetTag(const std::string& tag) { m_tag = tag; }
	bool CompareTag(const std::string& tag) { return m_tag.compare(tag) == 0; }

private:
	std::string m_tag;
};
