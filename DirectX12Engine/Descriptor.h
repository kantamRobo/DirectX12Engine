#pragma once
#include <d3dx12.h>
#include <wrl.h>
namespace Descriptor {
	HRESULT CreateSRV();//SRV
	HRESULT CreateUAV();//UAVの作成
	HRESULT CreateCBV();// //CBVの作成
	HRESULT CreateDSV();// DSVの作成
	HRESULT CreateDescriptorHeap();//ディスクリプタヒープを作成する
	
	HRESULT Create_MAXSIZE_DescriptorHeap();//作成できる最大値までディスクリプタヒープを作成する。
	
	void PrepareDescriptorHeaps(ID3D12Device* p_device, Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> rtvheap, 
		UINT FrameBufferCount, UINT& m_rtvDescriptorSize);
	void CreateDepthBuffer(ID3D12Device* p_device, int width, int height, 
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvheap);
	//引数:ビューのタイプ


}

struct DescriptorHeapsContainer
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapRtv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapDsv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapSrv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapUav;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapCbv;
};