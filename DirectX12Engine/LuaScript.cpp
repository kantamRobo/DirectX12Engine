#include "LuaScript.h"
#include <mutex>
using namespace LuaScript;
using namespace std;
struct LuaInternal
{
	lua_State* m_luaState = NULL;
	int m_status = 0; //last call status

	~LuaInternal()
	{
		lua_close(m_luaState);
	}
};
LuaInternal luainternal;

lua_State* GetLuaState()
{
	return luainternal.m_luaState;
}

int Internal_DoFile(lua_State* L)
{
	int argc = SGetArgCount(L);

	if (argc > 0)
	{
		std::string filename = SGetString(L, 1);
		std::vector<uint8_t> filedata;
		/*
		if (wiHelper::FileRead(filename, filedata))
			{
				std::string command = string(filedata.begin(), filedata.end());
				int status = luaL_loadstring(L, command.c_str());
				if (status == 0)
				{
					status = lua_pcall(L, 0, LUA_MULTRET, 0);
				}

				if (status != 0)
				{
					const char* str = lua_tostring(L, -1);

					if (str == nullptr)
						return 0;

					stringstream ss("");
					ss << LuaScript_ERROR_PREFIX << str;
					wiBackLog::post(ss.str().c_str());
					lua_pop(L, 1); // remove error message
				}
			}
			else
		{
			SError(L, "dofile(string filename) not enough arguments!");
		}

		return 0;
		*/


		/*
		void Initialize()
		{
			luainternal.m_luaState = luaL_newstate();
			luaL_openlibs(luainternal.m_luaState);
			RegisterFunc("dofile", Internal_DoFile);
			RunText(LuaScript_Globals);

			MainComponent_BindLua::Bind();
			RenderPath_BindLua::Bind();
			RenderPath2D_BindLua::Bind();
			LoadingScreen_BindLua::Bind();
			RenderPath3D_BindLua::Bind();
			Texture_BindLua::Bind();
			wiRenderer_BindLua::Bind();
			Audio_BindLua::Bind();
			wiSprite_BindLua::Bind();
			wiImageParams_BindLua::Bind();
			SpriteAnim_BindLua::Bind();
			wiScene_BindLua::Bind();
			Vector_BindLua::Bind();
			Matrix_BindLua::Bind();
			wiInput_BindLua::Bind();
			wiSpriteFont_BindLua::Bind();
			wiBackLog_BindLua::Bind();
			wiNetwork_BindLua::Bind();
			wiIntersect_BindLua::Bind();

			wiBackLog::post("LuaScript Initialized");
		}


		*/


	}
}
	lua_State* GetLuaState()
	{
		return luainternal.m_luaState;
	}

	bool LuaScript::Success()
	{
		return luainternal.m_status == 0;
	}
	
	string GetErrorMsg()
	{
		if (Failed()) {
			LuaScript::lock.lock();
			string retVal = lua_tostring(luainternal.m_luaState, -1);
			LuaScript::lock.unlock();
			return retVal;
		}
		return string("");
	}
	string PopErrorMsg()
	{
		LuaScript::lock.lock();
		string retVal = lua_tostring(luainternal.m_luaState, -1);
		lua_pop(luainternal.m_luaState, 1); // remove error message
		LuaScript::lock.unlock();
		return retVal;
	}
	void LuaScript::PostErrorMsg()
	{
		if (Failed())
		{
			LuaScript::lock.lock();
			const char* str = lua_tostring(luainternal.m_luaState, -1);
			LuaScript::lock.unlock();
			if (str == nullptr)
				return;
			stringstream ss("");
			ss << LuaScript_ERROR_PREFIX << str;
			wiBackLog::post(ss.str().c_str());
			LuaScript::lock.lock();
			lua_pop(luainternal.m_luaState, 1); // remove error message
			LuaScript::lock.unlock();
		}
	}

	bool RunFile(const std::string& filename)
	{
		std::vector<uint8_t> filedata;
		if (wiHelper::FileRead(filename, filedata))
		{
			return RunText(string(filedata.begin(), filedata.end()));
		}
		return false;

		//lock.lock();
		//m_status = luaL_loadfile(m_luaState, filename.c_str());
		//lock.unlock();
		//
		//if (Success()) {
		//	return RunScript();
		//}

		//PostErrorMsg();
		//return false;
	}
	bool RunScript()
	{
		LuaScript::lock.lock();
		luainternal.m_status = lua_pcall(luainternal.m_luaState, 0, LUA_MULTRET, 0);
		LuaScript::lock.unlock();
		if (Failed())
		{
			PostErrorMsg();
			return false;
		}
		return true;
	}
	bool LuaScript::RunText(const std::string& script)
	{
		LuaScript::lock.lock();
		luainternal.m_status = luaL_loadstring(luainternal.m_luaState, script.c_str());
		LuaScript::lock.unlock();
		if (Success())
		{
			return RunScript();
		}

		PostErrorMsg();
		return false;
	}
	bool RegisterFunc(const std::string& name, lua_CFunction function)
	{
		LuaScript::lock.lock();
		lua_register(luainternal.m_luaState, name.c_str(), function);
		LuaScript::lock.unlock();

		PostErrorMsg();

		return Success();
	}
	void LuaScript::RegisterLibrary(const std::string& tableName, const luaL_Reg* functions)
	{
		LuaScript::lock.lock();
		if (luaL_newmetatable(luainternal.m_luaState, tableName.c_str()) != 0)
		{
			//table is not yet present
			lua_pushvalue(luainternal.m_luaState, -1);
			lua_setfield(luainternal.m_luaState, -2, "__index"); // Object.__index = Object
			LuaScript::lock.unlock();
			AddFuncArray(functions);
		}
		else
		{
			LuaScript::lock.unlock();
		}
	}
	bool RegisterObject(const std::string& tableName, const std::string& name, void* object)
	{
		RegisterLibrary(tableName, nullptr);

		LuaScript::lock.lock();
		// does this call need to be checked? eg. userData == nullptr?
		void** userData = static_cast<void**>(lua_newuserdata(luainternal.m_luaState, sizeof(void*)));
		*(userData) = object;

		luaL_setmetatable(luainternal.m_luaState, tableName.c_str());
		lua_setglobal(luainternal.m_luaState, name.c_str());

		LuaScript::lock.unlock();
		return true;
	}
	void AddFunc(const std::string& name, lua_CFunction function)
	{
		LuaScript::lock.lock();

		lua_pushcfunction(luainternal.m_luaState, function);
		lua_setfield(luainternal.m_luaState, -2, name.c_str());

		LuaScript::lock.unlock();
	}
	void LuaScript::AddFuncArray(const luaL_Reg* functions)
	{
		if (functions != nullptr)
		{
			LuaScript::lock.lock();
			luaL_setfuncs(luainternal.m_luaState, functions, 0);
			LuaScript::lock.unlock();
		}
	}
	void AddInt(const std::string& name, int data)
	{
		LuaScript::lock.lock();
		lua_pushinteger(luainternal.m_luaState, data);
		lua_setfield(luainternal.m_luaState, -2, name.c_str());
		LuaScript::lock.unlock();
	}

	void SetDeltaTime(double dt)
	{
		LuaScript::lock.lock();
		lua_getglobal(luainternal.m_luaState, "setDeltaTime");
		SSetDouble(luainternal.m_luaState, dt);
		lua_call(luainternal.m_luaState, 1, 0);
		LuaScript::lock.unlock();
	}
	void FixedUpdate()
	{
		TrySignal("wickedengine_fixed_update_tick");
	}
	void Update()
	{
		TrySignal("wickedengine_update_tick");
	}
	void Render()
	{
		TrySignal("wickedengine_render_tick");
	}

	inline void SignalHelper(lua_State* L, const std::string& name)
	{
		lua_getglobal(L, "signal");
		SSetString(L, name.c_str());
		lua_call(L, 1, 0);
	}
	void Signal(const std::string& name)
	{
		LuaScript::lock.lock();
		SignalHelper(luainternal.m_luaState, name);
		LuaScript::lock.unlock();
	}
	bool LuaScript::TrySignal(const std::string& name)
	{
		if (!LuaScript::lock.try_lock())
			return false;
		SignalHelper(luainternal.m_luaState, name);
		LuaScript::lock.unlock();
		return true;
	}

	void KillProcesses()
	{
		RunText("killProcesses();");
	}

	string LuaScript::SGetString(lua_State* L, int stackpos)
	{
		const char* str = lua_tostring(L, stackpos);
		if (str != nullptr)
			return str;
		return string("");
	}
	bool SIsString(lua_State* L, int stackpos)
	{
		return lua_isstring(L, stackpos) != 0;
	}
	bool SIsNumber(lua_State* L, int stackpos)
	{
		return lua_isnumber(L, stackpos) != 0;
	}
	int SGetInt(lua_State* L, int stackpos)
	{
		return static_cast<int>(SGetLongLong(L, stackpos));
	}
	long SGetLong(lua_State* L, int stackpos)
	{
		return static_cast<long>(SGetLongLong(L, stackpos));
	}
	long long LuaScript::SGetLongLong(lua_State* L, int stackpos)
	{
		return lua_tointeger(L, stackpos);
	}
	float LuaScript::SGetFloat(lua_State* L, int stackpos)
	{
		return static_cast<float>(SGetDouble(L, stackpos));
	}
	DirectX::XMFLOAT2 SGetFloat2(lua_State* L, int stackpos)
	{
		return DirectX::XMFLOAT2(SGetFloat(L, stackpos), SGetFloat(L, stackpos + 1));
	}
	DirectX::XMFLOAT3 SGetFloat3(lua_State* L, int stackpos)
	{
		return DirectX::XMFLOAT3(SGetFloat(L, stackpos), SGetFloat(L, stackpos + 1), SGetFloat(L, stackpos + 2));
	}
	DirectX::XMFLOAT4 SGetFloat4(lua_State* L, int stackpos)
	{
		return DirectX::XMFLOAT4(SGetFloat(L, stackpos), SGetFloat(L, stackpos + 1), SGetFloat(L, stackpos + 2), SGetFloat(L, stackpos + 3));
	}
	double SGetDouble(lua_State* L, int stackpos)
	{
		return lua_tonumber(L, stackpos);
	}
	bool SGetBool(lua_State* L, int stackpos)
	{
		return lua_toboolean(L, stackpos) != 0;
	}
	int LuaScript::SGetArgCount(lua_State* L)
	{
		return lua_gettop(L);
	}
	void* SGetUserData(lua_State* L)
	{
		return lua_touserdata(L, 1);
	}

	void SSetInt(lua_State* L, int data)
	{
		lua_pushinteger(L, (lua_Integer)data);
	}
	void SSetLong(lua_State* L, long data)
	{
		lua_pushinteger(L, (lua_Integer)data);
	}
	void SSetLongLong(lua_State* L, long long data)
	{
		lua_pushinteger(L, (lua_Integer)data);
	}
	void LuaScript::SSetFloat(lua_State* L, float data)
	{
		lua_pushnumber(L, (lua_Number)data);
	}
	void SSetFloat2(lua_State* L, const DirectX::XMFLOAT2& data)
	{
		SSetFloat(L, data.x);
		SSetFloat(L, data.y);
	}
	void SSetFloat3(lua_State* L, const DirectX::XMFLOAT3& data)
	{
		SSetFloat(L, data.x);
		SSetFloat(L, data.y);
		SSetFloat(L, data.z);
	}
	void SSetFloat4(lua_State* L, const DirectX::XMFLOAT4& data)
	{
		SSetFloat(L, data.x);
		SSetFloat(L, data.y);
		SSetFloat(L, data.z);
		SSetFloat(L, data.w);
	}
	void LuaScript::SSetDouble(lua_State* L, double data)
	{
		lua_pushnumber(L, (lua_Number)data);
	}
	void LuaScript::SSetString(lua_State* L, const std::string& data)
	{
		lua_pushstring(L, data.c_str());
	}
	void SSetBool(lua_State* L, bool data)
	{
		lua_pushboolean(L, static_cast<int>(data));
	}
	void SSetPointer(lua_State* L, void* data)
	{
		lua_pushlightuserdata(L, data);
	}
	void SSetNull(lua_State* L)
	{
		lua_pushnil(L);
	}

	void SError(lua_State* L, const std::string& error)
	{
		//retrieve line number for error info
		lua_Debug ar;
		lua_getstack(L, 1, &ar);
		lua_getinfo(L, "nSl", &ar);
		int line = ar.currentline;

		stringstream ss("");
		ss << LuaScript_ERROR_PREFIX << "Line " << line << ": ";
		if (!error.empty())
		{
			ss << error;
		}
		wiBackLog::post(ss.str().c_str());
	}

	void SAddMetatable(lua_State* L, const std::string& name)
	{
		luaL_newmetatable(L, name.c_str());
	}
