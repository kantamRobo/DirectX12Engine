#include "Load_ReloadScript.h"
#include <fstream>
#include <LuaScript.cpp>
#include <filesystem>

bool Load_ReloadScript::RunVSCode(const std::string& filename)
{

    STARTUPINFO startInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    //VSCode���N��
    /*
    BOOL runvscode =CreateProcess(
    (L"C:\\Users\\hatte\\AppData\\Local\\Programs\\Microsoft VS Code\\Code.exe"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &processInfo);
    */

    //�\�����[�V�����t�@�C���̋N��
    std::wstring filenamefirst = TEXT("\\");
    std::wstring filenamecore = std::wstring(filename.begin(), filename.end());
    filenamefirst.append(filenamecore);
    std::wstring filenamesecond = TEXT("\\");
    filenamefirst.append(filenamesecond);
    LPCWSTR file = filenamefirst.c_str();
    std::wstring filenamecore2 = std::wstring(filename.begin(), filename.end());
    
    std::wstring sln = TEXT(".sln");
    std::wstring solution = filenamecore2.append(sln);
    std::wstring complete = filenamefirst.append(filenamecore2);
    LPCWSTR exeL = complete .c_str();
    BOOL runsln = CreateProcess(exeL, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &processInfo);
   

    return false;
}

bool Load_ReloadScript::LoadScript(const std::string& filename, std::vector<uint8_t>& data)
{
    //�ǂݍ���
    std::ifstream file(filename);

    file.open(filename);

    if (file.is_open())
    {
        size_t  datasize = (size_t)file.tellg();
        file.seekg(0, file.beg);
        data.resize(datasize);
   
        file.read((char*)data.data(), datasize);
        file.close();
        //�e�L�X�g�𑖂点��

        LuaInternal luainternal;

        luainternal.m_status = luaL_loadstring(luainternal.m_luaState, filename.data());

        //�X�N���v�g�𑖂点��

        luainternal.m_status = lua_pcall(luainternal.m_luaState, 0, LUA_MULTRET, 0);

        return true;
    }

    

    return false;
}
