#pragma once

#include <wrl.h>
#include "Structures.h"
#include <dxcapi.h>
#include <Windows.h>
class ShaderCompiler
{
	void Create_Shader_Program(D3D12ShaderCompilerInfo& shaderCompiler, D3D12ShaderInfo& rgs);
	void Compile_Shader(D3D12ShaderInfo& compilerInfo);

	void Compile_Shader(D3D12ShaderInfo& info, IDxcBlob** blob);

};

