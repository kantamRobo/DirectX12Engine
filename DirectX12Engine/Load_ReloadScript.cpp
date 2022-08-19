#include "Load_ReloadScript.h"
#include <fstream>
#include <LuaScript.cpp>
#include <filesystem>

bool Load_ReloadScript::RunVSCode(const std::string& filename)
{

    STARTUPINFO startInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    //（まだ未完成)親プロセスを起動
    
    BOOL runvscode =CreateProcess(
    (L"C:\\Users\\hatte\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Visual Studio Code\\Visual Studio Code.lnk"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &processInfo);


    //(まだ未完成)子プロセスを起動
    std::wstring stemp = std::wstring(filename.begin(), filename.end());
    LPCWSTR file = stemp.c_str();
    std::wstring exe = TEXT(".sln");
    std::wstring solution = stemp.append(exe);
    LPCWSTR exeL = solution.c_str();
    BOOL runsln = CreateProcess(exeL, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &processInfo);



    return false;
}

bool Load_ReloadScript::LoadScript(const std::string& filename, std::vector<uint8_t>& data)
{
    //読み込み
    std::ifstream file(filename);

    file.open(filename);

    if (file.is_open())
    {
        size_t  datasize = (size_t)file.tellg();
        file.seekg(0, file.beg);
        data.resize(datasize);
   
        file.read((char*)data.data(), datasize);
        file.close();
        //テキストを走らせる

        LuaInternal luainternal;

        luainternal.m_status = luaL_loadstring(luainternal.m_luaState, filename.data());

        //スクリプトを走らせる

        luainternal.m_status = lua_pcall(luainternal.m_luaState, 0, LUA_MULTRET, 0);

        return true;
    }

    

    return false;
}
