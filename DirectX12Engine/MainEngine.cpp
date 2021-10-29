#include "MainEngine.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "ModelRendererWorker.h"
void MainEngine::Init(HWND hwnd)
{
	std::shared_ptr<Camera> maincamera = std::make_shared<Camera>();
	std::shared_ptr<Model> temp_model = std::make_shared<Model>();
	temp_model->Init("Hoge");
	m_core = std::make_shared<DX12EngineCore>(hwnd);
	DescriptorHeapsContainer tempdes{ m_core->m_heapDsv,m_core->m_heapRtv };
	m_rendererworker = std::make_shared<ModelRendererWorker>();
	Commands tempcommands{ m_rendererworker->m_commandAllocators,m_rendererworker->m_commandList,m_rendererworker->m_commandQueue };
	m_renderer = std::make_shared<ModelRenderer>(m_core,tempdes,tempcommands,temp_model);

	m_renderer->SetSwapchain(m_core->m_swapchain.Get());
	m_renderer->SetFrameFences(m_core->m_frameFences);
	m_renderer->SetFrameFenceValues(m_core->m_frameFenceValues);
	

	


}

