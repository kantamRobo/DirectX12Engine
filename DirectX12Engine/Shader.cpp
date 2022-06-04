#include "Shader.h"
#include "pch.h"
#include <d3dx12.h>
void Shader::LoadRaytracing(const wchar_t* filePath)
{
	std::ifstream shaderFile(filePath);
	if (shaderFile.good() == false) {
		std::wstring errormessage = L"シェーダーファイルのオープンに失敗しました。\n";
		errormessage += filePath;
		MessageBoxW(nullptr, errormessage.c_str(), L"エラー", MB_OK);
		std::abort();
	}

	std::stringstream strStream;
	strStream << shaderFile.rdbuf();
	std::string shader = strStream.str();
	//シェーダーのテキストファイルから、BLOBを作成する。
	CComPtr<IDxcLibrary> dxclib;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxclib));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"DXCLIBの作成に失敗しました。", L"エラー", MB_OK);
		std::abort();
	}
	CComPtr< IDxcIncludeHandler> includerHandler;
	hr = dxclib->CreateIncludeHandler(&includerHandler);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"CreateIncludeHandlerに失敗しました。", L"エラー", MB_OK);
		std::abort();
	}

	//dxcコンパイラの作成。
	CComPtr<IDxcCompiler> dxcCompiler;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"dxcコンパイラの作成に失敗しました。", L"エラー", MB_OK);
		std::abort();
	}
	//ソースコードのBLOBを作成する。
	uint32_t codePage = CP_UTF8;
	CComPtr< IDxcBlobEncoding> sourceBlob;
	hr = dxclib->CreateBlobFromFile(filePath, &codePage, &sourceBlob);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"シェーダーソースのBlobの作成に失敗しました。", L"エラー", MB_OK);
		std::abort();
	}

	CComPtr<IDxcIncludeHandler> dxcIncludeHandler;
	dxclib->CreateIncludeHandler(&dxcIncludeHandler);
	const wchar_t* args[] = {
		L"-I Assets\\shader",
	};
	//コンパイル。
	CComPtr<IDxcOperationResult> result;
	hr = dxcCompiler->Compile(
		sourceBlob, // pSource
		filePath, // pSourceName
		L"",		// pEntryPoint
		L"lib_6_3", // pTargetProfile
		args, 1, // pArguments, argCount
		nullptr, 0, // pDefines, defineCount
		dxcIncludeHandler, // pIncludeHandler
		&result); // ppResult
	if (SUCCEEDED(hr)) {
		result->GetStatus(&hr);
	}

	if (FAILED(hr))
	{
		if (result)
		{
			CComPtr<IDxcBlobEncoding> errorsBlob;
			hr = result->GetErrorBuffer(&errorsBlob);
			if (SUCCEEDED(hr) && errorsBlob)
			{
				std::string errormessage = "Compilation failed with errors:\n%hs\n";
				errormessage += (const char*)errorsBlob->GetBufferPointer();
				MessageBoxA(nullptr, errormessage.c_str(), "エラー", MB_OK);

			}
		}
		// Handle compilation error...
	}
	else {
		result->GetResult(&m_dxcBlob);
	}
}
a
void Shader::CreateTesslelationShader(ID3D12Device* device,
    Microsoft::WRL::ComPtr<ID3D12RootSignature> in_patchrootsignature,
    Microsoft::WRL::ComPtr<ID3D12PipelineState> in_patchpipelinestate,
    DirectX::RenderTargetState targetstate)
{
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

    Shader hullShader;
    hullShader.LoadRaytracing(L"HSterrein.hlsl");

    Shader DomainShader;
    hullShader.LoadRaytracing(L"DSterrein.hlsl");

    Shader VertexShader;
    hullShader.LoadRaytracing(L"VSterrein.hlsl");

    Shader Pixelshader;
    hullShader.LoadRaytracing(L"PSterrein.hlsl");





    Microsoft::WRL::ComPtr<ID3DBlob> hullshader = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> hullshadererror = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> domainshader = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> domainshadererror = nullptr;



    std::string errstrhull;
    errstrhull.resize(domainshadererror->GetBufferSize());


    std::memcpy(errstrhull.data(), hullshadererror->GetBufferPointer(), hullshadererror->GetBufferSize());


    OutputDebugStringA(errstrhull.c_str());

    std::string errstrdomain;
    errstrdomain.resize(domainshadererror->GetBufferSize());
    std::memcpy(errstrdomain.data(), domainshadererror->GetBufferPointer(), domainshadererror->GetBufferSize());



    OutputDebugStringA(errstrdomain.c_str());

    D3D12_EXPORT_DESC libExport[4];
    for (int i = 0; i < 4; i++)
    {
        libExport[i].Name = shaderDatas[i].entryPointName;
        libExport[i].ExportToRename = nullptr;
        libExport[i].Flags = D3D12_EXPORT_FLAG_NONE;
    };

    D3D12_DXIL_LIBRARY_DESC dxLibhulldesc;
    IDxcBlob* pBlobhull = hullShader.GetCompiledDxcBlob();
    dxLibhulldesc.DXILLibrary.pShaderBytecode = pBlobhull->GetBufferPointer();
    dxLibhulldesc.DXILLibrary.BytecodeLength = pBlobhull->GetBufferSize();
    dxLibhulldesc.NumExports = ARRAYSIZE(libExport);
    dxLibhulldesc.pExports = libExport;


    D3D12_DXIL_LIBRARY_DESC dxLibdomaindesc;
    IDxcBlob* pBlobdomain = DomainShader.GetCompiledDxcBlob();
    dxLibdomaindesc.DXILLibrary.pShaderBytecode = pBlobdomain->GetBufferPointer();
    dxLibdomaindesc.DXILLibrary.BytecodeLength = pBlobdomain->GetBufferSize();
    dxLibdomaindesc.NumExports = ARRAYSIZE(libExport);
    dxLibdomaindesc.pExports = libExport;

    D3D12_DXIL_LIBRARY_DESC dxLibvertexdesc;
    IDxcBlob* pBlobvertex = VertexShader.GetCompiledDxcBlob();
    dxLibhulldesc.DXILLibrary.pShaderBytecode = pBlobvertex->GetBufferPointer();
    dxLibhulldesc.DXILLibrary.BytecodeLength = pBlobvertex->GetBufferSize();
    dxLibhulldesc.NumExports = ARRAYSIZE(libExport);
    dxLibhulldesc.pExports = libExport;


    D3D12_DXIL_LIBRARY_DESC dxLibpixeldesc;
    IDxcBlob* pBlobpixel = Pixelshader.GetCompiledDxcBlob();
    dxLibdomaindesc.DXILLibrary.pShaderBytecode = pBlobpixel->GetBufferPointer();
    dxLibdomaindesc.DXILLibrary.BytecodeLength = pBlobpixel->GetBufferSize();
    dxLibdomaindesc.NumExports = ARRAYSIZE(libExport);
    dxLibdomaindesc.pExports = libExport;

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
        IID_PPV_ARGS(in_patchrootsignature.GetAddressOf()));



    D3D12_SHADER_BYTECODE patchVS = {};
    D3D12_SHADER_BYTECODE patchPS = {};
    D3D12_SHADER_BYTECODE patchHS = {};
    D3D12_SHADER_BYTECODE patchDS = {};

    patchVS.BytecodeLength = dxLibvertexdesc.DXILLibrary.BytecodeLength;
    patchVS.pShaderBytecode = dxLibvertexdesc.DXILLibrary.pShaderBytecode;

    patchPS.BytecodeLength = dxLibpixeldesc.DXILLibrary.BytecodeLength;
    patchPS.pShaderBytecode = dxLibpixeldesc.DXILLibrary.pShaderBytecode;

    patchHS.BytecodeLength = dxLibhulldesc.DXILLibrary.BytecodeLength;
    patchHS.pShaderBytecode = dxLibhulldesc.DXILLibrary.pShaderBytecode;

    patchDS.BytecodeLength = dxLibdomaindesc.DXILLibrary.BytecodeLength;
    patchDS.pShaderBytecode = dxLibdomaindesc.DXILLibrary.pShaderBytecode;

    TesselationEffectPipelineDescription pipeline(&input, DirectX::CommonStates::Opaque,
        DirectX::CommonStates::DepthDefault,
        DirectX::CommonStates::CullCounterClockwise,
        targetstate);
  
    pipeline.CreatePipelineState(device, 
        in_patchrootsignature.Get()
        , patchVS, patchPS, patchDS, patchHS, in_patchpipelinestate.GetAddressOf());
}
