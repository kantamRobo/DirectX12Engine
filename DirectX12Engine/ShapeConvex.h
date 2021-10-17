//
//	ShapeConvex.h
//
#pragma once
#include "Shape.h"


int FindPointFurrthestInDir(const DirectX::XMVECTOR* pts, const int num, const DirectX::XMVECTOR& dir)
{
	int maxIdx = 0;
	float maxDist = DirectX::XMVector3Dot(dir, pts[0]).m128_f32[0];
	for (int i = 1; i < num; i++)
	{
		float dist = DirectX::XMVector3Dot(dir, pts[i]).m128_f32[0];
		if (dist > maxDist)
		{
			maxDist = dist;
			maxIdx = i;
		}
	}
	return maxIdx;
}

float DistanceFromLine(const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b, const DirectX::XMVECTOR& pt)
{
	DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(b, a);
	ab =DirectX::XMVector3Normalize(ab);

	DirectX::XMVECTOR ray = DirectX::XMVectorSubtract(pt, a);
	DirectX::XMVECTOR projection = DirectX::XMVectorScale(ab, DirectX::XMVector3Dot(ray, ab).m128_f32[0]);
	DirectX::XMVECTOR perpindicular = DirectX::XMVectorSubtract(ray, projection);
	return DirectX::XMVector3LengthSq(perpindicular).m128_f32[0];
}
DirectX::XMVECTOR FindpointFurthestFromLine(const DirectX::XMVECTOR* pts,
	const int num, const DirectX::XMVECTOR& ptA, const DirectX::XMVECTOR& ptB)
{
	int maxIdx = 0;
	float maxDist = DistanceFromLine(ptA, ptB, pts[0]);
	for (int i = 1; i < num; i++)
	{
		float dist = DistanceFromLine(ptA, ptB, pts[i]);
		if (dist > maxDist)
		{
			maxDist = dist;
			maxIdx = i;
		}
	}
	return pts[maxIdx];
}

float DistanceFromTrinangle(const DirectX::XMVECTOR& a,
	const DirectX::XMVECTOR& b, 
	const DirectX::XMVECTOR& c, const DirectX::XMVECTOR& pt)
{
	DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(b, a);
	DirectX::XMVECTOR ac = DirectX::XMVectorSubtract(c, a);
	DirectX::XMVECTOR normal = DirectX::XMVector3Cross(ab, ac);
	normal = DirectX::XMVector3Normalize(normal);

	DirectX::XMVECTOR ray = DirectX::XMVectorSubtract(pt, a);
	float dist = DirectX::XMVector3Dot(ray, normal).m128_f32[0];
	return dist;

}

DirectX::XMVECTOR FindPointFurthestFromTriangle(const DirectX::XMVECTOR* pts, const int num, const DirectX::XMVECTOR& ptA, const DirectX::XMVECTOR& ptB, const DirectX::XMVECTOR& ptC)
{
	int maxIdx = 0;
	float maxDist = DistanceFromTrinangle(ptA, ptB, ptC, pts[0]);
	for (int i = 1; i < num; i++)
	{
		float dist = DistanceFromTrinangle(ptA, ptB, ptC, pts[i]);
		if (dist * dist > maxDist * maxDist)
		{
			maxDist = dist;
			maxIdx = i;
		}
		return pts[maxIdx];
	}
}
struct tri_t {
	int a;
	int b;
	int c;
};
void BuildTetrahedron(const DirectX::XMVECTOR* verts, const int num, std::vector<DirectX::XMVECTOR>& hullPts, std::vector<tri_t>& hullTris)
{
	hullPts.clear();
	hullTris.clear();

	DirectX::XMVECTOR points[4];

	int idx = FindPointFurrthestInDir(verts, num, DirectX::XMVectorSet(1.0f, 0, 0,0));
	points[0] = verts[idx];
	idx = FindPointFurrthestInDir(verts, num, DirectX::XMVectorScale
		(points[0] ,-1.0f));
	points[1] = verts[idx];
	idx = FindPointFurrthestInDir(verts, num, DirectX::XMVectorScale
	(points[0], -1.0f));
	points[1] = verts[idx];
	points[2] = FindpointFurthestFromLine(verts, num, points[0], points[1]);
	points[3] = FindPointFurthestFromTriangle(verts, num, points[0], points[1], points[2]);

	float dist = DistanceFromTrinangle(points[0], points[1], points[2], points[3]);
	if (dist > 0.0f) {
		std::swap(points[0], points[1]);
	}

	//élñ ëÃÇçÏÇÈ
	hullPts.push_back(points[0]);
	hullPts.push_back(points[1]);
	hullPts.push_back(points[2]);
	hullPts.push_back(points[3]);

	tri_t tri;
	tri.a = 0;
	tri.b = 1;
	tri.c = 2;
	hullTris.push_back(tri);

	tri.a = 0;
	tri.b = 2;
	tri.c = 3;
	hullTris.push_back(tri);

	tri.a = 2;
	tri.b = 1;
	tri.c = 3;
	hullTris.push_back(tri);

	tri.a = 1;
	tri.b = 0;
	tri.c = 3;
	hullTris.push_back(tri);
}

void ExpandConvexHull(std::vector<DirectX::XMVECTOR>& hullpoints, std::vector<tri_t>& hullTris, const std::vector<DirectX::XMVECTOR> verts)
{
	std::vector<DirectX::XMVECTOR> externalVerts = verts;
	RemoveInternalPoints(hullpoints, hullTris, externalVerts);

	while (externalVerts.size() > 0)
	{
		int ptIdx = FindPointFurrthestInDir(externalVerts.data(), (int)externalVerts.size(),
			externalVerts[0]);

		DirectX::XMVECTOR pt= DirectX::XMVectorSet( externalVerts[ptIdx].m128_f32[0], externalVerts[ptIdx].m128_f32[1], externalVerts[ptIdx].m128_f32[2],
			0);

		externalVerts.erase(externalVerts.begin() + ptIdx);

		AddPoint(hullpoints, hullTris, pt);

		RemoveInternalPoints(hullpoints, hullTris, externalVerts);
	}

	removeUnreferencedVerts(hullpoints, hullTris);
}
void RemoveInternalPoints(const std::vector<DirectX::XMVECTOR>& hullPoints
	, const std::vector<tri_t>& hullTris, std::vector<DirectX::XMVECTOR>& checkPts)
{
	for (int i = 0; i < checkPts.size(); i++)
	{
		const DirectX::XMVECTOR pt = checkPts[i];

		bool isExternal = false;

		for (int t = 0; t < hullTris.size(); t++)
		{
			const tri_t& tri = hullTris[t];
			const DirectX::XMVECTOR& a = hullPoints[tri.a];
			const DirectX::XMVECTOR& b = hullPoints[tri.b];
			const DirectX::XMVECTOR& c = hullPoints[tri.c];

			float dist = DistanceFromTrinangle(a, b, c, pt);
			if (dist > 0.0f)
			{
				isExternal = true;
				break;
			}
		}

		if (!isExternal)
		{
			checkPts.erase(checkPts.begin() + i);
			i--;
		}

		

	}
}

struct edge_t {
	int a;
	int b;

	bool operator == (const edge_t& rhs) const {
		return ((a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a));
	}
};

/*
====================================================
ShapeConvex
====================================================
*/
class ShapeConvex : public Shape {
public:
	explicit ShapeConvex(const DirectX::XMVECTOR* pts, const int num) {
		Build(pts, num);
	}
	void Build(const DirectX::XMVECTOR* pts, const int num);

	DirectX::XMVECTOR Support(const DirectX::XMVECTOR& dir, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient, const float bias) const ;

	DirectX::XMMATRIX InertiaTensor() const  { return m_inertiaTensor; }

	Bounds GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient) const ;
	Bounds GetBounds() const override { return m_bounds; }

	float FastestLinearSpeed(const DirectX::XMVECTOR& angularVelocity, const DirectX::XMVECTOR& dir) const;

	shapeType_t GetType() const override { return SHAPE_CONVEX; }

public:
	std::vector< DirectX::XMVECTOR  > m_points;
	Bounds m_bounds;
	DirectX::XMMATRIX m_inertiaTensor;
};

Bounds ShapeConvex::GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient) const
{
	DirectX::XMVECTOR corners[8];
	corners[0] = DirectX::XMVectorSet(m_bounds.mins.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.mins.m128_f32[2], m_bounds.mins.m128_f32[3]);
	corners[1] = DirectX::XMVectorSet(
		m_bounds.mins.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);
	corners[2] = DirectX::XMVectorSet(
		m_bounds.mins.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.mins.m128_f32[2], 0);
	corners[3] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.mins.m128_f32[2], 0);


	corners[4] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);
	corners[5] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.mins.m128_f32[2], 0);
	corners[6] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);
	corners[7] = DirectX::XMVectorSet(
		m_bounds.mins.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);

	Bounds bounds;
	for (int i = 0; i < 8; i++)
	{
		corners[i] = RotatePoint(orient, DirectX::XMVectorAdd(corners[i], pos));
		bounds.Expand(corners[i]);
	}

	return bounds;
}

float ShapeConvex::FastestLinearSpeed(const DirectX::XMVECTOR& angularVelocity, const DirectX::XMVECTOR& dir) const
{
	float maxSpeed = 0.0f;
	for (int i = 0; i < m_points.size(); i++)
	{
		DirectX::XMVECTOR r = DirectX::XMVectorSubtract(m_points[i], DirectX::XMLoadFloat3
		(&m_centerOfMass));
		DirectX::XMVECTOR linearVelocity = DirectX::XMVector3Cross(angularVelocity, r);

		float speed = DirectX::XMVector3Dot(dir, linearVelocity).m128_f32[0];
		maxSpeed = speed;
	}
	return maxSpeed;
}
