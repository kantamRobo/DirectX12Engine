//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Animation.h"
#include "RenderTexture.h"
#include "Character.h"
#include "Camera.h"
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

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
	std::unique_ptr<DirectX::SoundEffectInstance> m_nightLoop;
	std::unique_ptr<std::mt19937> m_random;
	float explodeDelay;
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	bool m_retryAudio;
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	
	std::unique_ptr<DirectX::SoundEffect> m_explode;
	std::unique_ptr<DirectX::SoundEffect> m_ambient;
	std::unique_ptr<DirectX::CommonStates> m_states;
    std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
    std::unique_ptr<DirectX::PBREffect> m_effect;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_radiance;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_irradiance;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_albedoMap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_normalMap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_rmaMap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_emissiveMap;


	std::unique_ptr<DX::RenderTexture> m_hdrScene;
	std::unique_ptr<DirectX::ToneMapPostProcess> m_toneMap;
	std::unique_ptr<DirectX::DescriptorHeap> m_ImguiresourceDescriptors;
	std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
	std::unique_ptr<DirectX::DescriptorHeap> m_renderDescriptors;

	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	DirectX::Model::EffectCollection m_modelNormal;
	std::unique_ptr<SkinnedCharacter> m_skinnedCharacter;
	DirectX::ModelBone::TransformArray m_drawBones;
	Camera m_camera;
	DX::AnimationSDKMESH m_animation;

	float nightVolume;
	float nightSlide;
	enum Descriptors
	{
		SceneTex,
		RadianceIBL,
		IrradianceIBL,
		AlbedoMap,
		NormalMap,
		RMAMap,
		EmissiveMap,
		Count
	};


	enum RTDescriptors
	{
		HDRScene,
		RTCount
	};


};
