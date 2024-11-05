//0�Ԃ̃e�N�X�`���X���b�g���g�p����
Texture2D g_texture : register(t0);
//0�Ԃ̃T���v���X���b�g���g�p����
SamplerState g_sampler : register(s0);

//���_�V�F�[�_�[����o�͂���f�[�^
struct VSOutput
{
	float4 Pos : SV_Position;	//���_�̍��W(�ˉe���W�n)
	float2 UV : TEXCOORD0;		//UV���W
};

//�}�g���N�X�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
}

//=====================================================
// ���_�V�F�[�_�[
//=====================================================
VSOutput VS(float4 pos : POSITION, float2 uv : TEXUV)
{
	VSOutput Out;
    //���_���W���o��
    Out.Pos = mul(pos, WorldViewProjection);
	//���_��UV���W���A�������H�������̂܂܏o��
	Out.UV = uv;
	return Out;
}

//=====================================================
// �s�N�Z���V�F�[�_�[
//=====================================================
float4 PS(VSOutput In) : SV_target0
{
	//�e�N�X�`������F���擾
	float4 texColor = g_texture.Sample(g_sampler, In.UV);

	//�e�N�X�`���̐F���o��
	return texColor;
}
