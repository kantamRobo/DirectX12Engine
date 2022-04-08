
#include "ImguiCore.h"
#include "imgui.h"
#include "imnodes.h"
#include "imnodes_internal.h"


ImguiCore::ImguiCore(const HWND& hwnd,ID3D12Device* device, const
	 DirectX::DescriptorHeap& desc,
	D3D12_CPU_DESCRIPTOR_HANDLE srvcpuhandle,
	const D3D12_GPU_DESCRIPTOR_HANDLE& srvgpuhandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();
	auto d3ddevice = device;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(d3ddevice, 3,
		DXGI_FORMAT_B8G8R8A8_UNORM, desc.Heap(),
		srvcpuhandle,
		srvgpuhandle);

}
void ImguiCore::ImguiCore_Tick()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	                         // Create a window {
	

}

bool show_demo_window = true;
bool show_another_window = false;
void ImguiCore::RenderMainpanel(DX::DeviceResources* deviceresources, float* x, float* y, float* z)
{
	
	ImGui::Begin("3DModel Position");
	ImGui::SetWindowSize(ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver);
	
	float min = 0;
	float max = 1.0f;
	ImGui::SliderFloat("x", x,min,0.5f);
	ImGui::SliderFloat("y", y, min, 0.5f);
	ImGui::SliderFloat("z", z, min, 0.5f);
	
	

	ImGui::End();
	ImGui::Begin("simple node editor");

	ImNodes::BeginNodeEditor();
	ImNodes::BeginNode(1);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("simple node :)");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(2);
	ImGui::Text("input");
	ImNodes::EndInputAttribute();

	ImNodes::BeginOutputAttribute(3);
	ImGui::Indent(40);
	ImGui::Text("output");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::EndNodeEditor();

	ImGui::End();
	
}

void ImguiCore::RenderNodes(DX::DeviceResources* deviceresources)
{
	
	

}

void ImguiCore::Render_AllGUI(DX::DeviceResources* deviceresources)
{
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), deviceresources->GetCommandList());
}

void ImguiCore::EndRenderImguicore()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

