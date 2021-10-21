#pragma once
#include <d3dx12.h>
class ModelRendererWorker
{
public:
	ModelRenderWorker();
	HRESULT CreateCommandQueue();
	//こちらでコマンド系インターフェースを作成する。

	void CreateCommandAllocators();
	void CreateCommandLists();
};

