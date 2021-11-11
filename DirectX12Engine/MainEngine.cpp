#include "MainEngine.h"
#include "DX12EngineCore.h"
#include "ModelRenderer.h"
#include "ModelRendererWorker.h"
#include "DescriptorHeap.h"
void MainEngine::Init(HWND hwnd)
{
	
	
	
	m_core = std::make_shared<DX12EngineCore>(hwnd);
	DescriptorHeap Heapsworker;
	
	Heapsworker.CreateRTVHeap(m_core->m_device.Get());
	Heapsworker.CreateSceneCBVHeaps(m_core->m_device.Get());
	DescriptorHeapsContainer ModelRendererDescriptors{ Heapsworker.m_heapRtv,Heapsworker.m_heapSrv,Heapsworker.m_HeapDsv,Heapsworker.m_heapCbv,Heapsworker.m_heapUav,Heapsworker.m_HeapSampler };
	m_rendererworker = std::make_shared<ModelRendererWorker>(m_core->m_device,m_core->FrameBufferCount,Heapsworker);
	Commands tempcommands{ m_rendererworker->m_commandAllocators,m_rendererworker->m_commandList,m_rendererworker->m_commandQueue };
	std::shared_ptr<Model> temp_model = std::make_shared<Model>(m_core, tempcommands, "Hoge");
	std::shared_ptr<Camera> maincamera = std::make_shared<Camera>(m_core,temp_model->modelmat);
	
	m_renderer = std::make_shared<ModelRenderer>(m_core, tempcommands, temp_model, m_rendererworker, ModelRendererDescriptors);
	
	

	


}

