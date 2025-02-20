#pragma once

class ResultCharacter {
public:
	ResultCharacter();
	~ResultCharacter();

	void Update();
	void Draw();

	const bool GetIsFinished() const { return m_isFinished; }
	void SetIsFinished() { m_isFinished = true; }

private:
	std::vector<Texture> m_characterTex;
	std::vector<Texture> m_presentBody;
	std::vector<Texture> m_presentHead;

	int m_totalPlayer = 0;

	bool m_isFinished = false;

	int m_frameCount = 0;


	//プレゼントポジション
	XMFLOAT2 m_bluePresentPosition;
	XMFLOAT2 m_redPresentPosition;
	XMFLOAT2 m_yellowPresentPosition;
	XMFLOAT2 m_purplePresentPosition;

	//キャラクターポジション
	XMFLOAT2 m_bluepos;
	XMFLOAT2 m_purplepos;
	XMFLOAT2 m_redpos;
	XMFLOAT2 m_yellowpos;

	//キャラクターターゲットポジション
	XMFLOAT2 m_blueTargetPos;
	XMFLOAT2 m_redTargetPos;
	XMFLOAT2 m_yellowTargetPos;
	XMFLOAT2 m_purpleTargetPos;
	//キャラクタースタートポジション
	XMFLOAT2 m_blueStartPos;
	XMFLOAT2 m_redStartPos;
	XMFLOAT2 m_yellowStartPos;
	XMFLOAT2 m_purpleStartPos;

	float m_blueTime = 0.0f;
	float m_purpleTime = 0.0f;
	float m_redTime = 0.0f;
	float m_yellowTime = 0.0f;


};



