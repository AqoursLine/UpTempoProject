/******************************************************
* GameObject.h		�v���C���[
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/06
* �ŏI�X�V���F2024/11/06
*******************************************************/
#pragma once

/****************************************************
* �Q�[���I�u�W�F�N�g�N���X
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
