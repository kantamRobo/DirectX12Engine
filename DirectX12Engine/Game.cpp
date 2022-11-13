//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <memory>
#include <Effects.h>

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
Game::Game() noexcept(false):m_retryAudio(false)
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

    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{

    DirectX::RenderTargetState targetstate;
    hwnd = window;
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources(hwnd);
    CreateDeviceDependentResources(window);

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_gamePad = std::make_unique<GamePad>();
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    m_audEngine = std::make_unique<AudioEngine>(eflags);

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
    if (m_retryAudio)
    {
        m_retryAudio = false;
        if (m_audEngine->Reset())
        {
            // TODO: restart any looped sounds here
        }
    }
    else if (!m_audEngine->Update())
    {
        if (m_audEngine->IsCriticalError())
        {
            m_retryAudio = true;
        }
    }
	

   
	

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
	

	/*
    * size_t nbones = m_skinnedcharacter->m_Model->bones.size();
    //Imgui�͐V�����t���[���ł̕`��ɔ�����
	m_imguicore.ImguiCore_Tick();
	ID3D12DescriptorHeap* imguiheap[] = { imguidescriptorheap->Heap() };
	imguiheap[0] = imguidescriptorheap->Heap();

	commandList->SetDescriptorHeaps(1, imguiheap);
    //GUIPanel��`�悷��
    //c_cameraPos = { camx,camy,camz,0 };

    	m_imguicore.RenderMainpanel(m_deviceResources.get(), &m_skinnedcharacter->m_transform.position.x,
		&m_skinnedcharacter->m_transform.position.y,
		&m_skinnedcharacter->m_transform.position.z);
    m_imguicore.RenderNodes(m_deviceResources.get());
    m_imguicore.Render_AllGUI(m_deviceResources.get());
    
    m_terrein->DrawTerrein(commandList, m_camera,m_effect);
    */
    //m_imguicore.ImguiCore_Tick();
	//m_animation.Apply(*m_skinnedcharacter->m_Model, nbones, m_drawBones.get());
    /*
    ImGui::Begin("Rendering Test Menu");
    ImGui::SetWindowSize(ImVec2(400, 500),
        ImGuiCond_::ImGuiCond_FirstUseEver);
        */
	/*
    ID3D12DescriptorHeap* heaps[] = { m_modelResources->Heap(),
		m_states->Heap()};
	*/
    /*
	commandList->SetDescriptorHeaps(
		static_cast<UINT>(std::size(heaps)), heaps);
    */
    /*
	m_Skinnedcharacterworld = DirectX::XMMatrixTranslation(m_skinnedcharacter->m_transform.position.x,
		m_skinnedcharacter->m_transform.position.y,
		m_skinnedcharacter->m_transform.position.z);
        */
  
    //m_camera.m_transform.position = c_cameraPos;
	//Model::UpdateEffectMatrices(m_modelNormal, m_Skinnedcharacterworld, m_camera.m_view, m_camera.m_proj);
  /*
     m_skinnedcharacter->m_Model->DrawSkinned(commandList, nbones, m_drawBones.get(),
        m_Skinnedcharacterworld, m_modelNormal.cbegin());
     ImGui::End();
     */
    
    //m_imguicore.ImguiCore_Tick();
   /*
    ID3D12DescriptorHeap* imguiheap[] = { imguidescriptorheap->Heap() };
    imguiheap[0] = imguidescriptorheap->Heap();
    */
    
    
    //commandList->SetDescriptorHeaps(1, imguiheap);
    
    
    //GUIPanel��`�悷��
    //c_cameraPos = { camx,camy,camz,0 };

    /*
    m_imguicore.RenderMainpanel(m_deviceResources.get(), &m_skinnedcharacter->m_transform.position.x,
        &m_skinnedcharacter->m_transform.position.y,
        &m_skinnedcharacter->m_transform.position.z);
    m_imguicore.RenderNodes(m_deviceResources.get());
    m_imguicore.Render_AllGUI(m_deviceResources.get());
    */
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
    m_audEngine->Suspend();

}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
    m_audEngine->Resume();
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
    //TODO(2022/06)�����_�����O�����́A�����ɂׂ����������A�����_�����O�����ꂼ��́i���Ƃ��΃A�j���[�V�����A�W�I���g���A���C�e�B���O�j�N���X
     //�ɏ����A���̎O��Init��Render�������ɏ����B

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
    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
        m_deviceResources->GetDepthBufferFormat());
    m_graphicsMemory = std::make_shared<GraphicsMemory>(device);
   // m_effect = std::make_shared<DirectX::BasicEffect>(device, 0, pipeline);

    m_terrein = std::make_unique<Terrein>(m_effect->m_basicffectrootsignature);
    m_terrein->Preparepatch(device, rtState, std::move(m_deviceResources), m_graphicsMemory);

   


    TesselationEffectPipelineDescription pipeline(&m_terrein->m_input, DirectX::CommonStates::Opaque,
        DirectX::CommonStates::DepthDefault,
        DirectX::CommonStates::CullCounterClockwise,
        rtState);
    
    HullShader.LoadShader(L"HSterrein.hlsl", L"MainHS");


    DomainShader.LoadShader(L"DSterrein.hlsl", L"MainDS");


    VertexShader.LoadShader(L"VSterrein.hlsl", L"MainVS");


    PixelShader.LoadShader(L"PSterrein.hlsl", L"MainPS");

    D3D12_SHADER_BYTECODE patchVS = {};
    D3D12_SHADER_BYTECODE patchPS = {};
    D3D12_SHADER_BYTECODE patchHS = {};
    D3D12_SHADER_BYTECODE patchDS = {};

    patchVS.BytecodeLength = VertexShader.dxLibdesc.DXILLibrary.BytecodeLength;
    patchVS.pShaderBytecode = VertexShader.dxLibdesc.DXILLibrary.pShaderBytecode;

    patchPS.BytecodeLength = PixelShader.dxLibdesc.DXILLibrary.BytecodeLength;
    patchPS.pShaderBytecode = PixelShader.dxLibdesc.DXILLibrary.pShaderBytecode;

    patchHS.BytecodeLength = HullShader.dxLibdesc.DXILLibrary.BytecodeLength;
    patchHS.pShaderBytecode = HullShader.dxLibdesc.DXILLibrary.pShaderBytecode;

    patchDS.BytecodeLength = DomainShader.dxLibdesc.DXILLibrary.BytecodeLength;
    patchDS.pShaderBytecode = DomainShader.dxLibdesc.DXILLibrary.pShaderBytecode;




    D3D12_DXIL_LIBRARY_DESC dxLibhulldesc;
    IDxcBlob* pBlobhull = HullShader.GetCompiledDxcBlob();
    dxLibhulldesc.DXILLibrary.pShaderBytecode = pBlobhull->GetBufferPointer();
    dxLibhulldesc.DXILLibrary.BytecodeLength = pBlobhull->GetBufferSize();
    dxLibhulldesc.NumExports = 1;
    dxLibhulldesc.pExports = &HullShader.libExport;


    D3D12_DXIL_LIBRARY_DESC dxLibdomaindesc;
    IDxcBlob* pBlobdomain = DomainShader.GetCompiledDxcBlob();
    dxLibdomaindesc.DXILLibrary.pShaderBytecode = pBlobdomain->GetBufferPointer();
    dxLibdomaindesc.DXILLibrary.BytecodeLength = pBlobdomain->GetBufferSize();
    dxLibdomaindesc.NumExports = 1;
    dxLibdomaindesc.pExports = &DomainShader.libExport;

    D3D12_DXIL_LIBRARY_DESC dxLibvertexdesc;
    IDxcBlob* pBlobvertex = VertexShader.GetCompiledDxcBlob();
    dxLibhulldesc.DXILLibrary.pShaderBytecode = pBlobvertex->GetBufferPointer();
    dxLibhulldesc.DXILLibrary.BytecodeLength = pBlobvertex->GetBufferSize();
    dxLibhulldesc.NumExports = 1;
    dxLibhulldesc.pExports = &VertexShader.libExport;


    D3D12_DXIL_LIBRARY_DESC dxLibpixeldesc;
    IDxcBlob* pBlobpixel = PixelShader.GetCompiledDxcBlob();
    dxLibdomaindesc.DXILLibrary.pShaderBytecode = pBlobpixel->GetBufferPointer();
    dxLibdomaindesc.DXILLibrary.BytecodeLength = pBlobpixel->GetBufferSize();
    dxLibdomaindesc.NumExports = 1;
    dxLibdomaindesc.pExports = &PixelShader.libExport;

    pipeline.CreatePipelineState(device, m_terrein->m_patchrootsignature.Get()
        , patchVS, patchPS, patchDS, patchHS, m_terrein->m_patchpipelinestate.GetAddressOf());

    
     
  
  
    // TODO: Initialize device dependent objects here (independent of window size).
	 m_states = std::make_unique<CommonStates>(device);
     
  
     //�n�C�g�}�b�v�ƃm�[�}���}�b�v���Z�b�g����
     m_effect->SetTexture(m_terrein->m_heightmapheap->GetGpuHandle(Descriptors::Count), m_states->LinearClamp());
     m_effect->SetTexture(m_terrein->m_normalmapheap->GetGpuHandle(Descriptors::Count), m_states->LinearClamp());
	
     
     
     /*
     m_skinnedcharacter = std::make_unique<SkinnedCharacter>();
    
	 m_skinnedcharacter->m_Model = Model::CreateFromSDKMESH(device, L"soldier.sdkmesh",
		 ModelLoader_IncludeBones);
         */
    /*
	 DX::ThrowIfFailed(
		 m_animation.Load(L"soldier.sdkmesh_anim")
	 );
     */
    
    /*
     imguidescriptorheap = std::make_unique<DescriptorHeap>(device, Descriptors::Count);
     imguidescriptorheap->Heap()->SetName(L"ImguiHeap");
     m_imguicore = ImguiCore(hwnd, m_deviceResources->GetD3DDevice(),*imguidescriptorheap,imguidescriptorheap->GetFirstCpuHandle(),imguidescriptorheap->GetFirstGpuHandle()
         );
         */
     /*
     m_skinnedcharacter->m_Model->materials[0].diffuseTextureIndex = 0;
     m_skinnedcharacter->m_Model->materials[0].samplerIndex = static_cast<int>(
		 CommonStates::SamplerIndex::AnisotropicClamp);
     */

	
	 //m_animation.Bind(*m_skinnedcharacter->m_Model);
    /*
	 m_drawBones = ModelBone::MakeArray(m_skinnedcharacter->m_Model->bones.size());
	 const auto& cull = CommonStates::CullClockwise; 
	*/
    ResourceUploadBatch resourceUpload(device);
     
	 resourceUpload.Begin();

     //m_skinnedcharacter->m_Model->LoadStaticBuffers(device, resourceUpload);

	// m_modelResources = m_skinnedcharacter->m_Model->LoadTextures(device, resourceUpload);
	/*
	 m_fxFactory = std::make_unique<EffectFactory>(m_modelResources->Heap(),
		 m_states->Heap());
         */
     
	 auto uploadResourcesFinished = resourceUpload.End(
		 m_deviceResources->GetCommandQueue());

	 uploadResourcesFinished.wait();

	
   

     /*
	 EffectPipelineStateDescription pd(
		 nullptr,
		 CommonStates::Opaque,
		 CommonStates::DepthDefault,
		 cull,
		 rtState);
         */
	
  
	 //m_modelNormal = m_skinnedcharacter->m_Model->CreateEffects(*m_fxFactory, pd, pd);
    
     //m_Skinnedcharacterworld = Matrix::Identity;
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
