#include "ModelRenderer.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "PipelineState.h"
#include <DirectXMath.h>
#include "Resourceworker.h"





D3D12_COMMAND_QUEUE_DESC queueDesc{
  D3D12_COMMAND_LIST_TYPE_DIRECT,
  0,
  D3D12_COMMAND_QUEUE_FLAG_NONE,
  0
};


void ModelRenderer::CreateSceneView(Microsoft::WRL::ComPtr<ID3D12Device> p_device, const DescriptorHeapsContainer& SceneCBVheap)
{


	// 定数バッファ/定数バッファビューの生成
	m_SceneconstantBuffers.resize(FrameBufferCount);
	m_cbSceneViews.resize(FrameBufferCount);


	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
		m_SceneconstantBuffers[i] = CreateBuffer(p_device.Get(), D3D12_RESOURCE_DIMENSION_BUFFER,
			bufferSize, sizeAligned, 1, DXGI_FORMAT_UNKNOWN, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
		cbDesc.BufferLocation = m_SceneconstantBuffers[i]->GetGPUVirtualAddress();
		cbDesc.SizeInBytes = bufferSize;

		CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(SceneCBVheap.m_heapSceneCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_cbvDescriptorSize);
		
		p_device->CreateConstantBufferView(&cbDesc, handleCBV);

	m_cbSceneViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(SceneCBVheap.m_heapSceneCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_cbvDescriptorSize);
	}

}

void ModelRenderer::CreateMaterialView(Microsoft::WRL::ComPtr<ID3D12Device> p_device, const DescriptorHeapsContainer& MaterialCBVheap)
{
	UINT bufferSize = sizeof(Material) + 255 & ~255;
	m_materialBuffers = CreateBuffer(p_device.Get(), D3D12_RESOURCE_DIMENSION_BUFFER,
		bufferSize,sizeAligned,1, DXGI_FORMAT_UNKNOWN,D3D12_TEXTURE_LAYOUT_ROW_MAJOR,&material);
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
	cbDesc.BufferLocation = m_materialBuffers->GetGPUVirtualAddress();
	cbDesc.SizeInBytes = bufferSize;

	D3D12_CPU_DESCRIPTOR_HANDLE handlematCBV = MaterialCBVheap.m_heapMaterialCbv->GetCPUDescriptorHandleForHeapStart();
	
	p_device->CreateConstantBufferView(&cbDesc, handlematCBV);
}





ModelRenderer::ModelRenderer(const std::shared_ptr<DX12EngineCore> core, const Commands& commands,
	std::shared_ptr<Model> in_model,
	const DescriptorHeapsContainer& descheaps) {


	


	
	m_core = core;
	m_model = in_model;
	CreateSceneView(m_core->m_device.Get(), descheaps);

	m_frameIndex = m_core->m_swapchain->GetCurrentBackBufferIndex();
	m_heapSrv = descheaps.m_heapSrv;
	m_heapSceneCbv = descheaps.m_heapSceneCbv;
	m_heapmatCbv = descheaps.m_heapMaterialCbv;
	m_heapRTV = descheaps.m_heapRtv;
	m_heapDSV = descheaps.m_HeapDsv;
	m_heapSampler = descheaps.m_HeapSampler;
	m_commands = commands;
	m_swapchain = m_core->m_swapchain.Get();
	m_viewport = m_core->m_viewport;
	m_scissorRect = m_core->m_scissorRect;
	m_frameFences = m_core->m_frameFences;
	m_frameFenceValues = m_core->m_frameFenceValues;
	

}




void ModelRenderer::Render(std::shared_ptr<Camera>
	 camera
)
{
	m_frameIndex = m_core->m_swapchain->GetCurrentBackBufferIndex();

	m_commands.allocators[m_frameIndex]->Reset();
	m_commands.list->Reset(
		m_commands.allocators[m_frameIndex].Get(),
		nullptr
	);

	// スワップチェイン表示可能からレンダーターゲット描画可能へ
	auto barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commands.list->ResourceBarrier(1, &barrierToRT);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
		m_heapRTV->GetCPUDescriptorHandleForHeapStart(),
		m_frameIndex, rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(
	m_heapDSV->GetCPUDescriptorHandleForHeapStart()
	);

	// カラーバッファ(レンダーターゲットビュー)のクリア
	const float clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // クリア色
	m_commands.list->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

	// デプスバッファ(デプスステンシルビュー)のクリア
	m_commands.list->ClearDepthStencilView(
		dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 描画先をセット
	m_commands.list->OMSetRenderTargets(1, &rtv, FALSE, &dsv);

	MakeCommand(m_commands.list,m_SceneconstantBuffers,camera);

	// レンダーターゲットからスワップチェイン表示可能へ
	auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	m_commands.list->ResourceBarrier(1, &barrierToPresent);

	m_commands.list->Close();

	ID3D12CommandList* lists[] = { m_commands.list.Get() };
	m_commands.queue->ExecuteCommandLists(1, lists);

	m_core->m_swapchain->Present(1, 0);

	WaitPreviousFrame();
}


void ModelRenderer::WaitPreviousFrame()
{
	// 現在のフェンスに GPU が到達後設定される値をセット.
	auto& fence = m_frameFences[m_frameIndex];
	const auto currentValue = ++m_frameFenceValues[m_frameIndex];
	m_commands.queue->Signal(fence.Get(), currentValue);

	// 次処理するコマンド（アロケータ－）のものは実行完了済みかを、
	// 対になっているフェンスで確認する.
	auto nextIndex = (m_frameIndex + 1) % FrameBufferCount;
	const auto finishExpected = m_frameFenceValues[nextIndex];
	const auto nextFenceValue = m_frameFences[nextIndex]->GetCompletedValue();
	if (nextFenceValue < finishExpected)
	{
		// GPU が処理中のため、イベントで待機する
		m_frameFences[nextIndex]->SetEventOnCompletion(finishExpected, m_fenceWaitEvent);
		WaitForSingleObject(m_fenceWaitEvent, GpuWaitTimeout);
	}
}


void ModelRenderer::MakeCommand(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& command, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_constantBuffers,std::shared_ptr<Camera> camera)
{
	
	/*
	// 各行列のセット.
	ShaderParameters shaderParams;
	XMStoreFloat4x4(&shaderParams.mtxWorld, XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(45.0f)));
	auto mtxView = XMMatrixLookAtLH(
		XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	auto mtxProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_viewport.Width / m_viewport.Height, 0.1f, 100.0f);
	XMStoreFloat4x4(&shaderParams.mtxView, XMMatrixTranspose(mtxView));
	XMStoreFloat4x4(&shaderParams.mtxProj, XMMatrixTranspose(mtxProj));
	*/
	DirectX::XMStoreFloat4x4(&m_model->modelmat, DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f),
		DirectX::XMMatrixRotationAxis(
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
			DirectX::XMConvertToRadians(45.0f))), DirectX::XMMatrixTranslation(m_model->m_position.x, m_model->m_position.y, m_model->m_position.z)));

	camera->Update(DirectX::XMLoadFloat4x4(&m_model->modelmat),&shaderParams);
	// 定数バッファの更新.
	auto& constantBuffer = m_constantBuffers[m_frameIndex];
	{
		void* p;
		CD3DX12_RANGE range(0, 0);
		constantBuffer->Map(0, &range, &p);
		//memcpy(p, &shaderParams, sizeof(shaderParams));
		memcpy(p, &shaderParams, sizeof(shaderParams));
		constantBuffer->Unmap(0, nullptr);
	}
	//↑CopyToVRAM
	// パイプラインステートのセット
	m_commands.list->SetPipelineState(m_model->m_modelPSO->m_graphicpipelinestate.Get());
	// ルートシグネチャのセット
	m_commands.list->SetGraphicsRootSignature(m_model->m_rootSignature.Get());
	// ビューポートとシザーのセット
	m_commands.list->RSSetViewports(1, &m_viewport);
	m_commands.list->RSSetScissorRects(1, &m_scissorRect);

	// ディスクリプタヒープをセット.
	ID3D12DescriptorHeap* heaps[] = {
	 m_heapSrv.Get(),m_heapSceneCbv.Get() ,m_heapSampler.Get(),m_heapmatCbv.Get()
	};
	m_commands.list->SetDescriptorHeaps(_countof(heaps), heaps);

	// プリミティブタイプ、頂点・インデックスバッファのセット
	m_commands.list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
	m_commands.list->IASetVertexBuffers(0, 1, &m_model->m_vertexBufferView);
	m_commands.list->IASetIndexBuffer(&m_model->m_indexBufferView);

	m_commands.list->SetGraphicsRootDescriptorTable(0, m_heapSrv->GetGPUDescriptorHandleForHeapStart());
	m_commands.list->SetGraphicsRootDescriptorTable(0, m_heapSceneCbv->GetGPUDescriptorHandleForHeapStart());
	m_commands.list->SetGraphicsRootDescriptorTable(0, m_heapmatCbv->GetGPUDescriptorHandleForHeapStart());
	//m_commands.list->SetGraphicsRootDescriptorTable(1, m_model->m_srv);
	m_commands.list->SetGraphicsRootDescriptorTable(2, m_heapSampler->GetGPUDescriptorHandleForHeapStart());

	// 描画命令の発行
	m_commands.list->DrawIndexedInstanced(m_model->m_indexCount, 1, 0, 0, 0);
}
