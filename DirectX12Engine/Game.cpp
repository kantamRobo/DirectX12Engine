//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false):
	m_retryAudio(false)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);

	m_hdrScene = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_skinnedCharacter = std::make_unique<SkinnedCharacter>();
	m_skinnedCharacter->m_transform.position = DirectX::SimpleMath::Vector3(0, 0, 0);
	m_skinnedCharacter->m_transform.rotation = DirectX::SimpleMath::Quaternion(0, 0, 0, 0);
	XMVECTORF32 color;
	color.v = XMColorSRGBToRGB(Colors::CornflowerBlue);
	m_hdrScene->SetClearColor(color);
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
	m_nightLoop.reset();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();
	// Setup Platform/Renderer backends
	
	const int NUM_FRAMES_IN_FLIGHT = 3;
	m_ImguiresourceDescriptors = std::make_unique<DescriptorHeap>(m_deviceResources->GetD3DDevice(), Descriptors::Count);
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(m_deviceResources->GetD3DDevice(), NUM_FRAMES_IN_FLIGHT,
		DXGI_FORMAT_R8G8B8A8_UNORM, m_ImguiresourceDescriptors->Heap(),
		m_ImguiresourceDescriptors->GetFirstCpuHandle(),
		m_ImguiresourceDescriptors->GetFirstGpuHandle());
	
	m_explode = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"explo1.wav");
	m_ambient = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"NightAmbienceSimple_02.wav");

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);
	std::random_device rd;
	m_random = std::make_unique<std::mt19937>(rd());

	explodeDelay = 2.f;


	m_nightLoop = m_ambient->CreateInstance();
	m_nightLoop->Play(true);
	nightVolume = 1.f;
	nightSlide = -0.1f;
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
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.



	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);



	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color



	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();

	ImGui::Text("counter = %d", counter);



	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	ImGui::Render();
    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.

	m_animation.Update(elapsedTime);

	auto time = static_cast<float>(timer.GetTotalSeconds());
	

	m_world = DirectX::SimpleMath::Matrix::CreateScale(1.0f)* DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_skinnedCharacter->m_transform.rotation)* DirectX::SimpleMath::Matrix::CreateTranslation(m_skinnedCharacter->m_transform.position);

	//m_world = XMMatrixRotationY(time);
	if (m_retryAudio)
	{
		m_retryAudio = false;
		if (m_audEngine->Reset())
		{
			// TODO: restart any looped sounds here
			if (m_nightLoop)
				m_nightLoop->Play(true);
		}
	}
	else if (!m_audEngine->Update())
	{
		if (m_audEngine->IsCriticalError())
		{
			m_retryAudio = true;
		}
	}
	explodeDelay -= elapsedTime;
	if (explodeDelay < 0.f)
	{
		m_explode->Play();

		std::uniform_real_distribution<float> dist(1.f, 10.f);
		explodeDelay = dist(*m_random);
	}

	nightVolume += elapsedTime * nightSlide;
	if (nightVolume < 0.f)
	{
		nightVolume = 0.f;
		nightSlide = -nightSlide;
	}
	else if (nightVolume > 1.f)
	{
		nightVolume = 1.f;
		nightSlide = -nightSlide;
	}
	m_nightLoop->SetVolume(nightVolume);

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	ImGui::Render();

    // Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}


    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
	auto commandList = m_deviceResources->GetCommandList();
	m_hdrScene->BeginScene(commandList);

	Clear();

    
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here.

	size_t nbones = m_model->bones.size();

	m_animation.Apply(*m_skinnedCharacter->m_Model.get(), nbones, m_drawBones.get());

	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
	commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	m_effect->SetWorld(m_world);
	m_effect->Apply(commandList);
	m_shape->Draw(commandList);


	m_hdrScene->EndScene(commandList);


	auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	m_toneMap->Process(commandList);

	Model::UpdateEffectMatrices(m_modelNormal, m_world,  m_camera.m_view, m_camera.m_proj);

	m_skinnedCharacter->m_Model->DrawSkinned(commandList, nbones, m_drawBones.get(),
		m_world, m_modelNormal.cbegin());



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
	auto rtvDescriptor = m_renderDescriptors->GetCpuHandle(RTDescriptors::HDRScene);
	auto dsvDescriptor = m_deviceResources->GetDepthStencilView();
	auto rtvDescriptor = m_renderDescriptors->GetCpuHandle(RTDescriptors::HDRScene);


	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	m_hdrScene->Clear(commandList);

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
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
	m_audEngine->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
	explodeDelay = 2.f;
    // TODO: Game is being power-resumed (or returning from minimize).
	m_audEngine->Resume();
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
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
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
	   m_shape = GeometricPrimitive::CreateSphere();

	   m_resourceDescriptors = std::make_unique<DescriptorHeap>(device,
		   Descriptors::Count);
	   m_renderDescriptors = std::make_unique<DescriptorHeap>(device,
		   D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		   D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		   RTDescriptors::RTCount);
	   {
		   EffectPipelineStateDescription pd(
			   &GeometricPrimitive::VertexType::InputLayout,
			   CommonStates::Opaque,
			   CommonStates::DepthDefault,
			   CommonStates::CullCounterClockwise,
			   hdrState);

		   m_effect = std::make_unique<PBREffect>(device,
			   EffectFlags::Texture | EffectFlags::Emissive, pd);

	   }
	   ResourceUploadBatch resourceUpload(device);

	   resourceUpload.Begin();

	   m_shape->LoadStaticBuffers(device, resourceUpload);

	   // Image-based lighting cubemaps.
	   DX::ThrowIfFailed(
		   CreateDDSTextureFromFile(device, resourceUpload,
			   L"SunSubMixer_diffuseIBL.dds",
			   m_radiance.ReleaseAndGetAddressOf()));
	   m_resourceDescriptors->GetCpuHandle(Descriptors::IrradianceIBL), true);

	   auto uploadResourcesFinished = resourceUpload.End(
		   m_deviceResources->GetCommandQueue());

	   uploadResourcesFinished.wait();

	   auto desc = m_radiance->GetDesc();

	   auto radianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::RadianceIBL);
	   auto irradianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::IrradianceIBL);

	   m_effect->SetIBLTextures(radianceTex, desc.MipLevels, irradianceTex, m_states->AnisotropicClamp());

	   CreateShaderResourceView(device, m_radiance.Get(),
		   m_resourceDescriptors->GetCpuHandle(Descriptors::RadianceIBL));

	   DX::ThrowIfFailed(
		   CreateDDSTextureFromFile(device, resourceUpload,
			   L"SunSubMixer_specularIBL.dds",
			   m_irradiance.ReleaseAndGetAddressOf()));

	   CreateShaderResourceView(device, m_irradiance.Get(),

		   DX::ThrowIfFailed(
			   CreateWICTextureFromFile(device, resourceUpload,
				   L"Sphere2Mat_baseColor.png",
				   m_albedoMap.ReleaseAndGetAddressOf())));

	   CreateShaderResourceView(device, m_albedoMap.Get(),
		   m_resourceDescriptors->GetCpuHandle(Descriptors::AlbedoMap));

	   DX::ThrowIfFailed(
		   CreateWICTextureFromFile(device, resourceUpload,
			   L"Sphere2Mat_normal.png",
			   m_normalMap.ReleaseAndGetAddressOf()));

	   CreateShaderResourceView(device, m_normalMap.Get(),
		   m_resourceDescriptors->GetCpuHandle(Descriptors::NormalMap));

	   DX::ThrowIfFailed(
		   CreateWICTextureFromFile(device, resourceUpload,
			   L"Sphere2Mat_occlusionRoughnessMetallic.png",
			   m_rmaMap.ReleaseAndGetAddressOf()));

	   CreateShaderResourceView(device, m_rmaMap.Get(),
		   m_resourceDescriptors->GetCpuHandle(Descriptors::RMAMap));

	   DX::ThrowIfFailed(
		   CreateWICTextureFromFile(device, resourceUpload,
			   L"Sphere2Mat_emissive.png",
			   m_emissiveMap.ReleaseAndGetAddressOf()));

	   CreateShaderResourceView(device, m_emissiveMap.Get(),
		   m_resourceDescriptors->GetCpuHandle(Descriptors::EmissiveMap));


	   m_hdrScene->SetDevice(device,
		   m_resourceDescriptors->GetCpuHandle(Descriptors::SceneTex),
		   m_renderDescriptors->GetCpuHandle(RTDescriptors::HDRScene));

	   RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
		   DXGI_FORMAT_UNKNOWN);

	   m_toneMap = std::make_unique<ToneMapPostProcess>(device,
		   rtState,
		   ToneMapPostProcess::Reinhard, ToneMapPostProcess::SRGB);

	   RenderTargetState hdrState(m_hdrScene->GetFormat(),
		   m_deviceResources->GetDepthBufferFormat());

	   m_effect->SetSurfaceTextures(
		   m_resourceDescriptors->GetGpuHandle(Descriptors::AlbedoMap),
		   m_resourceDescriptors->GetGpuHandle(Descriptors::NormalMap),
		   m_resourceDescriptors->GetGpuHandle(Descriptors::RMAMap),
		   m_states->AnisotropicClamp());

	   m_effect->SetEmissiveTexture(
		   m_resourceDescriptors->GetGpuHandle(Descriptors::EmissiveMap));

	   
	   m_skinnedCharacter->m_Model = Model::CreateFromSDKMESH(device, L"soldier.sdkmesh",
		   ModelLoader_IncludeBones);

	   DX::ThrowIfFailed(
		   m_animation.Load(L"soldier.sdkmesh_anim")
	   );
	   m_animation.Bind(*m_model);

	   m_drawBones = ModelBone::MakeArray(m_model->bones.size());

	   const auto& cull = CommonStates::CullClockwise;

	   ResourceUploadBatch resourceUpload(device);
	   const auto& cull = CommonStates::CullCounterClockwise;

	   m_drawBones = ModelBone::MakeArray(m_model->bones.size());

	

	   resourceUpload.Begin();

	   m_model->LoadStaticBuffers(device, resourceUpload);

	   m_modelResources = m_model->LoadTextures(device, resourceUpload);

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

	   m_modelNormal =m_skinnedCharacter->m_Model->CreateEffects(*m_fxFactory, pd, pd);

	 




}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

	static const XMVECTORF32 c_cameraPos = { 0.f, 0.f, 1.5f, 0.f };
	static const XMVECTORF32 c_lookAt = { 0.f, 0.25f, 0.f, 0.f };

	auto size = m_deviceResources->GetOutputSize();

	m_camera.m_transform.position = DirectX::SimpleMath::Vector3(0.f, 0.f, 1.5f);
	static const DirectX::XMVECTORF32 c_lookAt = { 0.f, 0.25f, 0.f, 0.f };
	m_camera.m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_camera.m_transform.position, c_lookAt.v, DirectX::SimpleMath::Vector3::UnitY);
	m_camera.m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(size.right) / float(size.bottom), 0.1f, 1000.f);


	m_effect->SetView(m_camera.m_view);
	m_effect->SetProjection(m_camera.m_proj);


	m_hdrScene->SetWindow(size);
	auto sceneTex = m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex);
	m_toneMap->SetHDRSourceTexture(sceneTex);

}

void Game::OnDeviceLost()
{ // Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
    // TODO: Add Direct3D resource cleanup here.
	m_states.reset();
	m_fxFactory.reset();
	m_shape.reset();
	m_effect.reset();
	m_radiance.Reset();
	m_irradiance.Reset();
	m_hdrScene->ReleaseDevice();
	m_toneMap.reset();
	m_resourceDescriptors.reset();
	m_renderDescriptors.reset();
	m_albedoMap.Reset();
	m_normalMap.Reset();
	m_rmaMap.Reset();
	m_emissiveMap.Reset();

	m_modelResources.reset();
	m_model.reset();
	m_modelNormal.clear();


    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

