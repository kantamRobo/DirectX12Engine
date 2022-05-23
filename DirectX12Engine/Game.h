//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Character.h"
#include "Camera.h"
#include "ImguiCore.h"
#include "Rigidshape.h"
#include "PlanePolygon.h"
#include "GraphicsMemory.h"
#include "Terrein.h"
#include <DirectXMath.h>
#include <Dbt.h>
#include <ksmedia.h>


// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);
    void OnNewAudioDevice() noexcept { m_retryAudio = true; }
    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;
    bool m_retryAudio;
private:
    
	DirectX::SimpleMath::Matrix m_Skinnedcharacterworld;
    
    float camx = 0;
    float camy = 0;
    float camz = 0;
    
    HWND hwnd;
    Camera m_camera;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
    std::unique_ptr<SkinnedCharacter> m_skinnedcharacter;
    std::unique_ptr<DirectX::DescriptorHeap> imguidescriptorheap;
    std::unique_ptr<DirectX::GamePad> m_gamePad;
    ImguiCore m_imguicore;
	DirectX::Model::EffectCollection m_modelNormal;
    DX::AnimationSDKMESH m_animation;
	DirectX::ModelBone::TransformArray m_drawBones;

    std::shared_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
    std::unique_ptr<DirectX::AudioEngine> m_audEngine;
   
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

  
    
    void CreateDeviceDependentResources(HWND hwnd);
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    std::unique_ptr<Terrein> m_terrein;
    std::shared_ptr<DirectX::BasicEffect> m_effect;
    // If using the DirectX Tool Kit for DX12, uncomment this line:
    // std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
};
