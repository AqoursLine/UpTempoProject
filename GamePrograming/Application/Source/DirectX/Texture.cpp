/******************************************************
* Texture.cpp		テクスチャ情報
* 制作者：ミヤタジョウジ
* 作成日：2024/10/10
* 最終更新日：2024/10/18
*******************************************************/
#include "framework.h"
#include <locale.h>

#include "Direct3D.h"
#include "Texture.h"

/******************************************************
* 画像の読込
* 戻り値
*	bool	読込が成功したか
* 引数
*	string	画像のパス
*******************************************************/
bool Texture::Load(const std::string& filename) {
	//マルチバイト文字列からワイド文字列へ変換
	setlocale(LC_CTYPE, "jpn");
	wchar_t wFilename[256];
	size_t ret;
	mbstowcs_s(&ret, wFilename, filename.c_str(), 256);

	//WIC画像を読み込む
	auto image = std::make_unique<ScratchImage>();
	if (FAILED(LoadFromWICFile(wFilename, WIC_FLAGS_NONE, &m_info, *image))) {
		//失敗
		m_info = {};
		return false;
	}

	//ミップマップの生成
	if (m_info.mipLevels == 1) {
		auto mipChain = std::make_unique<ScratchImage>();
		if (SUCCEEDED(GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), TEX_FILTER_DEFAULT, 0, *mipChain))) {
			image = std::move(mipChain);
		}
	}

	//リソースとシェーダーリソースビューを作成
	if (FAILED(CreateShaderResourceView(D3D.GetDevice(), image->GetImages(), image->GetImageCount(), m_info, &m_srv))) {
		//失敗
		m_info = {};
		return false;
	}

	//成功
	return true;
}

