//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <memory>


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;
enum Descriptors
{
	WindowsLogo,
	CourierFont,
	ControllerFont,
	GamerPic,
	Count
};
Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    hwnd = window;
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources(hwnd);
    CreateDeviceDependentResources(window);

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_gamePad = std::make_unique<GamePad>();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
    
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());
    
    // TODO: Add your game logic here.
    

	m_animation.Update(elapsedTime);

	
	
	float time = float(timer.GetTotalSeconds());
   
	//m_Skinnedcharacterworld = XMMatrixRotationY(time);
  
	

   
	

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{

	auto size = m_deviceResources->GetOutputSize();
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }
   
	
    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");
   

    // TODO: Add your rendering code here.
	size_t nbones = m_skinnedcharacter->m_Model->bones.size();
	/*
    //Imgui‚ÍV‚µ‚¢ƒtƒŒ[ƒ€‚Å‚Ì•`‰æ‚É”õ‚¦‚é
	m_imguicore.ImguiCore_Tick();
	ID3D12DescriptorHeap* imguiheap[] = { imguidescriptorheap->Heap() };
	imguiheap[0] = imguidescriptorheap->Heap();

	commandList->SetDescriptorHeaps(1, imguiheap);
    //GUIPanel‚ð•`‰æ‚·‚é
    //c_cameraPos = { camx,camy,camz,0 };

    	m_imguicore.RenderMainpanel(m_deviceResources.get(), &m_skinnedcharacter->m_transform.position.x,
		&m_skinnedcharacter->m_transform.position.y,
		&m_skinnedcharacter->m_transform.position.z);
    m_imguicore.RenderNodes(m_deviceResources.get());
    m_imguicore.Render_AllGUI(m_deviceResources.get());
    */
    m_imguicore.ImguiCore_Tick();
	m_animation.Apply(*m_skinnedcharacter->m_Model, nbones, m_drawBones.get());
    ImGui::Begin("Rendering Test Menu");
    ImGui::SetWindowSize(ImVec2(400, 500),
        ImGuiCond_::ImGuiCond_FirstUseEver);
	ID3D12DescriptorHeap* heaps[] = { m_modelResources->Heap(),
		m_states->Heap()};
	
	commandList->SetDescriptorHeaps(
		static_cast<UINT>(std::size(heaps)), heaps);
    
	m_Skinnedcharacterworld = DirectX::XMMatrixTranslation(m_skinnedcharacter->m_transform.position.x,
		m_skinnedcharacter->m_transform.position.y,
		m_skinnedcharacter->m_transform.position.z);

  
    //m_camera.m_transform.position = c_cameraPos;
	Model::UpdateEffectMatrices(m_modelNormal, m_Skinnedcharacterworld, m_camera.m_view, m_camera.m_proj);
  
     m_skinnedcharacter->m_Model->DrawSkinned(commandList, nbones, m_drawBones.get(),
        m_Skinnedcharacterworld, m_modelNormal.cbegin());
     ImGui::End();

    
    //m_imguicore.ImguiCore_Tick();
    ID3D12DescriptorHeap* imguiheap[] = { imguidescriptorheap->Heap() };
    imguiheap[0] = imguidescriptorheap->Heap();

    commandList->SetDescriptorHeaps(1, imguiheap);
    //GUIPanel‚ð•`‰æ‚·‚é
    //c_cameraPos = { camx,camy,camz,0 };

    m_imguicore.RenderMainpanel(m_deviceResources.get(), &m_skinnedcharacter->m_transform.position.x,
        &m_skinnedcharacter->m_transform.position.y,
        &m_skinnedcharacter->m_transform.position.z);
    m_imguicore.RenderNodes(m_deviceResources.get());
    m_imguicore.Render_AllGUI(m_deviceResources.get());

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
     m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());

    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto viewport = m_deviceResources->GetScreenViewport();
    auto scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
    m_gamePad->Resume();
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    m_gamePad->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 800;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources(HWND in_hwnd)
{
    auto device = m_deviceResources->GetD3DDevice();

    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
     m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    // TODO: Initialize device dependent objects here (independent of window size).
	 m_states = std::make_unique<CommonStates>(device);

	
     m_skinnedcharacter = std::make_unique<SkinnedCharacter>();
    
	 m_skinnedcharacter->m_Model = Model::CreateFromSDKMESH(device, L"soldier.sdkmesh",
		 ModelLoader_IncludeBones);
    
	 DX::ThrowIfFailed(
		 m_animation.Load(L"soldier.sdkmesh_anim")
	 );
    
    
     imguidescriptorheap = std::make_unique<DescriptorHeap>(device, Descriptors::Count);
     imguidescriptorheap->Heap()->SetName(L"ImguiHeap");
     m_imguicore = ImguiCore(hwnd, m_deviceResources->GetD3DDevice(),*imguidescriptorheap,imguidescriptorheap->GetFirstCpuHandle(),imguidescriptorheap->GetFirstGpuHandle()
         );

     m_skinnedcharacter->m_Model->materials[0].diffuseTextureIndex = 0;
     m_skinnedcharacter->m_Model->materials[0].samplerIndex = static_cast<int>(
		 CommonStates::SamplerIndex::AnisotropicClamp);
     

	
	 m_animation.Bind(*m_skinnedcharacter->m_Model);
    
	 m_drawBones = ModelBone::MakeArray(m_skinnedcharacter->m_Model->bones.size());
	 const auto& cull = CommonStates::CullClockwise; 
	 ResourceUploadBatch resourceUpload(device);

	 resourceUpload.Begin();

     m_skinnedcharacter->m_Model->LoadStaticBuffers(device, resourceUpload);

	 m_modelResources = m_skinnedcharacter->m_Model->LoadTextures(device, resourceUpload);
	
	 m_fxFactory = std::make_unique<EffectFactory>(m_modelResources->Heap(),
		 m_states->Heap());

	 auto uploadResourcesFinished = resourceUpload.End(
		 m_deviceResources->GetCommandQueue());

	 uploadResourcesFinished.wait();

	 RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
		 m_deviceResources->GetDepthBufferFormat());

	 EffectPipelineStateDescription pd(
		 nullptr,
		 CommonStates::Opaque,
		 CommonStates::DepthDefault,
		 cull,
		 rtState);

	
  
	 m_modelNormal = m_skinnedcharacter->m_Model->CreateEffects(*m_fxFactory, pd, pd);
     
	
   
       
     m_Skinnedcharacterworld = Matrix::Identity;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    
	
	static const XMVECTORF32 c_cameraPos = { 0.f, 0.f, 1.5f, 0.f };
	static const XMVECTORF32 c_lookAt = { 0.f, 0.25f, 0.f, 0.f };
    m_camera.m_transform.position = c_cameraPos;
    auto size = m_deviceResources->GetOutputSize();
	m_camera.m_view = Matrix::CreateLookAt(m_camera.m_transform.position, c_lookAt.v, Vector3::UnitY);
    m_camera.m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(size.right) / float(size.bottom), 0.1f, 1000.f);

	

    
	
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_skinnedcharacter.reset();
	m_imguicore.EndRenderImguicore();
    imguidescriptorheap.reset();
    // If using the DirectX Tool Kit for DX12, uncomment this line:
     m_graphicsMemory.reset();
	 m_states.reset();
	 m_fxFactory.reset();
	 m_modelResources.reset();
     m_skinnedcharacter->m_Model.reset();
	 m_modelNormal.clear();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources(hwnd);

    CreateWindowSizeDependentResources();
   
}
#pragma endregion
