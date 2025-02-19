//
//?w?b?_?[?ɐ??????
//

#include "Game/EffectManager.h"

std::list<Effect*> EffectManager::m_Effects;
Texture EffectManager::m_textures[EffectMax];

//?R???X?g???N?^
EffectManager::EffectManager()
{

	//???ꂢ?邩?H
	for (auto effect : m_Effects) {
		delete effect;
	}

	m_Effects.clear();

	bool test;//load?e?X?g?p

	//?e?N?X?`???ǂݍ???
	m_textures[TestEffect0].Load(L"Data/Texture/circle_exp.png");
	m_textures[TestEffect1].Load(L"Data/Texture/testEffect.png");
	m_textures[ObjectHitOther].Load(L"Data/Texture/ObjectEffect.png");
	m_textures[Jump].Load(L"Data/Texture/Jump.png");
	m_textures[AirJump].Load(L"Data/Texture/AirJump.png");
	m_textures[PlayerHitWall].Load(L"Data/Texture/WallEffect.png");
	m_textures[SpawnEffect].Load(L"Data/Texture/SpawnEffect.png");
}

//?f?X?g???N?^
EffectManager::~EffectManager()
{
	for (auto effect : m_Effects) {
		delete effect;
	}

	m_Effects.clear();
}



void EffectManager::Update()
{
	// ???X?g?̗v?f????ԂɊm?F
	for (auto it = m_Effects.begin(); it != m_Effects.end(); )
	{
		Effect* effect = *it;

		if (effect->GetUse()) {
			// Use()??true?̏ꍇ?̏???
			effect->Update();
			++it; // ???̗v?f??
		}
		else {
			// Use()??false?̏ꍇ?A?v?f??폜
			delete effect; // ?????????
			it = m_Effects.erase(it); // ?C?e???[?^??X?V
		}
	}
}

void EffectManager::Draw()
{
	for (auto effect : m_Effects) {
		effect->Draw();
	}
}

//?G?t?F?N?g?̐???
void EffectManager::CreateEffect(EffectType type, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time)
{
	switch (type)//?G?t?F?N?g?̎????ƂɕK?v	?`?掞?Ԃ͌Œ??????????
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5));//?Ō??}?W?b?N?i???o?[2?͉摜???Ƃɂ????Őݒ肷??
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6));
		break;

	case ObjectHitOther:
		m_Effects.push_back(new Effect(m_textures[ObjectHitOther], pos, size, rot, time, 10, 2));
		break;

	case Jump:
		m_Effects.push_back(new Effect(m_textures[Jump], pos, size, rot, time, 10, 6));
		break;

	case AirJump:
		m_Effects.push_back(new Effect(m_textures[AirJump], pos, size, rot, time, 10, 6));
		break; 

	case PlayerHitWall:
		m_Effects.push_back(new Effect(m_textures[PlayerHitWall], pos, size, rot, time, 10, 6)); // ?ݒ???
		break;

	case PlayerBlow:
		m_Effects.push_back(new Effect(m_textures[PlayerBlow], pos, size, rot, time, 10, 2));
		break;
	case SpawnEffect:
		m_Effects.push_back(new Effect(m_textures[SpawnEffect], pos, size, rot, time, 5, 6));
		break;

	default://?????????ǉ?
		break;
	}


}
//?????G?t?F?N?g?̐??? ?????Ƃ߂???
// CreateMoveEffect??
// (?`?掞?Ԃ??????Ă??Ȃ??ꍇ??reateEffect??Ăԑ???ool?^?̕ϐ???????A?h???X????????)
//?@???Ԏw?肷??????ot?܂œ??͂??Ă??̐???????͂??Ȃ?
void EffectManager::CreateMoveEffect(EffectType type, XMFLOAT2* pos, XMFLOAT2 size, float* rot, float time, bool* loopflag,int switchframe)
{
	switch (type)//?G?t?F?N?g?̎????ƂɕK?v	?`?掞?Ԃ͌Œ??????????
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5, loopflag, switchframe));//?}?W?b?N?i???o?[2?͉摜???Ƃɂ????Őݒ肷??
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6, loopflag, switchframe));
		break;

	case ObjectHitOther:
		m_Effects.push_back(new Effect(m_textures[ObjectHitOther], pos, size, rot, time, 10, 2, loopflag, switchframe));
		break;

	case Jump:
		m_Effects.push_back(new Effect(m_textures[Jump], pos, size, rot, time, 10, 6, loopflag, switchframe));
		break;

	case AirJump:
		m_Effects.push_back(new Effect(m_textures[AirJump], pos, size, rot, time, 10, 6, loopflag, switchframe));
		break;

	case PlayerHitWall:
		m_Effects.push_back(new Effect(m_textures[PlayerHitWall], pos, size, rot, time, 10, 6, loopflag, switchframe)); // ?ݒ???
		break;

	case PlayerBlow:
		m_Effects.push_back(new Effect(m_textures[PlayerBlow], pos, size, rot, time, 10, 2, loopflag, switchframe));
		break;

	default://?????????ǉ?
		break;
	}


}
