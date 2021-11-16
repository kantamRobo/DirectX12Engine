#include "MainEngine.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "ModelRendererWorker.h"
void MainEngine::Init(HWND hwnd)
{
	
	
	
	m_core = std::make_shared<DX12EngineCore>(hwnd);

	DescriptorHeapsContainer ModelRendererDescriptors;
	ModelRendererDescriptors.CreateRTVHeap(m_core->m_device.Get(), 2);
	ModelRendererDescriptors.CreateSceneCBVHeaps(m_core->m_device.Get());
	Commands commands{ m_core->m_commandAllocators,m_core->m_commandList,m_core->m_commandQueue };
	
	std::shared_ptr<Model> temp_model = std::make_shared<Model>(m_core, commands, "Hoge");
	std::shared_ptr<Camera> maincamera = std::make_shared<Camera>(m_core,temp_model->modelmat);
	
	m_renderer = std::make_shared<ModelRenderer>(m_core, 
		commands, temp_model, m_rendererworker, 
		ModelRendererDescriptors,Heapsworker);
	
	

	


}

