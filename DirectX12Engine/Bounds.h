//
//	Bounds.h
//
#pragma once
#include <math.h>
#include <assert.h>

#include <vector>

/*
====================================================
Bounds
====================================================
*/
class Bounds {
public:
	Bounds() { Clear(); }
	Bounds(const Bounds& rhs) : mins(rhs.mins), maxs(rhs.maxs) {}
	const Bounds& operator = (const Bounds& rhs);
	~Bounds() {}

	void Clear() { mins = DirectX::XMVectorSet(1e6,1e6,1e6,0); maxs = DirectX::XMVectorSet(1e6, 1e6, 1e6, 0); }
	bool DoesIntersect(const Bounds& rhs) const;
	void Expand(const DirectX::XMVECTOR* pts, const int num);
	void Expand(const DirectX::XMVECTOR& rhs);
	void Expand(const Bounds& rhs);

	
	float WidthX() const { return maxs.m128_f32[0] - mins.m128_f32[0]; }
	float WidthY() const { return maxs.m128_f32[1] - mins.m128_f32[1]; }
	float WidthZ() const { return maxs.m128_f32[2] - mins.m128_f32[2]; }

public:
	DirectX::XMVECTOR mins;
	DirectX::XMVECTOR maxs;
};