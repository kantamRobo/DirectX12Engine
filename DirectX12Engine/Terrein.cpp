
#include "Terrein.h"
#include <cmath>
#include <d3dcompiler.h>

using namespace std;
enum Descriptors
{
	WindowsLogo,
	CourierFont,
	ControllerFont,
	GamerPic,
	Count
};
Terrein::Terrein(ID3D12Device* device)
{
	//m_terreineditor = std::make_unique<TerreinEditor>();

	m_effect = std::make_unique<DirectX::BasicEffect>(device);

  
}
void Terrein::Preparegrayscale()
{
	//
	/*
	if ("terreinheightmap.png" != nullptr)
	{
		heightとwidthをロード


	}*/
	/*
	else
	{
		heightmap.m_grayscale.resize(heightmap.width);
		for (int i = 0; i < heightmap.width; i++) {
			heightmap.m_grayscale[i].resize(heightmap.height);
		
		}
		const int Defaultgrayscalevalue = 0;
		for (int currentwidth = 0; currentwidth < heightmap.width
			; currentwidth++)
		{
			for (int currentheight = 0; currentheight < heightmap.height; heightmap.height)
			{
				heightmap.m_grayscale[currentwidth][currentheight] = 0;
			}

		}
	}
	
	*/


}

void Terrein::SetGrayScale()
{
	/*
	int x = 0;
	int y = 0;
	x = x +m_terreineditor->Nodes[0].x;
	y = y + m_terreineditor->Nodes[0].y;
	m_terreineditor->Nodes.reserve(m_terreineditor->nodenum);
	
	for (x < m_terreineditor->nodenum; x += m_terreineditor->offset;)
	{
			
			m_terreineditor->Nodes[x].x = x;
			m_terreineditor->Nodes[x].y = -m_terreineditor->slope * (x * x);

		
	}
	ImU32 whiteColor = IM_COL32(255, 255, 255, 255);
for (int i = 0; i < m_terreineditor->nodenum; i += m_terreineditor->offset)
	{
	
	if (i + 1 != m_terreineditor->nodenum)
		m_terreineditor->drawListR->AddLine(m_terreineditor->Nodes[i], m_terreineditor->Nodes[i + 1], whiteColor);
	else
	{
		break;
	}
}
*/

}


void Terrein::PrepareNormalMap(const grayscale* heightMap, Normalmap* normalMap, unsigned int width, unsigned int height)
{
	for (unsigned int y = 0; y < height; y++)
	{
		unsigned int ym1 = (y - 1) & (height - 1);
		unsigned int yp1 = (y + 1) & (height - 1);

		std::vector<float> centerRow = {};
		std::vector<float> upperRow = {};
		std::vector<float> lowerRow = {};
		centerRow.resize(width);
		upperRow.resize(width);
		lowerRow.resize(width);
           
		for (unsigned int x = 0; x < width; x++)
		{
			
			 centerRow[x]= heightMap->m_grayscale[x][y] + (y * width);
			upperRow[x] = heightMap->m_grayscale[x][y] + (ym1 * width);
			lowerRow[x]= heightMap->m_grayscale[x][y] + (yp1 * width);
			unsigned int xm1 = (x - 1) & (width - 1);
			unsigned int xp1 = (x + 1) & (width - 1);


			//Calculate slopes
			float dx = (centerRow[xp1] - centerRow[xm1] * 0.5f);
			float dy = (lowerRow[x] - upperRow[x]) * 0.5f;
		
		//Normalie and clamp.
			float nz = 1.0f / sqrt(dx * dx + dy * dy + 1.0f);
			float nx = fmin(fmax(-dx * nz, -1.0f), 1.0f);
			float ny = fmin(fmax(-dy * nz, -1.0f), 1.0f);
			normalMap[x].m_normalmap->x = nx;
			normalMap[x].m_normalmap->y = ny;
			normalMap[x].m_normalmap->z = nz;
			//
		}
		normalMap->m_normalmap->x += width;
		normalMap->m_normalmap->y += width;
		normalMap->m_normalmap->z += width;
		//各要素に右辺値を加算するoperator +=を追加する
	}
}

void Terrein::Preparepatch(ID3D12Device* device, DirectX::RenderTargetState targetstate, 
DX::DeviceResources* devicesresources)
{
	


	DirectX::ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();
	
	const int divide = 10;
	const float edge = 200.0f;
	for (int z = 0; z < divide + 1; ++z)
	{
		for (int x = 0; x < divide + 1; ++x)
		{
			DirectX::VertexPosition v;
			/*
			v.position = DirectX::XMFLOAT3(m_terreineditor->Nodes[x].x, m_terreineditor->Nodes[x].y,
				m_terreineditor->slope * (
					m_terreineditor->Nodes[x].x *
					m_terreineditor->Nodes[x].x +
					m_terreineditor->Nodes[x].y *
					m_terreineditor->Nodes[x].y));
					*/
			/*v.textureCoordinate = DirectX::SimpleMath::Vector2(
				v.position.x / edge,
				v.position.z / edge
			);*/

			

			m_vertices.push_back(v);
		}
	}
	
	for (int z = 0; z < divide; ++z) {
		for (int x = 0; x < divide; ++x) {
			const UINT rows = divide + 1;
			UINT v0 = x, v1 = x + 1;

			v0 = v0 + rows * z;
			v1 = v1 + rows * z;
			indices.push_back(v0 + rows);
			indices.push_back(v1 + rows);
			indices.push_back(v0);
			indices.push_back(v1);
		}
		
	}
	
	m_heightmapheap = std::make_unique<DirectX::DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Descriptors::Count);
	m_normalmapheap = std::make_unique<DirectX::DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Descriptors::Count);
	DirectX::SharedGraphicsResource hullvertex;
	DirectX::SharedGraphicsResource hullindex;
	resourceUpload.Upload(m_patchvertexbuffer.Get(), hullvertex);
	resourceUpload.Upload(m_patchindexbuffer.Get(), hullindex);
	resourceUpload.Transition(
		m_patchvertexbuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	resourceUpload.Transition(
		m_patchindexbuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_INDEX_BUFFER);
	auto uploadResourcesFinished = resourceUpload.End(devicesresources->GetCommandQueue());

	uploadResourcesFinished.wait();

	


	Microsoft::WRL::ComPtr<ID3DBlob> hullshader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> hullshadererror = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> domainshader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> domainshadererror = nullptr;
	auto hullresult = D3DCompileFromFile(L"HSterrein.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "HSMain", "vs_5_0", D3DCOMPILE_DEBUG, 0, &hullshader,
		&hullshadererror);
	auto domainresult = D3DCompileFromFile(L"HSterrein.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "HSMain", "vs_5_0", D3DCOMPILE_DEBUG, 0, &hullshader,
		&hullshadererror);
	CD3DX12_SHADER_BYTECODE patchHS(hullshader.Get());
	CD3DX12_SHADER_BYTECODE patchDS(domainshader.Get());


	D3D12_INPUT_ELEMENT_DESC desc = {};
	desc.AlignedByteOffset = 0;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.InputSlot = 0;
	desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	desc.SemanticIndex = 0;
	desc.SemanticName = "WORLDPOS";

	D3D12_INPUT_LAYOUT_DESC input = {};
	input.NumElements = 1;
	input.pInputElementDescs = &desc;
	D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc = {};
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3DBlob> rootsigblob = nullptr;
	
	D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootsigblob.GetAddressOf(), error.GetAddressOf());
	device->CreateRootSignature(0, rootsigblob->GetBufferPointer(),
		rootsigblob->GetBufferSize(),
		IID_PPV_ARGS(m_patchrootsignature.GetAddressOf()));
	
	

	TesselationEffectPipelineDescription pipeline(&input, DirectX::CommonStates::Opaque,
		DirectX::CommonStates::DepthDefault,
		DirectX::CommonStates::CullCounterClockwise,
		targetstate);
	pipeline.CreatePipelineState(device,m_patchrootsignature.Get()
		, patchDS,patchHS,m_patchpipelinestate.GetAddressOf());

	
}


void Terrein::DrawTerrein(ID3D12GraphicsCommandList* command,const Camera in_camera)
{
	
	m_effect->SetWorld(world);
	m_effect->SetView(in_camera.m_view);
	m_effect->SetProjection(in_camera.m_proj);
	m_effect->Apply(command);
	D3D12_VERTEX_BUFFER_VIEW vbv={};
	vbv.BufferLocation = m_patchvertexbuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(DirectX::VertexPositionNormalTexture);
	vbv.SizeInBytes = static_cast<UINT>(sizeof(m_patchvertexbuffer));
	command->IASetVertexBuffers(0, 1, &vbv);
	
	D3D12_INDEX_BUFFER_VIEW ibv={};

	ibv.BufferLocation = m_patchindexbuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = static_cast<UINT>(sizeof(m_patchindexbuffer));
	ID3D12DescriptorHeap* heaps[] = { m_heightmapheap->Heap(),m_normalmapheap->Heap() };

	command->SetDescriptorHeaps(2,heaps);
	command->SetGraphicsRootSignature(m_patchrootsignature.Get());
	command->SetPipelineState(m_patchpipelinestate.Get());
	command->IASetVertexBuffers(0, 1, &vbv);
	command->IASetIndexBuffer(&ibv);
	command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	command->DrawIndexedInstanced(indices.size(),1,0,0,0);
}