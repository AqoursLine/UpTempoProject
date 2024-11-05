/******************************************************
* Texture.cpp		�e�N�X�`�����
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/10
* �ŏI�X�V���F2024/10/18
*******************************************************/
#include "framework.h"
#include <locale.h>

#include "Direct3D.h"
#include "Texture.h"

/******************************************************
* �摜�̓Ǎ�
* �߂�l
*	bool	�Ǎ�������������
* ����
*	string	�摜�̃p�X
*******************************************************/
bool Texture::Load(const std::string& filename) {
	//�}���`�o�C�g�����񂩂烏�C�h������֕ϊ�
	setlocale(LC_CTYPE, "jpn");
	wchar_t wFilename[256];
	size_t ret;
	mbstowcs_s(&ret, wFilename, filename.c_str(), 256);

	//WIC�摜��ǂݍ���
	auto image = std::make_unique<ScratchImage>();
	if (FAILED(LoadFromWICFile(wFilename, WIC_FLAGS_NONE, &m_info, *image))) {
		//���s
		m_info = {};
		return false;
	}

	//�~�b�v�}�b�v�̐���
	if (m_info.mipLevels == 1) {
		auto mipChain = std::make_unique<ScratchImage>();
		if (SUCCEEDED(GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), TEX_FILTER_DEFAULT, 0, *mipChain))) {
			image = std::move(mipChain);
		}
	}

	//���\�[�X�ƃV�F�[�_�[���\�[�X�r���[���쐬
	if (FAILED(CreateShaderResourceView(D3D.GetDevice(), image->GetImages(), image->GetImageCount(), m_info, &m_srv))) {
		//���s
		m_info = {};
		return false;
	}

	//����
	return true;
}

