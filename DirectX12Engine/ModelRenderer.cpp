#include "ModelRenderer.h"

//コアクラスでスワップチェーン、レンダーターゲットビュー、デプスステンシルビュー,フェンス
// ワーカークラスでパイプラインオブジェクト（シェーダー、ルートシグネチャ、PSO）を作り、
//作成したこれらを、エンジンの初期化クラスでこのクラスのコンストラクタに渡して初期化する)
ModelRenderer::ModelRenderer(IDXGISwapChain3* swapchain,
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>renderTargets,
	const UINT& rtvDescriptorSize,
	const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences,
	const DescriptorHeapsContainer& DSV_RTV,
	const Commands& commands,
	const GraphicPipeLineObjectContainer& pipelineobjContainer,
	std::vector<UINT64> m_frameFenceValues)
{

}



void ModelRenderer::Render(
)
{
	m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();

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
		m_DSV_RTV.heapRtv->GetCPUDescriptorHandleForHeapStart(),
		m_frameIndex, rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(
		m_DSV_RTV.heapDsv->GetCPUDescriptorHandleForHeapStart()
	);

	// カラーバッファ(レンダーターゲットビュー)のクリア
	const float clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // クリア色
	m_commands.list->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

	// デプスバッファ(デプスステンシルビュー)のクリア
	m_commands.list->ClearDepthStencilView(
		dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 描画先をセット
	m_commands.list->OMSetRenderTargets(1, &rtv, FALSE, &dsv);

	MakeCommand(m_commands.list.Get(),constantBuffers);

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

	m_swapchain->Present(1, 0);

	WaitPreviousFrame(m_commands.queue.Get(),m_frameFences,m_frameFenceValues);
}


void ModelRenderer::WaitPreviousFrame(ID3D12CommandQueue* queue,const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences, std::vector<UINT64> m_frameFenceValues)
{
	// 現在のフェンスに GPU が到達後設定される値をセット.
	auto& fence = m_frameFences[m_frameIndex];
	const auto currentValue = ++m_frameFenceValues[m_frameIndex];
	queue->Signal(fence.Get(), currentValue);

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


void ModelRenderer::MakeCommand(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& command,Camera* camera)
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
	camera->Update();
	// 定数バッファの更新.
	auto& constantBuffer = constantBuffers[m_frameIndex];
	{
		void* p;
		CD3DX12_RANGE range(0, 0);
		constantBuffer->Map(0, &range, &p);
		//memcpy(p, &shaderParams, sizeof(shaderParams));
		memcpy(p, &camera->shaderParams, sizeof(camera->shaderParams));
		constantBuffer->Unmap(0, nullptr);
	}
	//↑CopyToVRAM
	// パイプラインステートのセット
	command->SetPipelineState(pipelineobjContainer.m_pipeline.Get());
	// ルートシグネチャのセット
	command->SetGraphicsRootSignature(pipelineobjContainer.m_rootSignature.Get());
	// ビューポートとシザーのセット
	command->RSSetViewports(1, &m_viewport);
	command->RSSetScissorRects(1, &m_scissorRect);

	// ディスクリプタヒープをセット.
	ID3D12DescriptorHeap* heaps[] = {
	  m_heapSrvCbv.Get(), m_heapSampler.Get()
	};
	command->SetDescriptorHeaps(_countof(heaps), heaps);

	// プリミティブタイプ、頂点・インデックスバッファのセット
	command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
	command->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	command->IASetIndexBuffer(&m_indexBufferView);

	command->SetGraphicsRootDescriptorTable(0, m_cbViews[m_frameIndex]);
	command->SetGraphicsRootDescriptorTable(1, m_srv);
	command->SetGraphicsRootDescriptorTable(2, m_sampler);

	// 描画命令の発行
	command->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}