#include "MainEngine.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "ModelRendererWorker.h"
void MainEngine::Init(HWND hwnd)
{
	m_core = std::make_shared<DX12EngineCore>(hwnd);
	m_rendererworker = std::make_shared<ModelRendererWorker>();
	m_renderer = std::make_shared<ModelRenderer>();

	m_renderer->SetSwapchain(m_core->m_swapchain.Get());
	DescriptorHeapsContainer tempdes{ m_core->m_heapDsv,m_core->m_heapRtv };
	m_renderer->SetFrameDSV_RTV(tempdes);
	m_renderer->SetFrameFences(m_core->m_frameFences);
	m_renderer->SetFrameFenceValues(m_core->m_frameFenceValues);
	Commands temp { m_rendererworker->m_commandAllocators,m_rendererworker->m_commandList,m_rendererworker->m_commandQueue };
	m_renderer->SetCommands(temp);


}

