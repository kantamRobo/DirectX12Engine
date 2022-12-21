#include "Renderer.h"
#include <GraphicsMemory.h>
using namespace Microsoft::WRL;

//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
#define FMT_FLOAT3      DXGI_FORMAT_R32G32B32_FLOAT
#define FMT_FLOAT2      DXGI_FORMAT_R32G32_FLOAT
#define APPEND          D3D12_APPEND_ALIGNED_ELEMENT
#define IL_VERTEX       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
static const int InputElementCount = 4;
const D3D12_INPUT_ELEMENT_DESC InputElements[] = {
    { "POSITION", 0, FMT_FLOAT3, 0, APPEND, IL_VERTEX, 0 },
    { "NORMAL",   0, FMT_FLOAT3, 0, APPEND, IL_VERTEX, 0 },
    { "TEXCOORD", 0, FMT_FLOAT2, 0, APPEND, IL_VERTEX, 0 },
    { "TANGENT",  0, FMT_FLOAT3, 0, APPEND, IL_VERTEX, 0 }
};
const D3D12_INPUT_LAYOUT_DESC InputLayout = {
    InputElements,
    InputElementCount
};
Renderer::Renderer(DX::DeviceResources* deviceresources,UINT width,UINT height) {
   
    auto device = deviceresources->GetD3DDevice();
    std::unique_ptr<DirectX::GraphicsMemory> graphicmemory = std::make_unique< DirectX::GraphicsMemory>();
    //定数バッファの生成
    {
        

        auto incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (auto i = 0; i < 4; ++i)
        {
            //リソース生成
            m_CBDescriptorHeaps[i] = std::make_shared<DirectX::DescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

            m_transformCBBuffer[i] = graphicmemory->AllocateConstant(sizeof(Transform));

            
            auto address = m_transformCBBuffer[i].GpuAddress();
            auto handleCPU = m_CBDescriptorHeaps[i]->GetFirstCpuHandle();
            auto handleGPU = m_CBDescriptorHeaps[i]->GetFirstGpuHandle();

            handleCPU.ptr += incrementSize * i;
            handleGPU.ptr += incrementSize * i;

            
            
            auto eyePos = DirectX::XMVectorSet(0.0f, 1.0f, 2.0f, 0.0f);
            auto targetPos = DirectX::XMVectorZero();
            auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            auto fovY = DirectX::XMConvertToRadians(37.5f);
            auto aspect = static_cast<float>(width) / static_cast<float>(height);

            // 変換行列の設定.

       transform.World = DirectX::XMMatrixIdentity();
       transform.View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
       transform.World = DirectX::XMMatrixPerspectiveFovRH(fovY, aspect, 1.0f, 1000.0f);
        }


    }

    //ルートシグネチャの生成
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        D3D12_DESCRIPTOR_RANGE range = {};
        range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors = 1;
        range.BaseShaderRegister = 0;
        range.RegisterSpace = 0;
        range.OffsetInDescriptorsFromTableStart = 0;


        //ルートパラメーターの設定
        D3D12_ROOT_PARAMETER param[2] = {};
        param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[0].Descriptor.ShaderRegister = 0;
        param[0].Descriptor.RegisterSpace = 0;
        param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param[1].DescriptorTable.NumDescriptorRanges = 1;
        param[1].DescriptorTable.pDescriptorRanges = &range;
        param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



        // スタティックサンプラーの設定.
        D3D12_STATIC_SAMPLER_DESC sampler = {};
        sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
        sampler.MaxAnisotropy = 1;
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        sampler.MinLOD = -D3D12_FLOAT32_MAX;
        sampler.MaxLOD = +D3D12_FLOAT32_MAX;
        sampler.ShaderRegister = 0;
        sampler.RegisterSpace = 0;
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        // ルートシグニチャの設定.
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters = 2;
        desc.NumStaticSamplers = 1;
        desc.pParameters = param;
        desc.pStaticSamplers = &sampler;
        desc.Flags = flag;

        ComPtr<ID3DBlob> pBlob;
        ComPtr<ID3DBlob> pErrorBlob;

        // シリアライズ
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf());
        

        // ルートシグニチャを生成.
        hr = device->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(m_rootSignature.GetAddressOf()));
       
    }

    //パイプラインステートの生成
    {
        D3D12_RASTERIZER_DESC descRS;
        descRS.FillMode = D3D12_FILL_MODE_SOLID;
        descRS.CullMode = D3D12_CULL_MODE_NONE;
        descRS.FrontCounterClockwise = FALSE;
        descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        descRS.DepthClipEnable = FALSE;
        descRS.MultisampleEnable = FALSE;
        descRS.MultisampleEnable = FALSE;
        descRS.AntialiasedLineEnable = FALSE;
        descRS.ForcedSampleCount = 0;
        descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        // レンダーターゲットのブレンド設定.
        D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
            FALSE, FALSE,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL
        };
        // ブレンドステートの設定.
        D3D12_BLEND_DESC descBS;
        descBS.AlphaToCoverageEnable = FALSE;
        descBS.IndependentBlendEnable = FALSE;
        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            descBS.RenderTarget[i] = descRTBS;
        }

        // 深度ステンシルステートの設定.
        D3D12_DEPTH_STENCIL_DESC descDSS = {};
        descDSS.DepthEnable = TRUE;
        descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        descDSS.StencilEnable = FALSE;



        ComPtr<ID3DBlob> pVSBlob;
        ComPtr<ID3DBlob> pPSBlob;

        std::wstring vsPath;
        std::wstring psPath;



        vertexshader.LoadShader(L"Modelassimpvertex.hlsl", L"main_VS");
        pixelshader.LoadShader(L"Modelassimppixel.hlsl", L"main_PS");


        // 頂点シェーダ読み込み.
        auto hr = D3DReadFileToBlob(vsPath.c_str(), pVSBlob.GetAddressOf());
       

        // ピクセルシェーダ読み込み.
        hr = D3DReadFileToBlob(psPath.c_str(), pPSBlob.GetAddressOf());
       


        // パイプラインステートの設定.
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout = InputLayout;
        desc.pRootSignature = m_rootSignature.Get();
        desc.VS = { pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize() };
        desc.PS = { pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize() };
        desc.RasterizerState = descRS;
        desc.BlendState = descBS;
        desc.DepthStencilState = descDSS;
        desc.SampleMask = UINT_MAX;
        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;

        // パイプラインステートを生成.
        hr = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(m_pPSO.GetAddressOf()));
        
    }
}


void Renderer::Render(DX::DeviceResources* deviceresources)
{
    auto commandlist = deviceresources->GetCommandList();
    auto frameIndex = deviceresources->GetCurrentFrameIndex();
    // 描画処理.
    {
        ID3D12DescriptorHeap* descriptorheaps[] = { m_CBDescriptorHeaps->get()->Heap() };
        
        commandlist->SetGraphicsRootSignature(m_rootSignature.Get());
        commandlist->SetDescriptorHeaps(1, descriptorheaps);
        commandlist->SetGraphicsRootConstantBufferView(0, m_transformCBBuffer[(frameIndex+0)+1].GpuAddress());
        commandlist->SetGraphicsRootDescriptorTable(1, m_Texture.HandleGPU);
        commandlist->SetPipelineState(m_pPSO.Get());

        commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandlist->IASetVertexBuffers(0, 1, &m_VBV);
        commandlist->IASetIndexBuffer(&m_IBV);;

        auto count = static_cast<uint32_t>(m_Meshes[0].Indices.size());
        commandlist->DrawIndexedInstanced(count, 1, 0, 0, 0);
    }
}