#include "AssimpModel.h"

HRESULT AssimpModel::Init(DX::DeviceResources* deviceresources,DirectX::GraphicsMemory* graphicsmemory)
{
	auto device = deviceresources->GetD3DDevice();
	auto queue = deviceresources->GetCommandQueue();
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
	commandlist->SetComputeRootConstantBufferView(0,m_transformCBBuffer[frameindex * 2 + 0].GpuAddress());
	commandlist->DrawIndexedInstanced(m_indexBuffer.Size(),1,0,0,0);
	//二体目の描画
	commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandlist->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	commandlist->SetComputeRootConstantBufferView(0,m_transformCBBuffer[frameindex * 2 + 1].GpuAddress());
	commandlist->DrawIndexedInstanced(m_indexBuffer.Size(), 1, 0, 0, 0);

}
