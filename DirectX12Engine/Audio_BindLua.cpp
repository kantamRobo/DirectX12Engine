#include "Audio_BindLua.h"
#include "wiAudio.h"
#include "Vector_BindLua.h"

const char Audio_BindLua::className[] = "Audio";

Luna<Audio_BindLua>::FunctionType Audio_BindLua::methods[] = {
	lunamethod(Audio_BindLua, CreateSound),
	lunamethod(Audio_BindLua, CreateSoundInstance),
	lunamethod(Audio_BindLua, Play),
	lunamethod(Audio_BindLua, Pause),
	lunamethod(Audio_BindLua, Stop),
	lunamethod(Audio_BindLua, GetVolume),
	lunamethod(Audio_BindLua, SetVolume),
	lunamethod(Audio_BindLua, ExitLoop),
	lunamethod(Audio_BindLua, GetSubmixVolume),
	lunamethod(Audio_BindLua, SetSubmixVolume),
	lunamethod(Audio_BindLua, Update3D),
	lunamethod(Audio_BindLua, SetReverb),
	{ NULL, NULL }
};
Luna<Audio_BindLua>::PropertyType Audio_BindLua::properties[] = {
	{ NULL, NULL }
};

int Audio_BindLua::CreateSound(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSound_BindLua* sound = Luna<wiSound_BindLua>::lightcheck(L, 2);
		if (sound != nullptr)
		{
			bool result = wiAudio::CreateSound(LuaScript::SGetString(L, 1), &sound->sound);
			LuaScript::SSetBool(L, result);
		}
		else
		{
			LuaScript::SError(L, "CreateSound(string filename, Sound sound) second argument is not a Sound!");
		}
	}
	else
		LuaScript::SError(L, "CreateSound(string filename, Sound sound) not enough arguments!");
	return 0;
}
int Audio_BindLua::CreateSoundInstance(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSound_BindLua* sound = Luna<wiSound_BindLua>::lightcheck(L, 1);
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 2);
		if (sound != nullptr && soundinstance != nullptr)
		{
			bool result = wiAudio::CreateSoundInstance(&sound->sound, &soundinstance->soundinstance);
			LuaScript::SSetBool(L, result);
			return 1;
		}
		else
		{
			LuaScript::SError(L, "CreateSoundInstance(Sound sound, SoundInstance soundinstance) argument types don't match!");
		}
	}
	else
		LuaScript::SError(L, "CreateSoundInstance(Sound sound, SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::Play(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 1);
		if (soundinstance != nullptr)
		{
			wiAudio::Play(&soundinstance->soundinstance);
		}
		else
		{
			LuaScript::SError(L, "Play(SoundInstance soundinstance) argument is not a SoundInstance!");
		}
	}
	else
		LuaScript::SError(L, "Play(SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::Pause(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 1);
		if (soundinstance != nullptr)
		{
			wiAudio::Pause(&soundinstance->soundinstance);
		}
		else
		{
			LuaScript::SError(L, "Pause(SoundInstance soundinstance) argument is not a SoundInstance!");
		}
	}
	else
		LuaScript::SError(L, "Pause(SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::Stop(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 1);
		if (soundinstance != nullptr)
		{
			wiAudio::Stop(&soundinstance->soundinstance);
		}
		else
		{
			LuaScript::SError(L, "Stop(SoundInstance soundinstance) argument is not a SoundInstance!");
		}
	}
	else
		LuaScript::SError(L, "Stop(SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::GetVolume(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 1);
		float volume = wiAudio::GetVolume(soundinstance == nullptr ? nullptr : &soundinstance->soundinstance);
		LuaScript::SSetFloat(L, volume);
		return 1;
	}
	else
		LuaScript::SError(L, "GetVolume(opt SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::SetVolume(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		float volume = LuaScript::SGetFloat(L, 1);
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 2);
		wiAudio::SetVolume(volume, soundinstance == nullptr ? nullptr : &soundinstance->soundinstance);
		return 0;
	}
	else
		LuaScript::SError(L, "SetVolume(float volume, opt SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::ExitLoop(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 1);
		if (soundinstance != nullptr)
		{
			wiAudio::ExitLoop(&soundinstance->soundinstance);
		}
		else
		{
			LuaScript::SError(L, "ExitLoop(SoundInstance soundinstance) argument is not a SoundInstance!");
		}
	}
	else
		LuaScript::SError(L, "ExitLoop(SoundInstance soundinstance) not enough arguments!");
	return 0;
}
int Audio_BindLua::GetSubmixVolume(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiAudio::SUBMIX_TYPE type = (wiAudio::SUBMIX_TYPE)LuaScript::SGetInt(L, 1);
		float volume = wiAudio::GetSubmixVolume(type);
		LuaScript::SSetFloat(L, volume);
		return 1;
	}
	else
		LuaScript::SError(L, "GetSubmixVolume(int submixtype) not enough arguments!");
	return 0;
}
int Audio_BindLua::SetSubmixVolume(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 1)
	{
		wiAudio::SUBMIX_TYPE type = (wiAudio::SUBMIX_TYPE)LuaScript::SGetInt(L, 1);
		float volume = LuaScript::SGetFloat(L, 2);
		wiAudio::SetSubmixVolume(type, volume);
		return 0;
	}
	else
		LuaScript::SError(L, "SetSubmixVolume(int submixtype, float volume) not enough arguments!");
	return 0;
}
int Audio_BindLua::Update3D(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 1)
	{
		wiSoundInstance_BindLua* soundinstance = Luna<wiSoundInstance_BindLua>::lightcheck(L, 1);
		wiSoundInstance3D_BindLua* soundinstance3D = Luna<wiSoundInstance3D_BindLua>::lightcheck(L, 2);
		if (soundinstance != nullptr && soundinstance3D != nullptr)
		{
			wiAudio::Update3D(&soundinstance->soundinstance, soundinstance3D->soundinstance3D);
		}
		else
		{
			LuaScript::SError(L, "Update3D(SoundInstance soundinstance, SoundInstance3D instance3D) argument types mismatch!");
		}
	}
	else
		LuaScript::SError(L, "Update3D(SoundInstance soundinstance, SoundInstance3D instance3D) not enough arguments!");
	return 0;
}
int Audio_BindLua::SetReverb(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiAudio::SetReverb((wiAudio::REVERB_PRESET)LuaScript::SGetInt(L, 1));
	}
	else
		LuaScript::SError(L, "SetReverb(int reverbtype) not enough arguments!");
	return 0;
}

void Audio_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<Audio_BindLua>::Register(LuaScript::GetLuaState());

		LuaScript::RunText("audio = Audio()");

		LuaScript::RunText("SUBMIX_TYPE_SOUNDEFFECT = 0");
		LuaScript::RunText("SUBMIX_TYPE_MUSIC = 1");
		LuaScript::RunText("SUBMIX_TYPE_USER0 = 2");
		LuaScript::RunText("SUBMIX_TYPE_USER1 = 3");

		LuaScript::RunText("REVERB_PRESET_DEFAULT = 0");
		LuaScript::RunText("REVERB_PRESET_GENERIC = 1");
		LuaScript::RunText("REVERB_PRESET_FOREST = 2");
		LuaScript::RunText("REVERB_PRESET_PADDEDCELL = 3");
		LuaScript::RunText("REVERB_PRESET_ROOM = 4");
		LuaScript::RunText("REVERB_PRESET_BATHROOM = 5");
		LuaScript::RunText("REVERB_PRESET_LIVINGROOM = 6");
		LuaScript::RunText("REVERB_PRESET_STONEROOM = 7");
		LuaScript::RunText("REVERB_PRESET_AUDITORIUM = 8");
		LuaScript::RunText("REVERB_PRESET_CONCERTHALL = 9");
		LuaScript::RunText("REVERB_PRESET_CAVE = 10");
		LuaScript::RunText("REVERB_PRESET_ARENA = 11");
		LuaScript::RunText("REVERB_PRESET_HANGAR = 12");
		LuaScript::RunText("REVERB_PRESET_CARPETEDHALLWAY = 13");
		LuaScript::RunText("REVERB_PRESET_HALLWAY = 14");
		LuaScript::RunText("REVERB_PRESET_STONECORRIDOR = 15");
		LuaScript::RunText("REVERB_PRESET_ALLEY = 16");
		LuaScript::RunText("REVERB_PRESET_CITY = 17");
		LuaScript::RunText("REVERB_PRESET_MOUNTAINS = 18");
		LuaScript::RunText("REVERB_PRESET_QUARRY = 19");
		LuaScript::RunText("REVERB_PRESET_PLAIN = 20");
		LuaScript::RunText("REVERB_PRESET_PARKINGLOT = 21");
		LuaScript::RunText("REVERB_PRESET_SEWERPIPE = 22");
		LuaScript::RunText("REVERB_PRESET_UNDERWATER = 23");
		LuaScript::RunText("REVERB_PRESET_SMALLROOM = 24");
		LuaScript::RunText("REVERB_PRESET_MEDIUMROOM = 25");
		LuaScript::RunText("REVERB_PRESET_LARGEROOM = 26");
		LuaScript::RunText("REVERB_PRESET_MEDIUMHALL = 27");
		LuaScript::RunText("REVERB_PRESET_LARGEHALL = 28");
		LuaScript::RunText("REVERB_PRESET_PLATE = 29");

		wiSound_BindLua::Bind();
		wiSoundInstance_BindLua::Bind();
		wiSoundInstance3D_BindLua::Bind();
	}
}




const char wiSound_BindLua::className[] = "Sound";

Luna<wiSound_BindLua>::FunctionType wiSound_BindLua::methods[] = {
	{ NULL, NULL }
};
Luna<wiSound_BindLua>::PropertyType wiSound_BindLua::properties[] = {
	{ NULL, NULL }
};

void wiSound_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<wiSound_BindLua>::Register(LuaScript::GetLuaState());
	}
}




const char wiSoundInstance_BindLua::className[] = "SoundInstance";

Luna<wiSoundInstance_BindLua>::FunctionType wiSoundInstance_BindLua::methods[] = {
	lunamethod(wiSoundInstance_BindLua, SetSubmixType),
	{ NULL, NULL }
};
Luna<wiSoundInstance_BindLua>::PropertyType wiSoundInstance_BindLua::properties[] = {
	{ NULL, NULL }
};

int wiSoundInstance_BindLua::SetSubmixType(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		wiAudio::SUBMIX_TYPE type = (wiAudio::SUBMIX_TYPE)LuaScript::SGetInt(L, 1);
		soundinstance.type = type;
	}
	else
		LuaScript::SError(L, "SetSubmixType(int submixtype) not enough arguments!");
	return 0;
}

void wiSoundInstance_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<wiSoundInstance_BindLua>::Register(LuaScript::GetLuaState());
	}
}




const char wiSoundInstance3D_BindLua::className[] = "SoundInstance3D";

Luna<wiSoundInstance3D_BindLua>::FunctionType wiSoundInstance3D_BindLua::methods[] = {
	lunamethod(wiSoundInstance3D_BindLua, SetListenerPos),
	lunamethod(wiSoundInstance3D_BindLua, SetListenerUp),
	lunamethod(wiSoundInstance3D_BindLua, SetListenerFront),
	lunamethod(wiSoundInstance3D_BindLua, SetListenerVelocity),
	lunamethod(wiSoundInstance3D_BindLua, SetEmitterPos),
	lunamethod(wiSoundInstance3D_BindLua, SetEmitterUp),
	lunamethod(wiSoundInstance3D_BindLua, SetEmitterFront),
	lunamethod(wiSoundInstance3D_BindLua, SetEmitterVelocity),
	lunamethod(wiSoundInstance3D_BindLua, SetEmitterRadius),
	{ NULL, NULL }
};
Luna<wiSoundInstance3D_BindLua>::PropertyType wiSoundInstance3D_BindLua::properties[] = {
	{ NULL, NULL }
};

int wiSoundInstance3D_BindLua::SetListenerPos(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.listenerPos, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetListenerPos(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetListenerUp(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.listenerUp, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetListenerUp(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetListenerFront(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.listenerFront, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetListenerFront(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetListenerVelocity(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.listenerVelocity, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetListenerVelocity(Vector value) not enough arguments!");
	return 0;
}

int wiSoundInstance3D_BindLua::SetEmitterPos(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.emitterPos, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetEmitterPos(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetEmitterUp(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.emitterUp, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetEmitterUp(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetEmitterFront(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMStoreFloat3(&soundinstance3D.emitterFront, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetEmitterFront(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetEmitterVelocity(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			DirectX::XMStoreFloat3(&soundinstance3D.emitterVelocity, vec->vector);
		}
	}
	else
		LuaScript::SError(L, "SetEmitterVelocity(Vector value) not enough arguments!");
	return 0;
}
int wiSoundInstance3D_BindLua::SetEmitterRadius(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		soundinstance3D.emitterRadius = LuaScript::SGetFloat(L, 1);
	}
	else
		LuaScript::SError(L, "SetEmitterVelocity(Vector value) not enough arguments!");
	return 0;
}

void wiSoundInstance3D_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<wiSoundInstance3D_BindLua>::Register(LuaScript::GetLuaState());
	}
}
