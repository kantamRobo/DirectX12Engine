#pragma once
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "d3dx12.h"
#include <windef.h>
#include "pch.h"

#include "DeviceResources.h"
class ImguiCore
{
	public:
		

		ImguiCore() {};
		
		
		ImguiCore(const HWND& hwnd, ID3D12Device* device,  const DirectX::DescriptorHeap& desc, D3D12_CPU_DESCRIPTOR_HANDLE srvcpuhandle, const D3D12_GPU_DESCRIPTOR_HANDLE& srvgpuhandle);
		void ImguiCore_Tick();

 
		void RenderMainpanel(DX::DeviceResources* deviceresources, float* x, float* y, float* z);
		void RenderNodes(DX::DeviceResources* deviceresources);
		void EndRenderImguicore();

};

