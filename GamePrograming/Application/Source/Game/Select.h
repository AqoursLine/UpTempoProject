/******************************************************
* Select.h		選択画面基底クラス
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/1/16
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

	bool m_isFinished;

private:

};

