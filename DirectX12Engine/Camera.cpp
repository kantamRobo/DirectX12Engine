/*!
* @brief	カメラ
*/

#include "Camera.h"

#include "DX12EngineCore.h"
Camera::Camera(const std::shared_ptr<DX12EngineCore> in_core, const DirectX::XMMATRIX& modelMat, ShaderParameters* ModelrendererShaderparam)
{
	m_viewport = in_core->m_viewport;
	m_rect = in_core->m_scissorRect;
	m_position = DirectX::XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f);
	m_target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	SetViewProj(modelMat, ModelrendererShaderparam);
}

//モデルレンダラーで呼ぶ
void Camera::Update(const DirectX::XMMATRIX& modelMat, ShaderParameters* ModelrendererShaderparam)
{
	
	SetViewProj(modelMat, ModelrendererShaderparam);
}


void Camera::SetViewProj(const DirectX::XMMATRIX& ModelMat,ShaderParameters* ModelrendererShaderparam)
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
	
	DirectX::XMStoreFloat4x4(&ModelrendererShaderparam->mtxWorld, ModelMat);
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
	DirectX::XMStoreFloat4x4(&ModelrendererShaderparam->mtxView, DirectX::XMMatrixTranspose(mtxView));
	DirectX::XMStoreFloat4x4(&ModelrendererShaderparam->mtxProj, DirectX::XMMatrixTranspose(mtxProj));


}

void Camera::SetPosition(const DirectX::XMVECTOR& in_posvector)
{
	m_position = DirectX::XMVectorSet(in_posvector.m128_f32[0], in_posvector.m128_f32[1], in_posvector.m128_f32[2],0.0f);
}

void Camera::SetRotation(const DirectX::XMVECTOR& in_rotvector)
{
	m_rotation = DirectX::XMVectorSet(in_rotvector.m128_f32[0], in_rotvector.m128_f32[1], in_rotvector.m128_f32[2], 1.0f);
		
}

