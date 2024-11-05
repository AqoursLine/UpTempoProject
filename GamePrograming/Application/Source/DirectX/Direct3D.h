/******************************************************
* Direct3D.h		�_�C���N�gX11���Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/5
* �ŏI�X�V���F2024/10/21
*******************************************************/
#pragma once

#include "DirectX.h"

constexpr int VERTEX_MAX = (4);

class Texture;

//2D�p���_�\����
struct VertexType2D {
	XMFLOAT3 Pos;	//���W
	XMFLOAT2 UV;	//UV���W
};

/****************************************************
* Direct3D�N���X
*****************************************************/
class Direct3D {
public:
	//������
	bool Initialize(HWND hWnd, int width, int height);
	//�I��
	void Finalize();

	//=====================================================
	// �`��
	// ����
	//	Texture		�e�N�X�`���N���X
	//	float x		���WX
	//	float y		���WY
	//	float w		�T�C�YX
	//	float h		�T�C�YY
	//	float r		��]
	//	float u		�e�N�X�`�����WX
	//	float v		�e�N�X�`�����Wy
	//	float tw	�e�N�X�`���\����
	//	float th	�e�N�X�`���\������
	//=====================================================
	void Draw2D(const Texture& text, float x, float y, float w, float h, float r, float u, float v, float tw, float th);
	//���_�f�[�^�ݒ�
	void SetVertex();
	//�N���A
	void Clear();
	//�[�x
	void SetDepthEnable(bool eneble);
	//�v���[���g
	void Present();

	//�}�g���N�X
	void SetWorldViewProjection2D(void);
	void SetWorldMatrix(XMMATRIX worldMatrix);
	void SetViewMatrix(XMMATRIX viewMatrix);
	void SetProjectionMatrix(XMMATRIX projectionMatrix);
	void UpdateWorldViewProjection();

	//�Q�b�^�[
	ID3D11Device* GetDevice() { return m_device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() { return m_swapChain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() { return m_backBufferView.Get(); }

private:
	//DirectX�p�ϐ�
	ComPtr<ID3D11Device>				m_device = nullptr;				//Direct3D�f�o�C�X
	ComPtr<ID3D11DeviceContext>			m_deviceContext = nullptr;		//Direct3D�f�o�C�X�R���e�L�X�g
	ComPtr<IDXGISwapChain>				m_swapChain = nullptr;			//�X���b�v�`�F�C��
	ComPtr<ID3D11RenderTargetView>		m_backBufferView = nullptr;		//�o�b�N�o�b�t�@��RT�r���[
	ComPtr<ID3D11DepthStencilView>		m_depthStencilView = nullptr;	//�X�e���V���r���[

	//�V�F�[�_�[�p�ϐ�
	ComPtr<ID3D11VertexShader>	m_spriteVS = nullptr;			//���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader>	m_spritePS = nullptr;			//�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout>	m_spriteInputLayout = nullptr;	//���̓��C�A�E�g
	ComPtr<ID3D11Buffer>		m_constantBuffer = nullptr;		//�萔�o�b�t�@

	//�[�x�X�e���V��
	ComPtr<ID3D11DepthStencilState>		m_depthStencilEnable = nullptr;		//�[�x�L���X�e���V���X�e�[�g
	ComPtr<ID3D11DepthStencilState>		m_depthStencilDisable = nullptr;	//�[�x�����X�e���V���X�e�[�g

	//�`��p�ϐ�
	ComPtr<ID3D11Buffer>		m_vertexBuffer = nullptr;	//���_�o�b�t�@
	VertexType2D				m_vertex[VERTEX_MAX] = {};

	//�s��ϐ�
	XMMATRIX					m_worldMatrix;					//���[���h�s��
	XMMATRIX					m_viewMatrix;					//�r���[�s��
	XMMATRIX					m_projectionMatrix;				//�v���W�F�N�V�����s��


	// ���񂱂̃N���X�́A�ǂ�����ł��A�N�Z�X�ł���悤�ɃV���O���g���p�^�[���ɂ���
	//�B��̃C���X�^���X�p�̃|�C���^
	static inline Direct3D* s_instance;
	//�R���X�g���N�^��private�ɂ���
	Direct3D() {}

public:
	//�C���X�^���X�쐬
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new Direct3D();
	}
	//�C���X�^���X�폜
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//�B��̃C���X�^���X���擾
	static Direct3D& GetInstance() {
		return *s_instance;
	}
};

//Direct3D�̗B��̃C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
#define D3D Direct3D::GetInstance()
