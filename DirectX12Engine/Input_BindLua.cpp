#include "Input_BindLua.h"
#include "Vector_BindLua.h"

const char Input_BindLua::className[] = "Input";

Luna<Input_BindLua>::FunctionType Input_BindLua::methods[] = {
	lunamethod(Input_BindLua, Down),
	lunamethod(Input_BindLua, Press),
	lunamethod(Input_BindLua, Hold),
	lunamethod(Input_BindLua, GetPointer),
	lunamethod(Input_BindLua, SetPointer),
	lunamethod(Input_BindLua, GetPointerDelta),
	lunamethod(Input_BindLua, HidePointer),
	lunamethod(Input_BindLua, GetAnalog),
	lunamethod(Input_BindLua, GetTouches),
	lunamethod(Input_BindLua, SetControllerFeedback),
	{ NULL, NULL }
};
Luna<Input_BindLua>::PropertyType Input_BindLua::properties[] = {
	{ NULL, NULL }
};

int Input_BindLua::Down(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Input::BUTTON button = (Input::BUTTON)LuaScript::SGetInt(L, 1);
		int playerindex = 0;
		if (argc > 1)
		{
			playerindex = LuaScript::SGetInt(L, 2);
		}
		LuaScript::SSetBool(L, Input::Down(button, playerindex));
		return 1;
	}
	else
		LuaScript::SError(L, "Down(int code, opt int playerindex = 0) not enough arguments!");
	return 0;
}
int Input_BindLua::Press(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Input::BUTTON code = (Input::BUTTON)LuaScript::SGetInt(L, 1);
		int playerindex = 0;
		if (argc > 1)
		{
			playerindex = LuaScript::SGetInt(L, 2);
		}
		LuaScript::SSetBool(L, Input::Press(code, playerindex));
		return 1;
	}
	else
		LuaScript::SError(L, "Press(int code, opt int playerindex = 0) not enough arguments!");
	return 0;
}
int Input_BindLua::Hold(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Input::BUTTON button = (Input::BUTTON)LuaScript::SGetInt(L, 1);
		uint32_t duration = 30;
		if (argc > 1)
		{
			duration = LuaScript::SGetInt(L, 2);
		}
		bool continuous = false;
		if (argc > 2)
		{
			continuous = LuaScript::SGetBool(L, 3);
		}
		int playerindex = 0;
		if (argc > 3)
		{
			playerindex = LuaScript::SGetInt(L, 4);
		}
		LuaScript::SSetBool(L, Input::Hold(button, duration, continuous, playerindex));
		return 1;
	}
	else
		LuaScript::SError(L, "Hold(int code, opt int duration = 30, opt boolean continuous = false, opt int playerindex = 0) not enough arguments!");
	return 0;
}
int Input_BindLua::GetPointer(lua_State* L)
{
	XMFLOAT4 P = Input::GetPointer();
	Luna<Vector_BindLua>::push(L, new Vector_BindLua(XMLoadFloat4(&P)));
	return 1;
}
int Input_BindLua::SetPointer(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMFLOAT4 props;
			XMStoreFloat4(&props, vec->vector);
			Input::SetPointer(props);
		}
		else
			LuaScript::SError(L, "SetPointer(Vector props) argument is not a Vector!");
	}
	else
		LuaScript::SError(L, "SetPointer(Vector props) not enough arguments!");
	return 0;
}
int Input_BindLua::GetPointerDelta(lua_State* L)
{
	Luna<Vector_BindLua>::push(L, new Vector_BindLua(XMLoadFloat2(&Input::GetMouseState().delta_position)));
	return 1;
}
int Input_BindLua::HidePointer(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Input::HidePointer(LuaScript::SGetBool(L, 1));
	}
	else
		LuaScript::SError(L, "HidePointer(bool value) not enough arguments!");
	return 0;
}
int Input_BindLua::GetAnalog(lua_State* L)
{
	XMFLOAT4 result = XMFLOAT4(0, 0, 0, 0);

	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Input::GAMEPAD_ANALOG type = (Input::GAMEPAD_ANALOG)LuaScript::SGetInt(L, 1);
		int playerindex = 0;
		if (argc > 1)
		{
			playerindex = LuaScript::SGetInt(L, 2);
		}
		result = Input::GetAnalog(type, playerindex);
	}
	else
		LuaScript::SError(L, "GetAnalog(int type, opt int playerindex = 0) not enough arguments!");

	Luna<Vector_BindLua>::push(L, new Vector_BindLua(XMLoadFloat4(&result)));
	return 1;
}
int Input_BindLua::GetTouches(lua_State* L)
{
	auto& touches = Input::GetTouches();
	for (auto& touch : touches)
	{
		Luna<Touch_BindLua>::push(L, new Touch_BindLua(touch));
	}
	return (int)touches.size();
}
int Input_BindLua::SetControllerFeedback(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		ControllerFeedback_BindLua* fb = Luna<ControllerFeedback_BindLua>::lightcheck(L, 1); // Ferdinand Braun
		if (fb != nullptr)
		{
			int playerindex = 0;
			if (argc > 1)
			{
				playerindex = LuaScript::SGetInt(L, 2);
			}
			Input::SetControllerFeedback(fb->feedback, playerindex);
		}
		else
		{
			LuaScript::SError(L, "SetControllerFeedback(ControllerFeedback feedback, opt int playerindex = 0) first argument is not a ControllerFeedback!");
		}
	}
	else
		LuaScript::SError(L, "SetControllerFeedback(ControllerFeedback feedback, opt int playerindex = 0) not enough arguments!");
	return 0;
}

void Input_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<Input_BindLua>::Register(LuaScript::GetLuaState());
		LuaScript::RunText("input = Input()");

		LuaScript::RunText("MOUSE_BUTTON_LEFT			= 1");
		LuaScript::RunText("MOUSE_BUTTON_RIGHT			= 2");
		LuaScript::RunText("MOUSE_BUTTON_MIDDLE		= 3");

		LuaScript::RunText("KEYBOARD_BUTTON_UP			= 4");
		LuaScript::RunText("KEYBOARD_BUTTON_DOWN		= 5");
		LuaScript::RunText("KEYBOARD_BUTTON_LEFT		= 6");
		LuaScript::RunText("KEYBOARD_BUTTON_RIGHT		= 7");
		LuaScript::RunText("KEYBOARD_BUTTON_SPACE		= 8");
		LuaScript::RunText("KEYBOARD_BUTTON_RSHIFT		= 9");
		LuaScript::RunText("KEYBOARD_BUTTON_LSHIFT		= 10");
		LuaScript::RunText("KEYBOARD_BUTTON_F1			= 11");
		LuaScript::RunText("KEYBOARD_BUTTON_F2			= 12");
		LuaScript::RunText("KEYBOARD_BUTTON_F3			= 13");
		LuaScript::RunText("KEYBOARD_BUTTON_F4			= 14");
		LuaScript::RunText("KEYBOARD_BUTTON_F5			= 15");
		LuaScript::RunText("KEYBOARD_BUTTON_F6			= 16");
		LuaScript::RunText("KEYBOARD_BUTTON_F7			= 17");
		LuaScript::RunText("KEYBOARD_BUTTON_F8			= 18");
		LuaScript::RunText("KEYBOARD_BUTTON_F9			= 19");
		LuaScript::RunText("KEYBOARD_BUTTON_F10		= 20");
		LuaScript::RunText("KEYBOARD_BUTTON_F11		= 21");
		LuaScript::RunText("KEYBOARD_BUTTON_F12		= 22");
		LuaScript::RunText("KEYBOARD_BUTTON_ENTER		= 23");
		LuaScript::RunText("KEYBOARD_BUTTON_ESCAPE		= 24");
		LuaScript::RunText("KEYBOARD_BUTTON_HOME		= 25");
		LuaScript::RunText("KEYBOARD_BUTTON_RCONTROL	= 26");
		LuaScript::RunText("KEYBOARD_BUTTON_LCONTROL	= 27");
		LuaScript::RunText("KEYBOARD_BUTTON_DELETE		= 28");
		LuaScript::RunText("KEYBOARD_BUTTON_BACK		= 29");
		LuaScript::RunText("KEYBOARD_BUTTON_PAGEDOWN	= 30");
		LuaScript::RunText("KEYBOARD_BUTTON_PAGEUP		= 31");

		LuaScript::RunText("GAMEPAD_BUTTON_UP			= 257");
		LuaScript::RunText("GAMEPAD_BUTTON_LEFT		= 258");
		LuaScript::RunText("GAMEPAD_BUTTON_DOWN		= 259");
		LuaScript::RunText("GAMEPAD_BUTTON_RIGHT		= 260");
		LuaScript::RunText("GAMEPAD_BUTTON_1			= 261");
		LuaScript::RunText("GAMEPAD_BUTTON_2			= 262");
		LuaScript::RunText("GAMEPAD_BUTTON_3			= 263");
		LuaScript::RunText("GAMEPAD_BUTTON_4			= 264");
		LuaScript::RunText("GAMEPAD_BUTTON_5			= 265");
		LuaScript::RunText("GAMEPAD_BUTTON_6			= 266");
		LuaScript::RunText("GAMEPAD_BUTTON_7			= 267");
		LuaScript::RunText("GAMEPAD_BUTTON_8			= 268");
		LuaScript::RunText("GAMEPAD_BUTTON_9			= 269");
		LuaScript::RunText("GAMEPAD_BUTTON_10			= 270");
		LuaScript::RunText("GAMEPAD_BUTTON_11			= 271");
		LuaScript::RunText("GAMEPAD_BUTTON_12			= 272");
		LuaScript::RunText("GAMEPAD_BUTTON_13			= 273");
		LuaScript::RunText("GAMEPAD_BUTTON_14			= 274");

		//Analog
		LuaScript::RunText("GAMEPAD_ANALOG_THUMBSTICK_L	= 0");
		LuaScript::RunText("GAMEPAD_ANALOG_THUMBSTICK_R	= 1");
		LuaScript::RunText("GAMEPAD_ANALOG_TRIGGER_L		= 2");
		LuaScript::RunText("GAMEPAD_ANALOG_TRIGGER_R		= 3");

		//Touch
		LuaScript::RunText("TOUCHSTATE_PRESSED		= 0");
		LuaScript::RunText("TOUCHSTATE_RELEASED	= 1");
		LuaScript::RunText("TOUCHSTATE_MOVED		= 2");
	}

	Touch_BindLua::Bind();
	ControllerFeedback_BindLua::Bind();
}







const char Touch_BindLua::className[] = "Touch";

Luna<Touch_BindLua>::FunctionType Touch_BindLua::methods[] = {
	lunamethod(Touch_BindLua, GetState),
	lunamethod(Touch_BindLua, GetPos),
	{ NULL, NULL }
};
Luna<Touch_BindLua>::PropertyType Touch_BindLua::properties[] = {
	{ NULL, NULL }
};

int Touch_BindLua::GetState(lua_State* L)
{
	LuaScript::SSetInt(L, (int)touch.state);
	return 1;
}
int Touch_BindLua::GetPos(lua_State* L)
{
	Luna<Vector_BindLua>::push(L, new Vector_BindLua(XMLoadFloat2(&touch.pos)));
	return 1;
}

void Touch_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<Touch_BindLua>::Register(LuaScript::GetLuaState());
	}
}







const char ControllerFeedback_BindLua::className[] = "ControllerFeedback";

Luna<ControllerFeedback_BindLua>::FunctionType ControllerFeedback_BindLua::methods[] = {
	lunamethod(ControllerFeedback_BindLua, SetVibrationRight),
	lunamethod(ControllerFeedback_BindLua, SetVibrationLeft),
	lunamethod(ControllerFeedback_BindLua, SetLEDColor),
	{ NULL, NULL }
};
Luna<ControllerFeedback_BindLua>::PropertyType ControllerFeedback_BindLua::properties[] = {
	{ NULL, NULL }
};

int ControllerFeedback_BindLua::SetVibrationLeft(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		feedback.vibration_left = LuaScript::SGetFloat(L, 1);
	}
	else
	{
		LuaScript::SError(L, "SetVibrationLeft(float value) not enough arguments!");
	}
	return 0;
}
int ControllerFeedback_BindLua::SetVibrationRight(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		feedback.vibration_right = LuaScript::SGetFloat(L, 1);
	}
	else
	{
		LuaScript::SError(L, "SetVibrationRight(float value) not enough arguments!");
	}
	return 0;
}
int ControllerFeedback_BindLua::SetLEDColor(lua_State* L)
{
	int argc = LuaScript::SGetArgCount(L);
	if (argc > 0)
	{
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec != nullptr)
		{
			XMFLOAT4 col;
			XMStoreFloat4(&col, vec->vector);
			feedback.led_color = wiColor::fromFloat4(col);
		}
		else
		{
			feedback.led_color.rgba = LuaScript::SGetInt(L, 1);
		}
	}
	else
	{
		LuaScript::SError(L, "SetLEDColor(int hexcolor) not enough arguments!");
	}
	return 0;
}

void ControllerFeedback_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<ControllerFeedback_BindLua>::Register(LuaScript::GetLuaState());
	}
}
