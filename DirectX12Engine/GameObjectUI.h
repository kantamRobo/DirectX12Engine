#pragma once
#include "GameObject.h"
#include <vector>
#include "ImguiCore.h"
using namespace ImGui;
class GameObjectUI
{
public:
	GameObjectUI();
	GameObjectUI(const GameObject& in_object) {object = in_object; float pos[3] = { object.transform.position.x,
		object.transform.position.y,
		object.transform.position.z };
	ImGui::InputFloat3("position", pos);
	float rot[4] = { object.transform.rotation.x,
		object.transform.rotation.y,
		object.transform.rotation.z,
	object.transform.rotation.w };
	ImGui::InputFloat4("rotation", rot);
	};
	GameObject object;
	void DrawObjectUI();


	

};

