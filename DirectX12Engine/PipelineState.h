#pragma once
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <wrl.h>
class PipelineState
{

public:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_graphicpipelinestate;
	void CreatePipeLineState(ID3DBlob* pixelShader, ID3DBlob* vertexshader, ID3D12Device* device, ID3D12RootSignature* rootSignature);

};
