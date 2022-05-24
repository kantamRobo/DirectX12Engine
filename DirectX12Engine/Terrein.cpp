
#include "Terrein.h"
#include <cmath>
#include <d3dcompiler.h>
#include <algorithm>
#include "Utility.h"
using namespace std;
enum Descriptors
{
	WindowsLogo,
	CourierFont,
	ControllerFont,
	GamerPic,
	Count
};

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
	const std::shared_ptr<DX::DeviceResources> devicesresources,
	std::shared_ptr<DirectX::GraphicsMemory> graphicsMemory)
{
	DirectX::ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, resourceUpload, L"normalmap.png",
			m_normaltexture.ReleaseAndGetAddressOf()));

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, resourceUpload, L"heightmap.png",
			m_heighttexture.ReleaseAndGetAddressOf()));
	m_heightmapheap = std::make_unique<DirectX::DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Descriptors::Count);
	m_normalmapheap = std::make_unique<DirectX::DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Descriptors::Count);
	auto heightcpuhandle = m_heightmapheap->GetFirstCpuHandle();
	auto normalcpuhandle = m_normalmapheap->GetFirstCpuHandle();
	DirectX::CreateShaderResourceView(device, m_heighttexture.Get()
		, heightcpuhandle);
	DirectX::CreateShaderResourceView(device, m_normaltexture.Get()
		, normalcpuhandle);
	

	
	
	
	const int divide = 10;
	const float edge = 200.0f;
	for (int z = 0; z < divide + 1; ++z)
	{
		for (int x = 0; x < divide + 1; ++x)
		{
			DirectX::VertexPositionTexture v;
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
			m_indices.push_back(v0 + rows);
			m_indices.push_back(v1 + rows);
			m_indices.push_back(v0);
			m_indices.push_back(v1);
		}
		
	}
	


	//ここに頂点バッファとインデックスバッファのリソース作成処理を入れる

	

	vertexBuffer =  graphicsMemory->Allocate(sizeof(DirectX::VertexPositionTexture));
	memcpy(vertexBuffer.Memory(), m_vertices.data(), sizeof(DirectX::VertexPositionTexture));

	indexBuffer = graphicsMemory->Allocate(sizeof(UINT));
	memcpy(indexBuffer.Memory(), m_indices.data(), sizeof(UINT));

	auto vertexdesc = CD3DX12_RESOURCE_DESC(
		D3D12_RESOURCE_DIMENSION_BUFFER,
		D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
		sizeof(m_vertices), 1, 1, 1,
		DXGI_FORMAT_UNKNOWN,
		1, 0,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE);

	CD3DX12_HEAP_PROPERTIES vertexdefaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	//ここでエラーが出る可能性がある。そしたらUPLOADに変える。
	DX::ThrowIfFailed(device->CreateCommittedResource(
		&vertexdefaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexdesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_GRAPHICS_PPV_ARGS(m_staticpatchvertexbuffer.ReleaseAndGetAddressOf())));

	auto indexdesc = CD3DX12_RESOURCE_DESC(
		D3D12_RESOURCE_DIMENSION_BUFFER,
		D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
		sizeof(m_indices), 1, 1, 1,
		DXGI_FORMAT_UNKNOWN,
		1, 0,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE);

	CD3DX12_HEAP_PROPERTIES indexdefaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	//ここでエラーが出る可能性がある。そしたらUPLOADに変える。
	DX::ThrowIfFailed(device->CreateCommittedResource(
		&indexdefaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&indexdesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_GRAPHICS_PPV_ARGS(m_staticpatchindexbuffer.ReleaseAndGetAddressOf())));
	

	DirectX::ResourceUploadBatch upload(device);
	upload.Begin();
	upload.Upload(m_staticpatchvertexbuffer.Get(), vertexBuffer);

	upload.Transition(m_staticpatchvertexbuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	
	upload.Upload(m_staticpatchindexbuffer.Get(), indexBuffer);

	
	
	upload.Transition(m_staticpatchindexbuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);

	

	

	D3D12_INPUT_ELEMENT_DESC desc[2] = {};
	desc[0].AlignedByteOffset = 0;
	desc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc[0].InputSlot = 0;
	desc[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	desc[0].SemanticIndex = 0;
	desc[0].SemanticName = "POSITION";

	desc[1].AlignedByteOffset = 0;
	desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	//UVのテクスチャ座標、そのフォーマットの不一致には注意。
	desc[1].InputSlot = 0;
	desc[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	desc[1].SemanticIndex = 0;
	desc[1].SemanticName = "TEXCOORD0";
	

	Microsoft::WRL::ComPtr<ID3DBlob> vertexshader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexshadererror = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pixelshader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pixelshadererror = nullptr;

	auto vertexresult = m_handler->CompileShaderFromFile(L"VSterrein.hlsl", L"vs_6_0", vertexshader, vertexshadererror);
	auto pixelresult = m_handler->CompileShaderFromFile(L"PSterrein.hlsl", L"ps_6_0", pixelshader, pixelshadererror);
	

	std::string errstrvertex;
	errstrvertex.resize(vertexshadererror->GetBufferSize());


	std::memcpy(errstrvertex.data(), vertexshadererror->GetBufferPointer(), vertexshadererror->GetBufferSize());

	OutputDebugStringA(errstrvertex.c_str());
	std::string errstrpixel;
	errstrpixel.resize(vertexshadererror->GetBufferSize());


	std::memcpy(errstrpixel.data(), pixelshadererror->GetBufferPointer(), pixelshadererror->GetBufferSize());

	OutputDebugStringA(errstrpixel.c_str());

	
	
	Microsoft::WRL::ComPtr<ID3DBlob> hullshader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> hullshadererror = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> domainshader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> domainshadererror = nullptr;
	
	
	auto hullresult = m_handler->CompileShaderFromFile(L"HSterrein.hlsl", L"hs_6_0", hullshader, hullshadererror);

	auto domainresult = m_handler->CompileShaderFromFile(L"DSterrein.hlsl", L"DSterrein.hlsl", domainshader, domainshadererror);
	
	
	
	std::string errstrhull;
	errstrhull.resize(domainshadererror->GetBufferSize());

	
	std::memcpy(errstrhull.data(), hullshadererror->GetBufferPointer(), hullshadererror->GetBufferSize());
	

	OutputDebugStringA(errstrhull.c_str());

	std::string errstrdomain;
	errstrdomain.resize(domainshadererror->GetBufferSize());
	std::memcpy(errstrdomain.data(), domainshadererror->GetBufferPointer(), domainshadererror->GetBufferSize());



	OutputDebugStringA(errstrdomain.c_str());

	
	CD3DX12_SHADER_BYTECODE patchDS(domainshader.Get());
	CD3DX12_SHADER_BYTECODE patchHS(hullshader.Get());


	CD3DX12_SHADER_BYTECODE patchVS(vertexshader.Get());
	CD3DX12_SHADER_BYTECODE patchPS(pixelshader.Get());


	D3D12_INPUT_LAYOUT_DESC input = {};
	input.NumElements = _countof(desc);
	input.pInputElementDescs = desc;
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
	terreinpipeline = pipeline;
	terreinpipeline.CreatePipelineState(device,m_patchrootsignature.Get()
		, patchVS,patchPS,patchDS,patchHS,m_patchpipelinestate.GetAddressOf());

	
}


void Terrein::DrawTerrein(ID3D12GraphicsCommandList* command,const Camera in_camera,
	std::shared_ptr<DirectX::BasicEffect> in_effect)
{
	
	in_effect->SetWorld(world);
	in_effect->SetView(in_camera.m_view);
	in_effect->SetProjection(in_camera.m_proj);
	
	D3D12_VERTEX_BUFFER_VIEW vbv={};
	vbv.BufferLocation = m_staticpatchvertexbuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(DirectX::VertexPositionTexture);
	vbv.SizeInBytes = static_cast<UINT>(sizeof(m_staticpatchvertexbuffer));
	
	
	D3D12_INDEX_BUFFER_VIEW ibv={};

	ibv.BufferLocation = m_staticpatchindexbuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = static_cast<UINT>(sizeof(m_staticpatchindexbuffer));

	ID3D12DescriptorHeap* heaps[] = { m_heightmapheap->Heap(),m_normalmapheap->Heap() };

	command->SetDescriptorHeaps(2,heaps);
	in_effect->Apply_Any_RS_PSO(command, m_patchrootsignature.Get(), m_patchpipelinestate.Get());
	command->IASetVertexBuffers(0, 1, &vbv);
	command->IASetIndexBuffer(&ibv);
	command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	command->DrawIndexedInstanced(m_indices.size(),1,0,0,0);
}