#pragma once

namespace xihad { namespace ngn
{
	class GameScene;
	class TagListener;
	class ComponentFactory;
	struct GameObjectDepends
	{
	public:
		GameObjectDepends() : scene(0), listener(0), factory(0) { }

		GameScene*			scene;
		TagListener*		listener;
		ComponentFactory*	factory;
	};
}}

