//
//	ShapeConvex.h
//
#pragma once
#include "Shape.h"
#include "MathUtility.h"

using namespace MathUtility;
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

	//四面体を作る
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

	RemoveUnreferencedVerts(hullpoints, hullTris);
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

bool IsEdgeUnique(const std::vector<tri_t>& tris, const std::vector<int>& facingTris,
	const int ignoreTri, const edge_t& edge)
{
	for (int i = 0; i < facingTris.size(); i++)
	{
		const int triIdx = facingTris[i];
		if (ignoreTri == triIdx)
		{
			continue;
		}

		const tri_t& tri = tris[triIdx];

		edge_t edges[3];
		edges[0].a = tri.a;
		edges[0].b = tri.b;

		edges[1].a = tri.b;
		edges[1].b = tri.c;

		edges[2].a = tri.c;
		edges[2].b = tri.a;

		for (int e = 0; e < 3; e++)
		{
			if (edge == edges[e])
			{
				return false;
			}
		}

		return true;
	}
}

void AddPoint(std::vector<DirectX::XMVECTOR>& hullPoints, std::vector<tri_t>& hullTris
	, const DirectX::XMVECTOR pt)
{

	//この点に面する全ての三角形を見つける

	std::vector<int> facingTris;
	for (int i = static_cast<int>(hullTris.size() - 1); i >= 0; i--) {
		const tri_t& tri = hullTris[i];
		
		const DirectX::XMVECTOR& a = hullPoints[tri.a];
		const DirectX::XMVECTOR& b = hullPoints[tri.b];
		const DirectX::XMVECTOR& c = hullPoints[tri.c];

		const float dist = DistanceFromTrinangle(a, b, c, pt);

		if (dist > 0.0f)
		{
			facingTris.push_back(i);
		}
	}

	//三角形に特有の辺を見つける。これらは新しい三角形を形成する
	std::vector<edge_t> uniqueEdges;
	for (int i = 0; i < facingTris.size(); i++)
	{
		const int triIdx = facingTris[i];
		const tri_t& tri = hullTris[triIdx];

		edge_t edges[3];
		edges[0].a = tri.a;
		edges[0].b = tri.b;

		edges[1].a = tri.b;
		edges[1].b = tri.c;

		edges[2].a = tri.c;
		edges[2].b = tri.a;
		
		for (int e = 0; e < 3; e++)
		{
			if (IsEdgeUnique(hullTris, facingTris, triIdx, edges[e]))
			{
				uniqueEdges.push_back(edges[e]);
			}
		}

	}

	//そして、古い三角面を消す
	for (int i = 0; i < facingTris.size(); i++)
	{
		hullTris.erase(hullTris.begin() + facingTris[i]);
	}

	//そして新しい点を加える。
	hullPoints.push_back(pt);
	const int newPtIdx = (int)hullPoints.size() - 1;

	//そしてそれぞれの辺に対して三角形を加える

	for (int i = 0; i < uniqueEdges.size(); i++)
	{
		const edge_t& edge = uniqueEdges[i];

		tri_t tri;

		tri.a = edge.a;
		tri.b = edge.b;
		tri.c = newPtIdx;

		hullTris.push_back(tri);
	}
}

void RemoveUnreferencedVerts(std::vector<DirectX::XMVECTOR>& hullPoints,
	std::vector<tri_t>& hullTris)
{
	for (int i = 0; i < hullPoints.size(); i++)
	{
		bool isUsed = false;
		for (int j = 0; j < hullTris.size(); j++)
		{
			const tri_t& tri = hullTris[j];

			if (tri.a == i || tri.b == i || tri.c == i)
			{
				isUsed = true;
				break;
			}
		}

		if (isUsed)
		{
			continue;
		}

		for (int j = 0; j < hullTris.size(); j++)
		{
			tri_t& tri = hullTris[j];
			if (tri.a > i)
			{
				tri.a--;
			}
			if (tri.b > i)
			{
				tri.b--;
			}
			if (tri.c > i)
			{
				tri.c--;
			}
		}

		hullPoints.erase(hullPoints.begin() + i);

		i--;
	}
}


void BuildConvexHull(const std::vector<DirectX::XMVECTOR>& verts, std::vector<DirectX::XMVECTOR>& hullPts, std::vector<tri_t>& hullTris)
{
	if (verts.size() < 4)
	{
		return;
	}

	//四面体を作成する。
	BuildTetrahedron(verts.data(), (int)verts.size(), hullPts, hullTris);

	ExpandConvexHull(hullPts, hullTris, verts);

}

bool IsExternal(const std::vector<DirectX::XMVECTOR>& pts, const std::vector<tri_t>& tris, const DirectX::XMVECTOR& pt)
{
	bool IsExternal = false;

	for (int t = 0; t < tris.size(); t++)
	{
		const tri_t& tri = tris[t];
		const DirectX::XMVECTOR& a = pts[tri.a];
		const DirectX::XMVECTOR& b = pts[tri.b];
		const DirectX::XMVECTOR& c = pts[tri.c];

		//点が三角形の外にあるなら、external判定が真
		float dist = DistanceFromTrinangle(a, b, c, pt);
		if (dist > 0.0f){
			IsExternal = true;
			break;
		}
	}

	return IsExternal;
}

DirectX::XMVECTOR CalculateCenterOFMass(const std::vector<DirectX::XMVECTOR>& pts, const std::vector<tri_t>& tris)
{
	const int numSamples = 100;

	Bounds bounds;
	bounds.Expand(pts.data(), pts.size());

	DirectX::XMVECTOR cm=DirectX::XMVectorSet(0.0f, 0.0f,0.0f, 0.0f);

	const float dx = bounds.WidthX() / static_cast<float>(numSamples);
	const float dy = bounds.WidthY() / static_cast<float>(numSamples);
	const float dz = bounds.WidthZ() / static_cast<float>(numSamples);

	int sampleCount = 0;
	for (float x = bounds.mins.m128_f32[0]; x < bounds.maxs.m128_f32[0]; x += dx)
	{
		for (float y = bounds.mins.m128_f32[1]; y < bounds.maxs.m128_f32[1]; y += dy)
		{
			for (float z = bounds.mins.m128_f32[2]; x < bounds.maxs.m128_f32[2]; z += dz)
			{
				DirectX::XMVECTOR pt = DirectX::XMVectorSet(x, y, z, 0);

				if (IsExternal(pts, tris, pt))
				{
					continue;
				}
				cm = DirectX::XMVectorAdd( cm,pt);
				sampleCount++;
			}
		}
	}

	cm = DirectX::XMVectorDivide(cm, DirectX::XMVectorSet(sampleCount, sampleCount, sampleCount, 0));

	return cm;
}
//2021/10/19
DirectX::XMMATRIX CalculateInetiaTensor(const std::vector<DirectX::XMVECTOR>& pts, const std::vector<tri_t>& tris, const DirectX::XMVECTOR cm)
{
	const int numSamples = 100;
	
	Bounds bounds;
	bounds.Expand(pts.data(), static_cast<int>(pts.size()));

	DirectX::XMMATRIX tensor;
	SetMatrixZero(tensor);

	const float dx = bounds.WidthX() / static_cast<float>(numSamples);
	const float dy = bounds.WidthY() / static_cast<float>(numSamples);
	const float dz = bounds.WidthZ() / static_cast<float>(numSamples);
	int SampleCount = 0;
	for (float x = bounds.mins.m128_f32[0]; x < bounds.maxs.m128_f32[0]; x += dx)
	{
		for (float y = bounds.mins.m128_f32[1]; y < bounds.maxs.m128_f32[1]; y += dy)
		{
			for (float z = bounds.mins.m128_f32[2]; x < bounds.maxs.m128_f32[2]; z += dz)
			{
				DirectX::XMVECTOR pt = DirectX::XMVectorSet(x, y, z);

				if (IsExternal(pts, tris, pt))
				{
					continue;
				}
				//重心に関係のある点を取得する
				pt = DirectX::XMVectorSubtract(pt, cm);

				DirectX::XMMATRIX rhs = DirectX::XMMatrixSet(
					pt.m128_f32[1] * pt.m128_f32[1] + pt.m128_f32[2] * pt.m128_f32[2],//11
					0,//12
					0,//13
					0,//14
					0,//21
					pt.m128_f32[2] * pt.m128_f32[2] + pt.m128_f32[0] * pt.m128_f32[0],//22
					0,//23
					0,//24
					0,//31
					0,//32
					pt.m128_f32[0] * pt.m128_f32[0] + pt.m128_f32[1] * pt.m128_f32[1],//33
					0,//34
					0//41
					,0//42
					,0//43
					,0);//44
					AddXMMATRIX(tensor, rhs);
					SampleCount++;
			}
		}
	}
	tensor = DirectX::XMMatrixMultiply(tensor
		,DirectX::XMMatrixScaling(1 / 
			static_cast<float>(SampleCount),
			1 / static_cast<float>(SampleCount),
			1 / static_cast<float>(SampleCount)));

	return tensor;
}

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

void ShapeConvex::Build(const DirectX::XMVECTOR* pts, const int num)
{
	m_points.clear();
	m_points.reserve(num);

	for (int i = 0; i < num; i++)
	{
		m_points.push_back(pts[i]);
	}

	//凸包に拡張する
	std::vector<DirectX::XMVECTOR> hullPoints;
	std::vector<tri_t> hullTriangles;
	BuildConvexHull(m_points, hullPoints, hullTriangles);

	m_points = hullPoints;

	//バウンドを拡張する
	m_bounds.Clear();
	m_bounds.Expand(m_points.data(), m_points.size());
	DirectX::XMVECTOR temp_m_centerOfMass = DirectX::XMLoadFloat3(&m_centerOfMass);
	temp_m_centerOfMass = CalculateCenterOFMass(hullPoints, hullTriangles);
	DirectX::XMStoreFloat3(&m_centerOfMass, temp_m_centerOfMass);
	m_inertiaTensor = CalculateInetiaTensor(hullPoints, hullTriangles, DirectX::XMLoadFloat3(&m_centerOfMass));;
}

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
