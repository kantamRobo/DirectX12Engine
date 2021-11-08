/*!
* @brief	カメラ
*/

#include "Camera.h"


Camera::Camera(CD3DX12_VIEWPORT&  in_viewport, CD3DX12_RECT& in_scissorRect, const DirectX::XMMATRIX& modelMat)
{
	m_viewport = in_viewport;
	m_rect = in_scissorRect;
	m_position = DirectX::XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f);
	m_target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	SetViewProj(modelMat);
}


void Camera::Update(const DirectX::XMMATRIX& modelMat)
{
	
	SetViewProj(modelMat);
}


void Camera::SetViewProj(const DirectX::XMMATRIX& ModelMat)
{
	
	/*x += 0.01f;
	if (x <= 0)
	{
		x += 0.01f;
	}
	y += 0.001f;
	*/
	// 各行列のセット.
	//m_position=DirectX::XMVectorSet(x,y,0,0);
	//m_rotation = DirectX::XMVectorSet(x, 0, 0, 0);
	
	//DirectX::XMStoreFloat4x4(&shaderParams.mtxWorld, DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMConvertToRadians(45.0f)));
	
	DirectX::XMStoreFloat4x4(&shaderParams.mtxWorld, ModelMat);
	/*
	auto mtxView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f),//position
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),//target
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)//up
	);
	*/

	
	auto mtxView = DirectX::XMMatrixLookAtLH(
		m_position,//position
		m_target,//target
		up//up
	);
	
	auto mtxProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), m_viewport.Width / m_viewport.Height, 0.1f, 100.0f);
	DirectX::XMStoreFloat4x4(&shaderParams.mtxView, DirectX::XMMatrixTranspose(mtxView));
	DirectX::XMStoreFloat4x4(&shaderParams.mtxProj, DirectX::XMMatrixTranspose(mtxProj));


}

void Camera::SetPosition(const DirectX::XMVECTOR& in_posvector)
{
	m_position = DirectX::XMVectorSet(in_posvector.m128_f32[0], in_posvector.m128_f32[1], in_posvector.m128_f32[2],0.0f);
}

void Camera::SetRotation(const DirectX::XMVECTOR& in_rotvector)
{
	m_rotation = DirectX::XMVectorSet(in_rotvector.m128_f32[0], in_rotvector.m128_f32[1], in_rotvector.m128_f32[2], 1.0f);
		
}

