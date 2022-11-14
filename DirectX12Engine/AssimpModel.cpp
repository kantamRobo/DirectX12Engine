#include "AssimpModel.h"



using namespace Microsoft::WRL;
HRESULT AssimpModel::Init(DX::DeviceResources* deviceresources,DirectX::GraphicsMemory* graphicsmemory)
{
	auto device = deviceresources->GetD3DDevice();
	auto queue = deviceresources->GetCommandQueue();
	//頂点・インデックスバッファの作成
	{
		DirectX::ResourceUploadBatch upload(device);

		upload.Begin();
		m_vertexBuffer = graphicsmemory->Allocate(sizeof(DirectX::VertexPosition));
		m_indexBuffer = graphicsmemory->Allocate(sizeof(uint32_t));

		m_vertexBufferView.BufferLocation = m_vertexBuffer.GpuAddress();
		m_vertexBufferView.StrideInBytes = sizeof(DirectX::VertexPosition);
		m_vertexBufferView.SizeInBytes = static_cast<UINT>(m_vertexBuffer.Size());

		m_indexBufferView.BufferLocation = m_indexBuffer.GpuAddress();
		m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		m_indexBufferView.SizeInBytes = static_cast<UINT>(m_indexBuffer.Size());

		//フレームバッファ*2（二体のモデルを出す場合）分の定数バッファとビューを作成する
		for (int i = 0; i < 4; i++)
		{
			m_CBDescriptorHeaps[i] = std::make_shared<DirectX::DescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			m_transformCBBuffer[i] = graphicsmemory->AllocateConstant(sizeof(TransformConstantBufferData));


		}
		//ルートシグネチャ・パイプラインステートの作成
		{
			vertexshader.LoadShader(L"Modelassimpvertex.hlsl", L"main_VS");
			pixelshader.LoadShader(L"Modelassimppixel.hlsl", L"main_PS");
			//定数バッファ用のみを作成する
			CD3DX12_DESCRIPTOR_RANGE range;
			CD3DX12_ROOT_PARAMETER parameter;

			range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			parameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | //定数バッファーにアクセスする必要があるのは、入力アセンブラー ステージだけです。
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

			CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
			descRootSignature.Init(1, &parameter, 0, nullptr, rootSignatureFlags);

			ComPtr<ID3DBlob> pSignature;
			ComPtr<ID3DBlob> pError;
			auto serialize =D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pError.GetAddressOf());
			auto CreateRootsig = device->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
			NAME_D3D12_OBJECT(m_rootSignature);

			
				static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				};

				D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
				state.InputLayout = { inputLayout, _countof(inputLayout) };
				state.pRootSignature = m_rootSignature.Get();
				state.VS = CD3DX12_SHADER_BYTECODE(&m_vertexShader[0], m_vertexShader.size());
				state.PS = CD3DX12_SHADER_BYTECODE(&m_pixelShader[0], m_pixelShader.size());
				state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
				state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
				state.SampleMask = UINT_MAX;
				state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				state.NumRenderTargets = 1;
				state.RTVFormats[0] = deviceresources->GetBackBufferFormat();
				state.DSVFormat = deviceresources->GetDepthBufferFormat();
				state.SampleDesc.Count = 1;

				auto createpipelinestate =device->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&m_pipelineState));

				// シェーダー データは、パイプライン状態が作成された後に削除できます。
				m_vertexShader.clear();
				m_pixelShader.clear();
		}

		// Upload the resources to the GPU.
		auto finish = upload.End(queue);

		// Wait for the upload thread to terminate
		finish.wait();

		deviceresources->WaitForGpu();
	}
}

void AssimpModel::Render(DX::DeviceResources* deviceresources, DirectX::GraphicsMemory* graphicsmemory)
{
	auto commandlist = deviceresources->GetCommandList();
	auto frameindex = deviceresources->GetCurrentFrameIndex();
	//一体目の描画

	commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandlist->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	commandlist->IASetIndexBuffer(&m_indexBufferView);
	commandlist->SetComputeRootConstantBufferView(0,m_transformCBBuffer[frameindex * 2 + 0].GpuAddress());
	commandlist->DrawIndexedInstanced(m_indexBuffer.Size(),1,0,0,0);
	//二体目の描画
	commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandlist->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	commandlist->IASetIndexBuffer(&m_indexBufferView);
	commandlist->SetComputeRootConstantBufferView(0,m_transformCBBuffer[frameindex * 2 + 1].GpuAddress());
	commandlist->DrawIndexedInstanced(m_indexBuffer.Size(), 1, 0, 0, 0);

}
