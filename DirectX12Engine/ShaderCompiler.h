#pragma once

#include <wrl.h>
#include "Structures.h"
#include <dxcapi.h>
#include <Windows.h>
class ShaderCompiler
{
	void Create_RayGen_Program(D3D12Global& d3d, DXRGlobal& dxr, D3D12ShaderCompilerInfo& shaderCompiler);
	void Compile_Shader(D3D12ShaderCompilerInfo& compilerInfo, RtProgram& program);

	void Compile_Shader(D3D12ShaderCompilerInfo& compilerInfo, D3D12ShaderInfo& info, IDxcBlob** blob);

};

