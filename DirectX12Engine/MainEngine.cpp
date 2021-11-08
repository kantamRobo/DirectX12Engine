#include "MainEngine.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "ModelRendererWorker.h"
#include "DescriptorHeap.h"
void MainEngine::Init(HWND hwnd)
{
	
	
	
	m_core = std::make_shared<DX12EngineCore>(hwnd);
	DescriptorHeapsContainer tempdes{ m_core->m_heapDsv,m_core->m_heapRtv };
	DescriptorHeap CBV_SRVHeaps;
	CBV_SRVHeaps.PrepareCBV_SRVHeaps(m_core->m_device.Get());
	m_rendererworker = std::make_shared<ModelRendererWorker>();
	Commands tempcommands{ m_rendererworker->m_commandAllocators,m_rendererworker->m_commandList,m_rendererworker->m_commandQueue };
	std::shared_ptr<Model> temp_model = std::make_shared<Model>(m_core->m_device, tempcommands, "Hoge", m_core->m_swapchain->GetCurrentBackBufferIndex());
	std::shared_ptr<Camera> maincamera = std::make_shared<Camera>(m_core->m_viewport,m_core->m_scissorRect,temp_model->modelmat);
	m_renderer = std::make_shared<ModelRenderer>(m_core, tempdes, tempcommands, temp_model);
	m_renderer->SetSwapchain(m_core->m_swapchain.Get());
	m_renderer->SetViewPort_Rect(m_core->m_viewport, m_core->m_scissorRect);
	m_renderer->SetFrameFences(m_core->m_frameFences);
	m_renderer->SetFrameFenceValues(m_core->m_frameFenceValues);
	

	


}

