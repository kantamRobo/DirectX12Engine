#include "ShaderCompiler.h"
#include <string>
#include <vector>
#include <stierr.h>
#include <sstream>
#include <fstream>
#include <atlbase.h>
using namespace Microsoft::WRL;

void ShaderCompiler::LoadRaytracing(const wchar_t* filePath)
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
