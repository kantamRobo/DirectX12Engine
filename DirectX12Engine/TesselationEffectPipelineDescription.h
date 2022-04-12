#pragma once
#include <EffectPipelineStateDescription.h>
#include <stdexcept>
#include "DirectXHelpers.h"
#include "RenderTargetState.h"
struct TesselationEffectPipelineDescription:public DirectX::EffectPipelineStateDescription
{

    D3D12_INPUT_LAYOUT_DESC             inputLayout;
    D3D12_BLEND_DESC                    blendDesc;
    D3D12_DEPTH_STENCIL_DESC            depthStencilDesc;
    D3D12_RASTERIZER_DESC               rasterizerDesc;
    DirectX::RenderTargetState                   renderTargetState;
    D3D12_PRIMITIVE_TOPOLOGY_TYPE       primitiveTopology;
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE  stripCutValue;
    TesselationEffectPipelineDescription() {};
    TesselationEffectPipelineDescription(
        _In_opt_ const D3D12_INPUT_LAYOUT_DESC* iinputLayout,
        const D3D12_BLEND_DESC& blend,
        const D3D12_DEPTH_STENCIL_DESC& depthStencil,
        const D3D12_RASTERIZER_DESC& rasterizer,
        const DirectX::RenderTargetState& renderTarget,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE iprimitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE istripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED) noexcept
        :
        inputLayout{},
        blendDesc(blend),
        depthStencilDesc(depthStencil),
        rasterizerDesc(rasterizer),
        renderTargetState(renderTarget),
        primitiveTopology(iprimitiveTopology),
        stripCutValue(istripCutValue)
    {
        if (iinputLayout)
            this->inputLayout = *iinputLayout;
    }

    void CreatePipelineState(
        _In_ ID3D12Device* device,
        _In_ ID3D12RootSignature* rootSignature,
        const D3D12_SHADER_BYTECODE& HullShader,
        const D3D12_SHADER_BYTECODE& DomainShader,
        _Outptr_ ID3D12PipelineState** pPipelineState) const

    {
        auto psoDesc = GetDesc();
        psoDesc.pRootSignature = rootSignature;
        psoDesc.HS = HullShader;
        psoDesc.DS = DomainShader;

        HRESULT hr = device->CreateGraphicsPipelineState(
            &psoDesc,
            IID_GRAPHICS_PPV_ARGS(pPipelineState));

        if (FAILED(hr))
        {

            throw std::runtime_error("ERROR: CreatePipelineState failed to create a PSO. Enable the Direct3D Debug Layer for more information");
        }
    }
};

