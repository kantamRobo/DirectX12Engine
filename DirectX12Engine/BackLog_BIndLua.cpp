#include "BackLog_BindLua.h"
#include "wiBackLog.h"
#include "LuaScript.h"

#include <sstream>

using namespace std;

namespace wiBackLog_BindLua
{
	int backlog_clear(lua_State* L)
	{
		wiBackLog::clear();
		return 0;
	}
	int backlog_post(lua_State* L)
	{
		int argc = LuaScript::SGetArgCount(L);

		stringstream ss("");

		for (int i = 1; i <= argc; i++)
		{
			ss << LuaScript::SGetString(L, i);
		}

		if (!ss.str().empty())
			wiBackLog::post(ss.str().c_str());

		return 0;
	}
	int backlog_fontsize(lua_State* L)
	{
		int argc = LuaScript::SGetArgCount(L);

		if (argc > 0)
		{
			wiBackLog::setFontSize(LuaScript::SGetInt(L, 1));
		}
		else
			LuaScript::SError(L, "backlog_fontsize(int val) not enough arguments!");

		return 0;
	}
	int backlog_isactive(lua_State* L)
	{
		LuaScript::SSetBool(L, wiBackLog::isActive());
		return 1;
	}
	int backlog_fontrowspacing(lua_State* L)
	{
		int argc = LuaScript::SGetArgCount(L);
		if (argc > 0)
		{
			wiBackLog::setFontRowspacing(LuaScript::SGetFloat(L, 1));
		}
		else
			LuaScript::SError(L, "backlog_fontrowspacing(int val) not enough arguments!");
		return 0;
	}

	void Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			LuaScript::RegisterFunc("backlog_clear", backlog_clear);
			LuaScript::RegisterFunc("backlog_post", backlog_post);
			LuaScript::RegisterFunc("backlog_fontsize", backlog_fontsize);
			LuaScript::RegisterFunc("backlog_isactive", backlog_isactive);
			LuaScript::RegisterFunc("backlog_fontrowspacing", backlog_fontrowspacing);
		}
	}
}
