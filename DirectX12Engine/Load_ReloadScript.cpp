#include "Load_ReloadScript.h"
#include <fstream>
#include <LuaScript.cpp>

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
