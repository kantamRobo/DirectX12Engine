#pragma once
#include <assimp/matrix4x4.h>
#include <DirectXMath.h>

namespace MathUtility
{

	void ConvertToaiMatrix4x4toXMFLOAT4x4(const aiMatrix4x4& src, DirectX::XMFLOAT4X4& dst)
	{
		dst._11 = src.a1; dst._12 = src.a2; dst._13 = src.a3; dst._14 = src.a4;
		dst._21 = src.b1; dst._22 = src.b2; dst._23 = src.b3; dst._24 = src.b4;
		dst._31 = src.c1; dst._32 = src.c2; dst._33 = src.c3; dst._34 = src.c4;
		dst._41 = src.d1; dst._42 = src.d2; dst._43 = src.d3; dst._44 = src.d4;



	}

	void SetMatrixZero(DirectX::XMMATRIX& dst)
	{
		dst = DirectX::XMMatrixSet(0, 0, 0, 0
			, 0, 0, 0, 0
			, 0, 0, 0, 0
			, 0, 0, 0, 0);
	}

	void AddXMMATRIX(DirectX::XMMATRIX& dst, const DirectX::XMMATRIX& rhs)
	{
		dst.r[0].m128_f32[0] = dst.r[0].m128_f32[0] + rhs.r[0].m128_f32[0];
		dst.r[0].m128_f32[1] = dst.r[0].m128_f32[1] + rhs.r[0].m128_f32[1];
		dst.r[0].m128_f32[2] = dst.r[0].m128_f32[2] + rhs.r[0].m128_f32[2];
		dst.r[0].m128_f32[3] = dst.r[0].m128_f32[3] + rhs.r[0].m128_f32[3];

		dst.r[1].m128_f32[0] = dst.r[1].m128_f32[0] + rhs.r[1].m128_f32[0];
		dst.r[1].m128_f32[1] = dst.r[1].m128_f32[1] + rhs.r[1].m128_f32[1];
		dst.r[1].m128_f32[2] = dst.r[1].m128_f32[2] + rhs.r[1].m128_f32[2];
		dst.r[1].m128_f32[3] = dst.r[1].m128_f32[3] + rhs.r[1].m128_f32[3];

		dst.r[2].m128_f32[0] = dst.r[2].m128_f32[0] + rhs.r[2].m128_f32[0];
		dst.r[2].m128_f32[1] = dst.r[2].m128_f32[1] + rhs.r[2].m128_f32[1];
		dst.r[2].m128_f32[2] = dst.r[2].m128_f32[2] + rhs.r[2].m128_f32[2];
		dst.r[2].m128_f32[3] = dst.r[2].m128_f32[3] + rhs.r[2].m128_f32[3];

		dst.r[3].m128_f32[0] = dst.r[3].m128_f32[0] + rhs.r[3].m128_f32[0];
		dst.r[3].m128_f32[1] = dst.r[3].m128_f32[1] + rhs.r[3].m128_f32[1];
		dst.r[3].m128_f32[2] = dst.r[3].m128_f32[2] + rhs.r[3].m128_f32[2];
		dst.r[3].m128_f32[3] = dst.r[3].m128_f32[3] + rhs.r[3].m128_f32[3];
	}

	DirectX::XMVECTOR SetVectorZero(DirectX::XMVECTOR& dst)
	{
		dst = DirectX::XMVectorSet(0, 0, 0, 0);

		return dst;
	}
	bool XMVECTOREQUAL(const DirectX::XMVECTOR& lhs, const DirectX::XMVECTOR& rhs)
	{

		return lhs.m128_f32[0] == rhs.m128_f32[0] && lhs.m128_f32[1] == rhs.m128_f32[1]
			&& lhs.m128_f32[2] == rhs.m128_f32[2] && lhs.m128_f32[3] == rhs.m128_f32[3];
	}

	float GetVectorIndex(const DirectX::XMVECTOR lhs, const int idx)
	{
		return (&lhs.m128_f32[0])[idx];
	}
}


	