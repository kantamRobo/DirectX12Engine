
#include "Model.h"
#include <atlstr.h>
#include "MathUtility.h"
#include <stdexcept>
#include <coroutine>
#include <filesystem>
#include <iosfwd>
#include <dxcapi.h>
#include <DirectXTex/DirectXTex.h>
#include <DDSTextureLoader/DDSTextureLoader12.h>
#include <WICTextureLoader/WICTextureLoader12.h>
#include "DX12EngineCore.h"
#include "DescriptorHeap.h"
using namespace MathUtility;
using namespace  DirectX;

Model::Model(ID3D12Device* p_device, const Commands& in_commands, std::string pFile, const std::shared_ptr<DX12EngineCore> in_core,DescriptorHeap& CBV_SRVHeaps)
{

	m_frameIndex = in_core->m_swapchain->GetCurrentBackBufferIndex();

	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	m_pScene = importer.ReadFile(pFile,
		flag);

	//ÉÅÉbÉVÉÖÇÃÉÅÉÇÉäÇämï€
	meshes.clear();
	meshes.resize(m_pScene->mNumMeshes);

	//ÉÅÉbÉVÉÖÉfÅ[É^Çïœä∑
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = m_pScene->mMeshes[i];
		ProcessAssimpMesh(pMesh);
	}
	CreateVertexIndexBuffer(p_device);
	Prepare(p_device, in_commands, m_frameIndex,CBV_SRVHeaps);
}








void Model::ProcessAssimpMesh(const aiMesh* p_mesh)
{


	
         ConvertToaiMatrix4x4toXMFLOAT4x4(m_pScene->mRootNode->mTransformation, modelmat);

	
		 DirectX::XMMATRIX xmModelMat = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&modelmat));
		

		for (unsigned int v = 0; v < p_mesh->mNumVertices; v++)
		{
			Vertex vertex = {};
			vertex.Pos.x = p_mesh->mVertices[v].x;
			vertex.Pos.y = p_mesh->mVertices[v].y;
			vertex.Pos.z = p_mesh->mVertices[v].z;

			if (p_mesh->mTextureCoords[0])
			{
				vertex.UV.x = p_mesh->mTextureCoords[0][v].x;
				vertex.UV.y = p_mesh->mTextureCoords[0][v].y;
			}

			
			if (p_mesh->mNormals)
			{
				vertex.Normal.x = p_mesh->mNormals[v].x;
				vertex.Normal.y = p_mesh->mNormals[v].y;
				vertex.Normal.z = p_mesh->mNormals[v].z;
			}
			/*
			 if (srcMesh->mTangents)
			{
				dstTangent[0] = srcMesh->mTangents[v].x;
				dstTangent[1] = srcMesh->mTangents[v].y;
				dstTangent[2] = srcMesh->mTangents[v].z;
			}

			  if (srcMesh->mBitangents)
			{
				dstBitangent[0] = srcMesh->mBitangents[v].x;
				dstBitangent[1] = srcMesh->mBitangents[v].y;
				dstBitangent[2] = srcMesh->mBitangents[v].z;
			}
			*/

			
			vertices.push_back(vertex);
			unsigned int tempindex = 0;
			for (unsigned int f = 0; f < p_mesh->mNumFaces; f++)
			{
				assert(p_mesh->mFaces[f].mNumIndices == 3);

				aiFace face = p_mesh->mFaces[f];
				for (UINT j = 0; j < face.mNumIndices; j++) {
					indices.push_back(tempindex);

				}
			}




		}
		//Ç±ÇøÇÁÇÕÉ\Å[ÉXå≥ÅApaimeshÇÃëΩèdÉãÅ[ÉvÇ…Ç»Ç¡ÇƒÇ¢ÇÈÇ©Çí≤Ç◊ÇÈÅB
	/*
	UINT Nummesh = pScene->mNumMeshes;
	const aiMesh* submesh = nullptr;
	aiBone* pBone = nullptr;
	UINT nBoneIndex = 0;
	UINT NumBones = 0;
	for (UINT i = 0; i < Nummesh; i++)
	{
		submesh = pScene->mMeshes[i];

		for (UINT j = 0; j < submesh->mNumBones; j++)
		{
			nBoneIndex = 0;
			pBone = submesh->mBones[j];

			nBoneIndex = NumBones++;

			//XMMATRIX
			bones[NumBones].boneoffsetmatrix = DirectX::XMMatrixTranspose(pBone->mOffsetMatrix);
			//aiMatrix4x4Ç©ÇÁÅAXMMATRIXÇ÷ÇÃïœä∑ä÷êîÇçÏê¨ÇµÇƒÇ®Ç≠
		}

		for (UINT k = 0; k < pBone->mNumWeights; k++)
		{
			mesh.mBone.VertexID[k] = pBone->mWeights[k].mVertexId;
			mesh.mBone.Weights.weight[k] = pBone->mWeights[k].mWeight;
			
		}
	}
	*/
}

void Model::CreateVertexIndexBuffer(ID3D12Device* p_device) {
	// í∏ì_ÉoÉbÉtÉ@Ç∆ÉCÉìÉfÉbÉNÉXÉoÉbÉtÉ@ÇÃê∂ê¨.
	m_vertexBuffer = CreateBuffer(p_device,vertices.size(), vertices.data());
	m_indexBuffer = CreateBuffer(p_device,indices.size(), indices.data());
	m_indexCount = indices.size();

	// äeÉoÉbÉtÉ@ÇÃÉrÉÖÅ[Çê∂ê¨.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = vertices.size();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = indices.size();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}



void Model::Prepare(ID3D12Device* p_device,const Commands& in_commands,UINT in_FrameIndex, DescriptorHeap& SRV_CBV) {
	// ÉVÉFÅ[É_Å[ÇÉRÉìÉpÉCÉã.
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3DBlob> errBlob;
	hr = CompileShaderFromFile(L"simpleTexVS.hlsl", L"vs_6_0", m_vs, errBlob);
	if (FAILED(hr))
	{
		OutputDebugStringA((const char*)errBlob->GetBufferPointer());
	}
	hr = CompileShaderFromFile(L"simpleTexPS.hlsl", L"ps_6_0", m_ps, errBlob);
	if (FAILED(hr))
	{
		OutputDebugStringA((const char*)errBlob->GetBufferPointer());
	}

	CD3DX12_DESCRIPTOR_RANGE cbv, srv, sampler;
	cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0 ÉåÉWÉXÉ^
	srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ÉåÉWÉXÉ^
	sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0); // s0 ÉåÉWÉXÉ^

	CD3DX12_ROOT_PARAMETER rootParams[3];
	rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_VERTEX);
	rootParams[1].InitAsDescriptorTable(1, &srv, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParams[2].InitAsDescriptorTable(1, &sampler, D3D12_SHADER_VISIBILITY_PIXEL);

	// ÉãÅ[ÉgÉVÉOÉlÉ`ÉÉÇÃç\íz
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.Init(
		_countof(rootParams), rootParams,   //pParameters
		0, nullptr,   //pStaticSamplers
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
	if (FAILED(hr))
	{
		throw std::runtime_error("D3D12SerializeRootSignature faild.");
	}
	// RootSignature ÇÃê∂ê¨
	hr = p_device->CreateRootSignature(
		0,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature)
	);
	if (FAILED(hr))
	{
		throw std::runtime_error("CrateRootSignature failed.");
	}

	// ÉCÉìÉvÉbÉgÉåÉCÉAÉEÉg
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, offsetof(Vertex,Color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,UV), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA}
	};

	// ÉpÉCÉvÉâÉCÉìÉXÉeÅ[ÉgÉIÉuÉWÉFÉNÉgÇÃê∂ê¨.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	// ÉVÉFÅ[É_Å[ÇÃÉZÉbÉg
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.Get());
	// ÉuÉåÉìÉhÉXÉeÅ[Égê›íË
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// ÉâÉXÉ^ÉâÉCÉUÅ[ÉXÉeÅ[Ég
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// èoóÕêÊÇÕ1É^Å[ÉQÉbÉg
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// ÉfÉvÉXÉoÉbÉtÉ@ÇÃÉtÉHÅ[É}ÉbÉgÇê›íË
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };

	// ÉãÅ[ÉgÉVÉOÉlÉ`ÉÉÇÃÉZÉbÉg
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// É}ÉãÉ`ÉTÉìÉvÉãê›íË
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // Ç±ÇÍÇñYÇÍÇÈÇ∆äGÇ™èoÇ»Ç¢ÅïåxçêÇ‡èoÇ»Ç¢ÇÃÇ≈íçà”.

	hr = p_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateGraphicsPipelineState failed");
	}

	PrepareDescriptorHeapForCubeApp(p_device);
	
	// íËêîÉoÉbÉtÉ@/íËêîÉoÉbÉtÉ@ÉrÉÖÅ[ÇÃê∂ê¨
	m_constantBuffers.resize(FrameBufferCount);
	m_cbViews.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
		m_constantBuffers[i] = CreateBuffer(p_device,bufferSize, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
		cbDesc.BufferLocation = m_constantBuffers[i]->GetGPUVirtualAddress();
		cbDesc.SizeInBytes = bufferSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(SRV_CBV.m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
		p_device->CreateConstantBufferView(&cbDesc, handleCBV);

		m_cbViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(SRV_CBV.m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
	}

	// ÉeÉNÉXÉ`ÉÉÇÃê∂ê¨
	m_texture = CreateTexture(L"texture.tga",p_device,in_commands,m_frameIndex);

	// ÉTÉìÉvÉâÅ[ÇÃê∂ê¨
	D3D12_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D12_ENCODE_BASIC_FILTER(
		D3D12_FILTER_TYPE_LINEAR, // min
		D3D12_FILTER_TYPE_LINEAR, // mag
		D3D12_FILTER_TYPE_LINEAR, // mip
		D3D12_FILTER_REDUCTION_TYPE_STANDARD);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

	// ÉTÉìÉvÉâÅ[ópÉfÉBÉXÉNÉäÉvÉ^ÉqÅ[ÉvÇÃ0î‘ñ⁄ÇégópÇ∑ÇÈ
	auto descriptorSampler = CD3DX12_CPU_DESCRIPTOR_HANDLE(SRV_CBV.m_heapSampler->GetCPUDescriptorHandleForHeapStart(), SamplerDescriptorBase, SRV_CBV.m_samplerDescriptorSize);
	p_device->CreateSampler(&samplerDesc, descriptorSampler);
	SRV_CBV.m_sampler = CD3DX12_GPU_DESCRIPTOR_HANDLE(SRV_CBV.m_heapSampler->GetGPUDescriptorHandleForHeapStart(), SamplerDescriptorBase, SRV_CBV.m_samplerDescriptorSize);
	
	// ÉeÉNÉXÉ`ÉÉÇ©ÇÁÉVÉFÅ[É_Å[ÉäÉ\Å[ÉXÉrÉÖÅ[ÇÃèÄîı.
	auto srvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(SRV_CBV.m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), TextureSrvDescriptorBase, m_srvcbvDescriptorSize);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	p_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, srvHandle);
	SRV_CBV.m_srv = CD3DX12_GPU_DESCRIPTOR_HANDLE(SRV_CBV.m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), TextureSrvDescriptorBase, m_srvcbvDescriptorSize);

}

// éËìÆÇ≈ê∂ê¨î≈
Microsoft::WRL::ComPtr<ID3D12Resource1> Model::CreateTexture(const std::wstring& fileName,ID3D12Device* p_device,const Commands& in_commands,UINT in_frameIndex)
{
	TexMetadata  metadata = {};
	ScratchImage  scratchImg = {};

	
	LoadFromWICFile(
	fileName.c_str(), WIC_FLAGS_NONE,
		&metadata, scratchImg);

	auto img = scratchImg.GetImage(0, 0, 0);
	//ÉAÉbÉvÉçÅ[ÉhópÉäÉ\Å[ÉXÇÃçÏê¨
	D3D12_RESOURCE_DESC uploadHeap = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM, img->slicePitch, 1);
	D3D12_HEAP_PROPERTIES uploadHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	Microsoft::WRL::ComPtr<ID3D12Resource1> uploadbuff = nullptr;
	
	auto result = p_device->CreateCommittedResource(&uploadHeapProp
		, D3D12_HEAP_FLAG_NONE,
		&uploadHeap, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&uploadbuff));
	//ÉRÉsÅ[êÊÉäÉ\Å[ÉXÇÃçÏê¨
	D3D12_HEAP_PROPERTIES texheapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC texresdesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format,
		metadata.width, metadata.height, metadata.arraySize, metadata.mipLevels
		, static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension));
	Microsoft::WRL::ComPtr<ID3D12Resource1> texbuff = nullptr;

	result = p_device->CreateCommittedResource(&texheapprop, D3D12_HEAP_FLAG_NONE, &texresdesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texbuff));

	//ÉAÉbÉvÉçÅ[ÉhÉäÉ\Å[ÉXÇ÷É}ÉbÉv
	uint8_t* mapforImg = nullptr;
	result = uploadbuff->Map(0, nullptr, reinterpret_cast<void**>(&mapforImg));

	std::copy_n(img->pixels, img->slicePitch, mapforImg);
	//ÉRÉsÅ[
	uploadbuff->Unmap(0, nullptr);


	D3D12_TEXTURE_COPY_LOCATION  src = {};

	src.pResource = uploadbuff.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = metadata.width;
	src.PlacedFootprint.Footprint.Height =  metadata.height;
	src.PlacedFootprint.Footprint.Depth = metadata.depth;
	src.PlacedFootprint.Footprint.RowPitch = img->rowPitch+D3D12_TEXTURE_DATA_PITCH_ALIGNMENT- img->rowPitch%
		D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	src.PlacedFootprint.Footprint.Format = img->format;

	D3D12_TEXTURE_COPY_LOCATION dst = {};

	dst.pResource = texbuff.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;




	// ÉRÉ}ÉìÉhèÄîı.
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command;
	p_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, in_commands.allocators[in_frameIndex].Get(), nullptr, IID_PPV_ARGS(&command));
	Microsoft::WRL::ComPtr<ID3D12Fence1> fence;
	p_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	
	command->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

	// ÉRÉsÅ[å„Ç…ÇÕÉeÉNÉXÉ`ÉÉÇ∆ÇµÇƒÇÃÉXÉeÅ[ÉgÇ÷.
	auto barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		texbuff.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	command->ResourceBarrier(1, &barrierTex);

	command->Close();

	// ÉRÉ}ÉìÉhÇÃé¿çs
	ID3D12CommandList* cmds[] = { command.Get() };
	
		in_commands.queue->ExecuteCommandLists(1, cmds);
	// äÆóπÇµÇΩÇÁÉVÉOÉiÉãÇóßÇƒÇÈ.
	const UINT64 expected = 1;
	in_commands.queue->Signal(fence.Get(), expected);

	// ÉeÉNÉXÉ`ÉÉÇÃèàóùÇ™äÆóπÇ∑ÇÈÇ‹Ç≈ë“Ç¬.
	while (expected != fence->GetCompletedValue())
	{
		Sleep(1);
	}

	return texbuff;
}


HRESULT Model::CompileShaderFromFile(
	const std::wstring& fileName, const std::wstring& profile, Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob)
{
	using namespace std::filesystem;

	path filePath(fileName);
	std::ifstream infile(filePath);
	std::vector<char> srcData;
	if (!infile)
		throw std::runtime_error("shader not found");
	srcData.resize(uint32_t(infile.seekg(0, infile.end).tellg()));
	infile.seekg(0, infile.beg).read(srcData.data(), srcData.size());

	// DXC Ç…ÇÊÇÈÉRÉìÉpÉCÉãèàóù
	Microsoft::WRL::ComPtr<IDxcLibrary> library;
	Microsoft::WRL::ComPtr<IDxcCompiler> compiler;
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;
	Microsoft::WRL::ComPtr<IDxcOperationResult> dxcResult;

	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	library->CreateBlobWithEncodingFromPinned(srcData.data(), UINT(srcData.size()), CP_ACP, &source);
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

	LPCWSTR compilerFlags[] = {
  #if _DEBUG
	  L"/Zi", L"/O0",
  #else
	  L"/O2" // ÉäÉäÅ[ÉXÉrÉãÉhÇ≈ÇÕç≈ìKâª
  #endif
	};
	compiler->Compile(source.Get(), filePath.wstring().c_str(),
		L"main", profile.c_str(),
		compilerFlags, _countof(compilerFlags),
		nullptr, 0, // Defines
		nullptr,
		&dxcResult);

	HRESULT hr;
	dxcResult->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		dxcResult->GetResult(
			reinterpret_cast<IDxcBlob**>(shaderBlob.GetAddressOf())
		);
	}
	else
	{
		dxcResult->GetErrorBuffer(
			reinterpret_cast<IDxcBlobEncoding**>(errorBlob.GetAddressOf())
		);
	}
	return hr;
}
bool FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nposIndex)
{
	nposIndex = 0;
	if (pNodeAnim->mNumPositionKeys > 0)
	{
		return false;
	}

	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if ((AnimationTime >= (FLOAT)pNodeAnim->mPositionKeys[i].mTime)
			&& (AnimationTime < (FLOAT)pNodeAnim->mPositionKeys[i + 1].mTime))
		{
			nposIndex = i;
			return true;
		}
	}
	return false;
}


bool FindRotation(FLOAT AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nRotationIndex)
{
	nRotationIndex = 0;
	if (!(pNodeAnim->mNumRotationKeys > 0))
	{
		return false;
	}

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		// —œ∏Ò≈–∂œ ±º‰Tick «∑Ò‘⁄¡Ω∏ˆπÿºÅE°÷ÆºÅE
		if ((AnimationTime >= (FLOAT)pNodeAnim->mRotationKeys[i].mTime)
			&& (AnimationTime < (FLOAT)pNodeAnim->mRotationKeys[i + 1].mTime))
		{
			nRotationIndex = i;
			return true;
		}
	}

	return false;
}


BOOL FindScaling(FLOAT AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nScalingIndex)
{
	nScalingIndex = 0;
	if (!(pNodeAnim->mNumScalingKeys > 0))
	{
		return false;
	}

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		// —œ∏Ò≈–∂œ ±º‰Tick «∑Ò‘⁄¡Ω∏ˆπÿºÅE°÷ÆºÅE
		if ((AnimationTime >= (FLOAT)pNodeAnim->mScalingKeys[i].mTime)
			&& (AnimationTime < (FLOAT)pNodeAnim->mScalingKeys[i + 1].mTime))
		{
			nScalingIndex = i;
			return true;
		}
	}

	return false;
}
void CalcInterpolatedPosition(DirectX::XMVECTOR& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		mxOut = DirectX::XMVectorSet(pNodeAnim->mPositionKeys[0].mValue.x, pNodeAnim->mPositionKeys[0].mValue.y, pNodeAnim->mPositionKeys[0].mValue.z, 0);
		return;
	}

	UINT PositionIndex = 0;
	if (!FindPosition(AnimationTime, pNodeAnim, PositionIndex))
	{
		mxOut = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	UINT NextPositionIndex = (PositionIndex + 1);

	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float deltatime = static_cast<float>(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - static_cast<float>(pNodeAnim->mPositionKeys[PositionIndex].mTime)) / deltatime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	VectorLerp(mxOut, pNodeAnim->mPositionKeys[PositionIndex].mValue
		, pNodeAnim->mPositionKeys[NextPositionIndex].mValue, Factor);
}

void CalcInterpolatedRotation(DirectX::XMVECTOR& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		QuaternionEqual(mxOut, pNodeAnim->mRotationKeys[0].mValue);
		return;
	}

	UINT RotationIndex = 0;
	if (!FindRotation(AnimationTime, pNodeAnim, RotationIndex))
	{
		mxOut = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}
	UINT NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float Deltatime = static_cast<float>(pNodeAnim->mRotationKeys[NextRotationIndex].mTime
		- pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = AnimationTime - static_cast<float>(pNodeAnim->mRotationKeys[RotationIndex].mTime) / Deltatime;
	assert(Factor >= 0.0f && Factor <= 1.0f);


	DirectX::XMQuaternionNormalize(mxOut);
}

void CalcInterpolatedScaling(DirectX::XMVECTOR& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		VectorEqual(mxOut, pNodeAnim->mScalingKeys[0].mValue);
		return;
	}



	UINT ScalingIndex = 0;
	if (!FindScaling(AnimationTime, pNodeAnim, ScalingIndex))
	{// µ±«∞ ±º‰÷°√ª”–Àı∑≈±‰ªª£¨∑µªÿ 1.0Àı∑≈±»¿˝
		mxOut = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		return;
	}

	UINT NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (FLOAT)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (FLOAT)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	VectorLerp(mxOut
		, pNodeAnim->mScalingKeys[ScalingIndex].mValue
		, pNodeAnim->mScalingKeys[NextScalingIndex].mValue
		, Factor);


}

void Model::ProcessBoneNode(const aiAnimation* p_animation, const aiScene* pScene, const aiNode* node
	, float AnimationTime,const DirectX::XMMATRIX& ParentNodeTransform)
	{
		
	
	DirectX::XMFLOAT4X4 NodeTransformation = {};
	   
	   ConvertToaiMatrix4x4toXMFLOAT4x4(node->mTransformation, NodeTransformation);

		CStringA strNodeName(node->mName.data);
		const aiNodeAnim* pNodeAnim =SearchNodeAnim(p_animation, strNodeName);
		//ñºëOÇ™strNodeNameÇ∆àÍívÇ∑ÇÈÉAÉjÉÅÅ[ÉVÉáÉìÇ™ÅAp_AnimationÇÃÉ`ÉÉÉìÉlÉãÇÃíÜÇ©ÇÁë∂ç›ÇµÇƒÇ¢ÇΩÇÁ
		//ÇªÇÍÇpNodeAnimÇÃì‡óeÇ∆Ç∑ÇÈÅB
		
		DirectX::XMMATRIX NodeTransformationXM = DirectX::XMLoadFloat4x4(&NodeTransformation);

		if (pNodeAnim)
		{
			DirectX::XMVECTOR tempcalclatedScaling = {};
			CalcInterpolatedScaling(tempcalclatedScaling, AnimationTime, pNodeAnim);
			DirectX::XMMATRIX calclatedScaling = DirectX::XMMatrixScalingFromVector(tempcalclatedScaling);
			
			
			DirectX::XMVECTOR tempcalclatedRotation = {};
			CalcInterpolatedRotation(tempcalclatedRotation, AnimationTime, pNodeAnim);
			DirectX::XMMATRIX calclatedRotation = DirectX::XMMatrixScalingFromVector(tempcalclatedRotation);

			DirectX::XMVECTOR tempcalclatedTranslation = {};
			CalcInterpolatedPosition(tempcalclatedTranslation, AnimationTime, pNodeAnim);
			DirectX::XMMATRIX calclatedTranslation = DirectX::XMMatrixScalingFromVector(tempcalclatedTranslation);


			//pnodeanimÇÃÉfÅ[É^ÇóòópÇµÅA
			//ÉXÉPÅ[ÉäÉìÉOÅAâÒì]ÅAïΩçsà⁄ìÆÇÃåvéZÇçsÇ§
			//åvéZÇµÇΩè„ãLéOÇ¬ÇÃåãâ (calclated~)ÇÅANodeTransformationÇ…ë„ì¸Ç∑ÇÈÅB
			NodeTransformationXM = calclatedScaling * calclatedRotation * calclatedTranslation;
			// OpenGL£∫TranslationM* RotationM* ScalingM;
		}
		UINT nBoneIndex = 0;
		//êeÉmÅ[ÉhãÛä‘Ç÷Ç∆ç¿ïWïœä∑
		DirectX::XMMATRIX GlobalTransformation =DirectX::XMMatrixMultiply(NodeTransformationXM, ParentNodeTransform);
		//GPUÇ…ìnÇ∑É{Å[ÉìçsóÒÇ…ç¿ïWïœä∑
		bones[nBoneIndex].transformmatrixforGPU = bones[nBoneIndex].boneoffsetmatrix * GlobalTransformation*
			DirectX::XMLoadFloat4x4(&modelmat);
		for (UINT i = 0; i < node->mNumChildren; i++) {
			ProcessBoneNode(p_animation, pScene, node->mChildren[i], AnimationTime, GlobalTransformation);
		}
		
	}
__inline VOID VectorLerp(DirectX::XMVECTOR& vOut, aiVector3D& aivStart, aiVector3D& aivEnd, FLOAT t)
{
	DirectX::XMVECTOR vStart = DirectX::XMVectorSet(aivStart.x, aivStart.y, aivStart.z, 0);
	DirectX::XMVECTOR vEnd = DirectX::XMVectorSet(aivEnd.x, aivEnd.y, aivEnd.z, 0);
	vOut = DirectX::XMVectorLerp(vStart, vEnd, t);
}

__inline const DirectX::XMVECTOR& QuaternionEqual(DirectX::XMVECTOR& qDX, const aiQuaternion& qAI)
{
	qDX = DirectX::XMVectorSet(qAI.x, qAI.y, qAI.z, qAI.w);
	return qDX;
}

__inline VOID QuaternionSlerp(DirectX::XMVECTOR& vOut, aiQuaternion& qStart, aiQuaternion& qEnd, FLOAT t)
{
	//DirectXMathÀƒ‘™ ˝∫Ø ˝ π”√XMVECTOR 4 - vector¿¥±˙ÊæÀƒ‘™ ˝£¨∆‰÷–X°¢Y∫ÕZ∑÷¡ø « ∏¡ø≤ø∑÷£¨W∑÷¡ø «±ÅEø≤ø∑÷°£

	DirectX::XMVECTOR qdxStart;
	DirectX::XMVECTOR qdxEnd;
	QuaternionEqual(qdxStart, qStart);
	QuaternionEqual(qdxEnd, qEnd);

	vOut = DirectX::XMQuaternionSlerp(qdxStart, qdxEnd, t);
}

__inline const DirectX::XMVECTOR& VectorEqual(DirectX::XMVECTOR& vDX, const aiVector3D& vAI)
{
	vDX = DirectX::XMVectorSet(vAI.x, vAI.y, vAI.z, 0);
	return vDX;
}

aiNodeAnim* SearchNodeAnim(const aiAnimation* pAnimation, const CStringA strNodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++)
	{
		if (CStringA(pAnimation->mChannels[i]->mNodeName.data) == strNodeName)
		{
			return pAnimation->mChannels[i];
		}
	}

	return nullptr;
}

//------------------------------------------------------------------------------------------------
// Heap-allocating UpdateSubresources implementation
inline UINT64 UpdateSubresources(
	_In_ ID3D12GraphicsCommandList* pCmdList,
	_In_ ID3D12Resource* pDestinationResource,
	_In_ ID3D12Resource* pIntermediate,
	UINT64 IntermediateOffset,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
	_In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData) noexcept
{
	UINT64 RequiredSize = 0;
	UINT64 MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * NumSubresources;
	if (MemToAlloc > SIZE_MAX)
	{
		return 0;
	}
	void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
	if (pMem == nullptr)
	{
		return 0;
	}
	auto pLayouts = static_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
	UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + NumSubresources);
	UINT* pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);

	auto Desc = pDestinationResource->GetDesc();
	ID3D12Device* pDevice = nullptr;
	pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
	pDevice->Release();

	UINT64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
	HeapFree(GetProcessHeap(), 0, pMem);
	return Result;
}
//------------------------------------------------------------------------------------------------
// Returns required size of a buffer to be used for data upload
inline UINT64 GetRequiredIntermediateSize(
	_In_ ID3D12Resource* pDestinationResource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources) noexcept
{
	auto Desc = pDestinationResource->GetDesc();
	UINT64 RequiredSize = 0;

	ID3D12Device* pDevice = nullptr;
	pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
	pDevice->Release();

	return RequiredSize;
}