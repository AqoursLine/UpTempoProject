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

std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> Texture::m_textureCache;

/******************************************************
* 画像の読込
* 戻り値
*	bool	読込が成功したか
* 引数
*	string	画像のパス
*******************************************************/
bool Texture::Load(const std::wstring& filename) {
	////WIC画像を読み込む
	//auto image = std::make_unique<ScratchImage>();
	//if (FAILED(LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, &m_info, *image))) {
	//	//失敗
	//	m_info = {};
	//	return false;
	//}

	////ミップマップの生成
	//if (m_info.mipLevels == 1) {
	//	auto mipChain = std::make_unique<ScratchImage>();
	//	if (SUCCEEDED(GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), TEX_FILTER_DEFAULT, 0, *mipChain))) {
	//		image = std::move(mipChain);
	//	}
	//}

	////リソースとシェーダーリソースビューを作成
	//if (FAILED(CreateShaderResourceView(D3D.GetDevice(), image->GetImages(), image->GetImageCount(), m_info, &m_srv))) {
	//	//失敗
	//	m_info = {};
	//	return false;
	//}

	////成功
	//return true;


	/*********************************
	 テクスチャ読み込み高速化ver.
	**********************************/
	// キャッシュを利用して既にロード済みか確認する。
	if (m_textureCache.find(filename) != m_textureCache.end()) {
		// キャッシュにあったらそのデータを渡す。
		m_srv = m_textureCache[filename];
		return true;
	}

	// WIC画像を読み込む（フォーマットをRBGに統一。sRGBのプロファイル処理を省略。）
	auto image = std::make_unique<ScratchImage>();
	if (FAILED(LoadFromWICFile(filename.c_str(), WIC_FLAGS_FORCE_RGB | WIC_FLAGS_IGNORE_SRGB, &m_info, *image))) {
		// 失敗
		m_info = {};
		return false;
	}

	// ミップマップの生成
	//if (m_info.mipLevels == 1) {
	//    auto mipChain = std::make_unique<ScratchImage>();
	//    if (SUCCEEDED(GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), TEX_FILTER_DEFAULT, 0, *mipChain))) {
	//        image = std::move(mipChain);
	//    }
	//}

	// リソースとシェーダーリソースビューを作成
	if (FAILED(CreateShaderResourceView(D3D.GetDevice(), image->GetImages(), image->GetImageCount(), m_info, &m_srv))) {
		// 失敗
		m_info = {};
		return false;
	}

	// GPUでミップマップを生成（可能な場合）
	D3D.GetDeviceContext()->GenerateMips(m_srv.Get());

	// キャッシュに保存
	m_textureCache[filename] = m_srv;

	// 成功
	return true;
}
