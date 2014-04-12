#pragma once
#include "GameObject.h"
#include "TagManager.h"
#include <string>

namespace xihad { namespace ngn
{
	class GameScene;
	class ComponentFactory;
	struct GameObjectDepends
	{
	public:
		typedef TagManager<GameObject, std::string, 64> TagManagerT;

		GameObjectDepends() : scene(0), tagManager(0), factory(0) { }

		GameScene*			scene;
		TagManagerT*		tagManager;
		ComponentFactory*	factory;
	};
}}

