#include "Particle.hlsli"

// �O�p�`�̓��͂���A�O�p�`���o�͂��邪�A
// �r����UV�����H���ă^�C�����O
//[maxvertexcount(3)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	inout TriangleStream< GSOutput > output
//)
//{
//	for (uint i = 0; i < 3; i++)
//	{
//		GSOutput element;                   // �o�͗p���_�f�[�^
//		element.svpos = input[i].svpos;     // ���_���W���R�s�[
//		element.normal = input[i].normal;   // �@�����R�s�[
//		element.uv = input[i].uv;           // UV���R�s�[
//		// UV��2�{��
//		element.uv = input[i].uv * 2.0f;
//		output.Append(element);
//	}
//}

// �O�p�`�̓��͂���A�O�p�`��2�o��
//[maxvertexcount(6)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	inout TriangleStream< GSOutput > output
//)
//{
//	// 1�ڂ̎O�p�`
//	for (uint i = 0; i < 3; i++) {
//		GSOutput element;
//		element.svpos = input[i].svpos;
//		element.normal = input[i].normal;
//		element.uv = input[i].uv;
//		output.Append(element);
//	}
//	// ���݂̃X�g���b�v���I��
//	output.RestartStrip();
//
//	// 2�ڂ̎O�p�`
//	for (uint i = 0; i < 3; i++) {
//		GSOutput element;
//		// X������20���炷
//		element.svpos = input[i].svpos + float4(20.0f, 0.0f, 0.0f, 0.0f);
//		element.normal = input[i].normal;
//		// UV��5�{��
//		element.uv = input[i].uv * 5.0f;
//		output.Append(element);
//	}
//}

//// �O�p�`�̓��͂���A�_��1�o�͂���T���v��
//[maxvertexcount(1)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	// �_�X�g���[��
//	inout PointStream< GSOutput > output
//)
//{
//	GSOutput element;                   // �o�͗p���_�f�[�^
//	element.svpos = input[0].svpos;     // ���_���W���R�s�[
//	element.normal = input[0].normal;   // �@�����R�s�[
//	element.uv = input[0].uv;           // UV���R�s�[
//	// ���_��1�o�́i�o�̓��X�g�ɒǉ��j
//	output.Append(element);
//}
//
//// �O�p�`�̓��͂���A�_��3�o�͂���T���v��
//[maxvertexcount(3)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	// �_�X�g���[��
//	inout PointStream< GSOutput > output
//)
//{
//	for (uint i = 0; i < 3; i++)
//	{
//		GSOutput element;                   // �o�͗p���_�f�[�^
//		element.svpos = input[i].svpos;     // ���_���W���R�s�[
//		element.normal = input[i].normal;   // �@�����R�s�[
//		element.uv = input[i].uv;           // UV���R�s�[
//		// ���_���o�́i�o�̓��X�g�ɒǉ��j
//		output.Append(element);
//	}
//}
//
//
//// �O�p�`�̓��͂���A������1�o�͂���T���v��
//[maxvertexcount(2)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	// �����X�g���[��
//	inout LineStream< GSOutput > output
//)
//{
//	GSOutput element;                   // �o�͗p���_�f�[�^
//
//	// �����̎n�_
//	element.svpos = input[0].svpos;     // ���_���W���R�s�[
//	element.normal = input[0].normal;   // �@�����R�s�[
//	element.uv = input[0].uv;           // UV���R�s�[
//	output.Append(element);
//
//	// �����̏I�_
//	element.svpos = input[1].svpos;     // ���_���W���R�s�[
//	element.normal = input[1].normal;   // �@�����R�s�[
//	element.uv = input[1].uv;           // UV���R�s�[
//	output.Append(element);
//}
//
//// �O�p�`�̓��͂���A������3�o�͂���T���v��
//[maxvertexcount(6)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	// �����X�g���[��
//	inout LineStream< GSOutput > output
//)
//{
//	GSOutput element;                   // �o�͗p���_�f�[�^
//	// �O�p�`�̒��_1�_������
//	for (uint i = 0; i < 3; i++)
//	{
//		// �����̎n�_
//		element.svpos = input[i].svpos;     // ���_���W���R�s�[
//		element.normal = input[i].normal;   // �@�����R�s�[
//		element.uv = input[i].uv;           // UV���R�s�[
//		output.Append(element);
//
//		// �����̏I�_
//		if (i == 2) {
//			element.svpos = input[0].svpos;     // ���_���W���R�s�[
//			element.normal = input[0].normal;   // �@�����R�s�[
//			element.uv = input[0].uv;           // UV���R�s�[
//		}
//		else {
//			element.svpos = input[i + 1].svpos;
//			element.normal = input[i + 1].normal;
//			element.uv = input[i + 1].uv;
//		}
//		output.Append(element);
//		// ���݂̃X�g���b�v���I�����A���̃X�g���b�v���J�n
//		output.RestartStrip();
//	}
//}
//
//// �O�p�`�̓��͂���A������3�o�͂���T���v��
//[maxvertexcount(4)]
//void main(
//	triangle VSOutput input[3] : SV_POSITION,
//	// �����X�g���[��
//	inout LineStream< GSOutput > output
//)
//{
//	GSOutput element;                   // �o�͗p���_�f�[�^
//	// �O�p�`�̒��_1�_������
//	for (uint i = 0; i < 3; i++)
//	{
//		element.svpos = input[i].svpos;     // ���_���W���R�s�[
//		element.normal = input[i].normal;   // �@�����R�s�[
//		element.uv = input[i].uv;           // UV���R�s�[
//		// ���_��1�ǉ�
//		output.Append(element);
//	}
//	// �ŏ��̓_������1�x�ǉ�
//	element.svpos = input[0].svpos;     // ���_���W���R�s�[
//	element.normal = input[0].normal;   // �@�����R�s�[
//	element.uv = input[0].uv;           // UV���R�s�[
//	// ���_��1�ǉ�
//	output.Append(element);
//}

// �_�̓��͂���A�_���o��
//[maxvertexcount(1)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout PointStream< GSOutput > output
//)
//{
//	GSOutput element;
//	// ���_���
//	element.normal = input[0].normal;
//	element.uv = input[0].uv;
//	element.svpos = input[0].svpos;
//	output.Append(element);
//}

// �_�̓��͂���A�O�p�`���o��
//[maxvertexcount(3)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout TriangleStream< GSOutput > output
//)
//{
//	GSOutput element;
//	// ����
//	element.normal = input[0].normal;
//	element.uv = input[0].uv;
//	// 1�_��
//	element.svpos = input[0].svpos;
//	output.Append(element);
//	// 2�_��
//	element.svpos = input[0].svpos + float4(10.0f, 10.0f, 0, 0);
//	output.Append(element);
//	// 3�_��
//	element.svpos = input[0].svpos + float4(10.0f, 0, 0, 0);
//	output.Append(element);
//}

// �l�p�`�̒��_��
static const uint vnum = 4;

// �Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0,0), // ����
	float4(-0.5f, +0.5f, 0,0), // ����
	float4(+0.5f, -0.5f, 0,0), // �E��
	float4(+0.5f, +0.5f, 0,0)  // �E��
};

// ���オ0,0 �E����1,1
static const float2 uv_array[vnum] =
{
	float2(0, 1), // ����
	float2(0, 0), // ����
	float2(1, 1), // �E��
	float2(1, 0)  // �E��
};

// �_�̓��͂���A�l�p�`���o��
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	// 4�_���܂킷
	for (uint i = 0; i < vnum; i++) {
		// ���S����I�t�Z�b�g���r���{�[�h��]�i���f�����W�j
		//float4 offset = mul(matBillboard, offset_array[i]);
		//���S����̃I�t�Z�b�g���X�P�[�����O
		float4 offset = offset_array[i]*input[0].scale;
		// ���S����̃I�t�Z�b�g���r���{�[�h��]�i���f�����W�j
		offset = mul(matBillboard, offset);
		// �I�t�Z�b�g�����炷�i���[���h���W�j
		element.svpos = input[0].pos + offset;
		// �r���[�v���W�F�N�V�����ϊ�
		element.svpos = mul(mat, element.svpos);
		//element.uv = float2(0.5f, 0.5f);
		element.uv = uv_array[i];
		output.Append(element);
	}
}
