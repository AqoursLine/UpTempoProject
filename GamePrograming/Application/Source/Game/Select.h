/******************************************************
* Select.h		?I??????N???X
* ?????F?~???^?W???E?W
* ?쐬???F2025/1/16
* ?ŏI?X?V???F2025/1/16
*******************************************************/
#pragma once

class Select {
public:
	Select() = default;
	virtual~Select() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

	const bool GetIsFinished() const { return m_isFinished; }

protected:
	Texture m_backGroundTex;
	Texture m_cursorTex[4];
	Texture m_buttonTex[4];
	Texture m_changebuttonTex[4];
	Texture m_animObjectTex;
	Texture m_alphaTex;

	bool m_isFinished = false;

private:

};
