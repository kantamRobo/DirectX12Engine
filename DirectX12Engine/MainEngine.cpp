#include "MainEngine.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "Camera.h"
void MainEngine::Init(HWND hwnd)
{
	
	
	
	m_core = std::make_shared<DX12EngineCore>(hwnd);
	
	DescriptorHeapsContainer ModelRendererDescriptors;
	ModelRendererDescriptors.CreateRTVHeap(m_core->m_device.Get(), 2);
	m_core->CreateRenderTargetView(ModelRendererDescriptors.m_heapRtv);
	m_core->CreateFrameFences();
	m_core->CreateCommandAllocators();
	m_core->CreateCommandLists();
	
	ModelRendererDescriptors.CreateSceneCBVHeaps(m_core->m_device.Get());
	ModelRendererDescriptors.CreateRTVHeap(m_core->m_device.Get(), m_core->FrameBufferCount);
	ModelRendererDescriptors.CreateDSVHeap(m_core->m_device.Get());
	m_core->CreateDepthBuffer(ModelRendererDescriptors.m_HeapDsv);
	
	Commands commands{ m_core->m_commandAllocators,m_core->m_commandList,m_core->m_commandQueue };
	
	std::shared_ptr<Model> temp_model = std::make_shared<Model>(m_core, commands, "Hoge",ModelRendererDescriptors);
	
	
	m_renderer = std::make_shared<ModelRenderer>(m_core, 
		commands, temp_model, 
		 ModelRendererDescriptors,temp_model->m_modelMaterial);
	 maincamera = std::make_shared<Camera>(m_core, DirectX::XMLoadFloat4x4(&temp_model->modelmat), &m_renderer->shaderParams);
	m_core->m_commandList->Close();
	m_renderer->WaitPreviousFrame();


	


}

void MainEngine::Tick()
{
	m_renderer->Render(maincamera);
}