/******************************************************
* GameObject.h		?v???C???[
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/06
* ?ŏI?X?V???F2024/11/06
*******************************************************/
#pragma once

/****************************************************
* ?Q?[???I?u?W?F?N?g?N???X
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

	void SetType(const std::string& type) { m_type = type; }
	bool CompareType(const std::string& type) { return m_type.compare(type) == 0; }

	void SetIsDelete() { m_isDelete = true; }
	bool GetIsDelete() { return m_isDelete; }
private:
	std::string m_tag;
	std::string m_type;
	bool m_isDelete = false;
};
