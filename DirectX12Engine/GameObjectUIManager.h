#pragma once
#include "GameObjectUI.h"
class GameObjectUIManager
{
public:
	GameObjectUIManager();
	std::vector<GameObjectUI> objectUIpool;
	void AddGameObjectUI(const GameObject& gameobject);
	void DrawgameObjectUI();
};

