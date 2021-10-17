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

}