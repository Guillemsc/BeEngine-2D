#include "ModuleScene.h"
#include "GameObject.h"
#include "ResourcePrefab.h"
#include "Resource.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleAssets.h"
#include "GameObjectAbstraction.h"
#include "ModuleFileSystem.h"
#include "ModuleGameObject.h"
#include "Scene.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ModuleScene::ModuleScene() : Module()
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;

	return ret;
}

bool ModuleScene::CreateNewPrefab(GameObject * go)
{
	bool ret = false;

	App->assets->CreatePrefab(go);

	return ret;
}

bool ModuleScene::UpdatePrefab(GameObject * go)
{
	bool ret = false;

	if (go != nullptr)
	{
		if (go->GetHasPrefab())
		{
			GameObjectAbstraction abs;

			std::vector<GameObject*> gos;
			gos.push_back(go);

			abs.Abstract(gos);

			go->GetPrefab()->UpdatePrefab(abs);

			ret = true;
		}
	}

	return ret;
}

bool ModuleScene::UpdateFromPrefab(GameObject * go)
{
	bool ret = false;

	if (go != nullptr)
	{

	}

	return ret;
}

