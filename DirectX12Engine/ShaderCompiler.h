#pragma once

#include <wrl.h>
#include "Structures.h"
#include <dxcapi.h>
#include <Windows.h>
class ShaderCompiler
{
	IDxcBlob* GetCompiledDxcBlob() const
	{
		return m_dxcBlob;
	}
public:
	/// <summary>
	/// レイトレーシング用のシェーダーをロード。
	/// </summary>
	void LoadRaytracing(const wchar_t* filePath);
	/// <summary>
	/// コンパイル済みシェーダーデータを取得。
	/// </summary>
	/// <returns></returns>
private:
	ID3DBlob* m_blob = nullptr;	//コンパイル済みのシェーダーデータ。
	IDxcBlob* m_dxcBlob = nullptr;	//DXCコンパイラを使用したときのシェーダーデータ。
	bool m_isInited = false;		//初期化済み？

};

