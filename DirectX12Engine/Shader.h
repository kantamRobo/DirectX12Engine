#pragma once

#include <stierr.h>
#include <sstream>
#include <fstream>
#include <atlbase.h>
#include <string>
#include <d3dcompiler.h>
#include <dxcapi.h>
#include <Windows.h>
#include "dxc/include/dxc/Support/dxcapi.use.h"
//シェーダー
enum EShader {
	eShader_Raygeneration,		//カメラレイを生成するシェーダー。
	eShader_Miss,				//カメラレイがどこにもぶつからなかった時に呼ばれるシェーダー。
	eShader_PBRChs,				//もっとも近いポリゴンとカメラレイが交差したときに呼ばれるシェーダー。
	eShader_ShadowChs,			//もっとも近いポリゴンとシャドウレイが交差したときに呼ばれるシェーダー。
	eShader_ShadowMiss,			//シャドウレイがどこにもぶつからなかった時に呼ばれるシェーダー。
	eShader_Num,				//シェーダーの数。
};

//シェーダーデータ構造体。
struct ShaderData {
	const wchar_t* entryPointName;				//エントリーポイントの名前。
	
};

class Shader {
public:
	
	void LoadRaytracing(const wchar_t* filePath);
	/// <summary>
	/// コンパイル済みシェーダーデータを取得。
	/// </summary>
	/// <returns></returns>
	ID3DBlob* GetCompiledBlob() const
	{
		return m_blob;
	}
	IDxcBlob* GetCompiledDxcBlob() const
	{
		return m_dxcBlob;
	}
	/// <summary>
	/// 初期化済み？
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}
private:
	
private:
	ID3DBlob* m_blob = nullptr;	//コンパイル済みのシェーダーデータ。
	IDxcBlob* m_dxcBlob = nullptr;	//DXCコンパイラを使用したときのシェーダーデータ。
	bool m_isInited = false;		//初期化済み？
};

const ShaderData shaderDatas[] = {

	{ L"mainDS"},
	{ L"mainHS"},
	{ L"mainPS"},
	{ L"mainVS"}
};