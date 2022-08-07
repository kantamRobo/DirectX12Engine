#include "RenderPath2D_BindLua.h"
#include "wiSprite_BindLua.h"
#include "wiSpriteFont_BindLua.h"

#include <sstream>

using namespace std;

const char RenderPath2D_BindLua::className[] = "RenderPath2D";

Luna<RenderPath2D_BindLua>::FunctionType RenderPath2D_BindLua::methods[] = {
	lunamethod(RenderPath2D_BindLua, AddSprite),
	lunamethod(RenderPath2D_BindLua, AddFont),
	lunamethod(RenderPath2D_BindLua, RemoveSprite),
	lunamethod(RenderPath2D_BindLua, RemoveFont),
	lunamethod(RenderPath2D_BindLua, ClearSprites),
	lunamethod(RenderPath2D_BindLua, ClearFonts),
	lunamethod(RenderPath2D_BindLua, GetSpriteOrder),
	lunamethod(RenderPath2D_BindLua, GetFontOrder),
	lunamethod(RenderPath2D_BindLua, AddLayer),
	lunamethod(RenderPath2D_BindLua, GetLayers),
	lunamethod(RenderPath2D_BindLua, SetLayerOrder),
	lunamethod(RenderPath2D_BindLua, SetSpriteOrder),
	lunamethod(RenderPath2D_BindLua, SetFontOrder),
	lunamethod(RenderPath_BindLua, GetLayerMask),
	lunamethod(RenderPath_BindLua, SetLayerMask),
	{ NULL, NULL }
};
Luna<RenderPath2D_BindLua>::PropertyType RenderPath2D_BindLua::properties[] = {
	{ NULL, NULL }
};

int RenderPath2D_BindLua::AddSprite(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "AddSprite() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSprite_BindLua* sprite = Luna<wiSprite_BindLua>::lightcheck(L, 1);
		if (sprite != nullptr)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				if (argc > 1)
					ccomp->AddSprite(&sprite->sprite, LuaScript::SGetString(L, 2));
				else
					ccomp->AddSprite(&sprite->sprite);
			}
			else
			{
				LuaScript::SError(L, "AddSprite(Sprite sprite, opt string layer) not a RenderPath2D!");
			}
		}
		else
			LuaScript::SError(L, "AddSprite(Sprite sprite, opt string layer) argument is not a Sprite!");
	}
	else
	{
		LuaScript::SError(L, "AddSprite(Sprite sprite, opt string layer) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::AddFont(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "AddFont() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSpriteFont_BindLua* font = Luna<wiSpriteFont_BindLua>::lightcheck(L, 1);
		if (font != nullptr)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				if (argc > 1)
					ccomp->AddFont(&font->font, LuaScript::SGetString(L, 2));
				else
					ccomp->AddFont(&font->font);
			}
			else
			{
				LuaScript::SError(L, "AddFont(Font font, opt string layer) not a RenderPath2D!");
			}
		}
		else
			LuaScript::SError(L, "AddFont(Font font, opt string layer) argument is not a Font!");
	}
	else
	{
		LuaScript::SError(L, "AddFont(Font font, opt string layer) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::RemoveSprite(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "RemoveSprite() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSprite_BindLua* sprite = Luna<wiSprite_BindLua>::lightcheck(L, 1);
		if (sprite != nullptr)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				ccomp->RemoveSprite(&sprite->sprite);
			}
			else
			{
				LuaScript::SError(L, "RemoveSprite(Sprite sprite) not a RenderPath2D!");
			}
		}
		else
			LuaScript::SError(L, "RemoveSprite(Sprite sprite) argument is not a Sprite!");
	}
	else
	{
		LuaScript::SError(L, "RemoveSprite(Sprite sprite) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::RemoveFont(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "RemoveFont() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSpriteFont_BindLua* font = Luna<wiSpriteFont_BindLua>::lightcheck(L, 1);
		if (font != nullptr)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				ccomp->RemoveFont(&font->font);
			}
			else
			{
				LuaScript::SError(L, "RemoveFont(Font font) not a RenderPath2D!");
			}
		}
		else
			LuaScript::SError(L, "RemoveFont(Font font) argument is not a Font!");
	}
	else
	{
		LuaScript::SError(L, "RemoveFont(Font font) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::ClearSprites(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "ClearSprites() component is empty!");
		return 0;
	}
	RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
	if (ccomp != nullptr)
	{
		ccomp->ClearSprites();
	}
	else
	{
		LuaScript::SError(L, "ClearSprites() not a RenderPath2D!");
	}
	return 0;
}
int RenderPath2D_BindLua::ClearFonts(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "ClearFonts() component is empty!");
		return 0;
	}
	RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
	if (ccomp != nullptr)
	{
		ccomp->ClearFonts();
	}
	else
	{
		LuaScript::SError(L, "ClearFonts() not a RenderPath2D!");
	}
	return 0;
}
int RenderPath2D_BindLua::GetSpriteOrder(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "GetSpriteOrder() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSprite_BindLua* sprite = Luna<wiSprite_BindLua>::lightcheck(L, 1);
		if (sprite != nullptr)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				LuaScript::SSetInt(L, ccomp->GetSpriteOrder(&sprite->sprite));
				return 1;
			}
			else
			{
				LuaScript::SError(L, "GetSpriteOrder(Sprite sprite) not a RenderPath2D!");
			}
		}
		else
			LuaScript::SError(L, "GetSpriteOrder(Sprite sprite) argument is not a Sprite!");
	}
	else
	{
		LuaScript::SError(L, "GetSpriteOrder(Sprite sprite) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::GetFontOrder(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "GetFontOrder() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSpriteFont_BindLua* font = Luna<wiSpriteFont_BindLua>::lightcheck(L, 1);
		if (font != nullptr)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				LuaScript::SSetInt(L, ccomp->GetFontOrder(&font->font));
				return 1;
			}
			else
			{
				LuaScript::SError(L, "GetFontOrder(Font font) not a RenderPath2D!");
			}
		}
		else
			LuaScript::SError(L, "GetFontOrder(Font font) argument is not a Sprite!");
	}
	else
	{
		LuaScript::SError(L, "GetFontOrder(Font font) not enough arguments!");
	}
	return 0;
}

int RenderPath2D_BindLua::AddLayer(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "AddLayer() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			ccomp->AddLayer(LuaScript::SGetString(L, 1));
		}
		else
		{
			LuaScript::SError(L, "AddLayer(string name) not a RenderPath2D!");
		}
	}
	else
	{
		LuaScript::SError(L, "AddLayer(string name) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::GetLayers(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "GetLayers() component is empty!");
		return 0;
	}

	RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
	if (ccomp != nullptr)
	{
		stringstream ss("");
		for (auto& x : ccomp->layers)
		{
			ss << x.name << endl;
		}
		LuaScript::SSetString(L, ss.str());
		return 1;
	}
	else
	{
		LuaScript::SError(L, "GetLayers() not a RenderPath2D!");
	}

	return 0;
}
int RenderPath2D_BindLua::SetLayerOrder(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetLayerOrder() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 1)
	{
		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			ccomp->SetLayerOrder(LuaScript::SGetString(L, 1), LuaScript::SGetInt(L, 2));
		}
		else
		{
			LuaScript::SError(L, "SetLayerOrder(string name, int order) not a RenderPath2D!");
		}
	}
	else
	{
		LuaScript::SError(L, "SetLayerOrder(string name, int order) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::SetSpriteOrder(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetSpriteOrder() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 1)
	{
		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			wiSprite_BindLua* sprite = Luna<wiSprite_BindLua>::lightcheck(L, 1);
			if (sprite != nullptr)
			{
				ccomp->SetSpriteOrder(&sprite->sprite, LuaScript::SGetInt(L, 2));
			}
			else
			{
				LuaScript::SError(L, "SetSpriteOrder(Sprite sprite, int order) argument is not a Sprite!");
			}
		}
		else
		{
			LuaScript::SError(L, "SetSpriteOrder(Sprite sprite, int order) not a RenderPath2D!");
		}
	}
	else
	{
		LuaScript::SError(L, "SetSpriteOrder(Sprite sprite, int order) not enough arguments!");
	}
	return 0;
}
int RenderPath2D_BindLua::SetFontOrder(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetFontOrder() component is empty!");
		return 0;
	}
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 1)
	{
		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			wiSpriteFont_BindLua* font = Luna<wiSpriteFont_BindLua>::lightcheck(L, 1);
			if (font != nullptr)
			{
				ccomp->SetFontOrder(&font->font, LuaScript::SGetInt(L, 2));
			}
			else
			{
				LuaScript::SError(L, "SetFontOrder(Font font, int order) argument is not a Font!");
			}
		}
		else
		{
			LuaScript::SError(L, "SetFontOrder(Font font, int order) not a RenderPath2D!");
		}
	}
	else
	{
		LuaScript::SError(L, "SetFontOrder(Font font, int order) not enough arguments!");
	}
	return 0;
}

void RenderPath2D_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<RenderPath2D_BindLua>::Register(LuaScript::GetLuaState());
	}
}