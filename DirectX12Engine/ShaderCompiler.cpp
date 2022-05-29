#include "ShaderCompiler.h"
#include <string>
#include <vector>
using namespace Microsoft::WRL;


void ShaderCompiler::Create_Shader_Program(D3D12ShaderCompilerInfo& shaderCompiler, D3D12ShaderInfo& dxr)
{
	// Load and compile the ray generation shader
	dxr = D3D12ShaderInfo(D3D12ShaderInfo(L"shaders\\RayGen.hlsl", L"", L"lib_6_3"));
	Compile_Shader(dxr);
}
void ShaderCompiler::Compile_Shader(D3D12ShaderInfo& shaderInfo)
{
	Compile_Shader(shaderInfo, shaderInfo.blob);
}
void ShaderCompiler::Compile_Shader( D3D12ShaderInfo& info, IDxcBlob** blob)
{
HRESULT hr;
UINT32 code(0);
IDxcBlobEncoding* pShaderText(nullptr);

std::unique_ptr<D3D12ShaderCompilerInfo> compilerInfo;



	
	
	D3D12ShaderCompilerInfo shaderCompiler;

	HRESULT hr = shaderCompiler.DxcDllHelper.Initialize();
	
	hr = shaderCompiler.DxcDllHelper.CreateInstance(CLSID_DxcCompiler, &shaderCompiler.compiler);
	
	hr = shaderCompiler.DxcDllHelper.CreateInstance(CLSID_DxcLibrary, &shaderCompiler.library);
	
	compilerInfo = std::make_unique<D3D12ShaderCompilerInfo>();

// Load and encode the shader file
auto hr = compilerInfo->library->CreateBlobFromFile(info.filename, &code, &pShaderText);


// Create the compiler include handler
Microsoft::WRL::ComPtr<IDxcIncludeHandler> dxcIncludeHandler;
hr = compilerInfo->library->CreateIncludeHandler(&dxcIncludeHandler);


// Compile the shader
IDxcOperationResult* result;
hr = compilerInfo->compiler->Compile(
	pShaderText,
	info.filename,
	info.entryPoint,
	info.targetProfile,
	info.arguments,
	info.argCount,
	info.defines,
	info.defineCount,
	dxcIncludeHandler.Get(),
	&result);


// Verify the result
result->GetStatus(&hr);
if (FAILED(hr))
{
	IDxcBlobEncoding* error;
	hr = result->GetErrorBuffer(&error);
	

	// Convert error blob to a string
	std::vector<char> infoLog(error->GetBufferSize() + 1);
	memcpy(infoLog.data(), error->GetBufferPointer(), error->GetBufferSize());
	infoLog[error->GetBufferSize()] = 0;

	std::string errorMsg = "Shader Compiler Error:\n";
	errorMsg.append(infoLog.data());

	MessageBoxA(nullptr, errorMsg.c_str(), "Error!", MB_OK);
	return;
}

hr = result->GetResult(blob);

}
