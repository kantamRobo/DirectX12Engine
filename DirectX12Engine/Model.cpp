
#include "Model.h"
#include <atlstr.h>
#include "MathUtility.h"
#include <stdexcept>
#include <experimental/coroutine>
#include <experimental/filesystem>
#include <iosfwd>
#include <dxcapi.h>


using namespace MathUtility;
void Model::Init(const std::string pFile)
{
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

	//ƒƒbƒVƒ…‚Ìƒƒ‚ƒŠ‚ğŠm•Û
	meshes.clear();
	meshes.resize(m_pScene->mNumMeshes);

	//ƒƒbƒVƒ…ƒf[ƒ^‚ğ•ÏŠ·
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = m_pScene->mMeshes[i];
		ProcessAssimpMesh(pMesh);
	}

}

// for: NumChildren







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
		//‚±‚¿‚ç‚Íƒ\[ƒXŒ³Apaimesh‚Ì‘½dƒ‹[ƒv‚É‚È‚Á‚Ä‚¢‚é‚©‚ğ’²‚×‚éB
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
			//aiMatrix4x4‚©‚çAXMMATRIX‚Ö‚Ì•ÏŠ·ŠÖ”‚ğì¬‚µ‚Ä‚¨‚­
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
	// ’¸“_ƒoƒbƒtƒ@‚ÆƒCƒ“ƒfƒbƒNƒXƒoƒbƒtƒ@‚Ì¶¬.
	m_vertexBuffer = CreateBuffer(p_device,vertices.size(), vertices.data());
	m_indexBuffer = CreateBuffer(p_device,indices.size(), indices.data());
	m_indexCount = indices.size();

	// Šeƒoƒbƒtƒ@‚Ìƒrƒ…[‚ğ¶¬.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = vertices.size();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = indices.size();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}



void Model::Prepare(ID3D12Device* p_device,const Commands& in_commands) {
	// ƒVƒF[ƒ_[‚ğƒRƒ“ƒpƒCƒ‹.
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
	cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0 ƒŒƒWƒXƒ^
	srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ƒŒƒWƒXƒ^
	sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0); // s0 ƒŒƒWƒXƒ^

	CD3DX12_ROOT_PARAMETER rootParams[3];
	rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_VERTEX);
	rootParams[1].InitAsDescriptorTable(1, &srv, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParams[2].InitAsDescriptorTable(1, &sampler, D3D12_SHADER_VISIBILITY_PIXEL);

	// ƒ‹[ƒgƒVƒOƒlƒ`ƒƒ‚Ì\’z
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
	// RootSignature ‚Ì¶¬
	hr = p_device->CreateRootSignature(
		0,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature)
	);
	if (FAILED(hr))
	{
		throw std::runtime_error("CrateRootSignature failed.");
	}

	// ƒCƒ“ƒvƒbƒgƒŒƒCƒAƒEƒg
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, offsetof(Vertex,Color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,UV), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA}
	};

	// ƒpƒCƒvƒ‰ƒCƒ“ƒXƒe[ƒgƒIƒuƒWƒFƒNƒg‚Ì¶¬.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	// ƒVƒF[ƒ_[‚ÌƒZƒbƒg
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.Get());
	// ƒuƒŒƒ“ƒhƒXƒe[ƒgİ’è
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// ƒ‰ƒXƒ^ƒ‰ƒCƒU[ƒXƒe[ƒg
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// o—Íæ‚Í1ƒ^[ƒQƒbƒg
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// ƒfƒvƒXƒoƒbƒtƒ@‚ÌƒtƒH[ƒ}ƒbƒg‚ğİ’è
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };

	// ƒ‹[ƒgƒVƒOƒlƒ`ƒƒ‚ÌƒZƒbƒg
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// ƒ}ƒ‹ƒ`ƒTƒ“ƒvƒ‹İ’è
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // ‚±‚ê‚ğ–Y‚ê‚é‚ÆŠG‚ªo‚È‚¢•Œx‚ào‚È‚¢‚Ì‚Å’ˆÓ.

	hr = p_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateGraphicsPipelineState failed");
	}

	PrepareDescriptorHeapForCubeApp(p_device);

	// ’è”ƒoƒbƒtƒ@/’è”ƒoƒbƒtƒ@ƒrƒ…[‚Ì¶¬
	m_constantBuffers.resize(FrameBufferCount);
	m_cbViews.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
		m_constantBuffers[i] = CreateBuffer(p_device,bufferSize, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
		cbDesc.BufferLocation = m_constantBuffers[i]->GetGPUVirtualAddress();
		cbDesc.SizeInBytes = bufferSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
		p_device->CreateConstantBufferView(&cbDesc, handleCBV);

		m_cbViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
	}

	// ƒeƒNƒXƒ`ƒƒ‚Ì¶¬
	m_texture = CreateTexture("texture.tga",p_device,in_commands);

	// ƒTƒ“ƒvƒ‰[‚Ì¶¬
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

	// ƒTƒ“ƒvƒ‰[—pƒfƒBƒXƒNƒŠƒvƒ^ƒq[ƒv‚Ì0”Ô–Ú‚ğg—p‚·‚é
	auto descriptorSampler = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heapSampler->GetCPUDescriptorHandleForHeapStart(), SamplerDescriptorBase, m_samplerDescriptorSize);
	p_device->CreateSampler(&samplerDesc, descriptorSampler);
	m_sampler = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSampler->GetGPUDescriptorHandleForHeapStart(), SamplerDescriptorBase, m_samplerDescriptorSize);

	// ƒeƒNƒXƒ`ƒƒ‚©‚çƒVƒF[ƒ_[ƒŠƒ\[ƒXƒrƒ…[‚Ì€”õ.
	auto srvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), TextureSrvDescriptorBase, m_srvcbvDescriptorSize);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	p_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, srvHandle);
	m_srv = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), TextureSrvDescriptorBase, m_srvcbvDescriptorSize);

}
Microsoft::WRL::ComPtr<ID3D12Resource1> Model::CreateBuffer(ID3D12Device* p_device,UINT bufferSize, const void* initialData)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource1> buffer;
	auto heaprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto res = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	hr = p_device->CreateCommittedResource(
		&heaprop,
		D3D12_HEAP_FLAG_NONE,
		&res,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);

	// ‰Šúƒf[ƒ^‚Ìw’è‚ª‚ ‚é‚Æ‚«‚É‚ÍƒRƒs[‚·‚é
	if (SUCCEEDED(hr) && initialData != nullptr)
	{
		void* mapped;
		CD3DX12_RANGE range(0, 0);
		hr = buffer->Map(0, &range, &mapped);
		if (SUCCEEDED(hr))
		{
			memcpy(mapped, initialData, bufferSize);
			buffer->Unmap(0, nullptr);
		}
	}

	return buffer;
}

void Model::PrepareDescriptorHeapForCubeApp(ID3D12Device* p_device)
{
	// CBV/SRV ‚ÌƒfƒBƒXƒNƒŠƒvƒ^ƒq[ƒv
	//  0:ƒVƒF[ƒ_[ƒŠƒ\[ƒXƒrƒ…[
	//  1,2 : ’è”ƒoƒbƒtƒ@ƒrƒ…[ (FrameBufferCount”•ªg—p)
	UINT count = FrameBufferCount + 1;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	  count,
	  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
	  0
	};
	p_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_heapSrvCbv));
	m_srvcbvDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ƒ_ƒCƒiƒ~ƒbƒNƒTƒ“ƒvƒ‰[‚ÌƒfƒBƒXƒNƒŠƒvƒ^ƒq[ƒv
	D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	  1,
	  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
	  0
	};
	p_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_heapSampler));
	m_samplerDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
}

// è“®‚Å¶¬”Å
Microsoft::WRL::ComPtr<ID3D12Resource1> Model::CreateTexture(const std::string& fileName,ID3D12Device* p_device,const Commands& in_commands)
{
	Microsoft::WRL::ComPtr<ID3D12Resource1> texture;
	int texWidth = 0, texHeight = 0, channels = 0;
	auto* pImage = stbi_load(fileName.c_str(), &texWidth, &texHeight, &channels, 0);

	// ƒTƒCƒYEƒtƒH[ƒ}ƒbƒg‚©‚çƒeƒNƒXƒ`ƒƒƒŠƒ\[ƒX‚ÌDesc€”õ
	auto texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		texWidth, texHeight,
		1,  // ”z—ñƒTƒCƒY
		1   // ƒ~ƒbƒvƒ}ƒbƒv”
	);
	auto heaprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	// ƒeƒNƒXƒ`ƒƒ¶¬
	p_device->CreateCommittedResource(
		&heaprop,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texture)
	);

	// ƒXƒe[ƒWƒ“ƒOƒoƒbƒtƒ@€”õ
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts;
	UINT numRows;
	UINT64 rowSizeBytes, totalBytes;
	p_device->GetCopyableFootprints(&texDesc, 0, 1, 0, &layouts, &numRows, &rowSizeBytes, &totalBytes);
	Microsoft::WRL::ComPtr<ID3D12Resource1> stagingBuffer = CreateBuffer(totalBytes, nullptr);

	// ƒXƒe[ƒWƒ“ƒOƒoƒbƒtƒ@‚É‰æ‘œ‚ğƒRƒs[
	{
		const UINT imagePitch = texWidth * sizeof(uint32_t);
		void* pBuf;
		CD3DX12_RANGE range(0, 0);
		stagingBuffer->Map(0, &range, &pBuf);
		for (UINT h = 0; h < numRows; ++h)
		{
			auto dst = static_cast<char*>(pBuf) + h * rowSizeBytes;
			auto src = pImage + h * imagePitch;
			memcpy(dst, src, imagePitch);
		}
	}

	// ƒRƒ}ƒ“ƒh€”õ.
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command;
	p_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, [m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&command));
	Microsoft::WRL::ComPtr<ID3D12Fence1> fence;
	p_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	// “]‘—ƒRƒ}ƒ“ƒh
	D3D12_TEXTURE_COPY_LOCATION src{}, dst{};
	dst.pResource = texture.Get();
	dst.SubresourceIndex = 0;
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

	src.pResource = stagingBuffer.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = layouts;
	command->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

	// ƒRƒs[Œã‚É‚ÍƒeƒNƒXƒ`ƒƒ‚Æ‚µ‚Ä‚ÌƒXƒe[ƒg‚Ö.
	auto barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		texture.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	command->ResourceBarrier(1, &barrierTex);

	command->Close();

	// ƒRƒ}ƒ“ƒh‚ÌÀs
	ID3D12CommandList* cmds[] = { command.Get() };
	
		in_commands.queue->ExecuteCommandLists(1, cmds);
	// Š®—¹‚µ‚½‚çƒVƒOƒiƒ‹‚ğ—§‚Ä‚é.
	const UINT64 expected = 1;
	in_commands.queue->Signal(fence.Get(), expected);

	// ƒeƒNƒXƒ`ƒƒ‚Ìˆ—‚ªŠ®—¹‚·‚é‚Ü‚Å‘Ò‚Â.
	while (expected != fence->GetCompletedValue())
	{
		Sleep(1);
	}

	stbi_image_free(pImage);
	return texture;
}


HRESULT Model::CompileShaderFromFile(
	const std::wstring& fileName, const std::wstring& profile, Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob)
{
	using namespace std::experimental::filesystem;

	path filePath(fileName);
	std::ifstream infile(filePath);
	std::vector<char> srcData;
	if (!infile)
		throw std::runtime_error("shader not found");
	srcData.resize(uint32_t(infile.seekg(0, infile.end).tellg()));
	infile.seekg(0, infile.beg).read(srcData.data(), srcData.size());

	// DXC ‚É‚æ‚éƒRƒ“ƒpƒCƒ‹ˆ—
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
	  L"/O2" // ƒŠƒŠ[ƒXƒrƒ‹ƒh‚Å‚ÍÅ“K‰»
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
		// ÑÏ¸ñÅĞ¶ÏÊ±¼äTickÊÇ·ñÔÚÁ½¸ö¹Ø¼E¡Ö®¼E
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
		// ÑÏ¸ñÅĞ¶ÏÊ±¼äTickÊÇ·ñÔÚÁ½¸ö¹Ø¼E¡Ö®¼E
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
	{// µ±Ç°Ê±¼äÖ¡Ã»ÓĞËõ·Å±ä»»£¬·µ»Ø 1.0Ëõ·Å±ÈÀı
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
		//–¼‘O‚ªstrNodeName‚Æˆê’v‚·‚éƒAƒjƒ[ƒVƒ‡ƒ“‚ªAp_Animation‚Ìƒ`ƒƒƒ“ƒlƒ‹‚Ì’†‚©‚ç‘¶İ‚µ‚Ä‚¢‚½‚ç
		//‚»‚ê‚ğpNodeAnim‚Ì“à—e‚Æ‚·‚éB
		
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


			//pnodeanim‚Ìƒf[ƒ^‚ğ—˜—p‚µA
			//ƒXƒP[ƒŠƒ“ƒOA‰ñ“]A•½sˆÚ“®‚ÌŒvZ‚ğs‚¤
			//ŒvZ‚µ‚½ã‹LO‚Â‚ÌŒ‹‰Ê(calclated~)‚ğANodeTransformation‚É‘ã“ü‚·‚éB
			NodeTransformationXM = calclatedScaling * calclatedRotation * calclatedTranslation;
			// OpenGL£ºTranslationM* RotationM* ScalingM;
		}
		UINT nBoneIndex = 0;
		//eƒm[ƒh‹óŠÔ‚Ö‚ÆÀ•W•ÏŠ·
		DirectX::XMMATRIX GlobalTransformation =DirectX::XMMatrixMultiply(NodeTransformationXM, ParentNodeTransform);
		//GPU‚É“n‚·ƒ{[ƒ“s—ñ‚ÉÀ•W•ÏŠ·
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
	//DirectXMathËÄÔªÊıº¯ÊıÊ¹ÓÃXMVECTOR 4 - vectorÀ´±úæ¾ËÄÔªÊı£¬ÆäÖĞX¡¢YºÍZ·ÖÁ¿ÊÇÊ¸Á¿²¿·Ö£¬W·ÖÁ¿ÊÇ±E¿²¿·Ö¡£

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
