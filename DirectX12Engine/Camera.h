#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include <d3dx12.h>
#include "CubeApp.h"
class Camera
{
public:
	Camera() {};
	Camera(CD3DX12_VIEWPORT& in_viewport, CD3DX12_RECT& in_scissorRect);
	
	CubeApp::ShaderParameters shaderParams;
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_rect;
	void Update();
	void SetViewProj();
	void SetPosition(const DirectX::XMVECTOR& in_posvector);
	void SetRotation(const DirectX::XMVECTOR& in_rotvector);
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_rotation;
	DirectX::XMVECTOR m_target;
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//ÉJÉÅÉâÇìÆÇ©Ç∑
	float x = 0;
	float y = 0;
	float z = 0;
	void Move(const DirectX::XMVECTOR& move)
	{
		m_position = DirectX::XMVectorAdd(m_position, move);
		m_target = DirectX::XMVectorAdd(m_target, move);

	}
	//íçéãì_ÇìÆÇ©Ç∑
	void MoveTarget(const DirectX::XMVECTOR& move)
	{
		m_target = DirectX::XMVectorAdd(m_target, move);

	}
	//éãì_ÇìÆÇ©Ç∑
	void MovePosition(const DirectX::XMVECTOR& move)
	{
		m_position = DirectX::XMVectorAdd(m_position, move);

	}

};

