#include "GameObjectUIManager.h"

GameObjectUIManager::GameObjectUIManager()
{
}

void GameObjectUIManager::AddGameObjectUI(const GameObject& gameobject)
{

	objectUIpool.push_back(GameObjectUI(gameobject));
}

void GameObjectUIManager::DrawgameObjectUI()
{
	for (int i = 0; i < objectUIpool.size(); i++)
	{
		objectUIpool[i].DrawObjectUI();
	}

}
