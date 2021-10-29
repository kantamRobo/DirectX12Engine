#pragma once
#include <d3d12.h>
#include <memory>
class ModelRenderer;
class ModelRendererWorker;
class DX12EngineCore;
class MainEngine
{
public:
	
	
	void Init(HWND hwnd);
	void Tick();
	std::shared_ptr<DX12EngineCore> m_core;
	std::shared_ptr<ModelRenderer>m_renderer;
	std::shared_ptr<ModelRendererWorker>m_rendererworker;
	
};

