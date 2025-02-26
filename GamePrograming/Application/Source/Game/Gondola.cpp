/******************************************************
* Gondola.cpp	観覧車
* 制作者：イササトル
* 作成日：2025/01/17
* 最終更新日：2025/01/17
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Gondola.h"
#include "Game/Ferriswheel.h"

/****************************************************
* 観覧車初期化
*****************************************************/
Gondola::Gondola(float x, float y, float r, int angle, ThrowObject* wheel) : ThrowObject(x, y, r) {
	//テクスチャ設定
	m_uv.x = 0.0f;
	m_uv.y = 0.0f;
	m_texSize.x = 1.0f;
	m_texSize.y = 1.0f;


	//サイズ
	float aspect = m_texSize.x / m_texSize.y;
	float height = 135.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//描画用テクスチャサイズ
	m_textureSize = XMFLOAT2(180.0f, 180.0f);

	//軸取得
	FerrisWheel* pWheel = static_cast<FerrisWheel*>(wheel);
	//軸の角度取得
	float wheelRot = pWheel->GetRotate();
	//軸のポジション取得
	XMFLOAT2 wheelPos = pWheel->GetPos();

	//ポジション設定
	m_pos.x = cosf(wheelRot + XMConvertToRadians(angle * 90.0f)) * 230.0f + wheelPos.x;
	m_pos.y = sinf(wheelRot + XMConvertToRadians(angle * 90.0f)) * 230.0f + wheelPos.y;

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateCircle(&m_body, b2size.x * 0.5f);

	//回転ジョイント
	b2RevoluteJointDef jointdef = {};
	jointdef.bodyA = m_body;
	jointdef.bodyB = pWheel->GetBody();

	jointdef.localAnchorA.Set(0.0f, 0.0f);
	jointdef.localAnchorB = jointdef.bodyB->GetLocalPoint(jointdef.bodyA->GetPosition());

	m_revjoint = Physics::GetWorld()->CreateJoint(&jointdef);

	//タグセット
	SetTag("ThrowObject");

	//テクスチャ
	std::wstring filePath = L"Data/Texture/Gondola_";
	int num = rand() % 5;
	switch (num) {
		case 0:
			filePath += L"Blue";
			break;
		case 1:
			filePath += L"Cat";
			break;
		case 2:
			filePath += L"Star";
			break;
		case 3:
			filePath += L"White";
			break;
		case 4:
			filePath += L"Yellow";
			break;
	}

	filePath += L".png";

	m_tex.Load(filePath);

	// 与える力の設定
	m_ApplyImpact = { 2.0f,1.0f };

	m_weight = WEIGHT_NORMAL;
}

/****************************************************
* 観覧車終了
*****************************************************/
Gondola::~Gondola() {

}

/****************************************************
* ゴンドラ描画
*****************************************************/
void Gondola::Draw() {
	if (m_isPlayerCollision) {
		XMFLOAT2 size = XMFLOAT2(m_textureSize.x * m_sizeAdjust, m_textureSize.y * m_sizeAdjust);
		D3D.Draw2D(m_tex, m_pos, size, m_rot, m_uv, m_texSize, m_playerColor, PIXELMODE_SILHOUETTE);
	}

	D3D.Draw2D(m_tex, m_pos, m_textureSize, m_rot);
}

void Gondola::HoldTiming() {
	Physics::GetWorld()->DestroyJoint(m_revjoint);
}


