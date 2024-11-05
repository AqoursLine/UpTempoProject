/******************************************************
* Texture.cpp		�e�N�X�`�����
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/10
* �ŏI�X�V���F2024/10/18
*******************************************************/
#pragma once

#include "DirectX.h"

/****************************************************
* �e�N�X�`���N���X
*****************************************************/
class Texture {
public:
	//�摜�t�@�C����ǂݍ���
	bool Load(const std::string& filename);
	
	//�摜�n���h���擾
	const ComPtr<ID3D11ShaderResourceView> GetHandle() const { return m_srv; }
private:
	//�摜�f�[�^�ǂݎ��n���h��
	ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;
	//�摜���
	TexMetadata m_info = {};

};
