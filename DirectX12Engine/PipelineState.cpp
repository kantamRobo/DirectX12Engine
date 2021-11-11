#include "PipelineState.h"
#include <stddef.h>
#include <stdlib.h>
#include "Vertex.h"
void PipelineState::CreatePipeLineState(ID3DBlob* pixelShader,ID3DBlob* vertexshader,ID3D12Device* device,ID3D12RootSignature* rootSignature) {
	// インプットレイアウト
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, offsetof(Vertex,Color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,UV), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA}
	};

	// パイプラインステートオブジェクトの生成.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	// シェーダーのセット
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexshader);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
	// ブレンドステート設定
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// ラスタライザーステート
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// 出力先は1ターゲット
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// デプスバッファのフォーマットを設定
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };

	// ルートシグネチャのセット
	psoDesc.pRootSignature = rootSignature;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// マルチサンプル設定
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // これを忘れると絵が出ない＆警告も出ないので注意.

	auto hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_graphicpipelinestate));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateGraphicsPipelineState failed");
	}

}