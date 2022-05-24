#include <unordered_set>
#include <wrl.h>
#include <d3dcommon.h>
#include <dxcapi.h>
#include <wrl.h>
#include <filesystem>
#include <istream>
#include <fstream>
class CustomIncludeHandler 
{
public:
    Microsoft::WRL::ComPtr<IDxcIncludeHandler> pDefaultIncludeHandler;
    Microsoft::WRL::ComPtr<IDxcUtils> pUtils;
    

  

    std::unordered_set<std::string> IncludedFiles;
    HRESULT CompileShaderFromFile(
        const std::wstring& fileName, const std::wstring& profile, Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob);
};

