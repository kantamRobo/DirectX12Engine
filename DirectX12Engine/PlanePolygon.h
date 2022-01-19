#pragma once
#include <GraphicsMemory.h>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include <Effects.h>
#include <SimpleMath.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <array>
#include <initializer_list>
#include "Transform.h"
#include "pch.h"
#include "Character.h"
#include <Model.h>
class PlanePolygon
{
public:
	PlanePolygon() {};
	
	PlanePolygon(ID3D12Device* device);
	
	StaticCharacter planePolygon;
	
	std::unique_ptr<DirectX::EffectFactory> m_planepolyeffect;
	Transform m_transform;
	DirectX::SimpleMath::Matrix m_planepolyworld;
	DirectX::SimpleMath::Matrix m_planepolyview;
	DirectX::SimpleMath::Matrix m_planepolyproj;
	std::array<std::shared_ptr<btBvhTriangleMeshShape>, 2> planepolycollision;
	DirectX::Model::EffectCollection m_planepolyNormal;
	
};

