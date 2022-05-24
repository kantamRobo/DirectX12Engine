#include "CustomIncludeHandler.h"

HRESULT CustomIncludeHandler::CompileShaderFromFile(
    const std::wstring& fileName, const std::wstring& profile, Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob) {

    using namespace std::filesystem;

    path filePath(fileName);
    std::ifstream infile(filePath);
    std::vector<char> srcData;
    if (!infile)
        throw std::runtime_error("shader not found");
    srcData.resize(uint32_t(infile.seekg(0, infile.end).tellg()));
    infile.seekg(0, infile.beg).read(srcData.data(), srcData.size());

    // DXC によるコンパイル処理
    Microsoft::WRL::ComPtr<IDxcLibrary> library;
    Microsoft::WRL::ComPtr<IDxcCompiler> compiler;
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;
    Microsoft::WRL::ComPtr<IDxcOperationResult> dxcResult;
    Microsoft::WRL::ComPtr<IDxcIncludeHandler> pDefaultIncludeHandler;

    pDefaultIncludeHandler->QueryInterface(, ppvObject);

    DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
    library->CreateBlobWithEncodingFromPinned(srcData.data(), UINT(srcData.size()), CP_ACP, &source);
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
    Microsoft::WRL::ComPtr<IDxcUtils> pUtils;
    if (!pUtils)
    {
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(pUtils.GetAddressOf()));
        pUtils->CreateDefaultIncludeHandler(pDefaultIncludeHandler.GetAddressOf());
    }

    LPCWSTR compilerFlags[] = {
   #if _DEBUG
       L"/Zi", L"/O0",
   #else
       L"/O2" // リリースビルドでは最適化
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

    Microsoft::WRL::ComPtr<IDxcUtils> pincludeUtils;
    Microsoft::WRL::ComPtr<IDxcIncludeHandler> pincludeDefaultIncludeHandler;
    if (!pUtils)
    {
       DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(pincludeUtils.GetAddressOf()));
       pUtils->CreateDefaultIncludeHandler(pincludeDefaultIncludeHandler.GetAddressOf());
    }
    return hr;
}



