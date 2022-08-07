#include "RenderPath3D_BindLua.h"
#include "Texture_BindLua.h"

const char RenderPath3D_BindLua::className[] = "RenderPath3D";

Luna<RenderPath3D_BindLua>::FunctionType RenderPath3D_BindLua::methods[] = {
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

	lunamethod(RenderPath3D_BindLua, SetAO),
	lunamethod(RenderPath3D_BindLua, SetSSREnabled),
	lunamethod(RenderPath3D_BindLua, SetRaytracedReflectionsEnabled),
	lunamethod(RenderPath3D_BindLua, SetShadowsEnabled),
	lunamethod(RenderPath3D_BindLua, SetReflectionsEnabled),
	lunamethod(RenderPath3D_BindLua, SetFXAAEnabled),
	lunamethod(RenderPath3D_BindLua, SetBloomEnabled),
	lunamethod(RenderPath3D_BindLua, SetBloomThreshold),
	lunamethod(RenderPath3D_BindLua, SetColorGradingEnabled),
	lunamethod(RenderPath3D_BindLua, SetVolumeLightsEnabled),
	lunamethod(RenderPath3D_BindLua, SetLightShaftsEnabled),
	lunamethod(RenderPath3D_BindLua, SetLensFlareEnabled),
	lunamethod(RenderPath3D_BindLua, SetMotionBlurEnabled),
	lunamethod(RenderPath3D_BindLua, SetSSSEnabled),
	lunamethod(RenderPath3D_BindLua, SetDitherEnabled),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldEnabled),
	lunamethod(RenderPath3D_BindLua, SetEyeAdaptionEnabled),
	lunamethod(RenderPath3D_BindLua, SetMSAASampleCount),
	lunamethod(RenderPath3D_BindLua, SetSharpenFilterEnabled),
	lunamethod(RenderPath3D_BindLua, SetSharpenFilterAmount),
	lunamethod(RenderPath3D_BindLua, SetExposure),
	lunamethod(RenderPath3D_BindLua, SetMotionBlurStrength),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldFocus),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldStrength),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldAspect),
	{ NULL, NULL }
};
Luna<RenderPath3D_BindLua>::PropertyType RenderPath3D_BindLua::properties[] = {
	{ NULL, NULL }
};


int RenderPath3D_BindLua::SetAO(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetAO(AO value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		int value = LuaScript::SGetInt(L, 1);
		RenderPath3D::AO ao = (RenderPath3D::AO)value;
		((RenderPath3D*)component)->setAO(ao);
	}
	else
		LuaScript::SError(L, "SetAO(AO value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetSSREnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetSSREnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setSSREnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetSSREnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetRaytracedReflectionsEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetRaytracedReflectionsEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setRaytracedReflectionsEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetRaytracedReflectionsEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetShadowsEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetShadowsEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setShadowsEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetShadowsEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetReflectionsEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetShadowsEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setReflectionsEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetShadowsEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetFXAAEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetFXAAEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setFXAAEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetFXAAEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetBloomEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetBloomEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setBloomEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetBloomEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetBloomThreshold(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetBloomThreshold(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setBloomThreshold(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetBloomThreshold(float value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetColorGradingEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetColorGradingEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setColorGradingEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetColorGradingEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetVolumeLightsEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetVolumeLightsEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setVolumeLightsEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetVolumeLightsEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetLightShaftsEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetLightShaftsEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setLightShaftsEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetLightShaftsEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetLensFlareEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetLensFlareEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setLensFlareEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetLensFlareEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetMotionBlurEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetMotionBlurEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setMotionBlurEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetMotionBlurEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetSSSEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetSSSEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setSSSEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetSSSEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetDitherEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetDitherEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
		((RenderPath3D*)component)->setDitherEnabled(LuaScript::SGetBool(L, 1));
	else
		LuaScript::SError(L, "SetDitherEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetDepthOfFieldEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetDepthOfFieldEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setDepthOfFieldEnabled(LuaScript::SGetBool(L, 1));
	}
	else
		LuaScript::SError(L, "SetDepthOfFieldEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetEyeAdaptionEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetEyeAdaptionEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setEyeAdaptionEnabled(LuaScript::SGetBool(L, 1));
	}
	else
		LuaScript::SError(L, "SetEyeAdaptionEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetMSAASampleCount(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetMSAASampleCount(int value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setMSAASampleCount((uint32_t)LuaScript::SGetInt(L, 1));
	}
	else
		LuaScript::SError(L, "SetMSAASampleCount(int value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetSharpenFilterEnabled(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetSharpenFilterEnabled(bool value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setSharpenFilterEnabled(LuaScript::SGetBool(L, 1));
	}
	else
		LuaScript::SError(L, "SetSharpenFilterEnabled(bool value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetSharpenFilterAmount(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetSharpenFilterAmount(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setSharpenFilterAmount(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetSharpenFilterAmount(float value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetExposure(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetExposure(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setExposure(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetExposure(float value) not enough arguments!");
	return 0;
}


int RenderPath3D_BindLua::SetMotionBlurStrength(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetMotionBlurStrength(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setMotionBlurStrength(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetMotionBlurStrength(float value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetDepthOfFieldFocus(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetDepthOfFieldFocus(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setDepthOfFieldFocus(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetDepthOfFieldFocus(float value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetDepthOfFieldStrength(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetDepthOfFieldStrength(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setDepthOfFieldStrength(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetDepthOfFieldStrength(float value) not enough arguments!");
	return 0;
}
int RenderPath3D_BindLua::SetDepthOfFieldAspect(lua_State* L)
{
	if (component == nullptr)
	{
		LuaScript::SError(L, "SetDepthOfFieldAspect(float value) component is null!");
		return 0;
	}
	if (LuaScript::SGetArgCount(L) > 0)
	{
		((RenderPath3D*)component)->setDepthOfFieldAspect(LuaScript::SGetFloat(L, 1));
	}
	else
		LuaScript::SError(L, "SetDepthOfFieldAspect(float value) not enough arguments!");
	return 0;
}

void RenderPath3D_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<RenderPath3D_BindLua>::Register(LuaScript::GetLuaState());

		LuaScript::RunText("AO_DISABLED = 0");
		LuaScript::RunText("AO_SSAO = 1");
		LuaScript::RunText("AO_HBAO = 2");
		LuaScript::RunText("AO_MSAO = 3");
		LuaScript::RunText("AO_RTAO = 4");
	}
}
