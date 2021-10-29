#include "DX12EngineCore.h"
#include <stdexcept>
#include <wtypes.h>



DX12EngineCore::DX12EngineCore(HWND hwnd)
{

	HRESULT hr;
	UINT dxgiFlags = 0;
#if defined(_DEBUG)
	Microsoft::WRL::ComPtr<ID3D12Debug> debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;

#if 0 // GBV ��L��������ꍇ.
		Microsoft::WRL::ComPtr<ID3D12Debug3> debug3;
		debug.As(&debug3);
		if (debug3)
		{
			debug3->SetEnableGPUBasedValidation(true);
		}
#endif
	}
#endif
	Microsoft::WRL::ComPtr<IDXGIFactory3> factory;
	hr = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateDXGIFactory2 failed.");
	}

	// �n�[�h�E�F�A�A�_�v�^�̌���
	Microsoft::WRL::ComPtr<IDXGIAdapter1> useAdapter;
	{
		UINT adapterIndex = 0;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		while (DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter))
		{
			DXGI_ADAPTER_DESC1 desc1{};
			adapter->GetDesc1(&desc1);
			++adapterIndex;
			if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			// D3D12�͎g�p�\��
			hr = D3D12CreateDevice(
				adapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				__uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
				break;
		}
		adapter.As(&useAdapter); // �g�p����A�_�v�^�[

		hr = D3D12CreateDevice(useAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
		if (FAILED(hr))
		{
			throw new std::runtime_error("D3D12CreateDevice failed.");
		}


		// HWND ����N���C�A���g�̈�T�C�Y�𔻒肷��B
	// (�E�B���h�E�T�C�Y��������Ă�����g�p����̂��悢)
		RECT rect;
		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		// �X���b�v�`�F�C���̐���
		{
			DXGI_SWAP_CHAIN_DESC1 scDesc{};
			scDesc.BufferCount = FrameBufferCount;
			scDesc.Width = width;
			scDesc.Height = height;
			scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			scDesc.SampleDesc.Count = 1;

			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain;
			hr = factory->CreateSwapChainForHwnd(
				m_commandQueue.Get(),
				hwnd,
				&scDesc,
				nullptr,
				nullptr,
				&swapchain);
			if (FAILED(hr))
			{
				throw std::runtime_error("CreateSwapChainForHwnd failed.");
			}

			swapchain.As(&m_swapchain); // IDXGISwapChain4 �擾

		}

		/*
	// �R�}���h�L���[�̐���
	D3D12_COMMAND_QUEUE_DESC queueDesc{
	  D3D12_COMMAND_LIST_TYPE_DIRECT,
	  0,
	  D3D12_COMMAND_QUEUE_FLAG_NONE,
	  0
	};
	*/



	// �e�f�B�X�N���v�^�q�[�v�̏���.�i�f�B�X�N���v�^�N���X����Ăԁj
	//PrepareDescriptorHeaps();
	// �����_�[�^�[�Q�b�g�r���[�̐���.
		PrepareRenderTargetView();
		//// �f�v�X�o�b�t�@�֘A�̏���.(�f�C�X�N���v�^�N���X�����ԁj
		//CreateDepthBuffer(width, height);

		// �R�}���h�A���P�[�^�|�̏����i�����_�[����Ăԁj
		//CreateCommandAllocators();
		// �`��t���[�������p�t�F���X����.
		CreateFrameFences();
		// �R�}���h���X�g�̐���.(�����_�[����Ăԁj
		/*
		
		hr = m_device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_commandAllocators[0].Get(),
			nullptr,
			IID_PPV_ARGS(&m_commandList)
		);
		m_commandList->Close();
		*/
		m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, float(width), float(height));
		m_scissorRect = CD3DX12_RECT(0, 0, LONG(width), LONG(height));
		m_fenceWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		
	}
}
void DX12EngineCore::CreateFrameFences()
{
	HRESULT hr;
	m_frameFences.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = m_device->CreateFence(
			0,  // �����l
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_frameFences[i]));
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed CreateFence");
		}
	}
}


void DX12EngineCore::PrepareRenderTargetView()
{
	// �X���b�v�`�F�C���C���[�W�ւ̃����_�[�^�[�Q�b�g�r���[����
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
		// �Q�Ƃ���f�B�X�N���v�^�̕ύX
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}
}